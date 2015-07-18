//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemRender.h
//!	@brief	描画管理システム
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"
//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
SystemRender::SystemRender()
//: _backBuffer    (0)
: _pBackBuffer	 (nullptr)
, _pBackBufferHDR(nullptr)
, _isHDR         (false)
, _currentFrame	 (0)
, _prevFrame	 (0)
{
//	glGenFramebuffers(1, &_frameBuffer);
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
SystemRender::~SystemRender()
{
	cleanup();
//	glDeleteFramebuffers(1, &_frameBuffer);
}

//-----------------------------------------------------------------------------
//!	初期化
//-----------------------------------------------------------------------------
bool SystemRender::init()
{

	//-------------------------------------------------------------
	// バックバッファを作成
	//-------------------------------------------------------------
	SAFE_NEW(_pBackBuffer);
	if( _pBackBuffer == nullptr ) return false;
	
	// 今現在ウィンドウの解像度を変更したときにバックバッファの解像度は変わらないため
	// バックバッファの解像度も変更する仕組みを作る予定
	Size<s32>	windowSize = GmWindow()->getResolution();
	if( !_pBackBuffer->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA, GL_RGBA, GL_FLOAT, true) ){
		GM_ASSERT(false, "バックバッファの作成に失敗しました。");
		return false;
	}
	//---- HDR用バックバッファを作成
	SAFE_NEW(_pBackBufferHDR);
	if( _pBackBufferHDR == nullptr ) return false;
	
	if (!_pBackBufferHDR->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
		GM_ASSERT(false, "HDRバッファの作成に失敗しました。");
		return false;
	}

	//---- 1フレーム前バックバッファを作成
	SAFE_NEW(_pPrevFrameBackBuffer);
	if (_pPrevFrameBackBuffer == nullptr) return false;

	if (!_pPrevFrameBackBuffer->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)) {
		GM_ASSERT(false, "1フレーム前保存用バッファの作成に失敗しました。");
		return false;
	}



	//---- フィルタの作成
	{
		// モーションブラー
		if(!_motionBlurFilter.setup()) {
			return false;
		}
		// ブルーム
		if(!_bloomFilter.setup()) {
			return false;
		}
		// トーンマッピング
		if(!_toneMappingFilter.setup()) {
			return false;
		}
		// グレアフィルタ
		if(!_glareFilter.setup()) {
			return false;
		}
		// レンズフレア
		if(!_lensFlareFilter.setup()) {
			return false;
		}
	}


	

	//glDisable(GL_CULL_FACE);	// 表裏カリング = OFF(両面)
	//glDisable(GL_LIGHTING);		// 固定パイプラインシェーダー無効化
	glEnable(GL_DEPTH_TEST);	// ＺバッファON

	return true;
}

//-----------------------------------------------------------------------------
//	解放
//-----------------------------------------------------------------------------
void SystemRender::cleanup()
{
	_lensFlareFilter.cleanup();
	_glareFilter.cleanup();
	_toneMappingFilter.cleanup();
	_bloomFilter.cleanup();
	_motionBlurFilter.cleanup();

	SAFE_DELETE(_pBackBuffer);
	SAFE_DELETE(_pBackBufferHDR);

	// スタックしておいたテクスチャを削除
	vector<StackTexture*>::iterator	itr;
	for(itr=_copyTextureList.begin(); itr!=_copyTextureList.end();){
		StackTexture* pStock	= *itr;
		Texture*	  pTexture	= pStock->_pTexture;
		SAFE_DELETE(pTexture);
		SAFE_DELETE(pStock);
		itr = _copyTextureList.erase(itr);
	}
}

//-----------------------------------------------------------------------------
//! 描画の開始
//-----------------------------------------------------------------------------
void SystemRender::begin()
{
	//---- シェーダの開始
	GmShader()->setPass(SystemShader::SHADER_TEXTURE);
	GmShader()->beginShader();
	//-------------------------------------------------------------
	// HDRレンダリングのためにfp16バッファをバックバッファにする
	//-------------------------------------------------------------
	setHDR(true);
	Texture* pCurrent = getBackBuffer();
	// 描画先を設定
	setRenderTarget(pCurrent);

	//-------------------------------------------------------------
	// 画面のクリア
	//-------------------------------------------------------------
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);			// Ｚバッファの初期化値
	/*glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);*/
	glDepthFunc(GL_LESS);		// デプス方法設定
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	Size<s32> windowSize = GmWindow()->getResolution();

	glViewport(0,0,windowSize._w, windowSize._h);
}

