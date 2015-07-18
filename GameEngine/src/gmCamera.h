//-----------------------------------------------------------------------------
//!
//!	@file	Camera.h
//!	@brief	�J��������
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//!	�J����
//-----------------------------------------------------------------------------
class Camera : public CameraBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Camera();

	//! ���z�f�X�g���N�^
	virtual ~Camera(){};

	//! ������
	virtual bool	init(Vector3& lookAt, Vector3& _rot);

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V
	//!	@param	[in]	control		�R���g���[���[
	//!	@param	[in]	PlayerPos	�v���C���[���W
	//!	@param	[in]	mouseOffset	�}�E�X���W
	void			update(Controller& control, Vector3& PlayerPos,  POINT mouseOffset);
	//! �f�o�b�O�\��
	void			DebugRender();
	
	//@}
private:
	bool			_hitGround;	//!< �n�ʂƃq�b�g�`�F�b�N
	s32				_myLandScapeGridNum;	//!< �����h�X�P�[�v�p�O���b�h�ԍ�
};

//=============================================================================
//	END OF FILE
//=============================================================================