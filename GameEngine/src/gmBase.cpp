//-----------------------------------------------------------------------------
//!
//!	@file	gmBase.cpp
//!	@brief	基底クラス
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//	基底 Base
//-----------------------------------------------------------------------------
ClassType<Base>	Base::TYPE(NULL, "Base", "クラス基底");

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Base::Base()
: _refCount(1)		// 初期値は1
{
}

//=============================================================================
//	END OF FILE
//=============================================================================
