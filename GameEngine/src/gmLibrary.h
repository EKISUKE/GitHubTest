//-----------------------------------------------------------------------------
//!
//!	@file	gmLibrary.h
//!	@brief	�Q�[���G���W���C���N���[�h�w�b�_
//! @author	Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once
#pragma message(" + gmLibrary.h")

#include "targetver.h"

//--------------------------------------------------------------
// Windows�֘A
//--------------------------------------------------------------
#include <windows.h>

//---- timeGetTime�֘A
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>

//---- GDI+�w�b�_�֘A
#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")	// �����N���ɕK�v�ȃ��C�u����


//--------------------------------------------------------------
// C �����^�C�� �w�b�_�[ �t�@�C��
//--------------------------------------------------------------
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <locale.h>
#include <ctime>

#include <process.h>			// _beginthreadex()
using namespace std;

//--------------------------------------------------------------
// STL
//--------------------------------------------------------------
#include <vector>
#include <list>
#include <memory>

//--------------------------------------------------------------
// ���������[�N���o�p
//--------------------------------------------------------------
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif	// _DEBUG

//--------------------------------------------------------------
// OpenGL�֘A
//--------------------------------------------------------------
#include <glew/include/GL/glew.h>
#include <glew/include/GL/wglew.h>
#pragma comment(lib, "glew32.lib")
#include <freeglut/include/GL/freeglut.h>


//--------------------------------------------------------------
// XInput�֘A
//--------------------------------------------------------------
#pragma comment(lib,"XInput.lib")
#include<XInput/Xinput.h>


//-------------------------------------------------------------
// �Q�[���ݒ�
//-------------------------------------------------------------
#define	GAME_MODE 0						//!< �Q�[�����[�h

//-------------------------------------------------------------
//	�Q�[���G���W��
//-------------------------------------------------------------
#include "gmTypedef.h"					// �^
#include "gmType.h"						// �^���

#include "gmRefCounter.h"				// �Q�ƃJ�E���^
#include "gmBase.h"						// ���
#include "gmWindowInfo.h"				// �E�B���h�E���

//---- �\���̊֘A(Struct)
#include "gmVector.h"					// �x�N�g��
#include "gmMatrix.h"					// �s��
#include "gmQuaternion.h"				// �N�H�[�^�j�I��(�l����)
#include "gmGeometry.h"					// �`��


//---- ����֘A(Key)
#include "gmMouse.h"					// �}�E�X
#include "gmMouseManager.h"				// �}�E�X�Ǘ�
#include "gmController.h"				// �R���g���[���[
#include "gmControllerManager.h"		// �R���g���[���[�Ǘ�

//---- �V�F�[�_�֘A(Shader)
#include "gmShaderObjectBase.h"			// �V�F�[�_���
#include "gmVertexShader.h"				// ���_�V�F�[�_
#include "gmFragmentShader.h"			// �t���O�����g�V�F�[�_
#include "gmProgramShader.h"			// �v���O�����V�F�[�_
#include "gmVertexArrayObject.h"		// ���_�t�H�[�}�b�g

//---- �ėp�֐��֘A
#include "gmSystem.h"					// �ėp�֐�

//---- �`��֘A(Render)
#include "gmHDRTexReader.h"				// HDR�e�N�X�`���ǂݍ���
#include "gmTexture.h"					// �e�N�X�`��

//---- �J�����֘A(Camera)
#include "gmCameraBase.h"				// �J�������
#include "gmCamera.h"					// �J����
#include "gmCameraManager.h"			// �J�����Ǘ�

//---- �f�[�^�A�Z�b�g�֘A(Asset)		
#include "gmAssetBase.h"				// �A�Z�b�g���
#include "gmAssetTexture.h"				// �e�N�X�`��
#include "gmAssetModelX.h"				// ���f��(X�t�@�C��)

//---- �^�X�N�֘A(Task)
#include "gmTaskBase.h"					// �^�X�N���
#include "gmTaskModelX.h"				// ���f��(X�t�@�C��)

//---- �|�X�g�G�t�F�N�g�t�B���^�[(Post effect filter)
#include "gmTaskFilter.h"				// �t�B���^���
#include "gmTaskFilterBloom.h"			// Bloom�O���A
#include "gmTaskFilterGlare.h"			// �O���A
#include "gmTaskFilterTonemapping.h"	// �g�[���}�b�s���O
#include "gmTaskFilterMotionBlur.h"		// ���[�V�����u���[
#include "gmTaskFilterLensFlare.h"		// �����Y�t���A
#include "gmTaskFilterGaussian.h"		// �ڂ����t�B���^

//---- �����蔻��(Collision)
#include "gmAABBTree.h"					// AABBTree
#include "gmCollision.h"				// �Փ˔���
#include "gmCollisionWall.h"			// �Փ˔���(��)
#include "gmLandScape.h"				// �Փ˔���(�n��)
#include "gmCollisionManager.h"			// �ՓˊǗ��N���X


//---- �^�C�}�[
#include "gmTimer.h"					// �^�C�}�[
#include "gmProfiler.h"					// �v���t�@�C���[

//---- �V�[���J�ڊ֘A(Scene)
#include "gmSceneBase.h"				// �V�[�����

//---- �V�X�e���֘A (System)
#include "gmSystemAsset.h"				// �A�Z�b�g�Ǘ�
#include "gmSystemRender.h"				// �`��
#include "gmSystemShader.h"				// �V�F�[�_�[
#include "gmSystemScene.h"				// �V�[���Ǘ�

#include "gmSystemMain.h"				// ���C��


//! �O���[�o��
namespace Global {
	extern	bool	debugMode;			//!< �f�o�b�O���[�h
	extern	bool	filterEnable;		//!< �t�B���^�L���t���O
	extern	f32		deltaTime;			//!< �f���^�^�C��
};


//=============================================================================
//	END OF FILE
//=============================================================================