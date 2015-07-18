//-----------------------------------------------------------------------------
//!
//! @file	SoundPlayerManager.h
//! @brief	�T�E���h�Đ��N���X�Ǘ�
//! @author	Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//=============================================================================
// �T�E���h�Ǘ��N���X����
//=============================================================================

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SoundPlayerManager::SoundPlayerManager()
{
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SoundPlayerManager::~SoundPlayerManager()
{
}


//-----------------------------------------------------------------------------
//! �}�X�^�[�{�����[���ݒ�
//-----------------------------------------------------------------------------
void SoundPlayerManager::setMasterVolume(f32 ammount)
{
	// �T�E���h�v���C���[�̃}�X�^�[�{�����[���ύX
	SoundPlayer::setMasterVolume(ammount);
	// BGM,SE�ɓK�p
	IBGMManager()->attachMasterVolume();
	ISEManager()->attachMasterVolume();
}
//=============================================================================
// BGM�Ǘ��N���X����
//=============================================================================
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
BGMSoundPlayerManager::BGMSoundPlayerManager()
: _currentSound(BGM_TITLE)
, _prevSound   (BGM_TITLE)
{
	bool result;
	_soundList[BGM_TITLE] = new SoundPlayer();
	_soundList[BGM_GAME]  = new SoundPlayer();

	result = _soundList[BGM_TITLE]->loadMusic("../Sound/BGM/gameTitleBGM.mp3"	, true);
	result = _soundList[BGM_GAME]->loadMusic("../Sound//BGM/gameBattleBoss.mp3"	, true);
	setBGMVolume(1.0f);
}
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
BGMSoundPlayerManager::~BGMSoundPlayerManager()
{
	for(s32 i=0; i<BGM_MAX; ++i)
	{
		SAFE_DELETE(_soundList[(BGM_KIND)i]);
	}
}

//-----------------------------------------------------------------------------
//! �}�X�^�[�{�����[����K�p
//-----------------------------------------------------------------------------
void BGMSoundPlayerManager::attachMasterVolume()
{
	for (s32 i = 0; i<BGM_MAX; ++i){
		_soundList[(BGM_KIND)i]->attachMasterVolume();
	}
}

//-----------------------------------------------------------------------------
//! BGM�{�����[���ݒ�
//-----------------------------------------------------------------------------
void BGMSoundPlayerManager::setBGMVolume(f32 ammount)
{
	_bgmVolume = ammount;
	for(s32 i=0; i<BGM_MAX; ++i){
		_soundList[(BGM_KIND)i]->setVolume(ammount);
	}
}


//=============================================================================
// SE�Ǘ��N���X����
//=============================================================================
//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SESoundPlayerManager::SESoundPlayerManager()
{
	bool result;
	_soundList[SE_DAMAGE]  = new SoundPlayer();
	_soundList[SE_ATTACK]  = new SoundPlayer();
	_soundList[SE_HEAL]    = new SoundPlayer();
	_soundList[SE_CRASH]   = new SoundPlayer();
	_soundList[SE_BUTTON]  = new SoundPlayer();
	

	result = _soundList[SE_DAMAGE]->loadMusic("../Sound/SE/sen_ge_panchi09.mp3"		, false);
	result = _soundList[SE_ATTACK]->loadMusic("../Sound/SE/sen_ka_bokuto_kara01.mp3", false);
	result = _soundList[SE_HEAL]->loadMusic("../Sound/SE/magic-cure4.mp3"			, false);
	result = _soundList[SE_CRASH]->loadMusic("../Sound/SE/earth-tremor1.mp3"		, false);
	result = _soundList[SE_BUTTON]->loadMusic("../Sound/SE/decision12.mp3"			, false);
	setSEVolume(1.0f);
}
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SESoundPlayerManager::~SESoundPlayerManager()
{
	for( s32 i=0; i<SE_MAX; ++i )
	{
		SAFE_DELETE(_soundList[(SE_KIND)i]);
	}
}


//-----------------------------------------------------------------------------
//! �}�X�^�[�{�����[����K�p
//-----------------------------------------------------------------------------
void SESoundPlayerManager::attachMasterVolume()
{
	for (s32 i = 0; i<SE_MAX; ++i){
		_soundList[(SE_KIND)i]->attachMasterVolume();
	}
}


//-----------------------------------------------------------------------------
//! SE�{�����[���ݒ�
//-----------------------------------------------------------------------------
void SESoundPlayerManager::setSEVolume(f32 ammount)
{
	_seVolume = ammount;
	for (s32 i = 0; i<SE_MAX; ++i)
	{
		_soundList[(SE_KIND)i]->setVolume(ammount);
	}
}


//=============================================================================
//	END OF FILE
//=============================================================================