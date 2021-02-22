#include "pch.h"
#include "App.General.Lib.Main.h"

// Core

void libCore::LoadImageLibrary() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	NEW_MAINSTR;
	libCore::hinst = LoadLibrary(RES_LIBRARY);
	SAFE_DELETE(MAINSTR_FUNC);
	ASSERT(libCore::hinst);
	static AFX_EXTENSION_MODULE resdll = { NULL, NULL };
	VERIFY(AfxInitExtensionModule(resdll, libCore::hinst));
	new CDynLinkLibrary(resdll);
}

void libCore::LoadJPNLangLibrary()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	NEW_MAINSTR;
	libCore::Lang_hinst = LoadLibrary(JPN_RES_LIBRARY);
	SAFE_DELETE(MAINSTR_FUNC);
	ASSERT(libCore::Lang_hinst);
	static AFX_EXTENSION_MODULE resdll = { NULL, NULL };
	VERIFY(AfxInitExtensionModule(resdll, libCore::Lang_hinst));
	new CDynLinkLibrary(resdll);
}

void libCore::LoadENGLangLibrary()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	NEW_MAINSTR;
	libCore::Lang_hinst = LoadLibrary(ENG_RES_LIBRARY);
	SAFE_DELETE(MAINSTR_FUNC);
	ASSERT(libCore::Lang_hinst);
	static AFX_EXTENSION_MODULE resdll = { NULL, NULL };
	VERIFY(AfxInitExtensionModule(resdll, libCore::Lang_hinst));
	new CDynLinkLibrary(resdll);
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


DWORD libCoreUtility::FFmpegServerCheck() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HINTERNET    hNet;
	HINTERNET    hUrl = nullptr;
	HINTERNET hHttpSession;
	HINTERNET hHttpRequest;
	DWORD BufSizeTextSize = 1000;
	DWORD StatusCode = sizeof(DWORD);
	DWORD StatusCode_Size = sizeof(DWORD);
	wchar_t Path[MAX_PATH + 1], Drive[10], Dir[MAX_PATH], Name[MAX_PATH], Ext[MAX_PATH];
	wchar_t ptr[MAX_PATH] = { 0 };
	TCHAR m_cPath[MAX_PATH + 1]{};

	CString URL;

	if (0 != GetModuleFileName(NULL, Path, MAX_PATH)) {// 実行ファイルの完全パスを取得
		_tsplitpath_s(Path, Drive, Dir, Name, Ext);
		CString NAME = Name;
		URL = _T("https://www.gyan.dev/ffmpeg/builds/release-version");
	}

	// 初期化
	hNet = ::InternetOpen(L"FFmpegVersionCheck...",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);
	if (hNet == NULL)
		return NULL;

	/* サーバへ接続 */
	hHttpSession = InternetConnect(
		hNet,
		_T("www.gyan.dev"),
		INTERNET_DEFAULT_HTTP_PORT,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		0);

	/* HTTP要求の作成 */
	hHttpRequest = HttpOpenRequest(
		hHttpSession,
		_T("GET"),
		_T("/"),
		NULL,
		URL,
		NULL,
		0,
		0);

	/* 作成したHTTP要求の発行 */
	BOOL bHttpSendRequest = HttpSendRequest(
		hHttpRequest,
		NULL,
		0,
		NULL,
		0);

	if (bHttpSendRequest == 0) {//bHttpSendRequestが0の場合はインターネットに接続されていないので、エラーを返す。
		::InternetCloseHandle(hNet);
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
		return NULL;//106:ERR_INTERNET_DISCONNECTED
	}

	/* 返されたコンテンツの長さを取得 */
	HttpQueryInfo(
		hHttpRequest,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
		&StatusCode,
		&StatusCode_Size,
		NULL);

	if (StatusCode != HTTP_STATUS_OK) {
		OutputDebugString(_T("Server check completed.\n"));
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
		::InternetCloseHandle(hNet);
		return StatusCode;
	}
	else {
		OutputDebugString(_T("Server check completed.\n"));
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
		::InternetCloseHandle(hNet);
		return StatusCode;
	}
}


