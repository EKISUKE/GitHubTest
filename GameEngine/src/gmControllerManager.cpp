//-----------------------------------------------------------------------------
//!
//!	@file	gmControllerManager.cpp
//!	@brief	コントローラー管理
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//!	コンストラクタ
//-----------------------------------------------------------------------------
ControllerManager::ControllerManager()
: _currentCreateCount(0)
{
	
}

//-----------------------------------------------------------------------------
//!	デストラクタ
//-----------------------------------------------------------------------------
ControllerManager::~ControllerManager()
{
	vector<Controller*>::iterator itr;
	for( itr = _pVecController.begin(); itr != _pVecController.end(); )
	{
		//Controller* control = *itr;
		SAFE_DELETE(*itr);
		itr = _pVecController.erase(itr);
	}
}


//-----------------------------------------------------------------------------
//! コントローラーの作成
//!	@retval 作成したコントローラーのポインタ nullなら失敗
//-----------------------------------------------------------------------------
Controller*	ControllerManager::createController()
{
	// コントローラー作成
	Controller*	controller = new Controller(++_currentCreateCount);
	// 登録
	_pVecController.push_back(controller);
	return controller;
}



//=============================================================================
//	END OF FILE
//=============================================================================