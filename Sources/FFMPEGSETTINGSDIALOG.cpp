// FFMPEGSETTINGSDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "FFMPEGSETTINGSDIALOG.h"
#include "afxdialogex.h"


// FFMPEGSETTINGSDIALOG ダイアログ

IMPLEMENT_DYNAMIC(FFMPEGSETTINGSDIALOG, CDialogEx)

FFMPEGSETTINGSDIALOG::FFMPEGSETTINGSDIALOG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FFMPEG_SETTINGDIALOG, pParent)
	, xv_FPS(_T(""))
	, xv_ExePath(_T("Resources\\ffmpeg\\ffmpeg.exe"))
	, xv_AUDIOBITRATE(_T(""))
	, xv_VIDEOCODEC(_T(""))
	, xv_AUDIOCODEC(_T(""))
	, xv_CUSTOMPARAM(_T(""))
	, xv_QPLEVEL(_T(""))
{

	FPS = _T("");
	PRESET = _T("");
	VIDEOCODEC = _T("");
	AUDIOCODEC = _T("");
	AUDIOBITRATE = _T("");
	FINALPARAM = _T("");
	HIDEBANNER = _T("");
	STRCT = _T("");
	QP = _T("");
	OVERWRITE = _T("");
	CUSTOMPARAM = _T("");
	FPS2 = _T("");
}

FFMPEGSETTINGSDIALOG::~FFMPEGSETTINGSDIALOG()
{
}

void FFMPEGSETTINGSDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, xv_FPS);
	DDV_MaxChars(pDX, xv_FPS, 5);
	DDX_Text(pDX, IDC_EDIT1, xv_ExePath);
	DDX_Control(pDX, IDC_COMBO_PRESET, m_PRESET);
	DDX_Text(pDX, IDC_EDIT_AUDIO_BITRATE, xv_AUDIOBITRATE);
	DDV_MaxChars(pDX, xv_AUDIOBITRATE, 4);
	DDX_Text(pDX, IDC_EDIT_VIDEO_CODEC, xv_VIDEOCODEC);
	DDX_Text(pDX, IDC_EDIT_AUDIO_CODEC, xv_AUDIOCODEC);
	DDX_Text(pDX, IDC_EDIT_COMMAND_PARAM, xv_CUSTOMPARAM);
	DDX_Text(pDX, IDC_EDIT_QPLEVEL, xv_QPLEVEL);
	DDV_MaxChars(pDX, xv_QPLEVEL, 2);
}


BEGIN_MESSAGE_MAP(FFMPEGSETTINGSDIALOG, CDialogEx)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT2, &FFMPEGSETTINGSDIALOG::OnEnChangeEdit2)
	ON_BN_CLICKED(IDOK, &FFMPEGSETTINGSDIALOG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &FFMPEGSETTINGSDIALOG::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, &FFMPEGSETTINGSDIALOG::OnCbnSelchangeComboPreset)
	ON_EN_CHANGE(IDC_EDIT_AUDIO_BITRATE, &FFMPEGSETTINGSDIALOG::OnEnChangeEditAudioBitrate)
	ON_BN_CLICKED(IDC_CHECK_ADVANCED, &FFMPEGSETTINGSDIALOG::OnBnClickedCheckAdvanced)
	ON_BN_CLICKED(IDC_CHECK_HIDE_BANNER, &FFMPEGSETTINGSDIALOG::OnBnClickedCheckHideBanner)
	ON_BN_CLICKED(IDC_CHECK_QP, &FFMPEGSETTINGSDIALOG::OnBnClickedCheckQp)
	ON_EN_CHANGE(IDC_EDIT_COMMAND_PARAM, &FFMPEGSETTINGSDIALOG::OnEnChangeEditCommandParam)
	ON_EN_CHANGE(IDC_EDIT_VIDEO_CODEC, &FFMPEGSETTINGSDIALOG::OnEnChangeEditVideoCodec)
	ON_EN_CHANGE(IDC_EDIT_AUDIO_CODEC, &FFMPEGSETTINGSDIALOG::OnEnChangeEditAudioCodec)
	ON_EN_CHANGE(IDC_EDIT_QPLEVEL, &FFMPEGSETTINGSDIALOG::OnEnChangeEditQplevel)
	ON_BN_CLICKED(IDC_CHECK_AAC, &FFMPEGSETTINGSDIALOG::OnBnClickedCheckAac)
	ON_BN_CLICKED(IDC_CHECK_OVERWRITE, &FFMPEGSETTINGSDIALOG::OnBnClickedCheckOverwrite)
