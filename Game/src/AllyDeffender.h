//-----------------------------------------------------------------------------
//!
//!	@file	AllyDeffender.h
//!	@brief	味方防御タイプ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
// ここでmyNameをマクロで宣言
DEF_COL_NAME_HEADER(AllyDeffender);
DEF_COL_ATTACK_NAME_HEADER(AllyDeffenderAttack);
//-----------------------------------------------------------------------------
//!	味方(防御タイプ)
//-----------------------------------------------------------------------------
class	AllyDeffender : public AllyBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	AllyDeffender();
	//! デストラクタ
	virtual ~AllyDeffender();
	//! 初期化
	virtual bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	virtual void	Update();
	//! 命令時の更新
	virtual void	UpdateByCommand();
	//! プレイヤーの周りに常にいる
	//!	@param	[in]	player	プレイヤー座標
	virtual void	UpdateNormal(Vector3& playerPos);
	//! 描画
	virtual void	Render(bool isShadow);

	//! ダメージ計算と当たり判定処理
	virtual s32		calcDamageAndCollision();

	//@}

private:
	bool			_isSet;		//!< プレイヤーの近くに着いたかどうか
};


//=============================================================================
//	END OF FILE
//=============================================================================