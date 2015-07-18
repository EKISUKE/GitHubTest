//-----------------------------------------------------------------------------
//!
//! @file	SoundPlayer.h
//! @brief	サウンド再生クラス
//! @author	Mikael Nikander(FMOD部分) Yuki Ishigaki(音量調節関数やコメント)
//!
//-----------------------------------------------------------------------------
#pragma once

#include <fmod/inc/fmod.h>
#include <fmod/inc/fmod.hpp>
#pragma comment( lib, "fmod_vc.lib" )
//#include "common.h"

//-----------------------------------------------------------------------------
//!	サウンド再生クラス
//-----------------------------------------------------------------------------
class SoundPlayer {
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	SoundPlayer();
	//! デストラクタ
	virtual ~SoundPlayer();


	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 読み込み
	//!	@param	[in]	filePath	ファイルパス名
	//!	@param	[in]	isLoop		ループするかどうか
	//!	@return	true: 成功 false: 失敗
	bool	loadMusic(GM_CSTR filePath, bool isLoop);
	//! 再生
	void	playMusic();
	//! 停止
	void	stopMusic();
	//! 一時停止
	void	pauseMusic();

	//! マスターボリュームを適用する
	void	attachMasterVolume();
	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! 再生中フラグ取得
	bool		getPlaying()		const { return _isPlaying; }
	//! 読み込み完了フラグ取得
	bool		getLoaded()			const { return _isLoaded; }

	//! 音量取得
	f32			getVolume()			const { return _ammount; }

	//! マスターボリューム取得
	static f32	getMasterVolume()		  { return _masterVolume; }

	//! 音量設定
	void		setVolume		(f32 ammount);
	//! マスターボリューム設定
	static void	setMasterVolume	(f32 ammount);
		
	//@}
private:
	bool			_isPlaying;
	bool			_isLoaded;
	FMOD::System*	_pSystem;
	FMOD::Sound*	_pSound;
	FMOD::Channel*	_pChannel;
	f32				_ammount;
	u32				_version;
	void*			_pExtraDriverData;

	//---- 音量調節用
	static f32		_masterVolume;

};

//=============================================================================
//	END OF FILE
//=============================================================================

