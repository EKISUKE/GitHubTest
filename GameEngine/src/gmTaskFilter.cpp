//-----------------------------------------------------------------------------
//!
//!	@file	gmTaskFilter.cpp
//!	@brief	ポストエフェクトフィルタ基底
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
TaskFilter::TaskFilter()
{
	_pWorkTexture[0] = nullptr;
	_pWorkTexture[1] = nullptr;
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
TaskFilter::~TaskFilter()
{
}


//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool TaskFilter::setup(Size<s32>& workTexResolution)
{
	//-------------------------------------------------------------
	// 描画ターゲットテクスチャの作成
	//-------------------------------------------------------------
	// ミップ段数指定で縮小バッファ作成
	for( s32 i=0; i<2; i++ ) {
		SAFE_NEW(_pWorkTexture[i]);
		if( _pWorkTexture[i] == nullptr ) return false;
		
		if (!_pWorkTexture[i]->createRenderTarget(workTexResolution._w, workTexResolution._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
			MessageBox(NULL, "RenderTargetの作成に失敗しました。"__FILE__, "ERROR", MB_OK);
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void TaskFilter::cleanup()
{
	SAFE_DELETE(_pWorkTexture[0]);
	SAFE_DELETE(_pWorkTexture[1]);
}

//-----------------------------------------------------------------------------
//! 描画
//-----------------------------------------------------------------------------
void TaskFilter::render()
{
	//-------------------------------------------------------------
	// バックバッファの内容をテクスチャにコピー
	//-------------------------------------------------------------
	copyToWorkTexture(3);
	
	//-------------------------------------------------------------
	// コピーしたテクスチャを使用してシェーダで描き戻す
	//-------------------------------------------------------------
	s32	workTextureIndex = 0;
	s32 mipLevel		 = 3;
	s32	shaderPass		 = 3;

	put(workTextureIndex, mipLevel, shaderPass);
}


//-----------------------------------------------------------------------------
//! バックバッファからワークテクスチャにコピー
//! @param	[in]	shaderPass  コピーするときに使うシェーダ番号
//!	@param	[in]	mipLevel	コピー先のミップレベル
//!	@param	[in]	isPrevFrame	1フレーム前のものをコピーするかどうか
//-----------------------------------------------------------------------------
void TaskFilter::copyToWorkTexture(s32 shaderPass, s32 mipLevel, bool isPrevFrame)
{
	//-------------------------------------------------------------
	//! バックバッファからワークテクスチャにコピー
	//-------------------------------------------------------------
	Texture* pSrc;
	if(isPrevFrame){
		pSrc = GmRender()->getPrevFrameBackBuffer();
	}else{
		pSrc = GmRender()->getBackBuffer();
	}
	Texture* pDest = _pWorkTexture[0];

	// 描画先変更
	//setRenderTarget(pDest, 0);
	setRenderTarget(pDest, mipLevel);
	

	// 画面にテクスチャ描画
	put(pSrc, mipLevel, shaderPass);

	// 元に戻す
	setBackBuffer();
	
}

//-----------------------------------------------------------------------------
//! ワークテクスチャを全画面に張り付け
//! @param	[in]	index		ワークテクスチャの番号(0 or 1)
//! @param	[in]	mipLevel	ミップレベル
//!	@param	[in]	shaderPass	シェーダーパス番号(-1ならこの中でシェーダを開始、終了しない)
//!	@param	[in]	pSubTexture	サブテクスチャ
//-----------------------------------------------------------------------------
void TaskFilter::put(s32 index, s32 mipLevel, s32 shaderPass, Texture* pSubTexture)
{
//	glDisable(GL_CULL_FACE);
	put(getWorkTexture(index), mipLevel, shaderPass, pSubTexture);
//	glEnable(GL_CULL_FACE);
}

void TaskFilter::put(Texture* pTexture, s32 mipLevel, s32 shaderPass, Texture* pSubTexture)
{

	//-------------------------------------------------------------
	// 幅と高さを自動で取得
	//-------------------------------------------------------------
	//---- テクスチャの幅と高さ
	s32		tw = pTexture->getWidth(mipLevel);
	s32		th = pTexture->getHeight(mipLevel);

	//-------------------------------------------------------------
	// シェーダ定数(Shader Constant)の設定
	//-------------------------------------------------------------
	//---- シェーダの変更
	// シェーダパスが-1以外なら
	if( shaderPass != -1 ) {
		// シェーダを変更
		GmShader()->changeShader((SystemShader::SHADER_MODE)shaderPass);
	}

	//---- テクスチャサイズ
	Vector4	 textureSize((f32)tw, (f32)th, 1.0f/(f32)tw, 1.0f/(f32)th);
	GmShader()->setUniform4fv("gTextureSize", (GLfloat*)&textureSize);
	//---- ミップ番号
	GmShader()->setUniform1f("gMipLod", (GLfloat)mipLevel);
	//---- テクスチャ
	GmShader()->bindTexture("BaseTexture", pTexture->getColorBuffer(), 0);

	if( pSubTexture ) {
		GmShader()->bindTexture("BaseTexture2", pSubTexture->getColorBuffer(), 1);
	}
	//-------------------------------------------------------------
	// 描画 (Draw)
	//-------------------------------------------------------------
	static GLuint VB;
	static bool	Initialized = false;
	if( !Initialized ) {
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Initialized = true;
	}

	const Vertex	v[4] = {
		{ Float3(-1.0f, -1.0f, 0.0f), Color(255,255,255,255), Float2(0.0f, 0.0f) },
		{ Float3(+1.0f, -1.0f, 0.0f), Color(255,255,255,255), Float2(1.0f, 0.0f) },
		{ Float3(-1.0f, +1.0f, 0.0f), Color(255,255,255,255), Float2(0.0f, 1.0f) },
		{ Float3(+1.0f, +1.0f, 0.0f), Color(255,255,255,255), Float2(1.0f, 1.0f) }
	};


	//---- 描画
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	// 頂点バッファバインド
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// データ転送
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4, v);

	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _position));
	GmShader()->setAndEnableVertexAttribute("uv"			, 2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _uv));
	GmShader()->setAndEnableVertexAttribute("color"			, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _color));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("uv");
	GmShader()->DisableVertexAttribute("color");

	GmShader()->unbindTexture();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

//	glDisable(GL_TEXTURE_2D);
	// 元に戻す
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	// シェーダパスが-1以外なら
	if( shaderPass != -1 ) {
		// 以前のシェーダに戻す
		GmShader()->beginPrevShader();
	}
//	glPopAttrib();
}

//-----------------------------------------------------------------------------
//! 描画ターゲットの設定
//! @param	[in]	index		ワークテクスチャの番号(0 or 1)
//!	@param	[in]	mipLevel	ミップレベル
//-----------------------------------------------------------------------------
void TaskFilter::setRenderTarget(s32 index, s32 mipLevel)
{
	setRenderTarget( getWorkTexture(index), mipLevel );
}
void TaskFilter::setRenderTarget(Texture* pRenderTarget, s32 mipLevel)
{
	GmRender()->setRenderTarget(pRenderTarget, Float2(0,0), mipLevel);
}

//-----------------------------------------------------------------------------
//! 描画ターゲットをバックバッファに戻す
//-----------------------------------------------------------------------------
void TaskFilter::setBackBuffer()
{
	s32 mipLevel = 0;
	GmRender()->setRenderTarget(GmRender()->getBackBuffer(), Float2(0,0), mipLevel);
}


//=============================================================================
//	END OF FILE
//=============================================================================