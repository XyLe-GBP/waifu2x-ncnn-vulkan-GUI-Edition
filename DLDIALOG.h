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

	DECLARE_MESSAGE_MAP()
};
