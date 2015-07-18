//-----------------------------------------------------------------------------
//!
//!	@file	Enemy.cpp
//!	@brief	�������
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"


//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
EnemyBase::EnemyBase()
: Man			(Man::KEY_ATTACKER, Man::TYPE_ENEMY)
, _pSquad		(nullptr)
, _pPrevTarget	(nullptr)
, _goGoal		(true)
, _modelScale	(1.0f)
{
}

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//!	@param	[in]	myType	�L�[�^�C�v
//-----------------------------------------------------------------------------
EnemyBase::EnemyBase(Man::KEY_TYPE myType)
: Man			(myType, Man::TYPE_ENEMY)
, _pSquad		(nullptr)
, _pPrevTarget	(nullptr)
, _goGoal		(true)
, _modelScale	(1.0f)
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
EnemyBase::~EnemyBase()
{
}

//-----------------------------------------------------------------------------
//!	������
//-----------------------------------------------------------------------------
bool EnemyBase::Initialize()
{
	if( _keyType != KEY_LEADER) {
		_goalPos = Vector3(0.0f);
	}else{
		_goalPos = IObjDataManager()->_deffencePos[0];
	}
	// ���W�ݒ�
	ResetPosition();
	// ������
	Man::Initialize();

	return true;
}

//-----------------------------------------------------------------------------
//! �A�j���[�V����������
//-----------------------------------------------------------------------------
void EnemyBase::initAnimation()
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
void EnemyBase::Update()
{
	// ���S�t���O�������Ă����珈�����Ȃ�
	if( _isDead ) return;
	// �������Ă��镪�����烊�[�_�[�̈ʒu���擾
	if( _keyType != KEY_LEADER ) { 
		_goalPos = _pSquad->getLeader()->getPosition();
	}

	s32	currentAnimation = _pTaskModel->getAnimation();

	// ���[�_�[�̖��߂ɂ���ď����ύX
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
			// �U���A�j���[�V�����Đ�
			_pTaskModel->setAnimation(_aniAttack);
			// �U��
			Attack();

		}else if ( currentAnimation == _aniAttack ) {

			// �U���Đ����Ȃ�
			// �Đ��䗦�擾
			f32	aniRate = _pTaskModel->getAnimationRate();
			// �Đ���������������
			if( aniRate >= 0.5f && !_isAttack ) {
				// �U��
				Attack();
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
		// ���S�t���O�𗧂Ă�
		_isDead = true;
	}
}

//-----------------------------------------------------------------------------
//!	�`��
//-----------------------------------------------------------------------------
void EnemyBase::Render(bool isShadow)
{
	// ���S�t���O�������Ă����珈�����Ȃ�
	if( _isDead ) return;
	// �J�����O����邩�ǂ���
	Sphere hitSphere = *_myCollision.getHitSphere();
	if (GmRender()->isRender(hitSphere)) return;

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
void EnemyBase::glutRender()
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
void EnemyBase::debugRender()
{
	// ���S�t���O�������Ă����珈�����Ȃ�
	if( _isDead ) return;
	drawAABB( _colAABB, Vector4(1.0f, 1.0f, 1.0f, 1.0f), _position );
	if( _isAttack ) {
		drawAABB( _attackAABB, Vector4(1.0f, 0.0f, 0.0f, 1.0f), _attackPosition );
	}
}

//-----------------------------------------------------------------------------
//! �X�e�[�^�X�̕`��(2D)
//-----------------------------------------------------------------------------
void EnemyBase::StatusRender()
{
	// ���S�t���O�������Ă����珈�����Ȃ�
	if( _isDead ) return;
	// �`����W�ݒ�
	Vector3 drawPos;
	drawPos._x = _position._x - _myStatus->getParam()->_GaugeScaleW * 0.5f;
	drawPos._y = _position._y + _radius + (20.0f * _modelScale);
	drawPos._z = _position._z;

	// �̗͂�Ԃŕ\��
	Color gaugeColor(255, 0, 0);
	// �`��
	_myStatus->Render(false, gaugeColor, drawPos);
}

//-----------------------------------------------------------------------------
//! ���W�ݒ肵�Ȃ���
//-----------------------------------------------------------------------------
void EnemyBase::ResetPosition()
{
# if 0 
	// ���W�ݒ肵�Ȃ���
	_position._x = (f32)( rand() % 1000 - 500 );
	_position._z = (f32)( rand() % 1000 - 500 );
#else
	// ���W�ݒ肵�Ȃ���(�I�t�Z�b�g�v�Z)
	_position._x = (f32)( rand() % 200 - 100 );
	_position._z = (f32)( rand() % 200 - 100 );
	// �ڕW���W�ɑ���
	_position = _goalPos + _position;
	_position._y = _goalPos._y;
#endif
	// �U���������Ă�t���OOFF
	_hitAttack = false;
}

//-----------------------------------------------------------------------------
//! �_���[�W�v�Z
//-----------------------------------------------------------------------------
s32 EnemyBase::calcDamageAndCollision()
{
	// �_���[�W�ۑ��p
	s32 damage = 0;

	//---- �����蔻��
	for( s32 i=0; i<_myCollision.getHitCount(); ++i )
	{
		Collision* obj = _myCollision.getHitObj(i);
		// ���O���v���C���[�̍U���Ȃ�
		if( isPlayerAttack(obj) ){
			_hitAttack = true;
			damage += (s32)IObjDataManager()->_pPlayer->getStatus()->getParam()->_atkPow;

		// �v���C���[����(�G)�̍U���Ȃ�
		}else if( isAllyAttackerAttack(obj)  ){
			_hitAttack = true;
			damage += (s32)IObjDataManager()->_attackerStatus->getParam()->_atkPow;
		}else if( isAllyDeffenderAttack(obj) ){
			_hitAttack = true;
			damage += (s32)IObjDataManager()->_deffenderStatus->getParam()->_atkPow;
		}
	}

	return damage;
}

//-----------------------------------------------------------------------------
//! �t���[�Y���邩�ǂ���
//-----------------------------------------------------------------------------
bool EnemyBase::checkFreeze(const s32&	currentAnimation)
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

//-----------------------------------------------------------------------------
//! �_�������T���Đݒ�
//-----------------------------------------------------------------------------
bool EnemyBase::searchAndSetTarget()
{
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

	// �G(�v���C���[�̖���)�̃f�[�^���擾
	vector<AllyBase*> pAllyData = IObjDataManager()->_pAllyData;
	vector<AllyBase*>::iterator itr;
	f32 minLength = FLT_MAX;
	Man* target = nullptr;
	for( itr = pAllyData.begin(); itr != pAllyData.end(); itr++ )
	{
		AllyBase* ally = *itr;
		// ���S���Ă����珈�����Ȃ�
		if( ally->isDead() ) continue;
		// �������瑊��ւ̃x�N�g��
		f32 length = (ally->getPosition() - _position).squareLength();
		if( minLength > length )
		{
			// �X�V
			minLength = length;
			target	  = ally;
		}
	}
	Player* player = IObjDataManager()->_pPlayer;
	// �v���C���[�ւ̋���
	f32 toPlayerLength = (player->getPosition() - _position).squareLength();
	if( minLength > toPlayerLength )
	{
		// �X�V
		target = player;
	}

	if( target != nullptr ) {
		// ����p�^�[�Q�b�g�ɐݒ肵�Ă���
		_pPrevTarget = target;
		// ��ԋ߂�����^�[�Q�b�g�ɂ���
		_key->setTarget(target);
	}else{
		return false;
	}

	return true;
}
//=============================================================================
//	END OF FILE
//=============================================================================