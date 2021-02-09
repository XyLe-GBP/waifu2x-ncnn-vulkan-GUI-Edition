// FFMPEGVIDEOSETTINGS.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "FFMPEGVIDEOSETTINGS.h"
#include "afxdialogex.h"
//#include "replacestr.h"


// FFMPEGVIDEOSETTINGS ダイアログ

IMPLEMENT_DYNAMIC(FFMPEGVIDEOSETTINGS, CDialogEx)

FFMPEGVIDEOSETTINGS::FFMPEGVIDEOSETTINGS(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FFMPEG_VIDEOSETTINGDIALOG, pParent)
	, xv_OUTNAME(_T(""))
	, xv_OUTDIR(_T(""))
	, xv_PARAM(_T(""))
{

	OVERWRITE = _T("");
	MULTI = _T("");
	HIDEBANNER = _T("");
	OUTNAME = _T("");
	OUTDIR = _T("");
	FINALPARAM_V = _T("");
}

FFMPEGVIDEOSETTINGS::~FFMPEGVIDEOSETTINGS()
{
}

void FFMPEGVIDEOSETTINGS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PRESET_V, m_PRESET);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_NAME_V, xv_OUTNAME);
	DDX_Text(pDX, IDC_EDIT_OUTPUT_V, xv_OUTDIR);
	DDX_Text(pDX, IDC_EDIT_COMMAND_PRM_V, xv_PARAM);
}


BEGIN_MESSAGE_MAP(FFMPEGVIDEOSETTINGS, CDialogEx)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET_V, &FFMPEGVIDEOSETTINGS::OnCbnSelchangeComboPreset)
	ON_BN_CLICKED(IDC_CHECK_OVERWRITE_V, &FFMPEGVIDEOSETTINGS::OnBnClickedCheckOverwrite)
	ON_BN_CLICKED(IDC_CHECK_MULTI_V, &FFMPEGVIDEOSETTINGS::OnBnClickedCheckMulti)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT_NAME_V, &FFMPEGVIDEOSETTINGS::OnEnChangeEditOutputName)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_V, &FFMPEGVIDEOSETTINGS::OnBnClickedButtonOutput)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT_V, &FFMPEGVIDEOSETTINGS::OnEnChangeEditOutput)
	ON_EN_CHANGE(IDC_EDIT_COMMAND_PRM_V, &FFMPEGVIDEOSETTINGS::OnEnChangeEditCommandPrm)
	ON_BN_CLICKED(IDOK, &FFMPEGVIDEOSETTINGS::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &FFMPEGVIDEOSETTINGS::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_HIDE_BANNER_V, &FFMPEGVIDEOSETTINGS::OnBnClickedCheckHideBanner)
END_MESSAGE_MAP()


