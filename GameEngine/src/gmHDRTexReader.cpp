//-----------------------------------------------------------------------------
//!
//!	@file	gmHDRTexReader.cpp
//!	@brief	HDRテクスチャ読み込み
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"
using namespace HDR;


//=============================================================================
//	HDRテクスチャ読み込みクラスの実装
//=============================================================================

//-----------------------------------------------------------------------------
//! 余分な改行を取り除く
//-----------------------------------------------------------------------------
void HDRTexReader::removeCRCF(GM_STR p)
{
	for (s32 len = strlen((GM_CSTR)(p)) - 1; 0 <= len; len--)
	{
		// 改行がなければ終了
		if( p[len] != '\r' && p[len]!='\n' ) break;
		// 改行があれば終端コードいれる
		p[len] = '\0';
	}
}

//-----------------------------------------------------------------------------
//! データの評価
//-----------------------------------------------------------------------------
void HDRTexReader::valData(Info* pInfo, GM_CSTR s)
{
	if( strncmp(s, "#?", 2) == 0 )
	{
		// マジックナンバー
		strcat(pInfo->_magicNumber, s+2);
		removeCRCF(pInfo->_magicNumber);

	}else if( strncmp(s, "FORMAT=", 7) == 0 ) {
		// フォーマット
		strcat(pInfo->_fileFormat, s+7);
		removeCRCF(pInfo->_fileFormat);
	}
}

//-----------------------------------------------------------------------------
//! ヘッダの読み込み
//-----------------------------------------------------------------------------
void HDRTexReader::getHeader(Info* pInfo)
{
	while(true) {
		GM_CSTR token =  _pReader->getToken();

		// ヘッダ情報が終わったら終了
		if(_pReader->compare("-Y") || _pReader->compare("+X")) return;

		// データを比較して情報を取得
		valData(pInfo, token);
	}
}

//-----------------------------------------------------------------------------
//! サイズの読み込み
//-----------------------------------------------------------------------------
bool HDRTexReader::getResolution(Info* pInfo)
{
	//「-Y M +X N」のときは続くデータがN行のデータがM列ある構造
	// ここにきたときは-Yか+Xまできてなければヘッダ読み込み失敗している
	if(!_pReader->compare("-Y") && !_pReader->compare("+X")) return false;

	s32 w,h;
	if(_pReader->compare("-Y")) {			// -Yが先なら
		 h = atoi(_pReader->getToken());
		 // +Xをとばす
		 _pReader->getToken();
		 w = atoi(_pReader->getToken());
		 _pReader->getByte();	// \n飛ばし
		 pInfo->_flags |= YMAJOR;
	}else if (_pReader->compare("+X")) {	// +Xが先なら
		w = atoi(_pReader->getToken());
		// -Yをとばす
		_pReader->getToken();
		h = atoi(_pReader->getToken());
		_pReader->getByte();	// \n飛ばし
	}

	// サイズが0以下なら失敗
	if(w <= 0 || h <= 0 ) return false;

	// サイズを格納
	pInfo->_imageSize = Size<s32>(w,h);
	
	// スキャンラインの幅高さを設定する
	setScanWidthAndHeight(pInfo);

	return true;
}

//-----------------------------------------------------------------------------
//! スキャンラインの幅高さを設定する
//-----------------------------------------------------------------------------
void HDRTexReader::setScanWidthAndHeight(Info* pInfo)
{
	if(pInfo->_flags & YMAJOR){
		pInfo->_scanLength = pInfo->_imageSize._w;
		pInfo->_scanWidth  = pInfo->_imageSize._h;
	}else{
		pInfo->_scanLength = pInfo->_imageSize._h;
		pInfo->_scanWidth  = pInfo->_imageSize._w;
	}
}

