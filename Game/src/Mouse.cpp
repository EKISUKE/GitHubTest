//-----------------------------------------------------------------------------
//!
//!	@file	Mouse.cpp
//!	@brief	マウス制御
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Mouse::Mouse()
{
	//-------------------------------------------------------------
	// マウスカーソルの位置を取得
	//-------------------------------------------------------------
	GetCursorPos(&_point);

	// 初回実行時は現在のマウスカーソルの位置を以前の位置にもセットしておく
	_oldPoint    = _point;
	_changeWheel = 0.0f;
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
Mouse::~Mouse()
{
}

//-----------------------------------------------------------------------------
//! 1フレーム分の更新
//-----------------------------------------------------------------------------
void Mouse::Update()
{
	//---- 今入っているものを以前のフレームとして使用
	_oldPoint = _point;
	
	//-------------------------------------------------------------
	// マウスカーソルの位置を取得
	//-------------------------------------------------------------
	GetCursorPos(&_point);
}

//-----------------------------------------------------------------------------
//! ホイール
//-----------------------------------------------------------------------------
void Mouse::MouseWheel(int wheel_number, int direction, int x, int y)
{
	_oldWheel = _wheel;

	if(direction==1){
		_wheel++;
	}else{
		_wheel--;
	}
	
	_changeWheel = (_wheel - _oldWheel) * 2;
}

//-----------------------------------------------------------------------------
//! カーソル移動量を取得
//-----------------------------------------------------------------------------
POINT Mouse::getOffset() const
{
	POINT result;
	result.x = _point.x - _oldPoint.x;
	result.y = _point.y - _oldPoint.y;

	return result;
}





//=============================================================================
//	END OF FILE
//=============================================================================