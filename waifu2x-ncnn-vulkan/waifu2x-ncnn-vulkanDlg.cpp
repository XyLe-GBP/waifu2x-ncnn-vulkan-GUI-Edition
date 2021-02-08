
// waifu2x-ncnn-vulkanDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "waifu2x-ncnn-vulkan.h"
#include "waifu2x-ncnn-vulkanDlg.h"
#include "afxdialogex.h"
#include "direct.h"
#include <iostream>
#include <fstream>
#include <sstream>

// user includes
#include "WAITDIALOG.h"
#include "THREADWAITDIALOG.h"
#include "DLDIALOG.h"
#include "SETTINGSDIALOG.h"
#include "DELETEDIALOG.h"
#include "FFMPEGSETTINGSDIALOG.h"
#include "FFMPEGAUDIOSETTINGS.h"
#include "FFMPEGVIDEOSETTINGS.h"
#include "VIDEORESIZE.h"
#include "POPUPDlg.h"
#include "DOWNLOADDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CThreadWaitDlgThread.h"

#define WM_USER_COMPLETE_WAIFU2X_THREAD (WM_APP + 0x10)
#define WM_USER_COMPLETE_FFMPEG_THREAD (WM_APP + 0x20)
#define WM_USER_COMPLETE_DOWNLOAD_LOAD_XML (WM_APP + 0x30)
#define WM_USER_COMPLETE_DOWNLOAD_COUNT_LOAD_XML (WM_APP + 0x40)
#define WM_USER_COMPLETE_COUNT_THREAD (WM_APP + 0x50)
#define WM_USER_COMPLETE_COUNT_DLG_THREAD (WM_APP + 0x60)
#define WM_USER_COMPLETE_DELETE_LOAD_XML (WM_APP + 0x70)

UINT Cwaifu2xncnnvulkanDlg::DLThreadFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::DLFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::DLCount = 0;
UINT Cwaifu2xncnnvulkanDlg::DLCurCount = 0;
UINT Cwaifu2xncnnvulkanDlg::FILECOUNT = 0;
UINT Cwaifu2xncnnvulkanDlg::UPSCALE_COUNT = 0;
UINT Cwaifu2xncnnvulkanDlg::DELETECOUNT = 0;
UINT Cwaifu2xncnnvulkanDlg::ProgressThreadFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::Waifu2xThreadFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::Waifu2xReUpscalingFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::DeleteFileThreadFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::SuspendFlag = 0;

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL OnInitDialog();

// 実装
protected:
	NEW_CORE;
	NEW_MAINSTR;
	NEW_VERSIONSTR;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticHyper1();
	afx_msg void OnStnClickedStaticHyper2();
	afx_msg void OnStnClickedStaticHyper3();
	afx_msg void OnStnClickedStaticHyper4();
	afx_msg void OnStnClickedStaticIcon();
	afx_msg void OnDestroy();
	HICON ICO;
	HINSTANCE hInst = AfxGetInstanceHandle();
	CStatic xv_Static_AppVersionText, xv_Static_CmdVersionText, m_hyper1, m_hyper2, m_hyper3, m_hyper4, ICON;
	COLORREF m_hyperlink;
	CFont m_hlfont;
	CStatic xv_Static_FFmpeg_VersionText;
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
	ICO = NULL;
	m_hyperlink = NULL;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_APPVER, xv_Static_AppVersionText);
	DDX_Control(pDX, IDC_STATIC_CMDVER, xv_Static_CmdVersionText);
	DDX_Control(pDX, IDC_STATIC_HYPER1, m_hyper1);
	DDX_Control(pDX, IDC_STATIC_HYPER2, m_hyper2);
	DDX_Control(pDX, IDC_STATIC_HYPER3, m_hyper3);
	DDX_Control(pDX, IDC_STATIC_HYPER4, m_hyper4);
	DDX_Control(pDX, IDC_STATIC_ICON, ICON);
	DDX_Control(pDX, IDC_STATIC_FFVER, xv_Static_FFmpeg_VersionText);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_STN_CLICKED(IDC_STATIC_HYPER1, &CAboutDlg::OnStnClickedStaticHyper1)
	ON_STN_CLICKED(IDC_STATIC_HYPER2, &CAboutDlg::OnStnClickedStaticHyper2)
	ON_STN_CLICKED(IDC_STATIC_HYPER3, &CAboutDlg::OnStnClickedStaticHyper3)
	ON_STN_CLICKED(IDC_STATIC_HYPER4, &CAboutDlg::OnStnClickedStaticHyper4)
	ON_STN_CLICKED(IDC_STATIC_ICON, &CAboutDlg::OnStnClickedStaticIcon)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Core->LoadImageLibrary();
	if (hInst != NULL) {
		ICO = ExtractIcon(hInst, RES_LIBRARY, 0);//LoadIcon(Core->hinst, MAKEINTRESOURCE(1003));
	}
	else {
		MessageBox(_T("リソースの読み込みに失敗しました"), _T("エラー"), MB_ICONERROR | MB_OK);
	}
	CStatic* box = (CStatic*)GetDlgItem(IDC_STATIC_ICON);
	box->SetIcon(ICO);
	box->InvalidateRect(NULL, 1);

	this->xv_Static_AppVersionText.SetWindowText(APP_VERSION);
	this->xv_Static_CmdVersionText.SetWindowText(CMD_VERSION);
	this->xv_Static_FFmpeg_VersionText.SetWindowText(FF_VERSION);

	m_hyperlink = RGB(0, 0, 255);
	CFont* pFont = m_hyper1.GetFont();
	LOGFONT lFont;
	pFont->GetLogFont(&lFont);
	lFont.lfUnderline = (TRUE);
	m_hlfont.CreateFontIndirect(&lFont);
	m_hyper1.SetFont(&m_hlfont, TRUE);
	m_hyper2.SetFont(&m_hlfont, TRUE);
	m_hyper3.SetFont(&m_hlfont, TRUE);
	m_hyper4.SetFont(&m_hlfont, TRUE);

	return TRUE;
}

BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (pWnd == &this->m_hyper1)
	{
		HCURSOR hCursor = LoadCursor(Core->hinst, MAKEINTRESOURCE(104));
		::SetCursor(hCursor);
		return TRUE;
	}
	if (pWnd == &this->m_hyper2)
	{
		HCURSOR hCursor = LoadCursor(Core->hinst, MAKEINTRESOURCE(104));
		::SetCursor(hCursor);
		return TRUE;
	}
	if (pWnd == &this->m_hyper3)
	{
		HCURSOR hCursor = LoadCursor(Core->hinst, MAKEINTRESOURCE(104));
		::SetCursor(hCursor);
		return TRUE;
	}
	if (pWnd == &this->m_hyper4)
	{
		HCURSOR hCursor = LoadCursor(Core->hinst, MAKEINTRESOURCE(104));
		::SetCursor(hCursor);
		return TRUE;
	}
	if (pWnd == &this->ICON)
	{
		HCURSOR hCursor = LoadCursor(Core->hinst, MAKEINTRESOURCE(104));
		::SetCursor(hCursor);
		return TRUE;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd == &this->m_hyper1) {
		pDC->SetTextColor(m_hyperlink);
	}
	if (pWnd == &this->m_hyper2) {
		pDC->SetTextColor(m_hyperlink);
	}
	if (pWnd == &this->m_hyper3) {
		pDC->SetTextColor(m_hyperlink);
	}
	if (pWnd == &this->m_hyper4) {
		pDC->SetTextColor(m_hyperlink);
	}
	return hbr;
}

void CAboutDlg::OnStnClickedStaticHyper1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ShellExecute(NULL, _T("open"), _T("https://github.com/nihui/waifu2x-ncnn-vulkan/releases"), NULL, NULL, SW_SHOWNORMAL);
}


void CAboutDlg::OnStnClickedStaticHyper2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ShellExecute(NULL, _T("open"), _T("https://github.com/nagadomi/waifu2x"), NULL, NULL, SW_SHOWNORMAL);
}


void CAboutDlg::OnStnClickedStaticHyper3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ShellExecute(NULL, _T("open"), _T("https://github.com/lltcggie/waifu2x-caffe"), NULL, NULL, SW_SHOWNORMAL);
}


void CAboutDlg::OnStnClickedStaticHyper4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	ShellExecute(NULL, _T("open"), _T("https://xyle-official.com"), NULL, NULL, SW_SHOWNORMAL);
}


void CAboutDlg::OnStnClickedStaticIcon()
{
	ShellExecute(NULL, _T("open"), _T("https://twitter.com/X1LeP"), NULL, NULL, SW_SHOWNORMAL);
}


void CAboutDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	FreeLibrary(Core->hinst);
	SAFE_DELETE(CORE_FUNC);
	SAFE_DELETE(MAINSTR_FUNC);
	SAFE_DELETE(VERSIONSTR_FUNC);
	Core->FreeImageLibrary();
}

// Cwaifu2xncnnvulkanDlg ダイアログ



Cwaifu2xncnnvulkanDlg::Cwaifu2xncnnvulkanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WAIFU2XNCNNVULKAN_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINICON);
	FFmpegThreadFlag = 0;
	waifu2x_param = _T("");
	ffmpeg_mainparam = _T("");
	ffmpeg_videoparam = _T("");
	ffmpeg_audioparam = _T("");
	IMAGEPATH = _T("");
	IMAGEPATH_M = _T("");
	VIDEOPATH = _T("");
}

void Cwaifu2xncnnvulkanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, xv_CPUINFO);
	DDX_Control(pDX, IDC_EDIT4, xv_GPUINFO);
	DDX_Control(pDX, IDC_STATIC_READSTATUS, xv_Static_ReadStatus);
	DDX_Control(pDX, IDC_STATIC_FILE, xv_Static_File);
	DDX_Control(pDX, IDC_STATIC_FILEPATH, xv_Static_FilePath);
}

BEGIN_MESSAGE_MAP(Cwaifu2xncnnvulkanDlg, CDialogEx)
	// system
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_NCDESTROY()
	// user
	ON_BN_CLICKED(IDC_BUTTON1, &Cwaifu2xncnnvulkanDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cwaifu2xncnnvulkanDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Cwaifu2xncnnvulkanDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &Cwaifu2xncnnvulkanDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &Cwaifu2xncnnvulkanDlg::OnBnClickedButton6)
	ON_COMMAND(ID_READ_IMAGE, &Cwaifu2xncnnvulkanDlg::OnReadImage)
	ON_COMMAND(ID_READ_IMAGE_MULTI, &Cwaifu2xncnnvulkanDlg::OnReadImageMulti)
	ON_COMMAND(ID_READ_VIDEO, &Cwaifu2xncnnvulkanDlg::OnReadVideo)
	ON_COMMAND(ID_FILE_ALLDELETE, &Cwaifu2xncnnvulkanDlg::OnFileAlldelete)
	ON_COMMAND(ID_END_APP, &Cwaifu2xncnnvulkanDlg::OnEndApp)
	ON_COMMAND(ID_SETTINGS, &Cwaifu2xncnnvulkanDlg::OnSettings)
	ON_COMMAND(ID_FFMPEG_SETTING, &Cwaifu2xncnnvulkanDlg::OnFfmpegSettings)
	ON_COMMAND(ID_FFMPEG_AUDIOSETTINGS, &Cwaifu2xncnnvulkanDlg::OnFfmpegAudiosettings)
	ON_COMMAND(ID_FFMPEG_VIDEOSETTINGS, &Cwaifu2xncnnvulkanDlg::OnFfmpegVideosettings)
	ON_COMMAND(ID_ABOUT_WAIFU2X, &Cwaifu2xncnnvulkanDlg::OnAboutWaifu2x)
	ON_MESSAGE(WM_USER_COMPLETE_WAIFU2X_THREAD, Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xThread)// UserMessage
	ON_MESSAGE(WM_USER_COMPLETE_FFMPEG_THREAD, Cwaifu2xncnnvulkanDlg::OnCompleteFFmpegThread)// UserMessage
	ON_MESSAGE(WM_USER_COMPLETE_DOWNLOAD_LOAD_XML, Cwaifu2xncnnvulkanDlg::OnCompleteDLThread)// UserMessage
	ON_MESSAGE(WM_USER_COMPLETE_DOWNLOAD_COUNT_LOAD_XML, Cwaifu2xncnnvulkanDlg::OnCompleteDLCountThread)// UserMessage
	ON_MESSAGE(WM_USER_COMPLETE_DELETE_LOAD_XML, Cwaifu2xncnnvulkanDlg::OnCompleteDeleteThread)// UserMessage
	ON_MESSAGE(WM_USER_COMPLETE_COUNT_THREAD, Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xCountThread)// UserMessage
	ON_MESSAGE(WM_USER_COMPLETE_COUNT_DLG_THREAD, Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xCountDlgThread)// UserMessage
	ON_COMMAND(ID_VIDEORESIZE, &Cwaifu2xncnnvulkanDlg::OnVideoresize)
	ON_COMMAND(ID_VIDEOAUDIOEXPORT, &Cwaifu2xncnnvulkanDlg::OnVideoaudioexport)
	ON_COMMAND(ID_UPDATECHECK, &Cwaifu2xncnnvulkanDlg::OnUpdatecheck)
END_MESSAGE_MAP()


// Cwaifu2xncnnvulkanDlg メッセージ ハンドラー

