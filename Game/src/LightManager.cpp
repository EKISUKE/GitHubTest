//-----------------------------------------------------------------------------
//!
//!	@file	LightManager.cpp
//!	@brief	光源管理クラス
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
LightManager::LightManager()
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
LightManager::~LightManager()
{
	vector<LightBase*>::iterator itr;
	for( itr = _pVecLight.begin(); itr != _pVecLight.end(); )
	{
		LightBase* light = *itr;
		SAFE_DELETE(light);
		itr = _pVecLight.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//! ライトを探す
//!	@param	[in]	search	探すライトのポインタ
//-----------------------------------------------------------------------------
bool LightManager::sameLight(LightBase* search)
{
	vector<LightBase*>::iterator itr;
	// 同じシャドウがあるかどうか
	for( itr=_pVecLight.begin(); itr!=_pVecLight.end(); )	{
		LightBase* light = *itr;
		// 同じかどうか
		if( memcpy(search, light, sizeof(LightBase)) == 0 ) {
			// 同じなら返す
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//! 登録
//-----------------------------------------------------------------------------
void LightManager::regist(LightBase* light)
{
	if( sameLight(light)) {
		GM_ASSERT(false, "同じライトが登録されようとしました");
		return;
	}
	// 登録
	_pVecLight.push_back(light);
}

//-----------------------------------------------------------------------------
//! 解除
//-----------------------------------------------------------------------------
void LightManager::unregist(LightBase* light)
{
	vector<LightBase*>::iterator itr;
	LightBase* comp;
	for( itr=_pVecLight.begin(); itr!=_pVecLight.end(); )	{
		comp = *itr;
		// 同じかどうか
		if( memcpy(light, comp, sizeof(LightBase)) == 0 ) {
			// 同じなら解除
			SAFE_DELETE(comp);
			itr = _pVecLight.erase(itr);
			break;
		}
	}
}


//=============================================================================
//	END OF FILE
//=============================================================================