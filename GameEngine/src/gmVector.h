//-----------------------------------------------------------------------------
//!
//!	@file	gmVector.h
//!	@brief	ベクトル演算
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//---- プロトタイプ宣言
class Degree;
class Radian;
//-----------------------------------------------------------------------------
//! 角度 (degree 0度～360度)
//-----------------------------------------------------------------------------
class Degree
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Degree() {}

	//! コンストラクタ(f32直接渡し)
	explicit Degree(f32 degree);

	//! コンストラクタ(radian→degree変換)
	Degree(const Radian& radian);

	//! コピーコンストラクタ
	Degree(const Degree& degree) { _degree = degree._degree; }


	//@}
	//-------------------------------------------------------------
	//! @name 演算子オーバーロード
	//-------------------------------------------------------------
	//@{

	//! floatへキャスト
	operator	f32() const { return _degree; }

	//! 乗算
	Degree		operator * (f32 n) const { return Degree(_degree * n); }
	//! 加算
	Degree&		operator += (Degree n) { _degree += n._degree; return *this; }
	//! 減算
	Degree&		operator -= (Degree n) { _degree -= n._degree; return *this; }
	//! 乗算	
	Degree&		operator *= (f32 n) { _degree *= n; return *this; }

	//@}
public:
	f32			_degree;
};

//-----------------------------------------------------------------------------
//! 角度 (radian 0～2π)
//-----------------------------------------------------------------------------
class Radian
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Radian() {}

	//! コンストラクタ(f32直接渡し)
	explicit Radian(f32 radian);

	//! コンストラクタ(degree→radian変換)
	Radian(const Degree& degree);

	//! コピーコンストラクタ
	Radian(const Radian& radian) { _radian = radian._radian; }

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! floatへキャスト
	operator	f32() const { return _radian; }

	//! 加算
	Radian		operator + (f32 n) const { return Radian(_radian + n); }
	//! 乗算
	Radian		operator * (f32 n) const { return Radian(_radian * n); }
	
	//! 加算
	const Radian operator += (f32 n)    { _radian += n; return *this; }

	//! 加算
	const Radian operator += (Radian n) { _radian += n._radian; return *this; }


	//@}
public:
	f32			_radian;

};

//---- プロトタイプ宣言
struct Matrix;
struct Vector4;

//-----------------------------------------------------------------------------
//! ３要素ベクトル構造体
//-----------------------------------------------------------------------------
struct Vector3
{

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Vector3();

	//! コンストラクタ
	Vector3(f32 val);

	//! コンストラクタ
	Vector3(f32 x, f32 y, f32 z);

	//! コピーコンストラクタ
	Vector3(const Vector4& v);

	//@}
	//-------------------------------------------------------------
	//! @name 演算子オーバーロード
	//-------------------------------------------------------------
	//@{

	//! キャスト
	operator Float3& () const { return *(Float3*)&_x; }

	//! 配列アクセス (const)
	//!	@param	[in]	index	アクセス番号	0=x  1=y  2=z
	f32		operator [] (u32 index) const { return (&_x)[index]; }

	// 加算
	//!	@param	[in]	v	相手のベクトル
	Vector3	operator + ( const Vector3& v ) const;

	// 減算
	//!	@param	[in]	v	相手のベクトル
	Vector3 operator - ( const Vector3& v ) const;

	// 乗算
	//!	@param	[in]	v	相手のベクトル
	Vector3 operator * ( const Vector3& v ) const;

	// スカラ乗算
	//!	@param	[in]	s	拡大率
	Vector3 operator * ( float s ) const;


	//! 加算
	const Vector3& operator += (const Vector3& v);
	
	//! 減算
	const Vector3& operator -= (const Vector3& v);
	
	//! 乗算
	const Vector3& operator *= (const Vector3& v);
	
	//! スカラ乗算
	const Vector3& operator *= (f32 s);

	//! 正の値
	Vector3 operator + () const;

	//! 負の値
	Vector3 operator - () const;

