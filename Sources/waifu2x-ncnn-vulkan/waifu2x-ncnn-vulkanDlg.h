
// waifu2x-ncnn-vulkanDlg.h : ヘッダー ファイル
//

#pragma once
#include "../App.General.Lib/App.General.Lib.Main.h"
#include "Language.h"

// Cwaifu2xncnnvulkanDlg ダイアログ
class Cwaifu2xncnnvulkanDlg : public CDialogEx
{
// コンストラクション
	friend class THREADWAITDIALOG;
	friend class DLDIALOG;
	friend class DELETEDIALOG;
public:
	Cwaifu2xncnnvulkanDlg(CWnd* pParent = nullptr);	// 標準コンストラクター
	static UINT DLThread(LPVOID pParam);
	static UINT DLCountThread(LPVOID pParam);
	static UINT waifu2xThread(LPVOID pParam);
	static UINT waifu2xCountThread(LPVOID pParam);
	static UINT waifu2xCountDlgThread(LPVOID pParam);
	static UINT FFmpegThread(LPVOID pParam);
	static UINT DeleteThread(LPVOID pParam);
	static UINT DeleteMainThread(LPVOID pParam);
	void DLThread();
	void DLCountThread();
	void DeleteThread();
	void DeleteMainThread();
	void waifu2xThread();
	void waifu2xCountThread();
	void waifu2xCountDlgThread();
	void FFmpegThread();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAIFU2XNCNNVULKAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 実装
protected:
	HICON m_hIcon;
	NEW_CORE;
	NEW_MAINSTR;
	NEW_VERSIONSTR;
	NEW_COREUTIL;
	NEW_WAIFU2X_UTIL;

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
	static UINT DLCount;
	static UINT DLCurCount;
	static UINT DLThreadFlag;
	static UINT DLErrorFlag;
	static UINT FILECOUNT;
	static UINT UPSCALE_COUNT;
	static UINT DELETECURCOUNT;
	static UINT DELETEMAINCOUNT;
	static UINT DELETESUBCOUNT;
	static UINT ProgressThreadFlag;
	static UINT UpscaleExceptionFlag;
	static UINT Waifu2xThreadFlag;
	static UINT Waifu2xReUpscalingFlag;
	static UINT DeleteExceptionFlag;
	static UINT DeleteFileThreadFlag;
	static UINT SuspendFlag;
	CWinThread* pDLThread;
	CWinThread* pWaifu2xThread;
	CWinThread* pFFmpegThread;
	CWinThread* pWaifu2xCountThread;
	CWinThread* pWaifu2xCountDlgThread;
	CWinThread* pDLCountThread;
	CWinThread* pDeleteThread;
	CWinThread* pDeleteMainThread;
public: // user public

	UINT FFmpegThreadFlag;
	UINT ImageUpScaleFlag;
	UINT DeleteMainThreadFlag;

	CStatic xv_Static_ReadStatus;
	CStatic xv_Static_File;
	CStatic xv_Static_FilePath;

	CString waifu2x_param;
	CString ffmpeg_mainparam;
	CString ffmpeg_videoparam;
	CString ffmpeg_audioparam;
	CString UPSCALE_PATH;
	CString IMAGEPATH;
	CString IMAGEPATH_M;
	CString VIDEOPATH;
	CString MultipleOutPath;
	CString DELETEPATH;
	
	CEdit xv_CPUINFO;
	CEdit xv_GPUINFO;

	UINT SuspendCheck(LPCTSTR Path1, UINT Flag);
	UINT SuspendCheckMulti(LPCTSTR Path1, LPCTSTR Path2);
	UINT DeleteExceptionCheck();
	UINT UpscaleExceptionCheck();
	void LoadDlgStr();
	afx_msg LRESULT OnCompleteWaifu2xThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteFFmpegThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteDLThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteDLCountThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteWaifu2xCountThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteWaifu2xCountDlgThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteDeleteThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCompleteDeleteMainThread(WPARAM wParam, LPARAM lParam);
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
	afx_msg void OnLanguageJapanese();
	afx_msg void OnLanguageEnglish();
};