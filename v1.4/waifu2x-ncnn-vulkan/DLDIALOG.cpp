// DLDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "DLDIALOG.h"
#include "afxdialogex.h"


// DLDIALOG ダイアログ

IMPLEMENT_DYNAMIC(DLDIALOG, CDialogEx)

DLDIALOG::DLDIALOG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLDIALOG, pParent)
{

}

DLDIALOG::~DLDIALOG()
{
}

void DLDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DLDIALOG, CDialogEx)
END_MESSAGE_MAP()


// DLDIALOG メッセージ ハンドラー
