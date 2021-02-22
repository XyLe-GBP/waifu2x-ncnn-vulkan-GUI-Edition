// DLDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "waifu2x-ncnn-vulkanDlg.h"
#include "DLDIALOG.h"
#include "afxdialogex.h"

#define WM_USER_COMPLETE_MAIN (WM_USER + 0x30)

// DLDIALOG ダイアログ

IMPLEMENT_DYNAMIC(DLDIALOG, CDialogEx)

DLDIALOG::DLDIALOG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLDIALOG, pParent)
{
	hDCBackBuffer = NULL;
	hDCStatic = NULL;
	m_Hbrush = NULL;
	MAX = _T("");
	POS = _T("");
	PROGRESSTEXT = _T("");
	MAX_POS = 0;
	CUR_POS = 0;
}

DLDIALOG::~DLDIALOG()
{
}

void DLDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, xv_Progress1);
	DDX_Control(pDX, IDC_STATIC1, m_Static);
}


BEGIN_MESSAGE_MAP(DLDIALOG, CDialogEx)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER_COMPLETE_MAIN, DLDIALOG::OnCompleteMainThread)
END_MESSAGE_MAP()


// DLDIALOG メッセージ ハンドラー

BOOL DLDIALOG::OnInitDialog()
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

	LoadString(Core->Lang_hinst, IDS_STATIC_DLDLG, (LPTSTR)STATIC_DLDLG, 256);
	GetDlgItem(IDC_STATIC_DLTXT)->SetWindowText(STATIC_DLDLG);

	UINT TID = 1, TID2 = 2;
	m_TimerID = SetTimer(TID, 255, NULL);
	m_hTimerID = SetTimer(TID2, 1000, NULL);

	xv_Progress1.SetMarquee(TRUE, 100);
	xv_Progress1.SetRange32(0, Cwaifu2xncnnvulkanDlg::DLCurCount);
	MAX_POS = Cwaifu2xncnnvulkanDlg::DLCurCount;
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


UINT DLDIALOG::MainThread(LPVOID pParam)
{
	DLDIALOG* pFileView = dynamic_cast<DLDIALOG*>(reinterpret_cast<CWnd*>(pParam));
	if (pFileView)
	{
		pFileView->MainThread();
	}
	return 0;
}


void DLDIALOG::MainThread()
{
	INT cur, max;
	CFileFind find;

	xv_Progress1.GetRange(cur, max);
	while (max >= static_cast<INT>(Cwaifu2xncnnvulkanDlg::DLCount)) {
		if (max <= static_cast<INT>(Cwaifu2xncnnvulkanDlg::DLCount)) {
			Sleep(1000);
			break;
		}
		if (ExceptionCounter > 40) {
			Cwaifu2xncnnvulkanDlg::DLErrorFlag = 1;
			MessageBox(_T("An error occurred while downloading.\nPlease make sure that you have an Internet connection."), _T("Error"), MB_ICONWARNING | MB_OK);
			break;
		}
	}
	PostMessage(WM_USER_COMPLETE_MAIN);
}


LRESULT DLDIALOG::OnCompleteMainThread(WPARAM wParam, LPARAM lParam)
{
	PostMessage(WM_CLOSE);
	return 0;
}


void DLDIALOG::UpdateProgressText()
{
	HDC hdc;
	PAINTSTRUCT ps;
	UINT Lang;
	Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");

	CUR_POS = Cwaifu2xncnnvulkanDlg::DLCount;
	if (Lang == 0) {
		POS.Format(_T("ダウンロード中： %u バイト /"), CUR_POS);
		MAX.Format(_T(" %u バイト"), MAX_POS);
	}
	else if (Lang == 1) {
		POS.Format(_T("Downloaded: %u Byte /"), CUR_POS);
		MAX.Format(_T(" %u Byte"), MAX_POS);
	}
	else {
		POS.Format(_T("ダウンロード中： %u バイト /"), CUR_POS);
		MAX.Format(_T(" %u バイト"), MAX_POS);
	}
	PROGRESSTEXT = POS + MAX;

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


BOOL DLDIALOG::PreTranslateMessage(MSG* pMsg)
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


void DLDIALOG::OnDestroy()
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
	Cwaifu2xncnnvulkanDlg::DLThreadFlag = 1;
}


void DLDIALOG::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if (nIDEvent == m_TimerID) {
		xv_Progress1.SetPos(Cwaifu2xncnnvulkanDlg::DLCount);
		UpdateProgressText();
		if (ValueFlag != Cwaifu2xncnnvulkanDlg::DLCount) {
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