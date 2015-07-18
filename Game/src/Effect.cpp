//-----------------------------------------------------------------------------
//!	
//!	@file	Effect.cpp
//!	@brief	ゲーム内エフェクト
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//=============================================================================
// パーティクル基底
//=============================================================================

//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
ParticleBase::ParticleBase()
: BillBoard (Vector3(0,0,0), Vector3(0,0,0), Vector3(0,0,0), Radian(0.0f), nullptr)
, _mov		(Vector3(0,0,0))
, _active	(false)
, _delay	(-1)
, _life		(0)
, _lifeMax	(0)
, _aniMax	(0)
, _aniCount	(0)
, _maskColor(Color::WHITE)
{
	BillBoard::init();
}
//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
ParticleBase::ParticleBase(Vector3		position,
						   Vector3		size,
						   Vector3		dispSize,
						   Radian		rotation,
						   Texture*		texture)
: BillBoard (position, size, dispSize, rotation, texture)
, _mov		(Vector3(0,0,0))
, _active	(false)
, _delay	(-1)
, _life		(0)
, _lifeMax	(0)
, _aniMax	(0)
, _aniCount (0)
, _maskColor(Color::WHITE)

{
	
}

//-----------------------------------------------------------------------------
//! パーティクル生成
//-----------------------------------------------------------------------------
void ParticleBase::generate()
{
	// アクティブ有効にする
	_active = true;
}

//-----------------------------------------------------------------------------
//! パーティクル生成(パラメータ設定)
//!	@param	[in]	position	座標
//!	@param	[in]	mov			移動量
//!	@param	[in]	scale		スケール値
//!	@param	[in]	rotation	角度
//!	@param	[in]	life		寿命
//!	@param	[in]	delay		遅延タイマー
//!	@param	[in]	angVel		角速度
//-----------------------------------------------------------------------------
void ParticleBase::generate(Vector3	position,
							Vector3	mov,
							Vector3	scale,
							Radian	rotation,
							f32		life,
							f32		delay,
							Radian	angVel)
{
	_position		 = position;
	_scale			 = scale;
	_rotation		 = rotation;
	_mov			 = mov;
	_delay			 = delay;
	_life			 = life;
	_lifeMax		 = life;
	_angularVelocity = angVel;
	// 遅延がなければ描画
	if( _delay == 0 ){
		generate();
	}
}



//=============================================================================
// パーティクル(煙)
//=============================================================================

//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
ParticleSmoke::ParticleSmoke()
{		
}
//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
ParticleSmoke::ParticleSmoke(Vector3		position,
						     Vector3		scale,
						     Radian			rotation,
						     Texture*		texture)
: ParticleBase(position, 
			   Vector3((f32)_texture->getWidth(),
					   (f32)_texture->getHeight(),
					    0.0f),
			   scale, rotation, texture)
{
}


//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool ParticleSmoke::init()
{
	_maskColor = Color(245, 222, 179);
	_size      = Vector3((f32)_texture->getWidth(), (f32)_texture->getHeight(), 0);
	return true;
}
//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void ParticleSmoke::update()
{
	// アクティブじゃないとき
	if( !_active ) {
		// 遅延を減らす
		if( --_delay == 0 ){
			generate();
		}
	}
	else	// アクティブじゃなければ処理しない
	{
		// 空気抵抗
		_mov	*= 0.98f;
		// 上昇
//		_mov._y += 0.001f;
		// デルタタイム
		//_mov	*= deltaTime;

		_rotation += _angularVelocity;
		
		// 移動
		_position += _mov;
		_maskColor._a = (u32)( LinearInterpolation(0, 255, _life, _lifeMax) );

		_life = ceil( _life - (1.0f * Global::deltaTime) );
		//if( --_life <= 0 ) _active = false;
		if( _life <= 0 ) _active = false;
	}
}

//-----------------------------------------------------------------------------
//! パーティクル描画
//-----------------------------------------------------------------------------
void ParticleSmoke::drawParticle()
{
	// アクティブかどうか
	if( !_active ) return;

	setBlendMode(BM_BLEND_ALPHA);

	draw(_maskColor);

	setBlendMode(BM_NONE);
}

//=============================================================================
// パーティクル(ヒット)
//=============================================================================

//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
ParticleHit::ParticleHit()
{		
	_aniMax = 10;
}
//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
ParticleHit::ParticleHit(Vector3		position,
						 Vector3		scale,
						 Radian			rotation,
						 Texture*		texture)
