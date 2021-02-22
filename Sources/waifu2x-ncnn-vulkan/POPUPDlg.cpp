// POPUPDlg.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "waifu2x-ncnn-vulkanDlg.h"
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
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// POPUPDlg メッセージ ハンドラー

BOOL POPUPDlg::OnInitDialog()
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

	LoadString(Core->Lang_hinst, IDS_STATIC_POPUPDLG, (LPTSTR)STATIC_POPUPDLG, 256);
	GetDlgItem(IDC_STATIC_PPUP)->SetWindowText(STATIC_POPUPDLG);

	return TRUE;
}

void POPUPDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	FreeLibrary(Core->Lang_hinst);
	SAFE_DELETE(CORE_FUNC);
}