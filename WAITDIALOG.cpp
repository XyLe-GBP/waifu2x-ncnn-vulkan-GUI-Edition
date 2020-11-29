// WAITDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "WAITDIALOG.h"
#include "afxdialogex.h"


// WAITDIALOG ダイアログ

IMPLEMENT_DYNAMIC(WAITDIALOG, CDialogEx)

WAITDIALOG::WAITDIALOG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WAITDIALOG, pParent)
{

}

WAITDIALOG::~WAITDIALOG()
{
}

void WAITDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(WAITDIALOG, CDialogEx)
END_MESSAGE_MAP()


// WAITDIALOG メッセージ ハンドラー
