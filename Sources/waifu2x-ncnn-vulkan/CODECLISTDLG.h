#pragma once
#include "../App.General.Lib/App.General.Lib.Main.h"

// CODECLISTDLG ダイアログ

class CODECLISTDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CODECLISTDLG)

public:
	CODECLISTDLG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CODECLISTDLG();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FFMPEG_CODECDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();
	virtual void OnDestroy();

	NEW_CORE;
	NEW_COREUTIL;
	NEW_MAINSTR;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
private:
	CEdit m_hEditList;
	void SetDlgLang();
};