END_MESSAGE_MAP()


// FFMPEGSETTINGSDIALOG メッセージ ハンドラー
BOOL FFMPEGSETTINGSDIALOG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_PRESET.AddString(_T("デフォルト"));
	m_PRESET.AddString(_T("カスタム"));

	CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	edit1->SetWindowText(CURRENT_PATH + L"\\Resources\\ffmpeg\\ffmpeg.exe");

	UINT preset, adv, banner, strct, overw, qp;

	wchar_t fps[24], vc[100], ac[40], ab[20], qpl[20], p[1024];

	GetPrivateProfileString(L"FFMPEG_SETTINGS", L"0x2000", NULL, (LPWSTR)fps, 12, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_SETTINGS", L"0x2001", NULL, (LPWSTR)vc, 50, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_SETTINGS", L"0x2002", NULL, (LPWSTR)ac, 20, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_SETTINGS", L"0x2003", NULL, (LPWSTR)ab, 10, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_SETTINGS", L"0x2004", NULL, (LPWSTR)qpl, 10, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_SETTINGS", L"0x20FF", NULL, (LPWSTR)p, 512, L".\\settings.ini");
	preset = GetPrivateProfileInt(L"FFMPEG_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini");
	adv = GetPrivateProfileInt(L"FFMPEG_SETTINGS", L"0x1001", INFINITE, L".\\settings.ini");
	banner = GetPrivateProfileInt(L"FFMPEG_SETTINGS", L"0x1003", INFINITE, L".\\settings.ini");
	strct = GetPrivateProfileInt(L"FFMPEG_SETTINGS", L"0x1004", INFINITE, L".\\settings.ini");
	overw = GetPrivateProfileInt(L"FFMPEG_SETTINGS", L"0x1002", INFINITE, L".\\settings.ini");
	qp = GetPrivateProfileInt(L"FFMPEG_SETTINGS", L"0x1005", INFINITE, L".\\settings.ini");

	if (fps != L"") {
		FPS = L" -framerate ";
		FPS2 = L" -r ";
		FPS + (LPCTSTR)fps;
		FPS2 + (LPCTSTR)fps;
		CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
		edit2->SetWindowText((LPCTSTR)fps);
	}
	else {
		FPS = L"";
		FPS2 = L"";
		CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
		edit2->SetWindowText(NULL);
	}
	if (vc != L"") {
		VIDEOCODEC = L" -vcodec ";
		VIDEOCODEC + (LPCTSTR)vc;
		CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_VIDEO_CODEC);
		edit->SetWindowText((LPCTSTR)vc);
	}
	else {
		VIDEOCODEC = L"";
		CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_VIDEO_CODEC);
		edit->SetWindowText(L"hevc_nvenc");
	}
	if (ac != L"") {
		AUDIOCODEC = L" -acodec ";
		AUDIOCODEC + (LPCTSTR)ac;
		CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_CODEC);
		edit->SetWindowText((LPCTSTR)ac);
	}
	else {
		AUDIOCODEC = L"";
		CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_CODEC);
		edit->SetWindowText(L"aac");
	}
	if (ab != L"") {
		AUDIOBITRATE = L" -b:a ";
		AUDIOBITRATE + (LPCTSTR)ab;
		AUDIOBITRATE + L"k";
		CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_BITRATE);
		edit->SetWindowText((LPCTSTR)ab);
	}
	else {
		AUDIOBITRATE = L"";
		CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_BITRATE);
		edit->SetWindowText(L"192");
	}
	if (p != L"") {
		FINALPARAM = (LPCTSTR)p;
	}
	else {
		FINALPARAM = L"";
	}
	if (adv == 1) {
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		CButton* hbutton = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER);
		CButton* qbutton = (CButton*)GetDlgItem(IDC_CHECK_QP);
		CButton* sbutton = (CButton*)GetDlgItem(IDC_CHECK_AAC);
		CButton* owbutton = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE);
		CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
		CEdit* crf = (CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL);
		check1->SetCheck(BST_CHECKED);
		hbutton->EnableWindow(TRUE);
		qbutton->EnableWindow(TRUE);
		sbutton->EnableWindow(TRUE);
		owbutton->EnableWindow(TRUE);
		prm->EnableWindow(TRUE);
		prm->SetWindowText((LPCTSTR)p);
		crf->EnableWindow(TRUE);
	}
	else {
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		CButton* hbutton = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER);
		CButton* qbutton = (CButton*)GetDlgItem(IDC_CHECK_QP);
		CButton* sbutton = (CButton*)GetDlgItem(IDC_CHECK_AAC);
		CButton* owbutton = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE);
		CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
		CEdit* crf = (CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL);
		check1->SetCheck(BST_UNCHECKED);
		hbutton->EnableWindow(FALSE);
		qbutton->EnableWindow(FALSE);
		sbutton->EnableWindow(FALSE);
		owbutton->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		prm->SetWindowText(NULL);
		crf->EnableWindow(FALSE);
	}
	if (qp == 1) {
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_QP);
		CEdit* crf = (CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL);
		check1->SetCheck(BST_CHECKED);
		crf->EnableWindow(TRUE);
	}
	else {
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_QP);
		CEdit* crf = (CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL);
		check1->SetCheck(BST_UNCHECKED);
		crf->EnableWindow(FALSE);
	}
	if (qpl != L"") {
		QP = L" -qp ";
		QP + (LPCTSTR)qpl;
		CEdit* crf = (CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL);
		crf->SetWindowText((LPCTSTR)qpl);
	}
	else {
		QP = L"";
		CEdit* crf = (CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL);
		crf->SetWindowText(NULL);
	}
	if (banner == 1) {
		HIDEBANNER = L" -hide_banner";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER);
		check1->SetCheck(BST_CHECKED);
	}
	else {
		HIDEBANNER = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER);
		check1->SetCheck(BST_UNCHECKED);
	}
	if (strct == 1) {
		STRCT = L" -strict -2";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_AAC);
		check1->SetCheck(BST_CHECKED);
	}
	else {
		STRCT = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_AAC);
		check1->SetCheck(BST_UNCHECKED);
	}
	if (overw == 1) {
		OVERWRITE = L" -y";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE);
		check1->SetCheck(BST_CHECKED);
	}
	else {
		OVERWRITE = L"";
		CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE);
		check1->SetCheck(BST_UNCHECKED);
	}
	if (preset == 0) {
		m_PRESET.SetCurSel(0);
		FINALPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + L" -vcodec hevc_nvenc" + L" -acodec aac" + L" -b:a 192k" + L" -y" + L" %OutString%";
		CButton* abutton = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		CEdit* acedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_CODEC);
		CEdit* vcedit = (CEdit*)GetDlgItem(IDC_EDIT_VIDEO_CODEC);
		CEdit* aedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_BITRATE);
		abutton->EnableWindow(FALSE);
		vcedit->EnableWindow(FALSE);
		vcedit->SetWindowText(_T("hevc_nvenc"));
		acedit->EnableWindow(FALSE);
		acedit->SetWindowText(_T("aac"));
		aedit->EnableWindow(FALSE);
		aedit->SetWindowText(_T("192"));
		VIDEOCODEC = L"";
		AUDIOCODEC = L"";
		AUDIOBITRATE = L"";
	}
	else if (preset == 1) {
		m_PRESET.SetCurSel(1);
		CButton* abutton = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		CEdit* acedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_CODEC);
		CEdit* vcedit = (CEdit*)GetDlgItem(IDC_EDIT_VIDEO_CODEC);
		CEdit* aedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_BITRATE);
		abutton->EnableWindow(TRUE);
		vcedit->EnableWindow(TRUE);
		vcedit->SetWindowText(_T("hevc_nvenc"));
		acedit->EnableWindow(TRUE);
		acedit->SetWindowText(_T("aac"));
		aedit->EnableWindow(TRUE);
		aedit->SetWindowText(_T("192"));
		if (abutton->GetCheck() == BST_UNCHECKED) {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
		}
	}
	else {
		m_PRESET.SetCurSel(0);
		FINALPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + L" -vcodec hevc_nvenc" + L" -acodec aac" + L" -b:a 192k" + L" -y" + L" %OutString%";
		CButton* abutton = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		CEdit* acedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_CODEC);
		CEdit* vcedit = (CEdit*)GetDlgItem(IDC_EDIT_VIDEO_CODEC);
		CEdit* aedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_BITRATE);
		abutton->EnableWindow(FALSE);
		vcedit->EnableWindow(FALSE);
		vcedit->SetWindowText(_T("hevc_nvenc"));
		acedit->EnableWindow(FALSE);
		acedit->SetWindowText(_T("aac"));
		aedit->EnableWindow(FALSE);
		aedit->SetWindowText(_T("192"));
		VIDEOCODEC = L"";
		AUDIOCODEC = L"";
		AUDIOBITRATE = L"";
	}

	Core->LoadImageLibrary();
	m_hImage.Attach((HBITMAP)::LoadBitmap(Core->hinst, MAKEINTRESOURCE(1000)));
	CStatic* box = (CStatic*)GetDlgItem(IDC_STATIC_IMG);
	box->SetBitmap(m_hImage);
	box->InvalidateRect(NULL, 1);

	return TRUE;
}


