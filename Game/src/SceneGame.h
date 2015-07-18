//-----------------------------------------------------------------------------
//!
//!	@file	SceneGame.h
//!	@brief	�Q�[����
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------
// �O���[�o���ϐ�
//--------------------------------------------------------------
#define		ENEMY_ATTACKER		2									//!< �G�U���^�C�v�̍ő吔
#define		ENEMY_DEFFENDER		2									//!< �G�h��^�C�v�̍ő吔
#define		ENEMY_SQUAD_MEM_MAX (ENEMY_ATTACKER + ENEMY_DEFFENDER)	//!< �G�����̃����o�[�ő吔
#define		ENEMY_SQUAD_MAX		2									//!< �G�����̍ő吔
#define		ENEMY_COUNT_MAX		(ENEMY_SQUAD_MEM_MAX) * 2			//!< �G�S�̂̍ő吔



#define		ALLY_ATTACKER	4
#define		ALLY_DEFFENDER	6
#define		ALLY_HEALER		4
//#define		ALLY_ATTACKER	1
//#define		ALLY_DEFFENDER	1
//#define		ALLY_HEALER		1
#define		ALLY_COUNT_MAX	ALLY_ATTACKER + ALLY_DEFFENDER + ALLY_HEALER
#define		CHARA_SCALE	5.0f			// �L�����̃X�P�[���l

//-----------------------------------------------------------------------------
//! �Q�[����
//-----------------------------------------------------------------------------
//class SceneGame : public Singleton<SceneGame>
class SceneGame : public SceneBase
{
public:
	GM_TYPE(SceneGame)

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SceneGame();

	//! �f�X�g���N�^
	virtual			~SceneGame();
	

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! ������
	bool	init();
	//! �X�V
	void	update();
	//! �X�V
	void	update(s32& state);
	//! 3D�`��
	void	draw3D();
	//! 2D�`��
	void	draw2D();
	//! �t�B���^�[�̉e�����Ȃ��`��
	void	drawNoFilter();
	//! �^�C�}�[�̕`��
	void	drawDebugTimer();
	//! �`��
	void	render();
	//! �~�b�V�����`��
	void	missionRender();
	//! �Q�[���N���A�I�[�o�[�����`��
	void	gameEndRender();

	//! �����֐�
	void	revival();
	//! ���������֐�
	void	revivalAlly(Man* pAlly);
	//! �G�����֐�
	void	revivalEnemy(Man* pEnemy);
	
	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �����̒ǉ�
	bool			addAlly(AllyBase* pAlly);
	//! �G�̒ǉ�
	bool			addEnemy(EnemyBase* pEnemy);

	//! �}�E�X�̏��擾
	Mouse*			getMouse()	 { return _pMouse; }
	//! �J�����̏��擾
	Camera*			getCamera()  { return _pCamera; }
	//! �R���g���[���[�̏��擾
	Controller*		getControl() { return _pControl; }

	//@}
private:
	Mouse*			_pMouse;						//!< �}�E�X
	Camera*			_pCamera;						//!< �J����
	Controller*		_pControl;						//!< �R���g���[���[
	GLFont*			_pFont;							//!< �����\���p
	Player*			_pPlayer;						//!< �v���C���[
	EnemyBase*		_pEnemy[ENEMY_COUNT_MAX];		//!< �G

	AllyBase*		_pAlly[ALLY_COUNT_MAX];			//!< ����
	Map*			_pMap;							//!< �}�b�v

	Squad*			_pPlayerSquad;					//!< �v���C���[�̕���

	EnemyLeader*	_pEnemyLeader[ENEMY_SQUAD_MAX];	//!< �G�̃��[�_�[(�e�X�g)
	Squad*			_pEnemySquad[ENEMY_SQUAD_MAX];	//!< �G�̕���(�e�X�g)

	bool			_isShadow;						//!< �V���h�E�`��t���O
	bool			_gameClear;						//!< �Q�[���N���A�t���O
	bool			_gameOver;						//!< �Q�[���I�[�o�[�t���O

	Texture*		_pGameClearWord;				//!< �Q�[���N���A����
	Texture*		_pGameOverWord;					//!< �Q�[���I�[�o�[����
	Texture*		_pMissionWord[2];				//!< �~�b�V��������
	Texture*		_pXButtonImage;					//!< X�{�^���摜
	BillBoard*		_pXButtonUI;					//!< X�{�^��UI�p


	Light*			_pLight;						//!< ���C�g
	SkyBox*			_pSkyBox;						//!< �X�J�C�{�b�N�X

	f32				_waitGoTitle;					//!< �Q�[���I������^�C�g���ڍs�܂ł̑҂�����

	Performance*	_pUpdatePerformance;			//!< �X�V�̃p�t�H�[�}���X
	Performance*	_pRenderPerformance;			//!< �`��̃p�t�H�[�}���X

	s32				_allyRevivalCountMax;			//!< ���������ő吔
	s32				_enemyRevivalCountMax;			//!< �G�����ő吔
};


//=============================================================================
//	END OF FILE
//=============================================================================