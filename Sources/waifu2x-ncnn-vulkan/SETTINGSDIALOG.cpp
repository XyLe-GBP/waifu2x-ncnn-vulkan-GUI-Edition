// SETTINGSDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "waifu2x-ncnn-vulkanDlg.h"
#include "SETTINGSDIALOG.h"
#include "afxdialogex.h"


// SETTINGSDIALOG ダイアログ

IMPLEMENT_DYNAMIC(SETTINGSDIALOG, CDialogEx)

SETTINGSDIALOG::SETTINGSDIALOG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGDIALOG, pParent)
	, xv_BlockSize(_T(""))
	, xv_CommandLineParam(_T(""))
{

	FinalSettings = _T("");
	NoiseLevel = _T("");
	UpscaleLevel = _T("");
	SelectGPU = _T("");
	BlockSize = _T("");
	BlockSize_suffix = _T("");
	useThread = _T("");
	waifu2xModel = _T("");
	OutFormat = _T("");
	verbose_out = _T("");
	tta_mode = _T("");
}

SETTINGSDIALOG::~SETTINGSDIALOG()
{
}

void SETTINGSDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_NOISE, m_NoiseLevel);
	DDX_Control(pDX, IDC_COMBO_SCALE, m_UpscaleLevel);
	DDX_Control(pDX, IDC_COMBO_GPU, m_SelectGPU);
	DDX_Text(pDX, IDC_EDIT_BLOCKSIZE, xv_BlockSize);
	DDV_MaxChars(pDX, xv_BlockSize, 4);
	DDX_Control(pDX, IDC_CHECK_ADVANCED, m_AdvancedEnable);
	DDX_Text(pDX, IDC_EDIT_CMD, xv_CommandLineParam);
	DDX_Control(pDX, IDC_COMBO_THREAD, m_threads);
	DDX_Control(pDX, IDC_COMBO_MODEL, m_model);
	DDX_Control(pDX, IDC_COMBO_FORMAT, m_format);
	DDX_Control(pDX, IDC_CHECK_VERBOSE, m_verbose);
	DDX_Control(pDX, IDC_CHECK_TTA, m_tta);
}


BEGIN_MESSAGE_MAP(SETTINGSDIALOG, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_ADVANCED, &SETTINGSDIALOG::OnBnClickedCheck1)
	ON_BN_CLICKED(IDOK, &SETTINGSDIALOG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SETTINGSDIALOG::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_NOISE, &SETTINGSDIALOG::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO_SCALE, &SETTINGSDIALOG::OnCbnSelchangeCombo2)
	ON_EN_CHANGE(IDC_EDIT_BLOCKSIZE, &SETTINGSDIALOG::OnEnChangeEdit1)
	ON_CBN_SELCHANGE(IDC_COMBO_GPU, &SETTINGSDIALOG::OnCbnSelchangeCombo3)
	ON_EN_CHANGE(IDC_EDIT_CMD, &SETTINGSDIALOG::OnEnChangeEdit3)
	ON_CBN_SELCHANGE(IDC_COMBO_THREAD, &SETTINGSDIALOG::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, &SETTINGSDIALOG::OnCbnSelchangeCombo5)
	ON_CBN_SELCHANGE(IDC_COMBO_FORMAT, &SETTINGSDIALOG::OnCbnSelchangeCombo6)
	ON_BN_CLICKED(IDC_CHECK_VERBOSE, &SETTINGSDIALOG::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK_TTA, &SETTINGSDIALOG::OnBnClickedCheck3)
END_MESSAGE_MAP()


