// FFMPEGAUDIOSETTINGS.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "FFMPEGAUDIOSETTINGS.h"
#include "afxdialogex.h"


// FFMPEGAUDIOSETTINGS ダイアログ

IMPLEMENT_DYNAMIC(FFMPEGAUDIOSETTINGS, CDialogEx)

FFMPEGAUDIOSETTINGS::FFMPEGAUDIOSETTINGS(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FFMPEG_AUDIOSETTINGDIALOG, pParent)
	, xv_OUTNAME_A(_T(""))
	, xv_OUTDIR_A(_T(""))
	, xv_PARAM_A(_T(""))
{

	AUDIOSTREAM_A = _T("");
	AUDIOONLY_A = _T("");
	DISABLESTREAM_A = _T("");
	OVERWRITE_A = _T("");
	HIDEBANNER_A = _T("");
	MAP_CHAPTERS_A = _T("");
	MAP_METADATA_A = _T("");
	OUTDIR_A = _T("");
	OUTNAME_A = _T("");
	FINALPARAM_A = _T("");
	DISABLEDATA_A = _T("");
}

FFMPEGAUDIOSETTINGS::~FFMPEGAUDIOSETTINGS()
{
}

void FFMPEGAUDIOSETTINGS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PRESET_A, m_PRESET_A);
	DDX_Control(pDX, IDC_COMBO_AUDIO_STREAM_A, m_STREAM_A);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_NAME_A, xv_OUTNAME_A);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_A, xv_OUTDIR_A);
	DDX_Text(pDX, IDC_EDIT_COMMAND_PARAM_A, xv_PARAM_A);
}


BEGIN_MESSAGE_MAP(FFMPEGAUDIOSETTINGS, CDialogEx)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET_A, &FFMPEGAUDIOSETTINGS::OnCbnSelchangeComboPreset)
	ON_CBN_SELCHANGE(IDC_COMBO_AUDIO_STREAM_A, &FFMPEGAUDIOSETTINGS::OnCbnSelchangeComboAudioStream)
	ON_BN_CLICKED(IDC_CHECK_AUDIO_ONLY_A, &FFMPEGAUDIOSETTINGS::OnBnClickedCheckAudioOnly)
	ON_BN_CLICKED(IDC_CHECK_D_STREAM_A, &FFMPEGAUDIOSETTINGS::OnBnClickedCheckDStream)
	ON_BN_CLICKED(IDC_CHECK_OVERWRITE_A, &FFMPEGAUDIOSETTINGS::OnBnClickedCheckOverwrite)
	ON_BN_CLICKED(IDC_CHECK_DN_A, &FFMPEGAUDIOSETTINGS::OnBnClickedCheckDn)
	ON_BN_CLICKED(IDC_CHECK_HIDE_BANNER_A, &FFMPEGAUDIOSETTINGS::OnBnClickedCheckHideBanner)
	ON_BN_CLICKED(IDC_CHECK_CHAPTER_A, &FFMPEGAUDIOSETTINGS::OnBnClickedCheckChapter)
	ON_BN_CLICKED(IDC_CHECK_METADATA_A, &FFMPEGAUDIOSETTINGS::OnBnClickedCheckMetadata)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT_NAME_A, &FFMPEGAUDIOSETTINGS::OnEnChangeEditOutputName)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_A, &FFMPEGAUDIOSETTINGS::OnBnClickedButtonOutput)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT_A, &FFMPEGAUDIOSETTINGS::OnEnChangeEditOutput)
	ON_EN_CHANGE(IDC_EDIT_COMMAND_PARAM_A, &FFMPEGAUDIOSETTINGS::OnEnChangeEditCommandParam)
	ON_BN_CLICKED(IDOK, &FFMPEGAUDIOSETTINGS::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &FFMPEGAUDIOSETTINGS::OnBnClickedCancel)
END_MESSAGE_MAP()


