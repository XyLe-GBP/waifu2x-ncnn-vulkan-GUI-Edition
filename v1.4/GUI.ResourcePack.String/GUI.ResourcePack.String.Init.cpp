#include "pch.h"
#include "GUI.ResourcePack.String.Init.h"


// libMainString

CString libMainString::CURRENT_DIR() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TCHAR* crDir = (TCHAR*)malloc(sizeof(TCHAR) * MAX_PATH + 1);
	GetCurrentDirectory(MAX_PATH, crDir);
	if (crDir != NULL) {
		CString FUNC = crDir;
		free(crDir);
		return FUNC;
	}
	else {
		free(crDir);
		return NULL;
	}
}


CString libMainString::RESOURCELIB_PATH() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = libMainString::CURRENT_DIR() + RESOURCEDLL_PATH;
	return FUNCTION;
}


CString libMainString::DEFAULTTEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = _T("Ç»Ç…Ç‡ì«Ç›çûÇÒÇ≈Ç¢Ç‹ÇπÇÒ");
	return FUNCTION;
}


CString libMainString::READEDTEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = _T("ì«Ç›çûÇ›äÆóπ");
	return FUNCTION;
}


CString libMainString::NULLTEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = _T("");
	return FUNCTION;
}

// libVersionString

CString libVersionString::APP_VERSION_TEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = _T("1.4.0.1 (v1401)");
	return FUNCTION;
}


CString libVersionString::CMD_VERSION_TEXT() {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString FUNCTION = _T("20200818");
	return FUNCTION;
}

