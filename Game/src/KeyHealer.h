//-----------------------------------------------------------------------------
//!
//!	@file	KeyHealer.h
//!	@brief	回復タイプの操作
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!	AI(回復タイプ)
//-----------------------------------------------------------------------------
class KeyHealer : public AI
{
public:
	//! 行動状態
	//@{

	enum MODE
	{
		MODE_APPROACH,	//!< 目標に接近
		MODE_HEAL,		//!< 目標を回復
		MODE_DEBUF,		//!< 目標にデバフ
		MODE_MAX		//!< 状態遷移の最大
	};

	//@}
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	KeyHealer(Man* me, Man* leader);
	//! デストラクタ
	virtual ~KeyHealer();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	virtual void Update();

	//@}
private:
	//! 目標を捜索
	//void		 Search();
	//! 目標に接近
	void		 Approach();
	//! デバフ魔法
	void		 DebufMagic();
protected:
	f32			_searchReach;		//!< 目標捜索範囲
	f32			_searchRot;			//!< 探索方向
	f32			_debufReach;		//!< デバフ可能範囲
	f32			_debufDelay;		//!< デバフのディレイ(感覚)
	f32			_healReach;			//!< 回復可能範囲
	f32			_healDelay;			//!< 回復のディレイ(感覚)
	s32			_healPower;			//!< 回復量

	//---- 回復遅延用
	u32			_healOldTime;		//!< 前回の時間
	u32			_healNowTime;		//!< 今の時間

	MODE		_mode;				//!< 行動状態用
	bool		_TargetisEnemy;		//!< ターゲットが敵かどうか
};


//=============================================================================
//	END OF FILE
//=============================================================================