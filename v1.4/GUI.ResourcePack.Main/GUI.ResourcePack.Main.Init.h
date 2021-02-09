#pragma once

#include "pch.h"
#include <io.h>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <string>
#include "wininet.h" 
#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib, "version.lib")
#include <mmsystem.h>

#include "../GUI.ResourcePack.String/GUI.ResourcePack.String.Init.h"

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
	CString libCoreUtility::AppUpdateCheck();
	CString libCoreUtility::AppCurrentVersionCheck();
	bool libCoreUtility::DownloadFile(LPCTSTR pszURL, LPCTSTR pszLocalFile, DWORD dwBuffSize);
	BOOL libCoreUtility::ExtractZip(IShellDispatch* pShellDisp, TCHAR* ZipPath, TCHAR* OutPath);
	void libCoreUtility::FindDirectoryALL(wstring oFolderPath);
	DWORD libCoreUtility::FindDirectoryFileSize(wstring oFolderPath);
	UINT_PTR libCoreUtility::GetDirectoryFileCount(const std::basic_string<TCHAR>& src);
	UINT_PTR libCoreUtility::AfxGetFileSize(TCHAR* oFilePath);
	DWORD libCoreUtility::GetFileSizeAPI(const wchar_t* fp);
	DWORD libCoreUtility::GetFileSizeStat(const wchar_t* fp);
	BOOL libCoreUtility::DeleteDirectory(LPCTSTR lpPathName);
	CString libCoreUtility::AfxReplaceStr(CString& replacedStr, CString from, CString to);
	CStringA libCoreUtility::AfxReplaceStrA(CStringA& replacedStr, CStringA from, CStringA to);
};