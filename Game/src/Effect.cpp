//-----------------------------------------------------------------------------
//!	
//!	@file	Effect.cpp
//!	@brief	�Q�[�����G�t�F�N�g
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//=============================================================================
// �p�[�e�B�N�����
//=============================================================================

//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
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
//! �f�t�H���g�R���X�g���N�^
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
//! �p�[�e�B�N������
//-----------------------------------------------------------------------------
void ParticleBase::generate()
{
	// �A�N�e�B�u�L���ɂ���
	_active = true;
}

//-----------------------------------------------------------------------------
//! �p�[�e�B�N������(�p�����[�^�ݒ�)
//!	@param	[in]	position	���W
//!	@param	[in]	mov			�ړ���
//!	@param	[in]	scale		�X�P�[���l
//!	@param	[in]	rotation	�p�x
//!	@param	[in]	life		����
//!	@param	[in]	delay		�x���^�C�}�[
//!	@param	[in]	angVel		�p���x
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
	// �x�����Ȃ���Ε`��
	if( _delay == 0 ){
		generate();
	}
}



//=============================================================================
// �p�[�e�B�N��(��)
//=============================================================================

//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
ParticleSmoke::ParticleSmoke()
{		
}
//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
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
//! ������
//-----------------------------------------------------------------------------
bool ParticleSmoke::init()
{
	_maskColor = Color(245, 222, 179);
	_size      = Vector3((f32)_texture->getWidth(), (f32)_texture->getHeight(), 0);
	return true;
}
//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void ParticleSmoke::update()
{
	// �A�N�e�B�u����Ȃ��Ƃ�
	if( !_active ) {
		// �x�������炷
		if( --_delay == 0 ){
			generate();
		}
	}
	else	// �A�N�e�B�u����Ȃ���Ώ������Ȃ�
	{
		// ��C��R
		_mov	*= 0.98f;
		// �㏸
//		_mov._y += 0.001f;
		// �f���^�^�C��
		//_mov	*= deltaTime;

		_rotation += _angularVelocity;
		
		// �ړ�
		_position += _mov;
		_maskColor._a = (u32)( LinearInterpolation(0, 255, _life, _lifeMax) );

		_life = ceil( _life - (1.0f * Global::deltaTime) );
		//if( --_life <= 0 ) _active = false;
		if( _life <= 0 ) _active = false;
	}
}

//-----------------------------------------------------------------------------
//! �p�[�e�B�N���`��
//-----------------------------------------------------------------------------
void ParticleSmoke::drawParticle()
{
	// �A�N�e�B�u���ǂ���
	if( !_active ) return;

	setBlendMode(BM_BLEND_ALPHA);

	draw(_maskColor);

	setBlendMode(BM_NONE);
}

//=============================================================================
// �p�[�e�B�N��(�q�b�g)
//=============================================================================

//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
ParticleHit::ParticleHit()
{		
	_aniMax = 10;
}
//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
ParticleHit::ParticleHit(Vector3		position,
						 Vector3		scale,
						 Radian			rotation,
						 Texture*		texture)
: ParticleBase(position, Vector3(256,256,0), scale, rotation, texture)
{
}


//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool ParticleHit::init()
{
	_size = Vector3(256,256,0);
	//_size      = Vector3(_texture->getWidth(), _texture->getHeight(), 0);
	return true;
}
//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void ParticleHit::update()
{
	// �A�N�e�B�u����Ȃ��Ƃ�
	if( !_active ) {
		// �x�������炷
		if( --_delay == 0 ){
			generate();
		}
	}
	else	// �A�N�e�B�u����Ȃ���Ώ������Ȃ�
	{
		// �f���^�^�C��
		//_mov	*= deltaTime;

		//_maskColor._a = (u32)( LinearInterpolation(0, 255, _life, _lifeMax) );
		
		_aniCount = (u32)( LinearInterpolation(0, (f32)_aniMax, _life, _lifeMax) );
		
		_life = ceil( _life - (1.0f * Global::deltaTime) );
		//if( --_life <= 0 ) _active = false;
		if( _life <= 0 ) _active = false;
	}
}

