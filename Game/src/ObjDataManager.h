//-----------------------------------------------------------------------------
//!
//!	@file	ObjDataManager.cpp
//!	@brief	オブジェクトデータ管理
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//! プロトタイプ宣言
class Player;
class AllyBase;
class EnemyBase;

//-----------------------------------------------------------------------------
//!	操作管理
//-----------------------------------------------------------------------------
class ObjDataManager
{
private:
	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	ObjDataManager();

	//! コピーコンストラクタ
	ObjDataManager(ObjDataManager& src){}

	//! デストラクタ
	virtual ~ObjDataManager();

public:

	//@}
	//-------------------------------------------------------------
	//!	@name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! インスタンスの取得
	static ObjDataManager*	getInstance()
	{
		static ObjDataManager _myInstance;
		return &_myInstance;
	}

	//! プレイヤーのセット
	void	setPlayer(Player* player)		{ _pPlayer = player; }
	//! 味方の追加
	void	addAllyData(AllyBase* add)		{ _pAllyData.push_back(add); }
	//! 敵の追加
	void	addEnemyData(EnemyBase*	add)	{ _pEnemyData.push_back(add); }
	//! 防衛座標の追加
	void	addDeffencePos(Vector3&	pos)	{ _deffencePos.push_back(pos); }

	//@}
	
	Player*				_pPlayer;
	vector<AllyBase*>	_pAllyData;
	vector<EnemyBase*>	_pEnemyData;
	vector<Vector3>		_deffencePos;

	Status*				_enemyStatus;
	Status*				_attackerStatus;
	Status*				_healerStatus;
	Status*				_deffenderStatus;
};



//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
ObjDataManager*		IObjDataManager()
{
	return ObjDataManager::getInstance();
}

//=============================================================================
//	END OF FILE
//=============================================================================