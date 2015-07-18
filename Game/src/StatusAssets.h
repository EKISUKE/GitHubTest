//-----------------------------------------------------------------------------
//!
//!	@file	StatusAssets.h
//!	@brief	ステータスデータ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

class Status;

//-----------------------------------------------------------------------------
//!	ステータスデータ
//-----------------------------------------------------------------------------
class StatusAssets
{
public:

	// ステータスのタイプ
	enum STATUS_TYPE
	{
		STATUS_ATK,		// 攻撃タイプ
		STATUS_DEF,		// 防御タイプ
		STATUS_PLAYER,	// プレイヤー
		STATUS_ENEMY,	// 敵基本
		STATUS_HEAL,	// 回復タイプ

		STATUS_MAX
	};

	//-------------------------------------------------------------
	//!	@name 取得・設定
	//-------------------------------------------------------------
	//@{
	
	//! ステータスを取得
	Status*		getStatus(STATUS_TYPE status) { 
		Status*	result = _pStatusList[(s32)status]; 
		return result;
	}

	//! インスタンスの取得
	static StatusAssets*	getInstance()
	{
		static StatusAssets _myInstance;
		return &_myInstance;
	}
	//@}

private:
	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{
	//!	デフォルトコンストラクタ
	StatusAssets();
		
	//!	デストラクタ
	virtual ~StatusAssets();

	//! ステータスの初期化
	void	loadAssetStatus();
	//@}
	

private:
	Status*			_pStatusList[STATUS_MAX];
};


//-----------------------------------------------------------------------------
// アクセスインターフェース
//-----------------------------------------------------------------------------
inline
StatusAssets*	IStatusAssets()
{
	return StatusAssets::getInstance();
}
//=============================================================================
//	END OF FILE
//=============================================================================