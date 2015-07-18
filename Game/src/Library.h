//-----------------------------------------------------------------------------
//!
//!	@file	Library.h
//!	@brief	���C�u�����p�w�b�_
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//---- �Q�[���G���W���̂ق��C���N���[�h
#include "../../GameEngine/src/gmLibrary.h"

//// Windows�ŃR���\�[�����o���Ȃ�����
//#ifdef NDEBUG
//#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
//#endif




//---- ���C��
#include "Main.h"				//!< ���C���V�X�e��
//---- �e���v���[�g�n
#include "Singleton.h"			//!< �V���O���g���I�u�W�F�N�g


//---- �\����
#include "GLFont.h"				//!< �����\���p

//---- �T�E���h�v���C���[
#include "SoundPlayer.h"		//!< �T�E���h�v���C���[
#include "SoundPlayerManager.h"	//!< �T�E���h�Đ��Ǘ�


//---- ���C�g�n
#include "LightBase.h"			//!< ���C�g���
#include "Light.h"				//!< ���C�g
#include "LightManager.h"		//!< ���C�g�Ǘ�

//---- �V���h�E�֘A
#include "ShadowMap.h"			//!< �V���h�E�}�b�v(1��)
#include "CascadedShadow.h"		//!< �J�X�P�[�h�V���h�E


//---- �G�t�F�N�g
#include "BillBoard.h"			//!< �r���{�[�h
#include "Effect.h"				//!< �G�t�F�N�g

//---- �����蔻��
#include "CollisionObject.h"	//!< �I�u�W�F�N�g�p�����蔻��

//---- �I�u�W�F�N�g�n
#include "StatusAssets.h"		//!< �X�e�[�^�X�f�[�^
#include "Status.h"				//!< �X�e�[�^�X
#include "IObj.h"				//!< �I�u�W�F�N�g�C���^�[�t�F�[�X
#include "ObjDataManager.h"		//!< �I�u�W�F�N�g�f�[�^�Ǘ�
#include "ObjBase.h"			//!< �I�u�W�F�N�g���
//---- ����Ǘ��n
#include "Key.h"				//!< �I�u�W�F�N�g����C���^�[�t�F�[�X
#include "KeyAttacker.h"		//!< �U���^�C�v����
#include "KeyDeffender.h"		//!< �h��^�C�v����
#include "KeyHealer.h"			//!< �񕜃^�C�v����
#include "KeyInput.h"			//!< �L�[����


//---- �X�J�C�{�b�N�X
#include "SkyBox.h"				//!< �X�J�C�{�b�N�X


//---- �}�b�v
#include "Map.h"				//!< �}�b�v

//---- �L����
#include "Man.h"				//!< �Q�[�����L����

//---- ����
#include "Squad.h"				//!< ����
#include "SquadManager.h"		//!< �����Ǘ�

//---- �v���C���[
#include "Player.h"				//!< �v���C���[

//---- NPC�̃^�C�v
#include "Enemy.h"				//!< �G���
#include "EnemyLeader.h"		//!< �G���[�_�[�^�C�v
#include "EnemyAttacker.h"		//!< �G�U���^�C�v
#include "EnemyDeffender.h"		//!< �G�h��^�C�v

//---- ����
#include "Ally.h"				// �������
#include "AllyAttacker.h"		// �����U���^�C�v
#include "AllyDeffender.h"		// �����h��^�C�v
#include "AllyHealer.h"			// �����񕜃^�C�v

//---- �V�[���֌W
#include "SceneTitle.h"			// �^�C�g��
#include "SceneOption.h"		// �I�v�V����
#include "SceneGame.h"			// �Q�[����




//=============================================================================
//	END OF FILE
//=============================================================================