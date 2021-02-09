#pragma once

#ifdef _DEBUG
#define RESOURCEDLL_PATH _T("\\GUI.ResourcePack.dll")
#define RESOURCEDLL_TEXT _T("GUI.ResourcePack.dll")
#else
#define RESOURCEDLL_PATH _T("\\GUI.ResourcePack.dll")
#define RESOURCEDLL_TEXT _T("GUI.ResourcePack.dll")
#endif

#define NEW_MAINSTR libMainString* libMainStr = new libMainString;
#define NEW_VERSIONSTR libVersionString* libVersionStr = new libVersionString;
#define NEW_CORE libCore* Core = new libCore;
#define NEW_COREUTIL libCoreUtility* Utility = new libCoreUtility;
#define DEL_MAINSTR delete libMainStr;
#define DEL_VERSIONSTR delete libVersionStr;
#define DEL_CORE delete Core;
#define DEL_COREUTIL delete Utility;

#define CURRENT_PATH libMainStr->CURRENT_DIR()
#define DEFAULT_TEXT libMainStr->DEFAULTTEXT()
#define READED_TEXT libMainStr->READEDTEXT()
#define NULL_TEXT libMainStr->NULLTEXT()
#define NOTFOUND_TEXT _T(" が見つからないため、コードの実行を続行できません。プログラムを再インストールすると、この問題が解決する可能性があります。")

#define RES_LIBRARY libMainStr->RESOURCELIB_PATH()
#define APP_VERSION libVersionStr->APP_VERSION_TEXT()
#define CMD_VERSION libVersionStr->CMD_VERSION_TEXT()