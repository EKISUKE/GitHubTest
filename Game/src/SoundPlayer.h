//-----------------------------------------------------------------------------
//!
//! @file	SoundPlayer.h
//! @brief	�T�E���h�Đ��N���X
//! @author	Mikael Nikander(FMOD����) Yuki Ishigaki(���ʒ��ߊ֐���R�����g)
//!
//-----------------------------------------------------------------------------
#pragma once

#include <fmod/inc/fmod.h>
#include <fmod/inc/fmod.hpp>
#pragma comment( lib, "fmod_vc.lib" )
//#include "common.h"

//-----------------------------------------------------------------------------
//!	�T�E���h�Đ��N���X
//-----------------------------------------------------------------------------
class SoundPlayer {
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	SoundPlayer();
	//! �f�X�g���N�^
	virtual ~SoundPlayer();


	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! �ǂݍ���
	//!	@param	[in]	filePath	�t�@�C���p�X��
	//!	@param	[in]	isLoop		���[�v���邩�ǂ���
	//!	@return	true: ���� false: ���s
	bool	loadMusic(GM_CSTR filePath, bool isLoop);
	//! �Đ�
	void	playMusic();
	//! ��~
	void	stopMusic();
	//! �ꎞ��~
	void	pauseMusic();

	//! �}�X�^�[�{�����[����K�p����
	void	attachMasterVolume();
	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! �Đ����t���O�擾
	bool		getPlaying()		const { return _isPlaying; }
	//! �ǂݍ��݊����t���O�擾
	bool		getLoaded()			const { return _isLoaded; }

	//! ���ʎ擾
	f32			getVolume()			const { return _ammount; }

	//! �}�X�^�[�{�����[���擾
	static f32	getMasterVolume()		  { return _masterVolume; }

	//! ���ʐݒ�
	void		setVolume		(f32 ammount);
	//! �}�X�^�[�{�����[���ݒ�
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

	//---- ���ʒ��ߗp
	static f32		_masterVolume;

};

//=============================================================================
//	END OF FILE
//=============================================================================

