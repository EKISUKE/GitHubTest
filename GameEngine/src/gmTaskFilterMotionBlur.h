//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterMotionBlur.h
//!	@brief	モーションブラー
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	モーションブラー
//-----------------------------------------------------------------------------
class TaskFilterMotionBlur : public TaskFilter
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	TaskFilterMotionBlur();

	//! デストラクタ
	virtual ~TaskFilterMotionBlur();

	//@}
	//-------------------------------------------------------------
	//!	@name	タスク関数
	//! 関数を継承してカスタムします。
	//-------------------------------------------------------------
	//@{

	//! 初期化
	virtual bool		setup();

	//! 解放
	virtual void		cleanup();

	//! 並列更新
	virtual void		asyncUpdate(){}

	//! 直列更新
	virtual void		update(){}

	//! 描画
	virtual void		render();

	//@}
private:
	Matrix				_viewMatrixLast;
};


//=============================================================================
//	END OF FILE
//=============================================================================