BOOL Cwaifu2xncnnvulkanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	//_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(15935297);

	// TODO: 初期化をここに追加します。
	SetWindowText(_T("waifu2x-ncnn-vulkan GUI Edition [") + APP_VERSION + _T(", Feb. 9, 2021]"));

	MSG msg;
	CFileFind init;
	CString resdir = _T("\\Resources");
	CString initbat = _T("\\Resources\\gpu.bat");
	CString init1 = _T("\\Resources\\ffmpeg\\ffmpeg.exe");
	CString init2 = _T("\\Resources\\waifu2x-ncnn-vulkan\\waifu2x-ncnn-vulkan.exe");
	CString init3 = _T("\\Resources\\image2png.exe");
	CString tmp = CURRENT_PATH + resdir + _T("\\tmp");
	CString ffzip = CURRENT_PATH + resdir + _T("\\ffmpeg.zip");
	CString initbatch = CURRENT_PATH + initbat;
	CString initPath1 = CURRENT_PATH + init1;
	CString initPath2 = CURRENT_PATH + init2;
	CString initPath3 = CURRENT_PATH + init3;
	std::ofstream batchout(initbatch, std::ios::out | std::ios::trunc);
	if (!batchout) {
		MessageBox(_T("読み込みエラー"), _T("エラー"), MB_ICONERROR | MB_OK);
		AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
		AfxGetMainWnd()->DestroyWindow();
		PostQuitMessage(0);
		return TRUE;
	}
	batchout << "@echo off\nset GPUINFO=%~dp0gpuinfo.gl\nfor /F \"tokens=* skip=1\" %%n in ('WMIC path Win32_VideoController get Name ^| findstr \".\"') do set GPU_NAME=%%n\necho %GPU_NAME% >> %GPUINFO%\nexit 0\n";
	batchout.close();

	if (init.FindFile(initbatch)) {
		std::ifstream batch(initbatch, std::ios::in);
		if (!batch) {
			MessageBox(_T("読み込みエラー"), _T("エラー"), MB_ICONERROR | MB_OK);
			AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
			AfxGetMainWnd()->DestroyWindow();
			PostQuitMessage(0);
		}
		OutputDebugString(_T("CFileFind init.FindFile(") + initbatch + _T("): File readed success.\n"));
		std::string batfile = "@echo off\n";
		std::string batfilefull = "@echo off\nset GPUINFO=%~dp0gpuinfo.gl\nfor /F \"tokens=* skip=1\" %%n in ('WMIC path Win32_VideoController get Name ^| findstr \".\"') do set GPU_NAME=%%n\necho %GPU_NAME% >> %GPUINFO%\nexit 0\n";
		std::string batf = batfilefull;
		std::stringstream batch_stream;
		batch_stream << batch.rdbuf();
		std::string batch_string = batch_stream.str();
		size_t linepos = batch_string.find(batfile);
		if (linepos == std::string::npos) {
			batch.close();
			MessageBox(_T("このバッチファイルは不正です。"), _T("ファイル改竄エラー"), MB_ICONERROR | MB_OK);
			AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
			AfxGetMainWnd()->DestroyWindow();
			PostQuitMessage(0);
			return TRUE;
		}
		else if (batch_string != batf) {
			initbatch.Empty();
			batch.close();
			MessageBox(_T("このバッチファイルは不正、または変更されています。"), _T("ファイル改竄エラー"), MB_ICONERROR | MB_OK);
			AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
			AfxGetMainWnd()->DestroyWindow();
			PostQuitMessage(0);
			return TRUE;
		}
		else {
			OutputDebugString(_T("delete (") + initbatch + _T("): Empty.\n"));
			initbatch.Empty();
			batch.close();
		}
	}
	else {
		CString Err;
		Err.Format(_T("ファイル読み込みエラー: 0x%X"), -1);
		OutputDebugString(_T("Error.\n"));
		MessageBox(_T("ファイルが存在しません。\nアプリケーションを終了します。"), Err, MB_ICONERROR | MB_OK);
		AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
		AfxGetMainWnd()->DestroyWindow();
		PostQuitMessage(0);
	}

	GetPrivateProfileString(L"WAIFU2X_SETTINGS", L"0x20FF", NULL, Waifu2x->waifu2x_prm, 256, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_SETTINGS", L"0x20FF", NULL, Waifu2x->FFmpeg_prm, 256, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x20FF", NULL, Waifu2x->FFmpeg_video_prm, 256, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x20FF", NULL, Waifu2x->FFmpeg_audio_prm, 256, L".\\settings.ini");

	if (Waifu2x->waifu2x_prm != L"") {
		waifu2x_param = Waifu2x->waifu2x_prm;
	}
	else {
		waifu2x_param = L"";
	}
	if (Waifu2x->FFmpeg_prm != L"") {
		ffmpeg_mainparam = Waifu2x->FFmpeg_prm;
	}
	else {
		ffmpeg_mainparam = L"";
	}
	if (Waifu2x->FFmpeg_video_prm != L"") {
		ffmpeg_videoparam = Waifu2x->FFmpeg_video_prm;
	}
	else {
		ffmpeg_videoparam = L"";
	}
	if (Waifu2x->FFmpeg_audio_prm != L"") {
		ffmpeg_audioparam = Waifu2x->FFmpeg_audio_prm;
	}
	else {
		ffmpeg_audioparam = L"";
	}

	int CPUInfo[5] = { -1 };
	__cpuid(CPUInfo, 0x80000000);
	if (CPUInfo[0] >= 0x80000004)
	{
		CHAR szCPUBrandString[0x41] = { 0 };
		__cpuid(CPUInfo, 0x80000002);
		memcpy(szCPUBrandString, CPUInfo, sizeof(CPUInfo));
		__cpuid(CPUInfo, 0x80000003);
		memcpy(szCPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		__cpuid(CPUInfo, 0x80000004);
		memcpy(szCPUBrandString + 32, CPUInfo, sizeof(CPUInfo));

		const int tchrSize = sizeof(szCPUBrandString) + 1;
		TCHAR tchrText2[tchrSize] = { _T('¥0') };
		int res = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szCPUBrandString, sizeof(szCPUBrandString), tchrText2, tchrSize);

		CString CPUText = CString(tchrText2);
		CString CPUID = L"" + CPUText + L"\n";

		CString glcmd = L"cmd /c .\\Resources\\gpu.bat";
		TCHAR* glcmdT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
		if (NULL == glcmdT) {
			perror("can not malloc");
			OutputDebugString(_T("TCHAR syntax (glcmdT) malloc failed.\n"));
			AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
			AfxGetMainWnd()->DestroyWindow();
			PostQuitMessage(0);
		}
		else {
			ZeroMemory(&glcmdT[0], 256);
			_tcscpy_s(&glcmdT[0], 256, glcmd);
		}
		
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		::CreateProcess(NULL, glcmdT, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		CloseHandle(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		SAFE_FREE(glcmdT);

		std::ifstream gl(CURRENT_PATH + L"\\Resources\\gpuinfo.gl", std::ios::in);
		if (!gl) {
			MessageBox(_T("読み込みエラー"), _T("エラー"), MB_ICONERROR | MB_OK);
			AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
			AfxGetMainWnd()->DestroyWindow();
			PostQuitMessage(0);
		}
		
		std::stringstream gl_stream;
		gl_stream << gl.rdbuf();
		std::string gl_string = gl_stream.str();
		std::string glinfo;
		gl.close();
		DeleteFile(CURRENT_PATH + L"\\Resources\\gpu.bat");
		DeleteFile(CURRENT_PATH + L"\\Resources\\gpuinfo.gl");
		SetCurrentDirectory(CURRENT_PATH);

		CString GPUInfo(gl_string.c_str());
		CString Info = CPUID;

		OutputDebugString(_T("Detected CPU: ") + CPUID);
		OutputDebugString(_T("Detected GPU: ") + GPUInfo);

		ZeroMemory(&Waifu2x->cpuinfo[0], 256);
		ZeroMemory(&Waifu2x->gpuinfo[0], 256);
		_tcscpy_s(&Waifu2x->cpuinfo[0], 256, CPUID);
		_tcscpy_s(&Waifu2x->gpuinfo[0], 256, GPUInfo);

		CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT2);
		CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT4);
		edit1->SetWindowText(Waifu2x->cpuinfo);
		edit2->SetWindowText(Waifu2x->gpuinfo);
	}

	Waifu2x->StFont1 = xv_Static_ReadStatus.GetFont();
	LOGFONT lf1;
	Waifu2x->StFont1->GetLogFont(&lf1);
	lf1.lfWeight = FW_BOLD;
	Waifu2x->fnt1->CreateFontIndirectW(&lf1);
	this->xv_Static_ReadStatus.SetFont(Waifu2x->fnt1);
	OutputDebugString(_T("SetFont: FW_BOLD\n"));

	if (init.FindFile(initPath1)) {
		OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed success.\n"));
		OutputDebugString(_T("FFmpeg version check...\n"));
		UINT FLAG = 0;
		wchar_t ver[96]{};
		GetPrivateProfileString(L"FFMPEG_LATEST_VERSION", L"0x00000", NULL, (LPWSTR)ver, 48, L".\\settings.ini");
		CString VER;
		if (_tcscmp(ver, _T("")) == 0) {
			VER = _T("不明なバージョン");
			FLAG = 0;
		}
		else {
			VER = ver;
			FLAG = 1;
		}
		if (FLAG == 0) {
			UINT ret;
			ret = MessageBox(_T("最新バージョン：") + Utility->FFmpegVersionCheck() + _T("\n現在使用中のバージョン：") + VER + _T("\nFFmpegのアップデートが可能です。アップデートしますか？"), _T("FFmpeg アップデート確認"), MB_ICONINFORMATION | MB_YESNO);
			if (ret == IDYES) {
				Utility->DeleteDirectory(_T(".\\Resources\\ffmpeg"));
				bool ret;
				POPUPDlg* DIALOG = new POPUPDlg;
				DIALOG->Create(IDD_POPUP);
				DIALOG->ShowWindow(SW_SHOW);
				while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
					if (!AfxGetApp()->PumpMessage())
					{
						::PostQuitMessage(0);
						break;
					}
				}
				CString STATUSCODE;
				STATUSCODE.Format(_T("%d\n"), Utility->FFmpegServerCheck());
				OutputDebugString(_T("Server connecting...\nHTTP Status code:") + STATUSCODE);
				if (Utility->FFmpegServerCheck() != HTTP_STATUS_OK) {
					OutputDebugString(_T("Server connected error."));
					MessageBox(_T("サーバーとの通信に失敗しました。\nHTTP Status Code:") + STATUSCODE, _T("エラー"), MB_ICONERROR | MB_OK);
					AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
					AfxGetMainWnd()->DestroyWindow();
					PostQuitMessage(0);
				}
				OutputDebugString(_T("Server connected.\n"));
				ret = Utility->DownloadFile(DOWNLOAD_URL_STRING, _T(".\\Resources\\ffmpeg.zip"), 4096);
				DLCurCount = Utility->GetFileSizeStat(_T(".\\Resources\\ffmpeg.zip"));

				DeleteFile(_T(".\\Resources\\ffmpeg.zip"));

				pDLThread = AfxBeginThread(DLThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
				ASSERT(pDLThread);
				if (pDLThread)
				{
					pDLThread->m_pMainWnd = this;
					pDLThread->m_bAutoDelete = TRUE;
					pDLThread->ResumeThread();
				}
				pDLCountThread = AfxBeginThread(DLCountThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
				ASSERT(pDLCountThread);
				if (pDLCountThread)
				{
					pDLCountThread->m_pMainWnd = this;
					pDLCountThread->m_bAutoDelete = TRUE;
					pDLCountThread->ResumeThread();
				}

				DLDIALOG DLG;
				DLG.DoModal();

				WritePrivateProfileString(L"FFMPEG_LATEST_VERSION", L"0x00000", Utility->FFmpegVersionCheck(), L".\\settings.ini");

				while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
					if (!AfxGetApp()->PumpMessage())
					{
						::PostQuitMessage(0);
						break;
					}
				}

				if (ret) {
					DIALOG->DestroyWindow();
					SAFE_DELETE(DIALOG);
					Utility->checkExistenceOfFolder(".\\Resources\\ffmpeg");
					Utility->checkExistenceOfFolder(".\\Resources\\tmp");
					HRESULT hr;
					IShellDispatch* pShellDisp;
					if (CoInitializeEx(0, COINIT_MULTITHREADED) == 0) {
						OutputDebugString(_T("CoInitializeEx: S_OK\n"));
					}
					else {
						OutputDebugString(_T("CoInitializeEx: S_FALSE\n"));
					}
					hr = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pShellDisp));
					if (FAILED(hr)) {
						CoUninitialize();
						AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
						AfxGetMainWnd()->DestroyWindow();
						PostQuitMessage(0);
					}
					TCHAR* ffzipT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
					if (NULL == ffzipT) {
						perror("can not malloc");
						OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
						AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
						AfxGetMainWnd()->DestroyWindow();
						PostQuitMessage(0);
					}
					else {
						ZeroMemory(&ffzipT[0], 256);
						_tcscpy_s(&ffzipT[0], 256, ffzip);
					}
					TCHAR* tmpT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
					if (NULL == tmpT) {
						perror("can not malloc");
						OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
						AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
						AfxGetMainWnd()->DestroyWindow();
						PostQuitMessage(0);
					}
					else {
						ZeroMemory(&tmpT[0], 256);
						_tcscpy_s(&tmpT[0], 256, tmp);
					}
					Utility->ExtractZip(pShellDisp, ffzipT, tmpT);
					CoUninitialize();
					SAFE_FREE(ffzipT);
					SAFE_FREE(tmpT);
					CopyFile(_T(".\\Resources\\tmp\\ffmpeg-") + Utility->FFmpegVersionCheck() + _T("-essentials_build\\bin\\ffmpeg.exe"), _T(".\\Resources\\ffmpeg\\ffmpeg.exe"), FALSE);
					if (Utility->DeleteDirectory(tmp)) {
						DeleteFile(_T(".\\Resources\\ffmpeg.zip"));
						OutputDebugString(_T("DeleteDirectory(") + tmp + _T("): Delete success.\n"));
					}
					else {
						DeleteFile(_T(".\\Resources\\ffmpeg.zip"));
						OutputDebugString(_T("DeleteDirectory(") + tmp + _T("): Delete error.\n"));
					}
					if (init.FindFile(initPath1)) {
						MessageBox(_T("ダウンロードが完了しました。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
						OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed success.\n"));
						initPath1.Empty();
					}
					else {
						OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed error.\n"));
						initPath1.Empty();
						MessageBox(_T("予期せぬエラーが発生しました。"), _T("エラー"), MB_ICONERROR | MB_OK);
						AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
						AfxGetMainWnd()->DestroyWindow();
						PostQuitMessage(0);
					}
				}
				else {
					DIALOG->DestroyWindow();
					SAFE_DELETE(DIALOG);
					MessageBox(_T("ダウンロードに失敗しました。インターネット接続がされているか確認してください。\nウイルス対策ソフトをインストールしている場合は接続を許可してください。"), _T("エラー"), MB_ICONERROR | MB_OK);
				}
			}
			else if (ret == IDNO) {
				initPath1.Empty();
			}
			else {
				initPath1.Empty();
			}
		}
		else if (FLAG == 1) {
			if (Utility->FFmpegVersionCheck() > VER) {
				UINT ret;
				ret = MessageBox(_T("最新バージョン：") + Utility->FFmpegVersionCheck() + _T("\n現在使用中のバージョン：") + VER + _T("\nFFmpegのアップデートが可能です。アップデートしますか？"), _T("FFmpeg アップデート確認"), MB_ICONINFORMATION | MB_YESNO);
				if (ret == IDYES) {
					Utility->DeleteDirectory(_T(".\\Resources\\ffmpeg"));
					bool ret;
					POPUPDlg* DIALOG = new POPUPDlg;
					DIALOG->Create(IDD_POPUP);
					DIALOG->ShowWindow(SW_SHOW);
					while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
						if (!AfxGetApp()->PumpMessage())
						{
							::PostQuitMessage(0);
							break;
						}
					}
					CString STATUSCODE;
					STATUSCODE.Format(_T("%d\n"), Utility->FFmpegServerCheck());
					OutputDebugString(_T("Server connecting...\nHTTP Status code:") + STATUSCODE);
					if (Utility->FFmpegServerCheck() != HTTP_STATUS_OK) {
						OutputDebugString(_T("Server connected error."));
						MessageBox(_T("サーバーとの通信に失敗しました。\nHTTP Status Code:") + STATUSCODE, _T("エラー"), MB_ICONERROR | MB_OK);
						AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
						AfxGetMainWnd()->DestroyWindow();
						PostQuitMessage(0);
					}
					OutputDebugString(_T("Server connected.\n"));
					ret = Utility->DownloadFile(DOWNLOAD_URL_STRING, _T(".\\Resources\\ffmpeg.zip"), 4096);
					DLCurCount = Utility->GetFileSizeStat(_T(".\\Resources\\ffmpeg.zip"));

					DeleteFile(_T(".\\Resources\\ffmpeg.zip"));

					pDLThread = AfxBeginThread(DLThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
					ASSERT(pDLThread);
					if (pDLThread)
					{
						pDLThread->m_pMainWnd = this;
						pDLThread->m_bAutoDelete = TRUE;
						pDLThread->ResumeThread();
					}
					pDLCountThread = AfxBeginThread(DLCountThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
					ASSERT(pDLCountThread);
					if (pDLCountThread)
					{
						pDLCountThread->m_pMainWnd = this;
						pDLCountThread->m_bAutoDelete = TRUE;
						pDLCountThread->ResumeThread();
					}

					DLDIALOG DLG;
					DLG.DoModal();

					WritePrivateProfileString(L"FFMPEG_LATEST_VERSION", L"0x00000", Utility->FFmpegVersionCheck(), L".\\settings.ini");

					while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
						if (!AfxGetApp()->PumpMessage())
						{
							::PostQuitMessage(0);
							break;
						}
					}

					if (ret) {
						DIALOG->DestroyWindow();
						SAFE_DELETE(DIALOG);
						Utility->checkExistenceOfFolder(".\\Resources\\ffmpeg");
						Utility->checkExistenceOfFolder(".\\Resources\\tmp");
						HRESULT hr;
						IShellDispatch* pShellDisp;
						if (CoInitializeEx(0, COINIT_MULTITHREADED) == 0) {
							OutputDebugString(_T("CoInitializeEx: S_OK\n"));
						}
						else {
							OutputDebugString(_T("CoInitializeEx: S_FALSE\n"));
						}
						hr = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pShellDisp));
						if (FAILED(hr)) {
							CoUninitialize();
							AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
							AfxGetMainWnd()->DestroyWindow();
							PostQuitMessage(0);
						}
						TCHAR* ffzipT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
						if (NULL == ffzipT) {
							perror("can not malloc");
							OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
							AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
							AfxGetMainWnd()->DestroyWindow();
							PostQuitMessage(0);
						}
						else {
							ZeroMemory(&ffzipT[0], 256);
							_tcscpy_s(&ffzipT[0], 256, ffzip);
						}
						TCHAR* tmpT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
						if (NULL == tmpT) {
							perror("can not malloc");
							OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
							AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
							AfxGetMainWnd()->DestroyWindow();
							PostQuitMessage(0);
						}
						else {
							ZeroMemory(&tmpT[0], 256);
							_tcscpy_s(&tmpT[0], 256, tmp);
						}
						Utility->ExtractZip(pShellDisp, ffzipT, tmpT);
						CoUninitialize();
						SAFE_FREE(ffzipT);
						SAFE_FREE(tmpT);
						CopyFile(_T(".\\Resources\\tmp\\ffmpeg-") + Utility->FFmpegVersionCheck() + _T("-essentials_build\\bin\\ffmpeg.exe"), _T(".\\Resources\\ffmpeg\\ffmpeg.exe"), FALSE);
						if (Utility->DeleteDirectory(tmp)) {
							DeleteFile(_T(".\\Resources\\ffmpeg.zip"));
							OutputDebugString(_T("DeleteDirectory(") + tmp + _T("): Delete success.\n"));
						}
						else {
							DeleteFile(_T(".\\Resources\\ffmpeg.zip"));
							OutputDebugString(_T("DeleteDirectory(") + tmp + _T("): Delete error.\n"));
						}
						if (init.FindFile(initPath1)) {
							MessageBox(_T("ダウンロードが完了しました。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
							OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed success.\n"));
							initPath1.Empty();
						}
						else {
							OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed error.\n"));
							initPath1.Empty();
							MessageBox(_T("予期せぬエラーが発生しました。"), _T("エラー"), MB_ICONERROR | MB_OK);
							AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
							AfxGetMainWnd()->DestroyWindow();
							PostQuitMessage(0);
						}
					}
					else {
						DIALOG->DestroyWindow();
						SAFE_DELETE(DIALOG);
						MessageBox(_T("ダウンロードに失敗しました。インターネット接続がされているか確認してください。\nウイルス対策ソフトをインストールしている場合は接続を許可してください。"), _T("エラー"), MB_ICONERROR | MB_OK);
					}
				}
				else if (ret == IDNO) {
					initPath1.Empty();
				}
				else {
					initPath1.Empty();
				}
			}
			else {
				initPath1.Empty();
			}
		}
		else {
		initPath1.Empty();
		}
	}
	else
	{
		initbatch.Empty();
		CString Err;
		Err.Format(_T("ファイル読み込みエラー : 0x%X"), -1);

		INT_PTR sel = MessageBox(_T("ffmpeg.exe が見つかりません。ダウンロードしますか？\n(ダウンロードにはインターネット接続が必要です)"), Err, MB_ICONERROR | MB_YESNO);
		if (sel == IDYES)
		{
			bool ret;
			POPUPDlg* DIALOG = new POPUPDlg;
			DIALOG->Create(IDD_POPUP);
			DIALOG->ShowWindow(SW_SHOW);
			while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
				if (!AfxGetApp()->PumpMessage())
				{
					::PostQuitMessage(0);
					break;
				}
			}
			CString STATUSCODE;
			STATUSCODE.Format(_T("%d\n"), Utility->FFmpegServerCheck());
			OutputDebugString(_T("Server connecting...\nHTTP Status code:") + STATUSCODE);
			if (Utility->FFmpegServerCheck() != HTTP_STATUS_OK) {
				OutputDebugString(_T("Server connected error."));
				MessageBox(_T("サーバーとの通信に失敗しました。\nHTTP Status Code:") + STATUSCODE, _T("エラー"), MB_ICONERROR | MB_OK);
				AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
				AfxGetMainWnd()->DestroyWindow();
				PostQuitMessage(0);
			}
			OutputDebugString(_T("Server connected.\n"));
			ret = Utility->DownloadFile(DOWNLOAD_URL_STRING, _T(".\\Resources\\ffmpeg.zip"), 4096);
			DLCurCount = Utility->GetFileSizeStat(_T(".\\Resources\\ffmpeg.zip"));

			DeleteFile(_T(".\\Resources\\ffmpeg.zip"));

			pDLThread = AfxBeginThread(DLThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			ASSERT(pDLThread);
			if (pDLThread)
			{
				pDLThread->m_pMainWnd = this;
				pDLThread->m_bAutoDelete = TRUE;
				pDLThread->ResumeThread();
			}
			pDLCountThread = AfxBeginThread(DLCountThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			ASSERT(pDLCountThread);
			if (pDLCountThread)
			{
				pDLCountThread->m_pMainWnd = this;
				pDLCountThread->m_bAutoDelete = TRUE;
				pDLCountThread->ResumeThread();
			}

			DLDIALOG DLG;
			DLG.DoModal();

			WritePrivateProfileString(L"FFMPEG_LATEST_VERSION", L"0x00000", Utility->FFmpegVersionCheck(), L".\\settings.ini");

			while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
				if (!AfxGetApp()->PumpMessage())
				{
					::PostQuitMessage(0);
					break;
				}
			}

			if (ret) {
				DIALOG->DestroyWindow();
				SAFE_DELETE(DIALOG);
				Utility->checkExistenceOfFolder(".\\Resources\\ffmpeg");
				Utility->checkExistenceOfFolder(".\\Resources\\tmp");
				HRESULT hr;
				IShellDispatch* pShellDisp;
				if (CoInitializeEx(0, COINIT_MULTITHREADED) == 0) {
					OutputDebugString(_T("CoInitializeEx: S_OK\n"));
				}
				else {
					OutputDebugString(_T("CoInitializeEx: S_FALSE\n"));
				}
				hr = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pShellDisp));
				if (FAILED(hr)) {
					CoUninitialize();
					AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
					AfxGetMainWnd()->DestroyWindow();
					PostQuitMessage(0);
				}
				TCHAR* ffzipT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
				if (NULL == ffzipT) {
					perror("can not malloc");
					OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
					AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
					AfxGetMainWnd()->DestroyWindow();
					PostQuitMessage(0);
				}
				else {
					ZeroMemory(&ffzipT[0], 256);
					_tcscpy_s(&ffzipT[0], 256, ffzip);
				}
				TCHAR* tmpT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
				if (NULL == tmpT) {
					perror("can not malloc");
					OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
					AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
					AfxGetMainWnd()->DestroyWindow();
					PostQuitMessage(0);
				}
				else {
					ZeroMemory(&tmpT[0], 256);
					_tcscpy_s(&tmpT[0], 256, tmp);
				}
				Utility->ExtractZip(pShellDisp, ffzipT, tmpT);
				CoUninitialize();
				SAFE_FREE(ffzipT);
				SAFE_FREE(tmpT);
				CopyFile(_T(".\\Resources\\tmp\\ffmpeg-") + Utility->FFmpegVersionCheck() + _T("-essentials_build\\bin\\ffmpeg.exe"), _T(".\\Resources\\ffmpeg\\ffmpeg.exe"), FALSE);
				if (Utility->DeleteDirectory(tmp)) {
					DeleteFile(_T(".\\Resources\\ffmpeg.zip"));
					OutputDebugString(_T("DeleteDirectory(") + tmp + _T("): Delete success.\n"));
				}
				else {
					DeleteFile(_T(".\\Resources\\ffmpeg.zip"));
					OutputDebugString(_T("DeleteDirectory(") + tmp + _T("): Delete error.\n"));
				}
				if (init.FindFile(initPath1)) {
					MessageBox(_T("ダウンロードが完了しました。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
					OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed success.\n"));
					initPath1.Empty();
					Err.Empty();
				}
				else {
					OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed error.\n"));
					MessageBox(_T("予期せぬエラーが発生しました。"), _T("エラー"), MB_ICONERROR | MB_OK);
					AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
					AfxGetMainWnd()->DestroyWindow();
					PostQuitMessage(0);
				}
			}
			else {
				DIALOG->DestroyWindow();
				SAFE_DELETE(DIALOG);
				MessageBox(_T("ダウンロードに失敗しました。インターネット接続がされているか確認してください。\nウイルス対策ソフトをインストールしている場合は接続を許可してください。"), _T("エラー"), MB_ICONERROR | MB_OK);
			}
		}
		else if (sel == IDNO)
		{
			MessageBox(_T("ファイルが存在しないため、アプリケーションを続行することができません。\nアプリケーションを終了します。"), _T("エラー"), MB_ICONERROR | MB_OK);
			OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed error.\n"));
			AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
			AfxGetMainWnd()->DestroyWindow();
			PostQuitMessage(0);
			return TRUE;
		}
		else {
			AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
			AfxGetMainWnd()->DestroyWindow();
			PostQuitMessage(0);
		}
	}

	if (init.FindFile(initPath2)) {
		OutputDebugString(_T("CFileFind init.FindFile(") + initPath2 + _T("): File readed success.\n"));
		initPath2.Empty();
		initbatch.Empty();
	}
	else
	{
		initbatch.Empty();
		CString Err;
		Err.Format(_T("ファイル読み込みエラー : 0x%X"), -1);
		INT_PTR sel = MessageBox(_T("waifu2x.exe が見つかりません。"), Err, MB_ICONERROR | MB_ABORTRETRYIGNORE);
		if (sel == IDABORT)
		{
			AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
			AfxGetMainWnd()->DestroyWindow();
			PostQuitMessage(0);
		}
		else if (sel == IDRETRY)
		{
			if (init.FindFile(initPath2)) {
				OutputDebugString(_T("CFileFind init.FindFile(") + initPath2 + _T("): File readed success.\n"));
				Err.Empty();
				initPath2.Empty();
			}
			else {
				MessageBox(_T("ファイルが存在しません。\nアプリケーションを終了します。"), _T("エラー"), MB_ICONERROR | MB_OK);
				OutputDebugString(_T("CFileFind init.FindFile(") + initPath2 + _T("): File readed error.\n"));
				AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
				AfxGetMainWnd()->DestroyWindow();
				PostQuitMessage(0);
			}
		}
		else if (sel == IDIGNORE)
		{
			MessageBox(_T("この操作は無視できません。"), _T("エラー"), MB_ICONERROR | MB_OK);
			AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
			AfxGetMainWnd()->DestroyWindow();
			PostQuitMessage(0);
		}
		else {
			AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
			AfxGetMainWnd()->DestroyWindow();
			PostQuitMessage(0);
		}
	}

	if (init.FindFile(initPath3)) {
		OutputDebugString(_T("CFileFind init.FindFile(") + initPath3 + _T("): File readed success.\n"));
		initPath3.Empty();
	}
	else
	{
		CString Err;
		Err.Format(_T("ファイル読み込みエラー : 0x%X"), -1);
		INT_PTR sel = MessageBox(_T("image2png.exe が見つかりません。"), Err, MB_ICONERROR | MB_ABORTRETRYIGNORE);
		if (sel == IDABORT)
		{
			Err.Empty();
			initPath3.Empty();
			exit(EXIT_FAILURE);
		}
		else if (sel == IDRETRY)
		{
			if (init.FindFile(initPath2)) {
				OutputDebugString(_T("CFileFind init.FindFile(") + initPath3 + _T("): File readed success.\n"));
				Err.Empty();
				initPath3.Empty();
			}
			else {
				MessageBox(_T("ファイルが存在しません。\nアプリケーションを終了します。"), _T("エラー"), MB_ICONERROR | MB_OK);
				OutputDebugString(_T("CFileFind init.FindFile(") + initPath3 + _T("): File readed error.\n"));
				Err.Empty();
				initPath3.Empty();
				exit(EXIT_FAILURE);
			}
		}
		else if (sel == IDIGNORE)
		{
			MessageBox(_T("この操作は無視できません。"), _T("エラー"), MB_ICONERROR | MB_OK);
			Err.Empty();
			initPath3.Empty();
			exit(EXIT_FAILURE);
		}
		else {
			Err.Empty();
			initPath3.Empty();
			exit(EXIT_FAILURE);
		}
	}

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

BOOL Cwaifu2xncnnvulkanDlg::PreTranslateMessage(MSG* pMsg)
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

void Cwaifu2xncnnvulkanDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void Cwaifu2xncnnvulkanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR Cwaifu2xncnnvulkanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cwaifu2xncnnvulkanDlg::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	OutputDebugString(_T("Button clicked: button1\n"));
	if (VIDEOPATH.IsEmpty() == TRUE)
	{
		MessageBox(_T("ファイルが選択されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (waifu2x_param.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_mainparam.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_audioparam.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_videoparam.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else
	{
		if (Waifu2x->waifu2x_prm != _T("")) {
			waifu2x_param = Waifu2x->waifu2x_prm;
		}
		CString EXT;
		MSG msg;
		CString outDir = _T("\\Resources\\takeout\\");
		CString currentPath = CURRENT_PATH + outDir;

		CString w2xin;
		CString w2xout = currentPath + _T("_temp-project\\image-frames2x\\");

		wchar_t vext[10]{}, vname[MAX_PATH]{}, videopath[MAX_PATH]{};
		UINT psetv, ext;
		GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1FFF", NULL, (LPWSTR)vext, 10, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2000", NULL, (LPWSTR)vname, MAX_PATH, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2001", NULL, (LPWSTR)videopath, MAX_PATH, L".\\settings.ini");
		psetv = GetPrivateProfileInt(L"FFMPEG_VIDEO_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini");

		if (psetv == 0) {
			w2xin = currentPath + _T("_temp-project\\image-frames\\");
		}
		else if (psetv == 1) {
			if (_tcscmp(videopath, _T("")) != 0) {
				CString vp = (LPCTSTR)videopath;
				vp += _T("\\");
				if (_tcscmp(vname, _T("")) != 0) {
					CString vn = (LPCTSTR)vname;
					w2xin = vp;
				}
			}
		}

		ext = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x0005", INFINITE, L".\\settings.ini");
		if (ext == 0) {
			EXT = _T("JPG");
		}
		else if (ext == 1) {
			EXT = _T("PNG");
		}
		else if (ext == 2) {
			EXT = _T("WEBP");
		}

		Utility->AfxReplaceStr(waifu2x_param, L"$InFile", w2xin);
		Utility->AfxReplaceStr(waifu2x_param, L"$OutFile", w2xout);
		
		char currentPathC[1024];
		WideCharToMultiByte(CP_ACP, 0, currentPath, -1, currentPathC, sizeof(currentPathC), NULL, NULL);

		DELETEDIALOG* delDlg = new DELETEDIALOG;
		delDlg->Create(IDD_DELETEDIALOG);
		delDlg->ShowWindow(SW_SHOW);
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!AfxGetApp()->PumpMessage())
			{
				::PostQuitMessage(0);
				break;
			}
		}

		SHFILEOPSTRUCT tSHDeleteFile{};
		tSHDeleteFile.hwnd = ::GetDesktopWindow();
		tSHDeleteFile.wFunc = FO_DELETE;
		tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
		tSHDeleteFile.fAnyOperationsAborted = TRUE;
		tSHDeleteFile.hNameMappings = NULL;
		tSHDeleteFile.lpszProgressTitle = L"";

		// パス名のCStringの末尾に\0をつけて設定  
		currentPath += "?";
		currentPath.SetAt(currentPath.GetLength() - 1, NULL);
		tSHDeleteFile.pFrom = currentPath;
		tSHDeleteFile.pTo = NULL;
		SHFileOperation(&tSHDeleteFile);
		WaitForSingleObject(&tSHDeleteFile, INFINITE);
		delDlg->DestroyWindow();
		SAFE_DELETE(delDlg);

		if (_mkdir(currentPathC) == 0) {
			if (_mkdir("Resources\\takeout\\_temp-project") == 0) {
				if (_mkdir("Resources\\takeout\\_temp-project\\streams") == 0) {
					if (_mkdir("Resources\\takeout\\_temp-project\\image-frames") == 0) {
						if (_mkdir("Resources\\takeout\\_temp-project\\image-frames2x") == 0) {
							OutputDebugString(_T("all mkdir successed.\n"));
						}
						else {
							OutputDebugString(_T("_mkdir failed.\n"));
						}
					}
					else {
						OutputDebugString(_T("_mkdir failed.\n"));
					}
				}
				else {
					OutputDebugString(_T("_mkdir failed.\n"));
				}
			}
			else {
				OutputDebugString(_T("_mkdir failed.\n"));
			}
		}
		else {
			OutputDebugString(_T("_mkdir failed.\n"));
		}

		pFFmpegThread = AfxBeginThread(FFmpegThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		if (pFFmpegThread)
		{
			pFFmpegThread->m_pMainWnd = this;
			pFFmpegThread->m_bAutoDelete = TRUE;
			pFFmpegThread->ResumeThread();
		}
		WaitForSingleObject(pFFmpegThread->m_hThread, INFINITE);

		pWaifu2xCountThread = AfxBeginThread(waifu2xCountThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		if (pWaifu2xCountThread)
		{
			pWaifu2xCountThread->m_pMainWnd = this;
			pWaifu2xCountThread->m_bAutoDelete = TRUE;
			pWaifu2xCountThread->ResumeThread();
		}
		CThreadWaitDlgThread* pThread = DYNAMIC_DOWNCAST(CThreadWaitDlgThread, AfxBeginThread(RUNTIME_CLASS(CThreadWaitDlgThread), 0, 0, CREATE_SUSPENDED));
		pThread->m_bAutoDelete = TRUE;
		pThread->ResumeThread();
		while (pThread->m_Dlg.m_hWnd == 0) {
			Sleep(0);
		}

		while (ProgressThreadFlag != TRUE) {
		}
		while (pThread->m_Dlg.m_hWnd) {
		}
		ProgressThreadFlag = FALSE;
		
		if (SuspendFlag == TRUE) {
			while (pThread->m_Dlg.m_hWnd) {
			}
			SuspendFlag = FALSE;
			Utility->DeleteDirectory(CURRENT_PATH + _T("\\Resources\\takeout"));
			MessageBox(_T("作業が中止されました。"), _T("完了"), MB_ICONWARNING | MB_OK);
			CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
			CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
			CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
			CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
			CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
			button1->EnableWindow(FALSE);
			button2->EnableWindow(FALSE);
			button3->EnableWindow(FALSE);
			button4->EnableWindow(FALSE);
			button5->EnableWindow(FALSE);
			this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			return;
		}

		if (EXT == L"JPG") {
			CString prenameCom = L"/c ren *.png.jpg *.\nexit 0";
			CString prenameComext = L"/c ren *.png *.jpg\nexit 0";
			ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", prenameComext, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
		}
		else if (EXT == L"PNG") {
			CString prenameCom = L"/c ren *.png.png *.\nexit 0";
			ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
		}
		else if (EXT == L"WEBP") {
			CString prenameCom = L"/c ren *.png.webp *.\nexit 0";
			CString prenameComext = L"/c ren *.png *.webp\nexit 0";
			ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", prenameComext, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
		}
		else {
			CString renameCom = L"/c ren *.png.png *.\nexit 0";
			ShellExecute(NULL, L"open", L"cmd.exe", renameCom, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_SHOWNORMAL);
		}

		CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
		CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
		CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
		CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON6);
		CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON2);
		button1->EnableWindow(FALSE);
		button2->EnableWindow(TRUE);
		button3->EnableWindow(TRUE);
		button4->EnableWindow(FALSE);
		button5->EnableWindow(FALSE);

		MessageBox(_T("アップスケールが完了しました。\n更にアップスケール変換したい場合は、再変換ボタンをクリックしてください。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
	}
}

void Cwaifu2xncnnvulkanDlg::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	OutputDebugString(_T("Button clicked: button3\n"));
	Waifu2xReUpscalingFlag = TRUE;
	if (VIDEOPATH.IsEmpty() == TRUE)
	{
		MessageBox(_T("ファイルが選択されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (waifu2x_param.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_mainparam.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_audioparam.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_videoparam.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}

	if (_tcscmp(Waifu2x->waifu2x_prm, _T("")) != 0) {
		waifu2x_param = Waifu2x->waifu2x_prm;
	}
	UINT ext;
	CString EXT;
	MSG msg;
	CString outDir = L"\\Resources\\takeout\\_temp-project\\image-frames\\*.*";
	CString currentPath = CURRENT_PATH + outDir;
	CString current = L"\\Resources\\takeout\\";
	CString imgcurrentPath = CURRENT_PATH + current;
	CString w2xin = imgcurrentPath + L"_temp-project\\image-frames\\";
	CString w2xout = imgcurrentPath + L"_temp-project\\image-frames2x\\";

	ext = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x0005", INFINITE, L".\\settings.ini");
	if (ext == 0) {
		EXT = L"JPG";
	}
	else if (ext == 1) {
		EXT = L"PNG";
	}
	else if (ext == 2) {
		EXT = L"WEBP";
	}

	Utility->AfxReplaceStr(waifu2x_param, L"$InFile", w2xin);
	Utility->AfxReplaceStr(waifu2x_param, L"$OutFile", w2xout);

	DELETEDIALOG* delDlg = new DELETEDIALOG;
	delDlg->Create(IDD_DELETEDIALOG);
	delDlg->ShowWindow(SW_SHOW);
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage())
		{
			::PostQuitMessage(0);
			break;
		}
	}

	SHFILEOPSTRUCT tSHDeleteFile{};
	tSHDeleteFile.hwnd = ::GetDesktopWindow();
	tSHDeleteFile.wFunc = FO_DELETE;
	tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
	tSHDeleteFile.fAnyOperationsAborted = TRUE;
	tSHDeleteFile.hNameMappings = NULL;
	tSHDeleteFile.lpszProgressTitle = L"";

	// パス名のCStringの末尾に\0をつけて設定  
	currentPath += "?";
	currentPath.SetAt(currentPath.GetLength() - 1, NULL);
	tSHDeleteFile.pFrom = currentPath;
	tSHDeleteFile.pTo = NULL;
	SHFileOperation(&tSHDeleteFile);
	WaitForSingleObject(&tSHDeleteFile, INFINITE);
	delDlg->DestroyWindow();
	SAFE_DELETE(delDlg);

	POPUPDlg* POPUP = new POPUPDlg;
	POPUP->Create(IDD_POPUP);
	POPUP->ShowWindow(SW_SHOW);
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage())
		{
			::PostQuitMessage(0);
			break;
		}
	}

	if (EXT == L"JPG") {
		CString prenameCom = L"/c ren *.png.jpg *.\nexit 0";
		CString prenameComext = L"/c ren *.png *.jpg\nexit 0";
		ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
		ShellExecute(NULL, L"open", L"cmd.exe", prenameComext, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
	}
	else if (EXT == L"PNG") {
		CString prenameCom = L"/c ren *.png.png *.\nexit 0";
		ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
	}
	else if (EXT == L"WEBP") {
		CString prenameCom = L"/c ren *.png.webp *.\nexit 0";
		CString prenameComext = L"/c ren *.png *.webp\nexit 0";
		ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
		ShellExecute(NULL, L"open", L"cmd.exe", prenameComext, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
	}

	CString cmd = L"cmd /c move " + CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\*.* " + CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames\\\nexit 0";
	TCHAR* cmdT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
	if (NULL == cmdT) {
		perror("can not malloc");
		OutputDebugString(_T("TCHAR syntax (PRMT) malloc failed.\n"));
		return;
	}
	else {
		ZeroMemory(&cmdT[0], 256);
		_tcscpy_s(&cmdT[0], 256, cmd);
	}

	STARTUPINFO si/* = { sizeof(STARTUPINFO) }*/;
	memset(&si, 0, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	::CreateProcess(NULL, cmdT, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	SAFE_FREE(cmdT);

	POPUP->DestroyWindow();
	SAFE_DELETE(POPUP);
	POPUP = NULL;

	pWaifu2xCountThread = AfxBeginThread(waifu2xCountThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	if (pWaifu2xCountThread)
	{
		pWaifu2xCountThread->m_pMainWnd = this;
		pWaifu2xCountThread->m_bAutoDelete = TRUE;
		pWaifu2xCountThread->ResumeThread();
	}
	CThreadWaitDlgThread* pThread = DYNAMIC_DOWNCAST(CThreadWaitDlgThread, AfxBeginThread(RUNTIME_CLASS(CThreadWaitDlgThread), 0, 0, CREATE_SUSPENDED));
	pThread->m_bAutoDelete = TRUE;
	pThread->ResumeThread();
	while (pThread->m_Dlg.m_hWnd == 0) {
		Sleep(0);
	}

	while (ProgressThreadFlag != TRUE) {
	}
	while (pThread->m_Dlg.m_hWnd) {
	}
	ProgressThreadFlag = FALSE;

	if (SuspendFlag == TRUE) {
		while (pThread->m_Dlg.m_hWnd) {
		}
		SuspendFlag = FALSE;
		Utility->DeleteDirectory(CURRENT_PATH + _T("\\Resources\\takeout"));
		MessageBox(_T("作業が中止されました。"), _T("完了"), MB_ICONWARNING | MB_OK);
		CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
		CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
		CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
		CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
		CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
		button1->EnableWindow(FALSE);
		button2->EnableWindow(FALSE);
		button3->EnableWindow(FALSE);
		button4->EnableWindow(FALSE);
		button5->EnableWindow(FALSE);
		this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
		this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
		this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
		CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
		static1->InvalidateRect(NULL, 1);
		return;
	}

	if (EXT == L"JPG") {
		CString prenameCom = L"/c ren *.png.jpg *.\nexit 0";
		CString prenameComext = L"/c ren *.png *.jpg\nexit 0";
		ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
		ShellExecute(NULL, L"open", L"cmd.exe", prenameComext, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
	}
	else if (EXT == L"PNG") {
		CString prenameCom = L"/c ren *.png.png *.\nexit 0";
		ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
	}
	else if (EXT == L"WEBP") {
		CString prenameCom = L"/c ren *.png.webp *.\nexit 0";
		CString prenameComext = L"/c ren *.png *.webp\nexit 0";
		ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
		ShellExecute(NULL, L"open", L"cmd.exe", prenameComext, CURRENT_PATH + L"\\Resources\\takeout\\_temp-project\\image-frames2x\\", SW_HIDE);
	}

	Waifu2xReUpscalingFlag = FALSE;
	MessageBox(_T("アップスケールが完了しました。\n更にアップスケール変換したい場合は、再変換ボタンをクリックしてください。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
}


void Cwaifu2xncnnvulkanDlg::OnBnClickedButton4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	OutputDebugString(_T("Button clicked: button4\n"));
	if (VIDEOPATH.IsEmpty() == TRUE)
	{
		MessageBox(_T("ファイルが選択されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (waifu2x_param.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_mainparam.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_audioparam.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_videoparam.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}

	if (_tcscmp(Waifu2x->waifu2x_prm, _T("")) != 0) {
		ffmpeg_mainparam = Waifu2x->FFmpeg_prm;
	}

	CFileFind find;
	CString outDir = L"\\Resources\\takeout\\";
	CString currentPath = CURRENT_PATH + outDir;

	CString filter("動画ファイル (*.avi,*.mp4,*.wmv,*.webm,*.mkv,*.mov)|*.avi;*.mp4;*.wmv;*.webm;*.mkv;*.mov;||");
	CFileDialog selDlg(FALSE, _T("mp4"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
	intptr_t file = selDlg.DoModal();
	if (file == IDOK) {
		CString OutPath = selDlg.GetPathName();
		TCHAR* lpPath = (TCHAR*)malloc(sizeof(TCHAR) * 512); // Convert CString to TCHAR.
		if (NULL == lpPath) {
			perror("can not malloc");
			OutputDebugString(_T("TCHAR syntax (lpPath) malloc failed.\n"));
			return;
		}
		else {
			ZeroMemory(&lpPath[0], 512);
			_tcscpy_s(&lpPath[0], 512, OutPath);
		}
		
		CString inImageDir;
		CString inAudioDir;

		wchar_t aext[10]{}, aname[MAX_PATH]{}, audiopath[MAX_PATH]{}, vext[10]{}, vname[MAX_PATH]{}, videopath[MAX_PATH]{};
		UINT pseta, psetv;
		GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1FFF", NULL, (LPWSTR)aext, 10, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2000", NULL, (LPWSTR)aname, MAX_PATH, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2001", NULL, (LPWSTR)audiopath, MAX_PATH, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1FFF", NULL, (LPWSTR)vext, 10, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2000", NULL, (LPWSTR)vname, MAX_PATH, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2001", NULL, (LPWSTR)videopath, MAX_PATH, L".\\settings.ini");
		pseta = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini");
		psetv = GetPrivateProfileInt(L"FFMPEG_VIDEO_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini");

		if (pseta == 0) {
			inAudioDir = L".\\Resources\\takeout\\_temp-project\\streams\\audio.wav";
		}
		else if (pseta == 1) {
			if (_tcscmp(audiopath, _T("")) != 0) {
				CString ap = (LPCTSTR)audiopath;
				ap += _T("\\");
				if (_tcscmp(aname, _T("")) != 0) {
					CString an = (LPCTSTR)aname;
					inAudioDir = ap + an + (LPCTSTR)aext;
				}
			}
		}
		if (psetv == 0) {
			inImageDir = L".\\Resources\\takeout\\_temp-project\\image-frames2x\\image-%09d.png";
		}
		else if (psetv == 1) {
			if (_tcscmp(videopath, _T("")) != 0) {
				CString vp = (LPCTSTR)videopath;
				vp += _T("\\");
				if (_tcscmp(vname, _T("")) != 0) {
					CString vn = (LPCTSTR)vname;
					inImageDir = L".\\Resources\\takeout\\_temp-project\\image-frames2x\\";
					inImageDir += vn += (LPCTSTR)vext;
				}
			}
		}

		Utility->AfxReplaceStr(ffmpeg_mainparam, L"ffmpeg", _T(".\\Resources\\ffmpeg\\ffmpeg.exe"));
		Utility->AfxReplaceStr(ffmpeg_mainparam, L"$InFile1", inImageDir);
		Utility->AfxReplaceStr(ffmpeg_mainparam, L"$InFile2", inAudioDir);
		Utility->AfxReplaceStr(ffmpeg_mainparam, L"$OutFile", lpPath);
		TCHAR* ffmpegcmdT = (TCHAR*)malloc(sizeof(TCHAR) * 512); // Convert CString to TCHAR.
		if (NULL == ffmpegcmdT) {
			perror("can not malloc");
			OutputDebugString(_T("TCHAR syntax (ffmpegcmdT) malloc failed.\n"));
			return;
		}
		else {
			ZeroMemory(&ffmpegcmdT[0], 512);
			_tcscpy_s(&ffmpegcmdT[0], 512, ffmpeg_mainparam);
		}

		WAITDIALOG* waitDlg = new WAITDIALOG;

		waitDlg->Create(IDD_WAITDIALOG);
		waitDlg->ShowWindow(SW_SHOW);
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!AfxGetApp()->PumpMessage())
			{
				::PostQuitMessage(0);
				break;
			}
		}

		STARTUPINFO si/* = { sizeof(STARTUPINFO) }*/;
		memset(&si, 0, sizeof(STARTUPINFO));
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		::CreateProcess(NULL, ffmpegcmdT, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		CloseHandle(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);

		waitDlg->DestroyWindow();
		SAFE_DELETE(waitDlg);

		CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
		CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
		CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
		CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
		CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
		button1->EnableWindow(FALSE);
		button2->EnableWindow(FALSE);
		button3->EnableWindow(FALSE);
		button4->EnableWindow(FALSE);
		button5->EnableWindow(FALSE);

		DELETEDIALOG* delDlg = new DELETEDIALOG;
		delDlg->Create(IDD_DELETEDIALOG);
		delDlg->ShowWindow(SW_SHOW);
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!AfxGetApp()->PumpMessage())
			{
				::PostQuitMessage(0);
				break;
			}
		}

		if (_tcscmp(audiopath, _T("")) != 0) {
			CString ap = (LPCTSTR)audiopath;
			ap += _T("\\*.*");
			if (_tcscmp(aname, _T("")) != 0) {
				SHFILEOPSTRUCT tSHDeleteFile{};
				tSHDeleteFile.hwnd = ::GetDesktopWindow();
				tSHDeleteFile.wFunc = FO_DELETE;
				tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
				tSHDeleteFile.fAnyOperationsAborted = TRUE;
				tSHDeleteFile.hNameMappings = NULL;
				tSHDeleteFile.lpszProgressTitle = L"";

				// パス名のCStringの末尾に\0をつけて設定  
				ap += "?";
				ap.SetAt(ap.GetLength() - 1, NULL);
				tSHDeleteFile.pFrom = ap;
				tSHDeleteFile.pTo = NULL;
				SHFileOperation(&tSHDeleteFile);
				WaitForSingleObject(&tSHDeleteFile, INFINITE);
			}
		}
		if (_tcscmp(videopath, _T("")) != 0) {
			CString vp = (LPCTSTR)videopath;
			vp += _T("\\*.*");
			if (_tcscmp(vname, _T("")) != 0) {
				SHFILEOPSTRUCT tSHDeleteFile{};
				tSHDeleteFile.hwnd = ::GetDesktopWindow();
				tSHDeleteFile.wFunc = FO_DELETE;
				tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
				tSHDeleteFile.fAnyOperationsAborted = TRUE;
				tSHDeleteFile.hNameMappings = NULL;
				tSHDeleteFile.lpszProgressTitle = L"";

				// パス名のCStringの末尾に\0をつけて設定  
				vp += "?";
				vp.SetAt(vp.GetLength() - 1, NULL);
				tSHDeleteFile.pFrom = vp;
				tSHDeleteFile.pTo = NULL;
				SHFileOperation(&tSHDeleteFile);
				WaitForSingleObject(&tSHDeleteFile, INFINITE);
			}
		}

		SHFILEOPSTRUCT tSHDeleteFile{};
		tSHDeleteFile.hwnd = ::GetDesktopWindow();
		tSHDeleteFile.wFunc = FO_DELETE;
		tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
		tSHDeleteFile.fAnyOperationsAborted = TRUE;
		tSHDeleteFile.hNameMappings = NULL;
		tSHDeleteFile.lpszProgressTitle = L"";

		// パス名のCStringの末尾に\0をつけて設定  
		currentPath += "?";
		currentPath.SetAt(currentPath.GetLength() - 1, NULL);
		tSHDeleteFile.pFrom = currentPath;
		tSHDeleteFile.pTo = NULL;
		SHFileOperation(&tSHDeleteFile);
		WaitForSingleObject(&tSHDeleteFile, INFINITE);
		delDlg->DestroyWindow();
		SAFE_DELETE(delDlg);

		SAFE_FREE(lpPath);
		SAFE_FREE(ffmpegcmdT);

		if (find.FindFile(OutPath)) {
			MessageBox(_T("変換が完了しました。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
		}
		else {
			MessageBox(_T("変換に失敗しました。"), _T("エラー"), MB_ICONERROR | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
		}
	}
	else {
		MessageBox(_T("保存がキャンセルされました。"), _T("キャンセルされました。"), MB_ICONWARNING | MB_OK);

		wchar_t aext[10]{}, aname[MAX_PATH]{}, audiopath[MAX_PATH]{}, vext[10]{}, vname[MAX_PATH]{}, videopath[MAX_PATH]{};
		UINT pseta, psetv;
		GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1FFF", NULL, (LPWSTR)aext, 10, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2000", NULL, (LPWSTR)aname, MAX_PATH, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2001", NULL, (LPWSTR)audiopath, MAX_PATH, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2000", NULL, (LPWSTR)vext, 10, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2000", NULL, (LPWSTR)vname, MAX_PATH, L".\\settings.ini");
		GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2001", NULL, (LPWSTR)videopath, MAX_PATH, L".\\settings.ini");
		pseta = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini");
		psetv = GetPrivateProfileInt(L"FFMPEG_VIDEO_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini");

		DELETEDIALOG* delDlg = new DELETEDIALOG;
		delDlg->Create(IDD_DELETEDIALOG);
		delDlg->ShowWindow(SW_SHOW);
		MSG msg;
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!AfxGetApp()->PumpMessage())
			{
				::PostQuitMessage(0);
				break;
			}
		}

		if (pseta == 0) {
			OutputDebugString(_T("Audio settings preset detected: Default preset"));
		}
		else if (pseta == 1) {
			if (_tcscmp(audiopath, _T("")) != 0) {
				CString ap = (LPCTSTR)audiopath;
				ap += _T("\\*.*");
				if (_tcscmp(aname, _T("")) != 0) {
					SHFILEOPSTRUCT tSHDeleteFile{};
					tSHDeleteFile.hwnd = ::GetDesktopWindow();
					tSHDeleteFile.wFunc = FO_DELETE;
					tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
					tSHDeleteFile.fAnyOperationsAborted = TRUE;
					tSHDeleteFile.hNameMappings = NULL;
					tSHDeleteFile.lpszProgressTitle = L"";

					// パス名のCStringの末尾に\0をつけて設定  
					ap += "?";
					ap.SetAt(ap.GetLength() - 1, NULL);
					tSHDeleteFile.pFrom = ap;
					tSHDeleteFile.pTo = NULL;
					SHFileOperation(&tSHDeleteFile);
					WaitForSingleObject(&tSHDeleteFile, INFINITE);
					ZeroMemory(&tSHDeleteFile, sizeof(SHFILEOPSTRUCT));
				}
			}
		}
		if (psetv == 0) {
			OutputDebugString(_T("Video settings preset detected: Default preset"));
		}
		else if (psetv == 1) {
			if (_tcscmp(videopath, _T("")) != 0) {
				CString vp = (LPCTSTR)videopath;
				vp += _T("\\*.*");
				if (_tcscmp(vname, _T("")) != 0) {
					SHFILEOPSTRUCT tSHDeleteFile{};
					tSHDeleteFile.hwnd = ::GetDesktopWindow();
					tSHDeleteFile.wFunc = FO_DELETE;
					tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
					tSHDeleteFile.fAnyOperationsAborted = TRUE;
					tSHDeleteFile.hNameMappings = NULL;
					tSHDeleteFile.lpszProgressTitle = L"";

					// パス名のCStringの末尾に\0をつけて設定  
					vp += "?";
					vp.SetAt(vp.GetLength() - 1, NULL);
					tSHDeleteFile.pFrom = vp;
					tSHDeleteFile.pTo = NULL;
					SHFileOperation(&tSHDeleteFile);
					WaitForSingleObject(&tSHDeleteFile, INFINITE);
				}
			}
		}

		SHFILEOPSTRUCT tSHDeleteFile{};
		tSHDeleteFile.hwnd = ::GetDesktopWindow();
		tSHDeleteFile.wFunc = FO_DELETE;
		tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
		tSHDeleteFile.fAnyOperationsAborted = TRUE;
		tSHDeleteFile.hNameMappings = NULL;
		tSHDeleteFile.lpszProgressTitle = L"";

		// パス名のCStringの末尾に\0をつけて設定  
		currentPath += "?";
		currentPath.SetAt(currentPath.GetLength() - 1, NULL);
		tSHDeleteFile.pFrom = currentPath;
		tSHDeleteFile.pTo = NULL;
		SHFileOperation(&tSHDeleteFile);
		WaitForSingleObject(&tSHDeleteFile, INFINITE);

		delDlg->DestroyWindow();
		SAFE_DELETE(delDlg);

		CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
		CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
		CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
		CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
		CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
		button1->EnableWindow(FALSE);
		button2->EnableWindow(FALSE);
		button3->EnableWindow(FALSE);
		button4->EnableWindow(FALSE);
		button5->EnableWindow(FALSE);
		this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
		this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
		this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
		CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
		static1->InvalidateRect(NULL, 1);
		return;
	}
}


void Cwaifu2xncnnvulkanDlg::OnBnClickedButton6()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	UINT ext;
	CString EXT;
	wchar_t Drive[5], Dir[MAX_PATH], Name[MAX_PATH], Ext[MAX_PATH];
	OutputDebugString(_T("Button clicked: button6\n"));
	if (IMAGEPATH.IsEmpty() == TRUE)
	{
		MessageBox(_T("画像ファイルが選択されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (waifu2x_param.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else
	{
		ext = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x0005", INFINITE, L".\\settings.ini");
		if (ext == 0) {
			EXT = L"JPG";
		}
		else if (ext == 1) {
			EXT = L"PNG";
		}
		else if (ext == 2) {
			EXT = L"WEBP";
		}
		_tsplitpath_s(IMAGEPATH, Drive, Dir, Name, Ext);
		CString PRM = CURRENT_PATH + _T("\\Resources\\image2png ") + IMAGEPATH;
		TCHAR* PRMT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
		if (NULL == PRMT) {
			perror("can not malloc");
			OutputDebugString(_T("TCHAR syntax (PRMT) malloc failed.\n"));
			return;
		}
		else {
			ZeroMemory(&PRMT[0], 256);
			_tcscpy_s(&PRMT[0], 256, PRM);
		}
		
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		::CreateProcess(NULL, PRMT, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		CloseHandle(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		SAFE_FREE(PRMT);
		SetCurrentDirectory(CURRENT_PATH);
		CString TDir = Dir;
		CString TDrive = Drive;
		CString TName = Name;
		CString TIMAGEPATH = TDrive + TDir + TName + _T("_e.png");
		Utility->checkExistenceOfFolder(".\\Resources\\tmp");
		MSG msg;
		CFileFind find;
		CString outDir = L"\\Resources\\takeout\\";
		CString currentPath = CURRENT_PATH + outDir;
		CString InPath = CURRENT_PATH + _T("\\Resources\\tmp\\") + TName + _T("_e.png");
		CString OutPath;

		SHFILEOPSTRUCT tSHMoveFile{};
		tSHMoveFile.hwnd = ::GetDesktopWindow();
		tSHMoveFile.wFunc = FO_MOVE;
		tSHMoveFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
		tSHMoveFile.fAnyOperationsAborted = TRUE;
		tSHMoveFile.hNameMappings = NULL;
		tSHMoveFile.lpszProgressTitle = L"";

		// パス名のCStringの末尾に\0をつけて設定  
		TIMAGEPATH += "0";
		TIMAGEPATH.SetAt(TIMAGEPATH.GetLength() - 1, NULL);
		InPath += "0";
		InPath.SetAt(InPath.GetLength() - 1, NULL);
		tSHMoveFile.pFrom = TIMAGEPATH;
		tSHMoveFile.pTo = InPath;
		SHFileOperation(&tSHMoveFile);
		WaitForSingleObject(&tSHMoveFile, INFINITE);
		
		if (EXT == L"JPG") {
			CString filter("JPEGイメージ (*.jpg)|*.jpg;||");
			CFileDialog selDlg(FALSE, _T("jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
			intptr_t file = selDlg.DoModal();
			if (file == IDOK) {
				OutPath = selDlg.GetPathName();
			}
			else {
				if (Utility->DeleteDirectory(_T(".\\Resources\\tmp"))) {
					OutputDebugString(_T("DeleteDirectory successed.\n"));
				}
				else {
					OutputDebugString(_T("DeleteDirectory failed.\n"));
				}
				MessageBox(_T("保存がキャンセルされました。"), _T("キャンセルされました。"), MB_ICONWARNING | MB_OK);
				DELETEDIALOG* delDlg = new DELETEDIALOG;
				delDlg->Create(IDD_DELETEDIALOG);
				delDlg->ShowWindow(SW_SHOW);
				while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
					if (!AfxGetApp()->PumpMessage())
					{
						::PostQuitMessage(0);
						break;
					}
				}
				SHFILEOPSTRUCT tSHDeleteFile{};
				tSHDeleteFile.hwnd = ::GetDesktopWindow();
				tSHDeleteFile.wFunc = FO_DELETE;
				tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
				tSHDeleteFile.fAnyOperationsAborted = TRUE;
				tSHDeleteFile.hNameMappings = NULL;
				tSHDeleteFile.lpszProgressTitle = L"";

				// パス名のCStringの末尾に\0をつけて設定  
				currentPath += "?";
				currentPath.SetAt(currentPath.GetLength() - 1, NULL);
				tSHDeleteFile.pFrom = currentPath;
				tSHDeleteFile.pTo = NULL;
				SHFileOperation(&tSHDeleteFile);
				WaitForSingleObject(&tSHDeleteFile, INFINITE);
				delDlg->DestroyWindow();
				SAFE_DELETE(delDlg);
				CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
				CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
				CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
				CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
				CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
				button1->EnableWindow(FALSE);
				button2->EnableWindow(FALSE);
				button3->EnableWindow(FALSE);
				button4->EnableWindow(FALSE);
				button5->EnableWindow(FALSE);
				this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
				this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
				this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
				CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
				static1->InvalidateRect(NULL, 1);
				return;
			}
		}
		else if (EXT == L"PNG") {
			CString filter("PNGイメージ (*.png)|*.png;||");
			CFileDialog selDlg(FALSE, _T("png"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
			intptr_t file = selDlg.DoModal();
			if (file == IDOK) {
				OutPath = selDlg.GetPathName();
			}
			else {
				if (Utility->DeleteDirectory(_T(".\\Resources\\tmp"))) {
					OutputDebugString(_T("DeleteDirectory successed.\n"));
				}
				else {
					OutputDebugString(_T("DeleteDirectory failed.\n"));
				}
				MessageBox(_T("保存がキャンセルされました。"), _T("キャンセルされました。"), MB_ICONWARNING | MB_OK);
				DELETEDIALOG* delDlg = new DELETEDIALOG;
				delDlg->Create(IDD_DELETEDIALOG);
				delDlg->ShowWindow(SW_SHOW);
				while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
					if (!AfxGetApp()->PumpMessage())
					{
						::PostQuitMessage(0);
						break;
					}
				}
				SHFILEOPSTRUCT tSHDeleteFile{};
				tSHDeleteFile.hwnd = ::GetDesktopWindow();
				tSHDeleteFile.wFunc = FO_DELETE;
				tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
				tSHDeleteFile.fAnyOperationsAborted = TRUE;
				tSHDeleteFile.hNameMappings = NULL;
				tSHDeleteFile.lpszProgressTitle = L"";

				// パス名のCStringの末尾に\0をつけて設定  
				currentPath += "?";
				currentPath.SetAt(currentPath.GetLength() - 1, NULL);
				tSHDeleteFile.pFrom = currentPath;
				tSHDeleteFile.pTo = NULL;
				SHFileOperation(&tSHDeleteFile);
				WaitForSingleObject(&tSHDeleteFile, INFINITE);
				delDlg->DestroyWindow();
				SAFE_DELETE(delDlg);
				CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
				CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
				CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
				CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
				CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
				button1->EnableWindow(FALSE);
				button2->EnableWindow(FALSE);
				button3->EnableWindow(FALSE);
				button4->EnableWindow(FALSE);
				button5->EnableWindow(FALSE);
				this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
				this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
				this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
				CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
				static1->InvalidateRect(NULL, 1);
				return;
			}
		}
		else if (EXT == L"WEBP") {
			CString filter("WEBPイメージ (*.webp)|*.webp;||");
			CFileDialog selDlg(FALSE, _T("webp"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
			intptr_t file = selDlg.DoModal();
			if (file == IDOK) {
				OutPath = selDlg.GetPathName();
			}
			else {
				if (Utility->DeleteDirectory(_T(".\\Resources\\tmp"))) {
					OutputDebugString(_T("DeleteDirectory successed.\n"));
				}
				else {
					OutputDebugString(_T("DeleteDirectory failed.\n"));
				}
				MessageBox(_T("保存がキャンセルされました。"), _T("キャンセルされました。"), MB_ICONWARNING | MB_OK);
				DELETEDIALOG* delDlg = new DELETEDIALOG;
				delDlg->Create(IDD_DELETEDIALOG);
				delDlg->ShowWindow(SW_SHOW);
				while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
					if (!AfxGetApp()->PumpMessage())
					{
						::PostQuitMessage(0);
						break;
					}
				}
				SHFILEOPSTRUCT tSHDeleteFile{};
				tSHDeleteFile.hwnd = ::GetDesktopWindow();
				tSHDeleteFile.wFunc = FO_DELETE;
				tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
				tSHDeleteFile.fAnyOperationsAborted = TRUE;
				tSHDeleteFile.hNameMappings = NULL;
				tSHDeleteFile.lpszProgressTitle = L"";

				// パス名のCStringの末尾に\0をつけて設定  
				currentPath += "?";
				currentPath.SetAt(currentPath.GetLength() - 1, NULL);
				tSHDeleteFile.pFrom = currentPath;
				tSHDeleteFile.pTo = NULL;
				SHFileOperation(&tSHDeleteFile);
				WaitForSingleObject(&tSHDeleteFile, INFINITE);
				delDlg->DestroyWindow();
				SAFE_DELETE(delDlg);
				CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
				CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
				CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
				CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
				CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
				button1->EnableWindow(FALSE);
				button2->EnableWindow(FALSE);
				button3->EnableWindow(FALSE);
				button4->EnableWindow(FALSE);
				button5->EnableWindow(FALSE);
				this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
				this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
				this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
				CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
				static1->InvalidateRect(NULL, 1);
				return;
			}
		}

		if (_tcscmp(Waifu2x->waifu2x_prm, _T("")) != 0) {
			waifu2x_param = Waifu2x->waifu2x_prm;
		}
		TCHAR* OutPathT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
		if (NULL == OutPathT) {
			perror("can not malloc");
			OutputDebugString(_T("TCHAR syntax (OutPathT) malloc failed.\n"));
			return;
		}
		else {
			ZeroMemory(&OutPathT[0], 256);
			_tcscpy_s(&OutPathT[0], 256, OutPath);
			Utility->AfxReplaceStr(waifu2x_param, _T("$OutFile"), OutPathT);
		}
		TCHAR* InPathT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
		if (NULL == InPathT) {
			perror("can not malloc");
			OutputDebugString(_T("TCHAR syntax (InPathT) malloc failed.\n"));
			return;
		}
		else {
			ZeroMemory(&InPathT[0], 256);
			_tcscpy_s(&InPathT[0], 256, InPath);
			Utility->AfxReplaceStr(waifu2x_param, _T("$InFile"), InPathT);
		}

		DELETEDIALOG* delDlg = new DELETEDIALOG;
		delDlg->Create(IDD_DELETEDIALOG);
		delDlg->ShowWindow(SW_SHOW);
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!AfxGetApp()->PumpMessage())
			{
				::PostQuitMessage(0);
				break;
			}
		}
		SHFILEOPSTRUCT tSHDeleteFile{};
		tSHDeleteFile.hwnd = ::GetDesktopWindow();
		tSHDeleteFile.wFunc = FO_DELETE;
		tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
		tSHDeleteFile.fAnyOperationsAborted = TRUE;
		tSHDeleteFile.hNameMappings = NULL;
		tSHDeleteFile.lpszProgressTitle = L"";

		// パス名のCStringの末尾に\0をつけて設定  
		currentPath += "?";
		currentPath.SetAt(currentPath.GetLength() - 1, NULL);
		tSHDeleteFile.pFrom = currentPath;
		tSHDeleteFile.pTo = NULL;
		SHFileOperation(&tSHDeleteFile);
		WaitForSingleObject(&tSHDeleteFile, INFINITE);

		delDlg->DestroyWindow();
		SAFE_DELETE(delDlg);

		WAITDIALOG* waitDlg = new WAITDIALOG;

		waitDlg->Create(IDD_WAITDIALOG);
		waitDlg->ShowWindow(SW_SHOW);


		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!AfxGetApp()->PumpMessage())
			{
				::PostQuitMessage(0);
				break;
			}
		}

		AfxGetMainWnd()->EnableWindow(FALSE);

		pWaifu2xThread = AfxBeginThread(waifu2xThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		if (pWaifu2xThread)
		{
			pWaifu2xThread->m_pMainWnd = this;
			pWaifu2xThread->m_bAutoDelete = TRUE;
			// スレッド処理の開始
			pWaifu2xThread->ResumeThread();
		}
		WaitForSingleObject(pWaifu2xThread->m_hThread, INFINITE);

		waitDlg->DestroyWindow();
		SAFE_DELETE(waitDlg);
		AfxGetMainWnd()->EnableWindow(TRUE);

		if (Utility->DeleteDirectory(_T(".\\Resources\\tmp"))) {
			OutputDebugString(_T("DeleteDirectory successed.\n"));
		}
		else {
			OutputDebugString(_T("DeleteDirectory failed.\n"));
		}
		SAFE_FREE(InPathT);

		CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
		CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
		CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
		CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
		CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
		button1->EnableWindow(FALSE);
		button2->EnableWindow(FALSE);
		button3->EnableWindow(FALSE);
		button4->EnableWindow(FALSE);
		button5->EnableWindow(FALSE);

		if (find.FindFile(OutPathT)) {
			SAFE_FREE(OutPathT);
			OutPath.Empty();
			MessageBox(_T("変換が完了しました。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			return;
		}
		else {
			SAFE_FREE(OutPathT);
			OutPath.Empty();
			MessageBox(_T("変換に失敗しました。"), _T("エラー"), MB_ICONERROR | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			return;
		}
	}
}


void Cwaifu2xncnnvulkanDlg::OnBnClickedButton2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	wchar_t Drive[5], Dir[MAX_PATH], Name[MAX_PATH], Ext[MAX_PATH];
	UINT ext;
	CString EXT;
	OutputDebugString(_T("Button clicked: button2\n"));
	if (IMAGEPATH_M.IsEmpty() == TRUE)
	{
		MessageBox(_T("画像フォルダが選択されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else if (waifu2x_param.IsEmpty() == TRUE)
	{
		MessageBox(_T("変換設定が適切に設定されていません。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	else
	{
		ext = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x0005", INFINITE, L".\\settings.ini");
		if (ext == 0) {
			EXT = L"JPG";
		}
		else if (ext == 1) {
			EXT = L"PNG";
		}
		else if (ext == 2) {
			EXT = L"WEBP";
		}

		_tsplitpath_s(IMAGEPATH_M, Drive, Dir, Name, Ext);
		CString TDir = Dir;
		CString TDrive = Drive;
		CString TName = Name;
		CString TIMAGEPATH = TDrive + TDir + TName;
		Utility->checkExistenceOfFolder(".\\Resources\\tmp");
		MSG msg;
		CFileFind find;
		CString outDir = L"\\Resources\\tmp\\";
		CString currentPath = CURRENT_PATH + outDir;
		CString InPath = CURRENT_PATH + _T("\\Resources\\tmp\\") + TName;
		CString OutPath;

		wchar_t name[MAX_PATH]{}, dir[MAX_PATH]{};
		BROWSEINFO  binfo{};
		LPITEMIDLIST idlist;

		binfo.hwndOwner = NULL;
		binfo.pidlRoot = NULL;
		binfo.pszDisplayName = (LPWSTR)name;
		binfo.lpszTitle = L"出力フォルダの選択";
		binfo.ulFlags = BIF_RETURNONLYFSDIRS;
		binfo.lpfn = NULL;
		binfo.lParam = 0;
		binfo.iImage = 0;

		if ((idlist = SHBrowseForFolder(&binfo)) == NULL)
		{
			MessageBox(_T("キャンセルされました。"), _T("キャンセル"), MB_ICONWARNING | MB_OK);
			CoTaskMemFree(idlist);
			DELETEDIALOG* delDlg = new DELETEDIALOG;
			delDlg->Create(IDD_DELETEDIALOG);
			delDlg->ShowWindow(SW_SHOW);
			while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
				if (!AfxGetApp()->PumpMessage())
				{
					::PostQuitMessage(0);
					break;
				}
			}
			SHFILEOPSTRUCT tSHDeleteFile{};
			tSHDeleteFile.hwnd = ::GetDesktopWindow();
			tSHDeleteFile.wFunc = FO_DELETE;
			tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
			tSHDeleteFile.fAnyOperationsAborted = TRUE;
			tSHDeleteFile.hNameMappings = NULL;
			tSHDeleteFile.lpszProgressTitle = L"";

			// パス名のCStringの末尾に\0をつけて設定  
			currentPath += "?";
			currentPath.SetAt(currentPath.GetLength() - 1, NULL);
			tSHDeleteFile.pFrom = currentPath;
			tSHDeleteFile.pTo = NULL;
			SHFileOperation(&tSHDeleteFile);
			WaitForSingleObject(&tSHDeleteFile, INFINITE);
			delDlg->DestroyWindow();
			SAFE_DELETE(delDlg);
			CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
			CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
			CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
			CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
			CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
			button1->EnableWindow(FALSE);
			button2->EnableWindow(FALSE);
			button3->EnableWindow(FALSE);
			button4->EnableWindow(FALSE);
			button5->EnableWindow(FALSE);
			this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			return;
		}
		else
		{
			SHGetPathFromIDList(idlist, (LPWSTR)dir);

			if (PathIsDirectoryEmpty((LPCTSTR)dir)) {
				OutPath = (LPCTSTR)dir;
				MultipleOutPath = (LPCTSTR)dir;
				CoTaskMemFree(idlist);
			}
			else {
				intptr_t selectbox;
				selectbox = MessageBox(_T("指定されたディレクトリには既に他のファイルが存在します。\nこのまま画像ファイルを変換した場合、指定ディレクトリ内のファイルは\nすべて失われます。続行しますか？"), _T("確認"), MB_ICONWARNING | MB_OKCANCEL);
				if (selectbox == IDOK) {
					OutPath = (LPCTSTR)dir;
					MultipleOutPath = (LPCTSTR)dir;
					CString del = _T("\\*.*");
					del = OutPath + del;

					DELETEDIALOG* delDlg = new DELETEDIALOG;
					delDlg->Create(IDD_DELETEDIALOG);
					delDlg->ShowWindow(SW_SHOW);
					while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
						if (!AfxGetApp()->PumpMessage())
						{
							::PostQuitMessage(0);
							break;
						}
					}
					SHFILEOPSTRUCT tSHDeleteFile{};
					tSHDeleteFile.hwnd = ::GetDesktopWindow();
					tSHDeleteFile.wFunc = FO_DELETE;
					tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
					tSHDeleteFile.fAnyOperationsAborted = TRUE;
					tSHDeleteFile.hNameMappings = NULL;
					tSHDeleteFile.lpszProgressTitle = L"";

					// パス名のCStringの末尾に\0をつけて設定  
					del += "?";
					del.SetAt(del.GetLength() - 1, NULL);
					tSHDeleteFile.pFrom = del;
					tSHDeleteFile.pTo = NULL;
					SHFileOperation(&tSHDeleteFile);
					WaitForSingleObject(&tSHDeleteFile, INFINITE);

					delDlg->DestroyWindow();
					SAFE_DELETE(delDlg);
					del.Empty();

					CoTaskMemFree(idlist);
				}
				else if (selectbox == IDCANCEL) {
					CoTaskMemFree(idlist);
					return;
				}
			}
		}

		SHFILEOPSTRUCT tSHMoveFile{};
		tSHMoveFile.hwnd = ::GetDesktopWindow();
		tSHMoveFile.wFunc = FO_COPY;
		tSHMoveFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
		tSHMoveFile.fAnyOperationsAborted = TRUE;
		tSHMoveFile.hNameMappings = NULL;
		tSHMoveFile.lpszProgressTitle = L"";

		// パス名のCStringの末尾に\0をつけて設定  
		TIMAGEPATH += "0";
		TIMAGEPATH.SetAt(TIMAGEPATH.GetLength() - 1, NULL);
		InPath += "0";
		InPath.SetAt(InPath.GetLength() - 1, NULL);
		tSHMoveFile.pFrom = TIMAGEPATH;
		tSHMoveFile.pTo = InPath;
		SHFileOperation(&tSHMoveFile);
		WaitForSingleObject(&tSHMoveFile, INFINITE);

		CString PRM = CURRENT_PATH + _T("\\Resources\\image2png ") + IMAGEPATH_M;
		TCHAR* PRMT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
		if (NULL == PRMT) {
			perror("can not malloc");
			OutputDebugString(_T("TCHAR syntax (PRMT) malloc failed.\n"));
			return;
		}
		else {
			ZeroMemory(&PRMT[0], 256);
			_tcscpy_s(&PRMT[0], 256, PRM);
		}
		
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOW;
		::CreateProcess(NULL, PRMT, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		CloseHandle(pi.hThread);
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		SAFE_FREE(PRMT);
		SetCurrentDirectory(CURRENT_PATH);

		if (_tcscmp(Waifu2x->waifu2x_prm, _T("")) != 0) {
			waifu2x_param = Waifu2x->waifu2x_prm;
		}
		TCHAR* OutPathT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
		if (NULL == OutPathT) {
			perror("can not malloc");
			OutputDebugString(_T("TCHAR syntax (OutPathT) malloc failed.\n"));
			return;
		}
		else {
			ZeroMemory(&OutPathT[0], 256);
			_tcscpy_s(&OutPathT[0], 256, OutPath);
			Utility->AfxReplaceStr(waifu2x_param, _T("$OutFile"), OutPathT);
		}
		TCHAR* InPathT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
		if (NULL == InPathT) {
			perror("can not malloc");
			OutputDebugString(_T("TCHAR syntax (InPathT) malloc failed.\n"));
			return;
		}
		else {
			ZeroMemory(&InPathT[0], 256);
			_tcscpy_s(&InPathT[0], 256, InPath);
			Utility->AfxReplaceStr(waifu2x_param, _T("$InFile"), InPathT);
		}

		/*pWaifu2xThread = AfxBeginThread(waifu2xThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		ASSERT(pWaifu2xThread);
		if (pWaifu2xThread)
		{
			pWaifu2xThread->m_pMainWnd = this;
			pWaifu2xThread->m_bAutoDelete = TRUE;
			// スレッド処理の開始
			pWaifu2xThread->ResumeThread();
		}
		WaitForSingleObject(pWaifu2xThread->m_hThread, INFINITE);*/

		FILECOUNT = Utility->GetDirectoryFileCount(IMAGEPATH_M.GetString());
		ImageUpScaleFlag = TRUE;
		pWaifu2xCountThread = AfxBeginThread(waifu2xCountThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		if (pWaifu2xCountThread)
		{
			pWaifu2xCountThread->m_pMainWnd = this;
			pWaifu2xCountThread->m_bAutoDelete = TRUE;
			pWaifu2xCountThread->ResumeThread();
		}
		CThreadWaitDlgThread* pThread = DYNAMIC_DOWNCAST(CThreadWaitDlgThread, AfxBeginThread(RUNTIME_CLASS(CThreadWaitDlgThread), 0, 0, CREATE_SUSPENDED));
		pThread->m_bAutoDelete = TRUE;
		pThread->ResumeThread();
		while (pThread->m_Dlg.m_hWnd == 0) {
			Sleep(0);
		}

		while (ProgressThreadFlag != TRUE) {
		}
		while (pThread->m_Dlg.m_hWnd) {
		}
		ProgressThreadFlag = FALSE;

		if (SuspendFlag == TRUE) {
			while (pThread->m_Dlg.m_hWnd) {
			}
			SuspendFlag = FALSE;
			Utility->DeleteDirectory(CURRENT_PATH + _T("\\Resources\\tmp"));
			Utility->DeleteDirectory(OutPathT);
			MessageBox(_T("作業が中止されました。\n保存先に指定したディレクトリは削除されました。"), _T("完了"), MB_ICONWARNING | MB_OK);
			CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
			CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
			CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
			CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
			CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
			button1->EnableWindow(FALSE);
			button2->EnableWindow(FALSE);
			button3->EnableWindow(FALSE);
			button4->EnableWindow(FALSE);
			button5->EnableWindow(FALSE);
			this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			ImageUpScaleFlag = FALSE;
			SAFE_FREE(InPathT);
			SAFE_FREE(OutPathT);
			return;
		}
		ImageUpScaleFlag = FALSE;
		WaitForSingleObject(&pWaifu2xCountThread->m_hThread, INFINITE);

		if (EXT == L"JPG") {
			CString jrenameCom = L"/c ren *.jpg.jpg *.\nexit 0";
			CString prenameCom = L"/c ren *.png.jpg *.\nexit 0";
			CString prenameComext = L"/c ren *.png *.jpg\nexit 0";
			CString wrenameCom = L"/c ren *.webp.jpg *.\nexit 0";
			CString wrenameComext = L"/c ren *.webp *.jpg\nexit 0";
			ShellExecute(NULL, L"open", L"cmd.exe", jrenameCom, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", prenameComext, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", wrenameCom, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", wrenameComext, OutPathT, SW_HIDE);
		}
		else if (EXT == L"PNG") {
			CString jrenameCom = L"/c ren *.jpg.png *.\nexit 0";
			CString jrenameComext = L"/c ren *.jpg *.png\nexit 0";
			CString prenameCom = L"/c ren *.png.png *.\nexit 0";
			CString wrenameCom = L"/c ren *.webp.png *.\nexit 0";
			CString wrenameComext = L"/c ren *.webp *.png\nexit 0";
			ShellExecute(NULL, L"open", L"cmd.exe", jrenameCom, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", jrenameComext, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", wrenameCom, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", wrenameComext, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, OutPathT, SW_HIDE);
		}
		else if (EXT == L"WEBP") {
			CString jrenameCom = L"/c ren *.jpg.webp *.\nexit 0";
			CString jrenameComext = L"/c ren *.jpg *.webp\nexit 0";
			CString prenameCom = L"/c ren *.png.webp *.\nexit 0";
			CString prenameComext = L"/c ren *.png *.webp\nexit 0";
			CString wrenameCom = L"/c ren *.webp.webp *.\nexit 0";
			ShellExecute(NULL, L"open", L"cmd.exe", jrenameCom, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", jrenameComext, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", prenameCom, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", prenameComext, OutPathT, SW_HIDE);
			ShellExecute(NULL, L"open", L"cmd.exe", wrenameCom, OutPathT, SW_HIDE);
		}

		DELETEDIALOG* delDlg = new DELETEDIALOG;
		delDlg->Create(IDD_DELETEDIALOG);
		delDlg->ShowWindow(SW_SHOW);
		while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
			if (!AfxGetApp()->PumpMessage())
			{
				::PostQuitMessage(0);
				break;
			}
		}
		SHFILEOPSTRUCT tSHDeleteFile{};
		tSHDeleteFile.hwnd = ::GetDesktopWindow();
		tSHDeleteFile.wFunc = FO_DELETE;
		tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
		tSHDeleteFile.fAnyOperationsAborted = TRUE;
		tSHDeleteFile.hNameMappings = NULL;
		tSHDeleteFile.lpszProgressTitle = L"";

		// パス名のCStringの末尾に\0をつけて設定  
		currentPath += "?";
		currentPath.SetAt(currentPath.GetLength() - 1, NULL);
		tSHDeleteFile.pFrom = currentPath;
		tSHDeleteFile.pTo = NULL;
		SHFileOperation(&tSHDeleteFile);
		WaitForSingleObject(&tSHDeleteFile, INFINITE);

		delDlg->DestroyWindow();
		SAFE_DELETE(delDlg);

		if (Utility->DeleteDirectory(_T(".\\Resources\\tmp"))) {
			OutputDebugString(_T("DeleteDirectory successed.\n"));
		}
		else {
			OutputDebugString(_T("DeleteDirectory failed.\n"));
		}
		SAFE_FREE(InPathT);

		CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
		CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
		CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
		CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON2);
		CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON6);
		button1->EnableWindow(FALSE);
		button2->EnableWindow(FALSE);
		button3->EnableWindow(FALSE);
		button4->EnableWindow(FALSE);
		button5->EnableWindow(FALSE);

		if (PathIsDirectoryEmpty(OutPathT)) {
			SAFE_FREE(OutPathT);
			MessageBox(_T("変換に失敗しました。"), _T("エラー"), MB_ICONERROR | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			return;
		}
		else {
			SAFE_FREE(OutPathT);
			MessageBox(_T("変換が完了しました。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_File.SetWindowText(DEFAULT_TEXT);
			this->xv_Static_FilePath.SetWindowText(DEFAULT_TEXT);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			return;
		}
	}
}


void Cwaifu2xncnnvulkanDlg::OnReadImage()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnReadImage\n"));
	POPUPDlg* POPUP = new POPUPDlg;
	CString filter("画像ファイル (*.tif,*.tiff,*.jpg,*.jpeg,*.png,*.bmp,*.gif,*.webp)|*.tif;*.tiff;*.jpg;*.jpeg;*.png;*.bmp;*.gif;*.webp;||");
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_CREATEPROMPT | OFN_HIDEREADONLY, filter);
	CFileFind find;
	wchar_t Drive[5], Dir[MAX_PATH], Name[MAX_PATH], Ext[MAX_PATH];
	intptr_t filedialog = selDlg.DoModal();
	if (filedialog == IDOK)
	{
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		IMAGEPATH = selDlg.GetPathName();
		SetCurrentDirectory(CURRENT_PATH);
		_tsplitpath_s(IMAGEPATH, Drive, Dir, Name, Ext);
		CStringW NameC = Name;
		CString Num = NameC + Ext;
		DWORD fsize = Utility->GetFileSizeStat(IMAGEPATH);
		CString fsizeCS;
		fsizeCS.Format(_T("%u"), fsize);
		Num.MakeUpper();
		CString NumFinal = Num + _T(" ( ") + fsizeCS + _T(" バイト)");
		this->xv_Static_ReadStatus.SetWindowText(READED_TEXT);
		this->xv_Static_File.SetWindowText(NumFinal);
		this->xv_Static_FilePath.SetWindowText(IMAGEPATH);
		CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
		static1->InvalidateRect(NULL, 1);

		CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
		CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
		CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
		CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON6);
		CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON2);
		button1->EnableWindow(FALSE);
		button2->EnableWindow(FALSE);
		button3->EnableWindow(FALSE);
		button4->EnableWindow(TRUE);
		button5->EnableWindow(FALSE);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
	else if (filedialog == IDCANCEL) {
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
	else {
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
}


void Cwaifu2xncnnvulkanDlg::OnReadImageMulti()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnReadImageMulti\n"));
	POPUPDlg* POPUP = new POPUPDlg;
	wchar_t name[MAX_PATH]{}, dir[MAX_PATH]{};
	CFileFind find;
	wchar_t Drive[5], Dir[MAX_PATH], Name[MAX_PATH], Ext[MAX_PATH];
	BROWSEINFO  binfo{};
	LPITEMIDLIST idlist;

	binfo.hwndOwner = NULL;
	binfo.pidlRoot = NULL;
	binfo.pszDisplayName = (LPWSTR)name;
	binfo.lpszTitle = L"フォルダの選択";
	binfo.ulFlags = BIF_RETURNONLYFSDIRS;
	binfo.lpfn = NULL;
	binfo.lParam = 0;
	binfo.iImage = 0;

	if ((idlist = SHBrowseForFolder(&binfo)) == NULL)
	{
		MessageBox(_T("キャンセルされました。"), _T("キャンセル"), MB_ICONWARNING | MB_OK);
		CoTaskMemFree(idlist);
		SAFE_DELETE(POPUP);
	}
	else
	{
		SHGetPathFromIDList(idlist, (LPWSTR)dir);
		if (PathIsDirectoryEmpty((LPCTSTR)dir)) {
			MessageBox(_T("フォルダ内にファイルが見つかりません"), _T("エラー"), MB_ICONERROR | MB_OK);
			CoTaskMemFree(idlist);
			SAFE_DELETE(POPUP);
			return;
		}
		else {
			IMAGEPATH_M = (LPCTSTR)dir;
			CoTaskMemFree(idlist);
		}
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		SetCurrentDirectory(CURRENT_PATH);
		_tsplitpath_s(IMAGEPATH_M, Drive, Dir, Name, Ext);
		CStringW NameC = Name;
		CString Num = NameC + Ext;
		DWORD fsize = Utility->GetFileSizeStat(IMAGEPATH_M);
		CString fsizeCS;
		fsizeCS.Format(_T("%u"), fsize);
		Num.MakeUpper();
		CString NumFinal = Num + _T(" (ディレクトリ)");
		this->xv_Static_ReadStatus.SetWindowText(READED_TEXT);
		this->xv_Static_File.SetWindowText(NumFinal);
		this->xv_Static_FilePath.SetWindowText(IMAGEPATH_M);
		CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
		static1->InvalidateRect(NULL, 1);

		CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
		CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
		CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
		CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON6);
		CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON2);
		button1->EnableWindow(FALSE);
		button2->EnableWindow(FALSE);
		button3->EnableWindow(FALSE);
		button4->EnableWindow(FALSE);
		button5->EnableWindow(TRUE);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
}


void Cwaifu2xncnnvulkanDlg::OnReadVideo()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnReadVideo\n"));
	POPUPDlg* POPUP = new POPUPDlg;
	CString filter("動画ファイル (*.avi,*.mp4,*.wmv,*.webm,*.mkv,*.mov)|*.avi;*.mp4;*.wmv;*.webm;*.mkv;*.mov;||");
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	CFileFind find;
	wchar_t Drive[5], Dir[MAX_PATH], Name[MAX_PATH], Ext[MAX_PATH];
	INT_PTR filedialog = selDlg.DoModal();
	if (filedialog == IDOK)
	{
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		VIDEOPATH = selDlg.GetPathName();
		SetCurrentDirectory(CURRENT_PATH);
		_tsplitpath_s(VIDEOPATH, Drive, Dir, Name, Ext);
		CStringW NameC = Name;
		CString Num = NameC + Ext;
		DWORD fsize = Utility->GetFileSizeStat(VIDEOPATH);
		CString fsizeCS;
		fsizeCS.Format(_T("%u"), fsize);
		Num.MakeUpper();
		CString NumFinal = Num + _T(" ( ") + fsizeCS + _T(" バイト)");
		this->xv_Static_ReadStatus.SetWindowText(READED_TEXT);
		this->xv_Static_File.SetWindowText(NumFinal);
		this->xv_Static_FilePath.SetWindowText(VIDEOPATH);
		CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
		static1->InvalidateRect(NULL, 1);

		CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
		CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
		CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
		CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON6);
		CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON2);
		button1->EnableWindow(TRUE);
		button2->EnableWindow(FALSE);
		button3->EnableWindow(FALSE);
		button4->EnableWindow(FALSE);
		button5->EnableWindow(FALSE);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
	else if (filedialog == IDCANCEL) {
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
	else {
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
}


void Cwaifu2xncnnvulkanDlg::OnFileAlldelete()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnFileAlldelete\n"));
	CString outDir = _T("\\Resources\\takeout\\");
	CString currentPath = CURRENT_PATH + outDir;

	MSG msg;
	DELETEDIALOG* delDlg = new DELETEDIALOG;
	delDlg->Create(IDD_DELETEDIALOG);
	delDlg->ShowWindow(SW_SHOW);
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage())
		{
			::PostQuitMessage(0);
			break;
		}
	}

	SHFILEOPSTRUCT tSHDeleteFile{};
	tSHDeleteFile.hwnd = ::GetDesktopWindow();
	tSHDeleteFile.wFunc = FO_DELETE;
	tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
	tSHDeleteFile.fAnyOperationsAborted = TRUE;
	tSHDeleteFile.hNameMappings = NULL;
	tSHDeleteFile.lpszProgressTitle = L"";

	// パス名のCStringの末尾に\0をつけて設定  
	currentPath += "?";
	currentPath.SetAt(currentPath.GetLength() - 1, NULL);
	tSHDeleteFile.pFrom = currentPath;
	tSHDeleteFile.pTo = NULL;
	SHFileOperation(&tSHDeleteFile);
	WaitForSingleObject(&tSHDeleteFile, INFINITE);

	if (Utility->DeleteDirectory(_T(".\\Resources\\tmp"))) {
		OutputDebugString(_T("DeleteDirectory successed.\n"));
	}
	else {
		OutputDebugString(_T("DeleteDirectory failed.\n"));
	}

	delDlg->DestroyWindow();
	SAFE_DELETE(delDlg);
}


void Cwaifu2xncnnvulkanDlg::OnVideoresize()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnVideoresize\n"));
	POPUPDlg* POPUP = new POPUPDlg;
	CFileFind find;
	CString filter("動画ファイル (*.avi,*.mp4,*.wmv,*.webm,*.mkv,*.mov)|*.avi;*.mp4;*.wmv;*.webm;*.mkv;*.mov;||");
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	INT_PTR filedialog = selDlg.DoModal();
	if (filedialog == IDOK) {
		CString RV = selDlg.GetPathName();
		CString filter("動画ファイル (*.avi,*.mp4,*.wmv,*.webm,*.mkv,*.mov)|*.avi;*.mp4;*.wmv;*.webm;*.mkv;*.mov;||");
		CFileDialog selDlgs(FALSE, _T("mp4"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
		INT_PTR file = selDlgs.DoModal();
		if (file == IDOK) {
			CString OutPath = selDlgs.GetPathName();
			TCHAR* lpPath = (TCHAR*)malloc(sizeof(TCHAR) * 512); // Convert CString to TCHAR.
			if (NULL == lpPath) {
				perror("can not malloc");
				OutputDebugString(_T("TCHAR syntax (lpPath) malloc failed.\n"));
				return;
			}
			else {
				ZeroMemory(&lpPath[0], 512);
				_tcscpy_s(&lpPath[0], 512, OutPath);
			}

			VIDEORESIZE* DIALOG = new VIDEORESIZE;
			intptr_t ret = DIALOG->DoModal();
			if (ret == IDOK) {
				CString PRM = DIALOG->FINAL;
				Utility->AfxReplaceStr(PRM, L"ffmpeg", L".\\Resources\\ffmpeg\\ffmpeg.exe");
				Utility->AfxReplaceStr(PRM, L"$InFile", RV);
				Utility->AfxReplaceStr(PRM, L"$OutFile", lpPath);
				TCHAR* ffmpegcmdT = (TCHAR*)malloc(sizeof(TCHAR) * 512); // Convert CString to TCHAR.
				if (NULL == ffmpegcmdT) {
					perror("can not malloc");
					OutputDebugString(_T("TCHAR syntax (ffmpegcmdT) malloc failed.\n"));
					return;
				}
				else {
					ZeroMemory(&ffmpegcmdT[0], 512);
					_tcscpy_s(&ffmpegcmdT[0], 512, PRM);
				}

				WAITDIALOG* waitDlg = new WAITDIALOG;

				waitDlg->Create(IDD_WAITDIALOG);
				waitDlg->ShowWindow(SW_SHOW);
				MSG msg;
				while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
					if (!AfxGetApp()->PumpMessage())
					{
						::PostQuitMessage(0);
						break;
					}
				}

				STARTUPINFO si/* = { sizeof(STARTUPINFO) }*/;
				memset(&si, 0, sizeof(STARTUPINFO));
				PROCESS_INFORMATION pi;
				memset(&pi, 0, sizeof(PROCESS_INFORMATION));
				si.dwFlags = STARTF_USESHOWWINDOW;
				si.wShowWindow = SW_SHOW;
				::CreateProcess(NULL, ffmpegcmdT, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
				CloseHandle(pi.hThread);
				WaitForSingleObject(pi.hProcess, INFINITE);
				CloseHandle(pi.hProcess);

				waitDlg->DestroyWindow();
				SAFE_DELETE(waitDlg);
				SAFE_FREE(ffmpegcmdT);
				if (find.FindFile(lpPath)) {
					SAFE_FREE(lpPath);
					SAFE_DELETE(DIALOG);
					MessageBox(_T("解像度の変更が完了しました。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
					return;
				}
				else {
					SAFE_FREE(lpPath);
					SAFE_DELETE(DIALOG);
					MessageBox(_T("解像度の変更に失敗しました。"), _T("エラー"), MB_ICONERROR | MB_OK);
					return;
				}
			}
			else if (ret == IDCANCEL) {
				MessageBox(_T("キャンセルされました。"), _T("キャンセル"), MB_ICONWARNING | MB_OK);
				SAFE_DELETE(DIALOG);
				return;
			}
			else {
				SAFE_DELETE(DIALOG);
				return;
			}
		}
		else if (file == IDCANCEL) {
			MessageBox(_T("キャンセルされました。"), _T("キャンセル"), MB_ICONWARNING | MB_OK);
			return;
		}
	}
	else if (filedialog == IDCANCEL) {
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
	else {
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
}


void Cwaifu2xncnnvulkanDlg::OnVideoaudioexport()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnVideoaudioexport\n"));
	POPUPDlg* POPUP = new POPUPDlg;
	CFileFind find;
	CString filter("動画ファイル (*.avi,*.mp4,*.wmv,*.webm,*.mkv,*.mov)|*.avi;*.mp4;*.wmv;*.webm;*.mkv;*.mov;||");
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	intptr_t filedialog = selDlg.DoModal();
	if (filedialog == IDOK) {
		CString RV = selDlg.GetPathName();
		CString filter("音声ファイル (*.wav,*.mp3,*.m4a,*.wma,*.oga,*.ogg,*.opus)|*.wav;*.mp3;*.m4a;*.wma;*.oga;*.ogg;*.opus;||");
		CFileDialog selDlgs(FALSE, _T("wav"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);
		intptr_t file = selDlgs.DoModal();
		if (file == IDOK) {
			CString OutPath = selDlgs.GetPathName();
			TCHAR* lpPath = (TCHAR*)malloc(sizeof(TCHAR) * 512); // Convert CString to TCHAR.
			if (NULL == lpPath) {
				perror("can not malloc");
				OutputDebugString(_T("TCHAR syntax (lpPath) malloc failed.\n"));
				return;
			}
			else {
				ZeroMemory(&lpPath[0], 512);
				_tcscpy_s(&lpPath[0], 512, OutPath);
			}
			CString PRM;
			PRM = _T(".\\Resources\\ffmpeg\\ffmpeg.exe -i ") + RV + _T(" -vn -acodec copy -y ") + lpPath;
			TCHAR* ffmpegcmdT = (TCHAR*)malloc(sizeof(TCHAR) * 512); // Convert CString to TCHAR.
			if (NULL == ffmpegcmdT) {
				perror("can not malloc");
				OutputDebugString(_T("TCHAR syntax (ffmpegcmdT) malloc failed.\n"));
				return;
			}
			else {
				ZeroMemory(&ffmpegcmdT[0], 512);
				_tcscpy_s(&ffmpegcmdT[0], 512, PRM);
			}

			WAITDIALOG* waitDlg = new WAITDIALOG;

			waitDlg->Create(IDD_WAITDIALOG);
			waitDlg->ShowWindow(SW_SHOW);
			MSG msg;
			while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
				if (!AfxGetApp()->PumpMessage())
				{
					::PostQuitMessage(0);
					break;
				}
			}

			STARTUPINFO si/* = { sizeof(STARTUPINFO) }*/;
			memset(&si, 0, sizeof(STARTUPINFO));
			PROCESS_INFORMATION pi;
			memset(&pi, 0, sizeof(PROCESS_INFORMATION));
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_SHOW;
			::CreateProcess(NULL, ffmpegcmdT, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
			CloseHandle(pi.hThread);
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);

			waitDlg->DestroyWindow();
			SAFE_DELETE(waitDlg);
			SAFE_FREE(ffmpegcmdT);
			if (find.FindFile(lpPath)) {
				SAFE_FREE(lpPath);
				MessageBox(_T("音声の抜き出しが完了しました。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
				SAFE_DELETE(POPUP);
				return;
			}
			else {
				SAFE_FREE(lpPath);
				MessageBox(_T("音声の抜き出しに失敗しました。"), _T("エラー"), MB_ICONERROR | MB_OK);
				SAFE_DELETE(POPUP);
				return;
			}
		}
		else if (file == IDCANCEL) {
			SAFE_DELETE(POPUP);
			return;
		}
		else {
			SAFE_DELETE(POPUP);
			return;
		}
	}
	else if(filedialog == IDCANCEL) {
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
	else {
		POPUP->Create(IDD_POPUP);
		POPUP->ShowWindow(SW_SHOW);
		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);
		return;
	}
}


void Cwaifu2xncnnvulkanDlg::OnEndApp()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnEndApp\n"));
	AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
	AfxGetMainWnd()->DestroyWindow();
	PostQuitMessage(0);
}


void Cwaifu2xncnnvulkanDlg::OnSettings()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnSettings\n"));
	SETTINGSDIALOG* SETTINGSDlg = new SETTINGSDIALOG;
	INT_PTR ret = SETTINGSDlg->DoModal();
	if (ret == IDOK) {
		waifu2x_param = SETTINGSDlg->FinalSettings;
		GetPrivateProfileString(L"WAIFU2X_SETTINGS", L"0x20FF", NULL, Waifu2x->waifu2x_prm, 256, L".\\settings.ini");
		if (Waifu2x->waifu2x_prm != _T("")) {
			waifu2x_param = Waifu2x->waifu2x_prm;
		}
		else {
			waifu2x_param = _T("");
		}
		SAFE_DELETE(SETTINGSDlg);
		return;
	}
	else {
		SAFE_DELETE(SETTINGSDlg);
		return;
	}
}


void Cwaifu2xncnnvulkanDlg::OnAboutWaifu2x()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnAboutWaifu2x\n"));
	CAboutDlg About;
	About.DoModal();
}

void Cwaifu2xncnnvulkanDlg::DoEvents()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		if (!AfxGetApp()->PumpMessage())
			return;
	}
}

UINT Cwaifu2xncnnvulkanDlg::waifu2xThread(LPVOID pParam) {
	Cwaifu2xncnnvulkanDlg* pFileView = dynamic_cast<Cwaifu2xncnnvulkanDlg*>(reinterpret_cast<CWnd*>(pParam));
	if (pFileView)
	{
		pFileView->waifu2xThread();
	}
	return 0;
}

UINT Cwaifu2xncnnvulkanDlg::FFmpegThread(LPVOID pParam) {
	Cwaifu2xncnnvulkanDlg* pFileView = dynamic_cast<Cwaifu2xncnnvulkanDlg*>(reinterpret_cast<CWnd*>(pParam));
	if (pFileView)
	{
		pFileView->FFmpegThread();
	}
	return 0;
}


UINT Cwaifu2xncnnvulkanDlg::waifu2xCountThread(LPVOID pParam) {
	Cwaifu2xncnnvulkanDlg* pFileView = dynamic_cast<Cwaifu2xncnnvulkanDlg*>(reinterpret_cast<CWnd*>(pParam));
	if (pFileView)
	{
		pFileView->waifu2xCountThread();
	}
	return 0;
}

UINT Cwaifu2xncnnvulkanDlg::waifu2xCountDlgThread(LPVOID pParam) {
	Cwaifu2xncnnvulkanDlg* pFileView = dynamic_cast<Cwaifu2xncnnvulkanDlg*>(reinterpret_cast<CWnd*>(pParam));
	if (pFileView)
	{
		pFileView->waifu2xCountDlgThread();
	}
	return 0;
}

UINT Cwaifu2xncnnvulkanDlg::DLThread(LPVOID pParam)
{
	Cwaifu2xncnnvulkanDlg* pFileView = dynamic_cast<Cwaifu2xncnnvulkanDlg*>(reinterpret_cast<CWnd*>(pParam));
	if (pFileView)
	{
		pFileView->DLThread();
	}
	return 0;
}

UINT Cwaifu2xncnnvulkanDlg::DLCountThread(LPVOID pParam)
{
	Cwaifu2xncnnvulkanDlg* pFileView = dynamic_cast<Cwaifu2xncnnvulkanDlg*>(reinterpret_cast<CWnd*>(pParam));
	if (pFileView)
	{
		pFileView->DLCountThread();
	}
	return 0;
}

UINT Cwaifu2xncnnvulkanDlg::DeleteThread(LPVOID pParam)
{
	Cwaifu2xncnnvulkanDlg* pFileView = dynamic_cast<Cwaifu2xncnnvulkanDlg*>(reinterpret_cast<CWnd*>(pParam));
	if (pFileView)
	{
		pFileView->DeleteThread();
	}
	return 0;
}

void Cwaifu2xncnnvulkanDlg::waifu2xThread()
{
	OutputDebugString(_T("waifu2x Thread Started.\n"));

	Utility->AfxReplaceStr(waifu2x_param, L"waifu2x-ncnn-vulkan -i", CURRENT_PATH + L"\\Resources\\waifu2x-ncnn-vulkan\\waifu2x-ncnn-vulkan -i");
	if (NULL == lpPath) {
		perror("can not malloc");
		OutputDebugString(_T("TCHAR syntax (lpPath) malloc failed.\n"));
		return;
	}
	else {
		ZeroMemory(&lpPath[0], 512);
		_tcscpy_s(&lpPath[0], 512, waifu2x_param);
	}

	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	::CreateProcess(NULL, lpPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	OutputDebugString(_T("Process created.\n"));

	CloseHandle(pi.hThread);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	SetCurrentDirectory(CURRENT_PATH);

	SAFE_FREE(lpPath);

	this->PostMessage(WM_USER_COMPLETE_WAIFU2X_THREAD);
	Waifu2xThreadFlag = TRUE;
	OutputDebugString(_T("waifu2x Thread Ended.\n"));
}

void Cwaifu2xncnnvulkanDlg::FFmpegThread()
{
	OutputDebugString(_T("FFmpeg Thread Started.\n"));
	UINT pseta, psetv;
	wchar_t aext[10]{}, aname[MAX_PATH]{}, audiopath[MAX_PATH]{}, vext[10]{}, vname[MAX_PATH]{}, videopath[MAX_PATH]{};
	GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1FFF", NULL, (LPWSTR)aext, 10, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2000", NULL, (LPWSTR)aname, MAX_PATH, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2001", NULL, (LPWSTR)audiopath, MAX_PATH, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1FFF", NULL, (LPWSTR)vext, 10, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2000", NULL, (LPWSTR)vname, MAX_PATH, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2001", NULL, (LPWSTR)videopath, MAX_PATH, L".\\settings.ini");
	pseta = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini");
	psetv = GetPrivateProfileInt(L"FFMPEG_VIDEO_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini");

	CString outDir = L"\\Resources\\takeout\\";
	CString currentPath = CURRENT_PATH + outDir;

	CString ffmpegDir = L".\\Resources\\ffmpeg\\ffmpeg.exe";
	CString InFile = VIDEOPATH;
	CString audioOut;
	CString imageOut;

	if (pseta == 0) {
		audioOut = currentPath + L"_temp-project\\streams\\audio.wav";
	}
	else if (pseta == 1) {
		if (_tcscmp(audiopath, _T("")) != 0) {
			CString ap = (LPCTSTR)audiopath;
			ap += _T("\\");
			if (_tcscmp(aname, _T("")) != 0) {
				CString an = (LPCTSTR)aname;
				audioOut = ap + an + (LPCTSTR)aext;
			}
		}
	}
	if (psetv == 0) {
		imageOut = currentPath + L"_temp-project\\image-frames\\image-%09d.png";
	}
	else if (psetv == 1) {
		if (_tcscmp(videopath, _T("")) != 0) {
			CString vp = (LPCTSTR)videopath;
			vp += _T("\\");
			if (_tcscmp(vname, _T("")) != 0) {
				CString vn = (LPCTSTR)vname;
				imageOut = vp + vn + (LPCTSTR)vext;
			}
		}
	}

	if (_tcscmp(Waifu2x->FFmpeg_video_prm, _T("")) != 0) {
		ffmpeg_videoparam = Waifu2x->FFmpeg_video_prm;
	}
	if (_tcscmp(Waifu2x->FFmpeg_audio_prm, _T("")) != 0) {
		ffmpeg_audioparam = Waifu2x->FFmpeg_audio_prm;
	}

	Utility->AfxReplaceStr(ffmpeg_audioparam, L"ffmpeg", ffmpegDir);
	Utility->AfxReplaceStr(ffmpeg_audioparam, L"$InFile", InFile);
	Utility->AfxReplaceStr(ffmpeg_audioparam, L"$OutFile", audioOut);
	Utility->AfxReplaceStr(ffmpeg_videoparam, L"ffmpeg", ffmpegDir);
	Utility->AfxReplaceStr(ffmpeg_videoparam, L"$InFile", InFile);
	Utility->AfxReplaceStr(ffmpeg_videoparam, L"$OutFile", imageOut);
	TCHAR* audioPathT = (TCHAR*)malloc(sizeof(TCHAR) * 512); // Convert CString to TCHAR.
	if (NULL == audioPathT) {
		perror("can not malloc");
		OutputDebugString(_T("TCHAR syntax (audiopathT) malloc failed.\n"));
		return;
	}
	else {
		ZeroMemory(&audioPathT[0], 512);
		_tcscpy_s(&audioPathT[0], 512, ffmpeg_audioparam);
	}
	TCHAR* imagePathT = (TCHAR*)malloc(sizeof(TCHAR) * 512); // Convert CString to TCHAR.
	if (NULL == imagePathT) {
		perror("can not malloc");
		OutputDebugString(_T("TCHAR syntax (imagepathT) malloc failed.\n"));
		return;
	}
	else {
		ZeroMemory(&imagePathT[0], 512);
		_tcscpy_s(&imagePathT[0], 512, ffmpeg_videoparam);
	}

	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	::CreateProcess(NULL, audioPathT, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	::CreateProcess(NULL, imagePathT, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	SAFE_FREE(audioPathT);
	SAFE_FREE(imagePathT);

	std::wstring fc(currentPath.GetString());
	UPSCALE_PATH = currentPath + _T("_temp-project\\image-frames2x\\");
	FILECOUNT = Utility->GetDirectoryFileCount(fc + _T("_temp-project\\image-frames\\"));

	this->PostMessage(WM_USER_COMPLETE_FFMPEG_THREAD);
	FFmpegThreadFlag = TRUE;
	OutputDebugString(_T("FFmpeg Thread Ended.\n"));
}


void Cwaifu2xncnnvulkanDlg::waifu2xCountThread()
{
	OutputDebugString(_T("Count Thread Started.\n"));

	pWaifu2xThread = AfxBeginThread(waifu2xThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	ASSERT(pWaifu2xThread);
	if (pWaifu2xThread)
	{
		pWaifu2xThread->m_pMainWnd = this;
		pWaifu2xThread->m_bAutoDelete = TRUE;
		pWaifu2xThread->ResumeThread();
	}

	while (Waifu2xThreadFlag != TRUE) {
		if (ImageUpScaleFlag == TRUE) {
			UPSCALE_COUNT = Utility->GetDirectoryFileCount(MultipleOutPath.GetString());
		}
		else {
			UPSCALE_COUNT = Utility->GetDirectoryFileCount(UPSCALE_PATH.GetString());
		}
		if (Waifu2xThreadFlag == TRUE) {
			OutputDebugString(_T("Waifu2xThreadFlag TRUE.\n"));
			Waifu2xThreadFlag = FALSE;
			break;
		}
		if (SuspendFlag == TRUE) {
			break;
		}
		else {
			continue;
		}
	}
	UPSCALE_COUNT = 0;
	ProgressThreadFlag = TRUE;
	if (SuspendFlag == TRUE) {
		this->PostMessage(WM_USER_COMPLETE_WAIFU2X_THREAD);
	}
	this->PostMessage(WM_USER_COMPLETE_COUNT_THREAD);
	OutputDebugString(_T("Count Thread Roop Ended.\n"));
}

void Cwaifu2xncnnvulkanDlg::waifu2xCountDlgThread()
{
	CFrameWnd* pWnd = new CFrameWnd;
	pWnd->Create(NULL, L"CWinThread Test");
	THREADWAITDIALOG* dlg = new THREADWAITDIALOG(CWnd::FromHandle(pWnd->GetSafeHwnd()));
	dlg->DoModal();

	SAFE_DELETE(dlg);
	this->PostMessage(WM_USER_COMPLETE_COUNT_DLG_THREAD);
}

void Cwaifu2xncnnvulkanDlg::DLThread()
{
	OutputDebugString(_T("DLThread Started.\n"));

	bool ret;
	ret = Utility->DownloadFile(DOWNLOAD_URL_STRING, _T(".\\Resources\\ffmpeg.zip"), 4096);

	this->PostMessage(WM_USER_COMPLETE_DOWNLOAD_LOAD_XML);
	DLFlag = TRUE;
	OutputDebugString(_T("DLThread Ended.\n"));
}

void Cwaifu2xncnnvulkanDlg::DLCountThread()
{
	OutputDebugString(_T("DLCountThread Started.\n"));
	while (DLFlag != TRUE) {
		if (DLFlag == TRUE) {
			OutputDebugString(_T("DLFlag TRUE.\n"));
			DLFlag = FALSE;
			break;
		}
		else {
			OutputDebugString(_T("DLFlag FALSE, Rooped.\n"));
			while (DLThreadFlag != TRUE)
			{
				DLCount = Utility->GetFileSizeStat(_T(".\\Resources\\ffmpeg.zip"));
				if (DLThreadFlag == TRUE) {
					DLThreadFlag = FALSE;
					DLCount = 0;
					this->PostMessage(WM_USER_COMPLETE_DOWNLOAD_LOAD_XML);
					OutputDebugString(_T("DLCountThread Ended.\n"));
					break;
				}
				else {
					continue;
				}
			}
			break;
		}
	}
}

void Cwaifu2xncnnvulkanDlg::DeleteThread()
{
	OutputDebugString(_T("DeleteThread Started.\n"));
	while (DeleteFileThreadFlag != TRUE)
	{
		DELETECOUNT = Utility->GetFileSizeStat(_T(".\\Resources\\ffmpeg.zip"));
		if (DeleteFileThreadFlag == TRUE) {
			DeleteFileThreadFlag = FALSE;
			Sleep(2000);
			break;
		}
		else {
			continue;
		}
	}

	this->PostMessage(WM_USER_COMPLETE_DELETE_LOAD_XML);
	OutputDebugString(_T("DLCountThread Ended.\n"));
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteWaifu2xThread\n"));
	Utility->TerminateExeName(_T("waifu2x-ncnn-vulkan.exe"));
	//pWaifu2xThread->PostThreadMessage(WM_QUIT, 0, 0);
	
	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteFFmpegThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteFFmpegThread\n"));
	//pFFmpegThread->PostThreadMessage(WM_QUIT, 0, 0);

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xCountThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteWaifu2xCountThread\n"));
	//pWaifu2xCountThread->PostThreadMessage(WM_QUIT, 0, 0);

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xCountDlgThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteWaifu2xCountDlgThread\n"));
	//pWaifu2xCountDlgThread->PostThreadMessage(WM_QUIT, 0, 0);

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteDLThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteDLThread\n"));
	//pDLThread->PostThreadMessage(WM_QUIT, 0, 0);

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteDLCountThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteDLCountThread\n"));
	//pDLCountThread->PostThreadMessage(WM_QUIT, 0, 0);

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteDeleteThread(WPARAM wParam, LPARAM lParam)
{
	//pDeleteThread->PostThreadMessage(WM_QUIT, 0, 0);

	return 0;
}

void Cwaifu2xncnnvulkanDlg::OnFfmpegSettings()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnFfmpegSettings\n"));
	FFMPEGSETTINGSDIALOG* SETTINGSDlg = new FFMPEGSETTINGSDIALOG;
	INT_PTR ret = SETTINGSDlg->DoModal();
	if (ret == IDOK) {
		ffmpeg_mainparam = SETTINGSDlg->FINALPARAM;
		GetPrivateProfileString(L"FFMPEG_SETTINGS", L"0x20FF", NULL, Waifu2x->FFmpeg_prm, 256, L".\\settings.ini");
		if (_tcscmp(Waifu2x->FFmpeg_prm, _T("")) != 0) {
			ffmpeg_mainparam = Waifu2x->FFmpeg_prm;
		}
		else {
			ffmpeg_mainparam = L"";
		}
		SAFE_DELETE(SETTINGSDlg);
		return;
	}
	else {
		SAFE_DELETE(SETTINGSDlg);
		return;
	}
}

void Cwaifu2xncnnvulkanDlg::OnFfmpegAudiosettings()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnFfmpegAudioSettings\n"));
	FFMPEGAUDIOSETTINGS* SETTINGSDlg = new FFMPEGAUDIOSETTINGS;
	INT_PTR ret = SETTINGSDlg->DoModal();
	if (ret == IDOK) {
		ffmpeg_audioparam = SETTINGSDlg->FINALPARAM_A;
		GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x20FF", NULL, Waifu2x->FFmpeg_audio_prm, 256, L".\\settings.ini");
		if (_tcscmp(Waifu2x->FFmpeg_audio_prm, _T("")) != 0) {
			ffmpeg_audioparam = Waifu2x->FFmpeg_audio_prm;
		}
		else {
			ffmpeg_audioparam = L"";
		}
		SAFE_DELETE(SETTINGSDlg);
		return;
	}
	else {
		SAFE_DELETE(SETTINGSDlg);
		return;
	}
}


void Cwaifu2xncnnvulkanDlg::OnFfmpegVideosettings()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnFfmpegVideoSettings\n"));
	FFMPEGVIDEOSETTINGS* SETTINGSDlg = new FFMPEGVIDEOSETTINGS;
	INT_PTR ret = SETTINGSDlg->DoModal();
	if (ret == IDOK) {
		ffmpeg_videoparam = SETTINGSDlg->FINALPARAM_V;
		GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x20FF", NULL, Waifu2x->FFmpeg_video_prm, 256, L".\\settings.ini");
		if (_tcscmp(Waifu2x->FFmpeg_video_prm, _T("")) != 0) {
			ffmpeg_videoparam = Waifu2x->FFmpeg_video_prm;
		}
		else {
			ffmpeg_videoparam = L"";
		}
		SAFE_DELETE(SETTINGSDlg);
		return;
	}
	else {
		SAFE_DELETE(SETTINGSDlg);
		return;
	}
}


HBRUSH Cwaifu2xncnnvulkanDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	int id = pWnd->GetDlgCtrlID();
	// TODO: ここで DC の属性を変更してください。
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		switch (id)
		{
		case IDC_STATIC_READSTATUS:
			if (GetDlgItem(IDC_STATIC_READSTATUS)->GetWindowTextLength() == 6) {
				pDC->SetTextColor(RGB(0, 200, 0));
				pDC->SetBkMode(TRANSPARENT);
			}
			else if (GetDlgItem(IDC_STATIC_READSTATUS)->GetWindowTextLength() == 12) {
				pDC->SetTextColor(RGB(255, 0, 0));
				pDC->SetBkMode(TRANSPARENT);
			}
			break;

		default:
			break;
		}
	}


	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


void Cwaifu2xncnnvulkanDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	wchar_t aname[MAX_PATH]{}, audiopath[MAX_PATH]{}, vname[MAX_PATH]{}, videopath[MAX_PATH]{};
	GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2000", NULL, (LPWSTR)aname, MAX_PATH, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2001", NULL, (LPWSTR)audiopath, MAX_PATH, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2000", NULL, (LPWSTR)vname, MAX_PATH, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2001", NULL, (LPWSTR)videopath, MAX_PATH, L".\\settings.ini");
	CString outDir = L"\\Resources\\takeout\\";
	CString currentPath = CURRENT_PATH + outDir;

	MSG msg;
	DELETEDIALOG* delDlg = new DELETEDIALOG;
	delDlg->Create(IDD_DELETEDIALOG);
	delDlg->ShowWindow(SW_SHOW);
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage())
		{
			::PostQuitMessage(0);
			break;
		}
	}

	if (_tcscmp(audiopath, _T("")) != 0) {
		CString ap = (LPCTSTR)audiopath;
		ap += _T("\\*.*");
		if (_tcscmp(aname, _T("")) != 0) {
			SHFILEOPSTRUCT tSHDeleteFile{};
			tSHDeleteFile.hwnd = ::GetDesktopWindow();
			tSHDeleteFile.wFunc = FO_DELETE;
			tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
			tSHDeleteFile.fAnyOperationsAborted = TRUE;
			tSHDeleteFile.hNameMappings = NULL;
			tSHDeleteFile.lpszProgressTitle = L"";

			// パス名のCStringの末尾に\0をつけて設定  
			ap += "?";
			ap.SetAt(ap.GetLength() - 1, NULL);
			tSHDeleteFile.pFrom = ap;
			tSHDeleteFile.pTo = NULL;
			SHFileOperation(&tSHDeleteFile);
			WaitForSingleObject(&tSHDeleteFile, INFINITE);
		}
	}
	if (_tcscmp(videopath, _T("")) != 0) {
		CString vp = (LPCTSTR)videopath;
		vp += _T("\\*.*");
		if (_tcscmp(vname, _T("")) != 0) {
			SHFILEOPSTRUCT tSHDeleteFile{};
			tSHDeleteFile.hwnd = ::GetDesktopWindow();
			tSHDeleteFile.wFunc = FO_DELETE;
			tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
			tSHDeleteFile.fAnyOperationsAborted = TRUE;
			tSHDeleteFile.hNameMappings = NULL;
			tSHDeleteFile.lpszProgressTitle = L"";

			// パス名のCStringの末尾に\0をつけて設定  
			vp += "?";
			vp.SetAt(vp.GetLength() - 1, NULL);
			tSHDeleteFile.pFrom = vp;
			tSHDeleteFile.pTo = NULL;
			SHFileOperation(&tSHDeleteFile);
			WaitForSingleObject(&tSHDeleteFile, INFINITE);
		}
	}

	SHFILEOPSTRUCT tSHDeleteFile{};
	tSHDeleteFile.hwnd = ::GetDesktopWindow();
	tSHDeleteFile.wFunc = FO_DELETE;
	tSHDeleteFile.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
	tSHDeleteFile.fAnyOperationsAborted = TRUE;
	tSHDeleteFile.hNameMappings = NULL;
	tSHDeleteFile.lpszProgressTitle = L"";

	// パス名のCStringの末尾に\0をつけて設定  
	currentPath += "?";
	currentPath.SetAt(currentPath.GetLength() - 1, NULL);
	tSHDeleteFile.pFrom = currentPath;
	tSHDeleteFile.pTo = NULL;
	SHFileOperation(&tSHDeleteFile);
	WaitForSingleObject(&tSHDeleteFile, INFINITE);
	delDlg->DestroyWindow();
	SAFE_DELETE(delDlg);
	SAFE_DELETE(Waifu2x->fnt1);
	SAFE_DELETE(MAINSTR_FUNC);
	SAFE_DELETE(VERSIONSTR_FUNC);
	SAFE_DELETE(COREUTIL_FUNC);
	SAFE_FREE(lpPath);
	SAFE_FREE(Waifu2x->cpuinfo);
	SAFE_FREE(Waifu2x->gpuinfo);
	SAFE_FREE(Waifu2x->waifu2x_prm);
	SAFE_FREE(Waifu2x->FFmpeg_prm);
	SAFE_FREE(Waifu2x->FFmpeg_audio_prm);
	SAFE_FREE(Waifu2x->FFmpeg_video_prm);
	waifu2x_param.Empty();
	ffmpeg_mainparam.Empty();
	ffmpeg_audioparam.Empty();
	ffmpeg_videoparam.Empty();
	DeleteFile(L".\\Resources\\gpu.bat");
	DeleteFile(L".\\Resources\\ffmpeg.zip");
	SAFE_DELETE(WAIFU2X_UTIL_FUNC);
	OutputDebugString(_T("End Application.\n"));
}



void Cwaifu2xncnnvulkanDlg::OnUpdatecheck()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("OnUpdatecheck\n"));
	CString LatestVersion = Utility->AppUpdateCheck();
	if (LatestVersion == _T("")) {
		MessageBox(_T("情報取得中にエラーが発生しました。\nインターネット接続がされているか確認してください。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	CString CurrentVersion = Utility->AppCurrentVersionCheck();
	if (CurrentVersion == _T("")) {
		MessageBox(_T("情報取得中にエラーが発生しました。\nインターネット接続がされているか確認してください。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
	if (LatestVersion == CurrentVersion) {
		MessageBox(_T("最新バージョン：") + LatestVersion + _T("\n現在使用中のバージョン：") + CurrentVersion + _T("\nアプリケーションのバージョンは最新です。"), _T("完了"), MB_ICONINFORMATION | MB_OK);
		return;
	}
	else if (LatestVersion > CurrentVersion) {
		UINT ret;
		ret = MessageBox(_T("最新バージョン：") + LatestVersion + _T("現在使用中のバージョン：") + CurrentVersion + _T("\nアプリケーションのアップデートが可能です。サイトを開きますか？"), _T("アップデート確認"), MB_ICONINFORMATION | MB_YESNOCANCEL);
		if (ret == IDYES) {
			ShellExecute(NULL, _T("open"), _T("https://xyle-official.com/tools/waifu2x/#latest-release"), NULL, NULL, SW_SHOWNORMAL);
			return;
		}
		else if (ret == IDNO) {
			return;
		}
		else if (ret == IDCANCEL) {
			MessageBox(_T("キャンセルされました。"), _T("キャンセル"), MB_ICONWARNING | MB_OK);
		}
		else {
			return;
		}
	}
	else {
		MessageBox(_T("情報取得中にエラーが発生しました。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}
}