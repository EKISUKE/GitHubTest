//-----------------------------------------------------------------------------
//!
//!	@file	Player.h
//!	@brief	プレイヤー
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//! 名前と一致するかどうか
//bool	isPlayer(Collision* obj);
DEF_COL_NAME_HEADER(Player);
DEF_COL_ATTACK_NAME_HEADER(PlayerAttack);

//-----------------------------------------------------------------------------
//!	プレイヤー
//-----------------------------------------------------------------------------
class	Player	: public Man
{
public:
	// ! 命令列挙体
	enum	COMMAND
	{
		COMMAND_NONE,	// 通常
		COMMAND_ATTACK,	// 攻撃
		COMMAND_BACK,	// 撤退
		COMMAND_MAX
	};


	//! 状態
	enum   ANIMATION_STATE
	{
		STATE_NONE	 = -1,	// アニメーションなし
		STATE_MOVE,			// 移動中
		STATE_JUMP,			// ジャンプ
		STATE_ATTACK,		// 攻撃中
		STATE_WAIT,			// 待機
		STATE_DAMAGE,		// ダメージ
		STATE_GAMEOVER,		// ゲームオーバー
		
		STATE_MAX		
	};

public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Player();
	//! デストラクタ
	~Player();
	//! 初期化
	virtual bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新処理
	virtual void	Update();
	//! 通常時更新
	void	UpdateNormal(Vector3& stickState);
	//! 命令変更時更新
	void	UpdateCommandChange(Vector3& stickState);
	//! ゲームオーバー時の更新
	void	UpdateGameOver();

	//void	update(Controller& control, const f32 &CamAng);

	//! 描画処理
	virtual void	Render(bool isShadow);
	//! glut描画関連
	virtual void	glutRender();
	//! デバッグ描画
	virtual void	debugRender();
	//! ステータスの描画(2D)
	virtual void	StatusRender();
	//! 命令用描画
	void			CommandRender();
	
protected:
	//! ダメージ計算と当たり判定処理
	virtual s32		calcDamageAndCollision();
	//! アニメーションによる移動不可チェック
	virtual bool	checkFreeze(const s32& currentAnimation);

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{
public:
	//! 取得
	COMMAND			getCommand()	const { return _command; }
	//! ゲームオーバーかどうか
	bool			getGameOver()	const { return _gameOver; }
	//! 城に近いかどうか
	bool			getNearCastle() const { return _isNearCastle; }
	//! 城に近いかどうかフラグ設定
	void			setNearCastle(bool flags) { _isNearCastle = flags; }

	//@}
private:

	//virtual void	Jump();				//!< ジャンプ
	//virtual void	Attack();			//!< 攻撃
	bool				_commandChangeMode;	//!< コマンドチェンジフラグ
	COMMAND				_command;			//!< 命令用

	//Collision			_myCollision;		//!< 衝突判定用
	Collision			_attackCollision;	//!< 攻撃判定用
	
	ANIMATION_STATE		_aniState;			//!< アニメーション用ステータス

	bool				_isNearCastle;		//!< 城に近いかどうかフラグ
	bool				_gameOver;			//!< ゲームオーバーフラグ
	
};

//=============================================================================
//	END OF FILE
//=============================================================================