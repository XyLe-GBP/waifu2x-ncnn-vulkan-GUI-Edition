#pragma once
#include "../App.General.Lib/App.General.Lib.Main.h"

// FFMPEGSETTINGSDIALOG ダイアログ

class FFMPEGSETTINGSDIALOG : public CDialogEx
{
	DECLARE_DYNAMIC(FFMPEGSETTINGSDIALOG)

public:
	FFMPEGSETTINGSDIALOG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~FFMPEGSETTINGSDIALOG();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FFMPEG_SETTINGDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
protected:
	NEW_CORE;
	NEW_MAINSTR;
	CBitmap m_hImage;
public:
	CString FPS;
	CString PRESET;
	CString VIDEOCODEC;
	CString AUDIOCODEC;
	CString AUDIOBITRATE;
	CString FINALPARAM;
	CString HIDEBANNER;
	CString STRCT;
	CString QP;
	CString OVERWRITE;
	CString CUSTOMPARAM;
	CString FPS2;
private:
	CComboBox m_PRESET;
	CComboBox m_VIDEOCODEC;
	CComboBox m_AUDIOCODEC;
	CString xv_FPS;
	CString xv_ExePath;
	CString xv_VIDEOCODEC;
	CString xv_AUDIOCODEC;
	CString xv_AUDIOBITRATE;
	CString xv_CUSTOMPARAM;
	CString xv_QPLEVEL;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnCbnSelchangeComboPreset();
	afx_msg void OnEnChangeEditAudioBitrate();
	afx_msg void OnBnClickedCheckAdvanced();
	afx_msg void OnBnClickedCheckHideBanner();
	afx_msg void OnBnClickedCheckQp();
	afx_msg void OnEnChangeEditCommandParam();
	afx_msg void OnEnChangeEditVideoCodec();
	afx_msg void OnEnChangeEditAudioCodec();
	afx_msg void OnEnChangeEditQplevel();
	afx_msg void OnBnClickedCheckAac();
	afx_msg void OnBnClickedCheckOverwrite();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedButtonCodec();
};
