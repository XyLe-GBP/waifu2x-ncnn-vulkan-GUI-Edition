#pragma once
#include "../GUI.ResourcePack.Main/GUI.ResourcePack.Main.Init.h"

// SETTINGSDIALOG ダイアログ

class SETTINGSDIALOG : public CDialogEx
{
	DECLARE_DYNAMIC(SETTINGSDIALOG)

public:
	SETTINGSDIALOG(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~SETTINGSDIALOG();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
protected:
	NEW_CORE
	CBitmap m_hImage;

public:
	CComboBox m_NoiseLevel;
	CComboBox m_UpscaleLevel;
	CComboBox m_SelectGPU;
	CString xv_BlockSize;
	CButton m_AdvancedEnable;
	CString xv_CommandLineParam;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	CString FinalSettings;
	CString NoiseLevel;
	CString UpscaleLevel;
	CString SelectGPU;
	CString BlockSize;
	CString CommandLineParam;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnEnChangeEdit3();
	CString BlockSize_suffix;
	CComboBox m_threads;
	CComboBox m_model;
	CComboBox m_format;
	CButton m_verbose;
	CButton m_tta;
	CString useThread;
	CString waifu2xModel;
	CString OutFormat;
	CString verbose_out;
	CString tta_mode;
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo5();
	afx_msg void OnCbnSelchangeCombo6();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
};
