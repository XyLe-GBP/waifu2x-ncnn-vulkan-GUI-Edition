// THREADWAITDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "THREADWAITDIALOG.h"
#include "afxdialogex.h"
#include "waifu2x-ncnn-vulkanDlg.h"


// THREADWAITDIALOG ダイアログ

IMPLEMENT_DYNAMIC(THREADWAITDIALOG, CDialogEx)

THREADWAITDIALOG::THREADWAITDIALOG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_THREADWAITDIALOG, pParent)
{

	ThreadEndFlag = 0;
}

THREADWAITDIALOG::~THREADWAITDIALOG()
{
}

void THREADWAITDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(THREADWAITDIALOG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &THREADWAITDIALOG::OnBnClickedButton1)
END_MESSAGE_MAP()


// THREADWAITDIALOG メッセージ ハンドラー


void THREADWAITDIALOG::OnBnClickedButton1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int on_button;
	on_button = MessageBox(_T("処理が進行中です。処理を中止するとデータは失われます。\n続行しますか？"), _T("処理が進行中です"), MB_ICONWARNING | MB_YESNO);
	if (on_button == IDYES) {
		Cwaifu2xncnnvulkanDlg* MainWnd = new Cwaifu2xncnnvulkanDlg;
		ThreadEndFlag = TRUE;
	}
	else
	{
		return;
	}
}
