//-----------------------------------------------------------------------------
//!
//!	@file	Ally.cpp
//!	@brief	�������
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//=============================================================================
//	�������
//=============================================================================

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
AllyBase::AllyBase()
: Man(Man::KEY_ATTACKER,  Man::TYPE_ALLY)
, _pSquad		(nullptr)
, _pPrevTarget	(nullptr)
, _goGoal		(true)
{
	
}

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//!	@param	[in]	myType	�L�[�^�C�v
//-----------------------------------------------------------------------------
AllyBase::AllyBase(Man::KEY_TYPE myType)
: Man(myType,  Man::TYPE_ALLY)
, _pSquad		(nullptr)
, _pPrevTarget	(nullptr)
, _goGoal		(true)
{
	
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
AllyBase::~AllyBase()
{
}
//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool AllyBase::Initialize()
{
	// �v���C���[�̏����ʒu�ɐݒ肷��
	_leaderPos = Vector3(-3200.0f, 0.0f, -2600.0f);

	// ���W�ݒ�
	ResetPosition();
	// ������
	Man::Initialize();
	return true;
}


//! �A�j���[�V����������
void AllyBase::initAnimation()
{
	_aniMove	= _pTaskModel->searchAnimation("Run");
	_aniAttack	= _pTaskModel->searchAnimation("Attack");
	_aniWait	= _pTaskModel->searchAnimation("Wait");
	_aniDamage	= _pTaskModel->searchAnimation("Damage");
	_aniHeal	= _pTaskModel->searchAnimation("Heal");
}
//-----------------------------------------------------------------------------
//! �X�V(���̒��ōX�V���ׂ�Update���Ăяo��)
//-----------------------------------------------------------------------------
void AllyBase::Update()
{
	// ���S�t���O�������Ă����珈�����Ȃ�
	if( _isDead ) return;
	
	// �������Ă��镪�����烊�[�_�[�̈ʒu���擾
	_leaderPos = _pSquad->getLeader()->getPosition();

	// ���݂̃A�j���[�V�������擾
	s32	currentAnimation = _pTaskModel->getAnimation();


// �f�o�b�O�p
_DebugGoalPos = IObjDataManager()->_pPlayer->getPosition();

	// �v���C���[�̖��߂ɂ���ď����ύX
	UpdateByCommand();

	//---- �ړ�
	// �L�����̈ړ�(���X�e�B�b�N�̏��擾)
	_mov._x = sinf( _rotation._y )  * _speed;
	_mov._z = cosf( _rotation._y )  * _speed;

	if( _speed > 0.0f ) 
	{
		// �޸�ذ�ɒ���
		_rotation._y = TO_DEGREE(_rotation._y );
	}

	// �X�V
	_oldPosition = _position;

	// �t���[�Y���Ȃ�������
	if( !checkFreeze(currentAnimation) ){
		// �ړ����ĂȂ����
		if( _mov._x == 0.0f && _mov._z == 0.0f )
		{
			// �������[�V����
			//_aniState = STATE_WAIT;
			if( currentAnimation != _aniWait ) {
				_pTaskModel->setAnimation(_aniWait, ANIMATION_PLAY_REPEAT);
			}
		}else{
			// �������[�V����
			//_aniState = STATE_MOVE;
			if( currentAnimation != _aniMove ) {
				_pTaskModel->setAnimation(_aniMove, ANIMATION_PLAY_REPEAT);
			}
		}
	}

	// �U�����Ă���
	if( currentAnimation == _aniAttack ) {
		// �ړ����x�����Ȃ�����
		_mov._x *= 0.5f;
		_mov._z *= 0.5f;
	}else if( currentAnimation == _aniDamage ){
		// �ړ����x���Ȃ���
		_mov._x *= 0.0f;
		_mov._z *= 0.0f;
	}

	_position += _mov * Global::deltaTime;

	//---- �p�[�e�B�N��
	// �p�[�e�B�N���̐����҂����ԃJ�E���g
	_genCount += 1 * Global::deltaTime;

	Vector3 sub = _oldPosition - _position;

	// �ړ����Ă��銎�A�����҂����Ԃ��؂ꂽ��
	if( (abs(sub._x) > 0.0f || abs(sub._z) > 0.0f) && _genCount >= _waitGenTime ) {

		// �J�E���g���Z�b�g
		_genCount = 0;

		Vector3 pos = _position + Vector3( (f32)(rand() % 40 - 20) );

		Vector3 mov = -_mov;

		mov._x *= (rand() %  50) / 100.0f;
		mov._y  = (rand() % 100) / 100.0f;
		mov._z *= (rand() %  50) / 100.0f;

		f32		rot = ( rand() % 20 ) / 20.0f;
		Radian	angVel = Radian( ( rand() % 10 ) / 100.0f );

		// �p�[�e�B�N������
		IParticleMan()->generateParticle(pos,
										 mov,
										 Vector3(0.5f, 0.5f, 0.0f),
										 Radian(rot),
										 100, 0,
										 angVel,
										 ParticleManager::PARTICLE_SMOKE); 
	}


	// �����蔻��̋��̍X�V
	Man::Update();
	
	//---- �}�b�v�Ƃ̓����蔻��
	GmSystemCollision()->checkCollisionModel(_hitPosition, _radius, _hitGround);

	//---- ��A��ǂƂ̓����蔻��
	Vector3 sink = ICastleCol()->getSinkVal(_colAABB, _hitCastle, _position);
	
	if( _hitCastle )
	{
		_hitPosition += sink;
	}

	// �O�ɂ����Ȃ�����
	ICastleCol()->hitOutRange(_hitPosition);

	// �����蔻��p�̋����X�V
	setSphereParam(_hitPosition, _hitRadius);
	// ���W�̐ݒ�
	toPosition(_hitPosition, _position);

	// �d�͏���
	if( !_hitGround ){
		_mov._y -= 0.6f * Global::deltaTime;
	}
	else{
		_mov._y = 0.0f;
	}

	// �ő�d�͐ݒ�
	if( _mov._y <= -9.8f )
	{
		_mov._y = -9.8f; 
	}

	if( _aniAttack != -1 ) {

		// �U���{�^����������Ă��邩�ǂ���
		if( _key->getPushState("Attack") )
		{
			// �A�j���[�V�������U���Ɉڍs
			//_aniState = STATE_ATTACK;
			// �U���A�j���[�V�����Đ�
			_pTaskModel->setAnimation(_aniAttack);
		}else if ( currentAnimation == _aniAttack ) {

			// �U���Đ����Ȃ�
			// �Đ��䗦�擾
			f32	aniRate = _pTaskModel->getAnimationRate();
			// �Đ���������������
			if( aniRate >= 0.5f && !_isAttack ) {
				// �U��
				Attack();
				ICastleCol()->hitAttackAndDamage(_attackAABB, _attackPosition, (u32)_myStatus->getParam()->_atkPow);	
			}else if( _isAttack ){
				// �U���͈́A���W��߂�
				_attackPosition = Vector3( 0.0f, 0.0f, 0.0f );
				_attackRadius	= 0.0f;
			}

		}else{
			// ���̎��_�Ŕ��a������������ĂȂ����
			if( _attackRadius != 0.0f ){
				// �U���͈́A���W��߂�
				_attackPosition = Vector3( 0.0f, 0.0f, 0.0f );
				_attackRadius	= 0.0f;
			}
			// �U���t���O�����ɖ߂�
			_isAttack = false;
		}

	}

	// �̗͂��O�ȉ��Ȃ�
	if( _myStatus->getParam()->_HP <= 0 )
	{
		_isDead = true;
	}
	
	
}

//-----------------------------------------------------------------------------
//! �`��
//-----------------------------------------------------------------------------
void AllyBase::Render(bool isShadow)
{
	// ���S�t���O�������Ă����珈�����Ȃ�
	if( _isDead ) return;
	// �J�����O����邩�ǂ���
	Sphere hitSphere = *_myCollision.getHitSphere();
	if( GmRender()->isRender(hitSphere) ) return; 

	// �ړ��p�}�g���b�N�X
	Matrix	offsetMatrix(_pTaskModel->getWorldMatrix());
	offsetMatrix = offsetMatrix.translate(_position);
	Matrix rotateMatrix(_pTaskModel->getRotateMatrix());
	rotateMatrix = rotateMatrix.rotateY(Degree(_rotation._y) );
	_pTaskModel->setWorldMatrix(offsetMatrix);
	_pTaskModel->setRotateMatrix(rotateMatrix);
	
	_pTaskModel->render(isShadow);
	
}

//-----------------------------------------------------------------------------
//! glut�`��֘A
//-----------------------------------------------------------------------------
void AllyBase::glutRender()
{
	// ���S�t���O�������Ă����珈�����Ȃ�
	if( _isDead ) return;

	// ��
	GLfloat blue[]	 = { 0.0f, 0.0f, 1.0f, 1.0f };
	// ����
	GLfloat indigo[]	 = { 0.29f, 0.0f, 0.52f, 1.0f };
	// ��
	GLfloat green[]	 = { 0.0f, 1.0f, 0.0f, 1.0f };

	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	// �f�o�b�N�\��
	//_myCollision.DebugRender();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	_attackCollision.DebugRender();

}
//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void AllyBase::debugRender()
{
	// ���S�t���O�������Ă����珈�����Ȃ�
	if( _isDead ) return;

	drawAABB( _colAABB, Vector4(1.0f, 1.0f, 1.0f, 1.0f), _position );
	if( _isAttack ) {
		drawAABB( _attackAABB, Vector4(1.0f, 0.0f, 0.0f, 1.0f), _attackPosition );
	}
}

//-----------------------------------------------------------------------------
//! �X�e�[�^�X�̕`��(3D)
//-----------------------------------------------------------------------------
void AllyBase::StatusRender()
{
	// ���S�t���O�������Ă����珈�����Ȃ�
	if( _isDead ) return;

	// �`����W�ݒ�
	Vector3 drawPos;
	drawPos._x = _position._x - _myStatus->getParam()->_GaugeScaleW * 0.5f;
	drawPos._y = _position._y + _radius + 20.0f;
	drawPos._z = _position._z;

	// �̗͂�ŕ\��
	Color gaugeColor(0, 0, 255);
	// �`��
	_myStatus->Render(false, gaugeColor, drawPos);
}


//-----------------------------------------------------------------------------
//! ���W�Đݒ�
//-----------------------------------------------------------------------------
void AllyBase::ResetPosition()
{
	// ���W�ݒ肵�Ȃ���(�I�t�Z�b�g�v�Z)
	_position._x = (f32)( rand() % 200 - 100 );
	_position._z = (f32)( rand() % 200 - 100 );
	// �ڕW���W�ɑ���
	_position = _leaderPos + _position;
	_position._y = _leaderPos._y;
	// �U���������Ă�t���OOFF
	_hitAttack = false;
}

//-----------------------------------------------------------------------------
//! �_�������T���Đݒ�
//!	@return true:�G��ݒ芮�� false:�_���G�͂��Ȃ�
//-----------------------------------------------------------------------------
bool AllyBase::searchAndSetTarget()
{
	Vector3	playerPos = IObjDataManager()->_pPlayer->getPosition();
	// �O�̏�񂪂������炩�A���S���Ă��Ȃ����
	if( _pPrevTarget != nullptr && !_pPrevTarget->isDead() ) {
		f32 lengthToTarget = getLengthtoTarget(_pPrevTarget);
		// �{���͈͓��Ȃ�
		static const f32 searchLength = 200.0f;
		if( lengthToTarget < searchLength ) {
			// �������Ȃ�(�G�͐ݒ肳��Ă���)
			return true;
		}
	}

	// �G�̃X�e�[�^�X��������Ă���
	vector<EnemyBase*> pEnemyData = IObjDataManager()->_pEnemyData;
	vector<EnemyBase*>::iterator	itr;
	f32	 minLength = FLT_MAX;
	EnemyBase* target = nullptr;
	for( itr = pEnemyData.begin(); itr != pEnemyData.end(); itr++ )
	{
		EnemyBase* enemy = *itr;
		// ���S���Ă����珈�����Ȃ�
		if( enemy->isDead() ) continue;
		// �v���C���[����G�ւ̋������擾
		f32 length = (enemy->getPosition() - playerPos).squareLength();
		// ����̒������Z�����
		if( minLength > length )
		{
			// �X�V
			minLength = length;
			target	  = enemy;
		}
	}
	// ��ԋ߂��G���^�[�Q�b�g�ɐݒ�
	if( target != nullptr ) {
		// ����p�^�[�Q�b�g�ɐݒ肵�Ă���
		_pPrevTarget = target;
		// ��ԋ߂�����^�[�Q�b�g�ɂ���
		_key->setTarget(target);
	}else{
		// �_���G�͂��Ȃ�
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//! �A�j���[�V�����ɂ��ړ��s�`�F�b�N
//-----------------------------------------------------------------------------
bool AllyBase::checkFreeze(const s32&	currentAnimation)
{
	// �A�j���[�V�������Ȃ����
	if( currentAnimation != -1 ){
		// �U�����Ȃ�t���[�Y
		if( currentAnimation == _aniAttack ) return true;
		// �_���[�W���Ȃ�t���[�Y
		if( currentAnimation == _aniDamage ) return true;
		// �q�[�����Ȃ�t���[�Y
		if( currentAnimation == _aniHeal   ) return true;
	}
	// �����܂ł�����t���[�Y���Ȃ�
	return false;
}

//=============================================================================
//	END OF FILE
//=============================================================================