//-----------------------------------------------------------------------------
//!
//!	@file	gmMouse.h
//!	@brief	�}�E�X����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once
//-----------------------------------------------------------------------------
//! �}�E�X����
//-----------------------------------------------------------------------------
class Mouse
{
public:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Mouse();

	//! �f�X�g���N�^
	virtual ~Mouse();


	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! 1�t���[�����̍X�V
	void		Update();
	
	//! �}�E�X�z�C�[��
	void		MouseWheel(s32 wheel_number, s32 direction, s32 x, s32 y);

	//! �}�E�X�֐�
	void		MouseFunc(s32 button_number, s32 state, s32 x, s32 y);
	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���݂̃J�[�\���ʒu���擾
	POINT		getPosition() const { return _point; }

	//! �J�[�\���ړ��ʂ��擾
	POINT		getOffset() const;
	
	//! ���N���b�N���Ă邩�ǂ���
	bool		isLeftDown  () const { return _leftButton; }
	//! �E�N���b�N���Ă邩�ǂ���
	bool		isRightDown () const { return _rightButton; }
	//! �z�C�[���N���b�N���Ă��邩�ǂ���
	bool		isMiddleDown() const { return _middleButton; }

	

	//! �z�C�[���ύX�ʎ擾
	f32			getChangeWheel(){ 
		f32 result = _changeWheel;
		// �擾������O�ɂ���
		_changeWheel = 0.0f;
		return result; 
	}
	
	//@}
private:

	POINT		_oldPoint;			//!< 1�t���[���O�̈ʒu���W
	POINT		_point;				//!< ���݂̈ʒu���W

	//---- �}�E�X�{�^��
	bool		_leftButton;		//!< ���̃{�^���̏��
	bool		_rightButton;		//!< �E�̃{�^���̏��
	bool		_middleButton;		//!< �����̃{�^���̏��
	//---- �}�E�X�z�C�[��
	s32			_oldWheel;			//!< 1�t���[���O�̃z�C�[���l
	s32			_wheel;				//!< ���݂̃z�C�[���l
	f32			_changeWheel;		//!< �z�C�[���ύX��
};

//=============================================================================
//	END OF FILE
//=============================================================================