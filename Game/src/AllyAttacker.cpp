//-----------------------------------------------------------------------------
//!
//!	@file	AllyAttacker.cpp
//!	@brief	�����U���^�C�v
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

DEF_COL_NAME(AllyAttacker);
DEF_COL_ATTACK_NAME(AllyAttackerAttack);
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
AllyAttacker::AllyAttacker()
: AllyBase (Man::KEY_ATTACKER)
{
	// �����蔻�菉����
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);

	// ���f���ǂݍ���
	loadModel("../Model/Ally/Attacker/Ally_high.x", CHARA_SCALE);

	


	// ������
//	Initialize();

	// �A�j���[�V����������
	initAnimation();
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
AllyAttacker::~AllyAttacker()
{
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool AllyAttacker::Initialize()
{
	// �L�[�̍쐬
	createKey(this, _pSquad->getLeader());

	// ������
	AllyBase::Initialize();


	//_myStatus->loadCSV("../Assets/Status/Status - Attacker.csv");
	
	// �X�e�[�^�X������
	_myStatus->setParam(StatusAssets::STATUS_ATK);

	// �U�����̔��a�ݒ�
	_attackRadNum = _radius * 0.5f;
	_attackAABB._min = -Vector3(_radius * 0.5f);
	_attackAABB._max =  Vector3(_radius * 0.5f);
	_attackOffsetRange = _attackRadNum * 1.5f;

	return true;
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void AllyAttacker::Update()
{
	AllyBase::Update();
}

//-----------------------------------------------------------------------------
//! �X�V(�R�}���h�ɂ���ĕς��)(���͕ς��Ȃ�)
//-----------------------------------------------------------------------------
void AllyAttacker::UpdateByCommand()
{
	// �K�v���̎擾
	Vector3			playerPos	  = IObjDataManager()->_pPlayer->getPosition();
	Player::COMMAND playerCommand = IObjDataManager()->_pPlayer->getCommand();

	// �R�}���h���U�����A�v���C���[������̋����Ȃ�
	//if( playerCommand == Player::COMMAND_ATTACK && (playerPos - _position).length() <= 300.0f )
	if((playerPos - _position).length() <= 500.0f && !_goGoal)
	{
		// �^�[�Q�b�g�̌����A�ݒ�
		if( AllyBase::searchAndSetTarget() ){
			// �^�[�Q�b�g�������
			// �L�[�̍X�V
			_key->Update();

			// �p�x�����߂�
			// 3D��ł�xz�̏��ԂŊp�x�����߂���
			Vector3	StickState = _key->getVector();
	
			// �ړ�������΍X�V
			if( abs(atan2f(StickState._x, StickState._z) ) > 0.0f )
			{
				_rotation._y = atan2f( StickState._x, StickState._z );
				Status::Param* myParam = _myStatus->getParam();
				_speed = myParam->_speedMAX;
			}else{
				_speed = 0.0f;
			}
		}else{
			// �^�[�Q�b�g�����Ȃ���΃v���C���[�Ɍ�����
			_goGoal = true;
		}

	}else{
		// �v���C���[�Ɍ������t���OON
		_goGoal = true;
		// ����ȊO�͒ʏ�ʂ�
		UpdateNormal(playerPos);
	}

	// �����蔻�菈���ƃ_���[�W�v�Z
	s32 damage = calcDamageAndCollision();

	s32	currentAnimation = _pTaskModel->getAnimation();

	if( damage > 0 ) {
		// �_���[�W����
		Damage(damage);
		// �_���[�W�A�j���[�V������
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
void AllyAttacker::UpdateNormal(Vector3& playerPos)
{
	// �v���C���[�ւ̃x�N�g��
	Vector3	dir	   = playerPos - _position;
	// �����擾
	f32		length = dir.length();

	Circle	goalCircle(playerPos, 100.0f);
	Circle	myCircle(_position, _radius);

	if( goalCircle.isHit(myCircle) ){
		_goGoal = false;
		_speed = 0.0f;
	}else{
		Status::Param* myParam = _myStatus->getParam();
		_speed = myParam->_speedMAX;
		dir.normalize();
		_rotation._y = atan2f(dir._x, dir._z);
	}
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void AllyAttacker::Render(bool isShadow)
{
	AllyBase::Render(isShadow);
}

//-----------------------------------------------------------------------------
//! �_���[�W�v�Z�Ɠ����蔻�菈��
//-----------------------------------------------------------------------------
s32 AllyAttacker::calcDamageAndCollision()
{
	// �_���[�W�ۑ��p
	u32 damage = 0;
	//---- �����蔻��
	for( s32 i=0; i<_myCollision.getHitCount(); ++i )
	{
		Collision* obj = _myCollision.getHitObj(i);
		if( isAllyAttacker(obj) ){

			// �␳�ʒu���擾����
			Vector3	dir = _myCollision.getOffsetPos(obj);
			// �␳�ʒu�ɐݒ�
			_hitPosition = dir;
			// �����蔻��p�̋����X�V
			setSphereParam(_hitPosition, _hitRadius);

		}else if( isEnemyLeaderAttack(obj) || isEnemyAttakerAttack(obj) || isEnemyDeffenderAttack(obj) ){
			// �_���[�W����
			_hitAttack = true;
			damage += (u32)IObjDataManager()->_enemyStatus->getParam()->_atkPow;
		}
		// ���W�̐ݒ�
		toPosition(_hitPosition, _position);
	}

	return damage;
}

//=============================================================================
//	END OF FILE
//=============================================================================