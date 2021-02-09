#pragma once

#ifdef _DEBUG
#define RESOURCEDLL_PATH _T("\\App.General.resLib.dll")
#define RESOURCEDLL_TEXT _T("App.General.resLib.dll")
#else
#define RESOURCEDLL_PATH _T("\\App.General.resLib.dll")
#define RESOURCEDLL_TEXT _T("App.General.resLib.dll")
#endif

#define SAFE_FREE(ptr) { free(ptr); ptr = NULL; }
#define SAFE_DELETE(ptr) { delete ptr; ptr = NULL; }

#define NEW_MAINSTR libMainString* libMainStr = new libMainString
#define NEW_VERSIONSTR libVersionString* libVersionStr = new libVersionString
#define NEW_CORE libCore* Core = new libCore
#define NEW_COREUTIL libCoreUtility* Utility = new libCoreUtility
#define NEW_WAIFU2X_UTIL libWaifu2x* Waifu2x = new libWaifu2x
#define MAINSTR_FUNC libMainStr
#define VERSIONSTR_FUNC libVersionStr
#define CORE_FUNC Core
#define COREUTIL_FUNC Utility
#define WAIFU2X_UTIL_FUNC Waifu2x

#define CURRENT_PATH libMainStr->CURRENT_DIR()
#define DEFAULT_TEXT libMainStr->DEFAULTTEXT()
#define READED_TEXT libMainStr->READEDTEXT()
#define NULL_TEXT libMainStr->NULLTEXT()
#define NOTFOUND_TEXT _T(" が見つからないため、コードの実行を続行できません。プログラムを再インストールすると、この問題が解決する可能性があります。")
#define DOWNLOAD_URL_STRING _T("https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-essentials.zip")

#define RES_LIBRARY libMainStr->RESOURCELIB_PATH()
#define APP_VERSION libVersionStr->APP_VERSION_TEXT()
#define CMD_VERSION libVersionStr->CMD_VERSION_TEXT()
#define FF_VERSION libVersionStr->FF_VERSION_TEXT()