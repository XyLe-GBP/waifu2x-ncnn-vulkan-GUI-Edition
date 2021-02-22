#pragma once


// DLDIALOG ダイアログ

class DLDIALOG : public CDialogEx
{
	DECLARE_DYNAMIC(DLDIALOG)

public:
	DLDIALOG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DLDIALOG();
	static UINT MainThread(LPVOID pParam);
	void MainThread();
	void UpdateProgressText();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	NEW_CORE;
	NEW_COREUTIL;

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
	CProgressCtrl xv_Progress1;
	CStatic m_Static;
};