void FFMPEGSETTINGSDIALOG::OnEnChangeEdit2()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	if (GetDlgItem(IDC_EDIT2)->GetWindowTextLength() == 0) {
		FPS = L"";
		FPS2 = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else if (GetDlgItem(IDC_EDIT2)->GetWindowTextLength() != 0) {
		UpdateData(TRUE);
		FPS = L" -framerate " + xv_FPS;
		FPS2 = L" -r " + xv_FPS;
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else {
		FPS = L"";
		FPS2 = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGSETTINGSDIALOG::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	CString comboindex1;
	comboindex1.Format(_T("%d"), index1);

	if (((CComboBox*)GetDlgItem(IDC_COMBO_PRESET))->GetCurSel() == 0) {
		if (FINALPARAM == L"") {
			MessageBox(_T("設定エラー"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
		if (((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowTextLength() != 0) {
			FINALPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + L" -vcodec hevc_nvenc" + L" -acodec aac" + L" -b:a 192k" + L" -y" + L" %OutString%";
		}
		else {
			MessageBox(_T("FPSが指定されていません"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
	}
	else {
		if (CUSTOMPARAM == L"") {
			MessageBox(_T("設定エラー"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
		else {
			FINALPARAM = CUSTOMPARAM;
		}
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_ADVANCED))->GetCheck() != BST_UNCHECKED) {
		if (CUSTOMPARAM == L"") {
			MessageBox(_T("設定エラー"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
		else {
			FINALPARAM = CUSTOMPARAM;
		}
	}


	if (((CComboBox*)GetDlgItem(IDC_COMBO_PRESET))->GetCurSel() != 0) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x0000", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x0000", L"0", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x2000", xv_FPS, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x2000", L"", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_VIDEO_CODEC))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x2001", xv_VIDEOCODEC, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x2001", L"", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_AUDIO_CODEC))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x2002", xv_AUDIOCODEC, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x2002", L"", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_AUDIO_BITRATE))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x2003", xv_AUDIOBITRATE, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x2003", L"", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_ADVANCED))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x1001", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x1001", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_OVERWRITE))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x1002", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x1002", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x1003", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x1003", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_AAC))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x1004", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x1004", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_QP))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x1005", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x1005", L"0", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x2004", xv_QPLEVEL, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x2004", L"", L".\\settings.ini");
	}
	if (FINALPARAM != L"") {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x20FF", FINALPARAM, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_SETTINGS", L"0x20FF", L"", L".\\settings.ini");
	}
	CDialogEx::OnOK();
}


