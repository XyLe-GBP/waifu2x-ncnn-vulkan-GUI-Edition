#pragma once
#include "../App.General.Lib/App.General.Lib.Main.h"

// DELETEDIALOG ダイアログ

class DELETEDIALOG : public CDialogEx
{
	DECLARE_DYNAMIC(DELETEDIALOG)

public:
	DELETEDIALOG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DELETEDIALOG();
	static UINT MainThread(LPVOID pParam);
	void MainThread();
	void UpdateProgressText();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELETEDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	NEW_CORE;

	DECLARE_MESSAGE_MAP()
private:
	HDC hDCBackBuffer;
	HDC hDCStatic;
	HBRUSH m_Hbrush;
	CRect rc;
	CString MAX;
	CString POS;
	CString PROGRESSTEXT;
	UINT_PTR m_TimerID;
	UINT_PTR m_hTimerID;
	UINT MAX_POS;
	UINT CUR_POS;
	UINT ExceptionCounter;
	UINT ValueFlag;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnCompleteMainThread(WPARAM wParam, LPARAM lParam);
	CProgressCtrl xv_Progress;
	CStatic m_Static;
};
