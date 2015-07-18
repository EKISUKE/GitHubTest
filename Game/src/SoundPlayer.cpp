//-----------------------------------------------------------------------------
//!
//! @file	SoundPlayer.cpp
//! @brief	�T�E���h�Đ��N���X
//! @author	Mikael Nikander
//!
//-----------------------------------------------------------------------------
#include "Library.h"

//-----------------------------------------------------------------------------
// �ÓI�֐��̏�����
//-----------------------------------------------------------------------------
f32 SoundPlayer::_masterVolume	= 1.0f;

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
SoundPlayer::SoundPlayer()
: _isPlaying(false)
, _isLoaded(false)
{
	_pChannel			= 0;
	_pExtraDriverData	= 0;
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
SoundPlayer::~SoundPlayer()
{

}

//-----------------------------------------------------------------------------
//! �ǂݍ���
//!	@param	[in]	filePath	�t�@�C���p�X��
//!	@param	[in]	isLoop		���[�v���邩�ǂ���
//!	@return	true: ���� false: ���s
//-----------------------------------------------------------------------------
bool SoundPlayer::loadMusic(GM_CSTR fileName, bool isLoop)
{ 
	// FMOD�̏�����
	FMOD::System_Create(&_pSystem);

	// �o�[�W�����擾
	_pSystem->getVersion(&_version);

	// FMOD�̃o�[�W�������Ⴏ���
	// �ŐV�o�[�W������pCannnel�ŉ��y���~�߂��Ȃ����ߌ���ЂƂÂ��̂��g���Ă���
	// ���̂��߂��̃o�[�W������r�ŕK���ʂ��Ă��܂����߃R�[�h�̓R�����g�A�E�g��
	// ����ƁA�ŐV�o�[�W�����ł���Ă����PC���̂ɉe�����ăw�b�h�t�H�����特���������Ȃ��Ȃ铙
	// �s���肾�Ɗ������̂ŌÂ��o�[�W�������g���Ă���
	/*
	if(_version < FMOD_VERSION) {
		stringstream ss;
		ss << "FMOD lib version ";
		ss << _version;
		ss << "doesn't match header version ";
		ss << FMOD_VERSION;
		
		MessageBoxA(nullptr, ss.str().c_str(), "�o�[�W�����G���[", MB_OK);
	}
	*/

	// ������
	_pSystem->init(32, FMOD_INIT_NORMAL, _pExtraDriverData);
	
	// �ǂݍ���
	FMOD_RESULT result = _pSystem->createSound(fileName, FMOD_HARDWARE, 0, &_pSound);

	// �ǂݍ��ݐ����������ǂ���
	if( result != FMOD_OK ){
		return false;
	}

	// �Đ��X�e�[�^�X�ݒ�
	if( isLoop ) {
		_pSound->setMode(FMOD_LOOP_NORMAL);
	}else{
		_pSound->setMode(FMOD_LOOP_OFF);
	}
	
	// �{�����[���ݒ�
	setVolume(1.0f);

	// �ǂݍ��݊����t���OON
	_isLoaded = true;

	return _isLoaded;
}

//-----------------------------------------------------------------------------
//! �Đ�
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
//! ��~
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
//! �ꎞ��~
//-----------------------------------------------------------------------------
void SoundPlayer::pauseMusic()
{
//	_pChannel->isPlaying(&_isPlaying);
	if(!_isPlaying)	return;
	_pChannel->setPaused(true);
	_isPlaying = false;
}

//-----------------------------------------------------------------------------
//! �}�X�^�[�{�����[����K�p����
//-----------------------------------------------------------------------------
void SoundPlayer::attachMasterVolume()
{
	setVolume(_ammount);
}

//-----------------------------------------------------------------------------
//! ���ʐݒ�
//-----------------------------------------------------------------------------
void SoundPlayer::setVolume(f32 ammount)
{
	_ammount = ammount;
	_pChannel->setVolume(_ammount * _masterVolume);
}


//-----------------------------------------------------------------------------
//! �}�X�^�[�{�����[���ݒ�
//-----------------------------------------------------------------------------
void SoundPlayer::setMasterVolume(f32 ammount)
{
	_masterVolume = ammount;
}



