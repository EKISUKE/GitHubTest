//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterGlare.h
//!	@brief	グレアフィルター
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	グレアフィルター
//-----------------------------------------------------------------------------
class TaskFilterGlare : public TaskFilter
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	TaskFilterGlare();

	//! デストラクタ
	virtual ~TaskFilterGlare();

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
};


//=============================================================================
//	END OF FILE
//=============================================================================