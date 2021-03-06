//-----------------------------------------------------------------------------
//!
//!	@file	AllyAttacker.h
//!	@brief	味方攻撃タイプ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

// ここでmyNameをマクロで宣言
DEF_COL_NAME_HEADER(AllyAttacker);
DEF_COL_ATTACK_NAME_HEADER(AllyAttackerAttack);
//-----------------------------------------------------------------------------
//!	味方(攻撃タイプ)
//-----------------------------------------------------------------------------
class AllyAttacker : public AllyBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	AllyAttacker();
	//! デストラクタ
	virtual ~AllyAttacker();

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
};
//=============================================================================
//	END OF FILE
//=============================================================================