//-----------------------------------------------------------------------------
//! HDR ファイルの読み取り(フォーマットチェック)
//!	@return true : フォーマットが正しい
//-----------------------------------------------------------------------------
bool HDRTexReader::checkHeader(Info* pInfo)
{
	//---- データの初期化
	pInfo->_magicNumber[0]	= '\0';
	pInfo->_fileFormat[0]	= '\0';
	pInfo->_exposureTime	= 1.0000000000000;
	pInfo->_flags			= 0;
	pInfo->_scanLength		= 0;
	pInfo->_scanWidth		= 0;


	// ヘッダ情報の読み取り
	getHeader(pInfo);

	// ヘッダ情報のチェック
	if( pInfo->_magicNumber[0] == '\0' || strcmp("RADIANCE", pInfo->_magicNumber) )			return false;

	// フォーマットチェック
	if( pInfo->_fileFormat[0] == '\0' || strcmp("32-bit_rle_rgbe", pInfo->_fileFormat) )	return false;

	// サイズ読み込み
	if( !getResolution(pInfo) ) return false;

	return true;
}

//-----------------------------------------------------------------------------
//! 色情報を読み込む
//! @param	[in]	len		スキャンするラインの長さ
//-----------------------------------------------------------------------------
bool HDRTexReader::readColor(HDR::COLR* color, s32 len)
{
	s32	i, j;
	s32 code, val;

	// 読み込みタイプを決める
	// 最新なら最初4つの成分が
	// [ 2 | 2 | 画像サイズの上位7bit | 画像サイズの下位8bit ]
	if( len < MIN_E_LEN || MAX_E_LEN < len ) {
		// 古いタイプ
	}

	i = _pReader->getByte();

	if( i != 2 ) {
		// 最初が2でないのは古いタイプ

	}

	bool checkEOF = true;
	// 最低４つのデータが必要
	color[0][G] = _pReader->getByte();
	color[0][B] = _pReader->getByte();
	i = _pReader->getByte(&checkEOF);
	if (checkEOF) return false;

	// 最初のデータが"22"とかになっていなかったり、
	// ３バイト目のデータについて最大数を超えているかのチェック
	if (color[0][G] != 2 || color[0][B] & 128) {
		color[0][R] = 2;
		color[0][E] = i;
//		return(s_OldReadColors(scanline + 1, len - 1, fp));
	}

	// 長さ情報の確認(整合性)
	if( (color[0][B] << 8 | i) != len ) return false;

	// 各成分を読み込む
	for(i=0; i<4; ++i)
	{
		for(j=0; j<len; )
		{
			// データが途切れている
			code = _pReader->getByte(&checkEOF);
			if(checkEOF) return false;

			// コードが128以上なら圧縮されている
			if( 128 < code ) {
				// 長さ+値のランレングス
				code &= 127;
				val = _pReader->getByte();
				while(code--)	color[j++][i] = val;
			}else{
				// ランレングス圧縮されていないものがcode個続く
				while (code--)	color[j++][i] = _pReader->getByte();
			}
		}
	}
}

//-----------------------------------------------------------------------------
//! Radianceスキャンラインフォーマット→RGBE 変換
//!	@param	[in]	scanLength	スキャンラインの長さ
//!	@param	[in]	scanWidth	スキャンラインの幅
//!	@param	[in]	bytePerPixel	１要素のバイト数
//-----------------------------------------------------------------------------
u8* HDRTexReader::RadianceToRGBE(s32 scanLength, s32 scanWidth, GLuint bytePerPixel)
{
	s32			ret = 0;
	HDR::COLR*	scanColor;
	s32			x, y;
	
	u8*			pImage = nullptr;

	// メモリの確保
	if ((scanColor = (HDR::COLR *)malloc(scanLength*sizeof(HDR::COLR))) == nullptr)
	{
		MessageBoxA(nullptr, "メモリーの確保に失敗しました。", "エラー", MB_OK);
		// メモリ解放
		free((u8*)scanColor);
		return nullptr;
	}


	s32 w = scanWidth;
	s32 h = scanLength;
	// 保存先のメモリを確保する
	pImage = new u8[w * h * bytePerPixel];
	//srcImage = new u32[w * h * bytePerPixel];


	// 画像の変換
	for(y=0; y<scanWidth; ++y) 
	{
		if(!readColor(scanColor, scanLength)) 
		{
			MessageBoxA(nullptr, "カラーの読み込みに失敗しました。", "エラー", MB_OK);
			break;
		}

		
		for(x=0; x<scanLength; ++x) 
		{
			// x, y番目のピクセルのアドレスを取得
			u8* pDst = pImage + (y * scanWidth + x) * bytePerPixel;

			pDst[0] = scanColor[x][R];	// red
			pDst[1] = scanColor[x][G];	// green
			pDst[2] = scanColor[x][B];	// blue
			pDst[3] = scanColor[x][E];	// exposure

			/*pDst[0] = pDst[0] *  pow(2, (pDst[3] - 128));
			pDst[1] = pDst[1] *  pow(2, (pDst[3] - 128));
			pDst[2] = pDst[2] *  pow(2, (pDst[3] - 128));*/
		}

	}


	// メモリ解放
	free((u8*)scanColor);

	return pImage;
}

