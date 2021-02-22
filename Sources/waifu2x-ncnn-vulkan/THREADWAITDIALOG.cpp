// THREADWAITDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "THREADWAITDIALOG.h"
#include "afxdialogex.h"
#include "waifu2x-ncnn-vulkanDlg.h"
#include "CThreadWaitDlgThread.h"

#define WM_USER_COMPLETE_MAIN (WM_USER + 0x20)

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
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &THREADWAITDIALOG::OnBnClickedButtonCancel)
	ON_MESSAGE(WM_USER_COMPLETE_MAIN, THREADWAITDIALOG::OnCompleteMainThread)
END_MESSAGE_MAP()


// THREADWAITDIALOG メッセージ ハンドラー

BOOL THREADWAITDIALOG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UINT Lang;
	Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");
	if (Lang == 0) {
		Core->LoadJPNLangLibrary();
	}
	else if (Lang == 1) {
		Core->LoadENGLangLibrary();
	}
	else {
		Core->LoadJPNLangLibrary();
	}

	LoadString(Core->Lang_hinst, IDS_STATIC_THREADDLG, (LPTSTR)STATIC_THREADDLG, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_ABORT_CONFIRM, (LPTSTR)WARN_ABORT_CONFIRM, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_CONFIRM, (LPTSTR)WARN_CONFIRM, 256);
	GetDlgItem(IDC_STATIC_UPTXT)->SetWindowText(STATIC_THREADDLG);
	LoadString(Core->Lang_hinst, IDS_BTN_ABORT, (LPTSTR)BTN_ABORT, 256);
	GetDlgItem(IDC_BUTTON_CANCEL)->SetWindowText(BTN_ABORT);

	UINT TID = 1, TID2 = 2;
	m_TimerID = SetTimer(TID,255,NULL);
	m_hTimerID = SetTimer(TID2, 1000, NULL);

	xv_Progress.SetMarquee(TRUE, 100);
	xv_Progress.SetRange32(0, static_cast<UINT>(Cwaifu2xncnnvulkanDlg::FILECOUNT));
	MAX_POS = Cwaifu2xncnnvulkanDlg::FILECOUNT;
	OutputDebugString(_T("SetRange.\n"));

	hDCBackBuffer = CreateCompatibleDC(NULL);
	hDCStatic = ::GetDC(this->m_Static);
	this->m_Static.GetClientRect(&rc);

	CWinThread* pMainThread = NULL;
	pMainThread = AfxBeginThread(MainThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
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
	INT cur, max;

	xv_Progress.GetRange(cur, max);
	while (max >= static_cast<INT>(Cwaifu2xncnnvulkanDlg::UPSCALE_COUNT)) {
		if (Cwaifu2xncnnvulkanDlg::SuspendFlag == 1) {
			break;
		}
		if (max <= static_cast<INT>(Cwaifu2xncnnvulkanDlg::UPSCALE_COUNT)) {
			Sleep(1000);
			break;
		}
		if (ExceptionCounter > 50) {
			Cwaifu2xncnnvulkanDlg::UpscaleExceptionFlag = 1;
			MessageBox(_T("An unexpected error has occurred.\nA static variable stopped with an unexpected value.\nCheck the waifu2x conversion settings."), _T("Error"), MB_ICONERROR | MB_OK);
			break;
		}
	}
	PostMessage(WM_USER_COMPLETE_MAIN);
}

LRESULT THREADWAITDIALOG::OnCompleteMainThread(WPARAM wParam, LPARAM lParam)
{
	PostMessage(WM_CLOSE);
	return 0;
}

void THREADWAITDIALOG::UpdateProgressText()
{
	HDC hdc;
	PAINTSTRUCT ps;
	UINT Lang;
	Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");

	CUR_POS = Cwaifu2xncnnvulkanDlg::UPSCALE_COUNT;
	if (Lang == 0) {
		if (Cwaifu2xncnnvulkanDlg::Waifu2xReUpscalingFlag == FALSE) {
			POS.Format(_T("アップスケーリング中： %u ファイル /"), CUR_POS);
			MAX.Format(_T(" %u ファイル"), MAX_POS);
			PROGRESSTEXT = POS + MAX;
		}
		else if (Cwaifu2xncnnvulkanDlg::Waifu2xReUpscalingFlag == TRUE) {
			POS.Format(_T("再アップスケーリング中： %u ファイル /"), CUR_POS);
			MAX.Format(_T(" %u ファイル"), MAX_POS);
			PROGRESSTEXT = POS + MAX;
		}
		else {
			POS.Format(_T("アップスケーリング中： %u ファイル /"), CUR_POS);
			MAX.Format(_T(" %u ファイル"), MAX_POS);
			PROGRESSTEXT = POS + MAX;
		}
	}
	else if (Lang == 1) {
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
	}
	else {
		if (Cwaifu2xncnnvulkanDlg::Waifu2xReUpscalingFlag == FALSE) {
			POS.Format(_T("アップスケーリング中： %u ファイル /"), CUR_POS);
			MAX.Format(_T(" %u ファイル"), MAX_POS);
			PROGRESSTEXT = POS + MAX;
		}
		else if (Cwaifu2xncnnvulkanDlg::Waifu2xReUpscalingFlag == TRUE) {
			POS.Format(_T("再アップスケーリング中： %u ファイル /"), CUR_POS);
			MAX.Format(_T(" %u ファイル"), MAX_POS);
			PROGRESSTEXT = POS + MAX;
		}
		else {
			POS.Format(_T("アップスケーリング中： %u ファイル /"), CUR_POS);
			MAX.Format(_T(" %u ファイル"), MAX_POS);
			PROGRESSTEXT = POS + MAX;
		}
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
	this->m_Static.UpdateWindow();
}

void THREADWAITDIALOG::OnBnClickedButtonCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	INT_PTR ret = MessageBox(WARN_ABORT_CONFIRM, WARN_CONFIRM, MB_ICONWARNING | MB_YESNO);
	if (ret == IDYES) {
		Cwaifu2xncnnvulkanDlg::SuspendFlag = 1;
		return;
	}
	else {
		return;
	}
}

void THREADWAITDIALOG::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	FreeLibrary(Core->Lang_hinst);
	SAFE_DELETE(CORE_FUNC);
	KillTimer(m_TimerID);
	KillTimer(m_hTimerID);
	m_TimerID = 0;
	m_hTimerID = 0;
	if (hDCStatic) { ::ReleaseDC(this->m_Static, hDCStatic); hDCStatic = NULL; }
	if (hDCBackBuffer) { ::DeleteDC(hDCBackBuffer); hDCBackBuffer = NULL; }
	Cwaifu2xncnnvulkanDlg::ProgressThreadFlag = 1;
}

BOOL THREADWAITDIALOG::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return FALSE;

		case VK_ESCAPE:
			return FALSE;

		default:
			break;
		}
	}

	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) return(TRUE);

	return CDialog::PreTranslateMessage(pMsg);
}

void THREADWAITDIALOG::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	if (nIDEvent == m_TimerID) {
		xv_Progress.SetPos(Cwaifu2xncnnvulkanDlg::UPSCALE_COUNT);
		UpdateProgressText();
		if (ValueFlag != Cwaifu2xncnnvulkanDlg::UPSCALE_COUNT) {
			ExceptionCounter = 0;
		}
		else {
			ExceptionCounter++;
		}
	}
	if (nIDEvent == m_hTimerID) {
		ValueFlag = CUR_POS;
	}

	CDialogEx::OnTimer(nIDEvent);
}

