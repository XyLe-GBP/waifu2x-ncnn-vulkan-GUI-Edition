// GUI.ResourcePack.Main.h : GUI.ResourcePack.Main DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CGUIResourcePackMainApp
// このクラスの実装に関しては GUI.ResourcePack.Main.cpp をご覧ください
//

class CGUIResourcePackMainApp : public CWinApp
{
public:
	CGUIResourcePackMainApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
