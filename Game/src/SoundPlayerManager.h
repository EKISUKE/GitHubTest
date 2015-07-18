//-----------------------------------------------------------------------------
//!
//! @file	SoundPlayerManager.h
//! @brief	サウンド再生クラス管理
//! @author	Yuki Ishigaki
//!
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//! サウンド管理
//-----------------------------------------------------------------------------
class SoundPlayerManager
{
private:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	SoundPlayerManager();

	//! コピーコンストラクタ
	SoundPlayerManager(SoundPlayerManager& src){}

	//! デストラクタ
	virtual ~SoundPlayerManager();

	//@}
public:
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! インスタンス取得
	static SoundPlayerManager*	getInstance()
	{
		static SoundPlayerManager _myInstance;
		return &_myInstance;
	}
	//! マスターボリューム取得
	f32				getMasterVolume()	const	   { return SoundPlayer::getMasterVolume(); }

	//! マスターボリューム設定
	void			setMasterVolume(f32 ammount);
	//@}
};

//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
SoundPlayerManager*	ISoundManager()
{
	return SoundPlayerManager::getInstance();
}

//-----------------------------------------------------------------------------
//!	サウンドクラス(BGM用)
//-----------------------------------------------------------------------------
class BGMSoundPlayerManager
{
public:

	//! BGMの種類
	enum BGM_KIND
	{
		BGM_TITLE,
		BGM_GAME,

		BGM_MAX
	};

private:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	BGMSoundPlayerManager();

	//! コピーコンストラクタ
	BGMSoundPlayerManager(BGMSoundPlayerManager& src){}

	//! デストラクタ
	virtual ~BGMSoundPlayerManager();
	
	//@}
public:
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 設定したの音楽再生
	//!	@param	[in]	bgmkind BMGの種類
	void			playMusic(BGM_KIND bgmkind) {
		_prevSound    = _currentSound;
		_currentSound = bgmkind;
		_soundList[_currentSound]->playMusic(); 
	}

	//! 設定したの音楽停止
	//!	@param	[in]	bgmkind BMGの種類
	void			stopMusic(BGM_KIND bgmkind) {
		_soundList[bgmkind]->stopMusic(); 
	}

	//! ひとつ前の音楽を停止
	void			stopPrevMusic() {
		// 今再生しているのをとめる
		_soundList[_currentSound]->stopMusic(); 
	}

	//! マスターボリュームを適用
	void			attachMasterVolume();

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! 音楽を取得
	//!	@param	[in]	bgmkind BMGの種類
	SoundPlayer*	getMusic(BGM_KIND bgmkind) { return _soundList[bgmkind]; }

	//! インスタンス取得
	static BGMSoundPlayerManager*	getInstance()
	{
		static BGMSoundPlayerManager _myInstance;
		return &_myInstance;
	}
	//! BGMボリューム取得
	f32				getBGMVolume()	const	   { return _bgmVolume; }

	//! BGMボリューム設定
	void			setBGMVolume(f32 ammount);
	//@}
private:
	BGM_KIND		_currentSound;
	BGM_KIND		_prevSound;
	SoundPlayer*	_soundList[BGM_MAX];
	f32				_bgmVolume;
};

//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
BGMSoundPlayerManager*	IBGMManager()
{
	return BGMSoundPlayerManager::getInstance();
}


//-----------------------------------------------------------------------------
//!	サウンドクラス(SE用)
//-----------------------------------------------------------------------------
class SESoundPlayerManager
{
public:

	//! SEの種類
	enum SE_KIND
	{
		SE_DAMAGE,
		SE_ATTACK,
		SE_HEAL,
		SE_CRASH,
		SE_BUTTON,


		SE_MAX
	};

private:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	SESoundPlayerManager();

	//! コピーコンストラクタ
	SESoundPlayerManager(SESoundPlayerManager& src){}

	//! デストラクタ
	virtual ~SESoundPlayerManager();
	
	//@}
public:
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 設定したの音楽再生
	//!	@param	[in]	sekind SEの種類
	void			playMusic(SE_KIND sekind) {
		_soundList[sekind]->playMusic(); 
	}

	//! 設定したの音楽停止
	//!	@param	[in]	sekind SEの種類
	void			stopMusic(SE_KIND sekind) {
		_soundList[sekind]->stopMusic(); 
	}

	//! マスターボリュームを適用
	void			attachMasterVolume();

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! 音楽を取得
	//!	@param	[in]	sekind SEの種類
	SoundPlayer*	getMusic(SE_KIND sekind) { return _soundList[sekind]; }

	//! インスタンス取得
	static SESoundPlayerManager*	getInstance()
	{
		static SESoundPlayerManager _myInstance;
		return &_myInstance;
	}

	//! SEボリューム取得
	f32				getSEVolume() const { return _seVolume; }

	//! SEボリューム設定
	void			setSEVolume(f32 ammount);

	//@}
private:
	SoundPlayer*	_soundList[SE_MAX];
	f32				_seVolume;
};

//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
SESoundPlayerManager*	ISEManager()
{
	return SESoundPlayerManager::getInstance();
}


//=============================================================================
//	END OF FILE
//=============================================================================
