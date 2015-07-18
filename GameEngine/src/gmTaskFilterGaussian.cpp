//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterGaussian.cpp
//!	@brief	ぼかしフィルター
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
TaskFilterGaussian::TaskFilterGaussian()
: _blurPathCount(4)
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
TaskFilterGaussian::~TaskFilterGaussian()
{
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool TaskFilterGaussian::setup()
{
	// 親クラスの初期化を呼ぶ
	TaskFilter::setup();

	return true;
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void TaskFilterGaussian::cleanup()
{
	TaskFilter::cleanup();
}

//-----------------------------------------------------------------------------
//! 描画(渡された画像をぼかして保存先に保存する)
//!	@param	[in]	pSrc	ぼかす元画像
//!	@param	[out]	pDest	ぼかした画像保存先
//-----------------------------------------------------------------------------
void TaskFilterGaussian::render(Texture* pSrc, Texture* pDest)
{

	static const s32	SHADER_STANDARD			= SystemShader::SHADER_FILTER;
	static const s32	SHADER_GAUSSIAN_3X3_H	= SystemShader::SHADER_GAUSSIAN_3X3_H;
	static const s32	SHADER_GAUSSIAN_3X3_V	= SystemShader::SHADER_GAUSSIAN_3X3_V;
	//-------------------------------------------------------------
	// (1) 元画像をコピーする
	//-------------------------------------------------------------
	GmRender()->setRenderTarget(getWorkTexture(0), Float2(0, 0), 1);

	put(pSrc, 1, SHADER_STANDARD);

	setBackBuffer();

	//-------------------------------------------------------------
	// (2) コピーしたものをブラーしてぼかす
	//-------------------------------------------------------------
	s32		index = 0;
	s32		mip = 1;


#if 1
	//---- 水平ブラー
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_H);
	index ^= 1;

	//---- 垂直ブラー
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_V);
	index ^= 1;


	while (mip<_blurPathCount) {

		//---- ミップ段数を一段下にコピー
		setRenderTarget(index, mip + 1);
		put(index, mip, SHADER_STANDARD);
		mip++;

		//---- 水平ブラー
		setRenderTarget(index ^ 1, mip);
		put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_H);
		index ^= 1;

		//---- 垂直ブラー
		setRenderTarget(index ^ 1, mip);
		put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_V);
		index ^= 1;

	}
#else

	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_EXPAND_BLUR);
	index ^= 1;

#endif

	//-------------------------------------------------------------
	// (4) ぼかした結果を保存先に描画
	//-------------------------------------------------------------
	GmRender()->setRenderTarget(pDest, Float2(0, 0), 0);

	put(0, mip, SHADER_STANDARD);

	// バックバッファに戻す
	setBackBuffer();
	
}


//=============================================================================
//	END OF FILE
//=============================================================================