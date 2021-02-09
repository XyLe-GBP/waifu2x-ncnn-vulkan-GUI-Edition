// App.General.Lib.cpp : DLL の初期化ルーチンを定義します。
//

#include "pch.h"
#include "framework.h"
#include "App.General.Lib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: この DLL が MFC DLL に対して動的にリンクされる場合、
//		MFC 内で呼び出されるこの DLL からエクスポートされたどの関数も
//		関数の最初に追加される AFX_MANAGE_STATE マクロを
//		持たなければなりません。
//
//		例:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 通常関数の本体はこの位置にあります
//		}
//
//		このマクロが各関数に含まれていること、MFC 内の
//		どの呼び出しより優先することは非常に重要です。
//		it は、次の範囲内で最初のステートメントとして表示されるべきです
//		らないことを意味します、コンストラクターが MFC
//		DLL 内への呼び出しを行う可能性があるので、オブ
//		ジェクト変数の宣言よりも前でなければなりません。
//
//		詳細については MFC テクニカル ノート 33 および
//		58 を参照してください。
//

// CAppGeneralLibApp

BEGIN_MESSAGE_MAP(CAppGeneralLibApp, CWinApp)
END_MESSAGE_MAP()


// CAppGeneralLibApp の構築

CAppGeneralLibApp::CAppGeneralLibApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CAppGeneralLibApp オブジェクト

CAppGeneralLibApp theApp;


// CAppGeneralLibApp の初期化

BOOL CAppGeneralLibApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
