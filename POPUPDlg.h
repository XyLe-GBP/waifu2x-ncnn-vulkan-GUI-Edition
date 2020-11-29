#pragma once


// POPUPDlg ダイアログ

class POPUPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(POPUPDlg)

public:
	POPUPDlg(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~POPUPDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POPUP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