// FFMPEGVIDEOSETTINGS メッセージ ハンドラー
BOOL FFMPEGVIDEOSETTINGS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_PRESET.AddString(_T("デフォルト"));
	m_PRESET.AddString(_T("カスタム"));

	UINT preset, ow, multi, hb;

	wchar_t outfile[MAX_PATH], outdir[MAX_PATH], prm[MAX_PATH];

	GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2000", NULL, (LPWSTR)outfile, MAX_PATH, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2001", NULL, (LPWSTR)outdir, MAX_PATH, L".\\settings.ini");
	GetPrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x20FF", NULL, (LPWSTR)prm, MAX_PATH, L".\\settings.ini");
	preset = GetPrivateProfileInt(L"FFMPEG_VIDEO_SETTINGS", L"0x0000", INFINITE, L".\\settings.ini");
	ow = GetPrivateProfileInt(L"FFMPEG_VIDEO_SETTINGS", L"0x1001", INFINITE, L".\\settings.ini");
	multi = GetPrivateProfileInt(L"FFMPEG_VIDEO_SETTINGS", L"0x1002", INFINITE, L".\\settings.ini");
	hb = GetPrivateProfileInt(L"FFMPEG_VIDEO_SETTINGS", L"0x1003", INFINITE, L".\\settings.ini");

	if (outdir != L"") {
		CEdit* outf = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_V);
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_V);
		OUTDIR = L" ";
		OUTDIR + (LPCTSTR)outdir;
		outn->EnableWindow(TRUE);
		outf->SetWindowText((LPCTSTR)outdir);
	}
	else {
		CEdit* outf = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_V);
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_V);
		OUTDIR = L"";
		outn->EnableWindow(FALSE);
		outf->SetWindowText(OUTDIR);
	}
	if (outfile != L"") {
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_V);
		OUTNAME = (LPCTSTR)outfile;
		outn->SetWindowText((LPCTSTR)outfile);
	}
	else {
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_V);
		OUTNAME = L"";
		outn->SetWindowText(OUTNAME);
	}
	if (ow == 1) {
		OVERWRITE = L" -y";
		CButton* ow = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_V);
		ow->SetCheck(BST_CHECKED);
	}
	else {
		OVERWRITE = L"";
		CButton* ow = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_V);
		ow->SetCheck(BST_UNCHECKED);
	}
	if (multi == 1) {
		if (outfile != L"") {
			if (outdir != L"") {
				MULTI = L"-%09d.png";
			}
			else {
				MULTI = L"";
			}
		}
		else {
			MULTI = L"";
		}
		CButton* multi = (CButton*)GetDlgItem(IDC_CHECK_MULTI_V);
		multi->SetCheck(BST_CHECKED);
	}
	else {
		if (outfile != L"") {
			if (outdir != L"") {
				MULTI = L".png";
			}
			else {
				MULTI = L"";
			}
		}
		else {
			MULTI = L"";
		}
		CButton* multi = (CButton*)GetDlgItem(IDC_CHECK_MULTI_V);
		multi->SetCheck(BST_UNCHECKED);
	}
	if (hb == 1) {
		HIDEBANNER = L" -hide_banner";
		CButton* hb = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_V);
		hb->SetCheck(BST_CHECKED);
	}
	else {
		HIDEBANNER = L"";
		CButton* hb = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_V);
		hb->SetCheck(BST_UNCHECKED);
	}
	if (prm != L"") {
		FINALPARAM_V = (LPCTSTR)prm;
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
			prm->SetWindowText(FINALPARAM_V);
		}
	}
	else {
		FINALPARAM_V = L"";
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
			prm->SetWindowText(FINALPARAM_V);
		}
	}
	if (preset == 1) {
		m_PRESET.SetCurSel(1);
		CButton* ow = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_V);
		CButton* multi = (CButton*)GetDlgItem(IDC_CHECK_MULTI_V);
		CButton* outf = (CButton*)GetDlgItem(IDC_BUTTON_OUTPUT_V);
		CButton* hb = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_V);
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_V);
		CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
		ow->EnableWindow(TRUE);
		multi->EnableWindow(TRUE);
		outf->EnableWindow(TRUE);
		outn->EnableWindow(TRUE);
		hb->EnableWindow(TRUE);
		prm->EnableWindow(TRUE);
		FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
	}
	else if (preset == 0) {
		m_PRESET.SetCurSel(0);
		CButton* ow = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_V);
		CButton* multi = (CButton*)GetDlgItem(IDC_CHECK_MULTI_V);
		CButton* outf = (CButton*)GetDlgItem(IDC_BUTTON_OUTPUT_V);
		CButton* hb = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_V);
		CEdit* outfe = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_V);
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_V);
		CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
		ow->SetCheck(BST_UNCHECKED);
		multi->SetCheck(BST_UNCHECKED);
		hb->SetCheck(BST_UNCHECKED);
		ow->EnableWindow(FALSE);
		multi->EnableWindow(FALSE);
		outf->EnableWindow(FALSE);
		outn->EnableWindow(FALSE);
		hb->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		outfe->SetWindowText(NULL);
		outn->SetWindowText(NULL);
		prm->SetWindowText(NULL);
		FINALPARAM_V = L"ffmpeg -hide_banner -i %InString% -q:v 1 -y %OutString%";
	}
	else {
		m_PRESET.SetCurSel(0);
		CButton* ow = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_V);
		CButton* multi = (CButton*)GetDlgItem(IDC_CHECK_MULTI_V);
		CButton* outf = (CButton*)GetDlgItem(IDC_BUTTON_OUTPUT_V);
		CButton* hb = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_V);
		CEdit* outfe = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_V);
		CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_V);
		CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
		ow->SetCheck(BST_UNCHECKED);
		multi->SetCheck(BST_UNCHECKED);
		hb->SetCheck(BST_UNCHECKED);
		ow->EnableWindow(FALSE);
		multi->EnableWindow(FALSE);
		outf->EnableWindow(FALSE);
		outn->EnableWindow(FALSE);
		hb->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		outfe->SetWindowText(NULL);
		outn->SetWindowText(NULL);
		prm->SetWindowText(NULL);
		FINALPARAM_V = L"ffmpeg -hide_banner -i %InString% -q:v 1 -y %OutString%";
	}

	Core->LoadImageLibrary();
	m_hImage.Attach((HBITMAP)::LoadBitmap(Core->hinst, MAKEINTRESOURCE(1000)));
	CStatic* box = (CStatic*)GetDlgItem(IDC_STATIC_IMG);
	box->SetBitmap(m_hImage);
	box->InvalidateRect(NULL, 1);

	return TRUE;
}


