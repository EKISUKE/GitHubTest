//-----------------------------------------------------------------------------
//!
//! @file	SoundPlayerManager.h
//! @brief	�T�E���h�Đ��N���X�Ǘ�
//! @author	Yuki Ishigaki
//!
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//! �T�E���h�Ǘ�
//-----------------------------------------------------------------------------
class SoundPlayerManager
{
private:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SoundPlayerManager();

	//! �R�s�[�R���X�g���N�^
	SoundPlayerManager(SoundPlayerManager& src){}

	//! �f�X�g���N�^
	virtual ~SoundPlayerManager();

	//@}
public:
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�擾
	static SoundPlayerManager*	getInstance()
	{
		static SoundPlayerManager _myInstance;
		return &_myInstance;
	}
	//! �}�X�^�[�{�����[���擾
	f32				getMasterVolume()	const	   { return SoundPlayer::getMasterVolume(); }

	//! �}�X�^�[�{�����[���ݒ�
	void			setMasterVolume(f32 ammount);
	//@}
};

//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
SoundPlayerManager*	ISoundManager()
{
	return SoundPlayerManager::getInstance();
}

//-----------------------------------------------------------------------------
//!	�T�E���h�N���X(BGM�p)
//-----------------------------------------------------------------------------
class BGMSoundPlayerManager
{
public:

	//! BGM�̎��
	enum BGM_KIND
	{
		BGM_TITLE,
		BGM_GAME,

		BGM_MAX
	};

private:
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	BGMSoundPlayerManager();

	//! �R�s�[�R���X�g���N�^
	BGMSoundPlayerManager(BGMSoundPlayerManager& src){}

	//! �f�X�g���N�^
	virtual ~BGMSoundPlayerManager();
	
	//@}
public:
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �ݒ肵���̉��y�Đ�
	//!	@param	[in]	bgmkind BMG�̎��
	void			playMusic(BGM_KIND bgmkind) {
		_prevSound    = _currentSound;
		_currentSound = bgmkind;
		_soundList[_currentSound]->playMusic(); 
	}

	//! �ݒ肵���̉��y��~
	//!	@param	[in]	bgmkind BMG�̎��
	void			stopMusic(BGM_KIND bgmkind) {
		_soundList[bgmkind]->stopMusic(); 
	}

	//! �ЂƂO�̉��y���~
	void			stopPrevMusic() {
		// ���Đ����Ă���̂��Ƃ߂�
		_soundList[_currentSound]->stopMusic(); 
	}

	//! �}�X�^�[�{�����[����K�p
	void			attachMasterVolume();

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���y���擾
	//!	@param	[in]	bgmkind BMG�̎��
	SoundPlayer*	getMusic(BGM_KIND bgmkind) { return _soundList[bgmkind]; }

	//! �C���X�^���X�擾
	static BGMSoundPlayerManager*	getInstance()
	{
		static BGMSoundPlayerManager _myInstance;
		return &_myInstance;
	}
	//! BGM�{�����[���擾
	f32				getBGMVolume()	const	   { return _bgmVolume; }

	//! BGM�{�����[���ݒ�
	void			setBGMVolume(f32 ammount);
	//@}
private:
	BGM_KIND		_currentSound;
	BGM_KIND		_prevSound;
	SoundPlayer*	_soundList[BGM_MAX];
	f32				_bgmVolume;
};

//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
BGMSoundPlayerManager*	IBGMManager()
{
	return BGMSoundPlayerManager::getInstance();
}


//-----------------------------------------------------------------------------
//!	�T�E���h�N���X(SE�p)
//-----------------------------------------------------------------------------
class SESoundPlayerManager
{
public:

	//! SE�̎��
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
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SESoundPlayerManager();

	//! �R�s�[�R���X�g���N�^
	SESoundPlayerManager(SESoundPlayerManager& src){}

	//! �f�X�g���N�^
	virtual ~SESoundPlayerManager();
	
	//@}
public:
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �ݒ肵���̉��y�Đ�
	//!	@param	[in]	sekind SE�̎��
	void			playMusic(SE_KIND sekind) {
		_soundList[sekind]->playMusic(); 
	}

	//! �ݒ肵���̉��y��~
	//!	@param	[in]	sekind SE�̎��
	void			stopMusic(SE_KIND sekind) {
		_soundList[sekind]->stopMusic(); 
	}

	//! �}�X�^�[�{�����[����K�p
	void			attachMasterVolume();

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! ���y���擾
	//!	@param	[in]	sekind SE�̎��
	SoundPlayer*	getMusic(SE_KIND sekind) { return _soundList[sekind]; }

	//! �C���X�^���X�擾
	static SESoundPlayerManager*	getInstance()
	{
		static SESoundPlayerManager _myInstance;
		return &_myInstance;
	}

	//! SE�{�����[���擾
	f32				getSEVolume() const { return _seVolume; }

	//! SE�{�����[���ݒ�
	void			setSEVolume(f32 ammount);

	//@}
private:
	SoundPlayer*	_soundList[SE_MAX];
	f32				_seVolume;
};

//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
SESoundPlayerManager*	ISEManager()
{
	return SESoundPlayerManager::getInstance();
}


//=============================================================================
//	END OF FILE
//=============================================================================
