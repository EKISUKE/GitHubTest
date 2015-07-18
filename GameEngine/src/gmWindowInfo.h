//-----------------------------------------------------------------------------
//!
//!	@file	gmWindowInfo.h
//!	@brief	ウィンドウ情報クラス
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//! @note あとあとWindowInfoクラスを構造体にしてSystemWindowなどを作成し、
//!	そこでウィンドウのリサイズなどを行うようにする予定

//-----------------------------------------------------------------------------
//! ウィンドウ情報
//-----------------------------------------------------------------------------
class WindowInfo
{
public:

	//-------------------------------------------------------------
	//!	@name 取得
	//-------------------------------------------------------------
	//@{

	//! インスタンス取得
	static WindowInfo*	getInstance()
	{
		static WindowInfo	_myInstance;
		return &_myInstance;
	}

	//! 画面解像度を取得
	Size<s32>		getResolution()		const { return _windowResolution; }

	//! 最大画面解像度を取得
	Size<f32>		getResolutionMax()  const { return _windowResolutionMax; }

	//! 最大解像からの比率取得(今現在最大は1920x1080としている)
	Size<f32>		getRateResMax()	 const {
		return Size<f32>(
			_windowResolution._w / _windowResolutionMax._w,
			_windowResolution._h / _windowResolutionMax._h
			);
	}


	//! 画面比率取得
	f32				getWindowRatio() const { return (f32)_windowResolution._w / (f32)_windowResolution._h; }

	
	void			setResolutionMax(Size<f32> resolutionMax) { _windowResolutionMax = resolutionMax; }

	//@}

private:

	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	WindowInfo(){
		//_windowResolution		= Size<s32>(1600	, 900);
		_windowResolution		= Size<s32>(1920	, 1080);
		_windowResolutionMax	= Size<f32>(1920.0f	, 1080.0f);
	}
	//! コピーコンストラクタ
	WindowInfo(WindowInfo& info){}
	//! デストラクタ
	~WindowInfo(){}

	//@}
public:
	Size<f32>	_windowResolutionMax;	//!< 最大解像度
	Size<s32>	_windowResolution;		//!< 画面解像度
};


//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
WindowInfo*	GmWindow()
{
	return WindowInfo::getInstance();
}

//=============================================================================
//	END OF FILE
//=============================================================================
