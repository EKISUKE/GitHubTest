//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterMotionBlur.cpp
//!	@brief	モーションブラー
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
TaskFilterMotionBlur::TaskFilterMotionBlur()
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
TaskFilterMotionBlur::~TaskFilterMotionBlur()
{
}

Texture debugTexture;

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool TaskFilterMotionBlur::setup()
{
	// 親クラスの初期化を呼ぶ
	TaskFilter::setup();

debugTexture.loadAndTexImage2D("../Image/GameClear.png");
	return true;
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void TaskFilterMotionBlur::cleanup()
{
	TaskFilter::cleanup();
}

//-----------------------------------------------------------------------------
//! カメラからスクリーンまでの距離を求める
//!	@param	[in]	screenW		画面の解像度(幅)
//!	@param	[in]	screenH		画面の解像度(高さ)
//!	@param	[in]	acpect		アスペクト比
//!	@param	[in]	fov			画角
//!	@return	カメラからスクリーンまでの距離
//-----------------------------------------------------------------------------
f32 getScreenDistance(f32 screenW, f32 screenH, f32 aspect, Radian fov)
{
	f32 distance = 0.0f;

	//					  　　    +|
	//					  　　  ／||
	//					  　　／  ||
	//					  　／	  ||
	//					  ／	  ||
	//					／		  ||
	//				  ／		  || スクリーンサイズ ÷ 2
	//				／))fov		г||
	// カメラ位置 ● -------------||
	//			   |← distance →|
	//
	//  screenW / 2
	// ------------- = tan(fov)
	//   distance
	//			   screenW / 2
	// distance = --------------
	//				tan(fov)

	distance = (screenW / 2) / tanf(fov);

	return distance;
}


//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void TaskFilterMotionBlur::render()
{

	static const s32	SHADER_STANDARD			= SystemShader::SHADER_FILTER;

	//-------------------------------------------------------------
	// (1) バックバッファからワークバッファにコピー
	//-------------------------------------------------------------
	copyToWorkTexture(SHADER_STANDARD, 0);

#if 1

	// 半透明無効化
	setBlendMode(BM_NONE);
	//-------------------------------------------------------------
	// (2) 方向を指定
	//-------------------------------------------------------------
	Size<s32>	windowSize	= GmWindow()->getResolution();
	f32 screenDistance		= getScreenDistance(windowSize._w, windowSize._h, 16.0f / 9.0f, Degree(45.0f));

	Matrix viewMatrix		= GmScene()->getViewMatrix();
	Matrix cameraMatrix		= viewMatrix.inverseFast();
	Matrix cameraMatrixLast = _viewMatrixLast.inverseFast();

	// カメラのスクリーン上の中心座標の移動量を計算
	Vector3 screenPosition	   = Vector3(cameraMatrix._m[3])	 - Vector3(cameraMatrix._m[2])	   * screenDistance;
	Vector3 screenPositionLast = Vector3(cameraMatrixLast._m[3]) - Vector3(cameraMatrixLast._m[2]) * screenDistance;

	// ワールド座標の点がスクリーン上でどれくらい動いたかを計算
	Vector3 screenUpDir		= cameraMatrix._m[1];		// カメラの上方向のベクトル
	Vector3 screenRightDir	= cameraMatrix._m[0];		// カメラの右方向のベクトル

	Vector3	move	= screenPosition - screenPositionLast;	// ワールドでの移動量

	Vector3	blurDir;
	blurDir._x = Vector3::dot(move, screenRightDir);
	blurDir._y = Vector3::dot(move, screenUpDir) * -1.0f;
	blurDir._z = 0.0f;


	// 次のフレームのために保存
	_viewMatrixLast = viewMatrix;

	f32 scale = 0.005f;
//	f32 scale = 0.01f;

	blurDir *= scale;


	GmShader()->changeShader(SystemShader::SHADER_MOTION_BLUR);

	// ブラー方向設定
	GmShader()->setUniform2fv("gDir2D", (GLfloat*)& blurDir);
	//-------------------------------------------------------------
	// (3) ブラーを累計していく (4pixel単位)
	//-------------------------------------------------------------
	s32		index = 0;
	s32		mip   = 0;


	for( s32 i=0; i<3; i++ ) {
		
		GmShader()->setUniform1f("gStride", (GLfloat)(pow(4, (f32)i)) );

		//---- ブラー
		setRenderTarget(index ^ 1, mip);
		put(index, mip);

		index ^= 1;
	}

	GmShader()->beginPrevShader();

	setBackBuffer();

	//-------------------------------------------------------------
	// (4) バックバッファに書き戻す
	//-------------------------------------------------------------
	s32	workTextureIndex = 0;
	s32 mipLevel		 = mip;
	s32	shaderPass		 = SHADER_STANDARD;

	put(workTextureIndex, mipLevel, shaderPass);
#endif 
}


//=============================================================================
//	END OF FILE
//=============================================================================