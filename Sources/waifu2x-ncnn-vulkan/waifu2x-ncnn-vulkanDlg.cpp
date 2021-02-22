
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
#define WM_USER_COMPLETE_DELETE_MAIN_LOAD_XML (WM_APP + 0x80)

UINT Cwaifu2xncnnvulkanDlg::DLThreadFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::DLErrorFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::DLCount = 0;
UINT Cwaifu2xncnnvulkanDlg::DLCurCount = 0;
UINT Cwaifu2xncnnvulkanDlg::FILECOUNT = 0;
UINT Cwaifu2xncnnvulkanDlg::UPSCALE_COUNT = 0;
UINT Cwaifu2xncnnvulkanDlg::DELETECURCOUNT = 0;
UINT Cwaifu2xncnnvulkanDlg::DELETEMAINCOUNT = 0;
UINT Cwaifu2xncnnvulkanDlg::DELETESUBCOUNT = 0;
UINT Cwaifu2xncnnvulkanDlg::ProgressThreadFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::UpscaleExceptionFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::Waifu2xThreadFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::Waifu2xReUpscalingFlag = 0;
UINT Cwaifu2xncnnvulkanDlg::DeleteExceptionFlag = 0;
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
	void SetDlgLang();
	HICON ICO;
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
	ICO = ExtractIcon(AfxGetInstanceHandle(), RES_LIBRARY, 0);//LoadIcon(Core->hinst, MAKEINTRESOURCE(1003));
	if (!ICO) {
		MessageBox(ERROR_INSTANCE, ERROR_TITLE, MB_ICONERROR | MB_OK);
	}
	else {
		CStatic* box = (CStatic*)GetDlgItem(IDC_STATIC_ICON);
		box->SetIcon(ICO);
		box->InvalidateRect(NULL, 1);
	}
	SetDlgLang();

	this->xv_Static_AppVersionText.SetWindowText(APP_VERSION);
	this->xv_Static_CmdVersionText.SetWindowText(CMD_VERSION);
	this->xv_Static_FFmpeg_VersionText.SetWindowText(FF_VERSION);

	m_hyperlink = RGB(51, 102, 187);
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


void CAboutDlg::SetDlgLang()
{
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

	LoadString(Core->Lang_hinst, IDS_STATIC_ABT_HEADER, (LPTSTR)STATIC_ABT_HEADER, 256);
	GetDlgItem(IDC_STATIC_HED)->SetWindowText(STATIC_ABT_HEADER);
	LoadString(Core->Lang_hinst, IDS_STATIC_ABT_VER1, (LPTSTR)STATIC_ABT_VER1, 256);
	GetDlgItem(IDC_STATIC_V1)->SetWindowText(STATIC_ABT_VER1);
	LoadString(Core->Lang_hinst, IDS_STATIC_ABT_VER2, (LPTSTR)STATIC_ABT_VER2, 256);
	GetDlgItem(IDC_STATIC_V2)->SetWindowText(STATIC_ABT_VER2);
	LoadString(Core->Lang_hinst, IDS_STATIC_ABT_VER3, (LPTSTR)STATIC_ABT_VER3, 256);
	GetDlgItem(IDC_STATIC_V3)->SetWindowText(STATIC_ABT_VER3);
	LoadString(Core->Lang_hinst, IDS_STATIC_ABT_HYPER1, (LPTSTR)STATIC_ABT_HYPER1, 256);
	GetDlgItem(IDC_STATIC_HYPER1)->SetWindowText(STATIC_ABT_HYPER1);
	LoadString(Core->Lang_hinst, IDS_STATIC_ABT_HYPER2, (LPTSTR)STATIC_ABT_HYPER2, 256);
	GetDlgItem(IDC_STATIC_HYPER2)->SetWindowText(STATIC_ABT_HYPER2);
}


void CAboutDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	FreeLibrary(Core->hinst);
	FreeLibrary(Core->Lang_hinst);
	SAFE_DELETE(CORE_FUNC);
	SAFE_DELETE(MAINSTR_FUNC);
	SAFE_DELETE(VERSIONSTR_FUNC);
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
	DELETEPATH = _T("");
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
	ON_MESSAGE(WM_USER_COMPLETE_DELETE_MAIN_LOAD_XML, Cwaifu2xncnnvulkanDlg::OnCompleteDeleteMainThread)// UserMessage
	ON_MESSAGE(WM_USER_COMPLETE_COUNT_THREAD, Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xCountThread)// UserMessage
	ON_MESSAGE(WM_USER_COMPLETE_COUNT_DLG_THREAD, Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xCountDlgThread)// UserMessage
	ON_COMMAND(ID_VIDEORESIZE, &Cwaifu2xncnnvulkanDlg::OnVideoresize)
	ON_COMMAND(ID_VIDEOAUDIOEXPORT, &Cwaifu2xncnnvulkanDlg::OnVideoaudioexport)
	ON_COMMAND(ID_UPDATECHECK, &Cwaifu2xncnnvulkanDlg::OnUpdatecheck)
	ON_COMMAND(ID_LANGUAGE_JAPANESE, &Cwaifu2xncnnvulkanDlg::OnLanguageJapanese)
	ON_COMMAND(ID_LANGUAGE_ENGLISH, &Cwaifu2xncnnvulkanDlg::OnLanguageEnglish)
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
	CFileFind init;

	if (init.FindFile(RES_LIBRARY)) {
		if (init.FindFile(JPN_RES_LIBRARY)) {
			if (init.FindFile(ENG_RES_LIBRARY)) {
				LoadDlgStr();
			}
			else {
				MessageBox(_T("'waifu2xnvgui.English.dll' cannot be found, so the code execution cannot continue. Reinstalling the program may resolve this issue."), _T("waifu2xnvgui.exe - System error"), MB_ICONERROR | MB_OK);
				EndDialog(IDCANCEL);
				PostMessage(WM_COMMAND, IDCANCEL);
				return FALSE;
			}
		}
		else {
			MessageBox(_T("'waifu2xnvgui.Japanese.dll' cannot be found, so the code execution cannot continue. Reinstalling the program may resolve this issue."), _T("waifu2xnvgui.exe - System error"), MB_ICONERROR | MB_OK);
			EndDialog(IDCANCEL);
			PostMessage(WM_COMMAND, IDCANCEL);
			return FALSE;
		}
	}
	else {
		MessageBox(_T("'App.General.resLib.dll' cannot be found, so the code execution cannot continue. Reinstalling the program may resolve this issue."), _T("waifu2xnvgui.exe - System error"), MB_ICONERROR | MB_OK);
		EndDialog(IDCANCEL);
		PostMessage(WM_COMMAND, IDCANCEL);
		return FALSE;
	}

	SetWindowText(_T("waifu2x-ncnn-vulkan GUI Edition [") + APP_VERSION + _T(", Feb. 22, 2021]"));
	Utility->GetGPUInfo();

	MSG msg;
	
	CString resdir = _T("\\Resources");
	CString init1 = _T("\\Resources\\ffmpeg\\ffmpeg.exe");
	CString init2 = _T("\\Resources\\waifu2x-ncnn-vulkan\\waifu2x-ncnn-vulkan.exe");
	CString init3 = _T("\\Resources\\image2png.exe");
	CString tmp = CURRENT_PATH + resdir + _T("\\tmp");
	CString ffzip = CURRENT_PATH + resdir + _T("\\ffmpeg.zip");
	CString initPath1 = CURRENT_PATH + init1;
	CString initPath2 = CURRENT_PATH + init2;
	CString initPath3 = CURRENT_PATH + init3;

	if (init.FindFile(ffzip)) {
		DeleteFile(ffzip);
	}

	GetPrivateProfileString(L"WAIFU2X_SETTINGS", L"0x20FF", NULL, Waifu2x->waifu2x_prm, 256, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_SETTINGS", L"0x20FF", NULL, Waifu2x->FFmpeg_prm, 256, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x20FF", NULL, Waifu2x->FFmpeg_video_prm, 256, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x20FF", NULL, Waifu2x->FFmpeg_audio_prm, 256, L".\\settings.ini");

	if (_tcscmp(Waifu2x->waifu2x_prm, _T("")) != 0) {
		waifu2x_param = Waifu2x->waifu2x_prm;
	}
	else {
		waifu2x_param = _T("");
	}
	if (_tcscmp(Waifu2x->FFmpeg_prm, _T("")) != 0) {
		ffmpeg_mainparam = Waifu2x->FFmpeg_prm;
	}
	else {
		ffmpeg_mainparam = _T("");
	}
	if (_tcscmp(Waifu2x->FFmpeg_video_prm, _T("")) != 0) {
		ffmpeg_videoparam = Waifu2x->FFmpeg_video_prm;
	}
	else {
		ffmpeg_videoparam = _T("");
	}
	if (_tcscmp(Waifu2x->FFmpeg_audio_prm, _T("")) != 0) {
		ffmpeg_audioparam = Waifu2x->FFmpeg_audio_prm;
	}
	else {
		ffmpeg_audioparam = _T("");
	}

	CString CPUInfo = Utility->GetCPUInfo();
	CString GPUInfo = Utility->GetGPUInfo();

	OutputDebugString(_T("Detected CPU: ") + CPUInfo);
	OutputDebugString(_T("Detected GPU: ") + GPUInfo + _T("\n"));

	ZeroMemory(&Waifu2x->cpuinfo[0], 256);
	ZeroMemory(&Waifu2x->gpuinfo[0], 256);
	_tcscpy_s(&Waifu2x->cpuinfo[0], 256, CPUInfo);
	_tcscpy_s(&Waifu2x->gpuinfo[0], 256, GPUInfo);

	CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT2);
	CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT4);
	edit1->SetWindowText(Waifu2x->cpuinfo);
	edit2->SetWindowText(Waifu2x->gpuinfo);

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
			VER = TEXT_UNKNOWN;
			FLAG = 0;
		}
		else {
			VER = ver;
			FLAG = 1;
		}
		if (FLAG == 0) {
			UINT ret;
			ret = MessageBox(UPDATE_LATEST + Utility->FFmpegVersionCheck() + _T("\n") + UPDATE_CURRENT + VER + _T("\n") + UPDATE_FFMPEG, UPDATE_FFMPEG_TITLE, MB_ICONINFORMATION | MB_YESNO);
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
					MessageBox(ERROR_SERVER, ERROR_TITLE, MB_ICONERROR | MB_OK);
					EndDialog(IDCANCEL);
					PostMessage(WM_COMMAND, IDCANCEL);
					return FALSE;
				}
				OutputDebugString(_T("Server connected.\n"));
				ret = Utility->DownloadFile(DOWNLOAD_URL_STRING, _T(".\\Resources\\ffmpeg.zip"), 4096);
				DLCurCount = Utility->GetFileSizeStat(_T(".\\Resources\\ffmpeg.zip"));

				DeleteFile(_T(".\\Resources\\ffmpeg.zip"));

				pDLCountThread = AfxBeginThread(DLCountThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
				if (pDLCountThread)
				{
					pDLCountThread->m_pMainWnd = this;
					pDLCountThread->m_bAutoDelete = TRUE;
					pDLCountThread->ResumeThread();

					DLDIALOG DLG;
					DLG.DoModal();
				}

				if (DLErrorFlag == 1) {
					DeleteFile(_T(".\\Resources\\ffmpeg.zip"));
					DLErrorFlag = 0;
				}

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
						EndDialog(IDCANCEL);
						PostMessage(WM_COMMAND, IDCANCEL);
						return FALSE;
					}
					TCHAR* ffzipT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
					if (NULL == ffzipT) {
						perror("can not malloc");
						OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
						EndDialog(IDCANCEL);
						PostMessage(WM_COMMAND, IDCANCEL);
						return FALSE;
					}
					else {
						ZeroMemory(&ffzipT[0], 256);
						_tcscpy_s(&ffzipT[0], 256, ffzip);
					}
					TCHAR* tmpT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
					if (NULL == tmpT) {
						perror("can not malloc");
						OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
						EndDialog(IDCANCEL);
						PostMessage(WM_COMMAND, IDCANCEL);
						return FALSE;
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
						WritePrivateProfileString(L"FFMPEG_LATEST_VERSION", L"0x00000", Utility->FFmpegVersionCheck(), L".\\settings.ini");
						MessageBox(INFO_DOWNLOAD, INFO_TITLE, MB_ICONINFORMATION | MB_OK);
						OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed success.\n"));
						initPath1.Empty();
					}
					else {
						OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed error.\n"));
						initPath1.Empty();
						MessageBox(ERROR_EXCEPTION, ERROR_TITLE, MB_ICONERROR | MB_OK);
						EndDialog(IDCANCEL);
						PostMessage(WM_COMMAND, IDCANCEL);
						return FALSE;
					}
				}
				else {
					DIALOG->DestroyWindow();
					SAFE_DELETE(DIALOG);
					MessageBox(ERROR_DOWNLOAD, ERROR_TITLE, MB_ICONERROR | MB_OK);
					EndDialog(IDCANCEL);
					PostMessage(WM_COMMAND, IDCANCEL);
					return FALSE;
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
				ret = MessageBox(UPDATE_LATEST + Utility->FFmpegVersionCheck() + _T("\n") + UPDATE_CURRENT + VER + _T("\n") + UPDATE_FFMPEG, UPDATE_FFMPEG_TITLE, MB_ICONINFORMATION | MB_YESNO);
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
						MessageBox(ERROR_SERVER, ERROR_TITLE, MB_ICONERROR | MB_OK);
						EndDialog(IDCANCEL);
						PostMessage(WM_COMMAND, IDCANCEL);
						return FALSE;
					}
					OutputDebugString(_T("Server connected.\n"));
					ret = Utility->DownloadFile(DOWNLOAD_URL_STRING, _T(".\\Resources\\ffmpeg.zip"), 4096);
					DLCurCount = Utility->GetFileSizeStat(_T(".\\Resources\\ffmpeg.zip"));

					DeleteFile(_T(".\\Resources\\ffmpeg.zip"));

					pDLCountThread = AfxBeginThread(DLCountThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
					if (pDLCountThread)
					{
						pDLCountThread->m_pMainWnd = this;
						pDLCountThread->m_bAutoDelete = TRUE;
						pDLCountThread->ResumeThread();

						DLDIALOG DLG;
						DLG.DoModal();
					}

					if (DLErrorFlag == 1) {
						DeleteFile(_T(".\\Resources\\ffmpeg.zip"));
						DLErrorFlag = 0;
					}

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
							EndDialog(IDCANCEL);
							PostMessage(WM_COMMAND, IDCANCEL);
							return FALSE;
						}
						TCHAR* ffzipT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
						if (NULL == ffzipT) {
							perror("can not malloc");
							OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
							EndDialog(IDCANCEL);
							PostMessage(WM_COMMAND, IDCANCEL);
							return FALSE;
						}
						else {
							ZeroMemory(&ffzipT[0], 256);
							_tcscpy_s(&ffzipT[0], 256, ffzip);
						}
						TCHAR* tmpT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
						if (NULL == tmpT) {
							perror("can not malloc");
							OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
							EndDialog(IDCANCEL);
							PostMessage(WM_COMMAND, IDCANCEL);
							return FALSE;
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
							WritePrivateProfileString(L"FFMPEG_LATEST_VERSION", L"0x00000", Utility->FFmpegVersionCheck(), L".\\settings.ini");
							MessageBox(INFO_DOWNLOAD, INFO_TITLE, MB_ICONINFORMATION | MB_OK);
							OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed success.\n"));
							initPath1.Empty();
						}
						else {
							OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed error.\n"));
							initPath1.Empty();
							MessageBox(ERROR_EXCEPTION, ERROR_TITLE, MB_ICONERROR | MB_OK);
							EndDialog(IDCANCEL);
							PostMessage(WM_COMMAND, IDCANCEL);
							return FALSE;
						}
					}
					else {
						DIALOG->DestroyWindow();
						SAFE_DELETE(DIALOG);
						MessageBox(ERROR_DOWNLOAD, ERROR_TITLE, MB_ICONERROR | MB_OK);
						EndDialog(IDCANCEL);
						PostMessage(WM_COMMAND, IDCANCEL);
						return FALSE;
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
		INT_PTR sel = MessageBox(ERROR_FFMPEG, ERROR_TITLE, MB_ICONERROR | MB_YESNO);
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
				MessageBox(ERROR_SERVER, ERROR_TITLE, MB_ICONERROR | MB_OK);
				EndDialog(IDCANCEL);
				PostMessage(WM_COMMAND, IDCANCEL);
				return FALSE;
			}
			OutputDebugString(_T("Server connected.\n"));
			ret = Utility->DownloadFile(DOWNLOAD_URL_STRING, _T(".\\Resources\\ffmpeg.zip"), 4096);
			DLCurCount = Utility->GetFileSizeStat(_T(".\\Resources\\ffmpeg.zip"));

			DeleteFile(_T(".\\Resources\\ffmpeg.zip"));

			pDLCountThread = AfxBeginThread(DLCountThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			if (pDLCountThread)
			{
				pDLCountThread->m_pMainWnd = this;
				pDLCountThread->m_bAutoDelete = TRUE;
				pDLCountThread->ResumeThread();

				DLDIALOG DLG;
				DLG.DoModal();
			}

			if (DLErrorFlag == 1) {
				DeleteFile(_T(".\\Resources\\ffmpeg.zip"));
				DLErrorFlag = 0;
			}

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
					EndDialog(IDCANCEL);
					PostMessage(WM_COMMAND, IDCANCEL);
					return FALSE;
				}
				TCHAR* ffzipT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
				if (NULL == ffzipT) {
					perror("can not malloc");
					OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
					EndDialog(IDCANCEL);
					PostMessage(WM_COMMAND, IDCANCEL);
					return FALSE;
				}
				else {
					ZeroMemory(&ffzipT[0], 256);
					_tcscpy_s(&ffzipT[0], 256, ffzip);
				}
				TCHAR* tmpT = (TCHAR*)malloc(sizeof(TCHAR) * 256); // Convert CString to TCHAR.
				if (NULL == tmpT) {
					perror("can not malloc");
					OutputDebugString(_T("TCHAR syntax (tmpT) malloc failed.\n"));
					EndDialog(IDCANCEL);
					PostMessage(WM_COMMAND, IDCANCEL);
					return FALSE;
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
					MessageBox(INFO_DOWNLOAD, INFO_TITLE, MB_ICONINFORMATION | MB_OK);
					WritePrivateProfileString(L"FFMPEG_LATEST_VERSION", L"0x00000", Utility->FFmpegVersionCheck(), L".\\settings.ini");
					OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed success.\n"));
					initPath1.Empty();
				}
				else {
					OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed error.\n"));
					MessageBox(ERROR_EXCEPTION, ERROR_TITLE, MB_ICONERROR | MB_OK);
					EndDialog(IDCANCEL);
					PostMessage(WM_COMMAND, IDCANCEL);
					return FALSE;
				}
			}
			else {
				DIALOG->DestroyWindow();
				SAFE_DELETE(DIALOG);
				MessageBox(ERROR_DOWNLOAD, ERROR_TITLE, MB_ICONERROR | MB_OK);
				EndDialog(IDCANCEL);
				PostMessage(WM_COMMAND, IDCANCEL);
				return FALSE;
			}
		}
		else if (sel == IDNO)
		{
			MessageBox(ERROR_APPNOTFOUND, ERROR_TITLE, MB_ICONERROR | MB_OK);
			OutputDebugString(_T("CFileFind init.FindFile(") + initPath1 + _T("): File readed error.\n"));
			EndDialog(IDCANCEL);
			PostMessage(WM_COMMAND, IDCANCEL);
			return FALSE;
		}
		else {
			EndDialog(IDCANCEL);
			PostMessage(WM_COMMAND, IDCANCEL);
			return FALSE;
		}
	}

	if (init.FindFile(initPath2)) {
		OutputDebugString(_T("CFileFind init.FindFile(") + initPath2 + _T("): File readed success.\n"));
		initPath2.Empty();
	}
	else
	{
		INT_PTR sel = MessageBox(ERROR_WAIFU2X, ERROR_TITLE, MB_ICONERROR | MB_ABORTRETRYIGNORE);
		if (sel == IDABORT)
		{
			EndDialog(IDCANCEL);
			PostMessage(WM_COMMAND, IDCANCEL);
			return FALSE;
		}
		else if (sel == IDRETRY)
		{
			if (init.FindFile(initPath2)) {
				OutputDebugString(_T("CFileFind init.FindFile(") + initPath2 + _T("): File readed success.\n"));
				initPath2.Empty();
			}
			else {
				MessageBox(ERROR_APPNOTFOUND, ERROR_TITLE, MB_ICONERROR | MB_OK);
				OutputDebugString(_T("CFileFind init.FindFile(") + initPath2 + _T("): File readed error.\n"));
				EndDialog(IDCANCEL);
				PostMessage(WM_COMMAND, IDCANCEL);
				return FALSE;
			}
		}
		else if (sel == IDIGNORE)
		{
			MessageBox(ERROR_ABORT, ERROR_TITLE, MB_ICONERROR | MB_OK);
			EndDialog(IDCANCEL);
			PostMessage(WM_COMMAND, IDCANCEL);
			return FALSE;
		}
		else {
			EndDialog(IDCANCEL);
			PostMessage(WM_COMMAND, IDCANCEL);
			return FALSE;
		}
	}

	if (init.FindFile(initPath3)) {
		OutputDebugString(_T("CFileFind init.FindFile(") + initPath3 + _T("): File readed success.\n"));
		initPath3.Empty();
	}
	else
	{
		INT_PTR sel = MessageBox(ERROR_IMAGE2PNG, ERROR_TITLE, MB_ICONERROR | MB_ABORTRETRYIGNORE);
		if (sel == IDABORT)
		{
			initPath3.Empty();
			EndDialog(IDCANCEL);
			PostMessage(WM_COMMAND, IDCANCEL);
			return FALSE;
		}
		else if (sel == IDRETRY)
		{
			if (init.FindFile(initPath2)) {
				OutputDebugString(_T("CFileFind init.FindFile(") + initPath3 + _T("): File readed success.\n"));
				initPath3.Empty();
			}
			else {
				MessageBox(ERROR_APPNOTFOUND, ERROR_TITLE, MB_ICONERROR | MB_OK);
				OutputDebugString(_T("CFileFind init.FindFile(") + initPath3 + _T("): File readed error.\n"));
				initPath3.Empty();
				EndDialog(IDCANCEL);
				PostMessage(WM_COMMAND, IDCANCEL);
				return FALSE;
			}
		}
		else if (sel == IDIGNORE)
		{
			MessageBox(ERROR_ABORT, ERROR_TITLE, MB_ICONERROR | MB_OK);
			initPath3.Empty();
			EndDialog(IDCANCEL);
			PostMessage(WM_COMMAND, IDCANCEL);
			return FALSE;
		}
		else {
			initPath3.Empty();
			EndDialog(IDCANCEL);
			PostMessage(WM_COMMAND, IDCANCEL);
			return FALSE;
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
		MessageBox(ERROR_NOTSELECTED, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (waifu2x_param.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_mainparam.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_audioparam.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_videoparam.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else
	{
		if (Waifu2x->waifu2x_prm != _T("")) {
			waifu2x_param = Waifu2x->waifu2x_prm;
		}
		CString EXT;
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

		if (PathFileExists(currentPath)) {
			OutputDebugString(_T("Path: '") + currentPath + _T("' file exists.\n"));
			DELETEPATH = currentPath;
			DELETEMAINCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
			pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			if (pDeleteThread)
			{
				pDeleteThread->m_pMainWnd = this;
				pDeleteThread->m_bAutoDelete = TRUE;
				if (DeleteFileThreadFlag != FALSE) {
					DeleteFileThreadFlag = FALSE;
				}
				pDeleteThread->ResumeThread();

				DELETEDIALOG DIALOG;
				DIALOG.DoModal();
			}
		}
		else {
			OutputDebugString(_T("Path: '") + currentPath + _T("' file doesn't exist.\n"));
		}

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
			WaitForSingleObject(pFFmpegThread->m_hThread, INFINITE);
		}

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
			if (SuspendFlag == TRUE) {
				while (pThread->m_Dlg.m_hWnd) {
					if (pThread->m_Dlg.m_hWnd == 0) {
						break;
					}
				}

				if (PathFileExists(currentPath)) {
					OutputDebugString(_T("Path: '") + currentPath + _T("' file exists.\n"));
					DELETEPATH = currentPath;
					DELETEMAINCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
					pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
					if (pDeleteThread)
					{
						pDeleteThread->m_pMainWnd = this;
						pDeleteThread->m_bAutoDelete = TRUE;
						if (DeleteFileThreadFlag != FALSE) {
							DeleteFileThreadFlag = FALSE;
						}
						pDeleteThread->ResumeThread();

						DELETEDIALOG DIALOG;
						DIALOG.DoModal();
					}
				}
				else {
					OutputDebugString(_T("Path: '") + currentPath + _T("' file doesn't exist.\n"));
				}

				MessageBox(WARN_ABORT, INFO_TITLE, MB_ICONWARNING | MB_OK);
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
				this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
				CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
				static1->InvalidateRect(NULL, 1);
				if (Waifu2xThreadFlag != FALSE) {
					Waifu2xThreadFlag = FALSE;
				}
				if (ProgressThreadFlag != FALSE) {
					ProgressThreadFlag = FALSE;
				}
				if (SuspendFlag != FALSE) {
					SuspendFlag = FALSE;
				}
				return;
			}
		}
		while (pThread->m_Dlg.m_hWnd) {
			if (pThread->m_Dlg.m_hWnd == 0) {
				break;
			}
		}

		if (ProgressThreadFlag != FALSE) {
			ProgressThreadFlag = FALSE;
		}
		if (SuspendFlag != FALSE) {
			SuspendFlag = FALSE;
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

		if (Waifu2xThreadFlag != FALSE) {
			Waifu2xThreadFlag = FALSE;
		}

		if (UpscaleExceptionFlag == 1) {
			UpscaleExceptionFlag = 0;
			CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
			CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
			CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
			CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON6);
			CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON2);
			button1->EnableWindow(FALSE);
			button2->EnableWindow(FALSE);
			button3->EnableWindow(FALSE);
			button4->EnableWindow(FALSE);
			button5->EnableWindow(FALSE);
			MessageBox(ERROR_UPSCALE, ERROR_UPSCALE_TITLE, MB_ICONINFORMATION | MB_OK);
			return;
		}
		else {
			if (PathFileExists(w2xout)) {
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
				MessageBox(INFO_UPSCALE, INFO_UPSCALE_TITLE, MB_ICONINFORMATION | MB_OK);
				return;
			}
			else {
				CButton* button1 = (CButton*)GetDlgItem(IDC_BUTTON1);
				CButton* button2 = (CButton*)GetDlgItem(IDC_BUTTON3);
				CButton* button3 = (CButton*)GetDlgItem(IDC_BUTTON4);
				CButton* button4 = (CButton*)GetDlgItem(IDC_BUTTON6);
				CButton* button5 = (CButton*)GetDlgItem(IDC_BUTTON2);
				button1->EnableWindow(FALSE);
				button2->EnableWindow(FALSE);
				button3->EnableWindow(FALSE);
				button4->EnableWindow(FALSE);
				button5->EnableWindow(FALSE);
				MessageBox(ERROR_UPSCALE, ERROR_UPSCALE_TITLE, MB_ICONINFORMATION | MB_OK);
				return;
			}
		}
	}
}

