//-----------------------------------------------------------------------------
//!
//!	@file	gmLibrary.h
//!	@brief	ゲームエンジンインクルードヘッダ
//! @author	Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once
#pragma message(" + gmLibrary.h")

#include "targetver.h"

//--------------------------------------------------------------
// Windows関連
//--------------------------------------------------------------
#include <windows.h>

//---- timeGetTime関連
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>

//---- GDI+ヘッダ関連
#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")	// リンク時に必要なライブラリ


//--------------------------------------------------------------
// C ランタイム ヘッダー ファイル
//--------------------------------------------------------------
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <locale.h>
#include <ctime>

#include <process.h>			// _beginthreadex()
using namespace std;

//--------------------------------------------------------------
// STL
//--------------------------------------------------------------
#include <vector>
#include <list>
#include <memory>

//--------------------------------------------------------------
// メモリリーク検出用
//--------------------------------------------------------------
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif	// _DEBUG

//--------------------------------------------------------------
// OpenGL関連
//--------------------------------------------------------------
#include <glew/include/GL/glew.h>
#include <glew/include/GL/wglew.h>
#pragma comment(lib, "glew32.lib")
#include <freeglut/include/GL/freeglut.h>


//--------------------------------------------------------------
// XInput関連
//--------------------------------------------------------------
#pragma comment(lib,"XInput.lib")
#include<XInput/Xinput.h>


//-------------------------------------------------------------
// ゲーム設定
//-------------------------------------------------------------
#define	GAME_MODE 0						//!< ゲームモード

//-------------------------------------------------------------
//	ゲームエンジン
//-------------------------------------------------------------
#include "gmTypedef.h"					// 型
#include "gmType.h"						// 型情報

#include "gmRefCounter.h"				// 参照カウンタ
#include "gmBase.h"						// 基底
#include "gmWindowInfo.h"				// ウィンドウ情報

//---- 構造体関連(Struct)
#include "gmVector.h"					// ベクトル
#include "gmMatrix.h"					// 行列
#include "gmQuaternion.h"				// クォータニオン(四元数)
#include "gmGeometry.h"					// 形状


//---- 操作関連(Key)
#include "gmMouse.h"					// マウス
#include "gmMouseManager.h"				// マウス管理
#include "gmController.h"				// コントローラー
#include "gmControllerManager.h"		// コントローラー管理

//---- シェーダ関連(Shader)
#include "gmShaderObjectBase.h"			// シェーダ基底
#include "gmVertexShader.h"				// 頂点シェーダ
#include "gmFragmentShader.h"			// フラグメントシェーダ
#include "gmProgramShader.h"			// プログラムシェーダ
#include "gmVertexArrayObject.h"		// 頂点フォーマット

//---- 汎用関数関連
#include "gmSystem.h"					// 汎用関数

//---- 描画関連(Render)
#include "gmHDRTexReader.h"				// HDRテクスチャ読み込み
#include "gmTexture.h"					// テクスチャ

//---- カメラ関連(Camera)
#include "gmCameraBase.h"				// カメラ基底
#include "gmCamera.h"					// カメラ
#include "gmCameraManager.h"			// カメラ管理

//---- データアセット関連(Asset)		
#include "gmAssetBase.h"				// アセット基底
#include "gmAssetTexture.h"				// テクスチャ
#include "gmAssetModelX.h"				// モデル(Xファイル)

//---- タスク関連(Task)
#include "gmTaskBase.h"					// タスク基底
#include "gmTaskModelX.h"				// モデル(Xファイル)

//---- ポストエフェクトフィルター(Post effect filter)
#include "gmTaskFilter.h"				// フィルタ基底
#include "gmTaskFilterBloom.h"			// Bloomグレア
#include "gmTaskFilterGlare.h"			// グレア
#include "gmTaskFilterTonemapping.h"	// トーンマッピング
#include "gmTaskFilterMotionBlur.h"		// モーションブラー
#include "gmTaskFilterLensFlare.h"		// レンズフレア
#include "gmTaskFilterGaussian.h"		// ぼかしフィルタ

//---- 当たり判定(Collision)
#include "gmAABBTree.h"					// AABBTree
#include "gmCollision.h"				// 衝突判定
#include "gmCollisionWall.h"			// 衝突判定(壁)
#include "gmLandScape.h"				// 衝突判定(地面)
#include "gmCollisionManager.h"			// 衝突管理クラス


//---- タイマー
#include "gmTimer.h"					// タイマー
#include "gmProfiler.h"					// プロファイラー

//---- シーン遷移関連(Scene)
#include "gmSceneBase.h"				// シーン基底

//---- システム関連 (System)
#include "gmSystemAsset.h"				// アセット管理
#include "gmSystemRender.h"				// 描画
#include "gmSystemShader.h"				// シェーダー
#include "gmSystemScene.h"				// シーン管理

#include "gmSystemMain.h"				// メイン


//! グローバル
namespace Global {
	extern	bool	debugMode;			//!< デバッグモード
	extern	bool	filterEnable;		//!< フィルタ有効フラグ
	extern	f32		deltaTime;			//!< デルタタイム
};


//=============================================================================
//	END OF FILE
//=============================================================================