bool libCoreUtility::HtmlGet2(TCHAR* url, DWORD* status_code, DWORD* sz, SYSTEMTIME* LocalTime) {
	URL_COMPONENTS uc;
	HINTERNET hiopen;
	HINTERNET hiconnect;
	HINTERNET hirequest;

	TCHAR host_name[64]{};
	TCHAR path[64]{};
	TCHAR buf[64];

	ZeroMemory(&uc, sizeof(URL_COMPONENTS));
	uc.dwStructSize = sizeof(URL_COMPONENTS);
	uc.lpszHostName = host_name;
	uc.lpszUrlPath = path;
	uc.dwHostNameLength = sizeof(host_name) / sizeof(TCHAR);
	uc.dwUrlPathLength = sizeof(path) / sizeof(TCHAR);

	//      URLの解析

	unsigned url_len = (unsigned)_tcslen(url);
	if (InternetCrackUrl(url, url_len, ICU_ESCAPE, &uc) == FALSE) {
		MessageBox(0, _TEXT("InternetCrackUrl:URLの解析に失敗しました。"), _TEXT("エラー"), MB_OK);
		return false;
	}

	DWORD flags = 0;

	if (uc.nScheme == INTERNET_SCHEME_HTTP) {
		flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_NO_AUTO_REDIRECT;
	}
	else if (uc.nScheme == INTERNET_SCHEME_HTTPS) {
		flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_NO_AUTO_REDIRECT
			| INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID
			| INTERNET_FLAG_IGNORE_CERT_CN_INVALID;
	}
	else {
		MessageBox(0, _TEXT("HTTPでもHTTPSでもありません"), _TEXT("エラー"), MB_OK);
		return false;
	}
	_stprintf_s(buf, sizeof(buf) / sizeof(TCHAR), _TEXT("%s"), uc.lpszUrlPath);

	//      インターネットのサービスハンドルの取得(レジストリ内のIEの設定を使用)
	hiopen = InternetOpen(_TEXT("inetdemo"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hiopen == NULL) {
		MessageBox(0, _TEXT("InternetOpenで初期化に失敗しました"), _TEXT("エラー"), MB_OK);

		return false;
	}
	//      HTTPサーバーへの接続
	hiconnect = InternetConnect(hiopen, uc.lpszHostName, uc.nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	if (hiconnect == NULL) {
		InternetCloseHandle(hiopen);
		MessageBox(0, _TEXT("InternetConnectでHTTP接続に失敗しました"), _TEXT("エラー"), MB_OK);
		return false;
	}

	//      HTTPサーバーへのリクエストの初期化

	hirequest = HttpOpenRequest(hiconnect, _TEXT("GET"), buf, NULL, NULL, NULL, flags, NULL);
	if (hirequest == NULL) {
		InternetCloseHandle(hiconnect);
		InternetCloseHandle(hiopen);
		MessageBox(0, _TEXT("HttpOpenRequestでHTTP接続に失敗しました"), _TEXT("エラー"), MB_OK);
		return false;
	}
	if (HttpSendRequest(hirequest, NULL, 0, NULL, 0) == FALSE) {
		InternetCloseHandle(hirequest);
		InternetCloseHandle(hiconnect);
		InternetCloseHandle(hiopen);
		MessageBox(0, _TEXT("HtpSendRequestでHTTP要求送信に失敗しました"), _TEXT("エラー"), MB_OK);
		return false;
	}

	//      HTTPリクエストに関連する情報を取得する(ステータスコードを32ビットで返す)

	*status_code = 0;
	DWORD len = sizeof(DWORD);
	if (HttpQueryInfo(hirequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, status_code, &len, 0) == FALSE) {
		InternetCloseHandle(hirequest);
		InternetCloseHandle(hiconnect);
		InternetCloseHandle(hiopen);
		*status_code = 0xffffffff;
		return false;
	}
	if (*status_code != HTTP_STATUS_OK) {    //      エラーがー発生した
		InternetCloseHandle(hirequest);
		InternetCloseHandle(hiconnect);
		InternetCloseHandle(hiopen);
		return false;
	}
	//      ファイルサイズの取得
	*sz = 0;
	len = sizeof(*sz);
	if (HttpQueryInfo(hirequest, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, sz, &len, 0) == TRUE) {

	}
	SYSTEMTIME UTCTime{};
	len = sizeof(UTCTime);
	if (HttpQueryInfo(hirequest, HTTP_QUERY_LAST_MODIFIED | HTTP_QUERY_FLAG_SYSTEMTIME, &UTCTime, &len, 0) == TRUE) {
		FILETIME FileTime, LocalFileTime;

		SystemTimeToFileTime(&UTCTime, &FileTime);
		FileTimeToLocalFileTime(&FileTime, &LocalFileTime);
		FileTimeToSystemTime(&LocalFileTime, LocalTime);
	}

	InternetCloseHandle(hirequest);
	InternetCloseHandle(hiconnect);
	InternetCloseHandle(hiopen);
	return true;
}


CString libCoreUtility::FFmpegVersionCheck() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HINTERNET    hNet;
	HINTERNET    hUrl;
	HINTERNET hHttpSession;
	HINTERNET hHttpRequest;
	DWORD BufSizeTextSize = 1000;
	DWORD StatusCode = sizeof(DWORD);
	DWORD StatusCode_Size = sizeof(DWORD);
	wchar_t Path[MAX_PATH + 1], Drive[10], Dir[MAX_PATH], Name[MAX_PATH], Ext[MAX_PATH];
	wchar_t ptr[MAX_PATH] = { 0 };
	TCHAR m_cPath[MAX_PATH + 1];

	CString URL;

	if (0 != GetModuleFileName(NULL, Path, MAX_PATH)) {// 実行ファイルの完全パスを取得
		_tsplitpath_s(Path, Drive, Dir, Name, Ext);
		CString NAME = Name;
		URL = _T("https://www.gyan.dev/ffmpeg/builds/release-version");
	}

	// 初期化
	hNet = ::InternetOpen(L"FFmpegVersionCheck...",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);
	if (hNet == NULL)
		return NULL;

	/* サーバへ接続 */
	hHttpSession = InternetConnect(
		hNet,
		_T("www.gyan.dev"),
		INTERNET_DEFAULT_HTTP_PORT,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		0);

	/* HTTP要求の作成 */
	hHttpRequest = HttpOpenRequest(
		hHttpSession,
		_T("GET"),
		_T("/"),
		NULL,
		URL,
		NULL,
		0,
		0);

	/* 作成したHTTP要求の発行 */
	BOOL bHttpSendRequest = HttpSendRequest(
		hHttpRequest,
		NULL,
		0,
		NULL,
		0);

	if (bHttpSendRequest == 0) {//bHttpSendRequestが0の場合はインターネットに接続されていないので、エラーを返す。
		::InternetCloseHandle(hNet);
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
		return NULL;//106:ERR_INTERNET_DISCONNECTED
	}

	/* 返されたコンテンツの長さを取得 */
	HttpQueryInfo(
		hHttpRequest,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
		&StatusCode,
		&StatusCode_Size,
		NULL);

	if (StatusCode != HTTP_STATUS_OK) {
		OutputDebugString(_T("HTTP Request failed.\n"));
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
		::InternetCloseHandle(hNet);
		return NULL;
	}
	else {
		OutputDebugString(_T("HTTP Request completed.\n"));

		// バージョンチェック用のページ
		::_tcscpy_s(m_cPath, MAX_PATH, URL);

		// URLオープン
		hUrl = ::InternetOpenUrl(hNet,
			m_cPath,
			NULL,
			0,
			INTERNET_FLAG_RELOAD,
			0);
		if (hUrl == NULL)
		{
			::InternetCloseHandle(hHttpRequest);
			::InternetCloseHandle(hHttpSession);
			::InternetCloseHandle(hNet);
			return NULL;
		}

		// 読み込み
		TCHAR m_wBuf[256] = { sizeof(TCHAR) };
		DWORD dwReadSize = 0;
		::InternetReadFile(hUrl, m_wBuf, 256, &dwReadSize);
		if (dwReadSize == 256) {
			::InternetCloseHandle(hHttpRequest);
			::InternetCloseHandle(hHttpSession);
			::InternetCloseHandle(hUrl);
			::InternetCloseHandle(hNet);
			OutputDebugString(_T("Exception occured.\n"));
			return _T("-1");
		}
		m_wBuf[dwReadSize] = '\0';

		// クローズ
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
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
		char cVersion[256] = { 0 };
		char* cTmp = NULL, * cNext = NULL;
		cTmp = ::strtok_s(m_cBuf, "%d", &cNext);
		if (::strcmp(cTmp, "%d") == 0)
		{
			cTmp = ::strtok_s(NULL, "\n", &cNext);
			::strcpy_s(cVersion, 256, cTmp);
		}
		CString FINAL;
		FINAL = cTmp;
		OutputDebugString(_T("FFmpeg version: ") + FINAL + _T("\n"));

		return FINAL;
	}
}


CString libCoreUtility::AppUpdateCheck() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HINTERNET    hNet;
	HINTERNET    hUrl;
	HINTERNET hHttpSession;
	HINTERNET hHttpRequest;
	DWORD BufSizeTextSize = 1000;
	DWORD StatusCode = sizeof(DWORD);
	DWORD StatusCode_Size = sizeof(DWORD);
	wchar_t Path[MAX_PATH + 1], Drive[10], Dir[MAX_PATH], Name[MAX_PATH], Ext[MAX_PATH];
	wchar_t ptr[MAX_PATH] = { 0 };
	TCHAR m_cPath[MAX_PATH + 1];

	CString URL;
	CString URL2;

	if (0 != GetModuleFileName(NULL, Path, MAX_PATH)) {// 実行ファイルの完全パスを取得
		_tsplitpath_s(Path, Drive, Dir, Name, Ext);
		CString NAME = Name;
		URL = _T("https://res.xyle-official.com/versions/") + NAME.MakeLower() + _T("/");
		URL2 = _T("https://res.xyle-official.com/versions/") + NAME.MakeLower() + _T("/VERSIONINFO");
	}

	// 初期化
	hNet = ::InternetOpen(L"UpdateCheck...",
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		0);
	if (hNet == NULL)
		return NULL;

	/* サーバへ接続 */
	hHttpSession = InternetConnect(
		hNet,
		_T("xyle-official.com"),
		INTERNET_DEFAULT_HTTP_PORT,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		0);

	/* HTTP要求の作成 */
	hHttpRequest = HttpOpenRequest(
		hHttpSession,
		_T("GET"),
		_T("VERSIONINFO"),
		NULL,
		URL,
		NULL,
		0,
		0);

	/* 作成したHTTP要求の発行 */
	BOOL bHttpSendRequest = HttpSendRequest(
		hHttpRequest,
		NULL,
		0,
		NULL,
		0);

	if (bHttpSendRequest == 0) {//bHttpSendRequestが0の場合はインターネットに接続されていないので、エラーを返す。
		::InternetCloseHandle(hNet);
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
		return NULL;//106:ERR_INTERNET_DISCONNECTED
	}

	/* 返されたコンテンツの長さを取得 */
	HttpQueryInfo(
		hHttpRequest,
		HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
		&StatusCode,
		&StatusCode_Size,
		NULL);

	if (StatusCode != HTTP_STATUS_OK) {
		OutputDebugString(_T("HTTP Request failed.\n"));
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
		::InternetCloseHandle(hNet);
		return NULL;
	}
	else {
		OutputDebugString(_T("HTTP Request completed.\n"));

		// バージョンチェック用のページ
		::_tcscpy_s(m_cPath, MAX_PATH, URL2);

		// URLオープン
		hUrl = ::InternetOpenUrl(hNet,
			m_cPath,
			NULL,
			0,
			INTERNET_FLAG_RELOAD,
			0);
		if (hUrl == NULL)
		{
			::InternetCloseHandle(hHttpRequest);
			::InternetCloseHandle(hHttpSession);
			::InternetCloseHandle(hNet);
			return NULL;
		}

		// 読み込み
		TCHAR m_wBuf[256] = { sizeof(TCHAR) };
		DWORD dwReadSize = 0;
		::InternetReadFile(hUrl, m_wBuf, 256, &dwReadSize);
		if (dwReadSize == 256) {
			::InternetCloseHandle(hHttpRequest);
			::InternetCloseHandle(hHttpSession);
			::InternetCloseHandle(hUrl);
			::InternetCloseHandle(hNet);
			OutputDebugString(_T("Exception occured.\n"));
			return _T("-1");
		}
		m_wBuf[dwReadSize] = '\0';

		// クローズ
		::InternetCloseHandle(hHttpRequest);
		::InternetCloseHandle(hHttpSession);
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
		char cVersion[256] = { 0 };
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
}


CString libCoreUtility::AppCurrentVersionCheck() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DWORD dwLen;
	DWORD dwHandle;
	TCHAR cBuf[256] = { NULL };
	wchar_t Path[MAX_PATH + 1];

	if (0 != GetModuleFileName(NULL, Path, MAX_PATH)) {// 実行ファイルの完全パスを取得
	}
	const TCHAR* const AppName = Path;

	dwLen = ::GetFileVersionInfoSize(AppName, &dwHandle);
	if (dwLen > 0)
	{
		char* lpData = new char[static_cast<DWORD64>(dwLen) + 1];
		::GetFileVersionInfo(AppName, NULL, dwLen, lpData);

		// 情報を取得する
		void* pvVersion;
		UINT VersionLen;
		::VerQueryValue(lpData, L"\\StringFileInfo\\041104b0\\ProductVersion", &pvVersion, &VersionLen);

		::_stprintf_s(cBuf, 256, _T("%s"), static_cast<wchar_t*>(pvVersion));

		delete[] lpData;
	}
	return cBuf;
}