// FFMPEGAUDIOSETTINGS メッセージ ハンドラー
BOOL FFMPEGAUDIOSETTINGS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_PRESET_A.AddString(_T("デフォルト"));
	m_PRESET_A.AddString(_T("カスタム"));
	m_STREAM_A.AddString(_T("signed 16-bit PCM"));
	m_STREAM_A.AddString(_T("signed 24-bit PCM"));
	m_STREAM_A.AddString(_T("signed 32-bit PCM"));

	UINT preset, stream, ow, ao, hb, sn, dn, mapm, mapc;

	wchar_t outfile[MAX_PATH], outdir[MAX_PATH], prm[MAX_PATH];

	GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2000", NULL, (LPWSTR)outfile, MAX_PATH, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2001", NULL, (LPWSTR)outdir, MAX_PATH, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x20FF", NULL, (LPWSTR)prm, MAX_PATH, L".\\settings.ini");
	preset = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini");
	stream = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x0001", INFINITE, L".\\settings.ini");
	ow = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x1001", INFINITE, L".\\settings.ini");
	hb = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x1002", INFINITE, L".\\settings.ini");
	ao = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x1003", INFINITE, L".\\settings.ini");
	sn = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x1004", INFINITE, L".\\settings.ini");
	dn = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x1005", INFINITE, L".\\settings.ini");
	mapc = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x1006", INFINITE, L".\\settings.ini");
	mapm = GetPrivateProfileInt(L"FFMPEG_AUDIO_SETTINGS", L"0x1007", INFINITE, L".\\settings.ini");

	if (outdir != L"") {
		CEdit* outf = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_A);
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_A);
		OUTDIR_A = L" ";
		OUTDIR_A + (LPCTSTR)outdir;
		outn->EnableWindow(TRUE);
		outf->SetWindowText((LPCTSTR)outdir);
	}
	else {
		CEdit* outf = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_A);
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_A);
		OUTDIR_A = L"";
		outn->EnableWindow(FALSE);
		outf->SetWindowText(OUTDIR_A);
	}
	if (outfile != L"") {
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_A);
		OUTNAME_A = (LPCTSTR)outfile;
		outn->SetWindowText((LPCTSTR)outfile);
	}
	else {
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_A);
		OUTNAME_A = L"";
		outn->SetWindowText(OUTNAME_A);
	}
	if (ow == 1) {
		OVERWRITE_A = L" -y";
		CButton* ow = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_A);
		ow->SetCheck(BST_CHECKED);
	}
	else {
		OVERWRITE_A = L"";
		CButton* ow = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_A);
		ow->SetCheck(BST_UNCHECKED);
	}
	if (hb == 1) {
		HIDEBANNER_A = L" -hide_banner";
		CButton* hb = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_A);
		hb->SetCheck(BST_CHECKED);
	}
	else {
		HIDEBANNER_A = L"";
		CButton* hb = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_A);
		hb->SetCheck(BST_UNCHECKED);
	}
	if (ao == 1) {
		AUDIOONLY_A = L" -vn";
		CButton* aoB = (CButton*)GetDlgItem(IDC_CHECK_AUDIO_ONLY_A);
		aoB->SetCheck(BST_CHECKED);
	}
	else {
		AUDIOONLY_A = L"";
		CButton* aoB = (CButton*)GetDlgItem(IDC_CHECK_AUDIO_ONLY_A);
		aoB->SetCheck(BST_UNCHECKED);
	}
	if (sn == 1) {
		DISABLESTREAM_A = L" -sn";
		CButton* sbB = (CButton*)GetDlgItem(IDC_CHECK_D_STREAM_A);
		sbB->SetCheck(BST_CHECKED);
	}
	else {
		DISABLESTREAM_A = L"";
		CButton* sbB = (CButton*)GetDlgItem(IDC_CHECK_D_STREAM_A);
		sbB->SetCheck(BST_UNCHECKED);
	}
	if (dn == 1) {
		DISABLEDATA_A = L" -dn";
		CButton* dnB = (CButton*)GetDlgItem(IDC_CHECK_DN_A);
		dnB->SetCheck(BST_CHECKED);
	}
	else {
		DISABLEDATA_A = L"";
		CButton* dnB = (CButton*)GetDlgItem(IDC_CHECK_DN_A);
		dnB->SetCheck(BST_UNCHECKED);
	}
	if (mapc == 1) {
		MAP_CHAPTERS_A = L" -map_chapters -1";
		CButton* chB = (CButton*)GetDlgItem(IDC_CHECK_CHAPTER_A);
		chB->SetCheck(BST_CHECKED);
	}
	else {
		MAP_CHAPTERS_A = L"";
		CButton* chB = (CButton*)GetDlgItem(IDC_CHECK_CHAPTER_A);
		chB->SetCheck(BST_UNCHECKED);
	}
	if (mapm == 1) {
		MAP_METADATA_A = L" -map_metadata -1";
		CButton* meB = (CButton*)GetDlgItem(IDC_CHECK_METADATA_A);
		meB->SetCheck(BST_CHECKED);
	}
	else {
		MAP_METADATA_A = L"";
		CButton* meB = (CButton*)GetDlgItem(IDC_CHECK_METADATA_A);
		meB->SetCheck(BST_UNCHECKED);
	}
	if (stream == 0) {
		AUDIOSTREAM_A = L" -c:a pcm_s16le";
		CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A);
		m_STREAM_A.SetCurSel(0);
	}
	else if (stream == 1) {
		AUDIOSTREAM_A = L" -c:a pcm_s24le";
		CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A);
		m_STREAM_A.SetCurSel(1);
	}
	else if (stream == 2) {
		AUDIOSTREAM_A = L" -c:a pcm_s32le";
		CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A);
		m_STREAM_A.SetCurSel(2);
	}
	else {
		AUDIOSTREAM_A = L" -c:a pcm_s24le";
		CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A);
		m_STREAM_A.SetCurSel(1);
	}
	if (prm != L"") {
		FINALPARAM_A = (LPCTSTR)prm;
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			prm->SetWindowText(FINALPARAM_A);
		}
	}
	else {
		FINALPARAM_A = L"";
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			prm->SetWindowText(FINALPARAM_A);
		}
	}

	if (preset == 1) {
		CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		CComboBox* combo2 = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A);
		CButton* owB = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_A);
		CButton* aoB = (CButton*)GetDlgItem(IDC_CHECK_AUDIO_ONLY_A);
		CButton* outfB = (CButton*)GetDlgItem(IDC_BUTTON_OUTPUT_A);
		CButton* hbB = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_A);
		CButton* sbB = (CButton*)GetDlgItem(IDC_CHECK_D_STREAM_A);
		CButton* dnB = (CButton*)GetDlgItem(IDC_CHECK_DN_A);
		CButton* chB = (CButton*)GetDlgItem(IDC_CHECK_CHAPTER_A);
		CButton* meB = (CButton*)GetDlgItem(IDC_CHECK_METADATA_A);
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_A);
		CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
		m_PRESET_A.SetCurSel(1);
		owB->EnableWindow(TRUE);
		aoB->EnableWindow(TRUE);
		outfB->EnableWindow(TRUE);
		hbB->EnableWindow(TRUE);
		sbB->EnableWindow(TRUE);
		dnB->EnableWindow(TRUE);
		chB->EnableWindow(TRUE);
		meB->EnableWindow(TRUE);
		outn->EnableWindow(TRUE);
		prm->EnableWindow(TRUE);
		combo2->EnableWindow(TRUE);
		FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
		prm->SetWindowText(FINALPARAM_A);
	}
	else if (preset == 0) {
		CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		CComboBox* combo2 = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A);
		CButton* owB = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_A);
		CButton* aoB = (CButton*)GetDlgItem(IDC_CHECK_AUDIO_ONLY_A);
		CButton* outfB = (CButton*)GetDlgItem(IDC_BUTTON_OUTPUT_A);
		CButton* hbB = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_A);
		CButton* sbB = (CButton*)GetDlgItem(IDC_CHECK_D_STREAM_A);
		CButton* dnB = (CButton*)GetDlgItem(IDC_CHECK_DN_A);
		CButton* chB = (CButton*)GetDlgItem(IDC_CHECK_CHAPTER_A);
		CButton* meB = (CButton*)GetDlgItem(IDC_CHECK_METADATA_A);
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_A);
		CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
		m_PRESET_A.SetCurSel(0);
		owB->SetCheck(BST_UNCHECKED);
		aoB->SetCheck(BST_UNCHECKED);
		hbB->SetCheck(BST_UNCHECKED);
		sbB->SetCheck(BST_UNCHECKED);
		dnB->SetCheck(BST_UNCHECKED);
		chB->SetCheck(BST_UNCHECKED);
		meB->SetCheck(BST_UNCHECKED);
		owB->EnableWindow(FALSE);
		aoB->EnableWindow(FALSE);
		outfB->EnableWindow(FALSE);
		hbB->EnableWindow(FALSE);
		sbB->EnableWindow(FALSE);
		dnB->EnableWindow(FALSE);
		chB->EnableWindow(FALSE);
		meB->EnableWindow(FALSE);
		outn->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		combo2->EnableWindow(FALSE);
		outn->SetWindowText(NULL);
		prm->SetWindowText(NULL);
		FINALPARAM_A = L"ffmpeg -hide_banner -i %InString% -map_chapters -1 -map_metadata -1 -vn -sn -dn -c:a pcm_s24le -y %OutString%";
	}
	else {
		m_PRESET_A.SetCurSel(0);
		CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		CComboBox* combo2 = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A);
		CButton* owB = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_A);
		CButton* aoB = (CButton*)GetDlgItem(IDC_CHECK_AUDIO_ONLY_A);
		CButton* outfB = (CButton*)GetDlgItem(IDC_BUTTON_OUTPUT_A);
		CButton* hbB = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_A);
		CButton* sbB = (CButton*)GetDlgItem(IDC_CHECK_D_STREAM_A);
		CButton* dnB = (CButton*)GetDlgItem(IDC_CHECK_DN_A);
		CButton* chB = (CButton*)GetDlgItem(IDC_CHECK_CHAPTER_A);
		CButton* meB = (CButton*)GetDlgItem(IDC_CHECK_METADATA_A);
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_A);
		CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
		owB->SetCheck(BST_UNCHECKED);
		aoB->SetCheck(BST_UNCHECKED);
		hbB->SetCheck(BST_UNCHECKED);
		sbB->SetCheck(BST_UNCHECKED);
		dnB->SetCheck(BST_UNCHECKED);
		chB->SetCheck(BST_UNCHECKED);
		meB->SetCheck(BST_UNCHECKED);
		owB->EnableWindow(FALSE);
		aoB->EnableWindow(FALSE);
		outfB->EnableWindow(FALSE);
		hbB->EnableWindow(FALSE);
		sbB->EnableWindow(FALSE);
		dnB->EnableWindow(FALSE);
		chB->EnableWindow(FALSE);
		meB->EnableWindow(FALSE);
		outn->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		combo2->EnableWindow(FALSE);
		outn->SetWindowText(NULL);
		prm->SetWindowText(NULL);
		FINALPARAM_A = L"ffmpeg -hide_banner -i %InString% -map_chapters -1 -map_metadata -1 -vn -sn -dn -c:a pcm_s24le -y %OutString%";
	}

	Core->LoadImageLibrary();
	m_hImage.Attach((HBITMAP)::LoadBitmap(Core->hinst, MAKEINTRESOURCE(1000)));
	CStatic* box = (CStatic*)GetDlgItem(IDC_STATIC_IMG);
	box->SetBitmap(m_hImage);
	box->InvalidateRect(NULL, 1);

	return TRUE;
}

