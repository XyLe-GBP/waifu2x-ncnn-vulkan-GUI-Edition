#pragma once


// FILEDELETEDLG ダイアログ

class FILEDELETEDLG : public CDialogEx
{
	DECLARE_DYNAMIC(FILEDELETEDLG)

public:
	FILEDELETEDLG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~FILEDELETEDLG();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEDELETE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Static;
	CProgressCtrl xv_Progress;
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
};
