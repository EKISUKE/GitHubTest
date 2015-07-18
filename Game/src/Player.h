//-----------------------------------------------------------------------------
//!
//!	@file	Player.h
//!	@brief	�v���C���[
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//! ���O�ƈ�v���邩�ǂ���
//bool	isPlayer(Collision* obj);
DEF_COL_NAME_HEADER(Player);
DEF_COL_ATTACK_NAME_HEADER(PlayerAttack);

//-----------------------------------------------------------------------------
//!	�v���C���[
//-----------------------------------------------------------------------------
class	Player	: public Man
{
public:
	// ! ���ߗ񋓑�
	enum	COMMAND
	{
		COMMAND_NONE,	// �ʏ�
		COMMAND_ATTACK,	// �U��
		COMMAND_BACK,	// �P��
		COMMAND_MAX
	};


	//! ���
	enum   ANIMATION_STATE
	{
		STATE_NONE	 = -1,	// �A�j���[�V�����Ȃ�
		STATE_MOVE,			// �ړ���
		STATE_JUMP,			// �W�����v
		STATE_ATTACK,		// �U����
		STATE_WAIT,			// �ҋ@
		STATE_DAMAGE,		// �_���[�W
		STATE_GAMEOVER,		// �Q�[���I�[�o�[
		
		STATE_MAX		
	};

public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	Player();
	//! �f�X�g���N�^
	~Player();
	//! ������
	virtual bool	Initialize();

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �X�V����
	virtual void	Update();
	//! �ʏ펞�X�V
	void	UpdateNormal(Vector3& stickState);
	//! ���ߕύX���X�V
	void	UpdateCommandChange(Vector3& stickState);
	//! �Q�[���I�[�o�[���̍X�V
	void	UpdateGameOver();

	//void	update(Controller& control, const f32 &CamAng);

	//! �`�揈��
	virtual void	Render(bool isShadow);
	//! glut�`��֘A
	virtual void	glutRender();
	//! �f�o�b�O�`��
	virtual void	debugRender();
	//! �X�e�[�^�X�̕`��(2D)
	virtual void	StatusRender();
	//! ���ߗp�`��
	void			CommandRender();
	
protected:
	//! �_���[�W�v�Z�Ɠ����蔻�菈��
	virtual s32		calcDamageAndCollision();
	//! �A�j���[�V�����ɂ��ړ��s�`�F�b�N
	virtual bool	checkFreeze(const s32& currentAnimation);

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{
public:
	//! �擾
	COMMAND			getCommand()	const { return _command; }
	//! �Q�[���I�[�o�[���ǂ���
	bool			getGameOver()	const { return _gameOver; }
	//! ��ɋ߂����ǂ���
	bool			getNearCastle() const { return _isNearCastle; }
	//! ��ɋ߂����ǂ����t���O�ݒ�
	void			setNearCastle(bool flags) { _isNearCastle = flags; }

	//@}
private:

	//virtual void	Jump();				//!< �W�����v
	//virtual void	Attack();			//!< �U��
	bool				_commandChangeMode;	//!< �R�}���h�`�F���W�t���O
	COMMAND				_command;			//!< ���ߗp

	//Collision			_myCollision;		//!< �Փ˔���p
	Collision			_attackCollision;	//!< �U������p
	
	ANIMATION_STATE		_aniState;			//!< �A�j���[�V�����p�X�e�[�^�X

	bool				_isNearCastle;		//!< ��ɋ߂����ǂ����t���O
	bool				_gameOver;			//!< �Q�[���I�[�o�[�t���O
	
};

//=============================================================================
//	END OF FILE
//=============================================================================