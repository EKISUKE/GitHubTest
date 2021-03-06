//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterGaussian.h
//!	@brief	ぼかしフィルター
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	ブルームグレアフィルター
//-----------------------------------------------------------------------------
class TaskFilterGaussian : public TaskFilter
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	TaskFilterGaussian();

	//! デストラクタ
	virtual ~TaskFilterGaussian();

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

	//! 描画(渡された画像をぼかして保存先に保存する)
	//!	@param	[in]	pSrc	ぼかす元画像
	//!	@param	[out]	pDest	ぼかした画像保存先
	virtual void		render(Texture* pSrc, Texture* pDest);

	//@}
private:
	s32					_blurPathCount;		//!< ブラーを掛ける回数
};


//=============================================================================
//	END OF FILE
//=============================================================================