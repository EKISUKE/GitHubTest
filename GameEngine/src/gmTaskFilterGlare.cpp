//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterGlare.cpp
//!	@brief	グレアフィルター
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
TaskFilterGlare::TaskFilterGlare()
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
TaskFilterGlare::~TaskFilterGlare()
{
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool TaskFilterGlare::setup()
{
	// 親クラスの初期化を呼ぶ
	TaskFilter::setup();

	return true;
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void TaskFilterGlare::cleanup()
{
	TaskFilter::cleanup();
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void TaskFilterGlare::render()
{

	static const s32	SHADER_STANDARD	= SystemShader::SHADER_FILTER;
	static const s32	SHADER_SUBTRACT	= SystemShader::SHADER_SUBTRACT;

	//-------------------------------------------------------------
	// (1) バックバッファからワークバッファにコピー
	// グレアの場合はぼかすので1/2バッファの縮小しておくことで
	// 後段パスが軽量化される(バイリニアで4ピクセル平均)
	//-------------------------------------------------------------
	copyToWorkTexture(SHADER_STANDARD, 0);

	s32 divCount = 6;

	// 1/4の角度分ずらす
	f32 rotationOffset = (math::PI * 2.0f) * (0.25f * (1.0f / divCount));

	for(s32 d=0; d<divCount; d++)
	{
		// 半透明無効化
		setBlendMode(BM_NONE);

		// 光の方向を指定
		f32 angle = (math::PI * 2.0f) * ((f32)d * (1.0f / divCount)) + rotationOffset;
		f32 x	  = cosf(angle);
		f32 y	  = sinf(angle);


		GmShader()->changeShader(SystemShader::SHADER_GLARE);

		// ブラー方向設定
		GmShader()->setUniform2fv("gDir2D", (GLfloat*)&Vector3(x,y,0.0f));
		GmShader()->setUniform1f("gAttenuation", 0.99f);

		//-------------------------------------------------------------
		// (2) 高輝度の成分を抽出
		// 画像から閾値(Threshold)の分を減算
		//-------------------------------------------------------------

		//static float threshold = 0.7f;
		static float threshold = 7.9;
		static float scale	   = 0.1f;

		GmShader()->changeShader(SystemShader::SHADER_SUBTRACT);

		GmShader()->setUniform1fv("gThreshold", (GLfloat*)&threshold);
		GmShader()->setUniform1fv("scale", (GLfloat*)&scale);

		setRenderTarget(1, 1);
		put(0, 0);	// texuter[0] mip=0 : SUBTRACT

		GmShader()->beginPrevShader();

		
		////---- バックバッファをコピー
		//{
		//	stringstream name;
		//	name << "グレア減算";
		//	name << d;
		//	name << "回目";
		//	GmRender()->saveBackBuffer(name.str().c_str());
		//}

		//-------------------------------------------------------------
		// (3) ブラーを累計していく (8pixel単位)
		//-------------------------------------------------------------
		s32		index = 1;
		s32		mip	  = 1;

		for(s32 i=0; i<3; i++)
		{

			// ピクセルの間隔(1, 8, 64)
			GmShader()->setUniform1f("gStride", (GLfloat)(pow(8, (f32)i)) );

			//---- 水平ブラー
			setRenderTarget(index ^ 1, mip);
			put(index, mip);
			index ^= 1;
		}

		// 元に戻す
		GmShader()->beginPrevShader();

		//-------------------------------------------------------------
		// (4) 加算合成で全てをバックバッファに合成する
		//-------------------------------------------------------------
		setBackBuffer();

		s32	workTextureIndex = index;
		s32 mipLevel		 = mip;


		setBlendMode(BM_ADD_NOALPHA);
		put(workTextureIndex, mipLevel, SHADER_STANDARD);

	}

	setBlendMode(BM_NONE);
}


//=============================================================================
//	END OF FILE
//=============================================================================