bool libCoreUtility::DownloadFile(LPCTSTR pszURL, LPCTSTR pszLocalFile, DWORD dwBuffSize) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//TCHAR		pszHeader[] = _T("Accept: */*\r\n\r\n");
	BOOL		ret;
	DWORD		dwReadSize = 0;
	DWORD		dwWrittenSize;
	BYTE* pcbBuff;
	HINTERNET	hInternet;
	HINTERNET	hConnect;
	HANDLE		hFile;

	hInternet = ::InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return	false;

	hConnect = ::InternetOpenUrl(hInternet, pszURL, NULL, -1, INTERNET_FLAG_DONT_CACHE, 0);
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

BOOL libCoreUtility::ExtractZip(IShellDispatch* pShellDisp, TCHAR* ZipPath, TCHAR* OutPath) {
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


UINT libCoreUtility::GetDirectoryFileCount(const std::basic_string<TCHAR>& src) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	std::basic_string<TCHAR> root = src + _T("\\*.*");
	WIN32_FIND_DATA wfd = {};
	HANDLE handle = FindFirstFile(root.c_str(), &wfd);
	if (handle == INVALID_HANDLE_VALUE) return 0;

	UINT result = 0;
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


UINT_PTR libCoreUtility::AfxGetFileSize(TCHAR* oFilePath) {
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

// ディレクトリとディレクトリ内のファイルを一括削除
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

// WinAPIで全ファイルの削除
BOOL libCoreUtility::DeleteALLFiles(LPCTSTR DeletePathName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strPath = DeletePathName;
	SHFILEOPSTRUCT SHFILE{};
	SHFILE.hwnd = ::GetDesktopWindow();
	SHFILE.wFunc = FO_DELETE;
	SHFILE.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
	SHFILE.fAnyOperationsAborted = TRUE;
	SHFILE.hNameMappings = NULL;
	SHFILE.lpszProgressTitle = L"";

	strPath += "?";
	strPath.SetAt(strPath.GetLength() - 1, NULL);
	SHFILE.pFrom = strPath;
	SHFILE.pTo = NULL;
	SHFileOperation(&SHFILE);
	WaitForSingleObject(&SHFILE, INFINITE);
	ZeroMemory(&SHFILE, sizeof(SHFILEOPSTRUCT));

	return FALSE;
}

// WinAPIで'from'から'to'へ全ファイルをコピー
BOOL libCoreUtility::CopyALLFiles(LPCTSTR from, LPCTSTR to)
{
	CString strFrom = from;
	CString strTo = to;
	SHFILEOPSTRUCT SHFILE{};
	SHFILE.hwnd = ::GetDesktopWindow();
	SHFILE.wFunc = FO_COPY;
	SHFILE.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI;
	SHFILE.fAnyOperationsAborted = TRUE;
	SHFILE.hNameMappings = NULL;
	SHFILE.lpszProgressTitle = L"";

	strFrom += "0";
	strFrom.SetAt(strFrom.GetLength() - 1, NULL);
	strTo += "0";
	strTo.SetAt(strTo.GetLength() - 1, NULL);
	SHFILE.pFrom = strFrom;
	SHFILE.pTo = strTo;
	SHFileOperation(&SHFILE);
	WaitForSingleObject(&SHFILE, INFINITE);
	ZeroMemory(&SHFILE, sizeof(SHFILEOPSTRUCT));

	return FALSE;
}

// 特定文字列を置き換える [CString]
CString libCoreUtility::AfxReplaceStr(CString& replacedStr, CString from, CString to) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	const unsigned int pos = replacedStr.Find(from);
	const int len = from.GetLength();

	if (pos == std::string::npos || from.IsEmpty()) {
		return replacedStr;
	}

	replacedStr.Replace(from, to);
	return replacedStr;
}

// 特定文字列を置き換える [CStringA]
CStringA libCoreUtility::AfxReplaceStrA(CStringA& replacedStr, CStringA from, CStringA to) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	const unsigned int pos = replacedStr.Find(from);
	const int len = from.GetLength();

	if (pos == std::string::npos || from.IsEmpty()) {
		return replacedStr;
	}

	replacedStr.Replace(from, to);
	return replacedStr;
}

