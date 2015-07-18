//-----------------------------------------------------------------------------
//!
//!	@file	Main.cpp
//!	@brief	�A�v���P�[�V�����̃G���g���|�C���g
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"



//---- �O���[�o���ϐ�
bool	ExitFlag = false;;

extern Controller* gpControl;		// �R���g���[���[

// �L�[�{�[�h�p
//BYTE KeyTbl[256];


//! �V�[�����
extern enum SCENE_STATE
{
	SCENE_TITLE,	// �^�C�g��
	SCENE_GAME,		// �Q�[��
	SCENE_CLEAR,	// �Q�[���N���A
	SCENE_OVER,		// �Q�[���I�[�o�[
	SCENE_OPTION,	// �I�v�V����
	SCENE_EXIT,		// �I��
	SCENE_NUM		// �V�[���̍ő吔
};

SCENE_STATE	scene;	// �V�[�����

SceneTitle*	pTitle;	// �^�C�g���N���X
SceneGame*	pGame;	// �Q�[���N���X

s32			state;	//	�Q�[���̍X�V���� 0:�Q�[�� 1: �N���A 2: �I�[�o�[



//---------------------------------------------------------------------------
//! �������R�[���o�b�N
//---------------------------------------------------------------------------
bool Main::onInitialize()
{
	// �V�[���W�����v	
	//GmScene()->jump(SceneGame::TYPE);
	GmScene()->jump(SceneTitle::TYPE);

	return true;
}

//---------------------------------------------------------------------------
//! ����R�[���o�b�N
//---------------------------------------------------------------------------
void Main::onFinalize()
{
	GmControlMan()->getController(1)->DisableVibration();
}

//===========================================================================
//! �A�v���P�[�V�����J�n�G���g��
//===========================================================================
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	Main	systemMain;
	systemMain.mainLoop("GameEngine", "�Q�[���G���W���e�X�g");	
}

//=============================================================================
//	END OF FILE
//=============================================================================