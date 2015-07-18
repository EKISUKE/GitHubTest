//-----------------------------------------------------------------------------
//!
//!	@file	AllyHealer.cpp
//!	@brief	�����񕜃^�C�v
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

DEF_COL_NAME(AllyHealer);
DEF_COL_ATTACK_NAME(AllyHealerAttack);
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
AllyHealer::AllyHealer()
: AllyBase		(Man::KEY_HEALLER)
, _SeverityMax	( 10 )
, _isHeal		(false)
{
	// �����蔻�菉����
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);

	// ���f���ǂݍ���
	loadModel("../Model/Ally/Healer/Ally_high.x", CHARA_SCALE);


	// ������
//	Initialize();

	// �A�j���[�V����������
	initAnimation();
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
AllyHealer::~AllyHealer()
{
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool AllyHealer::Initialize()
{
	// �L�[�̍쐬
	createKey(this, _pSquad->getLeader());

	// ������
	AllyBase::Initialize();

	// �X�e�[�^�X������
	_myStatus->setParam(StatusAssets::STATUS_HEAL);

	// �U�����̔��a�ݒ�
	_attackRadNum = _radius * 0.5f;
	_attackAABB._min = -Vector3(_radius * 0.5f);
	_attackAABB._max =  Vector3(_radius * 0.5f);
	_attackOffsetRange = _attackRadNum * 1.5f;
	return true;
}

//-----------------------------------------------------------------------------
//! �q�[��
//!	@param	[in]	target	�ڕW
//-----------------------------------------------------------------------------
void AllyHealer::Heal(Man*	target)
{
	
	

	// �^�[�Q�b�g�̃X�e�[�^�X���Ƃ��Ă���
	Status::Param* targetParam = target->getStatus()->getParam();
	// �^�[�Q�b�g�̗̑͂��񕜂���
	f32 HP = targetParam->_HP;
	StatusHeal::ParamHeal* myParam = (StatusHeal::ParamHeal*)_myStatus->getParam();
	//HP += _myStatus->getParam()->_healPow;
	HP += myParam->_healPow;
	// HP��ݒ�
	targetParam->_HP = HP;
	// �͈͊O�w��
	if( targetParam->_HP > targetParam->_HPMAX )
	{
		targetParam->_HP = targetParam->_HPMAX;
	}

	// �^�[�Q�b�g�̍��W�擾
	Vector3 targetPos = target->getHitPosition();

	// �p�[�e�B�N������
	IParticleMan()->generateParticle(targetPos,
									 Vector3(0.0f),
									 Vector3(0.5f, 0.5f, 0.0f),
									 Radian(0.0f),
									 60, 0,
									 Radian(0.0f),
									 ParticleManager::PARTICLE_HEAL); 
	// �q�[������~
	ISEManager()->stopMusic(SESoundPlayerManager::SE_HEAL);
	// �q�[�����Đ�
	ISEManager()->playMusic(SESoundPlayerManager::SE_HEAL);

	_isHeal = true;
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void AllyHealer::Update()
{
	AllyBase::Update();
}

//-----------------------------------------------------------------------------
//! �X�V(�R�}���h�ɂ���ĕς��)(���͂����Ȃ�)
//-----------------------------------------------------------------------------
void AllyHealer::UpdateByCommand()
{

	//---- �v���C���[���擾
	Player*	pPlayer   = IObjDataManager()->_pPlayer;
	Vector3 playerPos = pPlayer->getPosition();

	// �񕜒��Ȃ�X�V���Ȃ�
	if( !_isHeal ) {
		// �^�[�Q�b�g�̌����A�ݒ�
		if( searchAndSetTarget() )
		{
			// �^�[�Q�b�g��������Ȃ����
			_keyUpdate = false;
		}
	}


	s32	currentAnimation = _pTaskModel->getAnimation();
		
	// �v���C���[�̎���ɂ���Ȃ��
	if( _keyUpdate ){
		
		// �L�[�̍X�V
		_key->Update();

		// �p�x�����߂�
		// 3D��ł�xz�̏��ԂŊp�x�����߂���
		Vector3	StickState = _key->getVector();
	
		// �q�[���R�}���h��ON�Ȃ�q�[������
		if( _key->getPushState("Heal") )
		{
			// �U���A�j���[�V�����Đ�(1�̓q�[��)
			_pTaskModel->setAnimation(_aniHeal);
		}else if ( currentAnimation == _aniHeal ) {
			// �q�[���Đ����Ȃ�
			// �Đ��䗦�擾
			f32	aniRate = _pTaskModel->getAnimationRate();
			// �Đ���������������
			if( aniRate >= 0.1f && !_isHeal) {
				// �q�[��
				Heal(_target);
			}
		}else{
			_isHeal = false;
		}


		// �ړ�������΍X�V
		if( abs(atan2f(StickState._x, StickState._z) ) > 0.0f )
		{
			_rotation._y = atan2f( StickState._x, StickState._z );
			Status::Param* myParam = _myStatus->getParam();
			_speed = myParam->_speedMAX;
		}else{
			_speed = 0.0f;
		}

	} else {
		// ����ȊO�͒ʏ�ʂ�
		UpdateNormal(playerPos);
	}

	// �����蔻�菈���ƃ_���[�W�v�Z
	s32 damage = calcDamageAndCollision();


	if( damage > 0 ) {
		// �_���[�W����
		Damage(damage);
		_pTaskModel->setAnimation(_aniDamage);
	}else if( currentAnimation == _aniDamage ){
		_isDamage = false;
	}
	_pTaskModel->update();
}

//-----------------------------------------------------------------------------
//! �X�V(�v���C���[�̎���ɏ�ɂ���)
//!	@param	[in]	player	�v���C���[���W
//-----------------------------------------------------------------------------
void AllyHealer::UpdateNormal(Vector3& playerPos)
{
	// �v���C���[�ւ̃x�N�g��
	Vector3	dir	   = playerPos - _position;
	// �����擾
	f32		length = dir.length();

	Circle	goalCircle(playerPos, 100.0f);
	Circle	myCircle(_position, _radius);

	// �v���C���[�̋߂��ɂ��邩�ǂ���
	if( goalCircle.isHit(myCircle) ){
		// �߂��ɂ���
		_speed = 0.0f;
		_keyUpdate = true;
	}else{
		// �߂��ɂ��Ȃ�
		_speed = 3.0f;
		dir = dir.normalize();
		_rotation._y = atan2f(dir._x, dir._z);
		_keyUpdate = false;
	}



}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void AllyHealer::Render(bool isShadow)
{
	AllyBase::Render(isShadow);
}

//-----------------------------------------------------------------------------
//! �_�������T���Đݒ�
//!	@return true:�G��ݒ芮�� false:�_���G�͂��Ȃ�
//-----------------------------------------------------------------------------
bool AllyHealer::searchAndSetTarget()
{

	Player*	pPlayer   = IObjDataManager()->_pPlayer;

	// �O�̏�񂪂������炩�A���S���Ă��Ȃ����
	if( _pPrevTarget != nullptr && !_pPrevTarget->isDead() ) {
		f32 lengthToTarget = getLengthtoTarget(_pPrevTarget);
		// �{���͈͓��Ȃ�
		static const f32 searchLength = 200.0f;
		if( lengthToTarget < searchLength ) {
			// �������Ȃ�
			return true;
		}
	}

	// �ŏ��̓v���C���[
	_target = pPlayer;
	// �����f�[�^�����ԑ̗͂̏��Ȃ�����������o��
	vector<AllyBase*>::iterator itr;
	// �v���C���[�̗̑�
	Status::Param*	playerParam = IObjDataManager()->_pPlayer->getStatus()->getParam();

	f32 PlayerHP	   = playerParam->_HP;
	f32	PlayerHPMAX	   = playerParam->_HPMAX;
	s32 playerSeverity = (s32)( LinearInterpolation( (f32)(_SeverityMax) , 0, PlayerHP, PlayerHPMAX) ); 
	// �ً}�x
	s32 Severity = playerSeverity;

	// ���ׂĂ�HP�������A�ً}�x���`�F�b�N
	for( itr = IObjDataManager()->_pAllyData.begin(); itr != IObjDataManager()->_pAllyData.end(); itr++)
	{
		AllyBase* ally = *itr;
		// ���S���Ă����珈�����Ȃ�
		if( ally->isDead() ) continue;
		Status::Param* allyParam = ally->getStatus()->getParam();
		f32 HP		= allyParam->_HP;
		f32 HPMAX	= allyParam->_HPMAX;
		// ����ً̋}�x
		s32 nowSeverity	  = (s32)LinearInterpolation((f32)_SeverityMax, 0, HP, HPMAX);
		// ����ً̋}�x������������
		if( Severity < nowSeverity )
		{
			// ����ōX�V
			Severity = nowSeverity;
			_target	 = ally;
		}
	}

	// ��ԋ߂��G���^�[�Q�b�g�ɐݒ�
	if( _target != nullptr ) {
		// ����p�^�[�Q�b�g�ɐݒ肵�Ă���
		_pPrevTarget = _target;
		// ��ԋ߂�����^�[�Q�b�g�ɂ���
		_key->setTarget(_target);
	}else{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//! �_���[�W�v�Z�Ɠ����蔻�菈��
//-----------------------------------------------------------------------------
s32 AllyHealer::calcDamageAndCollision()
{
	// �_���[�W�ۑ��p
	s32 damage = 0;
	//---- �����蔻��
	for( s32 i=0; i<_myCollision.getHitCount(); ++i )
	{
		Collision* obj = _myCollision.getHitObj(i);
		if( isAllyHealer(obj) ){

			// �␳�ʒu���擾����
			Vector3	dir = _myCollision.getOffsetPos(obj);
			// �␳�ʒu�ɐݒ�
			_hitPosition = dir;
			// �����蔻��p�̋����X�V
			setSphereParam(_hitPosition, _hitRadius);

		}else if( isEnemyLeaderAttack(obj) || isEnemyAttakerAttack(obj) || isEnemyDeffenderAttack(obj) ){
			// �_���[�W����
			_hitAttack = true;
			damage += (s32)IObjDataManager()->_enemyStatus->getParam()->_atkPow;
		}
		// ���W�̐ݒ�
		toPosition(_hitPosition, _position);
	}

	return damage;
}


//=============================================================================
//	END OF FILE
//=============================================================================