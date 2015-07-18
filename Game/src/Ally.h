//-----------------------------------------------------------------------------
//!
//!	@file	Ally.h
//!	@brief	味方基底
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	味方基底
//-----------------------------------------------------------------------------
class AllyBase : public Man
{
public:
	
	//----------------------------------------------------------
	//! @name 初期化
	//----------------------------------------------------------
	//@{

private:
	//! デフォルトコンストラクタ
	AllyBase();
public:
	//! コンストラクタ
	//!	@param	[in]	myType	キータイプ
	AllyBase(Man::KEY_TYPE myType);

	//! デストラクタ
	virtual ~AllyBase();

	//!	初期化
	virtual bool	Initialize();

	//! アニメーション初期化
	void			initAnimation();
	
	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新(この中で更新すべきUpdateを呼び出す)
	virtual void	Update();
	//! 命令時の更新
	virtual void	UpdateByCommand() = 0;
	//! プレイヤーの周りに常にいる
	//!	@param	[in]	player	プレイヤー座標
	virtual void	UpdateNormal(Vector3& playerPos) = 0;
	//!	描画
	virtual void	Render(bool isShadow);

	//! glut描画関連
	virtual void	glutRender();
	//! デバッグ描画
	virtual void	debugRender();

	//! ステータスの描画(2D)
	virtual void	StatusRender();

	//! 攻撃
	//virtual void	Attack();

protected:
	//! 座標設定しなおし
	void			ResetPosition();

	//! 狙う相手を探して設定
	//!	@return true:敵を設定完了 false:狙う敵はいない
	virtual bool	searchAndSetTarget();

	//! ダメージ計算と当たり判定処理
	virtual s32		calcDamageAndCollision(){ return 0; }

	//! アニメーションによる移動不可チェック
	virtual bool	checkFreeze(const s32& currentAnimation);
		
	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{
public:
	//! リーダー座標を設定
	//!	@param	[in]	pos	リーダー座標
	void			setLeaderPos(Vector3& pos) { _leaderPos = pos; }

	//! 分隊を設定
	void			setSquad(Squad*	squad)	   { _pSquad = squad; } 
	//@}
protected:
	//ALLY_TYPE		_allyType;
	//Collision	_myCollision;		//!< 衝突判定用
	Collision		_attackCollision;	//!< 攻撃判定用
	Vector3			_leaderPos;			//!< リーダーの座標
	Vector3			_DebugGoalPos;		//!< 目標座標(デバッグ用)
	Squad*			_pSquad;			//!< 自分の分隊
	Man*			_pPrevTarget;		//!< 一つ前のターゲット情報
	bool			_goGoal;			//!< ゴールへ向かうフラグ

	//---- アニメーション制御
	s32				_aniMove;
	s32				_aniAttack;
	s32				_aniWait;
	s32				_aniDamage;
	s32				_aniHeal;
};


//=============================================================================
//	END OF FILE
//=============================================================================