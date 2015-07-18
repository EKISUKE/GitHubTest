//-----------------------------------------------------------------------------
//!
//!	@file	Enemy.h
//!	@brief	敵基底
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//! 敵基底
//-----------------------------------------------------------------------------
class EnemyBase : public Man
{
public:
	//----------------------------------------------------------
	//! @name 初期化
	//----------------------------------------------------------
	//@{
private:
	//! デフォルトコンストラクタ
	EnemyBase();
public:
	//! コンストラクタ
	//!	@param	[in]	myType	キータイプ
	EnemyBase(Man::KEY_TYPE myType);

	//! デストラクタ
	virtual ~EnemyBase();

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
	//! リーダーの周りに常にいる
	//!	@param	[in]	leader	リーダー座標
	virtual void	UpdateNormal(Vector3& leaderPos) = 0;
	//!	描画
	virtual void	Render(bool isShadow);

	//! glut描画関連
	virtual void	glutRender();
	//! デバッグ描画
	virtual void	debugRender();

	//! ステータスの描画(2D)
	virtual void	StatusRender();

protected:
	//! 座標設定しなおし
	void			ResetPosition();
	//! ダメージ計算
	virtual s32		calcDamageAndCollision();
	//! アニメーションによる移動不可チェック
	virtual bool	checkFreeze(const s32&	currentAnimation);

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{
public:
	//! リーダー座標を設定
	//!	@param	[in]	pos	リーダー座標
//	void			setLeaderPos(Vector3& pos) { _leaderPos = pos; }

	//! 分隊を設定
	void			setSquad(Squad*	squad)	   { _pSquad = squad; } 

protected:
	//! 狙う相手を探して設定
	virtual bool	searchAndSetTarget();
	//@}

	Collision		_attackCollision;	//!< 攻撃判定用
	Vector3			_goalPos;			//!< 目標座標
	Squad*			_pSquad;			//!< 自分の分隊の
	Man*			_pPrevTarget;		//!< 一つ前のターゲット情報
	bool			_goGoal;			//!< ゴールへ向かうフラグ
	f32				_modelScale;		//!< モデルサイズ(キャラサイズに掛ける分)

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