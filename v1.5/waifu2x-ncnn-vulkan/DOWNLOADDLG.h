#pragma once


// DOWNLOADDLG ダイアログ

class DOWNLOADDLG : public CDialogEx
{
	DECLARE_DYNAMIC(DOWNLOADDLG)

public:
	DOWNLOADDLG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DOWNLOADDLG();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWNLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