CString libCoreUtility::AfxReplaceStrPtr(LPCTSTR& replacedStr, LPCTSTR from, LPCTSTR to) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strReplaced = replacedStr;
	CString strFrom = from;
	CString strTo = to;

	const unsigned int pos = strReplaced.Find(strFrom);
	const int len = strFrom.GetLength();

	if (pos == std::string::npos || strFrom.IsEmpty()) {
		return strReplaced;
	}

	strReplaced.Replace(strFrom, strTo);
	return strReplaced;
}

void libCoreUtility::RunProcess(LPWSTR lpCommandLine, WORD WindowFlags)
{
	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = WindowFlags;
	::CreateProcess(NULL, lpCommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);

	return;
}

// exe名を検索し、プロセスを強制終了させる
void libCoreUtility::TerminateExeName(const TCHAR* TrFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	PROCESSENTRY32 pe{};
	pe.dwSize = sizeof(pe);
	UINT len = 0;

	// ハンドルの取得
	HANDLE hSnap;
	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// ハンドルがエラー値の場合
	if (hSnap != INVALID_HANDLE_VALUE) {
		// 全てのプロセスをチェックする
		BOOL bResult = Process32First(hSnap, &pe);
		while (bResult)
		{
			// 次を取得
			bResult = Process32Next(hSnap, &pe);
			// 実行ファイル名でプロセスID検索
			if (!StrCmp(pe.szExeFile, TrFileName))
			{
				// プロセスIDからハンドルの取得
				HANDLE hProcess = OpenProcess(
					PROCESS_ALL_ACCESS,
					FALSE,
					pe.th32ProcessID);

				// プロセス強制終了
				TerminateProcess(hProcess, 0);

				// プロセスを閉じる
				DWORD dwExitCode = 0;
				GetExitCodeProcess(hProcess, &dwExitCode);
				CloseHandle(hProcess);
			}
		}
		CloseHandle(hSnap);
	}
	return;
}

