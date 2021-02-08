// CODECLISTDLG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "CODECLISTDLG.h"
#include "afxdialogex.h"
#include <sstream>


// CODECLISTDLG ダイアログ

IMPLEMENT_DYNAMIC(CODECLISTDLG, CDialogEx)

CODECLISTDLG::CODECLISTDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FFMPEG_CODECDIALOG, pParent)
{

}

CODECLISTDLG::~CODECLISTDLG()
{
}

void CODECLISTDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LIST, m_hEditList);
}


BEGIN_MESSAGE_MAP(CODECLISTDLG, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CODECLISTDLG::OnBnClickedOk)
END_MESSAGE_MAP()


// CODECLISTDLG メッセージ ハンドラー

BOOL CODECLISTDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	std::ifstream ifs(CURRENT_PATH + _T("\\Resources\\codecs.txt"));
	if (!ifs) {
		MessageBox(_T("読み込みエラー"), _T("エラー"), MB_ICONERROR | MB_OK);
	}
	std::stringstream sstream;
	sstream << ifs.rdbuf();
	std::string mainstr = sstream.str();

	CString CodecList;
	CodecList = mainstr.c_str();
	Utility->AfxReplaceStr(CodecList, _T("\n"), _T("\r\n"));
	m_hEditList.SetWindowText(CodecList);
	ifs.close();

	return TRUE;
}


void CODECLISTDLG::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}


void CODECLISTDLG::OnDestroy()
{
	CDialogEx::OnDestroy();

	DeleteFile(CURRENT_PATH + _T("\\Resources\\codecs.txt"));
	SAFE_DELETE(COREUTIL_FUNC);
	SAFE_DELETE(MAINSTR_FUNC);
}