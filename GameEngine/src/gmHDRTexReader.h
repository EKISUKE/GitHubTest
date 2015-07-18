//-----------------------------------------------------------------------------
//!
//!	@file	gmHDRTexReader.h
//!	@brief	HDRテクスチャ読み込み
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


namespace HDR
{
	typedef		 BYTE		COLR[4];	// RGBE
	static const u32		R = 0;		// RED
	static const u32		G = 1;		// GREEN
	static const u32		B = 2;		// BLUE
	static const u32		E = 3;		// EXPOSUER


	// スキャンラインの順番のためのフラグ
	#define XDECR			1		// 負の方向に伸びるかどうか
	#define YDECR			2		// 負の方向に伸びるかどうか 
	#define YMAJOR			4		// 列と行のどちらが優先か
	#define MIN_E_LEN		8		// 最小画像解像度
	#define MAX_E_LEN		0x7fff	// 最大画像解像度
}


//-----------------------------------------------------------------------------
//!	HDRテクスチャ読み込み
//-----------------------------------------------------------------------------
class HDRTexReader
{
public:

	class	Reader;
	struct	Info;

private:
	

	//! 余分な改行を取り除く
	void	removeCRCF(GM_STR p);
	//! データの評価
	void	valData(Info* pInfo, GM_CSTR s);
	//! ヘッダの読み込み
	void	getHeader(Info* pInfo);
	//! サイズの読み込み
	bool	getResolution(Info* pInfo);
	//! スキャンラインの幅高さを設定する
	void	setScanWidthAndHeight(Info* pInfo);
	//! HDR ファイルの読み取り(フォーマットチェック)
	//!	@return true : フォーマットが正しい
	bool	checkHeader(Info* pInfo);
	//! 色情報を読み込む
	bool	readColor(HDR::COLR* color, s32 len);

	//! Radianceスキャンラインフォーマット→RGBE 変換
	//!	@param	[in]	scanLength		スキャンラインの長さ
	//!	@param	[in]	scanWidth		スキャンラインの幅
	//!	@param	[in]	bytePerPixel	１要素のバイト数
	u8*		RadianceToRGBE(s32 scanLength, s32 scanWidth, GLuint bytePerPixel);

	//! RGBE -> RGB変換
	//!	@param	[in]	pInfo  テクスチャ情報
	//!	@param	[in]	pImage 変換元
	u32*	RGBEToRGB(Info* pInfo, u8*& pImage);

	//! int -> half floatにして返す
	//!	@param	[in]	pInfo			テクスチャ情報
	//!	@param	[in]	pImage			変換元
	//!	@param	[in]	bytePerPixel	１要素のバイト数
	u16*	IntToHalfFloat(Info* pInfo, u32*& pImage, GLuint bytePerPixel);

public:

	//! HDR読み込み
	//!	@param	[in]	filePath	ファイルパス
	//! @param	[in]	w			幅保存先
	//!	@param	[in]	h			高さ保存先
	void*	loadHDR(GM_CSTR filePath, s32& w, s32& h);

private:
	HDRTexReader::Reader*		_pReader;
};

//-----------------------------------------------------------------------------
//!	HDRテクスチャの情報
//-----------------------------------------------------------------------------
struct HDRTexReader::Info
{
	GM_CHAR		_magicNumber[64];	//!< #?の後 "RADIANCE"のみ
	GM_CHAR		_fileFormat[64];	//!< フォーマット
	f64			_exposureTime;		//!< 露光時間
	Size<s32>	_imageSize;			//!< 画像サイズ
	u8			_flags;				//!< フラグ
	s32			_scanLength;		//!< スキャンラインの長さ
	s32			_scanWidth;			//!< スキャンラインの幅
};

//-----------------------------------------------------------------------------
//!	構文パーサー
//-----------------------------------------------------------------------------
class HDRTexReader::Reader
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	Reader(void);
	//! デストラクタ
	virtual ~Reader(void);

	//! 初期化
	bool				init(GM_CSTR fileName);
	//! 解放
	void				cleanup(void);

	//@}
	//-------------------------------------------------------------
	//! @name 取得/設定
	//-------------------------------------------------------------
	//@{

	//! 次のトークンを１つ取得
	//!	@return トークン文字列の先頭（次回getToken()を実行すると消滅する一時的なもの）
	//!	@return	NULLならファイル終端
	GM_CSTR				getToken(void);

	//! 次の一文字をとる(getTokenと同じく一時的なもの)
	//!	@return	NULLならファイル終端
	BYTE				getByte(bool* eof = NULL);
	//@}

	//! 現在のトークンと文字列比較
	bool				compare(GM_CSTR token);

	//! 次のトークンの出現まで進める
	bool				searchNextToken(GM_CSTR token);


private:
	static const s32	TOKEN_LENGTH_MAX = 512;
	GM_CHAR				_token[TOKEN_LENGTH_MAX];

	GM_STR				_pCurrent;

	GM_STR				_pBuffer;
	s32					_bufferSize;
};


//=============================================================================
//	END OF FILE
//=============================================================================