//-----------------------------------------------------------------------------
//! RGBE -> RGB変換
//!	@param	[in]	pInfo  テクスチャ情報
//!	@param	[in]	pImage 変換元
//-----------------------------------------------------------------------------
u32* HDRTexReader::RGBEToRGB(Info* pInfo, u8*& pImage)
{
	s32  w					= pInfo->_scanWidth;
	s32  h					= pInfo->_scanLength;
	s32  bytePerPixel		= 3;					// RGBに変換するために4ではなく3
	s32  bytePerPixelSrc	= 4;					// RGBE用のバイト/ピクセル
	u32* convertResult		= new u32[w * h * bytePerPixel];

	// 変換していく
	for(s32 y=0; y<h; ++y)
	{
		for (s32 x = 0; x<w; ++x)
		{
			// x, y番目のピクセルのアドレスを取得
			u8* pSrc  = pImage		 + (y * w + x) * bytePerPixelSrc;
			u32* pDst = convertResult + (y * w + x) * bytePerPixel;
			
			pDst[0] = (u32)( pSrc[0] *  pow(2, (pSrc[3] - 128)) );
			pDst[1] = (u32)( pSrc[1] *  pow(2, (pSrc[3] - 128)) );
			pDst[2] = (u32)( pSrc[2] *  pow(2, (pSrc[3] - 128)) );
		}
	}

	// 変換し終わったのでpImageは削除する
	SAFE_DELETE(pImage);
	// ポインタ
	return convertResult;
}


//-----------------------------------------------------------------------------
//! int -> half floatにして返す
//!	@param	[in]	pInfo			テクスチャ情報
//!	@param	[in]	pImage			変換元
//!	@param	[in]	bytePerPixel	１要素のバイト数
//-----------------------------------------------------------------------------
u16* HDRTexReader::IntToHalfFloat(Info* pInfo, u32*& pImage, GLuint bytePerPixel)
{
	s32  w			   = pInfo->_scanWidth;
	s32  h			   = pInfo->_scanLength;
	s32  imageSize	   = w * h * bytePerPixel;
	u16* convertResult = new u16[imageSize];

	for(s32 i=0; i<imageSize; ++i){
		f32 floatVal	 = common::IntToFloat(pImage[i]);
		convertResult[i] = common::Float32to16(floatVal);
	}

	// 変換し終わったのでpImageは削除する
	SAFE_DELETE(pImage);

	return convertResult;

}

//-----------------------------------------------------------------------------
//! HDR読み込み
//!	@param	[in]	filePath	ファイルパス
// !@param	[in]	w			幅保存先
//!	@param	[in]	h			高さ保存先
//-----------------------------------------------------------------------------
void* HDRTexReader::loadHDR(GM_CSTR filePath, s32& w, s32& h)
{

	Info	info;

	void* pImage = nullptr;

	// 構文パーサーで一括読み込み
	_pReader = new HDRTexReader::Reader();
	if( !_pReader->init(filePath) ) {
		MessageBoxA(nullptr, "構文パーサーでの一括読み込みに失敗しました", "エラー", MB_OK );
		return false;
	}

	// ヘッダを調べる
	if( !checkHeader(&info) )
	{
		MessageBoxA(nullptr, "HDRテクスチャのフォーマットがおかしいです。", "エラー", MB_OK);
		return false;
	}

	// コンバートする
	if ( ( pImage = RadianceToRGBE(info._scanLength, info._scanWidth, 4) ) == nullptr){
		// 失敗したので解放
		SAFE_DELETE(pImage);
		return nullptr;
	}

	// RGBに変換する
	pImage = RGBEToRGB(&info, (u8*&)pImage);

	// halfFloatに変換する
	pImage = IntToHalfFloat(&info, (u32*&)pImage, 3);

	w = info._imageSize._w;
	h = info._imageSize._h;


	return pImage;
}



