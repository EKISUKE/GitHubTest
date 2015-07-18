//-----------------------------------------------------------------------------
//!
//!	@file	gmTexture.cpp
//!	@brief	テクスチャ
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//=============================================================================
//	テクスチャの実装
//=============================================================================

//-----------------------------------------------------------------------------
//! クラス内容解放
//-----------------------------------------------------------------------------
void Texture::cleanup()
{
	if( _textureHandle != 0xffffffffUL ) {
		glDeleteTextures(1, &_textureHandle);
		_textureHandle = 0xffffffffUL;
	}
}

//-----------------------------------------------------------------------------
//! 空のテクスチャを作成
//! @param	[in]	w	テクスチャの幅
//! @param	[in]	h	テクスチャの高さ
//! @retval	true	成功
//! @retval	false	失敗
//-----------------------------------------------------------------------------
bool Texture::create(s32 w, s32 h)
{
	//-------------------------------------------------------------
	// ① テクスチャを生成
	//-------------------------------------------------------------
	glGenTextures(1, &_textureHandle);

	//-------------------------------------------------------------
	// ② テクスチャをGPUに設定
	//-------------------------------------------------------------
	glBindTexture(GL_TEXTURE_2D, _textureHandle);

	//-------------------------------------------------------------
	// ③ 画像イメージをVRAMへ転送
	//-------------------------------------------------------------

	//---- 画像に必要なメモリを確保
	u8*	pImage = new u8 [w * h * 4];

	for( s32 y=0; y<h; y++ ) {
		for( s32 x=0; x<w; x++ ) {
			u8* p = &pImage[(y * w + x) * 4];

			p[0] = rand();		// R
			p[1] = rand();		// G
			p[2] = rand();		// B
			p[3] = rand();		// A
		}
	}

	_internalFormat = 4;
	//---- 転送
	glTexImage2D(
				 GL_TEXTURE_2D,
				 0,					// ミップマップ段数
				 _internalFormat,	// コンポーネント数 3:RGB 4:RGBA
				 w,					// 幅
				 h,					// 高さ
				 0,					// ボーダーカラー 1:あり 0:なし
				 _format,			// [転送元] コンポーネント
				 GL_UNSIGNED_BYTE,	// [転送元] の1要素あたりのサイズ
				 &pImage[0]);		// [転送元] イメージのメモリの先頭

	//---- ワークメモリを解放
	delete [] pImage;

	return true;
}

//-----------------------------------------------------------------------------
//! TGAファイルを読み込み
//! @param	[in]	filePath		ファイル名(例"a.bmp")
//!	@param	[in]	bytePerPixel	１要素のバイト数
//-----------------------------------------------------------------------------
u8* Texture::loadTGA(GM_CSTR filePath, GLuint& bytePerPixel)
{
	// tgaの読み込み
	FILE *fp;

	GLubyte header[18];

	// ファイルを開く
	if( fopen_s(&fp, filePath, "rb") != 0 )
	{
		return false;
	}

	// ヘッダ情報の読み込み
	fread(header, 1, sizeof(header), fp);

	f32 w,h;
	// 幅と高さを決める
	w = header[13] * 256 + header[12];
	h = header[15] * 256 + header[14];
		
	// ビットの深さ
	GLuint bpp = header[16];
		
	// そのビットの深さによってフォーマット決定
	if( bpp == 24 )
	{
		_format = GL_RGB;
	}else if( bpp == 32 ){
		_format = GL_RGBA;
	}

	// 1ピクセルのバイト数を求める
	bytePerPixel = bpp/8;

	// 画像サイズを計算
	s32 imageSize = w * h * bytePerPixel;
	// サイズを保存
	_width  = w;
	_height = h;

	// ■ロードする画像のメモリの解像度を変更/設定(この位置に任意に記述して下さい)
	u8* pImage = new u8 [imageSize];

	// テクセルデータを一気に読み取り
	fread(pImage, 1, imageSize, fp);

	// BGR(A)をRGB(A)にコンバート
	for( s32 i=0; i<imageSize; i+= bytePerPixel)
	{
		u8 temp		= pImage[i];
		pImage[i]	= pImage[i+2];
		pImage[i+2] = temp;
	}

	// 順番変更
	for( s32 i=0; i<imageSize; i++ )
	{
		s32 partIndex	  = imageSize - 1 - i;
		u8 temp			  = pImage[i];
		pImage[i]		  = pImage[partIndex];
		pImage[partIndex] = temp;
	}

	// ファイルを閉じる
	fclose(fp);

	return &pImage[0];
	//return (u32*)pImage;
}