void Cwaifu2xncnnvulkanDlg::OnBnClickedButton3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	OutputDebugString(_T("Button clicked: button3\n"));
	if (VIDEOPATH.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_NOTSELECTED, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (waifu2x_param.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_mainparam.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_audioparam.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_videoparam.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	Waifu2xReUpscalingFlag = TRUE;

	if (_tcscmp(Waifu2x->waifu2x_prm, _T("")) != 0) {
		waifu2x_param = Waifu2x->waifu2x_prm;
	}
	UINT ext;
	CString EXT;
	MSG msg;
	CString outDir = L"\\Resources\\takeout\\_temp-project\\image-frames\\*.*";
	CString currentPath = CURRENT_PATH + outDir;
	CString current = CURRENT_PATH + L"\\Resources\\takeout\\";
	CString w2xin = current + L"_temp-project\\image-frames\\";
	CString w2xout = current + L"_temp-project\\image-frames2x\\";

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

	if (PathFileExists(w2xin)) {
		OutputDebugString(_T("Path: '") + currentPath + _T("' file exists.\n"));
		DELETEPATH = currentPath;
		DELETEMAINCOUNT = Utility->GetDirectoryFileCount(w2xin.GetString());
		pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		if (pDeleteThread)
		{
			pDeleteThread->m_pMainWnd = this;
			pDeleteThread->m_bAutoDelete = TRUE;
			if (DeleteFileThreadFlag != FALSE) {
				DeleteFileThreadFlag = FALSE;
			}
			pDeleteThread->ResumeThread();

			DELETEDIALOG DIALOG;
			DIALOG.DoModal();
		}
	}
	else {
		OutputDebugString(_T("Path: '") + currentPath + _T("' file doesn't exist.\n"));
	}

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

	Utility->RunProcess(cmdT, SW_HIDE);

	SAFE_FREE(cmdT);

	POPUP->DestroyWindow();
	SAFE_DELETE(POPUP);

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
		if (SuspendFlag == TRUE) {
			while (pThread->m_Dlg.m_hWnd) {
				if (pThread->m_Dlg.m_hWnd == 0) {
					break;
				}
			}

			if (PathFileExists(current)) {
				OutputDebugString(_T("Path: '") + current + _T("' file exists.\n"));
				DELETEPATH = current;
				DELETEMAINCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
				pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
				if (pDeleteThread)
				{
					pDeleteThread->m_pMainWnd = this;
					pDeleteThread->m_bAutoDelete = TRUE;
					if (DeleteFileThreadFlag != FALSE) {
						DeleteFileThreadFlag = FALSE;
					}
					pDeleteThread->ResumeThread();

					DELETEDIALOG DIALOG;
					DIALOG.DoModal();
				}
			}
			else {
				OutputDebugString(_T("Path: '") + current + _T("' file doesn't exist.\n"));
			}

			MessageBox(WARN_ABORT, INFO_TITLE, MB_ICONWARNING | MB_OK);
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
			this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			if (Waifu2xThreadFlag != FALSE) {
				Waifu2xThreadFlag = FALSE;
			}
			if (ProgressThreadFlag != FALSE) {
				ProgressThreadFlag = FALSE;
			}
			if (Waifu2xReUpscalingFlag != FALSE) {
				Waifu2xReUpscalingFlag = FALSE;
			}
			if (SuspendFlag != FALSE) {
				SuspendFlag = FALSE;
			}
			return;
		}
	}
	while (pThread->m_Dlg.m_hWnd) {
		if (pThread->m_Dlg.m_hWnd == 0) {
			break;
		}
	}

	if (ProgressThreadFlag != FALSE) {
		ProgressThreadFlag = FALSE;
	}
	if (SuspendFlag != FALSE) {
		SuspendFlag = FALSE;
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

	if (Waifu2xReUpscalingFlag != FALSE) {
		Waifu2xReUpscalingFlag = FALSE;
	}
	if (Waifu2xThreadFlag != FALSE) {
		Waifu2xThreadFlag = FALSE;
	}

	if (UpscaleExceptionFlag == 1) {
		UpscaleExceptionFlag = 0;
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
		this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
		this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
		this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
		CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
		static1->InvalidateRect(NULL, 1);
		MessageBox(ERROR_REUPSCALE, ERROR_REUPSCALE_TITLE, MB_ICONINFORMATION | MB_OK);
		return;
	}
	else {
		if (PathFileExists(w2xout)) {
			MessageBox(INFO_REUPSCALE, INFO_REUPSCALE_TITLE, MB_ICONINFORMATION | MB_OK);
			return;
		}
		else {
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
			this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			MessageBox(ERROR_REUPSCALE, ERROR_REUPSCALE_TITLE, MB_ICONINFORMATION | MB_OK);
			return;
		}
	}
}


void Cwaifu2xncnnvulkanDlg::OnBnClickedButton4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	OutputDebugString(_T("Button clicked: button4\n"));
	if (VIDEOPATH.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_NOTSELECTED, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (waifu2x_param.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_mainparam.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_audioparam.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (ffmpeg_videoparam.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}

	if (_tcscmp(Waifu2x->waifu2x_prm, _T("")) != 0) {
		ffmpeg_mainparam = Waifu2x->FFmpeg_prm;
	}

	CFileFind find;
	CString outDir = L"\\Resources\\takeout\\";
	CString currentPath = CURRENT_PATH + outDir;

	CString filter = FILTER_MOVIE;
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
		Utility->AfxReplaceStr(ffmpeg_mainparam, L"$OutFile", _T("\"$OutFile\""));
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

		Utility->RunProcess(ffmpegcmdT, SW_SHOW);

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

		if (_tcscmp(audiopath, _T("")) != 0) {
			CString ap = (LPCTSTR)audiopath;
			ap += _T("\\*.*");
			if (_tcscmp(aname, _T("")) != 0) {
				Utility->DeleteALLFiles(ap);
			}
		}
		if (_tcscmp(videopath, _T("")) != 0) {
			CString vp = (LPCTSTR)videopath;
			vp += _T("\\*.*");
			if (_tcscmp(vname, _T("")) != 0) {
				Utility->DeleteALLFiles(vp);
			}
		}

		if (PathFileExists(currentPath)) {
			OutputDebugString(_T("Path: '") + currentPath + _T("' file exists.\n"));
			DELETEPATH = currentPath;
			DELETEMAINCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
			pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			if (pDeleteThread)
			{
				pDeleteThread->m_pMainWnd = this;
				pDeleteThread->m_bAutoDelete = TRUE;
				if (DeleteFileThreadFlag != FALSE) {
					DeleteFileThreadFlag = FALSE;
				}
				pDeleteThread->ResumeThread();

				DELETEDIALOG DIALOG;
				DIALOG.DoModal();
			}
		}
		else {
			OutputDebugString(_T("Path: '") + currentPath + _T("' file doesn't exist.\n"));
		}

		SAFE_FREE(lpPath);
		SAFE_FREE(ffmpegcmdT);

		if (find.FindFile(OutPath)) {
			MessageBox(INFO_SUCCESS, INFO_TITLE, MB_ICONINFORMATION | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
		}
		else {
			MessageBox(ERROR_FAILED, ERROR_TITLE, MB_ICONERROR | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
		}
	}
	else {
		MessageBox(WARN_SAVECANCEL, WARN_CANCEL, MB_ICONWARNING | MB_OK);

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

		if (pseta == 0) {
			OutputDebugString(_T("Audio settings preset detected: Default preset"));
		}
		else if (pseta == 1) {
			if (_tcscmp(audiopath, _T("")) != 0) {
				CString ap = (LPCTSTR)audiopath;
				ap += _T("\\*.*");
				if (_tcscmp(aname, _T("")) != 0) {
					Utility->DeleteALLFiles(ap);
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
					Utility->DeleteALLFiles(vp);
				}
			}
		}

		if (PathFileExists(currentPath)) {
			OutputDebugString(_T("Path: '") + currentPath + _T("' file exists.\n"));
			DELETEPATH = currentPath;
			DELETEMAINCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
			pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			if (pDeleteThread)
			{
				pDeleteThread->m_pMainWnd = this;
				pDeleteThread->m_bAutoDelete = TRUE;
				if (DeleteFileThreadFlag != FALSE) {
					DeleteFileThreadFlag = FALSE;
				}
				pDeleteThread->ResumeThread();

				DELETEDIALOG DIALOG;
				DIALOG.DoModal();
			}
		}
		else {
			OutputDebugString(_T("Path: '") + currentPath + _T("' file doesn't exist.\n"));
		}

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
		this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
		this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
		this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
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
		MessageBox(ERROR_IMAGENOT2, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (waifu2x_param.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
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
		CString PRM = CURRENT_PATH + _T("\\Resources\\image2png \"") + IMAGEPATH + _T("\"");
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
		
		Utility->RunProcess(PRMT, SW_SHOW);

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

		Utility->CopyALLFiles(TIMAGEPATH, InPath);
		
		if (EXT == L"JPG") {
			CString filter = FILTER_JPEG;
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
				MessageBox(WARN_SAVECANCEL, WARN_CANCEL, MB_ICONWARNING | MB_OK);

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
				this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
				CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
				static1->InvalidateRect(NULL, 1);
				return;
			}
		}
		else if (EXT == L"PNG") {
			CString filter = FILTER_PNG;
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
				MessageBox(WARN_SAVECANCEL, WARN_CANCEL, MB_ICONWARNING | MB_OK);
				
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
				this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
				CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
				static1->InvalidateRect(NULL, 1);
				return;
			}
		}
		else if (EXT == L"WEBP") {
			CString filter = FILTER_WEBP;
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
				MessageBox(WARN_SAVECANCEL, WARN_CANCEL, MB_ICONWARNING | MB_OK);
				
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
				this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
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
			Utility->AfxReplaceStr(waifu2x_param, _T("$OutFile"), _T("\"$OutFile\""));
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
			Utility->AfxReplaceStr(waifu2x_param, _T("$InFile"), _T("\"$InFile\""));
			Utility->AfxReplaceStr(waifu2x_param, _T("$InFile"), InPathT);
		}

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

		pWaifu2xThread = AfxBeginThread(waifu2xThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		if (pWaifu2xThread)
		{
			pWaifu2xThread->m_pMainWnd = this;
			pWaifu2xThread->m_bAutoDelete = TRUE;
			pWaifu2xThread->ResumeThread();
			WaitForSingleObject(pWaifu2xThread->m_hThread, INFINITE);
		}

		POPUP->DestroyWindow();
		SAFE_DELETE(POPUP);

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
			MessageBox(INFO_SUCCESS, INFO_TITLE, MB_ICONINFORMATION | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			if (Waifu2xThreadFlag != FALSE) {
				Waifu2xThreadFlag = FALSE;
			}
			return;
		}
		else {
			SAFE_FREE(OutPathT);
			OutPath.Empty();
			MessageBox(ERROR_FAILED, ERROR_TITLE, MB_ICONERROR | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			if (Waifu2xThreadFlag != FALSE) {
				Waifu2xThreadFlag = FALSE;
			}
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
		MessageBox(ERROR_IMAGENOT, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	else if (waifu2x_param.IsEmpty() == TRUE)
	{
		MessageBox(ERROR_SETTINGS, ERROR_TITLE, MB_ICONERROR | MB_OK);
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
		binfo.lpszTitle = TEXT_SELOUTPUTFOLDER;
		binfo.ulFlags = BIF_RETURNONLYFSDIRS;
		binfo.lpfn = NULL;
		binfo.lParam = 0;
		binfo.iImage = 0;

		if ((idlist = SHBrowseForFolder(&binfo)) == NULL)
		{
			if (PathFileExists(currentPath)) {
				OutputDebugString(_T("Path: '") + currentPath + _T("' file exists.\n"));
				DELETEPATH = currentPath;
				DELETEMAINCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
				if (DELETEMAINCOUNT <= 500) {
					Utility->DeleteDirectory(DELETEPATH);
				}
				else {
					pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
					if (pDeleteThread)
					{
						pDeleteThread->m_pMainWnd = this;
						pDeleteThread->m_bAutoDelete = TRUE;
						if (DeleteFileThreadFlag != FALSE) {
							DeleteFileThreadFlag = FALSE;
						}
						pDeleteThread->ResumeThread();

						DELETEDIALOG DIALOG;
						DIALOG.DoModal();
					}
				}
			}
			else {
				OutputDebugString(_T("Path: '") + currentPath + _T("' file doesn't exist.\n"));
			}

			MessageBox(WARN_CANCEL, WARN_CANCEL_TITLE, MB_ICONWARNING | MB_OK);
			CoTaskMemFree(idlist);
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
			this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
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
				selectbox = MessageBox(WARN_EXIST, WARN_CONFIRM, MB_ICONWARNING | MB_YESNO);
				if (selectbox == IDYES) {
					OutPath = (LPCTSTR)dir;
					MultipleOutPath = (LPCTSTR)dir;
					CString del = _T("\\*.*");
					del = OutPath + del;

					if (PathFileExists(OutPath)) {
						OutputDebugString(_T("Path: '") + OutPath + _T("' file exists.\n"));
						DELETEPATH = del;
						DELETEMAINCOUNT = Utility->GetDirectoryFileCount(OutPath.GetString());
						if (DELETEMAINCOUNT <= 500) {
							Utility->DeleteALLFiles(DELETEPATH);
						}
						else {
							pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
							if (pDeleteThread)
							{
								pDeleteThread->m_pMainWnd = this;
								pDeleteThread->m_bAutoDelete = TRUE;
								if (DeleteFileThreadFlag != FALSE) {
									DeleteFileThreadFlag = FALSE;
								}
								pDeleteThread->ResumeThread();

								DELETEDIALOG DIALOG;
								DIALOG.DoModal();
							}
						}
					}
					else {
						OutputDebugString(_T("Path: '") + OutPath + _T("' file doesn't exist.\n"));
					}

					CoTaskMemFree(idlist);
				}
				else if (selectbox == IDNO) {
					CoTaskMemFree(idlist);
					return;
				}
			}
		}

		Utility->CopyALLFiles(TIMAGEPATH, InPath);

		CString PRM = CURRENT_PATH + _T("\\Resources\\image2png \"") + IMAGEPATH_M + _T("\"");
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
		
		Utility->RunProcess(PRMT, SW_SHOW);

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
			Utility->AfxReplaceStr(waifu2x_param, _T("$OutFile"), _T("\"$OutFile\""));
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
			Utility->AfxReplaceStr(waifu2x_param, _T("$InFile"), _T("\"$InFile\""));
			Utility->AfxReplaceStr(waifu2x_param, _T("$InFile"), InPathT);
		}

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
			if (SuspendFlag == TRUE) {
				while (pThread->m_Dlg.m_hWnd) {
					if (pThread->m_Dlg.m_hWnd == 0) {
						break;
					}
				}
				if (PathFileExists(currentPath)) {
					OutputDebugString(_T("Path: '") + currentPath + _T("' file exists.\n"));
					DELETEPATH = currentPath;
					DELETEMAINCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
					if (DELETEMAINCOUNT <= 500) {
						Utility->DeleteDirectory(DELETEPATH);
					}
					else {
						pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
						if (pDeleteThread)
						{
							pDeleteThread->m_pMainWnd = this;
							pDeleteThread->m_bAutoDelete = TRUE;
							if (DeleteFileThreadFlag != FALSE) {
								DeleteFileThreadFlag = FALSE;
							}
							pDeleteThread->ResumeThread();

							DELETEDIALOG DIALOG;
							DIALOG.DoModal();
						}
					}
				}
				else {
					OutputDebugString(_T("Path: '") + currentPath + _T("' file doesn't exist.\n"));
				}

				if (PathFileExists(OutPath)) {
					OutputDebugString(_T("Path: '") + OutPath + _T("' file exists.\n"));
					DELETEPATH = OutPath + _T("\\*.*");
					DELETEMAINCOUNT = Utility->GetDirectoryFileCount(OutPath.GetString());
					if (DELETEMAINCOUNT <= 500) {
						Utility->DeleteALLFiles(DELETEPATH);
					}
					else {
						pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
						if (pDeleteThread)
						{
							pDeleteThread->m_pMainWnd = this;
							pDeleteThread->m_bAutoDelete = TRUE;
							if (DeleteFileThreadFlag != FALSE) {
								DeleteFileThreadFlag = FALSE;
							}
							pDeleteThread->ResumeThread();

							DELETEDIALOG DIALOG;
							DIALOG.DoModal();
						}
					}
				}
				else {
					OutputDebugString(_T("Path: '") + OutPath + _T("' file doesn't exist.\n"));
				}

				MessageBox(WARN_ABORT2, INFO_TITLE, MB_ICONWARNING | MB_OK);
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
				this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
				CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
				static1->InvalidateRect(NULL, 1);
				SAFE_FREE(InPathT);
				SAFE_FREE(OutPathT);
				if (ImageUpScaleFlag != FALSE) {
					ImageUpScaleFlag = FALSE;
				}
				if (Waifu2xThreadFlag != FALSE) {
					Waifu2xThreadFlag = FALSE;
				}
				if (ProgressThreadFlag != FALSE) {
					ProgressThreadFlag = FALSE;
				}
				if (SuspendFlag != FALSE) {
					SuspendFlag = FALSE;
				}
				return;
			}
		}
		while (pThread->m_Dlg.m_hWnd) {
			if (pThread->m_Dlg.m_hWnd == 0) {
				break;
			}
		}

		if (ProgressThreadFlag != FALSE) {
			ProgressThreadFlag = FALSE;
		}
		if (ImageUpScaleFlag != FALSE) {
			ImageUpScaleFlag = FALSE;
		}
		if (SuspendFlag != FALSE) {
			SuspendFlag = FALSE;
		}

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

		if (PathFileExists(currentPath)) {
			OutputDebugString(_T("Path: '") + currentPath + _T("' file exists.\n"));
			DELETEPATH = currentPath;
			DELETEMAINCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
			if (DELETEMAINCOUNT <= 500) {
				Utility->DeleteDirectory(DELETEPATH);
			}
			else {
				pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
				if (pDeleteThread)
				{
					pDeleteThread->m_pMainWnd = this;
					pDeleteThread->m_bAutoDelete = TRUE;
					if (DeleteFileThreadFlag != FALSE) {
						DeleteFileThreadFlag = FALSE;
					}
					pDeleteThread->ResumeThread();

					DELETEDIALOG DIALOG;
					DIALOG.DoModal();
				}
			}
		}
		else {
			OutputDebugString(_T("Path: '") + currentPath + _T("' file doesn't exist.\n"));
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

		if (UpscaleExceptionFlag == 1) {
			UpscaleExceptionFlag = 0;
			SAFE_FREE(OutPathT);
			MessageBox(INFO_SUCCESS, INFO_TITLE, MB_ICONINFORMATION | MB_OK);
			this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
			this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
			CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
			static1->InvalidateRect(NULL, 1);
			if (Waifu2xThreadFlag != FALSE) {
				Waifu2xThreadFlag = FALSE;
			}
			return;
		}
		else {
			if (PathIsDirectoryEmpty(OutPathT)) {
				SAFE_FREE(OutPathT);
				MessageBox(ERROR_FAILED, ERROR_TITLE, MB_ICONERROR | MB_OK);
				this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
				CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
				static1->InvalidateRect(NULL, 1);
				if (Waifu2xThreadFlag != FALSE) {
					Waifu2xThreadFlag = FALSE;
				}
				return;
			}
			else {
				SAFE_FREE(OutPathT);
				MessageBox(INFO_SUCCESS, INFO_TITLE, MB_ICONINFORMATION | MB_OK);
				this->xv_Static_ReadStatus.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_File.SetWindowText(STATIC_NOTREAD);
				this->xv_Static_FilePath.SetWindowText(STATIC_NOTREAD);
				CStatic* static1 = (CStatic*)GetDlgItem(IDC_STATIC_READSTATUS);
				static1->InvalidateRect(NULL, 1);
				if (Waifu2xThreadFlag != FALSE) {
					Waifu2xThreadFlag = FALSE;
				}
				return;
			}
		}
	}
}


void Cwaifu2xncnnvulkanDlg::OnReadImage()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnReadImage\n"));
	POPUPDlg* POPUP = new POPUPDlg;
	CString filter = FILTER_IMAGE;
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
		CString NumFinal = Num + _T(" ( ") + fsizeCS + TEXT_BYTES;
		this->xv_Static_ReadStatus.SetWindowText(STATIC_READED);
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
	binfo.lpszTitle = TEXT_SELFOLDER;
	binfo.ulFlags = BIF_RETURNONLYFSDIRS;
	binfo.lpfn = NULL;
	binfo.lParam = 0;
	binfo.iImage = 0;

	if ((idlist = SHBrowseForFolder(&binfo)) == NULL)
	{
		MessageBox(WARN_CANCEL, WARN_CANCEL_TITLE, MB_ICONWARNING | MB_OK);
		CoTaskMemFree(idlist);
		SAFE_DELETE(POPUP);
	}
	else
	{
		SHGetPathFromIDList(idlist, (LPWSTR)dir);
		if (PathIsDirectoryEmpty((LPCTSTR)dir)) {
			MessageBox(ERROR_FOLDER, ERROR_TITLE, MB_ICONERROR | MB_OK);
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
		CString NumFinal = Num + TEXT_DIRECTORYS;
		this->xv_Static_ReadStatus.SetWindowText(STATIC_READED);
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
	CString filter = FILTER_MOVIE;
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
		CString NumFinal = Num + _T(" ( ") + fsizeCS + TEXT_BYTES;
		this->xv_Static_ReadStatus.SetWindowText(STATIC_READED);
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

	if (PathFileExists(currentPath)) {
		OutputDebugString(_T("Path: '") + currentPath + _T("' file exists.\n"));
		DELETEPATH = currentPath;
		DELETEMAINCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
		pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		if (pDeleteThread)
		{
			pDeleteThread->m_pMainWnd = this;
			pDeleteThread->m_bAutoDelete = TRUE;
			if (DeleteFileThreadFlag != FALSE) {
				DeleteFileThreadFlag = FALSE;
			}
			pDeleteThread->ResumeThread();

			DELETEDIALOG DIALOG;
			DIALOG.DoModal();
		}
	}
	else {
		OutputDebugString(_T("Path: '") + currentPath + _T("' file doesn't exist.\n"));
	}

	if (Utility->DeleteDirectory(_T(".\\Resources\\tmp"))) {
		OutputDebugString(_T("DeleteDirectory successed.\n"));
	}
	else {
		OutputDebugString(_T("DeleteDirectory failed.\n"));
	}

	return;
}


void Cwaifu2xncnnvulkanDlg::OnVideoresize()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	OutputDebugString(_T("Menu clicked: OnVideoresize\n"));
	POPUPDlg* POPUP = new POPUPDlg;
	CFileFind find;
	CString filter = FILTER_MOVIE;
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	INT_PTR filedialog = selDlg.DoModal();
	if (filedialog == IDOK) {
		CString RV = selDlg.GetPathName();
		CString filter = FILTER_MOVIE;
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

			SAFE_DELETE(POPUP);
			VIDEORESIZE* DIALOG = new VIDEORESIZE;
			INT_PTR ret = DIALOG->DoModal();
			if (ret == IDOK) {
				CString PRM = DIALOG->FINAL;
				Utility->AfxReplaceStr(PRM, L"ffmpeg", CURRENT_PATH + _T("\\Resources\\ffmpeg\\ffmpeg.exe"));
				Utility->AfxReplaceStr(PRM, L"$InFile", L"\"$InFile\"");
				Utility->AfxReplaceStr(PRM, L"$InFile", RV);
				Utility->AfxReplaceStr(PRM, L"$OutFile", L"\"$OutFile\"");
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
					MessageBox(INFO_RESIZE, INFO_TITLE, MB_ICONINFORMATION | MB_OK);
					return;
				}
				else {
					SAFE_FREE(lpPath);
					SAFE_DELETE(DIALOG);
					MessageBox(ERROR_RESIZE, ERROR_TITLE, MB_ICONERROR | MB_OK);
					return;
				}
			}
			else if (ret == IDCANCEL) {
				MessageBox(WARN_CANCEL, WARN_CANCEL_TITLE, MB_ICONWARNING | MB_OK);
				SAFE_DELETE(DIALOG);
				return;
			}
			else {
				SAFE_DELETE(DIALOG);
				return;
			}
		}
		else if (file == IDCANCEL) {
			MessageBox(WARN_CANCEL, WARN_CANCEL_TITLE, MB_ICONWARNING | MB_OK);
			SAFE_DELETE(POPUP);
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
	CString filter = FILTER_MOVIE;
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	intptr_t filedialog = selDlg.DoModal();
	if (filedialog == IDOK) {
		CString RV = selDlg.GetPathName();
		CString filter = FILTER_SOUND;
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
			PRM = CURRENT_PATH + _T("\\Resources\\ffmpeg\\ffmpeg.exe -i \"") + RV + _T("\" -vn -acodec copy -y \"") + lpPath + _T("\"");
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
				MessageBox(INFO_SOUND, INFO_TITLE, MB_ICONINFORMATION | MB_OK);
				SAFE_DELETE(POPUP);
				return;
			}
			else {
				SAFE_FREE(lpPath);
				MessageBox(ERROR_SOUND, ERROR_TITLE, MB_ICONERROR | MB_OK);
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

UINT Cwaifu2xncnnvulkanDlg::DeleteMainThread(LPVOID pParam)
{
	Cwaifu2xncnnvulkanDlg* pFileView = dynamic_cast<Cwaifu2xncnnvulkanDlg*>(reinterpret_cast<CWnd*>(pParam));
	if (pFileView)
	{
		pFileView->DeleteMainThread();
	}
	return 0;
}

void Cwaifu2xncnnvulkanDlg::waifu2xThread()
{
	OutputDebugString(_T("waifu2x Thread Started.\n"));

	Utility->AfxReplaceStr(waifu2x_param, L"waifu2x-ncnn-vulkan -i", CURRENT_PATH + L"\\Resources\\waifu2x-ncnn-vulkan\\waifu2x-ncnn-vulkan.exe -i");
	TCHAR* lpPath = (TCHAR*)malloc(sizeof(TCHAR) * 512); // TCHAR配列をmallocで確保する
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

	CString ffmpegDir = CURRENT_PATH + L"\\Resources\\ffmpeg\\ffmpeg.exe";
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
	Utility->AfxReplaceStr(ffmpeg_audioparam, L"$InFile", L"\"$InFile\"");
	Utility->AfxReplaceStr(ffmpeg_audioparam, L"$InFile", InFile);
	Utility->AfxReplaceStr(ffmpeg_audioparam, L"$OutFile", L"\"$OutFile\"");
	Utility->AfxReplaceStr(ffmpeg_audioparam, L"$OutFile", audioOut);
	Utility->AfxReplaceStr(ffmpeg_videoparam, L"ffmpeg", ffmpegDir);
	Utility->AfxReplaceStr(ffmpeg_videoparam, L"$InFile", L"\"$InFile\"");
	Utility->AfxReplaceStr(ffmpeg_videoparam, L"$InFile", InFile);
	Utility->AfxReplaceStr(ffmpeg_videoparam, L"$OutFile", L"\"$OutFile\"");
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
	if (pWaifu2xThread)
	{
		pWaifu2xThread->m_pMainWnd = this;
		pWaifu2xThread->m_bAutoDelete = TRUE;
		pWaifu2xThread->ResumeThread();
	}

	while (ProgressThreadFlag != TRUE) {
		if (ImageUpScaleFlag == TRUE) {
			UPSCALE_COUNT = Utility->GetDirectoryFileCount(MultipleOutPath.GetString());
		}
		else {
			UPSCALE_COUNT = Utility->GetDirectoryFileCount(UPSCALE_PATH.GetString());
		}
		if (ProgressThreadFlag == TRUE) {
			break;
		}
		if (SuspendFlag == TRUE) {
			break;
		}
		else {
			continue;
		}
	}

	if (SuspendFlag == TRUE) {
		this->PostMessage(WM_USER_COMPLETE_WAIFU2X_THREAD);
	}
	this->PostMessage(WM_USER_COMPLETE_COUNT_THREAD);
	OutputDebugString(_T("Count Thread Ended.\n"));
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
	OutputDebugString(_T("DLThread Ended.\n"));
}

void Cwaifu2xncnnvulkanDlg::DLCountThread()
{
	OutputDebugString(_T("DLCountThread Started.\n"));

	pDLThread = AfxBeginThread(DLThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	ASSERT(pDLThread);
	if (pDLThread)
	{
		pDLThread->m_pMainWnd = this;
		pDLThread->m_bAutoDelete = TRUE;
		pDLThread->ResumeThread();
	}

	while (DLThreadFlag != TRUE) {
		DLCount = Utility->GetFileSizeStat(_T(".\\Resources\\ffmpeg.zip"));
		if (DLThreadFlag == TRUE) {
			break;
		}
		else {
			continue;
		}
	}

	this->PostMessage(WM_USER_COMPLETE_DOWNLOAD_LOAD_XML);
	OutputDebugString(_T("DLCountThread Ended.\n"));
}

void Cwaifu2xncnnvulkanDlg::DeleteThread()
{
	OutputDebugString(_T("DeleteThread Started.\n"));
	pDeleteMainThread = AfxBeginThread(DeleteMainThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	if (pDeleteMainThread)
	{
		pDeleteMainThread->m_pMainWnd = this;
		pDeleteMainThread->m_bAutoDelete = TRUE;
		pDeleteMainThread->ResumeThread();
	}

	while (DeleteFileThreadFlag != TRUE)
	{
		DELETESUBCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
		DELETECURCOUNT = DELETEMAINCOUNT - DELETESUBCOUNT;
		if (DeleteFileThreadFlag == TRUE) {
			break;
		}
		else {
			continue;
		}
	}

	this->PostMessage(WM_USER_COMPLETE_DELETE_LOAD_XML);
	OutputDebugString(_T("DeleteThread Ended.\n"));
}

void Cwaifu2xncnnvulkanDlg::DeleteMainThread()
{
	OutputDebugString(_T("DeleteMainThread Started.\n"));

	if (DELETEPATH.Find(_T("*")) == -1) {
		Utility->DeleteDirectory(DELETEPATH);
	}
	else {
		Utility->DeleteALLFiles(DELETEPATH);
	}

	this->PostMessage(WM_USER_COMPLETE_DELETE_MAIN_LOAD_XML);
	OutputDebugString(_T("DeleteMainThread Ended.\n"));
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteWaifu2xThread\n"));
	Utility->TerminateExeName(_T("waifu2x-ncnn-vulkan.exe"));
	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteFFmpegThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteFFmpegThread\n"));

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xCountThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteWaifu2xCountThread\n"));
	UPSCALE_COUNT = 0;
	ProgressThreadFlag = FALSE;

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteWaifu2xCountDlgThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteWaifu2xCountDlgThread\n"));

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteDLThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteDLThread\n"));

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteDLCountThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteDLCountThread\n"));
	DLCount = 0;
	DLThreadFlag = FALSE;

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteDeleteThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteDeleteThread\n"));
	DELETEMAINCOUNT = 0;
	DELETESUBCOUNT = 0;
	DELETECURCOUNT = 0;

	return 0;
}

afx_msg LRESULT Cwaifu2xncnnvulkanDlg::OnCompleteDeleteMainThread(WPARAM wParam, LPARAM lParam)
{
	OutputDebugString(_T("Thread: OnCompleteDeleteMainThread\n"));

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
	UINT Lang;
	Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");

	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		switch (id)
		{
		case IDC_STATIC_READSTATUS:
			if (Lang == 0) {
				if (GetDlgItem(IDC_STATIC_READSTATUS)->GetWindowTextLength() == 6) {
					pDC->SetTextColor(RGB(0, 200, 0));
					pDC->SetBkMode(TRANSPARENT);
				}
				else if (GetDlgItem(IDC_STATIC_READSTATUS)->GetWindowTextLength() == 12) {
					pDC->SetTextColor(RGB(255, 0, 0));
					pDC->SetBkMode(TRANSPARENT);
				}
			}
			else if (Lang == 1) {
				if (GetDlgItem(IDC_STATIC_READSTATUS)->GetWindowTextLength() == 12) {
					pDC->SetTextColor(RGB(0, 200, 0));
					pDC->SetBkMode(TRANSPARENT);
				}
				else if (GetDlgItem(IDC_STATIC_READSTATUS)->GetWindowTextLength() == 16) {
					pDC->SetTextColor(RGB(255, 0, 0));
					pDC->SetBkMode(TRANSPARENT);
				}
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

	if (_tcscmp(audiopath, _T("")) != 0) {
		CString ap = (LPCTSTR)audiopath;
		ap += _T("\\*.*");
		if (_tcscmp(aname, _T("")) != 0) {
			Utility->DeleteALLFiles(ap);
		}
	}
	if (_tcscmp(videopath, _T("")) != 0) {
		CString vp = (LPCTSTR)videopath;
		vp += _T("\\*.*");
		if (_tcscmp(vname, _T("")) != 0) {
			Utility->DeleteALLFiles(vp);
		}
	}

	if (PathFileExists(currentPath)) {
		OutputDebugString(_T("Path: '") + currentPath + _T("' file exists.\n"));
		DELETEPATH = currentPath;
		DELETEMAINCOUNT = Utility->GetDirectoryFileCount(DELETEPATH.GetString());
		pDeleteThread = AfxBeginThread(DeleteThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
		if (pDeleteThread)
		{
			pDeleteThread->m_pMainWnd = this;
			pDeleteThread->m_bAutoDelete = TRUE;
			if (DeleteFileThreadFlag != FALSE) {
				DeleteFileThreadFlag = FALSE;
			}
			pDeleteThread->ResumeThread();

			DELETEDIALOG DIALOG;
			DIALOG.DoModal();
		}
	}
	else {
		OutputDebugString(_T("Path: '") + currentPath + _T("' file doesn't exist.\n"));
	}

	FreeLibrary(Core->Lang_hinst);
	SAFE_DELETE(CORE_FUNC);
	SAFE_DELETE(Waifu2x->fnt1);
	SAFE_DELETE(MAINSTR_FUNC);
	SAFE_DELETE(VERSIONSTR_FUNC);
	SAFE_DELETE(COREUTIL_FUNC);
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
		MessageBox(ERROR_UPDATE, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	CString CurrentVersion = Utility->AppCurrentVersionCheck();
	if (CurrentVersion == _T("")) {
		MessageBox(ERROR_UPDATE, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
	if (LatestVersion == CurrentVersion) {
		MessageBox(UPDATE_LATEST + LatestVersion + _T("\n") + UPDATE_CURRENT + CurrentVersion + _T("\n") + UPDATE_APP_LATEST, INFO_TITLE, MB_ICONINFORMATION | MB_OK);
		return;
	}
	else if (LatestVersion > CurrentVersion) {
		UINT ret;
		ret = MessageBox(UPDATE_LATEST + LatestVersion + _T("\n") + UPDATE_CURRENT + CurrentVersion + _T("\n") + UPDATE_APP_CONFIRM, UPDATE_APP_TITLE, MB_ICONINFORMATION | MB_YESNOCANCEL);
		if (ret == IDYES) {
			ShellExecute(NULL, _T("open"), _T("https://xyle-official.com/tools/waifu2x/#latest-release"), NULL, NULL, SW_SHOWNORMAL);
			return;
		}
		else if (ret == IDNO) {
			return;
		}
		else if (ret == IDCANCEL) {
			MessageBox(WARN_CANCEL, WARN_CANCEL_TITLE, MB_ICONWARNING | MB_OK);
		}
		else {
			return;
		}
	}
	else {
		MessageBox(ERROR_UPDATE_ELSE, ERROR_TITLE, MB_ICONERROR | MB_OK);
		return;
	}
}

void Cwaifu2xncnnvulkanDlg::OnLanguageJapanese()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	UINT Lang;
	Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");
	if (Lang == 0) {
		MessageBox(_T("既に言語が'日本語'に設定されています。"), _T("エラー"), MB_ICONERROR | MB_OK);
		return;
	}

	this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_CHECKED);
	if (this->GetMenu()->GetMenuState(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND) == MF_CHECKED) {
		this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_UNCHECKED);
	}

	WritePrivateProfileString(L"LANGUAGE", L"0x0000", L"0", L".\\settings.ini");

	UINT ret;
	ret = MessageBox(_T("言語設定が変更されました。変更にはアプリケーションを再起動する必要があります。\n続行しますか？"), _T("再起動の確認"), MB_ICONINFORMATION | MB_YESNO);
	if (ret == IDYES) {
		TCHAR strPath[MAX_PATH];
		::GetModuleFileName(AfxGetInstanceHandle(), strPath, sizeof(strPath) / sizeof(TCHAR));
		TCHAR strCurDir[MAX_PATH];
		::GetCurrentDirectory(sizeof(strCurDir) / sizeof(TCHAR), strCurDir);
		DWORD dwProcessId = ::GetCurrentProcessId();
		CString strCmdLine;
		strCmdLine.Format(_T("/waitterminate %u"), dwProcessId);
		::ShellExecute(NULL, NULL, strPath, strCmdLine, strCurDir, SW_SHOWNORMAL);
		PostQuitMessage(0);
		return;
	}
	else if (ret == IDNO) {
		Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");
		if (Lang == 0) {
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_CHECKED);
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_UNCHECKED);
			return;
		}
		else if (Lang == 1) {
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_UNCHECKED);
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_CHECKED);
			return;
		}
	}
	else {
		Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");
		if (Lang == 0) {
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_CHECKED);
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_UNCHECKED);
			return;
		}
		else if (Lang == 1) {
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_UNCHECKED);
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_CHECKED);
			return;
		}
	}
}


void Cwaifu2xncnnvulkanDlg::OnLanguageEnglish()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	UINT Lang;
	Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");
	if (Lang == 1) {
		MessageBox(_T("The language has already been set to 'English'."), _T("Error"), MB_ICONERROR | MB_OK);
		return;
	}

	this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_CHECKED);
	if (this->GetMenu()->GetMenuState(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND) == MF_CHECKED) {
		this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_UNCHECKED);
	}

	WritePrivateProfileString(L"LANGUAGE", L"0x0000", L"1", L".\\settings.ini");

	UINT ret;
	ret = MessageBox(_T("The language setting has been changed, You will need to restart the application to make changes.\nDo you want to continue?"), _T("Confirm reboot"), MB_ICONINFORMATION | MB_YESNO);
	if (ret == IDYES) {
		TCHAR strPath[MAX_PATH];
		::GetModuleFileName(AfxGetInstanceHandle(), strPath, sizeof(strPath) / sizeof(TCHAR));
		TCHAR strCurDir[MAX_PATH];
		::GetCurrentDirectory(sizeof(strCurDir) / sizeof(TCHAR), strCurDir);
		DWORD dwProcessId = ::GetCurrentProcessId();
		CString strCmdLine;
		strCmdLine.Format(_T("/waitterminate %u"), dwProcessId);
		::ShellExecute(NULL, NULL, strPath, strCmdLine, strCurDir, SW_SHOWNORMAL);
		PostQuitMessage(0);
		return;
	}
	else if (ret == IDNO) {
		Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");
		if (Lang == 0) {
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_CHECKED);
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_UNCHECKED);
			return;
		}
		else if (Lang == 1) {
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_UNCHECKED);
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_CHECKED);
			return;
		}
	}
	else {
		Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");
		if (Lang == 0) {
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_CHECKED);
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_UNCHECKED);
			return;
		}
		else if (Lang == 1) {
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_UNCHECKED);
			this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_CHECKED);
			return;
		}
	}
}


