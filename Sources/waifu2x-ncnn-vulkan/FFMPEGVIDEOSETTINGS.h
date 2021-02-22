#pragma once
#include "../App.General.Lib/App.General.Lib.Main.h"

// FFMPEGVIDEOSETTINGS ダイアログ

class FFMPEGVIDEOSETTINGS : public CDialogEx
{
	DECLARE_DYNAMIC(FFMPEGVIDEOSETTINGS)

public:
	FFMPEGVIDEOSETTINGS(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~FFMPEGVIDEOSETTINGS();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FFMPEG_VIDEOSETTINGDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
protected:
	NEW_CORE;
	CBitmap m_hImage;
public:
	afx_msg void OnCbnSelchangeComboPreset();
	afx_msg void OnBnClickedCheckOverwrite();
	afx_msg void OnBnClickedCheckMulti();
	afx_msg void OnEnChangeEditOutputName();
	afx_msg void OnBnClickedButtonOutput();
	afx_msg void OnEnChangeEditOutput();
	afx_msg void OnEnChangeEditCommandPrm();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCheckHideBanner();
	CString FINALPARAM_V;
private:
	CComboBox m_PRESET;
	CString xv_OUTNAME;
	CString xv_OUTDIR;
	CString xv_PARAM;
	CString OVERWRITE;
	CString MULTI;
	CString HIDEBANNER;
	CString OUTNAME;
	CString OUTDIR;
	void SetDlgLang();
};
