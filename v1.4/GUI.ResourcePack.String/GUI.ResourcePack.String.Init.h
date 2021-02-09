#pragma once

#include "pch.h"
#include "GUI.ResourcePack.String.Define.h"


class libMainString {
public:
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
};