void FFMPEGVIDEOSETTINGS::OnCbnSelchangeComboPreset()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CComboBox* combo1 = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
	CButton* ow = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_V);
	CButton* multi = (CButton*)GetDlgItem(IDC_CHECK_MULTI_V);
	CButton* outf = (CButton*)GetDlgItem(IDC_BUTTON_OUTPUT_V);
	CButton* hb = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_V);
	CEdit* outn = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_V);
	CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
	int index1 = combo1->GetCurSel();
	if (index1 != CB_ERR) {
		TRACE("%d", index1);
	}

	if (index1 == 0) {
		ow->SetCheck(BST_UNCHECKED);
		multi->SetCheck(BST_UNCHECKED);
		hb->SetCheck(BST_UNCHECKED);
		ow->EnableWindow(FALSE);
		multi->EnableWindow(FALSE);
		outf->EnableWindow(FALSE);
		outn->EnableWindow(FALSE);
		hb->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		outn->SetWindowText(NULL);
		prm->SetWindowText(NULL);
		FINALPARAM_V = L"ffmpeg -hide_banner -i %InString% -q:v 1 -y %OutString%";
	}
	else if (index1 == 1) {
		ow->EnableWindow(TRUE);
		multi->EnableWindow(TRUE);
		outf->EnableWindow(TRUE);
		outn->EnableWindow(TRUE);
		hb->EnableWindow(TRUE);
		prm->EnableWindow(TRUE);
		FINALPARAM_V = L"ffmpeg -i %InString% -q:v 1 %OutString%";
		prm->SetWindowText(FINALPARAM_V);
	}
	else {
		ow->SetCheck(BST_UNCHECKED);
		multi->SetCheck(BST_UNCHECKED);
		hb->SetCheck(BST_UNCHECKED);
		ow->EnableWindow(FALSE);
		multi->EnableWindow(FALSE);
		outf->EnableWindow(FALSE);
		outn->EnableWindow(FALSE);
		hb->EnableWindow(FALSE);
		prm->EnableWindow(FALSE);
		outn->SetWindowText(NULL);
		prm->SetWindowText(NULL);
		FINALPARAM_V = L"ffmpeg -hide_banner -i %InString% -q:v 1 -y %OutString%";
	}
}


