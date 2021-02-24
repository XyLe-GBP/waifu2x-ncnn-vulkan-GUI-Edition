#pragma once


// DLDIALOG ダイアログ

class DLDIALOG : public CDialogEx
{
	DECLARE_DYNAMIC(DLDIALOG)

public:
	DLDIALOG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DLDIALOG();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	NEW_CORE;
	NEW_COREUTIL;

	DECLARE_MESSAGE_MAP()
private:
	HDC hDCBackBuffer;
	HDC hDCStatic;
	HBRUSH m_Hbrush;
	CProgressCtrl xv_Progress1;
	CRect rc;
	CStatic m_Static;
	CString MAX;
	CString POS;
	CString PROGRESSTEXT;
	UINT_PTR m_TimerID;
	UINT_PTR m_hTimerID;
	UINT MAX_POS;
	UINT CUR_POS;
	UINT ExceptionCounter;
	UINT ValueFlag;
	void UpdateProgressText();
};
