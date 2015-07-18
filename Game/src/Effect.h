//-----------------------------------------------------------------------------
//!	
//!	@file	Effect.h
//!	@brief	ゲーム内エフェクト
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
#pragma message(" + Effect.h")

//-----------------------------------------------------------------------------
//! パーティクル基底
//-----------------------------------------------------------------------------
class ParticleBase : public BillBoard
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	ParticleBase();

	//! コンストラクタ
	ParticleBase(Vector3		position,
				 Vector3		size,
				 Vector3		scale,
				 Radian			rotation,
				 Texture*		texture);

	//! デストラクタ
	~ParticleBase(){}

	//! 初期化
	virtual bool	init() = 0;

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! パーティクル生成
	void	generate();

	//! パーティクル生成(パラメータ設定)
	//!	@param	[in]	position	座標
	//!	@param	[in]	mov			移動量
	//!	@param	[in]	scale		スケール値
	//!	@param	[in]	rotation	角度
	//!	@param	[in]	life		寿命
	//!	@param	[in]	delay		遅延タイマー
	//!	@param	[in]	angVel		角速度
	void	generate(Vector3		position,
					 Vector3		mov,
					 Vector3		size,
					 Radian			rotation,
					 f32			life,
					 f32			delay,
					 Radian			angVel);

	//! 更新
	virtual void	update() = 0;

	//! パーティクル描画
	virtual void	drawParticle() = 0;

	//@}

	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! 寿命と遅延タイマーの設定
	void		setLifeAndDelay(f32 life, f32	delayTimer=0){
		_life  = life;
		_delay = delayTimer;
	}

	//! 角速度設定
	void		setAngularVelocity(Radian angVel)
	{
		_angularVelocity = angVel;
	}

	//! アクティブかどうか
	bool		isActive() const { return _active; }

//	Vector4		getColor() const { return _maskColor; }

	//@}

protected:
	Vector3		_mov;				//!< 移動量
	bool		_active;			//!< アクティブフラグ
	f32			_delay;				//!< 遅延
	f32			_life;				//!< 寿命
	f32			_lifeMax;			//!< 寿命最大値
	s32			_aniMax;			//!< ア二メーション最大
	s32			_aniCount;			//!< アニメーションカウント
	Radian		_angularVelocity;	//!< 角速度
	Color		_maskColor;			//!< マスクの色
};

//-----------------------------------------------------------------------------
//! パーティクルエフェクト(煙)
//-----------------------------------------------------------------------------
class ParticleSmoke : public ParticleBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	ParticleSmoke();

	//! コンストラクタ
	ParticleSmoke(Vector3		position,
				  Vector3		scale,
				  Radian		rotation,
				  Texture*		texture);

	//! デストラクタ
	~ParticleSmoke(){}

	//! 初期化
	virtual bool	init();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{
	
	//! 更新
	virtual void	update();

	//! パーティクル描画
	virtual void	drawParticle();

	//@}
};


//-----------------------------------------------------------------------------
//! パーティクルエフェクト(ヒット)
//-----------------------------------------------------------------------------
class ParticleHit : public ParticleBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	ParticleHit();

	//! コンストラクタ
	ParticleHit(Vector3		position,
				Vector3		scale,
				Radian		rotation,
				Texture*	texture);

	//! デストラクタ
	~ParticleHit(){}

	//! 初期化
	virtual bool	init();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{
	
	//! 更新
	virtual void	update();

	//! パーティクル描画
	virtual void	drawParticle();

	//@}
};


//-----------------------------------------------------------------------------
//! パーティクルエフェクト(ヒール)
//-----------------------------------------------------------------------------
class ParticleHeal : public ParticleBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	ParticleHeal();

	//! コンストラクタ
	ParticleHeal(Vector3	position,
				 Vector3	scale,
				 Radian		rotation,
				 Texture*	texture);

	//! デストラクタ
	~ParticleHeal(){}

	//! 初期化
	virtual bool	init();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{
	
	//! 更新
	virtual void	update();

	//! パーティクル描画
	virtual void	drawParticle();

	//@}
};




//-----------------------------------------------------------------------------
//! パーティクルエフェクト管理クラス
//-----------------------------------------------------------------------------
class ParticleManager
{
public:

	//! @name	パーティクルの種類
	//@{
	enum	PARTICLE_TYPE
	{
		PARTICLE_SMOKE,
		PARTICLE_HIT,
		PARTICLE_HEAL,
		PARTICLE_MAX
	};
	//@}

private:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	ParticleManager();
	//! コピーコンストラクタ
	ParticleManager(ParticleManager& src){}
	//! デストラクタ
	virtual ~ParticleManager();

public:
	//! 初期化
	bool		init();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 更新
	void		update();

	//! 描画
	void		render();

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! インスタンス取得
	static ParticleManager*	getInstance()
	{
		static ParticleManager	_myInstance;
		return &_myInstance;
	}

		
	//@}
	//-------------------------------------------------------------
	//! @name 追加・削除
	//-------------------------------------------------------------
	//@{

	//! 生成
	//!	@param	[in]	position	座標
	//!	@param	[in]	mov			移動量
	//!	@param	[in]	scale		スケール値
	//!	@param	[in]	rotation	角度
	//!	@param	[in]	life		寿命
	//!	@param	[in]	delay		遅延タイマー
	//!	@param	[in]	angVel		角速度
	void		generateParticle(Vector3			position,
								 Vector3			mov,
								 Vector3			scale,
								 Radian				rotation,
								 f32				life,
								 f32				delay,
								 Radian				angVel,
								 PARTICLE_TYPE		type);
	

	//@}

private:
	static const s32			EFFECT_MAX = 10000;

	ParticleBase*				_effectList[EFFECT_MAX];
	ParticleBase*				_deleteList[EFFECT_MAX];
	Texture*					_effectTexture[PARTICLE_MAX];	//!< テクスチャ
	s32							_particleCount;
	s32							_deleteCount;
	GLuint						_vbo;
};

//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
ParticleManager*	IParticleMan()
{
	return ParticleManager::getInstance();
}



//=============================================================================
// END OF FILE
//=============================================================================