void FFMPEGSETTINGSDIALOG::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


void FFMPEGSETTINGSDIALOG::OnCbnSelchangeComboPreset()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	if (index1 == 0) {
		PRESET = L"DEFAULT";
		CButton* abutton = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		CEdit* acedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_CODEC);
		CEdit* vcedit = (CEdit*)GetDlgItem(IDC_EDIT_VIDEO_CODEC);
		CEdit* aedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_BITRATE);
		CButton* hbutton = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER);
		CButton* qbutton = (CButton*)GetDlgItem(IDC_CHECK_QP);
		CButton* sbutton = (CButton*)GetDlgItem(IDC_CHECK_AAC);
		CButton* owbutton = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE);
		CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
		CEdit* crf = (CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL);
		hbutton->SetCheck(BST_UNCHECKED);
		qbutton->SetCheck(BST_UNCHECKED);
		sbutton->SetCheck(BST_UNCHECKED);
		owbutton->SetCheck(BST_UNCHECKED);
		abutton->SetCheck(BST_UNCHECKED);
		hbutton->EnableWindow(FALSE);
		qbutton->EnableWindow(FALSE);
		sbutton->EnableWindow(FALSE);
		owbutton->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		prm->SetWindowText(NULL);
		crf->EnableWindow(FALSE);
		crf->SetWindowText(NULL);
		abutton->EnableWindow(FALSE);
		vcedit->EnableWindow(FALSE);
		vcedit->SetWindowText(_T("hevc_nvenc"));
		acedit->EnableWindow(FALSE);
		acedit->SetWindowText(_T("aac"));
		aedit->EnableWindow(FALSE);
		aedit->SetWindowText(_T("192"));
		FINALPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + L" -vcodec hevc_nvenc" + L" -acodec aac" + L" -b:a 192k" + L" -y" + L" %OutString%";
		return;
	}
	else if (index1 == 1) {
		PRESET = L"CUSTOM";
		CButton* abutton = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		CEdit* acedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_CODEC);
		CEdit* vcedit = (CEdit*)GetDlgItem(IDC_EDIT_VIDEO_CODEC);
		CEdit* aedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_BITRATE);
		abutton->EnableWindow(TRUE);
		vcedit->EnableWindow(TRUE);
		vcedit->SetWindowText(_T("hevc_nvenc"));
		acedit->EnableWindow(TRUE);
		acedit->SetWindowText(_T("aac"));
		aedit->EnableWindow(TRUE);
		aedit->SetWindowText(_T("192"));
		VIDEOCODEC = L" -vcodec hevc_nvenc";
		AUDIOCODEC = L" -acodec aac";
		AUDIOBITRATE = L" -b:a 192k";
		if (abutton->GetCheck() == BST_UNCHECKED) {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
		}
		return;
	}
	else {
		PRESET = L"";
		CButton* abutton = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		CEdit* acedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_CODEC);
		CEdit* vcedit = (CEdit*)GetDlgItem(IDC_EDIT_VIDEO_CODEC);
		CEdit* aedit = (CEdit*)GetDlgItem(IDC_EDIT_AUDIO_BITRATE);
		CButton* hbutton = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER);
		CButton* qbutton = (CButton*)GetDlgItem(IDC_CHECK_QP);
		CButton* sbutton = (CButton*)GetDlgItem(IDC_CHECK_AAC);
		CButton* owbutton = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE);
		CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
		CEdit* crf = (CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL);
		hbutton->SetCheck(BST_UNCHECKED);
		qbutton->SetCheck(BST_UNCHECKED);
		sbutton->SetCheck(BST_UNCHECKED);
		owbutton->SetCheck(BST_UNCHECKED);
		abutton->SetCheck(BST_UNCHECKED);
		hbutton->EnableWindow(FALSE);
		qbutton->EnableWindow(FALSE);
		sbutton->EnableWindow(FALSE);
		owbutton->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		prm->SetWindowText(NULL);
		crf->EnableWindow(FALSE);
		crf->SetWindowText(NULL);
		abutton->EnableWindow(FALSE);
		vcedit->EnableWindow(FALSE);
		vcedit->SetWindowText(NULL);
		acedit->EnableWindow(FALSE);
		acedit->SetWindowText(NULL);
		aedit->EnableWindow(FALSE);
		aedit->SetWindowText(NULL);
		return;
	}
}


