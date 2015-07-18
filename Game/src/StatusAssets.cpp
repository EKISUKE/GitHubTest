//-----------------------------------------------------------------------------
//!
//!	@file	StatusAssets.h
//!	@brief	�X�e�[�^�X�f�[�^
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//-----------------------------------------------------------------------------
//!	�f�t�H���g�R���X�g���N�^
//-----------------------------------------------------------------------------
StatusAssets::StatusAssets()
{
	loadAssetStatus();
}

//-----------------------------------------------------------------------------
//! �X�e�[�^�X�̏�����
//-----------------------------------------------------------------------------
void StatusAssets::loadAssetStatus()
{
	static const GM_CSTR loadPath[STATUS_MAX] = {
		"../Assets/Status/Status - Attacker.csv",
		"../Assets/Status/Status - Deffender.csv",
		"../Assets/Status/Status - Player.csv",
		"../Assets/Status/Status - Enemy.csv",
		"../Assets/Status/Status - Healer.csv",
	};

	// �q�[���[�ȊO�͓ǂݍ���
	for( s32 i=0; i<STATUS_HEAL; ++i ) {
		_pStatusList[i] = nullptr;
		SAFE_NEW( _pStatusList[i] );
		_pStatusList[i]->loadCSV(loadPath[i]);
	}
	// �q�[���[�͈Ⴄ�N���X�Ȃ̂ł�����new����
	_pStatusList[STATUS_HEAL] = new StatusHeal();
	_pStatusList[STATUS_HEAL]->loadCSV(loadPath[STATUS_HEAL]);
}

//-----------------------------------------------------------------------------
//!	�f�X�g���N�^
//-----------------------------------------------------------------------------
StatusAssets::~StatusAssets()
{
}

//=============================================================================
//	END OF FILE
//=============================================================================