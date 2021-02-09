#include "pch.h"
#include "GUI.ResourcePack.Main.Init.h"

// Core

void libCore::LoadImageLibrary() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	NEW_MAINSTR
	libCore::hinst = LoadLibrary(RES_LIBRARY);
	if (libCore::hinst == NULL) {
		MessageBox(NULL, NULL_TEXT + RESOURCEDLL_TEXT + NOTFOUND_TEXT, _T("GUI.ResourcePack.Main.dll - システム エラー"), MB_ICONERROR | MB_OK);
		DEL_MAINSTR
		AfxGetMainWnd()->SendMessage(WM_CLOSE, 0, 0);
		AfxGetMainWnd()->DestroyWindow();
		PostQuitMessage(0);
	}
	else {
		DEL_MAINSTR
		ASSERT(libCore::hinst);
		static AFX_EXTENSION_MODULE resdll = { NULL, NULL };
		VERIFY(AfxInitExtensionModule(resdll, libCore::hinst));
		new CDynLinkLibrary(resdll);
	}
}

void libCore::FreeImageLibrary() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//GlobalUnlock(hinst);
	GlobalFree(libCore::hinst);
}

// Utility

bool libCoreUtility::checkExistenceOfFolder(const std::string folder_name) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (_mkdir(folder_name.c_str()) == 0) {
		return true;
	}
	else {
		return false;
	}
}


CString libCoreUtility::AppUpdateCheck() {
	HINTERNET    hNet;
	HINTERNET    hUrl;
	TCHAR m_cPath[MAX_PATH + 1];

	// 初期化
	hNet = ::InternetOpen(L"UpdateCheck...",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);
	if (hNet == NULL)
		return NULL;

	// バージョンチェック用のページ
	::_tcscpy_s(m_cPath, MAX_PATH, L"https://res.xyle-official.com/versions/waifu2x/VERSIONINFO");

	// URLオープン
	hUrl = ::InternetOpenUrl(hNet,
		m_cPath,
		NULL,
		0,
		INTERNET_FLAG_RELOAD,
		0);
	if (hUrl == NULL)
	{
		::InternetCloseHandle(hNet);
		return NULL;
	}

	// 読み込み
	TCHAR m_wBuf[256];
	DWORD dwReadSize;
	::InternetReadFile(hUrl, m_wBuf, 256, &dwReadSize);
	m_wBuf[dwReadSize] = '\0';

	// クローズ
	::InternetCloseHandle(hUrl);
	::InternetCloseHandle(hNet);

	// 変換
	char m_cBuf[256];

	const int cchWideByte = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)m_wBuf, -1, NULL, NULL);
	LPWSTR lpw = new WCHAR[cchWideByte];
	::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)m_wBuf, -1, lpw, cchWideByte);

	const int cchMultiByte = ::WideCharToMultiByte(CP_ACP, 0, lpw, -1, NULL, 0, NULL, NULL);
	LPSTR lpa = new CHAR[cchMultiByte];

	const int nMultiCount = ::WideCharToMultiByte(CP_ACP, 0, lpw, -1, lpa, cchMultiByte, NULL, NULL);

	::strcpy_s(m_cBuf, 256, lpa);

	delete[] lpa;
	delete[] lpw;

	// 解析
	char cVersion[256] = {0};
	char* cTmp = NULL, * cNext = NULL;
	cTmp = ::strtok_s(m_cBuf, ":", &cNext);
	if (::strcmp(cTmp, "version") == 0)
	{
		cTmp = ::strtok_s(NULL, "\n", &cNext);
		::strcpy_s(cVersion, 256, cTmp);
	}
	CString FINAL;
	FINAL = cVersion;

	return FINAL;
}