//-----------------------------------------------------------------------------
//! 描画の終了
//-----------------------------------------------------------------------------
void SystemRender::end()
{
	// バックバッファのミップマップ作成
	generateMipMap();
	// LDRバッファにコピー
	switchToLDR();

	// デプス情報をLDRバッファにコピーしておく(フィルタの影響を受けないもの用)
//	copyTexture(_pBackBufferHDR, _pBackBuffer, Texture::TEX_DEPTH);

	getGLError();
	//-------------------------------------------------------------
	// 描画終了（シーン終了）
	//-------------------------------------------------------------
	//---- シェーダの終了
	GmShader()->endShader();
}

//-----------------------------------------------------------------------------
//! 画面更新
//-----------------------------------------------------------------------------
void SystemRender::flip()
{

	// デフォルトのフレームバッファにする
	unBindFrame();

	drawBackBuffer(getBackBuffer());


	//glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_ALPHA_TEST);
	
	// バックバッファスワップ
	glutSwapBuffers();
}

//-----------------------------------------------------------------------------
//! フレーム描画開始
//-----------------------------------------------------------------------------
void SystemRender::bindFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _frameInfoList[_currentFrame]->getFrameBuffer());	
}

//-----------------------------------------------------------------------------
//! 一つ前のものに戻す
//-----------------------------------------------------------------------------
void SystemRender::bindPrevRender()
{
	// 今のフレーム番号を一つ前にもどす
	_currentFrame = _prevFrame;
	bindFrame();
}

//-----------------------------------------------------------------------------
//! フレームの描画終了
//-----------------------------------------------------------------------------
void SystemRender::unBindFrame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//-----------------------------------------------------------------------------
//! テクスチャを画面に描画
//! @param	[in]	pTexture		描画するテクスチャ
//! @param	[in]	color			ディフューズカラー
//! @param	[in]	emissiveColor	エミッシブカラー
//! @param	[in]	x				X座標
//! @param	[in]	y				Y座標
//! @param	[in]	scale			スケール(デフォルトは1.0f)
//! @param	[in]	shaderPass		シェーダの番号(デフォルトは-1)
//! @param	[in]	mipLevel		ミップマップ番号(デフォルトは0)
//! @param	[in]	isCopy			コピーかどうか(コピーならサイズをミップマップでとってこない)
//! @param	[in]	isFrameBuffer	フレームバッファかどうか
//!	(フレームバッファなら上下逆さまにしないで描画する。
//! OpenGLのuvは左下原点なのでテクスチャ描画のときは上下逆さで描画しないといけないため)
//-----------------------------------------------------------------------------
void SystemRender::putTexture(Texture* pTexture, f32 x, f32 y, Color& color, Color& emissiveColor, f32 scale, s32 shaderPass, s32 mipLevel, bool isCopy, bool isFrameBuffer)
{
	GLuint	texHandle = pTexture->getColorBuffer();
	if( texHandle == 0 ) {
		texHandle = pTexture->getTextureHandle();
	}
	s32 mip = 0;
	if(!isCopy) {
		mip = mipLevel;
	}
	Size<s32> textureSize = pTexture->getSize(mip); 
	putTexture(texHandle, x, y, textureSize, color, emissiveColor, scale, shaderPass, mipLevel, isFrameBuffer);
}