void FFMPEGVIDEOSETTINGS::OnBnClickedCheckOverwrite()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_V);
	if (check1->GetCheck() == BST_CHECKED) {
		OVERWRITE = L" -y";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
			FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
			prm->SetWindowText(FINALPARAM_V);
			return;
		}
		else {
			return;
		}
	}
	else {
		OVERWRITE = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
			FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
			prm->SetWindowText(FINALPARAM_V);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGVIDEOSETTINGS::OnBnClickedCheckMulti()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_MULTI_V);
	if (check1->GetCheck() == BST_CHECKED) {
		if (GetDlgItem(IDC_EDIT_OUTPUT_V)->GetWindowTextLength() != 0) {
			if (GetDlgItem(IDC_EDIT_OUTPUT_NAME_V)->GetWindowTextLength() != 0) {
				MULTI = L"-%09d.png";
			}
			else {
				MULTI = L"";
			}
		}
		else {
			MULTI = L"";
		}
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
			FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
			prm->SetWindowText(FINALPARAM_V);
			return;
		}
		else {
			return;
		}
	}
	else {
		if (GetDlgItem(IDC_EDIT_OUTPUT_V)->GetWindowTextLength() != 0) {
			if (GetDlgItem(IDC_EDIT_OUTPUT_NAME_V)->GetWindowTextLength() != 0) {
				MULTI = L".png";
			}
			else {
				MULTI = L"";
			}
		}
		else {
			MULTI = L"";
		}
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
			FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
			prm->SetWindowText(FINALPARAM_V);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGVIDEOSETTINGS::OnBnClickedCheckHideBanner()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* check1 = (CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_V);
	if (check1->GetCheck() == BST_CHECKED) {
		HIDEBANNER = L" -hide_banner";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
			FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
			prm->SetWindowText(FINALPARAM_V);
			return;
		}
		else {
			return;
		}
	}
	else {
		HIDEBANNER = L"";
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
			FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
			prm->SetWindowText(FINALPARAM_V);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGVIDEOSETTINGS::OnEnChangeEditOutputName()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	CComboBox* pr = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
	CEdit* out = (CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_V);
	if (pr->GetCurSel() != 0) {
		if (GetDlgItem(IDC_EDIT_OUTPUT_V)->GetWindowTextLength() == 0) {
			if (GetDlgItem(IDC_EDIT_OUTPUT_NAME_V)->GetWindowTextLength() != 0) {
				UpdateData(FALSE);
				OUTNAME = _T("");
				MessageBox(_T("出力ディレクトリが指定されていません。\n先にディレクトリを指定してください。"), _T("エラー"), MB_ICONERROR | MB_OK);
				CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
				if (preset != 0) {
					CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
					FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
					prm->SetWindowText(FINALPARAM_V);
					out->SetWindowText(L"");
					return;
				}
				else {
					out->SetWindowText(L"");
					return;
				}
			}
		}
		else if (GetDlgItem(IDC_EDIT_OUTPUT_V)->GetWindowTextLength() != 0) {
			if (GetDlgItem(IDC_EDIT_OUTPUT_NAME_V)->GetWindowTextLength() != 0) {
				UpdateData(TRUE);
				OUTNAME = xv_OUTNAME;
				CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
				if (preset != 0) {
					CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
					FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
					prm->SetWindowText(FINALPARAM_V);
					return;
				}
				else {
					return;
				}
			}
			else {
				OUTNAME = _T("");
				CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
				if (preset != 0) {
					CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
					FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
					prm->SetWindowText(FINALPARAM_V);
					return;
				}
				else {
					return;
				}
			}
		}
		else {
			OUTNAME = _T("");
			CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
			if (preset != 0) {
				CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
				FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
				prm->SetWindowText(FINALPARAM_V);
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


void FFMPEGVIDEOSETTINGS::OnBnClickedButtonOutput()
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
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
			FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
			prm->SetWindowText(FINALPARAM_V);
			return;
		}
		else {
			return;
		}
	}
	else
	{
		SHGetPathFromIDList(idlist, (LPWSTR)dir);
		xv_OUTDIR = (LPCTSTR)dir;
		OUTDIR = L" " + xv_OUTDIR;
		UpdateData(FALSE);
		CoTaskMemFree(idlist);
		CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);
		if (preset != 0) {
			CEdit* prm = (CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V);
			FINALPARAM_V = L"ffmpeg" + HIDEBANNER + L" -i %InString% -q:v 1" + OVERWRITE + L" %OutString%";
			prm->SetWindowText(FINALPARAM_V);
			return;
		}
		else {
			return;
		}
	}
}


