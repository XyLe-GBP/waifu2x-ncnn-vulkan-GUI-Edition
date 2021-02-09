// DELETEDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "DELETEDIALOG.h"
#include "afxdialogex.h"


// DELETEDIALOG ダイアログ

IMPLEMENT_DYNAMIC(DELETEDIALOG, CDialogEx)

DELETEDIALOG::DELETEDIALOG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELETEDIALOG, pParent)
{

}

DELETEDIALOG::~DELETEDIALOG()
{
}

void DELETEDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DELETEDIALOG, CDialogEx)
END_MESSAGE_MAP()


// DELETEDIALOG メッセージ ハンドラー
