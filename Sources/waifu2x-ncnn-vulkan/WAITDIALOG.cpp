// WAITDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "waifu2x-ncnn-vulkanDlg.h"
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
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// WAITDIALOG メッセージ ハンドラー

BOOL WAITDIALOG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UINT Lang;
	Lang = GetPrivateProfileInt(L"LANGUAGE", L"0x0000", INFINITE, L".\\settings.ini");
	if (Lang == 0) {
		Core->LoadJPNLangLibrary();
	}
	else if (Lang == 1) {
		Core->LoadENGLangLibrary();
	}
	else {
		Core->LoadJPNLangLibrary();
	}

	LoadString(Core->Lang_hinst, IDS_STATIC_WAITDLG, (LPTSTR)STATIC_WAITDLG, 256);
	GetDlgItem(IDC_STATIC_WAITTXT)->SetWindowText(STATIC_WAITDLG);

	return TRUE;
}

void WAITDIALOG::OnDestroy()
{
	CDialogEx::OnDestroy();

	FreeLibrary(Core->Lang_hinst);
	SAFE_DELETE(CORE_FUNC);
}