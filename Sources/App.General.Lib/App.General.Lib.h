// App.General.Lib.h : App.General.Lib DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CAppGeneralLibApp
// このクラスの実装に関しては App.General.Lib.cpp をご覧ください
//

class CAppGeneralLibApp : public CWinApp
{
public:
	CAppGeneralLibApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
