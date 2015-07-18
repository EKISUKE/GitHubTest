//-----------------------------------------------------------------------------
//!
//!	@file	Man.h
//!	@brief	ゲーム内キャラ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!	ゲーム内キャラ
//-----------------------------------------------------------------------------
class	Man : public ObjBase
{
public:

	enum TYPE 
	{
		TYPE_PLAYER,
		TYPE_ALLY,
		TYPE_ENEMY, 
	};

	enum KEY_TYPE
	{
		KEY_INPUT,
		KEY_ATTACKER,
		KEY_DEFFENDER,	
		KEY_HEALLER,	
		KEY_FEINTER,
		KEY_LEADER,
		KEY_MAX
	};
	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

private:
	//! デフォルトコンストラクタ
	Man();
public:

	//! コンストラクタ
	Man(KEY_TYPE myType, TYPE manType);

	//! デストラクタ
	virtual ~Man();

	//!	初期化
	virtual bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	virtual void	Update();
	//!	描画
	virtual void	Render(bool	isShadow);
	//! glut描画
	virtual void	glutRender() = 0;
	//! デバッグ描画
	virtual void	debugRender() = 0;

	//! ステータスの描画(2D)
	virtual void	StatusRender() = 0;

	//! HPアウトラインのテクスチャ読み込み
	bool			loadHPOutLineTexture(GM_CSTR fileName);

	//! モデルの読み込み
	bool			loadModel(GM_CSTR fileName, f32 scale = 1.0f);
	//! モデル読み込みされたかどうか
	bool			isModelLoaded();

	//! キーの作成
	bool			createKey(Man* me, Man* leader);
	
	//! ステータスの作成
	bool			createStatus();

	//@}
	//-------------------------------------------------------------
	//!	@name 取得・設定
	//-------------------------------------------------------------
	//@{

	//---- セット
	void			setHitObj( bool flag )	  { _hitObj    = flag; }
	void			setHitAttack( bool flag ) { _hitAttack = flag; }
	void			setPosition(Vector3& pos) { _position  = pos;  }
	//---- 取得
	bool			isDead				 () const { return _isDead; }
	bool			getHitObj			 () const { return _hitObj;	}
	bool			getHitAttack		 () const { return _hitAttack; }
	bool			getisAttack			 () const { return _isAttack;	}
	TYPE			getType				 () const { return _type;		}
	Status*			getStatus			 () const { return _myStatus;	}
	//! タイプ取得					    
	KEY_TYPE		getKeyType			 () const { return _keyType; }
	//Vector3			getHitPosition	 	  () const { return _hitPosition;	 }
	Vector3			getAttackPosition	 () const { return _attackPosition; }
										 
	//f32				getRadius		     () const { return _hitRadius;		}
	f32				getAttackRadius		 () const { return _attackRadius;	}
										 
	Sphere			getAttackSphere		 () const { return _attackSphere; };
										 
	//! 当たり判定用AABB情報取得 		   
	AABB			getColAABB			 () const { return _colAABB; }
	//! 当たり判定用AABB情報取得 		   
	AABB			getAttackAABB		 () const { return _attackAABB; }

	//! 距離取得
	f32		getLengthtoTarget (Man* target) const {
		Vector3 targetPos = target->getPosition();
		return (targetPos - _position).length();
	}

	//@}
	//-------------------------------------------------------------
	//!	@name	座標変換関数
	//-------------------------------------------------------------
	//@{

	//! 当たり判定用の座標にする
	//!	@param	[in]　position		座標
	//!	@param	[in]　hitPosition	当たり判定用の座標保存用
	void	toHitPosition(Vector3 position, Vector3& hitPosition)
	{
		position._y += _hitRadius;
		hitPosition  = position;
	}

	//! 座標にする
	//!	@param	[in]　hitPosition	当たり判定用の座標
	//!	@param	[in]　position		座標保存用
	void	toPosition(Vector3 hitPosition, Vector3& position)
	{
		hitPosition._y -= _hitRadius;
		position		= hitPosition;
	}

	//@}
protected:
	virtual void		Jump();						//!< ジャンプ
	virtual void		Attack();					//!< 攻撃
	virtual void		Damage(const s32& damage);	//!< ダメージ
		

	//! 座標設定しなおし
	virtual void		ResetPosition();

	//! 狙う相手を探して設定
	//!	@return true:敵を設定完了 false:狙う敵はいない
	virtual bool		searchAndSetTarget(){return true;}

	//! ダメージ計算
	virtual s32			calcDamageAndCollision(){return 0;}

	//! アニメーションによる移動不可チェック
	virtual bool		checkFreeze(const s32& currentAnimation){return false;}

	

	bool				_hitGround;					//!< 接地フラグ
	bool				_hitCastle;					//!< 城、城壁との衝突フラグ
	bool				_hitObj;					//!< オブジェクトとのヒットフラグ
	bool				_hitAttack;					//!< オブジェクトとに攻撃されたかどうか
	bool				_isAttack;					//!< 攻撃フラグ
	bool				_isJump;					//!< ジャンプフラグ
	bool				_isDamage;					//!< ダメージフラグ
	bool				_isDead;					//!< 死亡フラグ

	//---- 基本情報
	Vector3				_attackPosition;			//!< 攻撃用
	f32					_speed;						//!< 移動速度
	//---- 攻撃情報
	f32					_attackRadius;				//!< 攻撃の半径
	f32					_attackRadNum;				//!< 攻撃の半径の初期値
	f32					_attackOffsetRange;			//!< 攻撃の距離
	f32					_waitGenTime;				//!< パーティクルの生成待ち時間
	f32					_genCount;					//!< 生成待ちカウント
	Status*				_myStatus;					//!< ステータス
	AssetModelX*		_pAssetModel;				//!< モデルデータ
	TaskModelX*			_pTaskModel;				//!< モデル変更用

	TYPE				_type;						//!< 自分の種類
	KEY_TYPE			_keyType;					//!< キーの種類
	KeyBase*			_key;						//!< 自分の操作キー

	Sphere				_attackSphere;				//!< 攻撃用の球

	AABB				_colAABB;					//!< 当たり判定用AABB
	AABB				_attackAABB;				//!< 攻撃衝突判定用AABB

	f32					_height;					//!< 高さ

	Collision			_myCollision;				//!< 当たり判定用

	s32					_myLandScapleGridNum;		//!< ランドスケープのグリッド番号
	
	Texture*			_hpOutLine;			//!< HPゲージのアウトライン

};


//-----------------------------------------------------------------------------
//!	キャラ管理クラス
//-----------------------------------------------------------------------------
class ManManager : public Singleton<ManManager>
{
	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//!	デフォルトコンストラクタ
	ManManager()
	: _nowIndex(0)
	{
		// 全初期化
		for( s32 i=0; i<LIST_MAX; ++i )
		{
			_AttackListForPlayer[i] = NULL;
			_AttackListForEnemy[i] = NULL;
		}
	}
	//!	デストラクタ
	virtual ~ManManager();

	//@}
	//-------------------------------------------------------------
	//!	@name 登録・解除
	//-------------------------------------------------------------
	//@{

	//! 登録
	void	Regist(Man*	regist);
	//! 解除
	void	UnRegist(Man* regist);

	//@}

	//! 更新
	void	Update();
private:
	static const s32 LIST_MAX = 50;
	Man*		_AttackListForPlayer[LIST_MAX];	//!< プレイヤーのための攻撃リスト
	Man*		_AttackListForEnemy[LIST_MAX];	//!< 敵のための攻撃リスト
	s32			_nowIndex;
};

//=============================================================================
//	END OF FILE
//=============================================================================