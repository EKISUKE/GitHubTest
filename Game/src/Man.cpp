//-----------------------------------------------------------------------------
//!
//!	@file	Man.cpp
//!	@brief	�Q�[�����L����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"




//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
Man::Man()
: _speed				(0.0f)
, _attackPosition		(0.0f, 0.0f, 0.0f)
, _attackRadius			(0.0f)
, _waitGenTime			(15.0f)
, _genCount				(0.0f)
, _attackOffsetRange	(20.0f)
, _hitGround			(false)
, _hitCastle			(false)
, _hitObj				(false)
, _hitAttack			(false)
, _isAttack				(false)
, _isJump				(false)
, _isDamage				(false)
, _pAssetModel			(nullptr)
, _pTaskModel			(nullptr)
, _hpOutLine			(nullptr)
, _key					(nullptr)
, _myStatus				(nullptr)
, _myLandScapleGridNum	(0)
{
}

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
Man::Man(KEY_TYPE myType, TYPE manType)
: _speed				(0.0f)
, _attackPosition		(0.0f, 0.0f, 0.0f)
, _attackRadius			(0.0f)
, _waitGenTime			(15.0f)
, _genCount				(0.0f)
, _attackOffsetRange	(20.0f)
, _hitGround			(false)
, _hitCastle			(false)
, _hitObj				(false)
, _hitAttack			(false)
, _isAttack				(false)
, _isJump				(false)
, _isDamage				(false)
, _pAssetModel			(nullptr)
, _pTaskModel			(nullptr)
, _hpOutLine			(nullptr)
, _key					(nullptr)
, _myStatus				(nullptr)
, _myLandScapleGridNum	(0)
, _type					(manType)
, _keyType				(myType)
{
	createStatus();
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
Man::~Man() 
{
	SAFE_DELETE(_pAssetModel);
	SAFE_DELETE(_pTaskModel);
	SAFE_DELETE(_hpOutLine);
	SAFE_DELETE(_key);
	SAFE_DELETE(_myStatus);
};

//-----------------------------------------------------------------------------
//! HP�A�E�g���C���̃e�N�X�`���ǂݍ���
//-----------------------------------------------------------------------------
bool Man::loadHPOutLineTexture(GM_CSTR fileName)
{
	// ���łɓǂݍ��܂�Ă�����
	if( _hpOutLine != nullptr ){
		return true;
	}

	SAFE_NEW(_hpOutLine);
	//! �ǂݍ���
	if( !_hpOutLine->loadAndTexImage2D(fileName) )
	{
		MessageBox(NULL, (LPCWSTR)fileName, L"HP�Q�[�W�̓ǂݍ��ݎ��s",MB_OK); 
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
//! ���f���̓ǂݍ���
//-----------------------------------------------------------------------------
bool Man::loadModel(GM_CSTR fileName, f32 scale)
{
	// ���f�������łɓǂݍ��܂�Ă��邩�ǂ���
	if( isModelLoaded() )
	{
		return true;
	}
	
	// ���f��
	SAFE_NEW(_pAssetModel);
	// �������m�ۂł��Ă��邩�ǂ���
	if( _pAssetModel == nullptr )
	{
		return false;
	}

	// �ǂݍ���
	if( !_pAssetModel->load(fileName) )
	{
		MessageBox(NULL, (LPCWSTR)fileName, L"���f���̓ǂݍ��݂Ɏ��s���܂���",MB_OK); 
		return false;
	}

	SAFE_NEW(_pTaskModel);
	_pTaskModel->setModel(_pAssetModel);
	_pTaskModel->setScale(scale);
	_pTaskModel->init();

	// �����蔻��pAABB�쐬
	f32 minX = _pAssetModel->getMinX();
	f32 maxX = _pAssetModel->getMaxX();
	f32 minY = _pAssetModel->getMinY();
	f32 maxY = _pAssetModel->getMaxY();
	f32 minZ = _pAssetModel->getMinZ();
	f32 maxZ = _pAssetModel->getMaxZ();
	Vector3 maxVec(maxX, maxY, maxZ);
	Vector3 minVec(minX, minY, minZ);
	_colAABB.setEmpty();
	_colAABB.expand(maxVec);
	_colAABB.expand(minVec);

	_radius = (_colAABB._max - _colAABB._min).length() * 0.5f;

	// �����蔻��p���a�擾
	_hitRadius = _radius;


	return true;
}
//-----------------------------------------------------------------------------
//! ���f���ǂݍ��݂��ꂽ���ǂ���
//-----------------------------------------------------------------------------
bool Man::isModelLoaded()
{
	if( _pAssetModel != nullptr )
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//! �L�[�̍쐬
//-----------------------------------------------------------------------------
bool Man::createKey(Man* me, Man* leader)
{

	// ���łɍ쐬����Ă����珈�����Ȃ�
	if( _key != nullptr ) return true;

	switch( _keyType )
	{
		case KEY_INPUT:
			_key = new KeyInput(me, leader);
			break;
		case KEY_ATTACKER:
			_key = new KeyAttacker(me, leader);
			break;
		case KEY_DEFFENDER:	
			_key = new KeyDeffender(me, leader);
			break;
		case KEY_HEALLER:
			_key = new KeyHealer(me, leader);
			break;
		case KEY_LEADER:
			_key = new KeyAttacker(me, leader);
			break;
		/*case KEY_FEINTER:	
			break;*/
	}
	// ���������s
	if( _key == nullptr ){
		LPCWSTR str[KEY_MAX] =
		{
			L"KeyInput",
			L"KeyAttacker",
			L"KeyDeffender",
			L"KeyHealer",
			L"KeyFeinter",
			L"KeyLeader"
		};
		MessageBox(NULL, str[_keyType], L"�L�[�̍쐬���s", MB_OK); 
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//! �X�e�[�^�X�̍쐬
//-----------------------------------------------------------------------------
bool Man::createStatus()
{
	// ���łɍ쐬����Ă����珈�����Ȃ�
	if( _myStatus != nullptr ) return true;

	switch( _keyType )
	{
		case KEY_INPUT:
			_myStatus = new Status();
			break;
		case KEY_ATTACKER:
			_myStatus = new Status();
			break;
		case KEY_DEFFENDER:	
			_myStatus = new Status();
			break;
		case KEY_HEALLER:
			_myStatus = new StatusHeal();
			break;
		case KEY_LEADER:
			_myStatus = new Status();
			break;
		/*case KEY_FEINTER:	
			break;*/
	}
	// ���������s
	if( _myStatus == NULL ){
		LPCWSTR str[KEY_MAX] =
		{
			L"KeyInput",
			L"KeyAttacker",
			L"KeyDeffender",
			L"KeyHealer",
			L"KeyFeinter",
			L"KeyLeaader",
		};
		MessageBox(NULL, str[_keyType], L"�X�e�[�^�X�̍쐬���s", MB_OK); 
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//!	������
//-----------------------------------------------------------------------------
bool Man::Initialize()
{

	// �X�e�[�^�X��������
	//SAFE_NEW(_myStatus);
	
	// �t���O�֘A������
	_hitGround = false;
	_hitCastle = false;
	_hitObj	   = false;
	_isAttack  = false;
	_isJump	   = false;
	_isDead	   = false;
			   
	// �������Ă��郊�X�g��������
	_myCollision.clearHitObj();
	// �����蔻��p�̍��W�X�V
	toHitPosition(_position, _hitPosition);

	// �����蔻��p�̋����X�V
	setSphereParam(_hitPosition, _hitRadius);

	

	return true;
}

//-----------------------------------------------------------------------------
//! �W�����v
//-----------------------------------------------------------------------------
void Man::Jump()
{
	// �n�ʂƐڒn���Ă���
	//if( _hitGround  ) {
	if( !_isJump ) {
		// �����ݒ�
		_mov._y += 10.0f;
		// �ڒn�t���OOFF
		_hitGround = false;
		// �W�����v�t���OON
		_isJump	   = true;
	}
}

//-----------------------------------------------------------------------------
//! �U��
//-----------------------------------------------------------------------------
void Man::Attack()
{
	if( !_isAttack ) {
		// ���W����ǂꂭ�炢����邩
		Vector3 attackOffset;
		attackOffset._x = _attackOffsetRange * sinf( _rotation._y / 180.0f * math::PI);
		attackOffset._y = _hitRadius;				   
		attackOffset._z = _attackOffsetRange * cosf( _rotation._y / 180.0f * math::PI);
		// ���W�ɂ��ꕪ����
	    _attackPosition =  _position + attackOffset;
		// �U���͈͐ݒ�
		_attackRadius	= _attackRadNum;

		// �U������~
		ISEManager()->stopMusic(SESoundPlayerManager::SE_ATTACK);
		// �U�����Đ�
		ISEManager()->playMusic(SESoundPlayerManager::SE_ATTACK);

		// �U���t���OON
		_isAttack		= true;
	}
}

//-----------------------------------------------------------------------------
//! �_���[�W
//-----------------------------------------------------------------------------
void Man::Damage(const s32& damage)
{
	// �ŏ��l�ȉ��Ȃ�
	//if(_myStatus->getHP() <= 0 ){ 
	s32 nowHP = (s32)_myStatus->getParam()->_HP;
	nowHP  -= damage;
	_myStatus->getParam()->_HP = (f32)nowHP;

	if(_myStatus->getParam()->_HP <= 0 ) {
		// �ŏ��l�ŌŒ�
		_myStatus->getParam()->_HP = 0;
		return;
	}

	// �_���[�W����~
	ISEManager()->stopMusic(SESoundPlayerManager::SE_DAMAGE);
	// �_���[�W���Đ�
	ISEManager()->playMusic(SESoundPlayerManager::SE_DAMAGE);

	// �p�[�e�B�N������
	IParticleMan()->generateParticle(_hitPosition,
									 Vector3(0.0f),
									 Vector3(0.5f, 0.5f, 0.0f),
									 Radian(0.0f),
									 10, 0,
									 Radian(0.0f),
									 ParticleManager::PARTICLE_HIT); 

	_isDamage = true;
}

//-----------------------------------------------------------------------------
//! ���W�Đݒ�
//-----------------------------------------------------------------------------
void Man::ResetPosition()
{
	// ���W�ݒ肵�Ȃ���
	_position._x = (f32)( rand() % 200 - 100 );
	_position._z = (f32)( rand() % 200 - 100 );
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void Man::Update()
{
	// �������n�ʂ�艺�Ɍ������Ƃ��̋~�ϑ[�u
	if( _position._y <= -1000.0f ){
		_position._y = 100.0f;
	}

	// �����蔻��p�̍��W�X�V
	toHitPosition(_position, _hitPosition);

	// �����蔻��p�̋����X�V
	//setSphereParam(_position, _hitRadius);
	setSphereParam(_hitPosition, _hitRadius);
	// �U���p�̋����X�V
	_attackSphere.setPosition(_attackPosition);
	_attackSphere.setRadius(_attackRadius);
}

//-----------------------------------------------------------------------------
//!	�`��
//-----------------------------------------------------------------------------
void Man::Render(bool isShadow)
{
}


//=============================================================================
//	END OF FILE
//=============================================================================