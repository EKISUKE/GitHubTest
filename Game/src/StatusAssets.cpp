//-----------------------------------------------------------------------------
//!
//!	@file	StatusAssets.h
//!	@brief	ステータスデータ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//-----------------------------------------------------------------------------
//!	デフォルトコンストラクタ
//-----------------------------------------------------------------------------
StatusAssets::StatusAssets()
{
	loadAssetStatus();
}

//-----------------------------------------------------------------------------
//! ステータスの初期化
//-----------------------------------------------------------------------------
void StatusAssets::loadAssetStatus()
{
	static const GM_CSTR loadPath[STATUS_MAX] = {
		"../Assets/Status/Status - Attacker.csv",
		"../Assets/Status/Status - Deffender.csv",
		"../Assets/Status/Status - Player.csv",
		"../Assets/Status/Status - Enemy.csv",
		"../Assets/Status/Status - Healer.csv",
	};

	// ヒーラー以外は読み込み
	for( s32 i=0; i<STATUS_HEAL; ++i ) {
		_pStatusList[i] = nullptr;
		SAFE_NEW( _pStatusList[i] );
		_pStatusList[i]->loadCSV(loadPath[i]);
	}
	// ヒーラーは違うクラスなのでここでnewする
	_pStatusList[STATUS_HEAL] = new StatusHeal();
	_pStatusList[STATUS_HEAL]->loadCSV(loadPath[STATUS_HEAL]);
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
StatusAssets::~StatusAssets()
{
}

//=============================================================================
//	END OF FILE
//=============================================================================