CString libCoreUtility::GetCPUInfo()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int CPUInfo[5] = { -1 };
	__cpuid(CPUInfo, 0x80000000);
	if (CPUInfo[0] >= 0x80000004)
	{
		CHAR szCPUBrandString[0x41] = { 0 };
		__cpuid(CPUInfo, 0x80000002);
		memcpy(szCPUBrandString, CPUInfo, sizeof(CPUInfo));
		__cpuid(CPUInfo, 0x80000003);
		memcpy(szCPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		__cpuid(CPUInfo, 0x80000004);
		memcpy(szCPUBrandString + 32, CPUInfo, sizeof(CPUInfo));

		const int tchrSize = sizeof(szCPUBrandString) + 1;
		TCHAR tchrText2[tchrSize]{};
		int res = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szCPUBrandString, sizeof(szCPUBrandString), tchrText2, tchrSize);

		CString CPUText = CString(tchrText2);
		CString CPUID = L"" + CPUText + L"\n";

		if (CPUText.Compare(_T("")) == 0) {
			return NULL;
		}
		else {
			return CPUID;
		}
	}
	return NULL;
}

CString libCoreUtility::GetGPUInfo()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString ret;
	HRESULT hres;
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres)) return NULL;
	hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	if (FAILED(hres)) { CoUninitialize(); return NULL; }
	IWbemLocator* pLoc = NULL;
	hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
	if (FAILED(hres)) { CoUninitialize(); return NULL; }
	IWbemServices* pSvc = NULL;
	hres = pLoc->ConnectServer(_bstr_t(L"root\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
	if (FAILED(hres)) { pLoc->Release(); CoUninitialize(); return NULL; }
	hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
	if (FAILED(hres)) { pSvc->Release(); pLoc->Release(); CoUninitialize(); return NULL; }
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_VideoController"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);
	if (FAILED(hres)) { pSvc->Release(); pLoc->Release(); CoUninitialize(); return NULL; }
	IWbemClassObject* pclsObj{};
	ULONG uReturn = 0;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if (0 == uReturn)break;
		VARIANT vtProp;
		VariantInit(&vtProp);
		hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);
		wcout << " OS Name : " << vtProp.bstrVal << endl;
		ret = vtProp.bstrVal;
		VariantClear(&vtProp);
	}
	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	pclsObj->Release();
	CoUninitialize();

	return ret;
}