CString libCoreUtility::AppCurrentVersionCheck() {
	DWORD dwLen;
	DWORD dwHandle;
	TCHAR cBuf[256] = {NULL};
	const TCHAR* const AppName = L"waifu2x-ncnn-vulkan.exe";

	dwLen = ::GetFileVersionInfoSize(AppName, &dwHandle);
	if (dwLen > 0)
	{
		char* lpData = new char[dwLen + 1];
		::GetFileVersionInfo(AppName, NULL, dwLen, lpData);

		// 情報を取得する
		void* pvVersion;
		UINT VersionLen;
		::VerQueryValue(lpData, L"\\StringFileInfo\\041104b0\\ProductVersion", &pvVersion, &VersionLen);

		::_stprintf_s(cBuf, 256, _T("%s"), static_cast<char*>(pvVersion));

		delete[] lpData;
	}
	return cBuf;
}


bool libCoreUtility::DownloadFile(LPCTSTR pszURL, LPCTSTR pszLocalFile, DWORD dwBuffSize) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TCHAR		pszHeader[] = _T("Accept: */*\r\n\r\n");
	BOOL		ret;
	DWORD		dwReadSize;
	DWORD		dwWrittenSize;
	BYTE* pcbBuff;
	HINTERNET	hInternet;
	HINTERNET	hConnect;
	HANDLE		hFile;

	hInternet = ::InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return	false;

	hConnect = ::InternetOpenUrl(hInternet, pszURL, pszHeader, -1, INTERNET_FLAG_DONT_CACHE, 0);
	pcbBuff = new BYTE[dwBuffSize];
	if (hConnect == NULL || pcbBuff == NULL)
	{
		if (hConnect)
			::InternetCloseHandle(hConnect);
		::InternetCloseHandle(hInternet);
		if (pcbBuff)
			delete[]	pcbBuff;
		return	false;
	}

	//保存先のファイルを作る。ファイルがあった場合は失敗
	hFile = ::CreateFile(pszLocalFile, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	ret = (hFile == INVALID_HANDLE_VALUE) ? FALSE : TRUE;

	while (ret)
	{
		::Sleep(0);
		ret = ::InternetReadFile(hConnect, pcbBuff, dwBuffSize, &dwReadSize);
		if (ret == FALSE || dwReadSize == 0)
			break;

		ret = ::WriteFile(hFile, pcbBuff, dwReadSize, &dwWrittenSize, NULL);
	}

	if (hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);

		//失敗時は作成したファイルを削除
		if (ret == FALSE)
			::DeleteFile(pszLocalFile);
	}

	::InternetCloseHandle(hConnect);
	::InternetCloseHandle(hInternet);

	delete[]	pcbBuff;

	return	ret ? true : false;
}

BOOL libCoreUtility::ExtractZip(IShellDispatch * pShellDisp, TCHAR * ZipPath, TCHAR * OutPath) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HRESULT hr;
	VARIANT vDtcDir;
	Folder* pOutDtc;
	// 展開先Folderオブジェクトを作成
	VariantInit(&vDtcDir);
	vDtcDir.vt = VT_BSTR;
	vDtcDir.bstrVal = SysAllocString(OutPath);
	hr = pShellDisp->NameSpace(vDtcDir, &pOutDtc);
	VariantClear(&vDtcDir);
	if (hr != S_OK) {
		MessageBox(NULL, TEXT("展開先フォルダーが見つかりませんでした。"), NULL, MB_ICONWARNING);
		return FALSE;
	}
	// ZIPファイルのFolderオブジェクトを作成
	VARIANT varZip;
	Folder* pZipFile;
	VariantInit(&varZip);
	varZip.vt = VT_BSTR;
	varZip.bstrVal = SysAllocString(ZipPath);
	hr = pShellDisp->NameSpace(varZip, &pZipFile);
	VariantClear(&varZip);
	if (hr != S_OK) {
		pOutDtc->Release();
		MessageBox(NULL, TEXT("ZIPファイルが見つかりませんでした。"), NULL, MB_ICONWARNING);
		return FALSE;
	}
	// ZIPファイルの中身を取得
	FolderItems* pZipItems;
	hr = pZipFile->Items(&pZipItems);
	if (hr != S_OK) {
		pZipFile->Release();
		pOutDtc->Release();
		return FALSE;
	}
	VARIANT vDisp, vOpt;
	VariantInit(&vDisp);
	vDisp.vt = VT_DISPATCH;
	vDisp.pdispVal = pZipItems;
	VariantInit(&vOpt);
	vOpt.vt = VT_I4;
	vOpt.lVal = 0; //FOF_SILENTを指定すると処理中の経過が表示されなくなります
	// ZIPファイルの中身を展開先フォルダーにコピー
	hr = pOutDtc->CopyHere(vDisp, vOpt);
	if (hr != S_OK) {
		pZipItems->Release();
		pZipFile->Release();
		pOutDtc->Release();
		MessageBox(NULL, TEXT("展開に失敗しました。"), NULL, MB_ICONWARNING);
		return FALSE;
	}

	pZipItems->Release();
	pZipFile->Release();
	pOutDtc->Release();

	return TRUE;
}

