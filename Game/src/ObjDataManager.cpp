//-----------------------------------------------------------------------------
//!
//!	@file	ObjDataManager.cpp
//!	@brief	オブジェクトデータ管理
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
ObjDataManager::ObjDataManager()
:_enemyStatus	 (nullptr)
,_attackerStatus (nullptr)
,_healerStatus	 (nullptr)
,_deffenderStatus(nullptr)
{
	SAFE_NEW(_enemyStatus);
	SAFE_NEW(_attackerStatus);
	SAFE_NEW(_healerStatus);
	SAFE_NEW(_deffenderStatus);

	_enemyStatus->loadCSV("../Assets/Status/Status - Enemy.csv");
	_attackerStatus->loadCSV("../Assets/Status/Status - Attacker.csv");
	_healerStatus->loadCSV("../Assets/Status/Status - Healer.csv");
	_deffenderStatus->loadCSV("../Assets/Status/Status - Deffender.csv");

	_deffencePos.push_back(Vector3(0.0f));
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
ObjDataManager::~ObjDataManager()
{
	SAFE_DELETE(_enemyStatus);
	SAFE_DELETE(_attackerStatus);
	SAFE_DELETE(_healerStatus);
	SAFE_DELETE(_deffenderStatus);
}



//=============================================================================
//	END OF FILE
//=============================================================================