void FFMPEGAUDIOSETTINGS::OnCbnSelchangeComboPreset()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
	CComboBox* combo2 = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A);
	CButton* owB = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_A);
	CButton* aoB = (CButton*)GetDlgItem(IDC_CHECK_AUDIO_ONLY_A);
	CButton* outfB = (CButton*)GetDlgItem(IDC_BUTTON_OUTPUT_A);
	CButton* hbB = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_A);
	CButton* sbB = (CButton*)GetDlgItem(IDC_CHECK_D_STREAM_A);
	CButton* dnB = (CButton*)GetDlgItem(IDC_CHECK_DN_A);
	CButton* chB = (CButton*)GetDlgItem(IDC_CHECK_CHAPTER_A);
	CButton* meB = (CButton*)GetDlgItem(IDC_CHECK_METADATA_A);
	CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_A);
	CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	if (index1 == 0) {
		owB->SetCheck(BST_UNCHECKED);
		aoB->SetCheck(BST_UNCHECKED);
		hbB->SetCheck(BST_UNCHECKED);
		sbB->SetCheck(BST_UNCHECKED);
		dnB->SetCheck(BST_UNCHECKED);
		chB->SetCheck(BST_UNCHECKED);
		meB->SetCheck(BST_UNCHECKED);
		owB->EnableWindow(FALSE);
		aoB->EnableWindow(FALSE);
		outfB->EnableWindow(FALSE);
		hbB->EnableWindow(FALSE);
		sbB->EnableWindow(FALSE);
		dnB->EnableWindow(FALSE);
		chB->EnableWindow(FALSE);
		meB->EnableWindow(FALSE);
		outn->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		combo2->EnableWindow(FALSE);
		outn->SetWindowText(NULL);
		prm->SetWindowText(NULL);
		FINALPARAM_A = L"ffmpeg -hide_banner -i %InString% -map_chapters -1 -map_metadata -1 -vn -sn -dn -c:a pcm_s24le -y %OutString%";
	}
	else if (index1 == 1) {
		owB->EnableWindow(TRUE);
		aoB->EnableWindow(TRUE);
		outfB->EnableWindow(TRUE);
		hbB->EnableWindow(TRUE);
		sbB->EnableWindow(TRUE);
		dnB->EnableWindow(TRUE);
		chB->EnableWindow(TRUE);
		meB->EnableWindow(TRUE);
		outn->EnableWindow(TRUE);
		prm->EnableWindow(TRUE);
		combo2->EnableWindow(TRUE);
		FINALPARAM_A = L"ffmpeg -i %InString% %OutString%";
		prm->SetWindowText(FINALPARAM_A);
	}
	else {
		owB->SetCheck(BST_UNCHECKED);
		aoB->SetCheck(BST_UNCHECKED);
		hbB->SetCheck(BST_UNCHECKED);
		sbB->SetCheck(BST_UNCHECKED);
		dnB->SetCheck(BST_UNCHECKED);
		chB->SetCheck(BST_UNCHECKED);
		meB->SetCheck(BST_UNCHECKED);
		owB->EnableWindow(FALSE);
		aoB->EnableWindow(FALSE);
		outfB->EnableWindow(FALSE);
		hbB->EnableWindow(FALSE);
		sbB->EnableWindow(FALSE);
		dnB->EnableWindow(FALSE);
		chB->EnableWindow(FALSE);
		meB->EnableWindow(FALSE);
		outn->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		combo2->EnableWindow(FALSE);
		outn->SetWindowText(NULL);
		prm->SetWindowText(NULL);
		FINALPARAM_A = L"";
	}
}


