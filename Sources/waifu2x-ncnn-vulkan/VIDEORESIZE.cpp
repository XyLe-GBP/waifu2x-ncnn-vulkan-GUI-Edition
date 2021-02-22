// VIDEORESIZE.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "waifu2x-ncnn-vulkanDlg.h"
#include "VIDEORESIZE.h"
#include "afxdialogex.h"


// VIDEORESIZE ダイアログ

IMPLEMENT_DYNAMIC(VIDEORESIZE, CDialogEx)

VIDEORESIZE::VIDEORESIZE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIDEORESIZEDIALOG, pParent)
	, m_nRadio(FALSE)
{

	SIZE = _T("");
	FINAL = _T("");
}

VIDEORESIZE::~VIDEORESIZE()
{
}

void VIDEORESIZE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Resize);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
}


BEGIN_MESSAGE_MAP(VIDEORESIZE, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO2, &VIDEORESIZE::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &VIDEORESIZE::OnBnClickedRadio1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &VIDEORESIZE::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &VIDEORESIZE::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &VIDEORESIZE::OnBnClickedCancel)
END_MESSAGE_MAP()


// VIDEORESIZE メッセージ ハンドラー
BOOL VIDEORESIZE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgLang();

	m_nRadio = 0;

	m_Resize.ResetContent();
	m_Resize.AddString(_T("8K (7680x4320)"));
	m_Resize.AddString(_T("4K 2160p (3840x2160)"));
	m_Resize.AddString(_T("WQHD 1440p (2560x1440)"));
	m_Resize.AddString(_T("FHD 1080p (1920x1080)"));
	m_Resize.AddString(_T("WXGA++ (1600x900)"));
	m_Resize.AddString(_T("HD 720p (1280x720)"));
	m_Resize.AddString(_T("1024x576"));
	m_Resize.AddString(_T("768x432"));
	m_Resize.AddString(_T("640x360"));
	m_Resize.AddString(_T("480x270"));
	m_Resize.AddString(_T("320x180"));
	m_Resize.AddString(_T("160x90"));
	m_Resize.SetCurSel(3);
	SIZE = L"1920x1080";

	return TRUE;
}


void VIDEORESIZE::OnBnClickedRadio1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_nRadio = 0;

	m_Resize.ResetContent();
	m_Resize.AddString(_T("8K (7680x4320)"));
	m_Resize.AddString(_T("4K 2160p (3840x2160)"));
	m_Resize.AddString(_T("WQHD 1440p (2560x1440)"));
	m_Resize.AddString(_T("FHD 1080p (1920x1080)"));
	m_Resize.AddString(_T("WXGA++ (1600x900)"));
	m_Resize.AddString(_T("HD 720p (1280x720)"));
	m_Resize.AddString(_T("1024x576"));
	m_Resize.AddString(_T("768x432"));
	m_Resize.AddString(_T("640x360"));
	m_Resize.AddString(_T("480x270"));
	m_Resize.AddString(_T("320x180"));
	m_Resize.AddString(_T("160x90"));
	m_Resize.SetCurSel(3);
	SIZE = L"1920x1080";
}


void VIDEORESIZE::OnBnClickedRadio2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_nRadio = 1;

	m_Resize.ResetContent();
	m_Resize.AddString(_T("QUXGA (3200x2400)"));
	m_Resize.AddString(_T("QXGA (2048x1536)"));
	m_Resize.AddString(_T("UXGA (1600x1200)"));
	m_Resize.AddString(_T("QVGA (1280x960)"));
	m_Resize.AddString(_T("XGA (1024x768)"));
	m_Resize.AddString(_T("SVGA (800x600)"));
	m_Resize.AddString(_T("VGA (640x480)"));
	m_Resize.AddString(_T("QVGA (320x240)"));
	m_Resize.AddString(_T("QQVGA (160x120)"));
	m_Resize.SetCurSel(2);
	SIZE = L"1600x1200";
}


void VIDEORESIZE::OnCbnSelchangeCombo1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	switch (m_nRadio)
	{
	case 0:
		switch (m_Resize.GetCurSel())
		{
		case 0:
			SIZE = L"7680x4320";
			break;
		case 1:
			SIZE = L"3840x2160";
			break;
		case 2:
			SIZE = L"2560x1440";
			break;
		case 3:
			SIZE = L"1920x1080";
			break;
		case 4:
			SIZE = L"1600x900";
			break;
		case 5:
			SIZE = L"1280x720";
			break;
		case 6:
			SIZE = L"1024x576";
			break;
		case 7:
			SIZE = L"768x432";
			break;
		case 8:
			SIZE = L"640x360";
			break;
		case 9:
			SIZE = L"480x270";
			break;
		case 10:
			SIZE = L"320x180";
			break;
		case 11:
			SIZE = L"160x90";
			break;
		default:
			SIZE = L"";
			break;
		}
		break;
	case 1:
		switch (m_Resize.GetCurSel())
		{
		case 0:
			SIZE = L"3200x2400";
			break;
		case 1:
			SIZE = L"2048x1536";
			break;
		case 2:
			SIZE = L"1600x1200";
			break;
		case 3:
			SIZE = L"1280x960";
			break;
		case 4:
			SIZE = L"1024x768";
			break;
		case 5:
			SIZE = L"800x600";
			break;
		case 6:
			SIZE = L"640x480";
			break;
		case 7:
			SIZE = L"320x240";
			break;
		case 8:
			SIZE = L"160x120";
			break;
		default:
			SIZE = L"";
			break;
		}
		break;
	default:
		break;
	}
}


void VIDEORESIZE::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (SIZE == L"") {
		MessageBox(ERROR_SIZE, ERROR_TITLE, MB_ICONERROR | MB_OK);
	}
	FINAL = _T("ffmpeg -i $InFile -s ") + SIZE + _T(" -y $OutFile");

	CDialogEx::OnOK();
}


void VIDEORESIZE::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


void VIDEORESIZE::OnDestroy()
{
	CDialogEx::OnDestroy();

	FreeLibrary(Core->Lang_hinst);
	SAFE_DELETE(CORE_FUNC);
}


void VIDEORESIZE::SetDlgLang()
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

	LoadString(Core->Lang_hinst, IDS_RESIZE_TITLE, (LPTSTR)RESIZE_TITLE, 256);
	SetWindowText(RESIZE_TITLE);
	LoadString(Core->Lang_hinst, IDS_ERROR_TITLE, (LPTSTR)ERROR_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_SIZE, (LPTSTR)ERROR_SIZE, 256);
	LoadString(Core->Lang_hinst, IDS_GRP_ASPCT, (LPTSTR)STATIC_GRP_ASPECT, 256);
	GetDlgItem(IDC_STATIC_GRP_ASP)->SetWindowText(STATIC_GRP_ASPECT);
	LoadString(Core->Lang_hinst, IDS_GRP_SIZE, (LPTSTR)STATIC_GRP_SIZE, 256);
	GetDlgItem(IDC_STATIC_GRP_RS)->SetWindowText(STATIC_GRP_SIZE);
}
