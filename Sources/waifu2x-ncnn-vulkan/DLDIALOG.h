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
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
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
	UINT MAX_POS;
	UINT CUR_POS;
public:
	CProgressCtrl xv_Progress1;
	CStatic m_Static;
};
