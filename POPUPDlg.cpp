// POPUPDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "POPUPDlg.h"
#include "afxdialogex.h"


// POPUPDlg ダイアログ

IMPLEMENT_DYNAMIC(POPUPDlg, CDialogEx)

POPUPDlg::POPUPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POPUP, pParent)
{

}

POPUPDlg::~POPUPDlg()
{
}

void POPUPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(POPUPDlg, CDialogEx)
END_MESSAGE_MAP()


// POPUPDlg メッセージ ハンドラー

BOOL POPUPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}
