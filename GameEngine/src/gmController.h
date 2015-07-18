//-----------------------------------------------------------------------------
//!
//!	@file	gmController.h
//!	@brief	�R���g���[���[
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�R���g���[���[
//-----------------------------------------------------------------------------
class Controller
{
public:

	//! �R�}���h�񋓑�
	//@{
    enum PAD_STATE{
		// �f�W�^��
		PAD_UP,				//	�\���L�[�̏�
		PAD_DOWN,			//	�\���L�[�̉�
		PAD_LEFT,			//	�\���L�[�̍�
		PAD_RIGHT,			//	�\���L�[�̉E
		PAD_START,			//	Start
		PAD_BACK,			//	Back
		PAD_LEFT_THUMB,		//	���X�e�B�b�N
		PAD_RIGHT_THUMB,	//	�E�X�e�B�b�N
		PAD_LB,				//	LB
		PAD_RB,				//	RB
        PAD_A,				//	A�{�^��
        PAD_B,				//	B�{�^��
        PAD_X,				//	X�{�^��
        PAD_Y,				//	Y�{�^��
		// �A�i���O
		PAD_LEFT_U_STICK,	//	���X�e�B�b�N�̏�
		PAD_LEFT_D_STICK,	//	���X�e�B�b�N�̉�
		PAD_LEFT_L_STICK,	//	���X�e�B�b�N�̍�
		PAD_LEFT_R_STICK,	//	���X�e�B�b�N�̉E
		PAD_RIGHT_U_STICK,	//	�E�X�e�B�b�N�̏�
        PAD_RIGHT_D_STICK,	//	�E�X�e�B�b�N�̉�
		PAD_RIGHT_L_STICK,	//	�E�X�e�B�b�N�̍�
        PAD_RIGHT_R_STICK,	//	�E�X�e�B�b�N�̉E
        PAD_LT,				//	���g���K�[
		PAD_RT,				//	�E�g���K�[

		PAD_MAX
    };
	//@}

	//!	���̓��[�h
	//@{
	enum MODE{
		DIGITAL,
		ANALOG,
	};
	//@}

public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//!	�R���X�g���N�^
    Controller(s32 playerNum);

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	void	Update();

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//!	�ڑ��m�F�֐�
    bool			IsConnected();
	//!	���̓`�F�b�N�֐�
	//!	@param	[in]	p_state	�p�b�h�̎��
	bool			GetPadState(PAD_STATE p_state);
	//! �������ꂽ���`�F�b�N
	//!	@param	[in]	p_state	�p�b�h�̎��
	virtual bool	GetPushState(PAD_STATE p_state);
	
	//!	�U���J�n
	//!	@param [in] L_vib	���U���l(0.0f ~ 1.0f)
	//!	@param [in] R_vib	�E�U���l(0.0f ~ 1.0f)
	void			EnableVibration( f32 L_vib, f32 R_vib );
	//! �U����~
	void			DisableVibration();

	//!	�A�i���O�l���擾
	//! @param	[in]	Thumb		true:���X�e�B�b�N false:�E�X�e�B�b�N
	//!	@param	[in]	KnockSize	�������� 0.0f�`1.0f
	virtual Vector3	getStickState(bool Thumb, f32& KnockSize);
	
	
protected :
	
	//! �f�W�^���̓��̓`�F�b�N
	//!	@param	[in]	state	XINPUT�̃p�b�h�̎��
	//!	@param	[in]	p_state	�p�b�h�̎��
	bool			GetDigitalState(XINPUT_STATE state, PAD_STATE p_state);
	//!	�A�i���O�̓��̓`�F�b�N
	//!	@param	[in]	state	XINPUT�̃p�b�h�̎��
	//!	@param	[in]	p_state	�p�b�h�̎��
	bool			GetAnalogState(XINPUT_STATE state, PAD_STATE p_state);
	//!	XINPUT�ɕϊ�
	//!	@param	[in]	p_state	�p�b�h�̎��
	void			To_XInput(PAD_STATE &p_state);
	//!	���̓��[�h�`�F�b�N�֐�
	//!	@param	[in]	p_state	�p�b�h�̎��
	void			CheckMode(PAD_STATE p_state);

	//@}

	s32				_controllerNum;			//!<	�ڑ��R���g���[���[��
	MODE			mode;					//!<	���̓��[�h

	s32				XInputPadState[14];		//!<	XInput�̃p�b�h�̗񋓂������Ă���
	bool			nowPadState[PAD_MAX];	//!<	���̃L�[
	bool			prevPadState[PAD_MAX];	//!<	�ЂƂO�̃p�b�h�̏��
};


//=============================================================================
//	END OF FILE
//=============================================================================