//-----------------------------------------------------------------------------
//! �p�[�e�B�N���`��
//-----------------------------------------------------------------------------
void ParticleHit::drawParticle()
{
	// �A�N�e�B�u���ǂ���
	if( !_active ) return;

	setBlendMode(BM_ADD_ALPHA);
	glDisable(GL_DEPTH_TEST);

	draw(_maskColor, _aniCount);

	glEnable(GL_DEPTH_TEST);
	setBlendMode(BM_NONE);
}

//=============================================================================
// �p�[�e�B�N��(�q�[��)
//=============================================================================

//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
ParticleHeal::ParticleHeal()
{		
	_aniMax = 8;
}
//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
ParticleHeal::ParticleHeal(Vector3		position,
						   Vector3		scale,
						   Radian		rotation,
						   Texture*		texture)
: ParticleBase(position, Vector3(256,256,0), scale, rotation, texture)
{
}


//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool ParticleHeal::init()
{
	_size = Vector3(200,200,0);
	//_size      = Vector3(_texture->getWidth(), _texture->getHeight(), 0);
	return true;
}
//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void ParticleHeal::update()
{
	// �A�N�e�B�u����Ȃ��Ƃ�
	if( !_active ) {
		// �x�������炷
		if( --_delay == 0 ){
			generate();
		}
	}
	else	// �A�N�e�B�u����Ȃ���Ώ������Ȃ�
	{
		_aniCount = (u32)( LinearInterpolation(0, (f32)_aniMax, _life, _lifeMax) );
				
		// ������0�Ȃ�A�N�e�B�u�t���OOFF
		_life = ceil( _life - (1.0f * Global::deltaTime) );
		//if( --_life <= 0 ) _active = false;
		if( _life <= 0 ) _active = false;
	}
}

//-----------------------------------------------------------------------------
//! �p�[�e�B�N���`��
//-----------------------------------------------------------------------------
void ParticleHeal::drawParticle()
{
	// �A�N�e�B�u���ǂ���
	if( !_active ) return;

	setBlendMode(BM_ADD_ALPHA);
	glDisable(GL_DEPTH_TEST);

	draw(_maskColor, _aniCount);

	glEnable(GL_DEPTH_TEST);
	setBlendMode(BM_NONE);
}





//=============================================================================
// �p�[�e�B�N���Ǘ��N���X����
//=============================================================================

ParticleManager::ParticleManager()
: _particleCount(0)
, _deleteCount  (0)
{
	// �e�N�X�`���̃������m��
	for( s32 i=0; i<PARTICLE_MAX; ++i )	{
		//_effectTexture[i] = new Texture();
		_effectTexture[i] = nullptr;
		SAFE_NEW(_effectTexture[i]);
	}
	// �ǂݍ��݂Ɠ]��
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
//! �f�X�g���N�^
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
//! ������
//-----------------------------------------------------------------------------
bool ParticleManager::init()
{
	
	return true;
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void ParticleManager::update()
{
	
	for( s32 i=0; i<_particleCount; i++ ) {
		ParticleBase* p =_effectList[i];

		// �A�N�e�B�u����Ȃ����
		if( !p->isActive() ) {
			// �o�����̐������炷
			_particleCount--;

			// �폜���X�g�ɓ����
			_deleteList[_deleteCount] = p;
			_deleteCount++;

			// �����Ɉړ�
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

//---- Z�\�[�g�̃N�C�b�N�\�[�g�p�R�[���o�b�N�֐�
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
//! �`��
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
//! ����
//!	@param	[in]	position	���W
//!	@param	[in]	mov			�ړ���
//!	@param	[in]	scale		�X�P�[���l
//!	@param	[in]	rotation	�p�x
//!	@param	[in]	life		����
//!	@param	[in]	delay		�x���^�C�}�[
//!	@param	[in]	angVel		�p���x
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

	// �G�t�F�N�g�𐶐�
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
		// �e�N�X�`���ݒ�
		generateEffect->setTexture(_effectTexture[type]);
		// ������
		generateEffect->init();
		// �G�t�F�N�g����
		generateEffect->generate(position, mov, scale, rotation, life, delay, angVel);
		_particleCount++;

		// �`�悷��O��Z�\�[�g����
//		qsort(&_effectList[0], _particleCount, sizeof(ParticleBase), &zSortFunc);
	}
	
}


//=============================================================================
// END OF FILE
//=============================================================================