void FFMPEGAUDIOSETTINGS::OnCbnSelchangeComboAudioStream()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	if (index1 == 0) {
		AUDIOSTREAM_A = L" -c:a pcm_s16le";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 1) {
		AUDIOSTREAM_A = L" -c:a pcm_s24le";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else if (index1 == 2) {
		AUDIOSTREAM_A = L" -c:a pcm_s32le";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else {
		AUDIOSTREAM_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGAUDIOSETTINGS::OnBnClickedCheckAudioOnly()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* button = (CButton*)GetDlgItem(IDC_CHECK_AUDIO_ONLY_A);
	if (button->GetCheck() == BST_CHECKED) {
		AUDIOONLY_A = L" -vn";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else if (button->GetCheck() == BST_UNCHECKED) {
		AUDIOONLY_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else {
		AUDIOONLY_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGAUDIOSETTINGS::OnBnClickedCheckDStream()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* button = (CButton*)GetDlgItem(IDC_CHECK_D_STREAM_A);
	if (button->GetCheck() == BST_CHECKED) {
		DISABLESTREAM_A = L" -sn";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else if (button->GetCheck() == BST_UNCHECKED) {
		DISABLESTREAM_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else {
		DISABLESTREAM_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGAUDIOSETTINGS::OnBnClickedCheckOverwrite()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* button = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_A);
	if (button->GetCheck() == BST_CHECKED) {
		OVERWRITE_A = L" -y";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else if (button->GetCheck() == BST_UNCHECKED) {
		OVERWRITE_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else {
		OVERWRITE_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGAUDIOSETTINGS::OnBnClickedCheckDn()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* button = (CButton*)GetDlgItem(IDC_CHECK_DN_A);
	if (button->GetCheck() == BST_CHECKED) {
		DISABLEDATA_A = L" -dn";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else if (button->GetCheck() == BST_UNCHECKED) {
		DISABLEDATA_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else {
		DISABLEDATA_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGAUDIOSETTINGS::OnBnClickedCheckHideBanner()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* button = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_A);
	if (button->GetCheck() == BST_CHECKED) {
		HIDEBANNER_A = L" -hide_banner";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else if (button->GetCheck() == BST_UNCHECKED) {
		HIDEBANNER_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else {
		HIDEBANNER_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGAUDIOSETTINGS::OnBnClickedCheckChapter()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* button = (CButton*)GetDlgItem(IDC_CHECK_CHAPTER_A);
	if (button->GetCheck() == BST_CHECKED) {
		MAP_CHAPTERS_A = L" -map_chapters -1";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else if (button->GetCheck() == BST_UNCHECKED) {
		MAP_CHAPTERS_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else {
		MAP_CHAPTERS_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGAUDIOSETTINGS::OnBnClickedCheckMetadata()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* button = (CButton*)GetDlgItem(IDC_CHECK_METADATA_A);
	if (button->GetCheck() == BST_CHECKED) {
		MAP_METADATA_A = L" -map_metadata -1";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else if (button->GetCheck() == BST_UNCHECKED) {
		MAP_METADATA_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else {
		MAP_METADATA_A = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGAUDIOSETTINGS::OnEnChangeEditOutputName()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	CComboBox* pr = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
	CEdit* out = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_A);
	if (pr->GetCurSel() != 0) {
		if (GetDlgItem(IDC_EDIT_OUTPUT_A)->GetWindowTextLength() == 0) {
			if (GetDlgItem(IDC_EDIT_OUTPUT_NAME_A)->GetWindowTextLength() != 0) {
				UpdateData(FALSE);
				OUTNAME_A = _T("");
				MessageBox(_T("出力ディレクトリが指定されていません。\n先にディレクトリを指定してください。"), _T("エラー"), MB_ICONERROR | MB_OK);
				CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
				if (preset != 0) {
					CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
					FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
					prm->SetWindowText(FINALPARAM_A);
					out->SetWindowText(L"");
					return;
				}
				else {
					out->SetWindowText(L"");
					return;
				}
			}
		}
		else if (GetDlgItem(IDC_EDIT_OUTPUT_A)->GetWindowTextLength() != 0) {
			if (GetDlgItem(IDC_EDIT_OUTPUT_NAME_A)->GetWindowTextLength() != 0) {
				UpdateData(TRUE);
				OUTNAME_A = xv_OUTNAME_A;
				CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
				if (preset != 0) {
					CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
					FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
					prm->SetWindowText(FINALPARAM_A);
					return;
				}
				else {
					return;
				}
			}
			else {
				OUTNAME_A = _T("");
				CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
				if (preset != 0) {
					CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
					FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
					prm->SetWindowText(FINALPARAM_A);
					return;
				}
				else {
					return;
				}
			}
		}
		else {
			OUTNAME_A = _T("");
			CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
			if (preset != 0) {
				CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
				FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
				prm->SetWindowText(FINALPARAM_A);
				return;
			}
			else {
				return;
			}
		}
	}
	else {
		return;
	}
}


void FFMPEGAUDIOSETTINGS::OnBnClickedButtonOutput()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	wchar_t name[MAX_PATH], dir[MAX_PATH];
	BROWSEINFO  binfo;
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
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
	else
	{
		SHGetPathFromIDList(idlist, (LPWSTR)dir);
		xv_OUTDIR_A = (LPCTSTR)dir;
		OUTDIR_A = L" " + xv_OUTDIR_A;
		UpdateData(FALSE);
		CoTaskMemFree(idlist);
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A);
			FINALPARAM_A = L"ffmpeg" + HIDEBANNER_A + L" -i %InString%" + MAP_CHAPTERS_A + MAP_METADATA_A + AUDIOONLY_A + DISABLESTREAM_A + DISABLEDATA_A + AUDIOSTREAM_A + OVERWRITE_A + L" %OutString%";
			prm->SetWindowText(FINALPARAM_A);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGAUDIOSETTINGS::OnEnChangeEditOutput()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
}


void FFMPEGAUDIOSETTINGS::OnEnChangeEditCommandParam()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	if (GetDlgItem(IDC_EDIT_COMMAND_PARAM_A)->GetWindowTextLength() == 0) {
		FINALPARAM_A = _T("");
	}
	else if (GetDlgItem(IDC_EDIT_COMMAND_PARAM_A)->GetWindowTextLength() != 0) {
		UpdateData(TRUE);
		FINALPARAM_A = xv_PARAM_A;
	}
	else {
		FINALPARAM_A = _T("");
	}
}


void FFMPEGAUDIOSETTINGS::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	/*TCHAR crDir[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH + 1, crDir);
	CString outDir = L"\\Resources\\takeout\\";
	CString currentPath = crDir + outDir;
	CString audion = L"audio.wav";
	CString audioOut = currentPath + L"_temp-project\\streams";*/
	CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A);
	CComboBox* audios = (CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A);
	int iaudios = audios->GetCurSel();
	CString ia;
	ia.Format(_T("%d"), iaudios);

	if (preset->GetCurSel() == 1) {
		if (GetDlgItem(IDC_EDIT_OUTPUT_A)->GetWindowTextLength() == 0) {
			MessageBox(_T("出力ディレクトリが指定されていません"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
		else if (GetDlgItem(IDC_EDIT_OUTPUT_NAME_A)->GetWindowTextLength() == 0) {
			MessageBox(_T("出力ファイル名が指定されていません"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
		else if (GetDlgItem(IDC_EDIT_COMMAND_PARAM_A)->GetWindowTextLength() == 0) {
			MessageBox(_T("パラメーターが指定されていません"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
	}
	else if (preset->GetCurSel() == 0) {
		if (FINALPARAM_A == L"") {
			MessageBox(_T("設定エラー"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
	}

	if (((CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A))->GetCurSel() != 0) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x0000", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x0000", L"0", L".\\settings.ini");
	}
	if (((CComboBox*)GetDlgItem(IDC_COMBO_AUDIO_STREAM_A))->GetCurSel() != -1) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x0001", ia, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x0001", L"", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_A))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1001", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1001", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_A))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1002", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1002", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_AUDIO_ONLY_A))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1003", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1003", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_D_STREAM_A))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1004", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1004", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_DN_A))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1005", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1005", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_CHAPTER_A))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1006", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1006", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_METADATA_A))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1007", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1007", L"0", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_A))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1FFF", L".wav", L".\\settings.ini");
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2000", xv_OUTNAME_A, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x1FFF", L"", L".\\settings.ini");
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2000", L"", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_A))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2001", xv_OUTDIR_A, L".\\settings.ini");
		if (((CComboBox*)GetDlgItem(IDC_COMBO_PRESET_A))->GetCurSel() == 0) {
			WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2001", L"", L".\\settings.ini");
		}
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x2001", L"", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PARAM_A))->GetWindowTextLength() != 0) {
		//replaceOtherStr(xv_PARAM_A, L"%OutString%", OUTDIR_A + OUTNAME_A + L".wav");
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x20FF", xv_PARAM_A, L".\\settings.ini");
	}
	else if (FINALPARAM_A != L"") {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x20FF", FINALPARAM_A, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_AUDIO_SETTINGS", L"0x20FF", L"", L".\\settings.ini");
	}

	CDialogEx::OnOK();
}


void FFMPEGAUDIOSETTINGS::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


void FFMPEGAUDIOSETTINGS::OnDestroy()
{
	CDialogEx::OnDestroy();

	FreeLibrary(Core->hinst);
	DEL_CORE
	Core->FreeImageLibrary();
}