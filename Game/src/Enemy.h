//-----------------------------------------------------------------------------
//!
//!	@file	Enemy.h
//!	@brief	�G���
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------
//! �G���
//-----------------------------------------------------------------------------
class EnemyBase : public Man
{
public:
	//----------------------------------------------------------
	//! @name ������
	//----------------------------------------------------------
	//@{
private:
	//! �f�t�H���g�R���X�g���N�^
	EnemyBase();
public:
	//! �R���X�g���N�^
	//!	@param	[in]	myType	�L�[�^�C�v
	EnemyBase(Man::KEY_TYPE myType);

	//! �f�X�g���N�^
	virtual ~EnemyBase();

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
	//! ���[�_�[�̎���ɏ�ɂ���
	//!	@param	[in]	leader	���[�_�[���W
	virtual void	UpdateNormal(Vector3& leaderPos) = 0;
	//!	�`��
	virtual void	Render(bool isShadow);

	//! glut�`��֘A
	virtual void	glutRender();
	//! �f�o�b�O�`��
	virtual void	debugRender();

	//! �X�e�[�^�X�̕`��(2D)
	virtual void	StatusRender();

protected:
	//! ���W�ݒ肵�Ȃ���
	void			ResetPosition();
	//! �_���[�W�v�Z
	virtual s32		calcDamageAndCollision();
	//! �A�j���[�V�����ɂ��ړ��s�`�F�b�N
	virtual bool	checkFreeze(const s32&	currentAnimation);

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{
public:
	//! ���[�_�[���W��ݒ�
	//!	@param	[in]	pos	���[�_�[���W
//	void			setLeaderPos(Vector3& pos) { _leaderPos = pos; }

	//! ������ݒ�
	void			setSquad(Squad*	squad)	   { _pSquad = squad; } 

protected:
	//! �_�������T���Đݒ�
	virtual bool	searchAndSetTarget();
	//@}

	Collision		_attackCollision;	//!< �U������p
	Vector3			_goalPos;			//!< �ڕW���W
	Squad*			_pSquad;			//!< �����̕�����
	Man*			_pPrevTarget;		//!< ��O�̃^�[�Q�b�g���
	bool			_goGoal;			//!< �S�[���֌������t���O
	f32				_modelScale;		//!< ���f���T�C�Y(�L�����T�C�Y�Ɋ|���镪)

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