	//---- 比較演算
	//! 等しい
	bool	operator == (const Vector3& v);
	//! 等しくない
	bool	operator != (const Vector3& v);


	//@}
	//-------------------------------------------------------------
	//! @name 変換関数
	//-------------------------------------------------------------
	//@{

	//! ベクトルの長さ
	f32		length() const;

	//! ベクトルの長さの二乗(長さ比較用)
	f32		squareLength() const;

	//! 正規化
	//! @return 正規化済みベクトル
	Vector3		normalize() const;

	//! ベクトルの変換
	//! v' ← this * m;
	//!	@param	[in]	m	ベクトル変換のための行列
	//!	@return	変換後のベクトル
	Vector3		transform(const Matrix& m);

	//! 内積
	//!	@param	[in]	a	ベクトルA
	//!	@param	[in]	b	ベクトルB
	//! @return 内積結果
	static f32		dot(const Vector3& a, const Vector3& b);


	//! 外積
	//!	@param	[in]	a	ベクトルA
	//!	@param	[in]	b	ベクトルB
	//! @return 外積結果
	static Vector3	cross(const Vector3& a, const Vector3& b);

	//! べき乗
	//!	@param	[in]	vec		べき乗元ベクトル
	//! @param	[in]	powVec	べき乗数ベクトル
	static Vector3	powByVec(const Vector3&	vec, const Vector3& powVec);


	//! べき乗
	//!	@param	[in]	vec		べき乗元ベクトル
	//! @param	[in]	powVal	べき乗数
	static Vector3	powByVal(const Vector3&	vec, const float& powVal);

	//@}

	// 基本メンバー
	f32		_x;
	f32		_y;
	f32		_z;
};


//-----------------------------------------------------------------------------
//! ４要素ベクトル構造体
//-----------------------------------------------------------------------------
struct Vector4
{
	
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//!	デフォルトコンストラクタ
	Vector4();

	//! コンストラクタ(xyzw指定)
	Vector4(f32 x, f32 y, f32 z, f32 w);

	//!	コンストラクタ(ベクトル指定)
	Vector4(const Vector3 v, f32 w);

	//! コンストラクタ(カラー)
	Vector4(const Color color);

	//@}
	//-------------------------------------------------------------
	//! @name 変換関数
	//-------------------------------------------------------------
	//@{

	//! ベクトルの変換
	//! v' ← this * m;
	//!	@param	[in]	m	ベクトル変換のための行列
	//!	@return	変換後のベクトル
	Vector4		transform(const Matrix& m);

	//@}
	//-------------------------------------------------------------
	//! @name 演算子オーバーロード
	//-------------------------------------------------------------
	//@{

	//! 配列アクセス (const)
	//!	@param	[in]	index	アクセス番号	0=x  1=y  2=z	3=w
//	f32		operator [] (u32 index) const { return (&_x)[index]; }
	f32&	operator [] (u32 index)  { return (&_x)[index]; }


	//@}
	// 基本メンバー
	f32		_x;
	f32		_y;
	f32		_z;
	f32		_w;
};

//-----------------------------------------------------------------------------
//! 頂点
//-----------------------------------------------------------------------------
struct Vertex {
	Float3		_position;		//!< xyz座標
	Vector4		_color;			//!< カラー
	Float2		_uv;			//!< テクスチャ座標UV
	Float3		_normal;		//!< 法線
};

//-----------------------------------------------------------------------------
//! フィルター用頂点構造体
//-----------------------------------------------------------------------------
struct FilterVertex {
	Float3		_position;		//!< xyz座標
	Vector4		_diffuseColor;	//!< ディフューズカラー
	Vector4		_emissiveColor;	//!< 自己発光カラー
	Float2		_uv;			//!< テクスチャ座標UV
};

//-----------------------------------------------------------------------------
//! 頂点バッファコンテキスト
//-----------------------------------------------------------------------------
struct VertexContext {
	Vector3		_pos;
	Float2		_uv;
	Vector4		_color;
};


//=============================================================================
//	END OF FILE
//=============================================================================