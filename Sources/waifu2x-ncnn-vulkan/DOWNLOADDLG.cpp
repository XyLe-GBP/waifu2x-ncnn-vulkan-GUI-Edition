// DOWNLOADDLG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "DOWNLOADDLG.h"
#include "afxdialogex.h"


// DOWNLOADDLG ダイアログ

IMPLEMENT_DYNAMIC(DOWNLOADDLG, CDialogEx)

DOWNLOADDLG::DOWNLOADDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DOWNLOAD, pParent)
{

}

DOWNLOADDLG::~DOWNLOADDLG()
{
}

void DOWNLOADDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DOWNLOADDLG, CDialogEx)
END_MESSAGE_MAP()


// DOWNLOADDLG メッセージ ハンドラー
