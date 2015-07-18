//-----------------------------------------------------------------------------
//!
//!	@file	EnemyDeffender.cpp
//!	@brief	�G�h��^�C�v
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

DEF_COL_NAME(EnemyDeffender);
DEF_COL_ATTACK_NAME(EnemyDeffenderAttack);
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
EnemyDeffender::EnemyDeffender()
: EnemyBase(Man::KEY_DEFFENDER)
{
	// �����蔻�菉����
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);

	// ���f���T�C�Y�ύX
	_modelScale = 1.2f;
	// ���f���ǂݍ���
	loadModel("../Model/Enemy/Attacker/Enemy.x", CHARA_SCALE * _modelScale);

	// �A�j���[�V����������
	initAnimation();
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
EnemyDeffender::~EnemyDeffender()
{
}
//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool EnemyDeffender::Initialize()
{

	// �L�[�̍쐬
	createKey(this, _pSquad->getLeader());

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
void EnemyDeffender::Update()
{
	EnemyBase::Update();
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void EnemyDeffender::UpdateByCommand()
{
	if( !_goGoal ){
		// �^�[�Q�b�g�̌����A�ݒ�
		EnemyBase::searchAndSetTarget();
		
		// �L�[�̍X�V
		_key->Update();

		// �p�x�����߂�
		// 3D��ł�xz�̏��ԂŊp�x�����߂���
		Vector3	StickState = _key->getVector();
	
	
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

	// �_���[�W�ۑ��p
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
void EnemyDeffender::UpdateNormal(Vector3& leaderPos)
{
	Vector3 dir = leaderPos - _position;
	// �����擾
	f32		length = dir.length();
	Circle	goalCircle(leaderPos, 100.0f);
	Circle	myCircle(_position, _radius);

	if( goalCircle.isHit(myCircle) ){
		_speed = 0.0f;
		_goGoal = false;
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
void EnemyDeffender::Render(bool isShadow)
{
	EnemyBase::Render(isShadow);
}


//=============================================================================
//	END OF FILE
//=============================================================================