//-----------------------------------------------------------------------------
//!
//!	@file	KeyInput.cpp
//!	@brief	�L�[����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include"Library.h"

//=============================================================================
// �v���C���[����
//=============================================================================
//-----------------------------------------------------------------------------
//! �f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
KeyInput::KeyInput(Man* me, Man* leader)
: KeyBase(me, leader)
, Controller(1)
{
	_keyType = KEY_TYPE_INPUT;
	_vecKeyCommand.push_back( KeyComPtr(new KeyCommand("CommandChange")) );
}

//-----------------------------------------------------------------------------
//! �X�V
//-----------------------------------------------------------------------------
void KeyInput::Update()
{
	f32	KnockSize = 0.0f;
	// �X�e�B�b�N���̍X�V
	_movVector	 = Controller::getStickState(true, KnockSize);
	_stickVector = _movVector;

	
	// �L�[�ő���
	if( GetKeyState('W') & 0x80 )
	{
		_movVector._z =  1.0f;
	}
	if( GetKeyState('A') & 0x80 )
	{
		_movVector._x = -1.0f;
	}
	if( GetKeyState('S') & 0x80 )
	{
		_movVector._z = -1.0f;
	}
	if( GetKeyState('D') & 0x80 )
	{
		_movVector._x =  1.0f;
	}
	f32		stickRot = -1.0f;

	if( _movVector.length() > 0.1f ){
		stickRot = TO_DEGREE(atan2f(_movVector._z, _movVector._x) );
	}

	if( stickRot != -1.0f ){
		
		//// �͈͊O�w��
		//if( stickRot >=  180.0f )  stickRot -= 360.0f;
		//if( stickRot <= -180.0f )  stickRot += 360.0f;

		// �J�����R���g���[���[�̃V���O���g���ŌĂяo��
		CameraBase*	cam = GmCameraMan()->getCurrentCamera();
		// �J�����̊p�x�擾
		f32		camRot	 = cam->getRotation()._y;
		// �J�������猩��_movVector�̐i�ނׂ��p�x
		f32		Rot = TO_RADIAN(stickRot - camRot);
		// �L�[�{�[�h�œ��������Ƃ���KnockSize��-1
		if( KnockSize == 0.0f ) {
			// 1�ɂ��Ă���
			KnockSize = 1.0f;
		}
		_movVector._x = KnockSize * sinf(Rot);
		_movVector._z = KnockSize * cosf(Rot);
	}

	enum{
		ATTACK = (1 << 0),
		JUMP   = (1 << 1),
	};

	Mouse* pMouse = GmMouseMan()->getMouse();
	u32 key = 0;
	
	bool attack = GetPushState(PAD_X) | pMouse->isLeftDown();
	bool jump   = GetPushState(PAD_A) | ( (GetKeyState(VK_SPACE) & 0x80) != 0 );
	key |= (attack) ? ATTACK : 0;
	key |= (jump)   ? JUMP	 : 0;
	static u32 oldKey = 0;
	u32		   trigger = (key ^ oldKey) & key;
	

	// �{�^�����͂ɉ����ăR�}���h��Ԑؑ�
	static u32 oldTime = 0;
	u32 nowTime = timeGetTime();
	Status* myStatus = _me->getStatus();
	f32 delay = myStatus->getParam()->_atkDelay;
	// �҂����Ԃ��Ȃ����
	if( nowTime - oldTime > 100 *  delay) {
		// �U���t���O�����Ă���
		if( trigger & ATTACK )	{
			setCommandFlag("Attack", true ); 
			oldTime = nowTime;
		}else{ 
			setCommandFlag("Attack", false); 
		}
	}else{
		setCommandFlag("Attack", false);
	}

	if( trigger & JUMP  )		{ setCommandFlag("Jump"			 , true ); }
	else						{ setCommandFlag("Jump"			 , false); }
	if( GetPadState (PAD_RT) )	{ setCommandFlag("CommandChange" , true ); }
	else						{ setCommandFlag("CommandChange" , false); }
}
//=============================================================================
//	END OF FILE
//=============================================================================