// libMainString

// wstringをstringに変換する
std::string libMainString::TWStringToString(const std::wstring& arg_wstr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	std::string result;
	size_t length = arg_wstr.size();
	size_t convLength;
	char* c = (char*)malloc(sizeof(char) * length * 2);
	wcstombs_s(&convLength, c, sizeof(char) * length * 2, arg_wstr.c_str(), length * 2);
	if (c) {
		result = c;
	}
	SAFE_FREE(c);

	return result;
};

// stringをwstringに変換する
std::wstring libMainString::StringToWString(const std::string& arg_str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	std::wstring result;
	size_t length = arg_str.size();
	size_t convLength;
	wchar_t* wc = (wchar_t*)malloc(sizeof(wchar_t) * (length + 2));
	mbstowcs_s(&convLength, wc, length + 1, arg_str.c_str(), _TRUNCATE);
	if (wc) {
		result = wc;
	}
	SAFE_FREE(wc);

	return result;
};

// カレントディレクトリを取得する関数
CString libMainString::CURRENT_DIR() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TCHAR* crDir = (TCHAR*)malloc(sizeof(TCHAR) * MAX_PATH + 1);
	GetCurrentDirectory(MAX_PATH, crDir);
	if (crDir != NULL) {
		CString FUNC = crDir;
		SAFE_FREE(crDir);
		return FUNC;
	}
	else {
		SAFE_FREE(crDir);
		return NULL;
	}
}

