//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilterTonemapping.cpp
//!	@brief	トーンマッピングフィルター
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
TaskFilterToneMapping::TaskFilterToneMapping()
: _adaptationIndex(0)
{
	for( s32 i=0; i<2; ++i ) {
		_pAdaptationTexture[i] = nullptr;
	}
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
TaskFilterToneMapping::~TaskFilterToneMapping()
{
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool TaskFilterToneMapping::setup()
{
	// 親クラスの初期化を呼ぶ
	TaskFilter::setup();

	//----------------------------------------------------------
	// 順応テクスチャ(1x1)
	//----------------------------------------------------------
	for( s32 i=0; i<2; ++i ) {
		Texture*&	p = _pAdaptationTexture[i];

		SAFE_NEW(p);
		if( p == nullptr ) return false;

		if (!p->createRenderTarget(1, 1, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB)) {
			MessageBox(NULL, "RenderTargetの作成に失敗しました. " __FILE__, "ERROR", MB_OK);
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void TaskFilterToneMapping::cleanup()
{
	TaskFilter::cleanup();

	SAFE_DELETE(_pAdaptationTexture[0]);
	SAFE_DELETE(_pAdaptationTexture[1]);
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void TaskFilterToneMapping::render()
{
	static const s32	SHADER_STANDARD				= SystemShader::SHADER_FILTER;
	static const s32	SHADER_GET_LUMINANCE		= SystemShader::SHADER_GET_LUMINANCE;
	static const s32	SHADER_LUMINANCE_REDUCTION	= SystemShader::SHADER_LUMINANCE_REDUCTION;
	static const s32	SHADER_LIGHT_ADAPTATION		= SystemShader::SHADER_LIGHT_ADAPTATION;
	static const s32	SHADER_TONEMAPPING_FINAL	= SystemShader::SHADER_TONEMAPPING_FINAL;
	

	//-------------------------------------------------------------
	// (1) バックバッファからワークバッファにコピー
	// グレアの場合はぼかすので1/2バッファの縮小しておくことで
	// 後段パスが軽量化される(バイリニアで4ピクセル平均)
	//-------------------------------------------------------------
	copyToWorkTexture(SHADER_STANDARD, 0);

	// 半透明無効化
	setBlendMode(BM_NONE);
	//-------------------------------------------------------------
	// (2) 輝度成分を抽出し、log(対数)空間で輝度を保存する
	// 2x2の4ピクセル分を参照して1/2縮小も同時におこなう
	//-------------------------------------------------------------
	s32	mip	  = 1;
	s32	index = 0;

	setRenderTarget(index, mip);
	put(0, 0, SHADER_GET_LUMINANCE);

	//-------------------------------------------------------------
	// (3) 1x1ピクセルになるまで縮小を繰り返し、最高輝度を取得
	//-------------------------------------------------------------
	Texture*	pTexture = getWorkTexture(0);

	s32			mipLevelMax = pTexture->getMipLevelMax();

#if 1

	do{
		//---- ミップ段数を2段下にコピー(1/4縮小)
		s32	nextMip = min(mip + 2, mipLevelMax);	// 最大値以上にしない
		setRenderTarget(index, nextMip);
		put(index, mip, SHADER_LUMINANCE_REDUCTION);
		// ミップ段数を更新
		mip = nextMip;
	}while( mip < mipLevelMax );

	//GmRender()->setHDR(false);		// LDRバッファに切り替える
	//setBackBuffer();
	//put(index, mip, SHADER_STANDARD);
	//
	//return;
	// workTexture(0, mip)に集計結果がある

	//-------------------------------------------------------------
	// (4) 明順応 / 暗順応
	//-------------------------------------------------------------
	// 順応変換用テクスチャに今の結果を保存する
	setRenderTarget(_pAdaptationTexture[_adaptationIndex ^ 1]);

	put(index, mip, SHADER_LIGHT_ADAPTATION, _pAdaptationTexture[_adaptationIndex]);

	_adaptationIndex ^= 1;	// 切り替え

	//-------------------------------------------------------------
	// (4) 結果を貼り付ける
	//-------------------------------------------------------------
	GmRender()->setHDR(false);		// LDRバッファに切り替える
	setBackBuffer();

	s32	workTextureIndex = index;
	s32 mipLevel		 = mip;
	s32 shaderPass		 = SHADER_TONEMAPPING_FINAL;

	// BaseTexcture = 1x1に縮小したデータ
	// BaseTexture	= 元のHDRテクスチャ

	put(_pAdaptationTexture[_adaptationIndex], 0, shaderPass, getWorkTexture(0) );
#endif 
}


//=============================================================================
//	END OF FILE
//=============================================================================