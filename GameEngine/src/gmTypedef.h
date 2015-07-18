//-----------------------------------------------------------------------------
//!
//!	@file	gmTypedef.h
//!	@brief	型宣言
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once
#pragma message(" + gmTypedef.h")

typedef   signed char		s8;			//!<  8bit符号あり整数
typedef	unsigned char		u8;			//!<  8bit符号なし整数
//typedef			 char		c8;			//!<  8bit整数

typedef   signed short		s16;		//!< 16bit符号あり整数
typedef	unsigned short		u16;		//!< 16bit符号なし整数

typedef   signed int		s32;		//!< 32bit符号あり整数
typedef	unsigned int		u32;		//!< 32bit符号なし整数

typedef   signed long long	s64;		//!< 64bit符号あり整数
typedef	unsigned long long	u64;		//!< 64bit符号なし整数

typedef float				f32;		//!< 単精度浮動小数点数
typedef double				f64;		//!< 倍精度浮動小数点数


/*
#define SAFE_RELEASE( PTR_ )		\
	if( PTR_ != NULL ) {			\
		PTR_->Release();			\
		PTR_ = NULL;				\
	}								\
*/

//---- GM関連
typedef char*		GM_STR;
typedef const char*	GM_CSTR;
typedef char		GM_CHAR;


//-----------------------------------------------------------------------------
//!	Float2
//-----------------------------------------------------------------------------
struct Float2
{
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{
	//! デフォルトコンストラクタ
	Float2() {}

	//! コンストラクタ
	Float2(f32 x, f32 y) : _x(x), _y(y) {}

	//@}

	//---- メンバ変数
	f32		_x;
	f32		_y;
};

//-----------------------------------------------------------------------------
//!	Float3
//-----------------------------------------------------------------------------
struct Float3 
{
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Float3() {}
	//! コンストラクタ
	Float3(f32 x, f32 y, f32 z) : _x(x), _y(y), _z(z) {}

	//@}

	//---- メンバ変数
	f32		_x;
	f32		_y;
	f32		_z;
};

//-----------------------------------------------------------------------------
//!	カラー構造体
//-----------------------------------------------------------------------------
struct Color
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Color() {}

	//! コンストラクタ(rgba値指定)
	Color(u8 r, u8 g, u8 b, u8 a = 255)
	: _r(r), _g(g), _b(b), _a(a) {}

	//@}

	//-------------------------------------------------------------
	//! @name 静的な定数
	//-------------------------------------------------------------
	//@{

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

	static Color		NONE;		//!< 無色

	//@}
public:
	//---- メンバ変数
	u8	_r;
	u8	_g;
	u8	_b;
	u8	_a;
};


//-----------------------------------------------------------------------------
//! サイズ構造体(テンプレート)
//-----------------------------------------------------------------------------
template<typename T>
struct Size
{
	//! デフォルトコンストラクタ
	Size()		   : _w(0), _h(0){}
	//! コンストラクタ
	Size(T w, T h) : _w(w), _h(h){}

	//! 演算子オーバーロード
	Size& operator / (Size s)
	{
		return Size(_w / s._w, _h / s._h);
	}

	T		_w;
	T		_h;
};


//-----------------------------------------------------------------------------
//!	安全な確保（ゲームエンジンオブジェクト）
//-----------------------------------------------------------------------------
template <typename T>
void SAFE_NEW(T*& p)
{
	if( p== nullptr ) {
		p = new T();
	}
}


//-----------------------------------------------------------------------------
//!	安全な解放（ゲームエンジンオブジェクト）
//-----------------------------------------------------------------------------
template <typename T>
void GM_RELEASE(T*& p)
{
	if( p != nullptr ) {
		p->release();
		p = nullptr;
	}
}

//-----------------------------------------------------------------------------
//!	安全な解放
//-----------------------------------------------------------------------------
template <typename T>
void SAFE_DELETE(T*& p)
{
	if( p != nullptr ) {
		delete p;
		p = nullptr;
	}
}


//---------------------------------------------------------------------------
//! メッセージ付きデバッグアサーション
//!	【使用方法】GM_ASSERT(条件, "メッセージ")
//!	【例】 GM_ASSERT(p != NULL, "ポインタがNULLです。")
//---------------------------------------------------------------------------
#define GM_ASSERT( CONDITION_, MESSAGE_ )	assert(CONDITION_ && MESSAGE_)





#define GM_ALIGN16	__declspec( align(16) )


#if 0

GM_ALIGN16 struct Vector4
{
	f32		_x;
	f32		_y;
	f32		_z;
	f32		_w;

	//! デフォルトコンストラクタ
	Vector4() {}
	//! コンストラクタ
	Vector4(f32 x, f32 y, f32 z, f32 w) : _x(x), _y(y), _z(z), _w(w) {}

};


GM_ALIGN16 struct Matrix
{
	Vector4		_m[4];

	//! デフォルトコンストラクタ
	Matrix() {}

	//! 初期化
	Matrix(
		f32 m11, f32 m12, f32 m13, f32 m14,
		f32 m21, f32 m22, f32 m23, f32 m24,
		f32 m31, f32 m32, f32 m33, f32 m34,
		f32 m41, f32 m42, f32 m43, f32 m44
		)
	{
		_m[0] = Vector4(m11, m12, m13, m14);
		_m[1] = Vector4(m21, m22, m23, m24);
		_m[2] = Vector4(m31, m32, m33, m34);
		_m[3] = Vector4(m41, m42, m43, m44);
	}

	//! Ｚ軸中心の回転
	static Matrix	rotZ(f32 radian)
	{
		f32	c = cos(radian);
		f32	s = sin(radian);
	
		return Matrix(
			   c,    s, 0.0f, 0.0f,
			  -s,    c, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}




};


#endif


// 構造体内のメンバ変数の先頭からのオフセット値を計算
//	【使用方法】u32 offset = GM_OFFSET_OF(Vector3, _y);
#define GM_OFFSET_OF( STRUCT_, MEMBER_VALUE_ )		(size_t)(&((STRUCT_*)0)->MEMBER_VALUE_)





//=============================================================================
// END OF FILE
//=============================================================================