: ParticleBase(position, Vector3(256,256,0), scale, rotation, texture)
{
}


//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool ParticleHit::init()
{
	_size = Vector3(256,256,0);
	//_size      = Vector3(_texture->getWidth(), _texture->getHeight(), 0);
	return true;
}
//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void ParticleHit::update()
{
	// アクティブじゃないとき
	if( !_active ) {
		// 遅延を減らす
		if( --_delay == 0 ){
			generate();
		}
	}
	else	// アクティブじゃなければ処理しない
	{
		// デルタタイム
		//_mov	*= deltaTime;

		//_maskColor._a = (u32)( LinearInterpolation(0, 255, _life, _lifeMax) );
		
		_aniCount = (u32)( LinearInterpolation(0, (f32)_aniMax, _life, _lifeMax) );
		
		_life = ceil( _life - (1.0f * Global::deltaTime) );
		//if( --_life <= 0 ) _active = false;
		if( _life <= 0 ) _active = false;
	}
}

//-----------------------------------------------------------------------------
//! パーティクル描画
//-----------------------------------------------------------------------------
void ParticleHit::drawParticle()
{
	// アクティブかどうか
	if( !_active ) return;

	setBlendMode(BM_ADD_ALPHA);
	glDisable(GL_DEPTH_TEST);

	draw(_maskColor, _aniCount);

	glEnable(GL_DEPTH_TEST);
	setBlendMode(BM_NONE);
}

//=============================================================================
// パーティクル(ヒール)
//=============================================================================

//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
ParticleHeal::ParticleHeal()
{		
	_aniMax = 8;
}
//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
ParticleHeal::ParticleHeal(Vector3		position,
						   Vector3		scale,
						   Radian		rotation,
						   Texture*		texture)
: ParticleBase(position, Vector3(256,256,0), scale, rotation, texture)
{
}


//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool ParticleHeal::init()
{
	_size = Vector3(200,200,0);
	//_size      = Vector3(_texture->getWidth(), _texture->getHeight(), 0);
	return true;
}
//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void ParticleHeal::update()
{
	// アクティブじゃないとき
	if( !_active ) {
		// 遅延を減らす
		if( --_delay == 0 ){
			generate();
		}
	}
	else	// アクティブじゃなければ処理しない
	{
		_aniCount = (u32)( LinearInterpolation(0, (f32)_aniMax, _life, _lifeMax) );
				
		// 寿命が0ならアクティブフラグOFF
		_life = ceil( _life - (1.0f * Global::deltaTime) );
		//if( --_life <= 0 ) _active = false;
		if( _life <= 0 ) _active = false;
	}
}

//-----------------------------------------------------------------------------
//! パーティクル描画
//-----------------------------------------------------------------------------
void ParticleHeal::drawParticle()
{
	// アクティブかどうか
	if( !_active ) return;

	setBlendMode(BM_ADD_ALPHA);
	glDisable(GL_DEPTH_TEST);

	draw(_maskColor, _aniCount);

	glEnable(GL_DEPTH_TEST);
	setBlendMode(BM_NONE);
}





//=============================================================================
// パーティクル管理クラス実装
//=============================================================================

