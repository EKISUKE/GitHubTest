//-----------------------------------------------------------------------------
//!
//!	@file	gmWindowInfo.h
//!	@brief	�E�B���h�E���N���X
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//! @note ���Ƃ���WindowInfo�N���X���\���̂ɂ���SystemWindow�Ȃǂ��쐬���A
//!	�����ŃE�B���h�E�̃��T�C�Y�Ȃǂ��s���悤�ɂ���\��

//-----------------------------------------------------------------------------
//! �E�B���h�E���
//-----------------------------------------------------------------------------
class WindowInfo
{
public:

	//-------------------------------------------------------------
	//!	@name �擾
	//-------------------------------------------------------------
	//@{

	//! �C���X�^���X�擾
	static WindowInfo*	getInstance()
	{
		static WindowInfo	_myInstance;
		return &_myInstance;
	}

	//! ��ʉ𑜓x���擾
	Size<s32>		getResolution()		const { return _windowResolution; }

	//! �ő��ʉ𑜓x���擾
	Size<f32>		getResolutionMax()  const { return _windowResolutionMax; }

	//! �ő�𑜂���̔䗦�擾(�����ݍő��1920x1080�Ƃ��Ă���)
	Size<f32>		getRateResMax()	 const {
		return Size<f32>(
			_windowResolution._w / _windowResolutionMax._w,
			_windowResolution._h / _windowResolutionMax._h
			);
	}


	//! ��ʔ䗦�擾
	f32				getWindowRatio() const { return (f32)_windowResolution._w / (f32)_windowResolution._h; }

	
	void			setResolutionMax(Size<f32> resolutionMax) { _windowResolutionMax = resolutionMax; }

	//@}

private:

	//-------------------------------------------------------------
	//!	@name ������
	//-------------------------------------------------------------
	//@{

	//! �R���X�g���N�^
	WindowInfo(){
		//_windowResolution		= Size<s32>(1600	, 900);
		_windowResolution		= Size<s32>(1920	, 1080);
		_windowResolutionMax	= Size<f32>(1920.0f	, 1080.0f);
	}
	//! �R�s�[�R���X�g���N�^
	WindowInfo(WindowInfo& info){}
	//! �f�X�g���N�^
	~WindowInfo(){}

	//@}
public:
	Size<f32>	_windowResolutionMax;	//!< �ő�𑜓x
	Size<s32>	_windowResolution;		//!< ��ʉ𑜓x
};


//-----------------------------------------------------------------------------
// �A�N�Z�X�C���^�[�t�F�C�X
//-----------------------------------------------------------------------------
__forceinline
WindowInfo*	GmWindow()
{
	return WindowInfo::getInstance();
}

//=============================================================================
//	END OF FILE
//=============================================================================
