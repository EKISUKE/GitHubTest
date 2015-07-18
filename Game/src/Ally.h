//-----------------------------------------------------------------------------
//!
//!	@file	Ally.h
//!	@brief	�������
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�������
//-----------------------------------------------------------------------------
class AllyBase : public Man
{
public:
	
	//----------------------------------------------------------
	//! @name ������
	//----------------------------------------------------------
	//@{

private:
	//! �f�t�H���g�R���X�g���N�^
	AllyBase();
public:
	//! �R���X�g���N�^
	//!	@param	[in]	myType	�L�[�^�C�v
	AllyBase(Man::KEY_TYPE myType);

	//! �f�X�g���N�^
	virtual ~AllyBase();

	//!	������
	virtual bool	Initialize();

	//! �A�j���[�V����������
	void			initAnimation();
	
	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V(���̒��ōX�V���ׂ�Update���Ăяo��)
	virtual void	Update();
	//! ���ߎ��̍X�V
	virtual void	UpdateByCommand() = 0;
	//! �v���C���[�̎���ɏ�ɂ���
	//!	@param	[in]	player	�v���C���[���W
	virtual void	UpdateNormal(Vector3& playerPos) = 0;
	//!	�`��
	virtual void	Render(bool isShadow);

	//! glut�`��֘A
	virtual void	glutRender();
	//! �f�o�b�O�`��
	virtual void	debugRender();

	//! �X�e�[�^�X�̕`��(2D)
	virtual void	StatusRender();

	//! �U��
	//virtual void	Attack();

protected:
	//! ���W�ݒ肵�Ȃ���
	void			ResetPosition();

	//! �_�������T���Đݒ�
	//!	@return true:�G��ݒ芮�� false:�_���G�͂��Ȃ�
	virtual bool	searchAndSetTarget();

	//! �_���[�W�v�Z�Ɠ����蔻�菈��
	virtual s32		calcDamageAndCollision(){ return 0; }

	//! �A�j���[�V�����ɂ��ړ��s�`�F�b�N
	virtual bool	checkFreeze(const s32& currentAnimation);
		
	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{
public:
	//! ���[�_�[���W��ݒ�
	//!	@param	[in]	pos	���[�_�[���W
	void			setLeaderPos(Vector3& pos) { _leaderPos = pos; }

	//! ������ݒ�
	void			setSquad(Squad*	squad)	   { _pSquad = squad; } 
	//@}
protected:
	//ALLY_TYPE		_allyType;
	//Collision	_myCollision;		//!< �Փ˔���p
	Collision		_attackCollision;	//!< �U������p
	Vector3			_leaderPos;			//!< ���[�_�[�̍��W
	Vector3			_DebugGoalPos;		//!< �ڕW���W(�f�o�b�O�p)
	Squad*			_pSquad;			//!< �����̕���
	Man*			_pPrevTarget;		//!< ��O�̃^�[�Q�b�g���
	bool			_goGoal;			//!< �S�[���֌������t���O

	//---- �A�j���[�V��������
	s32				_aniMove;
	s32				_aniAttack;
	s32				_aniWait;
	s32				_aniDamage;
	s32				_aniHeal;
};


//=============================================================================
//	END OF FILE
//=============================================================================