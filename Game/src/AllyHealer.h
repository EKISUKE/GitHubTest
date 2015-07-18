//-----------------------------------------------------------------------------
//!
//!	@file	AllyHealer.h
//!	@brief	味方回復タイプ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

// ここでmyNameをマクロで宣言
DEF_COL_NAME_HEADER(AllyHealer);
DEF_COL_ATTACK_NAME_HEADER(AllyHealerAttack);
//-----------------------------------------------------------------------------
//!	味方(回復タイプ)
//-----------------------------------------------------------------------------
class	AllyHealer : public AllyBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	AllyHealer();
	//! デストラクタ
	virtual ~AllyHealer();

	//! 初期化
	virtual bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! ヒール
	//!	@param	[in]	target	目標
	void			Heal(Man*	target);
	//! 更新
	virtual void	Update();
	//! 命令時の更新
	virtual void	UpdateByCommand();
	//! プレイヤーの周りに常にいる
	//!	@param	[in]	player	プレイヤー座標
	virtual void	UpdateNormal(Vector3& playerPos);
	
	//! 描画
	virtual void	Render(bool isShadow);

	//! 狙う相手を探して設定
	//!	@return true:敵を設定完了 false:狙う敵はいない
	virtual bool	searchAndSetTarget();

	//! ダメージ計算と当たり判定処理
	virtual s32		calcDamageAndCollision();
	//@}
private:
	s32				_SeverityMax;	//!< 緊急度最大
	bool			_keyUpdate;		//!< キー更新
	bool			_isHeal;		//!< 回復中
	Man*			_target;		//!< ターゲット
};
//=============================================================================
//	END OF FILE
//=============================================================================