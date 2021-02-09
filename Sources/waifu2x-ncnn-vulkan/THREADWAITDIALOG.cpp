// THREADWAITDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "THREADWAITDIALOG.h"
#include "afxdialogex.h"
#include "waifu2x-ncnn-vulkanDlg.h"
#include "CThreadWaitDlgThread.h"

#define WM_USER_COMPLETE_MAIN (WM_USER + 1)

// THREADWAITDIALOG ダイアログ

IMPLEMENT_DYNAMIC(THREADWAITDIALOG, CDialogEx)

THREADWAITDIALOG::THREADWAITDIALOG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREADWAITDIALOG, pParent)
{
	CUR_POS = 0;
	MAX_POS = 0;
	POS = _T("");
	MAX = _T("");
	PROGRESSTEXT = _T("");
	hDCBackBuffer = NULL;
	hDCStatic = NULL;
	m_Hbrush = NULL;
}

THREADWAITDIALOG::~THREADWAITDIALOG()
{
}

void THREADWAITDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, xv_Progress);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_TEXT, m_Static);
}


BEGIN_MESSAGE_MAP(THREADWAITDIALOG, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &THREADWAITDIALOG::OnBnClickedButtonCancel)
	ON_MESSAGE(WM_USER_COMPLETE_MAIN, THREADWAITDIALOG::OnCompleteMainThread)
END_MESSAGE_MAP()


// THREADWAITDIALOG メッセージ ハンドラー

BOOL THREADWAITDIALOG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_TimerID = SetTimer(100,100,NULL);

	xv_Progress.SetRange32(0, static_cast<UINT>(Cwaifu2xncnnvulkanDlg::FILECOUNT));
	MAX_POS = Cwaifu2xncnnvulkanDlg::FILECOUNT;
	OutputDebugString(_T("SetRange.\n"));

	hDCBackBuffer = CreateCompatibleDC(NULL);
	hDCStatic = ::GetDC(this->m_Static);
	this->m_Static.GetClientRect(&rc);

	CWinThread* pMainThread = NULL;
	pMainThread = AfxBeginThread(MainThread, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	if (pMainThread)
	{
		pMainThread->m_pMainWnd = this;
		pMainThread->m_bAutoDelete = TRUE;
		pMainThread->ResumeThread();
	}

	return TRUE;
}
UINT THREADWAITDIALOG::MainThread(LPVOID pParam)
{
	THREADWAITDIALOG* pFileView = dynamic_cast<THREADWAITDIALOG*>(reinterpret_cast<CWnd*>(pParam));
	if (pFileView)
	{
		pFileView->MainThread();
	}
	return 0;
}

void THREADWAITDIALOG::MainThread()
{
	HDC hdc;
	PAINTSTRUCT ps;
	INT cur, max;

	xv_Progress.GetRange(cur, max);
	while (max >= static_cast<INT>(Cwaifu2xncnnvulkanDlg::UPSCALE_COUNT)) {
		if (Cwaifu2xncnnvulkanDlg::SuspendFlag == 1) {
			break;
		}
		if (Cwaifu2xncnnvulkanDlg::ProgressThreadFlag == 1) {
			break;
		}

		xv_Progress.SetPos(Cwaifu2xncnnvulkanDlg::UPSCALE_COUNT);

		CUR_POS = Cwaifu2xncnnvulkanDlg::UPSCALE_COUNT;
		if (Cwaifu2xncnnvulkanDlg::Waifu2xReUpscalingFlag == FALSE) {
			POS.Format(_T("Upscaling file(s): %u file(s) /"), CUR_POS);
			MAX.Format(_T(" %u file(s)"), MAX_POS);
			PROGRESSTEXT = POS + MAX;
		}
		else if (Cwaifu2xncnnvulkanDlg::Waifu2xReUpscalingFlag == TRUE) {
			POS.Format(_T("Re Upscaling file(s): %u file(s) /"), CUR_POS);
			MAX.Format(_T(" %u file(s)"), MAX_POS);
			PROGRESSTEXT = POS + MAX;
		}
		else {
			POS.Format(_T("Upscaling file(s): %u file(s) /"), CUR_POS);
			MAX.Format(_T(" %u file(s)"), MAX_POS);
			PROGRESSTEXT = POS + MAX;
		}
		SetWindowText(PROGRESSTEXT);
		OutputDebugString(PROGRESSTEXT + _T("\n"));
		::SetBkColor(hDCStatic, RGB(240, 240, 240));
		::SelectObject(hDCBackBuffer, hDCStatic);
		m_Hbrush = (HBRUSH)::SelectObject(hDCBackBuffer, CreateSolidBrush(RGB(240, 240, 240)));
		::PatBlt(hDCBackBuffer, 0, 0, rc.right, rc.bottom, PATCOPY);
		::SelectObject(hDCBackBuffer, m_Hbrush);
		::DrawText(hDCStatic, PROGRESSTEXT, -1, &rc, DT_RIGHT | DT_NOCLIP | DT_SINGLELINE);
		this->m_Static.InvalidateRect(rc, 0);
		hdc = ::BeginPaint(this->m_Static, &ps);
		::BitBlt(hdc, 0, 0, rc.right, rc.bottom, hDCBackBuffer, 0, 0, SRCCOPY);
		::EndPaint(this->m_Static, &ps);
		Sleep(255);
		this->m_Static.UpdateWindow();
	}
	this->PostMessage(WM_USER_COMPLETE_MAIN);
}

LRESULT THREADWAITDIALOG::OnCompleteMainThread(WPARAM wParam, LPARAM lParam)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT THREADWAITDIALOG::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CLOSE) {
		if (hDCStatic) { ::ReleaseDC(this->m_Static, hDCStatic); hDCStatic = NULL; }
		if (hDCBackBuffer) { ::DeleteDC(hDCBackBuffer); hDCBackBuffer = NULL; }
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

BOOL THREADWAITDIALOG::PreTranslateMessage(MSG* pMsg)
{
	// キーダウンメッセージの場合
	if (WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			// リターンキーが押下された場合、FALSEを返却し画面と閉じないようにします。
			return FALSE;

		case VK_ESCAPE:
			// ESCキーが押下された場合、FALSEを返却し画面と閉じないようにします。
			return FALSE;

		default:
			// 上記以外、何もしません。
			break;
		}
	}

	// 親クラスの関数を呼び出します。
	return CDialog::PreTranslateMessage(pMsg);
}

void THREADWAITDIALOG::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if (Cwaifu2xncnnvulkanDlg::Waifu2xThreadFlag == TRUE) {
		KillTimer(m_TimerID);
		EndDialog(0);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void THREADWAITDIALOG::OnBnClickedButtonCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	INT_PTR ret = MessageBox(_T("処理が進行中です。中止すると現在処理中の内容はすべて失われます。\n続行しますか？"), _T("確認"), MB_ICONWARNING | MB_YESNO);
	if (ret == IDYES) {
		Cwaifu2xncnnvulkanDlg::SuspendFlag = 1;
		return;
	}
	else {
		return;
	}
}