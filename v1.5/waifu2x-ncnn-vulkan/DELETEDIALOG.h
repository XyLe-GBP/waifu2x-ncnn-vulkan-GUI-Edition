#pragma once


// DELETEDIALOG ダイアログ

class DELETEDIALOG : public CDialogEx
{
	DECLARE_DYNAMIC(DELETEDIALOG)

public:
	DELETEDIALOG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~DELETEDIALOG();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELETEDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