//-----------------------------------------------------------------------------
//! GDI+で画像ファイル読み込み
//! 【対応画像形式】BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
//! @param	[in]	filePath		ファイル名(例"a.bmp")
//!	@param	[in]	bytePerPixel	１要素のバイト数
//!	@return 画像データ配列	nullptrなら失敗
//-----------------------------------------------------------------------------
u8* Texture::loadByGdiplus(GM_CSTR filePath, GLuint bytePerPixel)
{
	// 文字コードをワイド文字列に変換
	// 【注意】本来はこの箇所は文字列バッファ長の考慮のほかに
	// 文字列終端コードを処理するよりセキュアな対応が好ましい
	wchar_t		path[MAX_PATH];
	size_t		pathLength = 0;
	// ロケール指定
	setlocale(LC_ALL, "japanese");

	if (mbstowcs_s(&pathLength,     // [out]    変換された文字数
		&path[0],					// [out]    変換されたワイド文字列を格納するバッファのアドレス(変換先)
		MAX_PATH,					// [in]     出力側のバッファのサイズ(単位:文字数)
		filePath,					// [in]     マルチバイト文字列のアドレス(変換元)
		_TRUNCATE) != 0) {			// [in]     出力先に格納するワイド文字の最大数
		return nullptr;
	}

	bool	result = false;
	u8*		pImage = nullptr;

	// GDI+オブジェクト(画像展開に必要)
	Gdiplus::GdiplusStartupInput	gdiplusStartupInput;
	ULONG_PTR						gdiplusToken;

	//---- GDI+の初期設定
	if (Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) != Gdiplus::Ok) {
		return false;
	}

	//---- 画像ファイルを開く
	//	  【対抗画像形式】 BMP, JPEG, PNG, GIF, TIFF, WMF, EMF
	Gdiplus::Bitmap*		pBitmap = Gdiplus::Bitmap::FromFile(path);
	if (pBitmap && pBitmap->GetLastStatus() == Gdiplus::Ok) {
		//---- 画像サイズ分の領域確保
		_width  = pBitmap->GetWidth();
		_height = pBitmap->GetHeight();


		// ■ロードする画像のメモリの解像度を変更/設定(この位置に任意に記述して下さい)
		pImage = new u8[_width * _height * bytePerPixel];

		// メモリ確保できていれば
		if (pImage != nullptr) {
			//---- 画像イメージの読み込み
			for (s32 y = 0; y<_height; y++) {
				for (s32 x = 0; x<_width; x++) {
					Gdiplus::Color	srcColor;
					pBitmap->GetPixel(x, y, &srcColor);

					u8 r = srcColor.GetR();
					u8 g = srcColor.GetG();
					u8 b = srcColor.GetB();
					u8 a = srcColor.GetA();

					// ■ピクセルへの描画(この位置に任意に記述してください)
					// setPixel(x, y, r, g, b, a);

					// x, y番目のピクセルのアドレスを取得
					u8	*pDst = pImage + (y * _width + x) * bytePerPixel;
					// そのピクセルに色を設定
					pDst[0] = r;		// Red
					pDst[1] = g;		// Green
					pDst[2] = b;		// Blue
					if (bytePerPixel == 4) {	// RGBAのときのみ書き込み
						pDst[3] = a;		// Alpha
					}
				}
			}
		}

		delete pBitmap;

		//---- GDI+の開放
		Gdiplus::GdiplusShutdown(gdiplusToken);

	}else{
		//---- GDI+の開放
		Gdiplus::GdiplusShutdown(gdiplusToken);
		return nullptr;
	}

	return &pImage[0];
	
}

