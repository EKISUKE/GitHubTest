//-----------------------------------------------------------------------------
//!
//! @file	SoundPlayerManager.h
//! @brief	サウンド再生クラス管理
//! @author	Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//=============================================================================
// サウンド管理クラス実装
//=============================================================================

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
SoundPlayerManager::SoundPlayerManager()
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
SoundPlayerManager::~SoundPlayerManager()
{
}


//-----------------------------------------------------------------------------
//! マスターボリューム設定
//-----------------------------------------------------------------------------
void SoundPlayerManager::setMasterVolume(f32 ammount)
{
	// サウンドプレイヤーのマスターボリューム変更
	SoundPlayer::setMasterVolume(ammount);
	// BGM,SEに適用
	IBGMManager()->attachMasterVolume();
	ISEManager()->attachMasterVolume();
}
//=============================================================================
// BGM管理クラス実装
//=============================================================================
//-----------------------------------------------------------------------------
//! コンストラクタ
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
//! デストラクタ
//-----------------------------------------------------------------------------
BGMSoundPlayerManager::~BGMSoundPlayerManager()
{
	for(s32 i=0; i<BGM_MAX; ++i)
	{
		SAFE_DELETE(_soundList[(BGM_KIND)i]);
	}
}

//-----------------------------------------------------------------------------
//! マスターボリュームを適用
//-----------------------------------------------------------------------------
void BGMSoundPlayerManager::attachMasterVolume()
{
	for (s32 i = 0; i<BGM_MAX; ++i){
		_soundList[(BGM_KIND)i]->attachMasterVolume();
	}
}

//-----------------------------------------------------------------------------
//! BGMボリューム設定
//-----------------------------------------------------------------------------
void BGMSoundPlayerManager::setBGMVolume(f32 ammount)
{
	_bgmVolume = ammount;
	for(s32 i=0; i<BGM_MAX; ++i){
		_soundList[(BGM_KIND)i]->setVolume(ammount);
	}
}


//=============================================================================
// SE管理クラス実装
//=============================================================================
//-----------------------------------------------------------------------------
//! コンストラクタ
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
//! デストラクタ
//-----------------------------------------------------------------------------
SESoundPlayerManager::~SESoundPlayerManager()
{
	for( s32 i=0; i<SE_MAX; ++i )
	{
		SAFE_DELETE(_soundList[(SE_KIND)i]);
	}
}


//-----------------------------------------------------------------------------
//! マスターボリュームを適用
//-----------------------------------------------------------------------------
void SESoundPlayerManager::attachMasterVolume()
{
	for (s32 i = 0; i<SE_MAX; ++i){
		_soundList[(SE_KIND)i]->attachMasterVolume();
	}
}


//-----------------------------------------------------------------------------
//! SEボリューム設定
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