void SystemRender::putTexture(GLuint texHandle, f32 x, f32 y, Size<s32> texSize, Color& color, Color& emissiveColor, f32 scale, s32 shaderPass, s32 mipLevel, bool isFrameBuffer)
{
	Size<s32> screenSize  = GmWindow()->getResolution();

	f32 x1, y1, x2, y2;

	if(isFrameBuffer){
		x1 = (x						) * ( 2.0f / screenSize._w) - 1.0f;
		y1 = (y + texSize._h * scale) * (-2.0f / screenSize._h) + 1.0f;

		x2 = (x + texSize._w * scale) * ( 2.0f / screenSize._w) - 1.0f;
		y2 = (y						) * (-2.0f / screenSize._h) + 1.0f;
	}else{
		x1 = (x						) * ( 2.0f / screenSize._w) - 1.0f;
		y1 = (y						) * (-2.0f / screenSize._h) + 1.0f;

		x2 = (x + texSize._w * scale) * ( 2.0f / screenSize._w) - 1.0f;
		y2 = (y + texSize._h * scale) * (-2.0f / screenSize._h) + 1.0f;
	}

	static GLuint VB;
	static bool Initialized = false;
	if( !Initialized ) {
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FilterVertex) * 4, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Initialized = true;
	}

	//-------------------------------------------------------------
	// 頂点の作成
	//-------------------------------------------------------------
	/*const Vertex	v[4] = {
		{ Float3( x1,  y1, 0.0f), color, Float2(0.0f, 0.0f) },
		{ Float3( x2,  y1, 0.0f), color, Float2(1.0f, 0.0f) },
		{ Float3( x1,  y2, 0.0f), color, Float2(0.0f, 1.0f) },
		{ Float3( x2,  y2, 0.0f), color, Float2(1.0f, 1.0f) },
	};*/

	const FilterVertex v[4] = {
		{ Float3( x1,  y1, 0.0f), color, emissiveColor, Float2(0.0f, 0.0f) },
		{ Float3( x2,  y1, 0.0f), color, emissiveColor, Float2(1.0f, 0.0f) },
		{ Float3( x1,  y2, 0.0f), color, emissiveColor, Float2(0.0f, 1.0f) },
		{ Float3( x2,  y2, 0.0f), color, emissiveColor, Float2(1.0f, 1.0f) },
	};

	if( shaderPass == -1 ) {
		//! PUT_TEXTUREに変更
		GmShader()->changeShader(SystemShader::SHADER_PUT_TEXTURE);
		// シェーダーパスを更新
		shaderPass = SystemShader::SHADER_PUT_TEXTURE;
	}else{
		GmShader()->changeShader( (SystemShader::SHADER_MODE)shaderPass );
	}

	//---- 描画
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	

	//---- テクスチャ
	GmShader()->bindTexture("BaseTexture", texHandle, 0);
	//---- ミップ番号
	GmShader()->setUniform1f("gMipLod", (GLfloat)mipLevel);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

	getGLError();

	// 頂点バッファバインド
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// データ転送
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FilterVertex) * 4, v);


	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _position));
	GmShader()->setAndEnableVertexAttribute("uv"			, 2, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _uv));
	GmShader()->setAndEnableVertexAttribute("color"			, 4, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _diffuseColor));
	if( shaderPass == SystemShader::SHADER_PUT_TEXTURE ){
		GmShader()->setAndEnableVertexAttribute("emissiveColor"	, 4, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _emissiveColor));
	}

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("uv");
	GmShader()->DisableVertexAttribute("color");
	GmShader()->DisableVertexAttribute("emissiveColor");

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	GmShader()->unbindTexture();


	// 元に戻す
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	GmShader()->beginPrevShader();
}

//-----------------------------------------------------------------------------
//! バックバッファテクスチャにコピー
//! @param	[in]	pDest			コピー先テクスチャ
//! @param	[in]	copyMipMax		コピーするミップ段数
//-----------------------------------------------------------------------------
void SystemRender::copyBackBuffer(Texture* pDest, s32 copyMipMax)
{
	// 描画先設定
	Texture* pSrc = _frameInfoList[_currentFrame];
	s32		 mip  = 0;
	do {
		setRenderTarget(pDest, Float2(0, 0), mip);
		getGLError();
		// 画面にテクスチャ描画
		putTexture(pSrc, 0, 0, Color::WHITE, Color::WHITE, 1.0f, SystemShader::SHADER_FILTER, mip, true, true);
		// 次へ
		mip = min(mip + 1, copyMipMax);	// 最大値を超えない
	} while (mip < copyMipMax);
	// バックバッファに戻す
	setRenderTarget(pSrc, Float2(0, 0), 0);

}


