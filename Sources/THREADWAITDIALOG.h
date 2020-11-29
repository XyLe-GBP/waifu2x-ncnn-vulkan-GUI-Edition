#pragma once


// THREADWAITDIALOG ダイアログ

class THREADWAITDIALOG : public CDialogEx
{
	DECLARE_DYNAMIC(THREADWAITDIALOG)

public:
	THREADWAITDIALOG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~THREADWAITDIALOG();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREADWAITDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	int ThreadEndFlag;
};
