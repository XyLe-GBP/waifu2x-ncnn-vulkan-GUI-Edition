// FILEDELETEDLG.cpp : 実装ファイル
//

#include "pch.h"
#include "waifu2x-ncnn-vulkan.h"
#include "waifu2x-ncnn-vulkanDlg.h"
#include "FILEDELETEDLG.h"
#include "afxdialogex.h"


// FILEDELETEDLG ダイアログ

IMPLEMENT_DYNAMIC(FILEDELETEDLG, CDialogEx)

FILEDELETEDLG::FILEDELETEDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILEDELETE_DIALOG, pParent)
{
	CUR_POS = 0;
	MAX_POS = 0;
	POS = _T("");
	MAX = _T("");
	PROGRESSTEXT = _T("");
	hDCBackBuffer = NULL;
	hDCStatic = NULL;
	m_Hbrush = NULL;
}

FILEDELETEDLG::~FILEDELETEDLG()
{
}

void FILEDELETEDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_Static);
	DDX_Control(pDX, IDC_PROGRESS1, xv_Progress);
}


BEGIN_MESSAGE_MAP(FILEDELETEDLG, CDialogEx)
END_MESSAGE_MAP()


// FILEDELETEDLG メッセージ ハンドラー

BOOL FILEDELETEDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	xv_Progress.SetRange32(0, static_cast<UINT>(Cwaifu2xncnnvulkanDlg::FILECOUNT));
	MAX_POS = Cwaifu2xncnnvulkanDlg::FILECOUNT;
	OutputDebugString(_T("SetRange.\n"));

	hDCBackBuffer = CreateCompatibleDC(NULL);
	hDCStatic = ::GetDC(this->m_Static);
	this->m_Static.GetClientRect(&rc);

	return TRUE;
}


LRESULT FILEDELETEDLG::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	//RECT rec{};

	if (xv_Progress.m_hWnd) {
		INT nLower, nUpper;
		UINT UnL, UnU;

		xv_Progress.GetRange(nLower, nUpper);
		UnL = static_cast<UINT>(nLower);
		UnU = static_cast<UINT>(nUpper);
		if (UnU > Cwaifu2xncnnvulkanDlg::DELETECOUNT) {
			// プログレスバーの進行状況バーを更新
			xv_Progress.SetPos(Cwaifu2xncnnvulkanDlg::DELETECOUNT);
			UpdateData(FALSE);
		}
		else if (UnU >= Cwaifu2xncnnvulkanDlg::DELETECOUNT) {
			// 上限を超えていればダイアログを閉じる
			OutputDebugString(_T("Close.\n"));
			Cwaifu2xncnnvulkanDlg::ProgressThreadFlag = TRUE;
			PostMessage(WM_CLOSE);
		}
		else if (CUR_POS == Cwaifu2xncnnvulkanDlg::DELETECOUNT) {
			Sleep(300);
			if (CUR_POS == Cwaifu2xncnnvulkanDlg::DELETECOUNT) {
				Sleep(300);
				if (CUR_POS == Cwaifu2xncnnvulkanDlg::DELETECOUNT) {
					Sleep(300);
					if (CUR_POS == Cwaifu2xncnnvulkanDlg::DELETECOUNT) {
						Sleep(300);
						if (CUR_POS == Cwaifu2xncnnvulkanDlg::DELETECOUNT) {
							OutputDebugString(_T("nCount Error.\n"));
							PostMessage(WM_CLOSE);
						}
					}
				}
			}
		}
	}
	if (message == WM_PAINT) {
		CUR_POS = Cwaifu2xncnnvulkanDlg::DELETECOUNT;
		POS.Format(_T("Deleted file(s): %u file(s) /"), CUR_POS);
		MAX.Format(_T(" %u file(s)"), MAX_POS);
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