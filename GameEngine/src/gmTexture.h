//-----------------------------------------------------------------------------
//!
//!	@file	gmTexture.h
//!	@brief	テクスチャ
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	テクスチャ
//-----------------------------------------------------------------------------
class Texture
{
public:

	//! テクスチャハンドルの種類
	enum TEX_HANDLE
	{
		TEX_STANDARD,	// 標準のテクスチャ
		TEX_COLOR,		// カラーテクスチャ
		TEX_DEPTH,		// デプステクスチャ

		TEX_MAX
	};

	// 基本フォーマット
	enum FILE_FORMAT
	{
		FORMAT_JPG,
		FORMAT_BMP,
		FORMAT_GIF,
		FORMAT_TIFF,
		FORMAT_WMF,
		FORMAT_EMF,
		FORMAT_PNG,
		FORMAT_TGA,
		FORMAT_HDR,

		FORMAT_MAX,
		FORMAT_NONE
	};

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Texture()
	: _textureHandle(0xffffffffUL)
	, _width(0)
	, _height(0)
	, _mipLevelMax(-1)
	, _format(GL_NONE)
	, _internalFormat(-1)
	, _colorBuffer(0)
	, _depthBuffer(0)
	, _frameBuffer(0)
	, _fileFormat(FORMAT_NONE)
	{
	}
	//! デストラクタ
	virtual ~Texture() { cleanup(); }

	//! クラス内容解放
	void	cleanup();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! 空のテクスチャを作成
	//! @param	[in]	w	テクスチャの幅
	//! @param	[in]	h	テクスチャの高さ
	//! @retval	true	成功
	//! @retval	false	失敗
	bool		create(s32 w, s32 h);


	//! 画像ファイルを読み込みと転送
	//! @param	[in]	filePath		ファイル名(例"a.bmp")
	//!	@param	[in]	buildMipMaps	ミップマップを作成するかどうか
	//!	@param	[in]	isClamp			クランプするかどうか
	//!	@param	[in]	target			転送先
	//!	@param	[in]	mipLevel		ミップマップ段数
	//!	@parma	[in]	format			色のフォーマット
	//!	@param	[in]	type			１要素あたりのサイズ
	bool		loadAndTexImage2D(GM_CSTR filePath, 
								  bool	  buildMipMaps   = false,
								  bool	  isClamp	     = false,
								  GLenum  target	     = GL_TEXTURE_2D,
								  s32	  mipLevel	     = 0,
								  GLenum  format	     = GL_RGB,
								  GLenum  type		     = GL_UNSIGNED_BYTE
								  );
								  

	//! 描画ターゲットの作成
	//!	@param	[in]	w					幅
	//!	@param	[in]	h					高さ
	//!	@param	[in]	internalFormat		保持フォーマット(GL内部でどう持つか)
	//!	@param	[in]	format				ピクセルフォーマット
	//!	@param	[in]	type				色要素一つのタイプ
	//!	@param	[in]	buildMipMaps		ミップマップを作成するかどうか
	//!	@param	[in]	depthOnly			デプスバッファのみ作成するかどうか
	bool		createRenderTarget(s32 w, s32 h, GLint internalForamt=GL_RGBA, GLenum format=GL_RGBA, GLenum type = GL_FLOAT, bool buildMipMaps=true, bool depthOnly = false);

private:

	//! TGAファイルを読み込み
	//! @param	[in]	filePath		ファイル名(例"a.bmp")
	//!	@param	[in]	bytePerPixel	１要素のバイト数
	u8*			loadTGA(GM_CSTR filePath, /*u8*& image,*/ GLuint& bytePerPixel);

	//! GDI+で画像ファイル読み込み
	//! 【対応画像形式】BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	//! @param	[in]	filePath		ファイル名(例"a.bmp")
	//!	@param	[in]	bytePerPixel	１要素のバイト数
	//!	@return 画像データ配列
	u8*			loadByGdiplus(GM_CSTR filePath, GLuint bytePerPixel);

