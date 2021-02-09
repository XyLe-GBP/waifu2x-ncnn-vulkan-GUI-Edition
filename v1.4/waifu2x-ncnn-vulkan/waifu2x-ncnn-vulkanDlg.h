
// waifu2x-ncnn-vulkanDlg.h : ヘッダー ファイル
//

#pragma once
#include <memory>
#include "../GUI.ResourcePack.Main/GUI.ResourcePack.Main.Init.h"
#include "../GUI.ResourcePack.String/GUI.ResourcePack.String.Init.h"

// Cwaifu2xncnnvulkanDlg ダイアログ
class Cwaifu2xncnnvulkanDlg : public CDialogEx
{
// コンストラクション
public:
	Cwaifu2xncnnvulkanDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAIFU2XNCNNVULKAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;
	NEW_MAINSTR
	NEW_COREUTIL

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public: //system
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	void DoEvents();
	
private: // user private
	CWinThread* pThread;
	CWinThread* pThread2x;
	//void collationReserve();
	//void displayResultScreen();
public: // user public
	int ThreadFlag;
	
	CStatic xv_Static_ReadStatus;
	CStatic xv_Static_File;
	CStatic xv_Static_FilePath;

	CString waifu2x_param;
	CString ffmpeg_mainparam;
	CString ffmpeg_videoparam;
	CString ffmpeg_audioparam;
	CString IMAGEPATH;
	CString IMAGEPATH_M;
	CString VIDEOPATH;
	
	CEdit xv_CPUINFO;
	CEdit xv_GPUINFO;
	
	static UINT ThreadProc(LPVOID pParam);
	static UINT ThreadProc2x(LPVOID pParam);
	void ThreadProc();
	void ThreadProc2x();

	afx_msg LRESULT OnCompleteLoadXML(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteLoadXML2(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnReadImage();
	afx_msg void OnReadImageMulti();
	afx_msg void OnReadVideo();
	afx_msg void OnFileAlldelete();
	afx_msg void OnEndApp();
	afx_msg void OnSettings();
	afx_msg void OnFfmpegSettings();
	afx_msg void OnFfmpegAudiosettings();
	afx_msg void OnFfmpegVideosettings();
	afx_msg void OnAboutWaifu2x();
	afx_msg void OnVideoresize();
	afx_msg void OnVideoaudioexport();
	afx_msg void OnUpdatecheck();
};
