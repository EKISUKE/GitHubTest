//=============================================================================
//!
//!	@file	Key.cpp
//!	@brief	オブジェクト操作
//!	@author	YukiIshigaki
//!
//=============================================================================
#include"Library.h"

//=============================================================================
// AI操作
//=============================================================================

//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
AI::AI(Man* me, Man* leader)
: KeyBase(me, leader)
, _oldTime		(0)
, _nowTime		(0)
{
	_keyType = KEY_TYPE_AI;
}


//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void AI::Update()
{
}


//=============================================================================
//	END OF FILE
//=============================================================================