//-----------------------------------------------------------------------------
//!
//!	@file	gmController.cpp
//!	@brief	�R���g���[���[
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"



//-----------------------------------------------------------------------------
//!	�R���X�g���N�^
//-----------------------------------------------------------------------------
Controller::Controller(s32 playerNum)
{
	_controllerNum = playerNum -1;

	int	Index = 0;
	
	for( s32 i=0; i<16; i++ ){
		// �z��ԍ��p�̕ϐ�

		if( i == 10 || i == 11 ){
			continue;
		}

		XInputPadState[Index] = 0x001 << i;

		// ���̔ԍ���
		Index++;
	}

	// �������Ă���L�[�ƑO�̃t���[���ł̃L�[������
	for( s32 i=0; i<PAD_MAX; i++ ){
		prevPadState[i] = false;
		nowPadState[i]  = false;
	}
}

//-----------------------------------------------------------------------------
//!	�X�V
//-----------------------------------------------------------------------------
void Controller::Update()
{
}

//-----------------------------------------------------------------------------
//!	�ڑ��m�F
//-----------------------------------------------------------------------------
bool Controller::IsConnected()
{
	XINPUT_STATE state;	// �R���g���[�����擾�p
	// ������
	ZeroMemory( &state, sizeof(XINPUT_STATE) );

	// ��Ԏ擾
	DWORD Result = XInputGetState(_controllerNum, &state);

    if(Result == ERROR_SUCCESS){
        return true;	// �ڑ�����Ă��܂��B
    }else{
        return false;	// �ڑ�����Ă��܂���B
    }
}