void libCoreUtility::FindDirectoryALL(wstring oFolderPath) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	WIN32_FIND_DATA tFindFileData;
	wofstream ofs(L".\\list.index", ios::trunc);

	// 全てのファイル
	oFolderPath += L"\\*.*";

	// 最初に一致するファイルを取得
	HANDLE hFile = ::FindFirstFile(oFolderPath.c_str(), &tFindFileData);
	if (INVALID_HANDLE_VALUE == hFile) {
		return;
	}

	// L"¥¥*.*"を削除
	oFolderPath = oFolderPath.substr(0, oFolderPath.size() - 4);

	do {

		TCHAR* wpFileName = tFindFileData.cFileName;

		// フォルダかどうかを判定
		if (tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

			/*
				L"."とL".."はスキップ
			*/
			if (L'.' == wpFileName[0]) {
				if ((L'\0' == wpFileName[1])
					|| (L'.' == wpFileName[1] && L'\0' == wpFileName[2])
					) {
					continue;
				}
			}

			// フルパスの生成
			std::wstring oFullPath = oFolderPath + L"\\" + wpFileName;
			//std::wcout << L"(dir )" << oFullPath << std::endl;

			// 再起してサブフォルダを巡回する
			FindDirectoryALL(oFullPath);
		}
		else {

			// フルパスの生成
			std::wstring oFullPath = oFolderPath + L"\\" + wpFileName;
			ofs << oFullPath;
			ofs << "\n";
		}

		// 次に一致するファイルの検索
	} while (::FindNextFile(hFile, &tFindFileData));

	// 検索ハンドルを閉じる
	::FindClose(hFile);
}


DWORD libCoreUtility::FindDirectoryFileSize(wstring oFolderPath) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	WIN32_FIND_DATA tFindFileData;
	DWORD sizef = 0;

	// 全てのファイル
	oFolderPath += L"\\*.*";

	// 最初に一致するファイルを取得
	HANDLE hFile = ::FindFirstFile(oFolderPath.c_str(), &tFindFileData);
	if (INVALID_HANDLE_VALUE == hFile) {
		return -1L;
	}

	// L"¥¥*.*"を削除
	oFolderPath = oFolderPath.substr(0, oFolderPath.size() - 4);

	do {

		TCHAR* wpFileName = tFindFileData.cFileName;

		// フォルダかどうかを判定
		if (tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

			/*
				L"."とL".."はスキップ
			*/
			if (L'.' == wpFileName[0]) {
				if ((L'\0' == wpFileName[1])
					|| (L'.' == wpFileName[1] && L'\0' == wpFileName[2])
					) {
					continue;
				}
			}

			// フルパスの生成
			std::wstring oFullPath = oFolderPath + L"\\" + wpFileName;
			//std::wcout << L"(dir )" << oFullPath << std::endl;

			// 再起してサブフォルダを巡回する
			FindDirectoryFileSize(oFullPath);
		}
		else {

			// フルパスの生成
			std::wstring oFullPath = oFolderPath + L"\\" + wpFileName;
			DWORD size = GetFileSizeAPI(oFullPath.c_str());
			sizef += static_cast<UINT>(size);
			/*CStdioFile fin(oFullPath.c_str(), CFile::modeRead | CFile::typeBinary);
			CFileStatus status;
			if (fin.GetStatus(status)) {
				UINT_PTR size;
				size = status.m_size;
				sizef += size;
			}
			else {
				return -1L;
			}*/
		}

		// 次に一致するファイルの検索
	} while (::FindNextFile(hFile, &tFindFileData));

	// 検索ハンドルを閉じる
	::FindClose(hFile);
	return sizef;
}


