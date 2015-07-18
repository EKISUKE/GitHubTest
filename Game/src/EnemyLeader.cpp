//-----------------------------------------------------------------------------
//!
//!	@file	EnemyLeader.cpp
//!	@brief	�G�U���^�C�v
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

DEF_COL_NAME(EnemyLeader);
DEF_COL_ATTACK_NAME(EnemyLeaderAttack);
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
EnemyLeader::EnemyLeader()
: EnemyBase(Man::KEY_LEADER)
{
	// ������@�ݒ�(���͉��Ŏ��������[�_�Ƃ��Ă�)
	//_key = new KeyAttacker(this, this);
	// �����蔻�菉����
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);


	// ���f���T�C�Y�ύX
	_modelScale = 1.5f;

	// ���f���ǂݍ���(���[�_�[�͏����傫�߂ɂ���)
	loadModel("../Model/Enemy/Attacker/Enemy.x", CHARA_SCALE * _modelScale);

	// �L�[�̍쐬
	createKey(this, this);

	// �h�q���W�ݒ�
//	_deffencePos = IObjDataManager()->_deffencePos[0];

	// ������
//	Initialize();

	// �A�j���[�V����������
	initAnimation();
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
EnemyLeader::~EnemyLeader()
{
}
//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool EnemyLeader::Initialize()
{
	// ������
	EnemyBase::Initialize();
	
	// �X�e�[�^�X������
	_myStatus->setParam(StatusAssets::STATUS_ENEMY);

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
void EnemyLeader::Update()
{
	EnemyBase::Update();
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void EnemyLeader::UpdateByCommand()
{
	
	// ��苗���͂Ȃ�Ă��Ȃ����
	if( (_goalPos - _position).length() <= 3000.0f && !_goGoal){
		
		// �^�[�Q�b�g�̌����A�ݒ�
		EnemyBase::searchAndSetTarget();

		
		// �L�[�̍X�V
		_key->Update();

		//---- �L�����̈ړ�
		Vector3	StickState = _key->getVector();
		
		// �X�e�B�b�N���삪����Ă�����
		if( abs(atan2f(StickState._x, StickState._z) ) > 0.0f )
		{
			_rotation._y = atan2f( StickState._x, StickState._z );
			_speed = _myStatus->getParam()->_speedMAX;
		}else{
			_speed = 0.0f;
		}
	}else{
		UpdateNormal(_goalPos);
	}

	// �����蔻��ƃ_���[�W�v�Z
	s32 damage = EnemyBase::calcDamageAndCollision();
	
	s32 currentAnimation = _pTaskModel->getAnimation();

	if( damage > 0 ) {
		Damage(damage);
		// �_���[�W�A�j���[�V������
		_pTaskModel->setAnimation(_aniDamage);
	}else if( currentAnimation == _aniDamage ){
		_isDamage = false;
	}
	
	_pTaskModel->update();
		
}

//-----------------------------------------------------------------------------
//! ���[�_�[�̎���ɏ�ɂ���
	//!	@param	[in]	leader	���[�_�[���W
//-----------------------------------------------------------------------------
void EnemyLeader::UpdateNormal(Vector3& leaderPos)
{
	Vector3 dir = leaderPos - _position;
	// �����擾
	f32		length = dir.length();
	Circle	goalCircle(leaderPos, 500.0f);
	Circle	myCircle(_position, _radius);

	if( goalCircle.isHit(myCircle) ){
		_goGoal = false;
		_speed = 0.0f;
	}else{
		Status::Param* myParam = _myStatus->getParam();
		_speed = myParam->_speedMAX;
		dir = dir.normalize();
		_rotation._y = atan2f(dir._x, dir._z);
		_goGoal = true;
	}
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void EnemyLeader::Render(bool isShadow)
{
	EnemyBase::Render(isShadow);
}


//=============================================================================
//	END OF FILE
//=============================================================================