//-----------------------------------------------------------------------------
//!
//! @file	SoundPlayer.cpp
//! @brief	サウンド再生クラス
//! @author	Mikael Nikander
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//-----------------------------------------------------------------------------
// 静的関数の初期化
//-----------------------------------------------------------------------------
f32 SoundPlayer::_masterVolume	= 1.0f;

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
SoundPlayer::SoundPlayer()
: _isPlaying(false)
, _isLoaded(false)
{
	_pChannel			= 0;
	_pExtraDriverData	= 0;
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
SoundPlayer::~SoundPlayer()
{

}

//-----------------------------------------------------------------------------
//! 読み込み
//!	@param	[in]	filePath	ファイルパス名
//!	@param	[in]	isLoop		ループするかどうか
//!	@return	true: 成功 false: 失敗
//-----------------------------------------------------------------------------
bool SoundPlayer::loadMusic(GM_CSTR fileName, bool isLoop)
{ 
	// FMODの初期化
	FMOD::System_Create(&_pSystem);

	// バージョン取得
	_pSystem->getVersion(&_version);

	// FMODのバージョンが低ければ
	// 最新バージョンはpCannnelで音楽を止められないため現状ひとつ古いのを使っている
	// そのためこのバージョン比較で必ず通ってしまうためコードはコメントアウト中
	// それと、最新バージョンでやっているとPC自体に影響してヘッドフォンから音が聞こえなくなる等
	// 不安定だと感じたので古いバージョンを使っている
	/*
	if(_version < FMOD_VERSION) {
		stringstream ss;
		ss << "FMOD lib version ";
		ss << _version;
		ss << "doesn't match header version ";
		ss << FMOD_VERSION;
		
		MessageBoxA(nullptr, ss.str().c_str(), "バージョンエラー", MB_OK);
	}
	*/

	// 初期化
	_pSystem->init(32, FMOD_INIT_NORMAL, _pExtraDriverData);
	
	// 読み込み
	FMOD_RESULT result = _pSystem->createSound(fileName, FMOD_HARDWARE, 0, &_pSound);

	// 読み込み成功したかどうか
	if( result != FMOD_OK ){
		return false;
	}

	// 再生ステータス設定
	if( isLoop ) {
		_pSound->setMode(FMOD_LOOP_NORMAL);
	}else{
		_pSound->setMode(FMOD_LOOP_OFF);
	}
	
	// ボリューム設定
	setVolume(1.0f);

	// 読み込み完了フラグON
	_isLoaded = true;

	return _isLoaded;
}

//-----------------------------------------------------------------------------
//! 再生
//-----------------------------------------------------------------------------
void SoundPlayer::playMusic()
{
//	_pChannel->isPlaying(&_isPlaying);
	if(_isPlaying)	return;
	_pSystem->playSound(_pSound, 0, false, &_pChannel);
	setVolume(_ammount);
	_isPlaying = true;
}

//-----------------------------------------------------------------------------
//! 停止
//-----------------------------------------------------------------------------
void SoundPlayer::stopMusic()
{
	if(!_isLoaded)	return;

//	_pChannel->isPlaying(&_isPlaying);
	if(!_isPlaying)	return;

	_pChannel->stop();
	_isPlaying = false;
}

//-----------------------------------------------------------------------------
//! 一時停止
//-----------------------------------------------------------------------------
void SoundPlayer::pauseMusic()
{
//	_pChannel->isPlaying(&_isPlaying);
	if(!_isPlaying)	return;
	_pChannel->setPaused(true);
	_isPlaying = false;
}

//-----------------------------------------------------------------------------
//! マスターボリュームを適用する
//-----------------------------------------------------------------------------
void SoundPlayer::attachMasterVolume()
{
	setVolume(_ammount);
}

//-----------------------------------------------------------------------------
//! 音量設定
//-----------------------------------------------------------------------------
void SoundPlayer::setVolume(f32 ammount)
{
	_ammount = ammount;
	_pChannel->setVolume(_ammount * _masterVolume);
}


//-----------------------------------------------------------------------------
//! マスターボリューム設定
//-----------------------------------------------------------------------------
void SoundPlayer::setMasterVolume(f32 ammount)
{
	_masterVolume = ammount;
}