void Cwaifu2xncnnvulkanDlg::LoadDlgStr()
{
	UINT Lang;
	Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");
	if (Lang == 0) {
		Core->LoadJPNLangLibrary();
		this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_UNCHECKED);
		this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_CHECKED);
	}
	else if (Lang == 1) {
		Core->LoadENGLangLibrary();
		this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_CHECKED);
		this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_UNCHECKED);
	}
	else {
		Core->LoadJPNLangLibrary();
		this->GetMenu()->CheckMenuItem(ID_LANGUAGE_ENGLISH, MF_BYCOMMAND | MF_UNCHECKED);
		this->GetMenu()->CheckMenuItem(ID_LANGUAGE_JAPANESE, MF_BYCOMMAND | MF_CHECKED);
		WritePrivateProfileString(L"LANGUAGE", L"0x0000", L"0", L".\\settings.ini");
	}

	ASSERT(Core->Lang_hinst);

	LoadString(Core->Lang_hinst, IDS_ERROR_TITLE, (LPTSTR)ERROR_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_INSTANCE, (LPTSTR)ERROR_INSTANCE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_EXCEPTION, (LPTSTR)ERROR_EXCEPTION, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_SERVER, (LPTSTR)ERROR_SERVER, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_DOWNLOAD, (LPTSTR)ERROR_DOWNLOAD, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_FFMPEG, (LPTSTR)ERROR_FFMPEG, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_WAIFU2X, (LPTSTR)ERROR_WAIFU2X, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_IMAGE2PNG, (LPTSTR)ERROR_IMAGE2PNG, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_IMAGENOT, (LPTSTR)ERROR_IMAGENOT, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_IMAGENOT2, (LPTSTR)ERROR_IMAGENOT2, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_ABORT, (LPTSTR)ERROR_ABORT, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_APPNOTFOUND, (LPTSTR)ERROR_APPNOTFOUND, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_NOTSELECTED, (LPTSTR)ERROR_NOTSELECTED, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_SETTINGS, (LPTSTR)ERROR_SETTINGS, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_UPSCALE, (LPTSTR)ERROR_UPSCALE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_UPSCALE_TITLE, (LPTSTR)ERROR_UPSCALE_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_REUPSCALE, (LPTSTR)ERROR_REUPSCALE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_REUPSCALE_TITLE, (LPTSTR)ERROR_REUPSCALE_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_FAILED, (LPTSTR)ERROR_FAILED, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_FOLDER, (LPTSTR)ERROR_FOLDER, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_RESIZE, (LPTSTR)ERROR_RESIZE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_SOUND, (LPTSTR)ERROR_SOUND, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_UPDATE, (LPTSTR)ERROR_UPDATE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_UPDATE_ELSE, (LPTSTR)ERROR_UPDATE_ELSE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_BLOCKSIZE_LARGE, (LPTSTR)ERROR_BLOCKSIZE_LARGE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_BLOCKSIZE_SHORT, (LPTSTR)ERROR_BLOCKSIZE_SHORT, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_ABORT, (LPTSTR)WARN_ABORT, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_ABORT2, (LPTSTR)WARN_ABORT2, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_CANCEL, (LPTSTR)WARN_CANCEL, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_CANCEL_TITLE, (LPTSTR)WARN_CANCEL_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_SAVECANCEL, (LPTSTR)WARN_SAVECANCEL, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_CONFIRM, (LPTSTR)WARN_CONFIRM, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_EXIST, (LPTSTR)WARN_EXIST, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_ADVANCED, (LPTSTR)WARN_ADVANCED, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_CAUTION_TITLE, (LPTSTR)WARN_CAUTION_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_INFO_TITLE, (LPTSTR)INFO_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_INFO_DOWNLOAD, (LPTSTR)INFO_DOWNLOAD, 256);
	LoadString(Core->Lang_hinst, IDS_INFO_UPSCALE, (LPTSTR)INFO_UPSCALE, 256);
	LoadString(Core->Lang_hinst, IDS_INFO_UPSCALE_TITLE, (LPTSTR)INFO_UPSCALE_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_INFO_REUPSCALE, (LPTSTR)INFO_REUPSCALE, 256);
	LoadString(Core->Lang_hinst, IDS_INFO_REUPSCALE_TITLE, (LPTSTR)INFO_REUPSCALE_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_INFO_SUCCESS, (LPTSTR)INFO_SUCCESS, 256);
	LoadString(Core->Lang_hinst, IDS_INFO_RESIZE, (LPTSTR)INFO_RESIZE, 256);
	LoadString(Core->Lang_hinst, IDS_INFO_SOUND, (LPTSTR)INFO_SOUND, 256);
	LoadString(Core->Lang_hinst, IDS_UPDATE_LATEST, (LPTSTR)UPDATE_LATEST, 256);
	LoadString(Core->Lang_hinst, IDS_UPDATE_CURRENT, (LPTSTR)UPDATE_CURRENT, 256);
	LoadString(Core->Lang_hinst, IDS_UPDATE_FFMPEG, (LPTSTR)UPDATE_FFMPEG, 256);
	LoadString(Core->Lang_hinst, IDS_UPDATE_FFMPEG_TITLE, (LPTSTR)UPDATE_FFMPEG_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_UPDATE_APP_LATEST, (LPTSTR)UPDATE_APP_LATEST, 256);
	LoadString(Core->Lang_hinst, IDS_UPDATE_APP_CONFIRM, (LPTSTR)UPDATE_APP_CONFIRM, 256);
	LoadString(Core->Lang_hinst, IDS_UPDATE_APP_TITLE, (LPTSTR)UPDATE_APP_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_TEXT_SELFOLDER, (LPTSTR)TEXT_SELFOLDER, 256);
	LoadString(Core->Lang_hinst, IDS_TEXT_UNKNOWN, (LPTSTR)TEXT_UNKNOWN, 256);
	LoadString(Core->Lang_hinst, IDS_TEXT_DIRECTORYS, (LPTSTR)TEXT_DIRECTORYS, 256);
	LoadString(Core->Lang_hinst, IDS_TEXT_BYTES, (LPTSTR)TEXT_BYTES, 256);
	LoadString(Core->Lang_hinst, IDS_TEXT_SELOUTPUTFOLDER, (LPTSTR)TEXT_SELOUTPUTFOLDER, 256);
	LoadString(Core->Lang_hinst, IDS_FILTER_IMAGE, (LPTSTR)FILTER_IMAGE, 530);
	LoadString(Core->Lang_hinst, IDS_FILTER_MOVIE, (LPTSTR)FILTER_MOVIE, 256);
	LoadString(Core->Lang_hinst, IDS_FILTER_SOUND, (LPTSTR)FILTER_SOUND, 256);
	LoadString(Core->Lang_hinst, IDS_FILTER_JPEG, (LPTSTR)FILTER_JPEG, 256);
	LoadString(Core->Lang_hinst, IDS_FILTER_PNG, (LPTSTR)FILTER_PNG, 256);
	LoadString(Core->Lang_hinst, IDS_FILTER_WEBP, (LPTSTR)FILTER_WEBP, 256);

	LoadString(Core->Lang_hinst, IDS_MENU_FILE, (LPTSTR)MENU_FILE, 256);
	this->GetMenu()->ModifyMenu(0, MF_BYPOSITION | MF_STRING, 0, MENU_FILE);
	LoadString(Core->Lang_hinst, IDS_MENU_CONFIG, (LPTSTR)MENU_CONFIG, 256);
	this->GetMenu()->ModifyMenu(1, MF_BYPOSITION | MF_STRING, 1, MENU_CONFIG);
	LoadString(Core->Lang_hinst, IDS_MENU_TOOL, (LPTSTR)MENU_TOOL, 256);
	this->GetMenu()->ModifyMenu(2, MF_BYPOSITION | MF_STRING, 2, MENU_TOOL);
	LoadString(Core->Lang_hinst, IDS_MENU_HELP, (LPTSTR)MENU_HELP, 256);
	this->GetMenu()->ModifyMenu(3, MF_BYPOSITION | MF_STRING, 3, MENU_HELP);

	LoadString(Core->Lang_hinst, IDS_MENULIST_READFILE, (LPTSTR)MENULIST_READFILE, 256);
	this->GetMenu()->GetSubMenu(0)->ModifyMenu(0, MF_BYPOSITION | MF_STRING, 0, MENULIST_READFILE);
	LoadString(Core->Lang_hinst, IDS_MENULIST_IMG, (LPTSTR)MENULIST_IMG, 256);
	this->GetMenu()->GetSubMenu(0)->GetSubMenu(0)->ModifyMenu(0, MF_BYPOSITION | MF_STRING, ID_READ_IMAGE, MENULIST_IMG);
	LoadString(Core->Lang_hinst, IDS_MENULIST_IMGM, (LPTSTR)MENULIST_IMGM, 256);
	this->GetMenu()->GetSubMenu(0)->GetSubMenu(0)->ModifyMenu(1, MF_BYPOSITION | MF_STRING, ID_READ_IMAGE_MULTI, MENULIST_IMGM);
	LoadString(Core->Lang_hinst, IDS_MENULIST_MOV, (LPTSTR)MENULIST_MOV, 256);
	this->GetMenu()->GetSubMenu(0)->GetSubMenu(0)->ModifyMenu(2, MF_BYPOSITION | MF_STRING, ID_READ_VIDEO, MENULIST_MOV);
	LoadString(Core->Lang_hinst, IDS_MENULIST_TEMPFILE, (LPTSTR)MENULIST_TEMPFILE, 256);
	this->GetMenu()->GetSubMenu(0)->ModifyMenu(1, MF_BYPOSITION | MF_STRING, ID_FILE_ALLDELETE, MENULIST_TEMPFILE);
	LoadString(Core->Lang_hinst, IDS_MENULIST_ENDAPP, (LPTSTR)MENULIST_ENDAPP, 256);
	this->GetMenu()->GetSubMenu(0)->ModifyMenu(3, MF_BYPOSITION | MF_STRING, ID_END_APP, MENULIST_ENDAPP);
	LoadString(Core->Lang_hinst, IDS_MENULIST_WAIFU2X, (LPTSTR)MENULIST_WAIFU2X, 256);
	this->GetMenu()->GetSubMenu(1)->ModifyMenu(2, MF_BYPOSITION | MF_STRING, ID_SETTINGS, MENULIST_WAIFU2X);
	LoadString(Core->Lang_hinst, IDS_MENULIST_FFMPEG, (LPTSTR)MENULIST_FFMPEG, 256);
	this->GetMenu()->GetSubMenu(1)->ModifyMenu(3, MF_BYPOSITION | MF_STRING, 3, MENULIST_FFMPEG);
	LoadString(Core->Lang_hinst, IDS_MENULIST_FF_OUT, (LPTSTR)MENULIST_FF_OUT, 256);
	this->GetMenu()->GetSubMenu(1)->GetSubMenu(3)->ModifyMenu(0, MF_BYPOSITION | MF_STRING, ID_FFMPEG_SETTING, MENULIST_FF_OUT);
	LoadString(Core->Lang_hinst, IDS_MENULIST_FF_VID, (LPTSTR)MENULIST_FF_VID, 256);
	this->GetMenu()->GetSubMenu(1)->GetSubMenu(3)->ModifyMenu(1, MF_BYPOSITION | MF_STRING, ID_FFMPEG_VIDEOSETTINGS, MENULIST_FF_VID);
	LoadString(Core->Lang_hinst, IDS_MENULIST_FF_AUD, (LPTSTR)MENULIST_FF_AUD, 256);
	this->GetMenu()->GetSubMenu(1)->GetSubMenu(3)->ModifyMenu(2, MF_BYPOSITION | MF_STRING, ID_FFMPEG_AUDIOSETTINGS, MENULIST_FF_AUD);
	LoadString(Core->Lang_hinst, IDS_MENULIST_RESIZE, (LPTSTR)MENULIST_RESIZE, 256);
	this->GetMenu()->GetSubMenu(2)->GetSubMenu(0)->ModifyMenu(0, MF_BYPOSITION | MF_STRING, ID_VIDEORESIZE, MENULIST_RESIZE);
	LoadString(Core->Lang_hinst, IDS_MENULIST_SOUND, (LPTSTR)MENULIST_SOUND, 256);
	this->GetMenu()->GetSubMenu(2)->GetSubMenu(0)->ModifyMenu(1, MF_BYPOSITION | MF_STRING, ID_VIDEOAUDIOEXPORT, MENULIST_SOUND);
	LoadString(Core->Lang_hinst, IDS_MENULIST_UPDATE, (LPTSTR)MENULIST_UPDATE, 256);
	this->GetMenu()->GetSubMenu(3)->ModifyMenu(2, MF_BYPOSITION | MF_STRING, ID_UPDATECHECK, MENULIST_UPDATE);

	LoadString(Core->Lang_hinst, IDS_STATIC_GRP_FILE, (LPTSTR)STATIC_GRP_FILE, 256);
	GetDlgItem(IDC_STATIC_GRP1)->SetWindowText(STATIC_GRP_FILE);
	LoadString(Core->Lang_hinst, IDS_STATIC_GRP_IMG, (LPTSTR)STATIC_GRP_IMG, 256);
	GetDlgItem(IDC_STATIC_GRP2)->SetWindowText(STATIC_GRP_IMG);
	LoadString(Core->Lang_hinst, IDS_STATIC_GRP_IMGM, (LPTSTR)STATIC_GRP_IMGM, 256);
	GetDlgItem(IDC_STATIC_GRP3)->SetWindowText(STATIC_GRP_IMGM);
	LoadString(Core->Lang_hinst, IDS_STATIC_GRP_VID, (LPTSTR)STATIC_GRP_VID, 256);
	GetDlgItem(IDC_STATIC_GRP4)->SetWindowText(STATIC_GRP_VID);
	LoadString(Core->Lang_hinst, IDS_STATIC_GRP_INFO, (LPTSTR)STATIC_GRP_INFO, 256);
	GetDlgItem(IDC_STATIC_GRP5)->SetWindowText(STATIC_GRP_INFO);

	LoadString(Core->Lang_hinst, IDS_STATIC_HEADER, (LPTSTR)STATIC_HEADER, 256);
	GetDlgItem(IDC_STATIC_HEAD)->SetWindowText(STATIC_HEADER);
	LoadString(Core->Lang_hinst, IDS_STATIC_FILEREAD, (LPTSTR)STATIC_FILEREAD, 256);
	GetDlgItem(IDC_STATIC_READ)->SetWindowText(STATIC_FILEREAD);
	LoadString(Core->Lang_hinst, IDS_STATIC_FILENAME, (LPTSTR)STATIC_FILENAME, 256);
	GetDlgItem(IDC_STATIC_FL)->SetWindowText(STATIC_FILENAME);
	LoadString(Core->Lang_hinst, IDS_STATIC_FILEPATH, (LPTSTR)STATIC_FILEPATH, 256);
	GetDlgItem(IDC_STATIC_FP)->SetWindowText(STATIC_FILEPATH);
	LoadString(Core->Lang_hinst, IDS_STATIC_NOTREAD, (LPTSTR)STATIC_NOTREAD, 256);
	GetDlgItem(IDC_STATIC_READSTATUS)->SetWindowText(STATIC_NOTREAD);
	GetDlgItem(IDC_STATIC_FILE)->SetWindowText(STATIC_NOTREAD);
	GetDlgItem(IDC_STATIC_FILEPATH)->SetWindowText(STATIC_NOTREAD);
	LoadString(Core->Lang_hinst, IDS_STATIC_READED, (LPTSTR)STATIC_READED, 256);
	LoadString(Core->Lang_hinst, IDS_STATIC_CPU, (LPTSTR)STATIC_CPU, 256);
	GetDlgItem(IDC_STATIC_CP)->SetWindowText(STATIC_CPU);
	LoadString(Core->Lang_hinst, IDS_STATIC_GPU, (LPTSTR)STATIC_GPU, 256);
	GetDlgItem(IDC_STATIC_GP)->SetWindowText(STATIC_GPU);

	LoadString(Core->Lang_hinst, IDS_BTN_UPSCALE, (LPTSTR)BTN_UPSCALE, 256);
	GetDlgItem(IDC_BUTTON6)->SetWindowText(BTN_UPSCALE);
	GetDlgItem(IDC_BUTTON1)->SetWindowText(BTN_UPSCALE);
	LoadString(Core->Lang_hinst, IDS_BTN_REUPSCALE, (LPTSTR)BTN_REUPSCALE, 256);
	GetDlgItem(IDC_BUTTON3)->SetWindowText(BTN_REUPSCALE);
	LoadString(Core->Lang_hinst, IDS_BTN_MULTIUPSCALE, (LPTSTR)BTN_MULTIUPSCALE, 256);
	GetDlgItem(IDC_BUTTON2)->SetWindowText(BTN_MULTIUPSCALE);
	LoadString(Core->Lang_hinst, IDS_BTN_FINALENC, (LPTSTR)BTN_FINALENC, 256);
	GetDlgItem(IDC_BUTTON4)->SetWindowText(BTN_FINALENC);
}