// SETTINGSDIALOG メッセージ ハンドラー
BOOL SETTINGSDIALOG::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_COMBO_NOISE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COMBO_SCALE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COMBO_GPU)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COMBO_THREAD)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COMBO_MODEL)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_COMBO_FORMAT)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_ADVANCED)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_VERBOSE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_CHECK_TTA)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_BLOCKSIZE)->m_hWnd ||
			pMsg->hwnd == GetDlgItem(IDC_EDIT_CMD)->m_hWnd)
		{
			m_hToolTip.RelayEvent(pMsg);
		}
		else
		{
			m_hToolTip.Pop();
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL SETTINGSDIALOG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UINT Lang;
	Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");

	SetDlgLang();

	m_hToolTip.Create(this, TTS_ALWAYSTIP);
	m_hToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 512);
	m_hToolTip.AddTool(GetDlgItem(IDC_COMBO_NOISE), DESCRIPTION1);
	m_hToolTip.AddTool(GetDlgItem(IDC_COMBO_SCALE), DESCRIPTION2);
	m_hToolTip.AddTool(GetDlgItem(IDC_COMBO_GPU), DESCRIPTION3);
	m_hToolTip.AddTool(GetDlgItem(IDC_COMBO_THREAD), DESCRIPTION4);
	m_hToolTip.AddTool(GetDlgItem(IDC_COMBO_MODEL), DESCRIPTION5);
	m_hToolTip.AddTool(GetDlgItem(IDC_COMBO_FORMAT), DESCRIPTION6);
	m_hToolTip.AddTool(GetDlgItem(IDC_CHECK_ADVANCED), DESCRIPTION7);
	m_hToolTip.AddTool(GetDlgItem(IDC_CHECK_VERBOSE), DESCRIPTION8);
	m_hToolTip.AddTool(GetDlgItem(IDC_CHECK_TTA), DESCRIPTION9);
	m_hToolTip.AddTool(GetDlgItem(IDC_EDIT_BLOCKSIZE), DESCRIPTION10);
	m_hToolTip.AddTool(GetDlgItem(IDC_EDIT_CMD), DESCRIPTION11);

	if (Lang == 0) {
		m_NoiseLevel.AddString(_T("除去しない"));
		m_NoiseLevel.AddString(_T("レベル0"));
		m_NoiseLevel.AddString(_T("レベル1"));
		m_NoiseLevel.AddString(_T("レベル2"));
		m_NoiseLevel.AddString(_T("レベル3"));
		m_UpscaleLevel.AddString(_T("1倍"));
		m_UpscaleLevel.AddString(_T("2倍"));
		m_SelectGPU.AddString(_T("自動"));
		m_SelectGPU.AddString(_T("iGPU"));
		m_SelectGPU.AddString(_T("dGPU 0"));
		m_SelectGPU.AddString(_T("dGPU 1"));
		m_SelectGPU.AddString(_T("dGPU 2"));
	}
	else if (Lang == 1) {
		m_NoiseLevel.AddString(_T("Not remove"));
		m_NoiseLevel.AddString(_T("Level 0"));
		m_NoiseLevel.AddString(_T("Level 1"));
		m_NoiseLevel.AddString(_T("Level 2"));
		m_NoiseLevel.AddString(_T("Level 3"));
		m_UpscaleLevel.AddString(_T("x1"));
		m_UpscaleLevel.AddString(_T("x2"));
		m_SelectGPU.AddString(_T("Auto"));
		m_SelectGPU.AddString(_T("iGPU"));
		m_SelectGPU.AddString(_T("dGPU 0"));
		m_SelectGPU.AddString(_T("dGPU 1"));
		m_SelectGPU.AddString(_T("dGPU 2"));
	}
	else {
		m_NoiseLevel.AddString(_T("除去しない"));
		m_NoiseLevel.AddString(_T("レベル0"));
		m_NoiseLevel.AddString(_T("レベル1"));
		m_NoiseLevel.AddString(_T("レベル2"));
		m_NoiseLevel.AddString(_T("レベル3"));
		m_UpscaleLevel.AddString(_T("1倍"));
		m_UpscaleLevel.AddString(_T("2倍"));
		m_SelectGPU.AddString(_T("自動"));
		m_SelectGPU.AddString(_T("iGPU"));
		m_SelectGPU.AddString(_T("dGPU 0"));
		m_SelectGPU.AddString(_T("dGPU 1"));
		m_SelectGPU.AddString(_T("dGPU 2"));
	}
	
	m_threads.AddString(_T("1:2:2"));
	m_threads.AddString(_T("1:2"));
	m_threads.AddString(_T("2"));
	m_threads.AddString(_T("2:2"));
	m_threads.AddString(_T("2:2:2"));
	m_model.AddString(_T("CUnet (models-cunet)"));
	m_model.AddString(_T("RGB (models-upconv_7_anime_style_art_rgb)"));
	m_model.AddString(_T("Photo (models-upconv_7_photo)"));
	m_format.AddString(_T("JPG"));
	m_format.AddString(_T("PNG"));
	m_format.AddString(_T("WEBP"));

	unsigned int noise, upscale, selgpu, advanced, thread, model, format, verbose, tta;
	noise = 0;
	upscale = 0;
	selgpu = 0;
	advanced = 0;
	thread = 0;
	model = 0;
	format = 0;
	verbose = 0;
	tta = 0;

	wchar_t prm[1024]{}, blocksize[10]{};

	GetPrivateProfileString(L"WAIFU2X_SETTINGS", L"0x20FF", NULL, (LPWSTR)prm, 512, L".\\settings.ini");// Param
	GetPrivateProfileString(L"WAIFU2X_SETTINGS", L"0x2000", NULL, (LPWSTR)blocksize, 5, L".\\settings.ini"); // Blocksize
	noise = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini"); // Noise Level
	upscale = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x0001", INFINITE, L".\\settings.ini"); // Upscale Level
	selgpu = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x0002", INFINITE, L".\\settings.ini"); // Primary GPU
	advanced = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x1000", INFINITE, L".\\settings.ini"); // Advanced Enable
	thread = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x0003", INFINITE, L".\\settings.ini"); // Thread
	model = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x0004", INFINITE, L".\\settings.ini"); // Model
	format = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x0005", INFINITE, L".\\settings.ini"); // Format
	verbose = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x1001", INFINITE, L".\\settings.ini"); // Verbose
	tta = GetPrivateProfileInt(L"WAIFU2X_SETTINGS", L"0x1002", INFINITE, L".\\settings.ini"); // TTA

	switch (noise)
	{
	case 0:
		NoiseLevel = L" -n -1";
		m_NoiseLevel.SetCurSel(0);
		break;
	case 1:
		NoiseLevel = L" -n 0";
		m_NoiseLevel.SetCurSel(1);
		break;
	case 2:
		NoiseLevel = L" -n 1";
		m_NoiseLevel.SetCurSel(2);
		break;
	case 3:
		NoiseLevel = L" -n 2";
		m_NoiseLevel.SetCurSel(3);
		break;
	case 4:
		NoiseLevel = L" -n 3";
		m_NoiseLevel.SetCurSel(4);
		break;
	default:
		NoiseLevel = L" -n 2";
		m_NoiseLevel.SetCurSel(3);
		break;
	}

	switch (upscale)
	{
	case 0:
		UpscaleLevel = L" -s 1";
		m_UpscaleLevel.SetCurSel(0);
		break;
	case 1:
		UpscaleLevel = L" -s 2";
		m_UpscaleLevel.SetCurSel(1);
		break;
	default:
		UpscaleLevel = L" -s 2";
		m_UpscaleLevel.SetCurSel(1);
		break;
	}

	switch (selgpu)
	{
	case 0:
		SelectGPU = L" -g default";
		m_SelectGPU.SetCurSel(0);
		break;
	case 1:
		SelectGPU = L" -g -1";
		m_SelectGPU.SetCurSel(1);
		break;
	case 2:
		SelectGPU = L" -g 0";
		m_SelectGPU.SetCurSel(2);
		break;
	case 3:
		SelectGPU = L" -g 1";
		m_SelectGPU.SetCurSel(3);
		break;
	case 4:
		SelectGPU = L" -g 2";
		m_SelectGPU.SetCurSel(4);
		break;
	default:
		SelectGPU = L" -g default";
		m_SelectGPU.SetCurSel(0);
		break;
	}

	if (blocksize != L"") {
		BlockSize_suffix = L" -t ";
		BlockSize = (LPCTSTR)blocksize;
		CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_BLOCKSIZE);
		edit1->SetWindowText((LPCTSTR)blocksize);
	}
	else {
		BlockSize_suffix = L" -t ";
		BlockSize = L"0";
		CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_BLOCKSIZE);
		edit1->SetWindowText(L"0");
	}

	switch (thread)
	{
	case 0:
		useThread = L" -j 1:2:2";
		m_threads.SetCurSel(0);
		break;
	case 1:
		useThread = L" -j 1:2";
		m_threads.SetCurSel(1);
		break;
	case 2:
		useThread = L" -j 2";
		m_threads.SetCurSel(2);
		break;
	case 3:
		useThread = L" -j 2:2";
		m_threads.SetCurSel(3);
		break;
	case 4:
		useThread = L" -j 2:2:2";
		m_threads.SetCurSel(3);
		break;
	default:
		useThread = L" -j 1:2:2";
		m_threads.SetCurSel(0);
		break;
	}

	switch (model)
	{
	case 0:
		waifu2xModel = L" -m models-cunet";
		m_model.SetCurSel(0);
		break;
	case 1:
		waifu2xModel = L" -m models-upconv_7_anime_style_art_rgb";
		m_model.SetCurSel(1);
		break;
	case 2:
		waifu2xModel = L" -m models-upconv_7_photo";
		m_model.SetCurSel(2);
		break;
	default:
		waifu2xModel = L" -m models-cunet";
		m_model.SetCurSel(0);
		break;
	}

	switch (format)
	{
	case 0:
		OutFormat = L" -f jpg";
		m_format.SetCurSel(0);
		break;
	case 1:
		OutFormat = L" -f png";
		m_format.SetCurSel(1);
		break;
	case 2:
		OutFormat = L" -f webp";
		m_format.SetCurSel(2);
		break;
	default:
		OutFormat = L" -f png";
		m_format.SetCurSel(1);
		break;
	}

	if (verbose == 1) {
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
		check2->SetCheck(BST_CHECKED);
		verbose_out = L" -v";
	}
	else {
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
		check2->SetCheck(BST_UNCHECKED);
		verbose_out = L"";
	}
	if (tta == 1) {
		CButton* check3 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
		check3->SetCheck(BST_CHECKED);
		tta_mode = L" -x";
	}
	else {
		CButton* check3 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
		check3->SetCheck(BST_UNCHECKED);
		tta_mode = L"";
	}
	if (_tcscmp(prm, _T("")) != 0) {
		FinalSettings = (LPCTSTR)prm;
	}
	else {
		FinalSettings = L"";
	}
	if (advanced == 1) {
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		check1->SetCheck(BST_CHECKED);
		CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
		CButton* check3 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
		CComboBox* combo4 = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
		CComboBox* combo5 = (CComboBox*)GetDlgItem(IDC_COMBO_MODEL);
		CComboBox* combo6 = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
		edit1->EnableWindow(TRUE);
		check2->EnableWindow(TRUE);
		check3->EnableWindow(TRUE);
		combo4->EnableWindow(TRUE);
		combo5->EnableWindow(TRUE);
		combo6->EnableWindow(TRUE);
		edit1->SetWindowText(FinalSettings);
	}
	else {
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		check1->SetCheck(BST_UNCHECKED);
		CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
		CButton* check3 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
		CComboBox* combo4 = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
		CComboBox* combo5 = (CComboBox*)GetDlgItem(IDC_COMBO_MODEL);
		CComboBox* combo6 = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
		edit1->EnableWindow(FALSE);
		check2->EnableWindow(FALSE);
		check3->EnableWindow(FALSE);
		combo4->EnableWindow(FALSE);
		combo5->EnableWindow(FALSE);
		combo6->EnableWindow(FALSE);
		edit1->SetWindowText(NULL);
	}

	Core->LoadImageLibrary();
	m_hImage.Attach((HBITMAP)::LoadBitmap(Core->hinst, MAKEINTRESOURCE(102)));
	CStatic* box = (CStatic*)GetDlgItem(IDC_STATIC_IMG);
	box->SetBitmap(m_hImage);
	box->InvalidateRect(NULL, 1);

	return TRUE;
}