//-----------------------------------------------------------------------------
//! バックバッファをスタックにセーブ
//! @param	[in]	name	名前
//-----------------------------------------------------------------------------
void SystemRender::saveBackBuffer(GM_CSTR name)
{

	//-------------------------------------------------------------
	//! コピー先テクスチャとコピー保存用構造体の作成
	//-------------------------------------------------------------
	Texture* pDest = nullptr;	// テクスチャ描画先
	bool	 isNew = false;		// 新規作成するかどうかフラグ
	// リストから探す
	StackTexture*  pStock = getSame(name);
	// 見つかったなら
	if(pStock){
		// テクスチャをストックのものにする
		pDest = pStock->_pTexture;
	}else{
		// 新しく作成する。
		SAFE_NEW(pDest);
		SAFE_NEW(pStock);
		// 新規作成フラグON
		isNew	= true;
		// データを設定
		pStock->_pTexture = pDest;
		// コピーサイズ計算
		s32 copySize = min(strlen(name) + 1, sizeof(pStock->_name) - 1);
		memcpy(pStock->_name, name, copySize);
	}

	// 新規作成するなら
	if( isNew ) {
		Size<s32>	windowSize = GmWindow()->getResolution();
		// HDR なら
		if(isHDR()) {
			if (!pDest->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB, true)){
				return;
			}
		}else{
			if (!pDest->createRenderTarget(windowSize._w, windowSize._h, GL_RGBA, GL_RGBA, GL_FLOAT, true)){
				return;
			}
		}
	}

	//-------------------------------------------------------------
	//! バックバッファからコピー先テクスチャにコピー
	//-------------------------------------------------------------
	const s32	mipLevelMax = pDest->getMipLevelMax();
	// バックバッファをコピー
	copyBackBuffer(pDest, mipLevelMax);


	//-------------------------------------------------------------
	//! 作成したコピーを保存
	//-------------------------------------------------------------
	// 新規作成したものなら
	if(isNew){
		// ストックしておく
		_copyTextureList.push_back(pStock);
	}

}

//-----------------------------------------------------------------------------
//! スタック描画(ミップマップを回りに表示)
//! @param	[in]	pTexture	描画テクスチャ
//! @param	[in]	offset		オフセット
//-----------------------------------------------------------------------------
void SystemRender::renderTexture(Texture* pTexture, Vector3& offset)
{
	const s32	mipLevelMax = pTexture->getMipLevelMax();
	s32			mip			= 0;
	f32			scale		= 0.2f;
	Vector3		position	= offset;

	// 最大解像度との比率
	Size<f32> resScale = GmWindow()->getRateResMax();

	if (mipLevelMax <= 0) {
		putTexture(pTexture, position._x, position._y, Color::WHITE, Color::WHITE, scale, SystemShader::SHADER_FILTER, mip, false, true);
	}
	else{
		Size<s32>	texSize = pTexture->getSize(mip);
		f32			originalW = texSize._w * scale;

		// ミップマップ付で描画
		do{
			f32 w = pTexture->getWidth(mip) * scale;
			putTexture(pTexture, position._x, position._y, Color::WHITE, Color::WHITE, scale, SystemShader::SHADER_FILTER, mip, false, true);
			// 最初の一回のみ引く
			if (mip == 0) {
				position._x -= originalW;
			}
			mip = min(mip + 1, mipLevelMax);	// 最大値以上にしない
			position._x += w;
			position._y  = texSize._h * scale;
		} while (mip < mipLevelMax);
	}
	

}
//-----------------------------------------------------------------------------
//! スタックしたものを描画
//-----------------------------------------------------------------------------
void SystemRender::renderStackTexture()
{
	//---- リストのものを全描画
	Vector3	offset = Vector3(0.0f);
	f32	scale  = 0.2f;
	vector<StackTexture*>::iterator	itr;
	for (itr = _copyTextureList.begin(); itr != _copyTextureList.end(); ++itr)
	{
		Texture* pTexture = (*itr)->_pTexture;
		//putTexture(pTexture, offset, 800.0f, Color::WHITE, scale, SystemShader::SHADER_FILTER);
		renderTexture(pTexture, offset);
		f32 w = pTexture->getWidth();
		offset._x += (w * scale);
	}

}

