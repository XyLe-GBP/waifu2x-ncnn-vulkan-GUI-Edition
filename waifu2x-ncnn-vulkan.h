
// waifu2x-ncnn-vulkan.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'pch.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// Cwaifu2xncnnvulkanApp:
// このクラスの実装については、waifu2x-ncnn-vulkan.cpp を参照してください
//

class Cwaifu2xncnnvulkanApp : public CWinApp
{
public:
	Cwaifu2xncnnvulkanApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern Cwaifu2xncnnvulkanApp theApp;