void SETTINGSDIALOG::OnBnClickedCheck1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
	if (check1->GetCheck() == BST_UNCHECKED) {
		check1->SetCheck(BST_UNCHECKED);
		CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
		CButton* check3 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
		CComboBox* combo4 = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
		CComboBox* combo5 = (CComboBox*)GetDlgItem(IDC_COMBO_MODEL);
		CComboBox* combo6 = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
		edit1->EnableWindow(FALSE);
		check2->EnableWindow(FALSE);
		check3->EnableWindow(FALSE);
		combo4->EnableWindow(FALSE);
		combo5->EnableWindow(FALSE);
		combo6->EnableWindow(FALSE);
		edit1->SetWindowText(NULL);
		CommandLineParam = L"";
		return;
	}
	else if (check1->GetCheck() == BST_CHECKED) {
		int on_button;
		on_button = MessageBox(WARN_ADVANCED, WARN_CAUTION_TITLE, MB_ICONWARNING | MB_YESNO);
		if (on_button == IDYES) {
			if (check1->GetCheck() == BST_CHECKED) {
				CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
				CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
				CButton* check3 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
				CComboBox* combo4 = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
				CComboBox* combo5 = (CComboBox*)GetDlgItem(IDC_COMBO_MODEL);
				CComboBox* combo6 = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
				tta_mode = L"";
				verbose_out = L"";
				useThread = L" -j 1:2:2";
				waifu2xModel = L" -m models-cunet";
				OutFormat = L" -f png";
				FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
				edit1->EnableWindow(TRUE);
				check2->EnableWindow(TRUE);
				check3->EnableWindow(TRUE);
				combo4->EnableWindow(TRUE);
				combo5->EnableWindow(TRUE);
				combo6->EnableWindow(TRUE);
				edit1->SetWindowText(FinalSettings);
				CommandLineParam = FinalSettings;
				return;
			}
			else {
				check1->SetCheck(BST_UNCHECKED);
				CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
				CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
				CButton* check3 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
				CComboBox* combo4 = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
				CComboBox* combo5 = (CComboBox*)GetDlgItem(IDC_COMBO_MODEL);
				CComboBox* combo6 = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
				edit1->EnableWindow(FALSE);
				check2->EnableWindow(FALSE);
				check3->EnableWindow(FALSE);
				combo4->EnableWindow(FALSE);
				combo5->EnableWindow(FALSE);
				combo6->EnableWindow(FALSE);
				edit1->SetWindowText(NULL);
				tta_mode = L"";
				verbose_out = L"";
				useThread = L"";
				waifu2xModel = L"";
				OutFormat = L"";
				CommandLineParam = L"";
				return;
			}
		}
		else if (on_button == IDNO) {
			check1->SetCheck(BST_UNCHECKED);
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
			CButton* check3 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
			CComboBox* combo4 = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
			CComboBox* combo5 = (CComboBox*)GetDlgItem(IDC_COMBO_MODEL);
			CComboBox* combo6 = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
			edit1->EnableWindow(FALSE);
			check2->EnableWindow(FALSE);
			check3->EnableWindow(FALSE);
			combo4->EnableWindow(FALSE);
			combo5->EnableWindow(FALSE);
			combo6->EnableWindow(FALSE);
			edit1->SetWindowText(NULL);
			tta_mode = L"";
			verbose_out = L"";
			useThread = L"";
			waifu2xModel = L"";
			OutFormat = L"";
			CommandLineParam = L"";
			return;
		}
		else {
			check1->SetCheck(BST_UNCHECKED);
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
			CButton* check3 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
			CComboBox* combo4 = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
			CComboBox* combo5 = (CComboBox*)GetDlgItem(IDC_COMBO_MODEL);
			CComboBox* combo6 = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
			edit1->EnableWindow(FALSE);
			check2->EnableWindow(FALSE);
			check3->EnableWindow(FALSE);
			combo4->EnableWindow(FALSE);
			combo5->EnableWindow(FALSE);
			combo6->EnableWindow(FALSE);
			edit1->SetWindowText(NULL);
			tta_mode = L"";
			verbose_out = L"";
			useThread = L"";
			waifu2xModel = L"";
			OutFormat = L"";
			CommandLineParam = L"";
			return;
		}
	}
	else {
		check1->SetCheck(BST_UNCHECKED);
		CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
		CButton* check3 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
		CComboBox* combo4 = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
		CComboBox* combo5 = (CComboBox*)GetDlgItem(IDC_COMBO_MODEL);
		CComboBox* combo6 = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
		edit1->EnableWindow(FALSE);
		check2->EnableWindow(FALSE);
		check3->EnableWindow(FALSE);
		combo4->EnableWindow(FALSE);
		combo5->EnableWindow(FALSE);
		combo6->EnableWindow(FALSE);
		edit1->SetWindowText(NULL);
		tta_mode = L"";
		verbose_out = L"";
		useThread = L"";
		waifu2xModel = L"";
		OutFormat = L"";
		CommandLineParam = L"";
		return;
	}
	return;
}