//-----------------------------------------------------------------------------
//! ミップマップを作成
//-----------------------------------------------------------------------------
void SystemRender::generateMipMap()
{
	Texture* current = getBackBuffer();
	glBindTexture(GL_TEXTURE_2D, current->getColorBuffer());
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//-----------------------------------------------------------------------------
//! HDRモードをLDRモードに戻す
//-----------------------------------------------------------------------------
void SystemRender::switchToLDR()
{

	// HDRバッファを使っていない場合、もしくはすでにLDRになっている場合は処理しない
	if( isHDR() == false ) return;

	//-------------------------------------------------------------
	// トーンマッピング
	// ※ LDRモード戻す
	//-------------------------------------------------------------
	if (Global::filterEnable) {
		_bloomFilter.render();
		_glareFilter.render();
		_motionBlurFilter.render();
		_lensFlareFilter.render();
	}
	
	

	// トーンマッピング前のテクスチャを保存しておく
	saveBackBuffer("トーンマッピング前\0");
	
	// 1フレーム前のバッファを保存
	copyBackBuffer(_pPrevFrameBackBuffer, _pPrevFrameBackBuffer->getMipLevelMax());


	_toneMappingFilter.render();

	
	// スタックしておいたテクスチャの描画
	if( Global::debugMode ){
		renderStackTexture();
	}

	// フィルターの影響を受けないものを描画する
	GmScene()->drawNoFilter();

	
}


//-----------------------------------------------------------------------------
//! 同じフレームバッファを持っているかどうか
//!	@param	[in]	frame	調べるフレーム
//! @param	[out]	number	見つかったフレームの番号(見つからなければ-1)
//!	@return true : 持っている false : 持っていない
//-----------------------------------------------------------------------------
bool SystemRender::haveSameFrame(Texture* frame, s32& number)
{
	// リスト検索
	for( u32 i=0; i < _frameInfoList.size(); ++i ) {
		Texture* checkFrame = _frameInfoList[i];
		if( checkFrame == frame ) { 
			// 見つかった番号を記録
			number = i;
			return true;
		}
	}

	// 見つからなかったので-1
	number = -1;
	return false;	
}

//-----------------------------------------------------------------------------
//! 描画先設定
//! 描画先設定
//!	@param	[in]	pFrameBufferTexture	フレームバッファテクスチャ
//!	@param	[in]	mipLevel			書き込むミップマップ番号
//!	@param	[in]	renderSize			書き込むサイズ
//! @return true : 成功 false : 失敗
//-----------------------------------------------------------------------------
bool SystemRender::setRenderTarget(Texture* pFrameBufferTexture, Float2& offset, s32 mipLevel, Size<s32>* pRenderSize)
{
	s32 number = 0;

	// 前回のフレーム番号記録
	_prevFrame    = _currentFrame;

	// もっていなければ
	if (!haveSameFrame(pFrameBufferTexture, number)) {
		// 新しく登録
		_frameInfoList.push_back(pFrameBufferTexture);
		// 今のフレームはリストのサイズ - 1
		_currentFrame = _frameInfoList.size() - 1;
	}else{
		_currentFrame = number;
	}

	bindFrame();

	getGLError();

	// ミップ番号が設定されていたら
	if( mipLevel != -1 ) {
		// ミップ番号設定する
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pFrameBufferTexture->getColorBuffer(), mipLevel);
	}

	if(pRenderSize == nullptr) {
		Size<s32> renderTargetSize = pFrameBufferTexture->getSize(mipLevel);
		pRenderSize = &renderTargetSize;
	}
	
	

	glViewport((GLint)offset._x, (GLint)offset._y, (GLsizei)pRenderSize->_w, (GLsizei)pRenderSize->_h);
	return true;
}

//-----------------------------------------------------------------------------
//! 描画されるかどうか(カリング)
//! @param	[in]	mySphere 描画オブジェクトの球
//-----------------------------------------------------------------------------
bool SystemRender::isRender(Sphere& mySphere)
{
	Frustum camFrustum = GmCameraMan()->getCurrentCamera()->getViewFrustum();
	return !camFrustum.isHit(mySphere);
}


//-----------------------------------------------------------------------------
//! スタックテクスチャ内の同じ名前のものを取得
//! @param	[in]	name	名前
//!	@return	同じテクスチャ nullptr ならない
//-----------------------------------------------------------------------------
StackTexture* SystemRender::getSame(GM_CSTR	name)
{
	//---- リストの中から検索する
	vector<StackTexture*>::iterator	itr;
	for(itr=_copyTextureList.begin(); itr!=_copyTextureList.end(); ++itr)
	{
		GM_CSTR pName = (*itr)->_name;
		// 同じ名前なら
		if(strcmp(name, pName) == 0){
			// データを返す
			return *itr;
		}
	}
	// 見つからなければnullptrを返す
	return nullptr;
}
//=============================================================================
//	END OF FILE
//=============================================================================
