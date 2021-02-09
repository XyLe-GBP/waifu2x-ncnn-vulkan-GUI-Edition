#pragma once


// VIDEORESIZE ダイアログ

class VIDEORESIZE : public CDialogEx
{
	DECLARE_DYNAMIC(VIDEORESIZE)

public:
	VIDEORESIZE(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~VIDEORESIZE();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIDEORESIZEDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	BOOL m_nRadio;
	CComboBox m_Resize;
	CString FINAL;
	CString SIZE;
};
