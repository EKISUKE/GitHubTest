//-----------------------------------------------------------------------------
//!
//!	@file	Color.h
//!	@brief	カラー構造体
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	@struct カラー構造体
//-----------------------------------------------------------------------------
struct Color
{
public:
	//! デフォルトコンストラクタ
	Color() {}

	//! コンストラクタ(rgba値指定)
	Color(unsigned char r,
		  unsigned char g,
		  unsigned char b,
		  unsigned char a = 255)
	: _r(r), _g(g), _b(b), _a(a) {}

	//-------------------------------------------------------------
	// 静的な定数
	//-------------------------------------------------------------
	static Color		BLACK;		//!< 黒
	static Color		BLUE;		//!< 青
	static Color		RED;		//!< 赤
	static Color		MAGENTA;	//!< マゼンタ
	static Color		GREEN;		//!< 緑
	static Color		CYAN;		//!< シアン
	static Color		YELLOW;		//!< 黄
	static Color		WHITE;		//!< 白

	static Color		LTBLUE;		//!< 明るい青
	static Color		LTRED;		//!< 明るい赤
	static Color		LTMAGENTA;	//!< 明るいマゼンタ
	static Color		LTGREEN;	//!< 明るい緑
	static Color		LTCYAN;		//!< 明るいシアン
	static Color		LTYELLOW;	//!< 明るい黄
	static Color		LTGRAY;		//!< 明るいグレー

	static Color		DKBLUE;		//!< 暗い青
	static Color		DKRED;		//!< 暗い赤
	static Color		DKMAGENTA;	//!< 暗いマゼンタ
	static Color		DKGREEN;	//!< 暗い緑
	static Color		DKCYAN;		//!< 暗いシアン
	static Color		DKYELLOW;	//!< 暗い黄
	static Color		DKGRAY;		//!< 暗いグレー
public:
	unsigned char	_r;
	unsigned char	_g;
	unsigned char	_b;
	unsigned char	_a;
};

//===========================================================================
//	END OF FILE
//===========================================================================