void FFMPEGSETTINGSDIALOG::OnEnChangeEditVideoCodec()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	if (GetDlgItem(IDC_EDIT_VIDEO_CODEC)->GetWindowTextLength() == 0) {
		VIDEOCODEC = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
			return;
		}
	}
	else if (GetDlgItem(IDC_EDIT_VIDEO_CODEC)->GetWindowTextLength() != 0) {
		UpdateData(TRUE);
		VIDEOCODEC = L" -vcodec " + xv_VIDEOCODEC;
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
			return;
		}
	}
	else {
		VIDEOCODEC = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
			return;
		}
	}
}


void FFMPEGSETTINGSDIALOG::OnEnChangeEditAudioCodec()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	if (GetDlgItem(IDC_EDIT_AUDIO_CODEC)->GetWindowTextLength() == 0) {
		AUDIOCODEC = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
			return;
		}
	}
	else if (GetDlgItem(IDC_EDIT_AUDIO_CODEC)->GetWindowTextLength() != 0) {
		UpdateData(TRUE);
		AUDIOCODEC = L" -acodec " + xv_AUDIOCODEC;
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
			return;
		}
	}
	else {
		AUDIOCODEC = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
			return;
		}
	}
}


void FFMPEGSETTINGSDIALOG::OnEnChangeEditAudioBitrate()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	if (GetDlgItem(IDC_EDIT_AUDIO_BITRATE)->GetWindowTextLength() == 0) {
		AUDIOBITRATE = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
			return;
		}
	}
	else if (GetDlgItem(IDC_EDIT_AUDIO_BITRATE)->GetWindowTextLength() != 0) {
		UpdateData(TRUE);
		AUDIOBITRATE = L" -b:a " + xv_AUDIOBITRATE + L"k";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
			return;
		}
	}
	else {
		AUDIOBITRATE = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			CUSTOMPARAM = L"ffmpeg" + FPS + L" -hide_banner" + L" -strict -2" + L" -i %InString1%" + L" -i %InString2%" + FPS2 + L" -qp 0" + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + L" -y" + L" %OutString%";
			return;
		}
	}
}