//-----------------------------------------------------------------------------
//! HDRファイルを読み込む
//! @param	[in]	filePath		ファイル名(例"a.bmp")
//!	@param	[in]	bytePerPixel	１要素のバイト数
//-----------------------------------------------------------------------------
u16* Texture::loadHDR(GM_CSTR filePath, GLuint& bytePerPixel)
{
	u16* pImage;
	//u32* pImage;

	HDRTexReader	hdrReader;

	pImage = (u16*)hdrReader.loadHDR(filePath, _width, _height);
	if( pImage ==nullptr )
	{
		return nullptr;
	}


	return pImage;
}

//-----------------------------------------------------------------------------
//! キューブマップかどうか
//!	@param	[in]	target			転送先
//-----------------------------------------------------------------------------
bool Texture::isCubeMap(GLenum target)
{
	static const GLenum checkList[] = {
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	};

	// 一つでも一致すればキューブマップである
	static const s32 count = sizeof(checkList) / sizeof(GLenum);
	for(s32 i=0; i<count; ++i){
		if(checkList[i] == target){
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
//! 画像ファイルを読み込み
//! 【対応画像形式】BMP, JPEG, PNG, GIF, TIFF, WMF, EMF,TGA
//! @param	[in]	fileName		ファイル名(例"a.bmp")
//!	@param	[in]	bytePerPixel	１要素のバイト数
//!	@param	[in]	target			転送先
//-----------------------------------------------------------------------------
void*  Texture::load(GM_CSTR filePath, GLuint& bytePerPixel, GLenum target)
{
	//-------------------------------------------------------------
	// ① テクスチャを生成
	//-------------------------------------------------------------
	glGenTextures(1, &_textureHandle);

	//-------------------------------------------------------------
	// ② テクスチャをGPUに設定
	//-------------------------------------------------------------
	if(!isCubeMap(target)) {
		glBindTexture(target, _textureHandle);
	}

	//-------------------------------------------------------------
	// ③ 画像の読み込み
	//-------------------------------------------------------------

	void* pImage = nullptr;
	

	//---- ファイルフォーマットによって読み込み関数を呼び分け
	switch(_fileFormat)
	{
	case FORMAT_TGA:	// TGA読み込み
		pImage = loadTGA(filePath, bytePerPixel);
		break;
	case FORMAT_HDR:
		pImage		 = loadHDR(filePath, bytePerPixel);
		bytePerPixel = 3;	// 変換し終わったらRGB
		break;
	default:
		pImage = loadByGdiplus(filePath, bytePerPixel);
		break;
	}

	if(pImage == nullptr){
		return nullptr;
	}


	return pImage;

}


//-----------------------------------------------------------------------------
//! 画像ファイルを読み込みと転送
//! @param	[in]	filePath		ファイル名(例"a.bmp")
//!	@param	[in]	target			転送先
//!	@param	[in]	buildMipMaps	ミップマップを作成するかどうか
//!	@param	[in]	mipLevel		ミップマップ段数
//!	@parma	[in]	format			色のフォーマット
//!	@param	[in]	type			１要素あたりのサイズ
//-----------------------------------------------------------------------------
bool Texture::loadAndTexImage2D(GM_CSTR filePath, 
								bool	buildMipMaps,
								bool	isClamp,
								GLenum  target,
								s32		mipLevel,
								GLenum  format,
								GLenum  type)
{

	// フォーマットを設定(デフォルト状態)
	_format = format;
	// ファイルフォーマット設定
	if( checkFileFormat(filePath) )
	{
		GLuint bytePerPixel;
		if (_format == GL_RGBA){
			bytePerPixel = 4;
		}else{
			bytePerPixel = 3;
		}


		// フォーマットがHDRでなけば
		if (_fileFormat != FORMAT_HDR) {
			// 同じにする
			_internalFormat = _format;
		}else{
			// フォーマットはRGBにもどす。
			_format			= GL_RGB;
			_internalFormat = GL_RGB16F;
			type			= GL_HALF_FLOAT;
			//type			= GL_UNSIGNED_BYTE;
		}


		void*	pImage = load(filePath, bytePerPixel, target);
	

		/*if(type == GL_FLOAT){
			pImage = IntToFloat((u8*)pImage, bytePerPixel);
		}else if(type == GL_HALF_FLOAT){
			pImage = IntToFloat((u8*)pImage, bytePerPixel);
			pImage = FloatToHalf((f32*)pImage, bytePerPixel);
		}*/


		

		if( pImage != nullptr ) {
			if (buildMipMaps) {
				gluBuild2DMipmaps(GL_TEXTURE_2D, _internalFormat, _width, _height, _format, type, pImage);
				glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);					// 拡大時のフィルタ(GL_LINEAR_MIPMAP_LINEARはサポートしていない)
				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// 縮小時のフィルタ
				f32	maxSize = (f32)(max(_width, _height));
				_mipLevelMax = (s32)(log((f32)maxSize) / log(2.0f));

			}else if( target == GL_TEXTURE_1D ){
				glTexImage1D(
					target,				// 転送先
					0,					// ミップマップ段数
					_internalFormat,	// GL内部でどう持つか
					_width,				// 幅
					0,					// ボーダーカラー 1:あり 0:なし
					_format,			// [転送元] コンポーネント
					type,				// [転送元] の1要素あたりのサイズ	
					pImage);			// [転送元] イメージのメモリの先頭

				glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			}else{

				//---- 転送
				glTexImage2D(
					target,				// 転送先
					0,					// ミップマップ段数
					_internalFormat,	// コンポーネント数 3:RGB 4:RGBA
					_width,				// 幅
					_height,			// 高さ
					0,					// ボーダーカラー 1:あり 0:なし
					_format,			// [転送元] コンポーネント
					type,				// [転送元] の1要素あたりのサイズ	
					pImage);			// [転送元] イメージのメモリの先頭

				if(!isCubeMap(target)) {
					glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				}
			}

			// クランプフラグがONならクランプ
			if( isClamp ) 
			{
				glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			// 一時オブジェクト解放
			delete [] pImage;
			pImage = nullptr;

			if(!isCubeMap(target)) {
				glBindTexture(target, 0);
			}

			return true;
		}else{
			glBindTexture(target, 0);
			glDeleteTextures(1, &_textureHandle);
		}

	}	// checkFileFormat()


	glBindTexture(target, 0);
	// 読み込み失敗ファイル名を出力する。
	MessageBoxA(nullptr, filePath, "ファイル名", MB_OK);
	return false;
}
//-----------------------------------------------------------------------------
//! 描画ターゲットの作成
//!	@param	[in]	w					幅
//!	@param	[in]	h					高さ
//!	@param	[in]	internalFormat		保持フォーマット(GL内部でどう持つか)
//!	@param	[in]	format				ピクセルフォーマット
//!	@param	[in]	type				色要素一つのタイプ
//!	@param	[in]	buildMipMaps		ミップマップを作成するかどうか
//!	@param	[in]	depthOnly			デプスバッファのみ作成するかどうか
//-----------------------------------------------------------------------------
bool Texture::createRenderTarget(s32 w, s32 h, GLint internalForamt, GLenum format,  GLenum type, bool buildMipMaps, bool depthOnly)
{

	//----------------------------------------------------------
	// カラーバッファを作成、バインドして転送
	//----------------------------------------------------------
	// デプスのみなら作成しない
	if(!depthOnly) {
		glGenTextures(1, &_colorBuffer);
		glBindTexture(GL_TEXTURE_2D, _colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, internalForamt, w, h, 0, format, type, nullptr);
	
		// ミップマップを作成するなら
		if( buildMipMaps ) {
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);				// 拡大時のフィルタ(GL_LINEAR_MIPMAP_LINEARはサポートしていない)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	// 縮小時のフィルタ
			getGLError();
			f32	maxSize  = (f32)( max(w, h) );
			_mipLevelMax = (s32)( log((f32)maxSize) / log(2.0f) );
		}else{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	//----------------------------------------------------------
	// デプスバッファを作成、バインドして転送
	//----------------------------------------------------------
	glGenTextures(1, &_depthBuffer);
	glBindTexture(GL_TEXTURE_2D, _depthBuffer);
	//glTexImage2D(GL_TEXTURE_2D, mipLevel, GL_DEPTH_COMPONENT,		   w,		   h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexImage2D(GL_TEXTURE_2D,		   0, GL_DEPTH_COMPONENT, (GLsizei)w, (GLsizei)h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);								// [転送元] イメージのメモリの先頭

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	getGLError();

	// テクスチャ解放
	glBindTexture(GL_TEXTURE_2D, 0);

	// フレームバッファを作成しテクスチャをバインド
	glGenFramebuffers(1, &_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
	// デプスのみならバインドしない
	if(!depthOnly){
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer, 0);
	}
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT , GL_TEXTURE_2D, _depthBuffer, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
		return false;

	// デフォルトのレンダーターゲットに戻す
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//---- 記録しておく
	_width  = w;
	_height = h;
	_format = format;

	return true;
}

//-----------------------------------------------------------------------------
//! テクスチャをGLに設定
//!	@param	[in]	pTexture			設定するテクスチャ
//!	@param	[in]	texNum		設定するテクスチャ番号(GL_TEXTURE0 + texNum)
//-----------------------------------------------------------------------------
void Texture::set(Texture* pTexture, GLuint texNum)
{
	GM_ASSERT(texNum <= 31, "テクスチャの有効番号を越えています");
	if( pTexture ) {	//---- テクスチャON
		glActiveTexture(GL_TEXTURE0 + texNum);
		glEnable(GL_TEXTURE_2D);
		GLuint	textureHandle = pTexture->getTextureHandle();
		glBindTexture(GL_TEXTURE_2D, textureHandle);

		// テクスチャの品質設定
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {				//---- テクスチャOFF
		glActiveTexture(GL_TEXTURE0 + texNum);
		glDisable(GL_TEXTURE_2D);
	}
}


//-----------------------------------------------------------------------------
//! ファイルパスからフォーマット決定
//! @param	[in]	filePath		ファイル名(例"a.bmp")
//-----------------------------------------------------------------------------
bool Texture::checkFileFormat(GM_CSTR filePath)
{
	static const char *extensions[] =
	{
		".jpg",
		".bmp",
		".gif",
		".tif",
		".wmf",
		".emf",
		".png",
		".tga",
		".hdr",
	};

	//---- 拡張子からフォーマット決定
	string newPath = filePath;
	// .の位置取得
	s32	   extPos = newPath.rfind(".");
	if (extPos > 0){
		// その位置のアドレス取得
		char *tail = &newPath[extPos];
		for (u32 i=0; i<FORMAT_MAX; ++i) {
			if (strncmp(tail, extensions[i], 4) == 0) {
				if (i >= FORMAT_PNG /*&& i < FORMAT_HDR*/) {
					_format = GL_RGBA;
				/*}else if( i== FORMAT_HDR ) {
					_format = GL_RGBA32F;*/
				}
				_fileFormat = (FILE_FORMAT)i;
				break;
			}
		}
	}

	if(_format == FORMAT_NONE) {
		MessageBoxA(nullptr, "対応していないフォーマットです。", "テクスチャ読み込みエラー", MB_OK);
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------
//! フロートにコンバート
//!	@param	[in]	pImage			変換元
//!	@param	[in]	bytePerPixel	１要素のバイト数
//-----------------------------------------------------------------------------
f32* Texture::IntToFloat(u8* pImage, GLuint bytePerPixel)
{

	// 画像サイズを計算
	s32 imageSize = _width * _height * bytePerPixel;
	f32* result	  = new f32[imageSize];
	
	for (s32 i = 0; i<imageSize; ++i) {
		result[i] = common::ByteToFloat(pImage[i]);
	}

	// 前のデータは削除する
	SAFE_DELETE(pImage);

	return result;
}

//-----------------------------------------------------------------------------
//! float から half floatに変換
//!	@param	[in]	pImage			変換元
//!	@param	[in]	bytePerPixel	１要素のバイト数
//-----------------------------------------------------------------------------
u16* Texture::FloatToHalf(f32* pImage, GLuint bytePerPixel)
{
	// 画像サイズを計算
	s32 imageSize = _width * _height * bytePerPixel;
	u16* result = new u16[imageSize];

	for (s32 i = 0; i<imageSize; ++i) {
		result[i] = common::Float32to16(pImage[i]);
	}

	// 前のデータは削除する
	SAFE_DELETE(pImage);

	return result;
}

//=============================================================================
//	END OF FILE
//=============================================================================