ParticleManager::ParticleManager()
: _particleCount(0)
, _deleteCount  (0)
{
	// テクスチャのメモリ確保
	for( s32 i=0; i<PARTICLE_MAX; ++i )	{
		//_effectTexture[i] = new Texture();
		_effectTexture[i] = nullptr;
		SAFE_NEW(_effectTexture[i]);
	}
	// 読み込みと転送
	_effectTexture[PARTICLE_SMOKE]->loadAndTexImage2D("../Image/Effects/smoke.png", true);
	_effectTexture[PARTICLE_HIT]->loadAndTexImage2D("../Image/Effects/hit.png", true);
	_effectTexture[PARTICLE_HEAL]->loadAndTexImage2D("../Image/Effects/cure.png", true);

	/*for( s32 i=0; i<EFFECT_MAX; ++i )
	{
		_effectList[i] = nullptr;
		SAFE_NEW(_effectList[i]);
	}*/
	/*glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexContext) * EFFECT_MAX * 4, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
ParticleManager::~ParticleManager()
{
	for(s32 i=0; i<_particleCount; ++i){
		SAFE_DELETE(_effectList[i]);
	}
	while( _deleteCount >= 1 )
	{
		SAFE_DELETE(_deleteList[--_deleteCount]);
	}
	for( s32 i=0; i<PARTICLE_MAX; ++i )	{
		SAFE_DELETE(_effectTexture[i]);
	}
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool ParticleManager::init()
{
	
	return true;
}

//-----------------------------------------------------------------------------
//! 更新
//-----------------------------------------------------------------------------
void ParticleManager::update()
{
	
	for( s32 i=0; i<_particleCount; i++ ) {
		ParticleBase* p =_effectList[i];

		// アクティブじゃなければ
		if( !p->isActive() ) {
			// 出現中の数を減らす
			_particleCount--;

			// 削除リストに入れる
			_deleteList[_deleteCount] = p;
			_deleteCount++;

			// 末尾に移動
			//p = _effectList[_particleCount];
			_effectList[i] = _effectList[_particleCount];
			_effectList[_particleCount] = nullptr;
			i--;
			continue;
		}

		p->update();
	}
	
	while( _deleteCount >= 1 )
	{
		SAFE_DELETE(_deleteList[--_deleteCount]);
	}
	
}

//---- Zソートのクイックソート用コールバック関数
int zSortFunc(const void* p1, const void*p2)
{
	ParticleBase* pParticle1 = (ParticleBase*)p1;
	ParticleBase* pParticle2 = (ParticleBase*)p2;

	Vector3		  particlePos1 = pParticle1->getPosition();
	Vector3		  particlePos2 = pParticle2->getPosition();

	if( particlePos1._z == particlePos2._z) return 0;
	return (particlePos1._z < particlePos2._z) ? -1 : +1;
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void ParticleManager::render()
{
	/*glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	VertexContext* p = (VertexContext*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	
	
	Float2 uv[] = {
		Float2(0.0f,0.0f),
		Float2(1.0f,0.0f),
		Float2(0.0f,1.0f),
		Float2(1.0f,1.0f),
	};


	Vector3 pos[4];
	u32	bufferCount = 0;
	for( u32 i=0; i<EFFECT_MAX; ++i ) {
		if( _effectList[i]->isActive() )
		{
			_effectList[i]->calcBillBoardPosition(pos);
			for( u32 v=0; v<4; ++v ) {
				p[bufferCount]._pos   = pos[v];
				p[bufferCount]._color = _effectList[i]->getColor();
				p[bufferCount]._uv    = uv[i];
				bufferCount++;
			}
		}
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, _particleCount * 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
*/

	glDisable(GL_ALPHA_TEST);

	Vector3 viewPos		= IObjDataManager()->_pPlayer->getPosition();
	Vector3 particlePos = Vector3(0.0f);
	for( s32 i=0; i<_particleCount; ++i ) {
		particlePos = _effectList[i]->getPosition();
		if( (viewPos - particlePos).length() > 2000.0f ) continue;
		_effectList[i]->drawParticle();
	}
	glEnable(GL_ALPHA_TEST);
}


//-----------------------------------------------------------------------------
//! 生成
//!	@param	[in]	position	座標
//!	@param	[in]	mov			移動量
//!	@param	[in]	scale		スケール値
//!	@param	[in]	rotation	角度
//!	@param	[in]	life		寿命
//!	@param	[in]	delay		遅延タイマー
//!	@param	[in]	angVel		角速度
//-----------------------------------------------------------------------------
void ParticleManager::generateParticle(Vector3				position,
									   Vector3				mov,
 									   Vector3				scale,
									   Radian				rotation,
									   f32					life,
									   f32					delay,
									   Radian				angVel,
									   PARTICLE_TYPE		type)
{

	// エフェクトを生成
	ParticleBase* generateEffect = nullptr;

	if( _particleCount < EFFECT_MAX ){
		//generateEffect = _effectList[_particleCount];

		switch(type)
		{
		case PARTICLE_SMOKE:
			generateEffect = new ParticleSmoke();
			break;
		case PARTICLE_HIT:
			generateEffect = new ParticleHit();
			break;
		case PARTICLE_HEAL:
			generateEffect = new ParticleHeal();
			break;
		}

		_effectList[_particleCount] = generateEffect;
		// テクスチャ設定
		generateEffect->setTexture(_effectTexture[type]);
		// 初期化
		generateEffect->init();
		// エフェクト発生
		generateEffect->generate(position, mov, scale, rotation, life, delay, angVel);
		_particleCount++;

		// 描画する前にZソートする
//		qsort(&_effectList[0], _particleCount, sizeof(ParticleBase), &zSortFunc);
	}
	
}


//=============================================================================
// END OF FILE
//=============================================================================