	//! HDRファイルを読み込む
	//! @param	[in]	filePath		ファイル名(例"a.bmp")
	//!	@param	[in]	bytePerPixel	１要素のバイト数
	u16*		loadHDR(GM_CSTR filePath, GLuint& bytePerPixel);

	//! 画像ファイルを読み込み
	//! 【対応画像形式】BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	//! @param	[in]	filePath		ファイル名(例"a.bmp")
	//!	@param	[in]	bytePerPixel	１要素のバイト数
	//!	@param	[in]	target			転送先
	void*		load(GM_CSTR filePath, GLuint& bytePerPixel, GLenum target);
	
	//! キューブマップかどうか
	//!	@param	[in]	target			転送先
	bool		isCubeMap(GLenum target);

public:

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! GLテクスチャハンドルを取得
	GLuint		getTextureHandle() const { return _textureHandle; }

	//! テクスチャタイプからハンドル取得
	GLuint		getTextureHandle(TEX_HANDLE handleType) const 
	{ 
		if( handleType == TEX_STANDARD ) {
			return getTextureHandle();
		}else if( handleType == TEX_COLOR ) {
			return getColorBuffer();
		}else {
			return getDepthBuffer();
		}
	}

	//! カラーバッファ取得
	GLuint		getColorBuffer() const { return _colorBuffer; }
	//! デプスバッファ取得
	GLuint		getDepthBuffer() const { return _depthBuffer; } 
	//! フレームバッファハンドル取得
	GLuint		getFrameBuffer() const { return _frameBuffer; }
	//! サイズ取得
	Size<s32>	getSize(s32 mipLevel = 0)	const { return Size<s32>(getWidth(mipLevel), getHeight(mipLevel)); }
	//! 幅取得
	s32			getWidth(s32 mipLevel = 0)	const { 

		f32 mipPow = powf(2.0f, (f32)mipLevel);
		f32 width  = max(_width/mipPow, 1.0f);
		return (s32)width; 
	}
	//! 高さ取得
	s32			getHeight(s32 mipLevel = 0)	const {

		f32 mipPow = powf(2.0f, (f32)mipLevel);
		f32 height  = max(_height/mipPow, 1.0f);
		return (s32)height; 
	}

	//! ミップマップ最大数取得
	s32		getMipLevelMax() const { return _mipLevelMax; }

	//! テクスチャをGLに設定
	//!	@param	[in]	pTexture	設定するテクスチャ
	//!	@param	[in]	texNum		設定するテクスチャ番号(GL_TEXTURE0 + texNum)
	static void set(Texture* pTexture, GLuint texNum = 0);
	
private:
	//! ファイルパスからフォーマット決定
	//! @param	[in]	filePath		ファイル名(例"a.bmp")
	bool		checkFileFormat(GM_CSTR filePath);


	//! フロートにコンバート
	//!	@param	[in]	pImage			変換元
	//!	@param	[in]	bytePerPixel	１要素のバイト数
	f32*		IntToFloat(u8* pImage, GLuint bytePerPixel);

	//! float から half floatに変換
	//!	@param	[in]	pImage			変換元
	//!	@param	[in]	bytePerPixel	１要素のバイト数
	u16*		FloatToHalf(f32* pImage, GLuint bytePerPixel);

	//@}
private:
	s32			_width;				//!< 幅
	s32			_height;			//!< 高さ
	s32			_mipLevelMax;		//!< ミップレベル数
	GLenum		_format;			//!< テクスチャフォーマット
	GLint		_internalFormat;	//!< 保持フォーマット(GL内部でどう持つか)
	GLuint		_textureHandle;		//!< OpenGLテクスチャハンドル
	GLuint		_colorBuffer;		//!< カラーバッファ
	GLuint		_depthBuffer;		//!< デプスバッファ
	GLuint		_frameBuffer;		//!< フレームバッファ
	FILE_FORMAT	_fileFormat;		//!< ファイルフォーマット
};
//=============================================================================
//	END OF FILE
//=============================================================================