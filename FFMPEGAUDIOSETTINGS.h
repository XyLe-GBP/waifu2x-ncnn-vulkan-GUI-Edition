#pragma once
#include "../GUI.ResourcePack.Main/GUI.ResourcePack.Main.Init.h"

// FFMPEGAUDIOSETTINGS ダイアログ

class FFMPEGAUDIOSETTINGS : public CDialogEx
{
	DECLARE_DYNAMIC(FFMPEGAUDIOSETTINGS)

public:
	FFMPEGAUDIOSETTINGS(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~FFMPEGAUDIOSETTINGS();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FFMPEG_AUDIOSETTINGDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
protected:
	NEW_CORE
	CBitmap m_hImage;
public:
	CString FINALPARAM_A;
	afx_msg void OnCbnSelchangeComboPreset();
	afx_msg void OnCbnSelchangeComboAudioStream();
	afx_msg void OnBnClickedCheckAudioOnly();
	afx_msg void OnBnClickedCheckDStream();
	afx_msg void OnBnClickedCheckOverwrite();
	afx_msg void OnBnClickedCheckDn();
	afx_msg void OnBnClickedCheckHideBanner();
	afx_msg void OnBnClickedCheckChapter();
	afx_msg void OnBnClickedCheckMetadata();
	afx_msg void OnEnChangeEditOutputName();
	afx_msg void OnBnClickedButtonOutput();
	afx_msg void OnEnChangeEditOutput();
	afx_msg void OnEnChangeEditCommandParam();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
private:
	CComboBox m_PRESET_A;
	CComboBox m_STREAM_A;
	CString xv_OUTNAME_A;
	CString xv_OUTDIR_A;
	CString xv_PARAM_A;
	CString AUDIOSTREAM_A;
	CString AUDIOONLY_A;
	CString DISABLESTREAM_A;
	CString OVERWRITE_A;
	CString HIDEBANNER_A;
	CString MAP_CHAPTERS_A;
	CString MAP_METADATA_A;
	CString OUTDIR_A;
	CString OUTNAME_A;
	CString DISABLEDATA_A;
};