void SETTINGSDIALOG::OnBnClickedCheck2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_VERBOSE);
	if (check1->GetCheck() == BST_UNCHECKED) {
		verbose_out = L"";
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check2->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (check1->GetCheck() == BST_CHECKED) {
		verbose_out = L" -v";
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check2->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else {
		verbose_out = L"";
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check2->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
}


void SETTINGSDIALOG::OnBnClickedCheck3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_TTA);
	if (check1->GetCheck() == BST_UNCHECKED) {
		tta_mode = L"";
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check2->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (check1->GetCheck() == BST_CHECKED) {
		tta_mode = L" -x";
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check2->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else {
		tta_mode = L"";
		CButton* check2 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check2->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
}


void SETTINGSDIALOG::OnCbnSelchangeCombo1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_NOISE);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	if (index1 == 0) {
		NoiseLevel = L" -n -1";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 1) {
		NoiseLevel = L" -n 0";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 2) {
		NoiseLevel = L" -n 1";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 3) {
		NoiseLevel = L" -n 2";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 4) {
		NoiseLevel = L" -n 3";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else {
		NoiseLevel = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
}


void SETTINGSDIALOG::OnCbnSelchangeCombo2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_SCALE);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	if (index1 == 0) {
		UpscaleLevel = L" -s 1";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 1) {
		UpscaleLevel = L" -s 2";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else {
		UpscaleLevel = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
}


void SETTINGSDIALOG::OnEnChangeEdit1()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	if (GetDlgItem(IDC_EDIT_BLOCKSIZE)->GetWindowTextLength() == 0) {
		BlockSize_suffix = L"";
		BlockSize = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (GetDlgItem(IDC_EDIT_BLOCKSIZE)->GetWindowTextLength() != 0) {
		UpdateData(TRUE);
		BlockSize_suffix = L" -t ";
		BlockSize = xv_BlockSize;
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else {
		BlockSize_suffix = L"";
		BlockSize = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
}


void SETTINGSDIALOG::OnCbnSelchangeCombo3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_GPU);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	if (index1 == 0) {
		SelectGPU = L" -g default";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 1) {
		SelectGPU = L" -g -1";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 2) {
		SelectGPU = L" -g 0";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 3) {
		SelectGPU = L" -g 1";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 4) {
		SelectGPU = L" -g 2";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else {
		SelectGPU = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
}

void SETTINGSDIALOG::OnCbnSelchangeCombo4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	if (index1 == 0) {
		useThread = L" -j 1:2:2";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 1) {
		useThread = L" -j 1:2";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 2) {
		useThread = L" -j 2";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 3) {
		useThread = L" -j 2:2";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 4) {
		useThread = L" -j 2:2:2";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else {
		useThread = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
}


void SETTINGSDIALOG::OnCbnSelchangeCombo5()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_MODEL);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	if (index1 == 0) {
		waifu2xModel = L" -m models-cunet";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 1) {
		waifu2xModel = L" -m models-upconv_7_anime_style_art_rgb";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 2) {
		waifu2xModel = L" -m models-upconv_7_photo";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else {
		waifu2xModel = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
}


void SETTINGSDIALOG::OnCbnSelchangeCombo6()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	if (index1 == 0) {
		OutFormat = L" -f jpg";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 1) {
		OutFormat = L" -f png";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 2) {
		OutFormat = L" -f webp";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
	else {
		OutFormat = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (check1->GetCheck() == BST_CHECKED) {
			CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT_CMD);
			FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
			edit1->SetWindowText(FinalSettings);
			return;
		}
		else {
			return;
		}
	}
}


void SETTINGSDIALOG::OnEnChangeEdit3()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
	if (check1->GetCheck() == BST_CHECKED) {
		UpdateData(TRUE);
		CommandLineParam = xv_CommandLineParam;
		return;
	}
	else {
		return;
	}
}


void SETTINGSDIALOG::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
	if (check1->GetCheck() == BST_CHECKED) {
		FinalSettings = xv_CommandLineParam;
	}
	else {
		FinalSettings = L"waifu2x-ncnn-vulkan -i $InFile -o $OutFile" + NoiseLevel + UpscaleLevel + BlockSize_suffix + BlockSize + SelectGPU + useThread + OutFormat + waifu2xModel + verbose_out + tta_mode;
	}

	if (GetDlgItem(IDC_EDIT_BLOCKSIZE)->GetWindowTextLength() <= 2) {
		if (_ttoi(xv_BlockSize) == 0) {
			OutputDebugString(_T("Set Blocksize: Auto.\n"));
		}
		else if (_ttoi(xv_BlockSize) <= 32) {
			MessageBox(ERROR_BLOCKSIZE_SHORT, ERROR_TITLE, MB_ICONERROR | MB_OK);
			return;
		}
	}

	if (GetDlgItem(IDC_EDIT_BLOCKSIZE)->GetWindowTextLength() >= 4) {
		if (_ttoi(xv_BlockSize) > 5000) {
			MessageBox(ERROR_BLOCKSIZE_LARGE, ERROR_TITLE, MB_ICONERROR | MB_OK);
			return;
		}
	}

	if (FinalSettings == L"") {
		MessageBox(_T("Setting error."), _T("Error"), MB_ICONERROR | MB_OK);
		return;
	}

	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_NOISE);
	CComboBox* combo2 = (CComboBox*)GetDlgItem(IDC_COMBO_SCALE);
	CComboBox* combo3 = (CComboBox*)GetDlgItem(IDC_COMBO_GPU);
	CComboBox* combo4 = (CComboBox*)GetDlgItem(IDC_COMBO_THREAD);
	CComboBox* combo5 = (CComboBox*)GetDlgItem(IDC_COMBO_MODEL);
	CComboBox* combo6 = (CComboBox*)GetDlgItem(IDC_COMBO_FORMAT);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}
	int index2 = combo2->GetCurSel();
	if (index2 != CB_ERR) {
		TRACE("%d", index2);
	}
	int index3 = combo3->GetCurSel();
	if (index3 != CB_ERR) {
		TRACE("%d", index3);
	}
	int index4 = combo4->GetCurSel();
	if (index4 != CB_ERR) {
		TRACE("%d", index4);
	}
	int index5 = combo5->GetCurSel();
	if (index5 != CB_ERR) {
		TRACE("%d", index5);
	}
	int index6 = combo6->GetCurSel();
	if (index6 != CB_ERR) {
		TRACE("%d", index6);
	}
	CString comboindex1;
	CString comboindex2;
	CString comboindex3;
	CString comboindex4;
	CString comboindex5;
	CString comboindex6;
	comboindex1.Format(_T("%d"), index1);
	comboindex2.Format(_T("%d"), index2);
	comboindex3.Format(_T("%d"), index3);
	comboindex4.Format(_T("%d"), index4);
	comboindex5.Format(_T("%d"), index5);
	comboindex6.Format(_T("%d"), index6);

	WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x20FF", FinalSettings, L".\\settings.ini");
	if (combo1->GetCurSel() != CB_ERR) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0000", comboindex1, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0000", NULL, L".\\settings.ini");
	}
	if (combo2->GetCurSel() != CB_ERR) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0001", comboindex2, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0001", NULL, L".\\settings.ini");
	}
	if (combo3->GetCurSel() != CB_ERR) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0002", comboindex3, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0002", NULL, L".\\settings.ini");
	}
	if (combo4->GetCurSel() != CB_ERR) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0003", comboindex4, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0003", NULL, L".\\settings.ini");
	}
	if (combo5->GetCurSel() != CB_ERR) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0004", comboindex5, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0004", NULL, L".\\settings.ini");
	}
	if (combo6->GetCurSel() != CB_ERR) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0005", comboindex6, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x0005", NULL, L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_ADVANCED))->GetCheck() == BST_CHECKED) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x1000", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x1000", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_VERBOSE))->GetCheck() == BST_CHECKED) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x1001", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x1001", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_TTA))->GetCheck() == BST_CHECKED) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x1002", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x1002", L"0", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_BLOCKSIZE))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x2000", BlockSize, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x2000", L"", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_CMD))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x2001", CommandLineParam, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"WAIFU2X_SETTINGS", L"0x2001", L"", L".\\settings.ini");
	}

	CDialogEx::OnOK();
}


