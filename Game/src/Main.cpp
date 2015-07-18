//-----------------------------------------------------------------------------
//!
//!	@file	Main.cpp
//!	@brief	アプリケーションのエントリポイント
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"



//---- グローバル変数
bool	ExitFlag = false;;

extern Controller* gpControl;		// コントローラー

// キーボード用
//BYTE KeyTbl[256];


//! シーン状態
extern enum SCENE_STATE
{
	SCENE_TITLE,	// タイトル
	SCENE_GAME,		// ゲーム
	SCENE_CLEAR,	// ゲームクリア
	SCENE_OVER,		// ゲームオーバー
	SCENE_OPTION,	// オプション
	SCENE_EXIT,		// 終了
	SCENE_NUM		// シーンの最大数
};

SCENE_STATE	scene;	// シーン状態

SceneTitle*	pTitle;	// タイトルクラス
SceneGame*	pGame;	// ゲームクラス

s32			state;	//	ゲームの更新結果 0:ゲーム 1: クリア 2: オーバー



//---------------------------------------------------------------------------
//! 初期化コールバック
//---------------------------------------------------------------------------
bool Main::onInitialize()
{
	// シーンジャンプ	
	//GmScene()->jump(SceneGame::TYPE);
	GmScene()->jump(SceneTitle::TYPE);

	return true;
}

//---------------------------------------------------------------------------
//! 解放コールバック
//---------------------------------------------------------------------------
void Main::onFinalize()
{
	GmControlMan()->getController(1)->DisableVibration();
}

//===========================================================================
//! アプリケーション開始エントリ
//===========================================================================
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	Main	systemMain;
	systemMain.mainLoop("GameEngine", "ゲームエンジンテスト");	
}

//=============================================================================
//	END OF FILE
//=============================================================================