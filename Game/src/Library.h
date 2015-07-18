//-----------------------------------------------------------------------------
//!
//!	@file	Library.h
//!	@brief	ライブラリ用ヘッダ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//---- ゲームエンジンのほうインクルード
#include "../../GameEngine/src/gmLibrary.h"

//// Windowsでコンソールを出さないため
//#ifdef NDEBUG
//#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
//#endif




//---- メイン
#include "Main.h"				//!< メインシステム
//---- テンプレート系
#include "Singleton.h"			//!< シングルトンオブジェクト


//---- 構造体
#include "GLFont.h"				//!< 文字表示用

//---- サウンドプレイヤー
#include "SoundPlayer.h"		//!< サウンドプレイヤー
#include "SoundPlayerManager.h"	//!< サウンド再生管理


//---- ライト系
#include "LightBase.h"			//!< ライト基底
#include "Light.h"				//!< ライト
#include "LightManager.h"		//!< ライト管理

//---- シャドウ関連
#include "ShadowMap.h"			//!< シャドウマップ(1つ分)
#include "CascadedShadow.h"		//!< カスケードシャドウ


//---- エフェクト
#include "BillBoard.h"			//!< ビルボード
#include "Effect.h"				//!< エフェクト

//---- 当たり判定
#include "CollisionObject.h"	//!< オブジェクト用当たり判定

//---- オブジェクト系
#include "StatusAssets.h"		//!< ステータスデータ
#include "Status.h"				//!< ステータス
#include "IObj.h"				//!< オブジェクトインターフェース
#include "ObjDataManager.h"		//!< オブジェクトデータ管理
#include "ObjBase.h"			//!< オブジェクト基底
//---- 操作管理系
#include "Key.h"				//!< オブジェクト操作インターフェース
#include "KeyAttacker.h"		//!< 攻撃タイプ操作
#include "KeyDeffender.h"		//!< 防御タイプ操作
#include "KeyHealer.h"			//!< 回復タイプ操作
#include "KeyInput.h"			//!< キー操作


//---- スカイボックス
#include "SkyBox.h"				//!< スカイボックス


//---- マップ
#include "Map.h"				//!< マップ

//---- キャラ
#include "Man.h"				//!< ゲーム内キャラ

//---- 分隊
#include "Squad.h"				//!< 分隊
#include "SquadManager.h"		//!< 分隊管理

//---- プレイヤー
#include "Player.h"				//!< プレイヤー

//---- NPCのタイプ
#include "Enemy.h"				//!< 敵基底
#include "EnemyLeader.h"		//!< 敵リーダータイプ
#include "EnemyAttacker.h"		//!< 敵攻撃タイプ
#include "EnemyDeffender.h"		//!< 敵防御タイプ

//---- 味方
#include "Ally.h"				// 味方基底
#include "AllyAttacker.h"		// 味方攻撃タイプ
#include "AllyDeffender.h"		// 味方防御タイプ
#include "AllyHealer.h"			// 味方回復タイプ

//---- シーン関係
#include "SceneTitle.h"			// タイトル
#include "SceneOption.h"		// オプション
#include "SceneGame.h"			// ゲーム中




//=============================================================================
//	END OF FILE
//=============================================================================