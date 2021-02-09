// GUI.ResourcePack.String.h : GUI.ResourcePack.String DLL のメイン ヘッダー ファイル
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CGUIResourcePackStringApp
// このクラスの実装に関しては GUI.ResourcePack.String.cpp をご覧ください
//

class CGUIResourcePackStringApp : public CWinApp
{
public:
	CGUIResourcePackStringApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