UINT_PTR libCoreUtility::GetDirectoryFileCount(const std::basic_string<TCHAR> & src) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	std::basic_string<TCHAR> root = src + _T("\\*.*");
	WIN32_FIND_DATA wfd = {};
	HANDLE handle = FindFirstFile(root.c_str(), &wfd);
	if (handle == INVALID_HANDLE_VALUE) return 0;

	UINT_PTR result = 0;
	do {
		std::basic_string<TCHAR> path(wfd.cFileName);
		if (path != _T(".") && path != _T("..")) {
			++result;
			if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) result += GetDirectoryFileCount(src + _T('\\') + path);
		}
	} while (FindNextFile(handle, &wfd));
	FindClose(handle);

	return result;
}


UINT_PTR libCoreUtility::AfxGetFileSize(TCHAR * oFilePath) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStdioFile fin(oFilePath, CFile::modeRead | CFile::typeBinary);
	CFileStatus status;
	if (fin.GetStatus(status)) {
		UINT_PTR size = status.m_size;
		fin.Close();
		return size;
	}
	else {
		fin.Close();
		return -1L;
	}
}


DWORD libCoreUtility::GetFileSizeAPI(const wchar_t* fp) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	FILE* FP;
	_wfopen_s(&FP, fp, L"rb");
	if (FP == NULL) {
		return NULL;
	}

	DWORD size;
	INT_PTR size2 = _filelengthi64(_fileno(FP));
	size = static_cast<DWORD>(size2);

	fclose(FP);
	return size;
}


DWORD libCoreUtility::GetFileSizeStat(const wchar_t* fp) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _stat st;
	if (_wstat(fp, &st) != 0) {
		return NULL;
	}
	if ((st.st_mode & S_IFMT) != S_IFREG) {
		return NULL;
	}

	return st.st_size;
}


BOOL libCoreUtility::DeleteDirectory(LPCTSTR lpPathName) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CFileFind fnd;
	CString strPathName = lpPathName;
	strPathName.TrimRight('\\');
	strPathName += _T("\\*.*");
	if (fnd.FindFile(strPathName, 0))
	{
		int i = 1;
		while (i)
		{
			i = fnd.FindNextFile();

			// ファイル名が"."か".."の場合は次を検索
			if (fnd.IsDots())
				continue;

			// 削除するファイル名取得
			// GetFilePath()にはバグがあり正確に取得できない場合があるので使わない
			CString strDeleteFile = lpPathName;
			strDeleteFile.TrimRight('\\');
			strDeleteFile += _T("\\") + fnd.GetFileName();

			// フォルダだった場合、再帰呼び出しでそのフォルダを削除
			if (fnd.IsDirectory())
				DeleteDirectory(strDeleteFile);

			// ファイルの削除
			else
				::DeleteFile(strDeleteFile);
		}
		fnd.Close();

		// フォルダの削除
		return ::RemoveDirectory(lpPathName);
	}
	return FALSE;
}


CString libCoreUtility::AfxReplaceStr(CString & replacedStr, CString from, CString to) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	const unsigned int pos = replacedStr.Find(from);
	const int len = from.GetLength();

	if (pos == std::string::npos || from.IsEmpty()) {
		return replacedStr;
	}

	replacedStr.Replace(from, to);
	return replacedStr;
}


CStringA libCoreUtility::AfxReplaceStrA(CStringA & replacedStr, CStringA from, CStringA to) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	const unsigned int pos = replacedStr.Find(from);
	const int len = from.GetLength();

	if (pos == std::string::npos || from.IsEmpty()) {
		return replacedStr;
	}

	replacedStr.Replace(from, to);
	return replacedStr;
}
