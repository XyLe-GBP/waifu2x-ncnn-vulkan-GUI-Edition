// DLDIALOG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "waifu2x-ncnn-vulkanDlg.h"
#include "DLDIALOG.h"
#include "afxdialogex.h"


// DLDIALOG ダイアログ

IMPLEMENT_DYNAMIC(DLDIALOG, CDialogEx)

DLDIALOG::DLDIALOG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLDIALOG, pParent)
{
	hDCBackBuffer = NULL;
	hDCStatic = NULL;
	m_Hbrush = NULL;
	MAX = _T("");
	POS = _T("");
	PROGRESSTEXT = _T("");
	MAX_POS = 0;
	CUR_POS = 0;
}

DLDIALOG::~DLDIALOG()
{
}

void DLDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, xv_Progress1);
	DDX_Control(pDX, IDC_STATIC1, m_Static);
}


BEGIN_MESSAGE_MAP(DLDIALOG, CDialogEx)
END_MESSAGE_MAP()


// DLDIALOG メッセージ ハンドラー

BOOL DLDIALOG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	xv_Progress1.SetRange32(0, Cwaifu2xncnnvulkanDlg::DLCurCount);
	MAX_POS = Cwaifu2xncnnvulkanDlg::DLCurCount;
	OutputDebugString(_T("SetRange.\n"));

	hDCBackBuffer = CreateCompatibleDC(NULL);
	hDCStatic = ::GetDC(this->m_Static);
	this->m_Static.GetClientRect(&rc);

	return TRUE;
}


LRESULT DLDIALOG::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。
	HDC hdc;
	PAINTSTRUCT ps;

	if (xv_Progress1.m_hWnd) {
		INT nLower, nUpper;
		UINT UnL, UnU;

		xv_Progress1.GetRange(nLower, nUpper);
		UnL = static_cast<UINT>(nLower);
		UnU = static_cast<UINT>(nUpper);
		if (UnU > Cwaifu2xncnnvulkanDlg::DLCount) {
			// プログレスバーの進行状況バーを更新
			xv_Progress1.SetPos(Cwaifu2xncnnvulkanDlg::DLCount);
			UpdateData(FALSE);
		}
		else if (UnU >= Cwaifu2xncnnvulkanDlg::DLCount) {
			// 上限を超えていればダイアログを閉じる
			OutputDebugString(_T("Close.\n"));
			Cwaifu2xncnnvulkanDlg::DLThreadFlag = TRUE;
			PostMessage(WM_CLOSE);
		}
		else if (CUR_POS == Cwaifu2xncnnvulkanDlg::DLCount) {
			Sleep(300);
			if (CUR_POS == Cwaifu2xncnnvulkanDlg::DLCount) {
				Sleep(300);
				if (CUR_POS == Cwaifu2xncnnvulkanDlg::DLCount) {
					Sleep(300);
					if (CUR_POS == Cwaifu2xncnnvulkanDlg::DLCount) {
						Sleep(300);
						if (CUR_POS == Cwaifu2xncnnvulkanDlg::DLCount) {
							OutputDebugString(_T("nCount Error.\n"));
							PostMessage(WM_CLOSE);
							Cwaifu2xncnnvulkanDlg::DLThreadFlag = TRUE;
						}
					}
				}
			}
		}
	}
	if (message == WM_PAINT) {
		CUR_POS = Cwaifu2xncnnvulkanDlg::DLCount;
		POS.Format(_T("Downloaded: %u Byte /"), CUR_POS);
		MAX.Format(_T(" %u Byte"), MAX_POS);
		PROGRESSTEXT = POS + MAX;
		::SetBkColor(hDCStatic, RGB(240, 240, 240));
		::SelectObject(hDCBackBuffer, hDCStatic);
		m_Hbrush = (HBRUSH)::SelectObject(hDCBackBuffer, CreateSolidBrush(RGB(240, 240, 240)));
		::PatBlt(hDCBackBuffer, 0, 0, rc.right, rc.bottom, PATCOPY);
		::SelectObject(hDCBackBuffer, m_Hbrush);
		::DrawText(hDCStatic, PROGRESSTEXT, -1, &rc, DT_RIGHT | DT_NOCLIP | DT_SINGLELINE);
		this->m_Static.InvalidateRect(rc, 0);
		hdc = ::BeginPaint(this->m_Static, &ps);
		::BitBlt(hdc, 0, 0, rc.right, rc.bottom, hDCBackBuffer, 0, 0, SRCCOPY);
		::EndPaint(this->m_Static, &ps);
		Sleep(255);
		this->m_Static.UpdateWindow();
		return 0;
	}
	if (message == WM_CLOSE) {
		if (hDCStatic) { ::ReleaseDC(this->m_Static, hDCStatic); hDCStatic = NULL; }
		if (hDCBackBuffer) { ::DeleteDC(hDCBackBuffer); hDCBackBuffer = NULL; }
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}