void FFMPEGVIDEOSETTINGS::OnEnChangeEditOutput()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	if (GetDlgItem(IDC_EDIT_OUTPUT_V)->GetWindowTextLength() == 0) {
		OUTDIR = _T("");
	}
	else if (GetDlgItem(IDC_EDIT_OUTPUT_V)->GetWindowTextLength() != 0) {
		UpdateData(TRUE);
		OUTDIR = xv_OUTDIR;
	}
	else {
		OUTDIR = _T("");
	}
}


void FFMPEGVIDEOSETTINGS::OnEnChangeEditCommandPrm()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
	if (GetDlgItem(IDC_EDIT_COMMAND_PRM_V)->GetWindowTextLength() == 0) {
		FINALPARAM_V = _T("");
	}
	else if (GetDlgItem(IDC_EDIT_COMMAND_PRM_V)->GetWindowTextLength() != 0) {
		UpdateData(TRUE);
		FINALPARAM_V = xv_PARAM;
	}
	else {
		FINALPARAM_V = _T("");
	}
}


void FFMPEGVIDEOSETTINGS::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	/*TCHAR crDir[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH + 1, crDir);
	CString outDir = L"\\Resources\\takeout\\";
	CString currentPath = crDir + outDir;
	CString imagen = L"image-%09d.png";
	CString imageOut = currentPath + L"_temp-project\\image-frames";*/
	CComboBox* preset = (CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V);

	if (preset->GetCurSel() == 1) {
		if (GetDlgItem(IDC_EDIT_OUTPUT_V)->GetWindowTextLength() == 0) {
			MessageBox(_T("出力ディレクトリが指定されていません"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
		else if (GetDlgItem(IDC_EDIT_OUTPUT_NAME_V)->GetWindowTextLength() == 0) {
			MessageBox(_T("出力ファイル名が指定されていません"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
		else {

		}
	}
	else if (preset->GetCurSel() == 1) {
		if (FINALPARAM_V == L"") {
			MessageBox(_T("設定エラー"), _T("エラー"), MB_ICONERROR | MB_OK);
			return;
		}
	}

	if (((CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V))->GetCurSel() != 0) {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x0000", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x0000", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_OVERWRITE_V))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1001", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1001", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_MULTI_V))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1002", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1002", L"0", L".\\settings.ini");
	}
	if (((CButton*)GetDlgItem(IDC_CHECK_HIDE_BANNER_V))->GetCheck() != BST_UNCHECKED) {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1003", L"1", L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1003", L"0", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_NAME_V))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1FFF", L".png", L".\\settings.ini");
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2000", xv_OUTNAME, L".\\settings.ini");
		if (((CButton*)GetDlgItem(IDC_CHECK_MULTI_V))->GetCheck() != BST_UNCHECKED) {
			WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x1FFF", L"-%09d.png", L".\\settings.ini");
			WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2000", xv_OUTNAME, L".\\settings.ini");
		}
	}
	else {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2000", L"", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_OUTPUT_V))->GetWindowTextLength() != 0) {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2001", xv_OUTDIR, L".\\settings.ini");
		if (((CComboBox*)GetDlgItem(IDC_COMBO_PRESET_V))->GetCurSel() == 0) {
			WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2001", L"", L".\\settings.ini");
		}
	}
	else {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x2001", L"", L".\\settings.ini");
	}
	if (((CEdit*)GetDlgItem(IDC_EDIT_COMMAND_PRM_V))->GetWindowTextLength() != 0) {
		//replaceOtherStr(xv_PARAM, L"%OutString%", OUTDIR + OUTNAME + MULTI);
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x20FF", xv_PARAM, L".\\settings.ini");
	}
	else if (FINALPARAM_V != L"") {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x20FF", FINALPARAM_V, L".\\settings.ini");
	}
	else {
		WritePrivateProfileString(L"FFMPEG_VIDEO_SETTINGS", L"0x20FF", L"", L".\\settings.ini");
	}

	CDialogEx::OnOK();
}


void FFMPEGVIDEOSETTINGS::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


void FFMPEGVIDEOSETTINGS::OnDestroy()
{
	CDialogEx::OnDestroy();

	FreeLibrary(Core->hinst);
	DEL_CORE
	Core->FreeImageLibrary();
}