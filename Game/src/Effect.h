//-----------------------------------------------------------------------------
//!	
//!	@file	Effect.h
//!	@brief	�Q�[�����G�t�F�N�g
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
#pragma message(" + Effect.h")

//-----------------------------------------------------------------------------
//! �p�[�e�B�N�����
//-----------------------------------------------------------------------------
class ParticleBase : public BillBoard
{
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	ParticleBase();

	//! �R���X�g���N�^
	ParticleBase(Vector3		position,
				 Vector3		size,
				 Vector3		scale,
				 Radian			rotation,
				 Texture*		texture);

	//! �f�X�g���N�^
	~ParticleBase(){}

	//! ������
	virtual bool	init() = 0;

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �p�[�e�B�N������
	void	generate();

	//! �p�[�e�B�N������(�p�����[�^�ݒ�)
	//!	@param	[in]	position	���W
	//!	@param	[in]	mov			�ړ���
	//!	@param	[in]	scale		�X�P�[���l
	//!	@param	[in]	rotation	�p�x
	//!	@param	[in]	life		����
	//!	@param	[in]	delay		�x���^�C�}�[
	//!	@param	[in]	angVel		�p���x
	void	generate(Vector3		position,
					 Vector3		mov,
					 Vector3		size,
					 Radian			rotation,
					 f32			life,
					 f32			delay,
					 Radian			angVel);

	//! �X�V
	virtual void	update() = 0;

	//! �p�[�e�B�N���`��
	virtual void	drawParticle() = 0;

	//@}

	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �����ƒx���^�C�}�[�̐ݒ�
	void		setLifeAndDelay(f32 life, f32	delayTimer=0){
		_life  = life;
		_delay = delayTimer;
	}

	//! �p���x�ݒ�
	void		setAngularVelocity(Radian angVel)
	{
		_angularVelocity = angVel;
	}

	//! �A�N�e�B�u���ǂ���
	bool		isActive() const { return _active; }

//	Vector4		getColor() const { return _maskColor; }

	//@}

protected:
	Vector3		_mov;				//!< �ړ���
	bool		_active;			//!< �A�N�e�B�u�t���O
	f32			_delay;				//!< �x��
	f32			_life;				//!< ����
	f32			_lifeMax;			//!< �����ő�l
	s32			_aniMax;			//!< �A�񃁁[�V�����ő�
	s32			_aniCount;			//!< �A�j���[�V�����J�E���g
	Radian		_angularVelocity;	//!< �p���x
	Color		_maskColor;			//!< �}�X�N�̐F
};

//-----------------------------------------------------------------------------
//! �p�[�e�B�N���G�t�F�N�g(��)
//-----------------------------------------------------------------------------
class ParticleSmoke : public ParticleBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	ParticleSmoke();

	//! �R���X�g���N�^
	ParticleSmoke(Vector3		position,
				  Vector3		scale,
				  Radian		rotation,
				  Texture*		texture);

	//! �f�X�g���N�^
	~ParticleSmoke(){}

	//! ������
	virtual bool	init();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{
	
	//! �X�V
	virtual void	update();

	//! �p�[�e�B�N���`��
	virtual void	drawParticle();

	//@}
};


//-----------------------------------------------------------------------------
//! �p�[�e�B�N���G�t�F�N�g(�q�b�g)
//-----------------------------------------------------------------------------
class ParticleHit : public ParticleBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	ParticleHit();

	//! �R���X�g���N�^
	ParticleHit(Vector3		position,
				Vector3		scale,
				Radian		rotation,
				Texture*	texture);

	//! �f�X�g���N�^
	~ParticleHit(){}

	//! ������
	virtual bool	init();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{
	
	//! �X�V
	virtual void	update();

	//! �p�[�e�B�N���`��
	virtual void	drawParticle();

	//@}
};


//-----------------------------------------------------------------------------
//! �p�[�e�B�N���G�t�F�N�g(�q�[��)
//-----------------------------------------------------------------------------
class ParticleHeal : public ParticleBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	ParticleHeal();

	//! �R���X�g���N�^
	ParticleHeal(Vector3	position,
				 Vector3	scale,
				 Radian		rotation,
				 Texture*	texture);

	//! �f�X�g���N�^
	~ParticleHeal(){}

	//! ������
	virtual bool	init();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{
	
	//! �X�V
	virtual void	update();

	//! �p�[�e�B�N���`��
	virtual void	drawParticle();

	//@}
};




//-----------------------------------------------------------------------------
//! �p�[�e�B�N���G�t�F�N�g�Ǘ��N���X
//-----------------------------------------------------------------------------
class ParticleManager
{
public:

	//! @name	�p�[�e�B�N���̎��
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
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	ParticleManager();
	//! �R�s�[�R���X�g���N�^
	ParticleManager(ParticleManager& src){}
	//! �f�X�g���N�^
	virtual ~ParticleManager();

public:
	//! ������
	bool		init();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	void		update();

	//! �`��
	void		render();

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�擾
	static ParticleManager*	getInstance()
	{
		static ParticleManager	_myInstance;
		return &_myInstance;
	}

		
	//@}
	//-------------------------------------------------------------
	//! @name �ǉ��E�폜
	//-------------------------------------------------------------
	//@{

	//! ����
	//!	@param	[in]	position	���W
	//!	@param	[in]	mov			�ړ���
	//!	@param	[in]	scale		�X�P�[���l
	//!	@param	[in]	rotation	�p�x
	//!	@param	[in]	life		����
	//!	@param	[in]	delay		�x���^�C�}�[
	//!	@param	[in]	angVel		�p���x
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
	Texture*					_effectTexture[PARTICLE_MAX];	//!< �e�N�X�`��
	s32							_particleCount;
	s32							_deleteCount;
	GLuint						_vbo;
};

//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
ParticleManager*	IParticleMan()
{
	return ParticleManager::getInstance();
}



//=============================================================================
// END OF FILE
//=============================================================================