//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterBloom.cpp
//!	@brief	ブルームグレア
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
TaskFilterBloom::TaskFilterBloom()
:_brightness(5.0f)
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
TaskFilterBloom::~TaskFilterBloom()
{
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool TaskFilterBloom::setup()
{
	// 親クラスの初期化を呼ぶ
	TaskFilter::setup();

	return true;
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void TaskFilterBloom::cleanup()
{
	TaskFilter::cleanup();
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void TaskFilterBloom::render()
{

	static const s32	SHADER_STANDARD			= SystemShader::SHADER_FILTER;
	static const s32	SHADER_GAUSSIAN_3X3_H	= SystemShader::SHADER_GAUSSIAN_3X3_H;
	static const s32	SHADER_GAUSSIAN_3X3_V	= SystemShader::SHADER_GAUSSIAN_3X3_V;
	static const s32	SHADER_SUBTRACT			= SystemShader::SHADER_SUBTRACT;
	static const s32	SHADER_BLOOM_GLARE		= SystemShader::SHADER_BLOOM_GLARE;

	//-------------------------------------------------------------
	// (1) バックバッファからワークバッファにコピー
	// グレアの場合はぼかすので1/2バッファの縮小しておくことで
	// 後段パスが軽量化される(バイリニアで4ピクセル平均)
	//-------------------------------------------------------------
	copyToWorkTexture(SHADER_STANDARD, 1);

	// 半透明無効化
	setBlendMode(BM_NONE);
	//-------------------------------------------------------------
	// (2) 高輝度の成分を抽出
	// 画像から閾値(Threshold)の分を減算
	//-------------------------------------------------------------

	static float threshold = 1.3f;
	static float scale	   = 0.1f;

	if (GetKeyState(VK_OEM_PLUS) & 0x80){
		threshold += 0.1f;
	}
	else if (GetKeyState(VK_OEM_MINUS) & 0x80){
		threshold -= 0.1f;
	}

	GmShader()->changeShader(SystemShader::SHADER_SUBTRACT);

	GmShader()->setUniform1fv("gThreshold", (GLfloat*)&threshold);
	GmShader()->setUniform1fv("scale", (GLfloat*)&scale);

	setRenderTarget(1, 1);
	//setBackBuffer();
	put(0, 1);	// texuter[0] mip=0 : SUBTRACT

	GmShader()->beginPrevShader();

	//return;
	//-------------------------------------------------------------
	// (3) ガウシアンブラー→縮小を繰り返し、多段バッファ生成
	//-------------------------------------------------------------
//	s32		resultTextureIndex[16];	// 各ミップ段数のどちらに結果が入っているか
	s32		index = 1;
	s32		mip   = 1;

	//---- 水平ブラー
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_H);
	index ^= 1;

	//---- 垂直ブラー
	setRenderTarget(index ^ 1, mip);
	put(index, mip, SystemShader::SHADER_GAUSSIAN_3X3_V);
	index ^= 1;

#if 1	
	while( mip<REDUCTION_LEVEL-1 ) {
		
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

	//-------------------------------------------------------------
	// (4) 加算合成で全てをバックバッファに合成する
	//-------------------------------------------------------------
	setBackBuffer();
		
	s32	workTextureIndex = index;
	s32 mipLevel		 = 1;


	setBlendMode(BM_ADD_ALPHA);
	GmShader()->changeShader(SystemShader::SHADER_BLOOM_GLARE);
	GmShader()->setUniform1f("brightness", _brightness);
	put(workTextureIndex, mipLevel);
	GmShader()->beginPrevShader();
	setBlendMode(BM_NONE);
	
#endif 
}


//=============================================================================
//	END OF FILE
//=============================================================================