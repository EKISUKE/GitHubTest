//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemRender.h
//!	@brief	描画管理システム
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//! テクスチャスタック用
struct StackTexture{
public:
	Texture*	_pTexture;		//!< ストックされたテクスチャ
	GM_CHAR		_name[256];		//!< 名前
};

//-----------------------------------------------------------------------------
//!	描画管理
//-----------------------------------------------------------------------------
class SystemRender
{
private:
	//-------------------------------------------------------------
	//!	@name	初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	SystemRender();

	//! コピーコンストラクタ
	SystemRender(SystemRender& src) {}

	//! デストラクタ
	virtual					~SystemRender();

public:
	//! 初期化
	bool					init();
	
	//! 解放
	void					cleanup();

	//@}
	//-------------------------------------------------------------
	//!	@name	制御
	//-------------------------------------------------------------
	//@{

	//! 描画の開始
	void					begin();

	//! 描画の終了
	void					end();

	//! 画面更新
	void					flip();

	//! フレーム描画開始
	void					bindFrame();
	
	//! 一つ前のものに戻す
	void					bindPrevRender();

private:
	//! フレームの描画終了
	void					unBindFrame();

public:
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
	//! @param	[in]	isFrameBuffer	フレームバッファかどうか(フレームバッファなら上下逆さまにしないで描画する。OpenGLのuvは左下原点なのでテクスチャ描画のときは上下逆さで描画しないといけないため)
	void					putTexture(Texture* pTexture, f32 x, f32 y, Color& color = Color::WHITE, Color& emissiveColor = Color::NONE, f32 scale = 1.0f, s32 shaderPass = -1, s32 mipLevel = 0, bool isCopy = false, bool isFrameBuffer = false);

	void					putTexture(GLuint	texHandle, f32 x, f32 y, Size<s32> texSize, Color& color = Color::WHITE, Color& emissiveColor = Color::NONE, f32 scale = 1.0f, s32 shaderPass = -1, s32 mipLevel = 0, bool isFrameBuffer = false);

	//! バックバッファテクスチャにコピー
	//! @param	[in]	pDest			コピー先テクスチャ
	//! @param	[in]	copyMipMax		コピーするミップ段数(0ならミップはコピーされない)
	void					copyBackBuffer(Texture* pDest, s32 copyMipMax = 0);

	//! バックバッファをスタックにセーブ
	//! @param	[in]	name	名前
	void					saveBackBuffer(GM_CSTR name);

	//! スタック描画(ミップマップを回りに表示)
	//! @param	[in]	pTexture	描画テクスチャ
	//! @param	[in]	offset		オフセット
	void					renderTexture(Texture* pTexture, Vector3& offset);
	//! スタックしたものを描画
	void					renderStackTexture();

	//! ミップマップを作成
	void					generateMipMap();
	
	//! HDRモードをLDRモードに戻す
	void					switchToLDR();

	//@}
	//-------------------------------------------------------------
	//!	@name	取得・設定
	//-------------------------------------------------------------
	//@{

	//! バックバッファの取得
	Texture*				getBackBuffer()			 const { return (_isHDR) ? _pBackBufferHDR : _pBackBuffer; }
	
	//! １フレーム前のバッファを取得
	Texture*				getPrevFrameBackBuffer() const { return _pPrevFrameBackBuffer; }


	//! 同じフレームバッファを持っているかどうか
	//!	@param	[in]	frame	調べるフレーム
	//! @param	[out]	number	見つかったフレームの番号(見つからなければ-1)
	//!	@return true : 持っている false : 持っていない
	bool					haveSameFrame(Texture* frame, s32& number);

	//! HDRモードの切り替え
	void					setHDR(bool enable) { _isHDR = enable; }

	////! 描画先設定
	////!	@param	[in]	color	描画先テクスチャハンドル(カラー)
	////!	@param	[in]	depth	描画先テクスチャハンドル(デプス)
	////! @return true : 成功 false : 失敗
	//bool					setRenderTarget(const GLuint& color, const GLuint& depth);

	//! 描画先設定
	//!	@param	[in]	pFrameBufferTexture	フレームバッファテクスチャ
	//!	@param	[in]	mipLevel			書き込むミップマップ番号
	//!	@param	[in]	pRenderSize			書き込むサイズ
	//! @return true : 成功 false : 失敗
	bool					setRenderTarget(Texture* pFrameBufferTexture, Float2& offset = Float2(0, 0), s32 mipLevel = -1, Size<s32>* pRenderSize = nullptr);

	
	//! HDRモードを取得
	bool					isHDR() const { return _isHDR; }

	//!	インスタンスを取得
	static SystemRender*	getInstance()
	{
		static SystemRender	_myInstance;
		return &_myInstance;
	}

	//! 描画されるかどうか(カリング)
	//! @param	[in]	mySphere 描画オブジェクトの球
	bool					isRender(Sphere& mySphere);

private:
	//! ストックテクスチャ内の同じ名前のものを取得
	//! @param	[in]	name	名前
	//!	@return	同じテクスチャ	nullptr ならない
	StackTexture*			getSame(GM_CSTR	name);
	

	//@}
private:
	vector<Texture*>		_frameInfoList;			//!< フレームバッファ情報のリスト
	s32						_currentFrame;			//!< 今のフレームの配列番号
	s32						_prevFrame;				//!< 一つ前のフレームの配列番号
	//GLuint					_currentBuffer[2];	//!< 今のバッファ    (1:Color 2:Depth)
	//GLuint					_prevBuffer[2];		//!< 一つ前のバッファ(1:Color 2:Depth)
	//GLuint					_frameBuffer;		//!< フレームバッファ
	Texture*				_pBackBuffer;			//!< バックバッファ
	Texture*				_pBackBufferHDR;		//!< HDRバックバッファ(fp16形式)
	Texture*				_pPrevFrameBackBuffer;	//!< 1フレーム前のバッファ
	bool					_isHDR;					//!< 現在のHDRモード true:fp16 false:RGBA8

	TaskFilterMotionBlur	_motionBlurFilter;		//!< モーションブラー
	TaskFilterBloom			_bloomFilter;			//!< ブルーム
	TaskFilterToneMapping	_toneMappingFilter;		//!< トーンマッピング
	TaskFilterGlare			_glareFilter;			//!< グレア
	TaskFilterLensFlare		_lensFlareFilter;		//!< レンズフレア

	vector<StackTexture*>	_copyTextureList;		//!< コピー用テクスチャ(途中で保存してそのときの画面知る用)
};

//-----------------------------------------------------------------------------
//	アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
SystemRender*	GmRender()
{
	return SystemRender::getInstance();
}

//=============================================================================
//	END OF FILE
//=============================================================================