void FFMPEGSETTINGSDIALOG::OnBnClickedCheckAdvanced()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
	CButton* hbutton = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER);
	CButton* qbutton = (CButton*)GetDlgItem(IDC_CHECK_QP);
	CButton* sbutton = (CButton*)GetDlgItem(IDC_CHECK_AAC);
	CButton* owbutton = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE);
	CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
	CEdit* crf = (CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL);
	if (check1->GetCheck() == BST_UNCHECKED) {
		check1->SetCheck(BST_UNCHECKED);
		hbutton->EnableWindow(FALSE);
		qbutton->EnableWindow(FALSE);
		sbutton->EnableWindow(FALSE);
		owbutton->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		crf->EnableWindow(FALSE);
		prm->SetWindowText(NULL);
		HIDEBANNER = L" -hide_banner";
		STRCT = L" -strict -2";
		OVERWRITE = L" -y";
		QP = L" -qp 0";
		CUSTOMPARAM = L"";
		return;
	}
	else if (check1->GetCheck() == BST_CHECKED) {
		int on_button;
		on_button = MessageBox(_T("このオプションは上級者向けです。\n変更すると変換に不具合が生じる恐れがあります。有効にしますか？"), _T("警告"), MB_ICONWARNING | MB_YESNO);
		if (on_button == IDYES) {
			if (check1->GetCheck() == BST_CHECKED) {
				hbutton->EnableWindow(TRUE);
				qbutton->EnableWindow(TRUE);
				sbutton->EnableWindow(TRUE);
				owbutton->EnableWindow(TRUE);
				prm->EnableWindow(TRUE);
				crf->EnableWindow(TRUE);
				HIDEBANNER = L"";
				STRCT = L"";
				OVERWRITE = L"";
				QP = L"";
				CUSTOMPARAM = L"";
				CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
				if (adv->GetCheck() == BST_CHECKED) {
					CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
					CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
					prm->SetWindowText(CUSTOMPARAM);
					return;
				}
				else {
					return;
				}
			}
			else {
				check1->SetCheck(BST_UNCHECKED);
				hbutton->EnableWindow(FALSE);
				qbutton->EnableWindow(FALSE);
				sbutton->EnableWindow(FALSE);
				owbutton->EnableWindow(FALSE);
				prm->EnableWindow(FALSE);
				crf->EnableWindow(FALSE);
				prm->SetWindowText(NULL);
				HIDEBANNER = L" -hide_banner";
				STRCT = L" -strict -2";
				OVERWRITE = L" -y";
				QP = L" -qp 0";
				CUSTOMPARAM = L"";
				return;
			}
		}
		else if (on_button == IDNO) {
			check1->SetCheck(BST_UNCHECKED);
			hbutton->EnableWindow(FALSE);
			qbutton->EnableWindow(FALSE);
			sbutton->EnableWindow(FALSE);
			owbutton->EnableWindow(FALSE);
			prm->EnableWindow(FALSE);
			crf->EnableWindow(FALSE);
			prm->SetWindowText(NULL);
			HIDEBANNER = L" -hide_banner";
			STRCT = L" -strict -2";
			OVERWRITE = L" -y";
			QP = L" -qp 0";
			CUSTOMPARAM = L"";
			return;
		}
		else {
			check1->SetCheck(BST_UNCHECKED);
			hbutton->EnableWindow(FALSE);
			qbutton->EnableWindow(FALSE);
			sbutton->EnableWindow(FALSE);
			owbutton->EnableWindow(FALSE);
			prm->EnableWindow(FALSE);
			crf->EnableWindow(FALSE);
			prm->SetWindowText(NULL);
			HIDEBANNER = L" -hide_banner";
			STRCT = L" -strict -2";
			OVERWRITE = L" -y";
			QP = L" -qp 0";
			CUSTOMPARAM = L"";
			return;
		}
	}
	else {
		check1->SetCheck(BST_UNCHECKED);
		hbutton->EnableWindow(FALSE);
		qbutton->EnableWindow(FALSE);
		sbutton->EnableWindow(FALSE);
		owbutton->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		crf->EnableWindow(FALSE);
		prm->SetWindowText(NULL);
		HIDEBANNER = L" -hide_banner";
		STRCT = L" -strict -2";
		OVERWRITE = L" -y";
		QP = L" -qp 0";
		CUSTOMPARAM = L"";
		return;
	}
}


