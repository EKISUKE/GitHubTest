//-----------------------------------------------------------------------------
//!
//!	@file	Player.cpp
//!	@brief	�v���C���[
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"



// ������myName���}�N���Ő錾
DEF_COL_NAME( Player )
DEF_COL_ATTACK_NAME(PlayerAttack);

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
Player::Player()
: Man(Man::KEY_INPUT, Man::TYPE_PLAYER)
, _commandChangeMode(false)
, _gameOver			(false)
, _isNearCastle		(false)
{
	// �����蔻�菉����
	_myCollision.setCollisionState(&_hitSphere, myName);
	_attackCollision.setCollisionState(&_attackSphere, myAttackName);

	loadModel("../Model/Player/Player.x", 5.0f);
	
	loadHPOutLineTexture("../Image/UI/HPBar.png");

	// �L�[�̍쐬
	if( !createKey(this, this) )
	{
		MessageBoxA(NULL, "�v���C���[�̃L�[�̏������Ɏ��s���܂����B", "�G���[", MB_OK);
	}

	// ������
	Initialize();
}
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
Player::~Player()
{
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool Player::Initialize()
{
		
	_command = COMMAND_NONE;
	// ���W�̏�����
	_position._x = -3200.0f;
	_position._y = 0.0f;
	_position._z = -2600.0f;
	
	_rotation	 = Vector3(0.0f, 50.0f, 0.0f);

	// ������
	Man::Initialize();

	// �X�e�[�^�X������
	_myStatus->setParam(StatusAssets::STATUS_PLAYER);
	// �U�����̔��a�ݒ�
	_attackRadNum		= _radius * 0.5f;
	_attackAABB._min	= -Vector3(_radius * 0.5f);
	_attackAABB._max	=  Vector3(_radius * 0.5f);
	_attackOffsetRange	= _attackRadNum * 1.5f;

	// �Q�[���I�[�o�[�t���OOFF
	_gameOver			= false;
	return true;
}


Vector3 drawPointPos;

//-----------------------------------------------------------------------------
//! �X�V����
//-----------------------------------------------------------------------------
void Player::Update()
{
	// �X�V
	_key->Update();


	if( GetKeyState('R') & 0x80 ) {
		_position._x = 1300.0f;
		_position._y = 1000.0f;
		_position._z = 760.0f;
	}

	//---- �L��������
	//---- �ړ�����
	_speed = 0.0f;
	// �R���g���[���[�̃X�e�B�b�N�̊p�x�����߂�
	Vector3	StickState	= _key->getVector();
		

	// ���ߕύX�{�^����������Ă���ΕύX���[�h��(��Ŏg�p����)
	//if( _key->getPushState(KeyBase::KEY_COMMAND_COMMANDCHANGE) ) 
	/*if( _key->getPushState("CommandChange") )
	{
		_commandChangeMode = true;
	}else{
		_commandChangeMode = false;
	}*/
	

	////----�@�X�V���[�h�ɂ���čX�V�Ăѕ���(��Ŏg�p����)
	//if( _commandChangeMode ) {
	//	UpdateCommandChange(_key->getStickVector());
	//}else{
	//	UpdateNormal(StickState);
	//}

	
	if( _isDead ) {
		UpdateGameOver();
	}else{
		UpdateNormal(StickState);

		// �X�V
		_oldPosition = _position;

		// �U�����ĂȂ�������
		if( !checkFreeze(_aniState) ) {
			// �ړ����ĂȂ����
			if( _mov._x == 0.0f && _mov._z == 0.0f )
			{
				// �������[�V����
				_aniState = STATE_WAIT;
				if( _pTaskModel->getAnimation() != STATE_WAIT ) {
					_pTaskModel->setAnimationSpeed(1.0f);
					_pTaskModel->setAnimation(_aniState, ANIMATION_PLAY_REPEAT);
				}
			}else{
				// �������[�V����
				_aniState = STATE_MOVE;
				f32 knock = StickState.length();
				_pTaskModel->setAnimationSpeed(knock);
				if( _pTaskModel->getAnimation() != STATE_MOVE ) {
					_pTaskModel->setAnimation(_aniState, ANIMATION_PLAY_REPEAT);
					//_pTaskModel->setAnimation(_aniState, ANIMATION_PLAY_DEFAULT);
				}
			}
		}

	

		// �U�����Ă���
		if( _aniState == STATE_ATTACK ) {
			// �ړ����x�����Ȃ�����
			_mov._x *= 0.5f;
			_mov._z *= 0.5f;
		}else if( _aniState == STATE_DAMAGE ){
			// �_���[�W���󂯂Ă�����
			// �ړ����x�����Ȃ�����
			_mov._x *= 0.1f;
			_mov._z *= 0.1f;
		}


		// �ړ�
		_position += _mov * Global::deltaTime;

		// �u�Ԉړ��o�O���o�p
		if( (_oldPosition - _position).length() >= 1000.0f ) {
			s32 a = 0;
		}

		//---- �p�[�e�B�N��
		// �p�[�e�B�N���̐����҂����ԃJ�E���g
		_genCount += 1 * Global::deltaTime;

		Vector3 sub = _oldPosition - _position;

		// �ړ����Ă��銎�A�����҂����Ԃ��؂ꂽ��
		if( (abs(sub._x) > 0.0f || abs(sub._z) > 0.0f) 
			&& _genCount >= _waitGenTime 
			&& _hitGround) {

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

		// �����蔻��ƃ_���[�W�v�Z
		s32 damage = calcDamageAndCollision();

		// �_���[�W����������
		if( damage > 0 ) {
			Damage(damage);
			// �_���[�W�A�j���[�V������
			_aniState = STATE_DAMAGE;
			_pTaskModel->setAnimationSpeed(1.0f);
			_pTaskModel->setAnimation(_aniState);
		}else if( _aniState == STATE_DAMAGE ){
			_isDamage = false;
			// ���ݍĐ����̃A�j���[�V�����ɂ���(�Đ����I����Ă����STATE_NONE)
			_aniState = (ANIMATION_STATE)_pTaskModel->getAnimation();
		}
	
	

		//---- �n�ʂƂ̓����蔻��
	#if 1 
		GmSystemCollision()->checkCollisionModel(_hitPosition, _radius, _hitGround);
	#else
		if( _position._y <= 0.0f )
		{
			_hitGround = true;
		}else{
			_hitGround = false;
		}
	#endif

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
		// �`����W�ύX
		drawPointPos = GmSystemCollision()->getLastHitPosForWall();

	

		// �d�͏���
		if( !_hitGround )
		{
			_mov._y -= 0.6f * Global::deltaTime;
		}
		else{
			_mov._y = 0.0f;
			_isJump = false;
			// ���ݍĐ����̃A�j���[�V�����ɂ���(�Đ����I����Ă����STATE_NONE)
			_aniState = (ANIMATION_STATE)_pTaskModel->getAnimation();
		}

		// �ő�d�͐ݒ�
		if( _mov._y <= -9.8f )
		{
			_mov._y = -9.8f; 
		}
	
		_pTaskModel->update();

		if(_myStatus->getParam()->_HP <= 0 ){
			_isDead = true;
		}
	}		// isDead == false

	CameraBase* pCamera = GmCameraMan()->getCurrentCamera();

	if(_isNearCastle) {
		// ���݂̒����_�Ƃ̋������擾
		f32 lookAtLength = pCamera->getLookAtLength();
		static const f32 maxLookAtLength = 600.0f;
		if(lookAtLength < maxLookAtLength) {
			pCamera->changeLookAtLength(4.0f);
		}
	}else{
		pCamera->setLookAtLengthMax(200.0f);
	}
}


//-----------------------------------------------------------------------------
//! �ʏ펞�X�V
//-----------------------------------------------------------------------------
void Player::UpdateNormal(Vector3& stickState)
{

	// �X�e�B�b�N���삪��������(0.0�ȊO�̂Ƃ�)�̓v���C���[�̊p�x�Ɋ��蓖�Ă�
	if( abs( TO_DEGREE(atan2f( stickState._x, stickState._z )) )  > 0.0f ){
			_rotation._y = TO_DEGREE(atan2f( stickState._x, stickState._z ) );
			// ���x�ݒ�
			//_speed = _myStatus->getInfOfStatus(Status::STATUS_SPEEDMAX);
			Status::Param* myParam = (Status::Param*) _myStatus->getParam();
			_speed = myParam->_speedMAX;
	}
	

	// �ړ��ʐݒ�
	_mov._x = _speed * stickState._x;
	_mov._z = _speed * stickState._z;


	//---- �W�����v����
	//if( _key->getPushState(KeyBase::KEY_COMMAND_JUMP) ) 
	if( _key->getPushState("Jump") )
	{
		if( !_isJump ) {
			_aniState = STATE_JUMP;
			_pTaskModel->setAnimationSpeed(1.0f);
			_pTaskModel->setAnimation(_aniState);
		}
		Jump();
		
	}


	//---- �U������
	if( _key->getPushState("Attack") )
	{
		// �A�j���[�V�������U���Ɉڍs
		_aniState = STATE_ATTACK;
		// �U���A�j���[�V�����Đ�
		_pTaskModel->setAnimationSpeed(1.0f);
		_pTaskModel->setAnimation(_aniState);
	}else if ( _pTaskModel->getAnimation() == STATE_ATTACK ) {
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
		// ���ݍĐ����̃A�j���[�V�����ɂ���(�Đ����I����Ă����STATE_NONE)
		_aniState = (ANIMATION_STATE)_pTaskModel->getAnimation();
	}

	//---- �f�o�b�O�p�����蔻�薳����
	//if( control.GetPadState(control.PAD_RT) ){
	//_hitObj = false;
		
		
	/*if( _hitObj ){
		_mov._x = 0.0f;
		_mov._y = 0.0f;
		_mov._z = 0.0f;
	}*/
	
}

//-----------------------------------------------------------------------------
//! ���ߕύX���X�V
//-----------------------------------------------------------------------------
void Player::UpdateCommandChange(Vector3& stickState)
{
	// ���X�e�B�b�N�Ŗ��ߕύX
	Vector3 dir = stickState;

	dir.normalize();

	if( dir._z >  0.7f )
	{
		_command = COMMAND_ATTACK;
	}

	if( dir._z < -0.7f ) 
	{
		_command = COMMAND_BACK;
	}

	if( dir._z == 0.0f ){
		_command = COMMAND_NONE;
	}

}

//-----------------------------------------------------------------------------
//! �Q�[���I�[�o�[���̍X�V
//-----------------------------------------------------------------------------
void Player::UpdateGameOver()
{
	static bool isGameOverAnimation = false;
	if( _aniState != STATE_GAMEOVER ){
		_aniState = STATE_GAMEOVER;
		_pTaskModel->setAnimationSpeed(1.0f);
		_pTaskModel->setAnimation(_aniState);
		isGameOverAnimation = true;
	}
	// �Q�[���I�[�o�[�̃A�j���[�V�������n�܂��Ă�
	if( isGameOverAnimation ){
		if( _pTaskModel->getAnimation() == STATE_NONE ){
			_gameOver = true;
		}
	}

	_pTaskModel->update();
}


//-----------------------------------------------------------------------------
//! �`�揈��
//-----------------------------------------------------------------------------
void Player::Render(bool isShadow)
{
	static float debugRotation = 0.0f;

	debugRotation += 0.01f;

	// �ړ��p�}�g���b�N�X
	Matrix	offsetMatrix(_pTaskModel->getWorldMatrix());
	offsetMatrix = offsetMatrix.translate(_position);
	Matrix rotateMatrix(_pTaskModel->getRotateMatrix());
	rotateMatrix = rotateMatrix.rotateY(Degree(_rotation._y) );
	//rotateMatrix = rotateMatrix.rotateY(debugRotation);
	_pTaskModel->setWorldMatrix(offsetMatrix);
	_pTaskModel->setRotateMatrix(rotateMatrix);

	_pTaskModel->render(isShadow);

}

//-----------------------------------------------------------------------------
//! glut�`��֘A
//-----------------------------------------------------------------------------
void Player::glutRender()
{

	// ��
	//GLfloat red[]	 = { 1.0f, 0.0f, 0.0f, 0.3f };
	GLfloat red[]	 = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat blue[]	 = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat white[]	 = { 1.0f, 1.0f, 1.0f, 1.0f };

	//// �f�o�b�N�\��
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
	//_myCollision.DebugRender();
	// �U���̃r�W���A����
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	_attackCollision.DebugRender();

	CommandRender();
}

//-----------------------------------------------------------------------------
//! �f�o�b�O�`��
//-----------------------------------------------------------------------------
void Player::debugRender()
{
	drawAABB( _colAABB, Vector4(1.0f, 1.0f, 1.0f, 1.0f), _position );
	if( _isAttack ) {
		drawAABB( _attackAABB, Vector4(1.0f, 0.0f, 0.0f, 1.0f), _attackPosition );
	}
}

//-----------------------------------------------------------------------------
//! ���ߗp�`��
//-----------------------------------------------------------------------------
void Player::CommandRender()
{
	if( !_commandChangeMode ) return;
	
	GLfloat red[]	 = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat blue[]	 = { 0.0f, 0.0f, 1.0f, 1.0f };




	Vector3 CamPos  = GmCameraMan()->getCurrentCamera()->getPosition();
	Vector3 CamLook = GmCameraMan()->getCurrentCamera()->getLookAt();
	Vector3	WorldUp	= GmCameraMan()->getCurrentCamera()->getWorldUp();

	Vector3 axisZ	= (CamLook - CamPos).normalize();
	Vector3	axisX	= Vector3::cross(WorldUp, axisZ).normalize();
	Vector3	axisY	= Vector3::cross(axisZ, axisX).normalize();

	f32		CamRot	= GmCameraMan()->getCurrentCamera()->getRotation()._y;

	
	// ��̍��W
	Vector3	BasePos;
	// �U���̋��̍��W
	Vector3	position1;
	// �P�ނ̋��̍��W
	Vector3	position2;

	axisZ = axisZ.normalize();

	// �J�����̌��Ă��̕�����100�����L�т����W
	// ���̍��W������ʂ��炵�ĕ`��
	BasePos._x	= CamPos._x + axisZ._x * 100.0f;
	BasePos._y	= CamPos._y + axisZ._y * 100.0f;
	BasePos._z	= CamPos._z + axisZ._z * 100.0f;
	
	position1	= BasePos;
	position2	= BasePos;

	position1._y += axisY._y * 30.0f;

	position2._y -= axisY._y * 30.0f;

	f32	radius1 = 10.0f;
	f32	radius2 = 10.0f;

	if( _command == COMMAND_ATTACK )
	{
		radius1 = 16.0f;
	}else if( _command == COMMAND_BACK )
	{
		radius2 = 16.0f;
	}

	// �U���p�̋�
	glPushMatrix();
		glTranslatef(position1._x, position1._y, position1._z);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		setBlendMode(BM_ADD_ALPHA);		//================================ �y�����������J�n�z
		glutSolidSphere(radius1,16,16);
		setBlendMode(BM_NONE);			//================================ �y�����������I���z
	glPopMatrix();

	// �P�ޗp�̋�
	glPushMatrix();
		glTranslatef(position2._x, position2._y, position2._z);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
		setBlendMode(BM_ADD_ALPHA);		//================================ �y�����������J�n�z
		glutSolidSphere(radius2,16,16);
		setBlendMode(BM_NONE);			//================================ �y�����������I���z
	glPopMatrix();


	
}


//-----------------------------------------------------------------------------
//! �X�e�[�^�X�̕`��(2D)
//-----------------------------------------------------------------------------
void Player::StatusRender()
{
	// �̗͂�΂ŕ\��
	glColor3f(0,255,0);
	// �̗͂̕`��
	Vector3 drawPos(100.0, 100.0f, 0.0f);
	Color gaugeColor(0,255,0);
	// �̗̓Q�[�W�̕`��
	_myStatus->Render(true, gaugeColor, drawPos, _hpOutLine);
}

//-----------------------------------------------------------------------------
//! �_���[�W�v�Z�Ɠ����蔻�菈��
//-----------------------------------------------------------------------------
s32 Player::calcDamageAndCollision()
{
	s32 damage = 0;
	//---- �����蔻��
	for( s32 i=0; i<_myCollision.getHitCount(); ++i )
	{
		Collision* obj = _myCollision.getHitObj(i);
		//// ���O���G�Ȃ�
		//if( isEnemy(obj) ){
		//	// �␳�ʒu���擾����
		//	Vector3	dir = _myCollision.getOffsetPos(obj);
		//	// �␳�ʒu�ɐݒ�
		//	_hitPosition = dir;
		//	// �����蔻��p�̋����X�V
		//	setSphereParam(_hitPosition, _hitRadius);
		//}else if( isEnemyAttack(obj) ) {
		if( isEnemyLeaderAttack(obj) || isEnemyAttakerAttack(obj) || isEnemyDeffenderAttack(obj) ){
			// �_���[�W����(�h��X�e�[�^�X���l�����ă_���[�W�v�Z���s���\��)
			damage += (s32)( IObjDataManager()->_pEnemyData[0]->getStatus()->getParam()->_atkPow * 0.4f );
		}
		// ���W�̐ݒ�
		toPosition(_hitPosition, _position);
	}
	return damage;
}

//-----------------------------------------------------------------------------
//! �A�j���[�V�����ɂ��ړ��s�`�F�b�N
//-----------------------------------------------------------------------------
bool Player::checkFreeze(const s32&	currentAnimation)
{
	// �A�j���[�V�������Ȃ����
	if( currentAnimation != -1 ){
		// �U�����Ȃ�t���[�Y
		if( currentAnimation == STATE_ATTACK ) return true;
		// �_���[�W���Ȃ�t���[�Y
		if( currentAnimation == STATE_DAMAGE ) return true;
		// �W�����v���Ȃ�t���[�Y
		if( currentAnimation == STATE_JUMP   ) return true;
	}
	// �����܂ł�����t���[�Y���Ȃ�
	return false;
}
//=============================================================================
//	END OF FILE
//=============================================================================