// リソース用DLLのパスを取得する
CString libMainString::RESOURCELIB_PATH() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = libMainString::CURRENT_DIR() + RESOURCEDLL_PATH;
	return FUNCTION;
}

// 日本語リソース用DLLのパスを取得する
CString libMainString::JPN_RESOURCELIB_PATH() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = libMainString::CURRENT_DIR() + JPNRESOURCEDLL_PATH;
	return FUNCTION;
}

// 英語リソース用DLLのパスを取得する
CString libMainString::ENG_RESOURCELIB_PATH() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = libMainString::CURRENT_DIR() + ENGRESOURCEDLL_PATH;
	return FUNCTION;
}

// GUIのデフォルトテキスト
CString libMainString::DEFAULTTEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = _T("なにも読み込んでいません");
	return FUNCTION;
}

// GUIの読み込み完了テキスト
CString libMainString::READEDTEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = _T("読み込み完了");
	return FUNCTION;
}

// 空のテキスト
CString libMainString::NULLTEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = _T("");
	return FUNCTION;
}

// libVersionString

// アプリケーションのバージョンテキスト
CString libVersionString::APP_VERSION_TEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	NEW_COREUTIL;
	CString FUNCTION = Utility->AppCurrentVersionCheck();
	SAFE_DELETE(COREUTIL_FUNC);
	return FUNCTION;
}

// waifu2x-ncnn-vulkanのバージョンテキスト
CString libVersionString::CMD_VERSION_TEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = _T("20210210");
	return FUNCTION;
}

// FFmpegのバージョンテキスト
CString libVersionString::FF_VERSION_TEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	wchar_t ver[96]{};
	GetPrivateProfileString(L"FFMPEG_LATEST_VERSION", L"0x00000", NULL, (LPWSTR)ver, 48, L".\\settings.ini");
	CString VER = ver;
	VER += _T(" (www.gyan.dev)");
	return VER;
}