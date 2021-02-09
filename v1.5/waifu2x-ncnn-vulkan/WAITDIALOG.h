#pragma once


// WAITDIALOG ダイアログ

class WAITDIALOG : public CDialogEx
{
	DECLARE_DYNAMIC(WAITDIALOG)

public:
	WAITDIALOG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~WAITDIALOG();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAITDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