void SETTINGSDIALOG::SetDlgLang()
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

	LoadString(Core->Lang_hinst, IDS_2X_TITLE, (LPTSTR)W2X_TITLE, 256);
	SetWindowText(W2X_TITLE);
	LoadString(Core->Lang_hinst, IDS_ERROR_TITLE, (LPTSTR)ERROR_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_BLOCKSIZE_LARGE, (LPTSTR)ERROR_BLOCKSIZE_LARGE, 256);
	LoadString(Core->Lang_hinst, IDS_ERROR_BLOCKSIZE_SHORT, (LPTSTR)ERROR_BLOCKSIZE_SHORT, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_CAUTION_TITLE, (LPTSTR)WARN_CAUTION_TITLE, 256);
	LoadString(Core->Lang_hinst, IDS_WARN_ADVANCED, (LPTSTR)WARN_ADVANCED, 256);
	LoadString(Core->Lang_hinst, IDS_GRP_GENERAL, (LPTSTR)STATIC_GRP_GENERAL, 256);
	GetDlgItem(IDC_STATIC_GRP_GEN)->SetWindowText(STATIC_GRP_GENERAL);
	LoadString(Core->Lang_hinst, IDS_GRP_ADVANCED, (LPTSTR)STATIC_GRP_ADVANCED, 256);
	GetDlgItem(IDC_STATIC_GRP_ADV)->SetWindowText(STATIC_GRP_ADVANCED);
	LoadString(Core->Lang_hinst, IDS_CHECK_ADVANCED, (LPTSTR)CHECK_ADVANCED, 256);
	GetDlgItem(IDC_CHECK_ADVANCED)->SetWindowText(CHECK_ADVANCED);
	LoadString(Core->Lang_hinst, IDS_CHECK_VERBOSE, (LPTSTR)CHECK_VERBOSE, 256);
	GetDlgItem(IDC_CHECK_VERBOSE)->SetWindowText(CHECK_VERBOSE);
	LoadString(Core->Lang_hinst, IDS_CHECK_TTA, (LPTSTR)CHECK_TTA, 256);
	GetDlgItem(IDC_CHECK_TTA)->SetWindowText(CHECK_TTA);
	LoadString(Core->Lang_hinst, IDS_STATIC_DENOISE, (LPTSTR)STATIC_DENOISE, 256);
	GetDlgItem(IDC_STATIC_NS)->SetWindowText(STATIC_DENOISE);
	LoadString(Core->Lang_hinst, IDS_STATIC_UPSCALELV, (LPTSTR)STATIC_UPSCALELV, 256);
	GetDlgItem(IDC_STATIC_SC)->SetWindowText(STATIC_UPSCALELV);
	LoadString(Core->Lang_hinst, IDS_STATIC_USEGPU, (LPTSTR)STATIC_USEGPU, 256);
	GetDlgItem(IDC_STATIC_CNVGPU)->SetWindowText(STATIC_USEGPU);
	LoadString(Core->Lang_hinst, IDS_STATIC_BLOCKSIZE, (LPTSTR)STATIC_BLOCKSIZE, 256);
	GetDlgItem(IDC_STATIC_BLS)->SetWindowText(STATIC_BLOCKSIZE);
	LoadString(Core->Lang_hinst, IDS_STATIC_BLOCK, (LPTSTR)STATIC_BLOCK, 256);
	GetDlgItem(IDC_STATIC_BL)->SetWindowText(STATIC_BLOCK);
	LoadString(Core->Lang_hinst, IDS_STATIC_THREAD, (LPTSTR)STATIC_THREAD, 256);
	GetDlgItem(IDC_STATIC_THR)->SetWindowText(STATIC_THREAD);
	LoadString(Core->Lang_hinst, IDS_STATIC_FORMAT, (LPTSTR)STATIC_FORMAT, 256);
	GetDlgItem(IDC_STATIC_FMT)->SetWindowText(STATIC_FORMAT);
	LoadString(Core->Lang_hinst, IDS_STATIC_MODEL, (LPTSTR)STATIC_MODEL, 256);
	GetDlgItem(IDC_STATIC_MDL)->SetWindowText(STATIC_MODEL);
	LoadString(Core->Lang_hinst, IDS_STATIC_PARAM, (LPTSTR)STATIC_PARAM, 256);
	GetDlgItem(IDC_STATIC_PRM)->SetWindowText(STATIC_PARAM);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION1, (LPTSTR)DESCRIPTION1, 512);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION2, (LPTSTR)DESCRIPTION2, 512);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION3, (LPTSTR)DESCRIPTION3, 512);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION4, (LPTSTR)DESCRIPTION4, 512);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION5, (LPTSTR)DESCRIPTION5, 512);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION6, (LPTSTR)DESCRIPTION6, 512);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION7, (LPTSTR)DESCRIPTION7, 512);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION8, (LPTSTR)DESCRIPTION8, 512);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION9, (LPTSTR)DESCRIPTION9, 512);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION10, (LPTSTR)DESCRIPTION10, 512);
	LoadString(Core->Lang_hinst, IDS_DESCRIPTION11, (LPTSTR)DESCRIPTION11, 512);
}


void SETTINGSDIALOG::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}

void SETTINGSDIALOG::OnDestroy()
{
	CDialogEx::OnDestroy();

	FreeLibrary(Core->hinst);
	FreeLibrary(Core->Lang_hinst);
	SAFE_DELETE(CORE_FUNC);
}