void FFMPEGSETTINGSDIALOG::OnBnClickedCheckHideBanner()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER);
	if (check1->GetCheck() == BST_UNCHECKED) {
		HIDEBANNER = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else if (check1->GetCheck() == BST_CHECKED) {
		HIDEBANNER = L" -hide_banner";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else {
		HIDEBANNER = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGSETTINGSDIALOG::OnBnClickedCheckQp()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_QP);
	CEdit* crf = (CEdit*)GetDlgItem(IDC_EDIT_QPLEVEL);
	if (check1->GetCheck() == BST_UNCHECKED) {
		crf->EnableWindow(FALSE);
		crf->SetWindowText(NULL);
		QP = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else if (check1->GetCheck() == BST_CHECKED) {
		crf->EnableWindow(TRUE);
		crf->SetWindowText(_T("0"));
		QP = L" -qp 0";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else {
		crf->EnableWindow(FALSE);
		crf->SetWindowText(NULL);
		QP = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGSETTINGSDIALOG::OnEnChangeEditCommandParam()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	if (GetDlgItem(IDC_EDIT_COMMAND_PARAM)->GetWindowTextLength() == 0) {
		FINALPARAM = L"";
	}
	else if (GetDlgItem(IDC_EDIT_COMMAND_PARAM)->GetWindowTextLength() != 0) {
		UpdateData(TRUE);
		FINALPARAM = xv_CUSTOMPARAM;
	}
	else {
		FINALPARAM = L"";
	}
}



void FFMPEGSETTINGSDIALOG::OnEnChangeEditQplevel()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	if (GetDlgItem(IDC_EDIT_QPLEVEL)->GetWindowTextLength() == 0) {
		QP = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else if (GetDlgItem(IDC_EDIT_QPLEVEL)->GetWindowTextLength() != 0) {
		UpdateData(TRUE);
		QP = L" -qp " + xv_QPLEVEL;
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else {
		QP = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGSETTINGSDIALOG::OnBnClickedCheckAac()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_AAC);
	if (check1->GetCheck() == BST_UNCHECKED) {
		STRCT = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else if (check1->GetCheck() == BST_CHECKED) {
		STRCT = L" -strict -2";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else {
		STRCT = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGSETTINGSDIALOG::OnBnClickedCheckOverwrite()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE);
	if (check1->GetCheck() == BST_UNCHECKED) {
		OVERWRITE = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else if (check1->GetCheck() == BST_CHECKED) {
		OVERWRITE = L" -y";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
	else {
		OVERWRITE = L"";
		CButton* adv = (CButton*)GetDlgItem(IDC_CHECK_ADVANCED);
		if (adv->GetCheck() == BST_CHECKED) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM);
			CUSTOMPARAM = L"ffmpeg" + FPS + HIDEBANNER + STRCT + L" -i %InString1%" + L" -i %InString2%" + FPS2 + QP + VIDEOCODEC + AUDIOCODEC + AUDIOBITRATE + OVERWRITE + L" %OutString%";
			prm->SetWindowText(CUSTOMPARAM);
			return;
		}
		else {
			return;
		}
	}
}

void FFMPEGSETTINGSDIALOG::OnDestroy()
{
	CDialogEx::OnDestroy();

	FreeLibrary(Core->hinst);
	DEL_CORE
	DEL_MAINSTR
	Core->FreeImageLibrary();
}