//-----------------------------------------------------------------------------
//!	���̓`�F�b�N
//!	@param	[in]	p_state	�p�b�h�̎��
//-----------------------------------------------------------------------------
bool Controller::GetPadState(PAD_STATE p_state)
{
	XINPUT_STATE state;	// �R���g���[�����擾�p
    ZeroMemory( &state, sizeof(XINPUT_STATE) );	// ������
    DWORD dwResult;	// �֐����ʔ���p
	// ���擾
    dwResult = XInputGetState(_controllerNum,&state);
    if(dwResult == ERROR_SUCCESS){	//	��񂪂Ƃꂽ��
		CheckMode(p_state);	//	�A�i���O���[�h���f�W�^�����`�F�b�N
		//	���[�h�ɂ���ē��̓`�F�b�N�ؑ�
		switch(mode){
		case ANALOG:	//	�A�i���O
			if(GetAnalogState(state,p_state)){return true;}
			break;
		case DIGITAL:	//	�f�W�^��
			if(GetDigitalState(state,p_state)){return true;}
			break;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//!	�������ꂽ���`�F�b�N
//!	@param	[in]	p_state	�p�b�h�̎��
//-----------------------------------------------------------------------------
bool Controller::GetPushState(PAD_STATE p_state)
{
	// �Ȃ����Ă��Ȃ������珈�����Ȃ�
	if( !IsConnected() ) return false;

	// �O�̃t���[���̃L�[������
	prevPadState[p_state] = nowPadState[p_state];
	// ���݂̏�Ԃ̂��擾
	nowPadState[p_state] = GetPadState(p_state);
	//	�O��̂��̃{�^���̏�Ԃ�������ĂȂ����
	if((prevPadState[p_state] ^ nowPadState[p_state]) & nowPadState[p_state] ) 
	{
		return true;
	}
	
	return false;
}




//-----------------------------------------------------------------------------
//! �f�W�^���̓��̓`�F�b�N
//!	@param	[in]	state	XINPUT�̃p�b�h�̎��
//!	@param	[in]	p_state	�p�b�h�̎��
//-----------------------------------------------------------------------------
bool Controller::GetDigitalState(XINPUT_STATE state, PAD_STATE p_state)
{
	To_XInput(p_state);	//	XINPUT�ɕϊ�
	//	���̓`�F�b�N
	if(state.Gamepad.wButtons & p_state){return true;}	
	return false;
}

//-----------------------------------------------------------------------------
//!	�A�i���O�̓��̓`�F�b�N
//!	@param	[in]	state	XINPUT�̃p�b�h�̎��
//!	@param	[in]	p_state	�p�b�h�̎��
//-----------------------------------------------------------------------------
bool Controller::GetAnalogState(XINPUT_STATE state, PAD_STATE p_state)
{
	// �g���K�[�̓��͖����͈͐ݒ�
	int TRIGGER_DEADZONE = XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	//	�X�e�B�b�N�̓��͖�������ݒ�
	int L_STICK_THUMB_DEAD = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	int R_STICK_THUMB_DEAD = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	//	���͂̏�Ԃɂ���ē��̓`�F�b�N�ؑ�
	switch(p_state){
	//	�g���K�[
	case PAD_LT:
		if(state.Gamepad.bLeftTrigger  > TRIGGER_DEADZONE){return true;}
		break;
	case PAD_RT:
		if(state.Gamepad.bRightTrigger > TRIGGER_DEADZONE){return true;}	
		break;
	//	���X�e�B�b�N
	case PAD_LEFT_U_STICK:
		if(state.Gamepad.sThumbLY >  L_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_LEFT_D_STICK:
		if(state.Gamepad.sThumbLY < -L_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_LEFT_R_STICK:
		if(state.Gamepad.sThumbLX >  L_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_LEFT_L_STICK:
		if(state.Gamepad.sThumbLX < -L_STICK_THUMB_DEAD ){return true;}
		break;
	//	�E�X�e�B�b�N
	case PAD_RIGHT_U_STICK:
		if(state.Gamepad.sThumbRY >  R_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_RIGHT_D_STICK:
		if(state.Gamepad.sThumbRY < -R_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_RIGHT_R_STICK:
		if(state.Gamepad.sThumbRX >  R_STICK_THUMB_DEAD ){return true;}
		break;
	case PAD_RIGHT_L_STICK:
		if(state.Gamepad.sThumbRX < -R_STICK_THUMB_DEAD ){return true;}
		break;
	}
	return false;
}

//-----------------------------------------------------------------------------
//!	�A�i���O�l���擾
//! @param	[in]	Thumb	true:���X�e�B�b�N false:�E�X�e�B�b�N
//!	@param	[in]	KnockSize	�������� 0.0f�`1.0f
//-----------------------------------------------------------------------------
Vector3	Controller::getStickState(bool Thumb, f32& KnockSize)
{
	// �Ȃ����Ă��Ȃ������珈�����Ȃ�
	if( !IsConnected() ) return Vector3( 0.0f, 0.0f, 0.0f );

	//	�X�e�B�b�N�̓��͖�������ݒ�
	int L_STICK_THUMB_DEAD = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	int R_STICK_THUMB_DEAD = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	/*int L_STICK_THUMB_DEAD = 32768;
	int R_STICK_THUMB_DEAD = 32768;*/

	Vector3 dir(0.0f, 0.0f, 0.0f);
	XINPUT_STATE	state;
	ZeroMemory( &state, sizeof(XINPUT_STATE) );	// ������
	DWORD dwResult;	// �֐����ʔ���p
	dwResult = XInputGetState(_controllerNum,&state);

    if(dwResult == ERROR_SUCCESS){	//	��񂪂Ƃꂽ��
		// true�Ȃ獶
		if( Thumb ){

			Vector3 StickL(0.0f,0.0f,0.0f);
			StickL._x = state.Gamepad.sThumbLX;
			StickL._z = state.Gamepad.sThumbLY;
			s32 LX = (s32)( abs(StickL._x) );
			s32 LY = (s32)( abs(StickL._z) );

			if( LX >= L_STICK_THUMB_DEAD || LY >= L_STICK_THUMB_DEAD){
				dir = StickL;
			}
		}
		else
		// false�Ȃ�E
		{
			Vector3 StickR(0.0f,0.0f,0.0f);
			StickR._x = state.Gamepad.sThumbRX;
			StickR._z = state.Gamepad.sThumbRY;
			s32 RX = (s32)( abs(StickR._x) );
			s32 RY = (s32)( abs(StickR._z) );

			if( RX >= R_STICK_THUMB_DEAD || RY >= R_STICK_THUMB_DEAD){
				dir = StickR;
			}
			
		}
	}
	// ���������̌v�Z
	if( dir != 0.0f ) {
		KnockSize = LinearInterpolation(0.0f, 1.0f, dir.length(), 32767.0f);
		KnockSize = pow(KnockSize, 2.0f);
	}else{
		KnockSize = 0.0f;
	}

	return dir;
}



//-----------------------------------------------------------------------------
//!	XINPUT�ɕϊ�
//!	@param	[in]	p_state	�p�b�h�̎��
//-----------------------------------------------------------------------------
void Controller::To_XInput(PAD_STATE &p_state)
{
	if( p_state >= 10 ){ 
		p_state = (PAD_STATE)(p_state + 2);
	}
	p_state = (PAD_STATE)(0x001 << p_state);
}

//-----------------------------------------------------------------------------
//!	���̓��[�h�`�F�b�N
//!	@param	[in]	p_state	�p�b�h�̎��
//-----------------------------------------------------------------------------
void Controller::CheckMode(PAD_STATE p_state)
{

	int stateNum	   = p_state;
	int LastDigitalNum = PAD_Y;

	if( stateNum > LastDigitalNum ){
		mode = ANALOG;
	}else{
		mode = DIGITAL;
	}
}

//-----------------------------------------------------------------------------
//!	�U���J�n
//!	@param [in] L_vib	���U���l(0.0f ~ 1.0f)
//!	@param [in] R_vib	�E�U���l(0.0f ~ 1.0f)
//-----------------------------------------------------------------------------
void Controller::EnableVibration( f32 L_vib, f32 R_vib )
{
	//	�U���l��0.0~1.0f���� 0 ~ 65535�ɕϊ�
	L_vib = 65535 * L_vib;
	R_vib = 65535 * R_vib;
	//	�U���p�̍\����
	XINPUT_VIBRATION	vibration;
	//	������
	ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
	//	�U���l���
	vibration.wLeftMotorSpeed  = (WORD)( L_vib );
	vibration.wRightMotorSpeed = (WORD)( R_vib );
	//	���ʓ]��
	XInputSetState( _controllerNum, &vibration);
}
//-----------------------------------------------------------------------------
//!	�U����~
//-----------------------------------------------------------------------------
void Controller::DisableVibration()
{
	//	�U���p�̍\����
	XINPUT_VIBRATION	vibration;
	//	������
	ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
	//	�U���l���
	vibration.wLeftMotorSpeed  = 0;
	vibration.wRightMotorSpeed = 0;
	//	���ʓ]��
	XInputSetState( _controllerNum, &vibration);
}




//=============================================================================
//	END OF FILE
//=============================================================================