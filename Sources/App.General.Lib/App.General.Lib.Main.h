#pragma once

#include "pch.h"
#include <io.h>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Tlhelp32.h>
#include "wininet.h" 
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib, "version.lib")
#include <mmsystem.h>

#include "App.General.Lib.Define.h"

/*#ifdef GUIRESOURCEPACKMAIN_EXPORTS
#define GUIRESOURCEPACKMAIN_API __declspec(dllexport)
#else
#define GUIRESOURCEPACKMAIN_API __declspec(dllimport)
#endif*/

using namespace std;

class libCore {
public:
	HINSTANCE hinst;
	void libCore::LoadImageLibrary();
	void libCore::FreeImageLibrary();
};


class libCoreUtility {
public:
	static bool libCoreUtility::checkExistenceOfFolder(const std::string folder_name);
	DWORD libCoreUtility::FFmpegServerCheck();
	bool libCoreUtility::HtmlGet2(TCHAR* url, DWORD* status_code, DWORD* sz, SYSTEMTIME* LocalTime);
	CString libCoreUtility::FFmpegVersionCheck();
	CString libCoreUtility::AppUpdateCheck();
	CString libCoreUtility::AppCurrentVersionCheck();
	bool libCoreUtility::DownloadFile(LPCTSTR pszURL, LPCTSTR pszLocalFile, DWORD dwBuffSize);
	BOOL libCoreUtility::ExtractZip(IShellDispatch* pShellDisp, TCHAR* ZipPath, TCHAR* OutPath);
	void libCoreUtility::FindDirectoryALL(wstring oFolderPath);
	DWORD libCoreUtility::FindDirectoryFileSize(wstring oFolderPath);
	UINT libCoreUtility::GetDirectoryFileCount(const std::basic_string<TCHAR>& src);
	UINT_PTR libCoreUtility::AfxGetFileSize(TCHAR* oFilePath);
	DWORD libCoreUtility::GetFileSizeAPI(const wchar_t* fp);
	DWORD libCoreUtility::GetFileSizeStat(const wchar_t* fp);
	BOOL libCoreUtility::DeleteDirectory(LPCTSTR lpPathName);
	CString libCoreUtility::AfxReplaceStr(CString& replacedStr, CString from, CString to);
	CStringA libCoreUtility::AfxReplaceStrA(CStringA& replacedStr, CStringA from, CStringA to);
	void libCoreUtility::TerminateExeName(const TCHAR* TrFileName);
};


class libMainString {
public:
	string libMainString::TWStringToString(const wstring& arg_wstr);
	wstring libMainString::StringToWString(const string& arg_str);
	CString libMainString::CURRENT_DIR();
	CString libMainString::RESOURCELIB_PATH();
	CString libMainString::DEFAULTTEXT();
	CString libMainString::READEDTEXT();
	CString libMainString::NULLTEXT();
};


class libVersionString {
public:
	CString libVersionString::APP_VERSION_TEXT();
	CString libVersionString::CMD_VERSION_TEXT();
	CString libVersionString::FF_VERSION_TEXT();
};


class libWaifu2x {
public:
	CFont* StFont1 = nullptr;
	CFont* fnt1 = new CFont;
	TCHAR* cpuinfo = (TCHAR*)malloc(sizeof(TCHAR) * 256);
	TCHAR* gpuinfo = (TCHAR*)malloc(sizeof(TCHAR) * 256);
	TCHAR* waifu2x_prm = (TCHAR*)malloc(sizeof(TCHAR) * 256);
	TCHAR* FFmpeg_prm = (TCHAR*)malloc(sizeof(TCHAR) * 256);
	TCHAR* FFmpeg_video_prm = (TCHAR*)malloc(sizeof(TCHAR) * 256);
	TCHAR* FFmpeg_audio_prm = (TCHAR*)malloc(sizeof(TCHAR) * 256);
};