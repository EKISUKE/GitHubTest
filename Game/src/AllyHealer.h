//-----------------------------------------------------------------------------
//!
//!	@file	AllyHealer.h
//!	@brief	�����񕜃^�C�v
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

// ������myName���}�N���Ő錾
DEF_COL_NAME_HEADER(AllyHealer);
DEF_COL_ATTACK_NAME_HEADER(AllyHealerAttack);
//-----------------------------------------------------------------------------
//!	����(�񕜃^�C�v)
//-----------------------------------------------------------------------------
class	AllyHealer : public AllyBase
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	AllyHealer();
	//! �f�X�g���N�^
	virtual ~AllyHealer();

	//! ������
	virtual bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �q�[��
	//!	@param	[in]	target	�ڕW
	void			Heal(Man*	target);
	//! �X�V
	virtual void	Update();
	//! ���ߎ��̍X�V
	virtual void	UpdateByCommand();
	//! �v���C���[�̎���ɏ�ɂ���
	//!	@param	[in]	player	�v���C���[���W
	virtual void	UpdateNormal(Vector3& playerPos);
	
	//! �`��
	virtual void	Render(bool isShadow);

	//! �_�������T���Đݒ�
	//!	@return true:�G��ݒ芮�� false:�_���G�͂��Ȃ�
	virtual bool	searchAndSetTarget();

	//! �_���[�W�v�Z�Ɠ����蔻�菈��
	virtual s32		calcDamageAndCollision();
	//@}
private:
	s32				_SeverityMax;	//!< �ً}�x�ő�
	bool			_keyUpdate;		//!< �L�[�X�V
	bool			_isHeal;		//!< �񕜒�
	Man*			_target;		//!< �^�[�Q�b�g
};
//=============================================================================
//	END OF FILE
//=============================================================================