//=============================================================================
//	HDRTexReader::Reader			構文パーサー
//=============================================================================

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
HDRTexReader::Reader::Reader(void)
	: _pCurrent(nullptr)
	, _pBuffer(nullptr)
	, _bufferSize(0)
{
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
HDRTexReader::Reader::~Reader(void)
{
	cleanup();
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool HDRTexReader::Reader::init(GM_CSTR fileName)
{
	//-------------------------------------------------------------
	// ファイルを読み込む
	//-------------------------------------------------------------
	HANDLE	hFile = CreateFileA(
		fileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY,
		nullptr);

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBoxA(nullptr, fileName, "ファイル読み込みに失敗しました。", MB_OK);
		return false;
	}

	// ファイルサイズを取得
	u32		size = GetFileSize(hFile, nullptr);
	_pBuffer = new GM_CHAR[size + 1];
	_pBuffer[size] = ' ';

	_bufferSize = size + 1;
	// メモリ上のバッファに一括読み込み
	DWORD	readSize;
	ReadFile(hFile, _pBuffer, size, &readSize, nullptr);

	// ファイルを閉じる
	CloseHandle(hFile);


	_pCurrent = _pBuffer;
	return true;
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void HDRTexReader::Reader::cleanup(void)
{
	if (_pBuffer) {
		delete[] _pBuffer;
		_pBuffer = nullptr;
	}
}

//-----------------------------------------------------------------------------
//! 次のトークンを１つ取得
//!	@return トークン文字列の先頭（次回getToken()を実行すると消滅する一時的なもの）
//!	@return	NULLならファイル終端
//-----------------------------------------------------------------------------
GM_CSTR HDRTexReader::Reader::getToken(void)
{
	GM_STR& p	  = _pCurrent;
	GM_STR	pTail = _pBuffer + _bufferSize;

	// 最後まできていたら終了
	if( p >= pTail )	return nullptr;

	//---- 区切り記号、空白、改行をスキップ
	while( *p == ' ' || *p == '\t' || *p == 0x0a || *p == 0x0d )
	{
		p++;
		if (p >= pTail) return nullptr;
	}

	s32	count = 0;

	//---- 次の区切りまで検索
	do { 
		_token[count] = *p;

		// 次の文字へ
		count++;
		p++;

		// 最後まできていたら終了
		if (p >= pTail)	return nullptr;

		// 空白、改行があればループ終了
	} while ( !(*p == ' ' || *p == '\t' || *p == 0x0a || *p == 0x0d) );


	_token[count] = '\0';

	return _token;

}

//-----------------------------------------------------------------------------
//! 次の一文字をとる(getTokenと同じく一時的なもの)
//!	@return	NULLならファイル終端
//-----------------------------------------------------------------------------
BYTE HDRTexReader::Reader::getByte(bool* eof)
{
	GM_STR& p	  = _pCurrent;
	GM_STR	pTail = _pBuffer + _bufferSize;

	if (eof) *eof = false;
	// 最後まできていたら終了
	if (p >= pTail)	{
		if (eof) *eof = true;
	}

	s32	count = 0;

	//---- 次の区切りまで検索
	BYTE b = *p;

	// 次の文字へ
	count++;
	p++;

	return b;
}

//-----------------------------------------------------------------------------
//! 現在のトークンと文字列比較
//-----------------------------------------------------------------------------
bool HDRTexReader::Reader::compare(GM_CSTR token)
{
	return strncmp(_token, token, strlen(_token)) == 0;
}

//-----------------------------------------------------------------------------
//! 次のトークンの出現まで進める
//-----------------------------------------------------------------------------
bool HDRTexReader::Reader::searchNextToken(GM_CSTR token)
{
	// 一致する文字列まで進む
	while( compare(token) == false ) {
		if( getToken() == nullptr ) {
			return false;
		}
	}
	// 一致する文字を発見
	return true;
}

//=============================================================================
//	END OF FILE
//=============================================================================
