//-----------------------------------------------------------------------------
//!
//!	@file	Vector.cpp
//!	@brief	ベクトル演算
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"



//=============================================================================
//	デグリー、ラジアン実装
//=============================================================================

//---------------------------------------------------------------------------
//! コンストラクタ(f32直接渡し)
//---------------------------------------------------------------------------
Degree::Degree(f32 degree)
: _degree(degree)
{
}

//---------------------------------------------------------------------------
//! コンストラクタ(radian→degree変換)
//---------------------------------------------------------------------------
Degree::Degree(const Radian& radian)
{
	_degree = radian * (180.0f / math::PI) ;
}

//---------------------------------------------------------------------------
//! コンストラクタ(f32直接渡し)
//---------------------------------------------------------------------------
Radian::Radian(f32 radian)
: _radian(radian)
{
}

//---------------------------------------------------------------------------
//! コンストラクタ(degree→radian変換)
//---------------------------------------------------------------------------
Radian::Radian(const Degree& degree)
{
	_radian = degree * (math::PI / 180.0f) ;
}


//=============================================================================
//	３要素ベクトル
//=============================================================================

//-----------------------------------------------------------------------------
//! デフォルトコンストラクタ
//-----------------------------------------------------------------------------
Vector3::Vector3()
{

}

//! コンストラクタ
Vector3::Vector3(f32 val)
: _x(val)
, _y(val)
, _z(val)
{
}

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Vector3::Vector3(f32 x, f32 y, f32 z)
: _x(x)
, _y(y)
, _z(z)
{}

//-----------------------------------------------------------------------------
//! コピーコンストラクタ
//-----------------------------------------------------------------------------
Vector3::Vector3(const Vector4& v)
{
	_x = v._x;
	_y = v._y;
	_z = v._z;
}



//-----------------------------------------------------------------------------
// 加算
//!	@param	[in]	v	相手のベクトル
//-----------------------------------------------------------------------------
Vector3	Vector3::operator + ( const Vector3& v ) const
{
	Vector3 result;
	result._x = _x + v._x;
	result._y = _y + v._y;
	result._z = _z + v._z;

	return result;
}

//-----------------------------------------------------------------------------
// 減算
//!	@param	[in]	v	相手のベクトル
//-----------------------------------------------------------------------------
Vector3 Vector3::operator - ( const Vector3& v ) const
{
	Vector3 result;
	result._x = _x - v._x;
	result._y = _y - v._y;
	result._z = _z - v._z;

	return result;
}

//-----------------------------------------------------------------------------
// 乗算
//!	@param	[in]	v	相手のベクトル
//-----------------------------------------------------------------------------
Vector3 Vector3::operator * ( const Vector3& v ) const
{
	Vector3 result;
	result._x = _x * v._x;
	result._y = _y * v._y;
	result._z = _z * v._z;

	return result;
}

//-----------------------------------------------------------------------------
// スカラ乗算
//!	@param	[in]	s	拡大率
//-----------------------------------------------------------------------------
Vector3 Vector3::operator * ( float s ) const
{
	Vector3 result;
	result._x = _x * s;
	result._y = _y * s;
	result._z = _z * s;

	return result;
}


//-----------------------------------------------------------------------------
//! 加算
//-----------------------------------------------------------------------------
const Vector3& Vector3::operator += (const Vector3& v)
{
	_x += v._x;
	_y += v._y;
	_z += v._z;

	return *this;
}

//-----------------------------------------------------------------------------
//! 減算
//-----------------------------------------------------------------------------
const Vector3& Vector3::operator -= (const Vector3& v)
{
	_x -= v._x;
	_y -= v._y;
	_z -= v._z;

	return *this;
}
	
//-----------------------------------------------------------------------------
//! 乗算
//-----------------------------------------------------------------------------
const Vector3& Vector3::operator *= (const Vector3& v)
{
	_x *= v._x;
	_y *= v._y;
	_z *= v._z;

	return *this;
}
	
//-----------------------------------------------------------------------------
//! スカラ乗算
//-----------------------------------------------------------------------------
const Vector3& Vector3::operator *= (f32 s)
{
	_x *= s;
	_y *= s;
	_z *= s;

	return *this;
}

//-----------------------------------------------------------------------------
// 正の値
//-----------------------------------------------------------------------------
Vector3 Vector3::operator + () const
{
	return *this;
}

//-----------------------------------------------------------------------------
// 負の値
//-----------------------------------------------------------------------------
Vector3 Vector3::operator - () const
{
	return *this * -1.0f;
}


//-----------------------------------------------------------------------------
//! 等しい
//-----------------------------------------------------------------------------
bool Vector3::operator == (const Vector3& v)
{
	if( _x != v._x ) return false;
	if( _y != v._y ) return false;
	if( _z != v._z ) return false;
	return true;
}

//-----------------------------------------------------------------------------
//! 等しくない
//-----------------------------------------------------------------------------
bool Vector3::operator != (const Vector3& v)
{
	return !(*this == v);
}


//-----------------------------------------------------------------------------
// ベクトルの長さ
//-----------------------------------------------------------------------------
f32	Vector3::length() const
{
	return sqrtf( (_x * _x) + (_y * _y) + (_z * _z) + 0.00000001f );
}

//-----------------------------------------------------------------------------
//! ベクトルの長さの二乗(長さ比較用)
//-----------------------------------------------------------------------------
f32	Vector3::squareLength() const
{
	return (_x * _x) + (_y * _y) + (_z * _z);
}

//-----------------------------------------------------------------------------
//! 正規化
//! @return 正規化済みベクトル
//-----------------------------------------------------------------------------
Vector3	Vector3::normalize() const
{
	float	invL = 1.0f / length();		// ベクトルの長さ(長さの逆数)

	// 自分自身のベクトルを長さで割る(逆数の乗算)
	return (*this) * invL;
}

//-----------------------------------------------------------------------------
//! ベクトルの変換
//! v' ← this * m;
//!	@param	[in]	m	ベクトル変換のための行列
//!	@return	変換後のベクトル
//-----------------------------------------------------------------------------
Vector3	Vector3::transform(const Matrix& m)
{
	Vector3 result;

	result._x = (_x * m._m[0]._x) + (_y * m._m[1]._x) + (_z * m._m[2]._x) + ( /* 1.0f* */  m._m[3]._x );
	result._y = (_x * m._m[0]._y) + (_y * m._m[1]._y) + (_z * m._m[2]._y) + ( /* 1.0f* */  m._m[3]._y );
	result._z = (_x * m._m[0]._z) + (_y * m._m[1]._z) + (_z * m._m[2]._z) + ( /* 1.0f* */  m._m[3]._z );
	return result;
}

//-----------------------------------------------------------------------------
//! 内積
//!	@param	[in]	a	ベクトルA
//!	@param	[in]	b	ベクトルB
//! @return 内積結果
//-----------------------------------------------------------------------------
f32	Vector3::dot(const Vector3&	a, const Vector3& b)
{
	return ( a._x * b._x ) + ( a._y * b._y ) + ( a._z * b._z );
}


//-----------------------------------------------------------------------------
//! 外積
//!	@param	[in]	a	ベクトルA
//!	@param	[in]	b	ベクトルB
//! @return 外積結果
//-----------------------------------------------------------------------------
Vector3	Vector3::cross(const Vector3& a, const Vector3& b)
{
	Vector3 result;

	result._x = ( a._y * b._z )  - ( b._y * a._z );
	result._y = ( a._z * b._x )  - ( b._z * a._x );
	result._z = ( a._x * b._y )  - ( b._x * a._y );

	return result;
}


//-----------------------------------------------------------------------------
//! べき乗(ベクトルがべき乗数)
//!	@param	[in]	vec		べき乗元ベクトル
//! @param	[in]	powVec	べき乗数ベクトル
//-----------------------------------------------------------------------------
Vector3	Vector3::powByVec(const Vector3& vec, const Vector3& powVec)
{
	Vector3 result;
	result._x = pow(vec._x, powVec._x);
	result._y = pow(vec._y, powVec._y);
	result._z = pow(vec._z, powVec._z);
	return result;
}


//-----------------------------------------------------------------------------
//! べき乗(変数ひとつがべき乗数)
//!	@param	[in]	vec		べき乗元ベクトル
//! @param	[in]	powVal	べき乗数
//-----------------------------------------------------------------------------
Vector3	Vector3::powByVal(const Vector3& vec, const float& powVal)
{
	return powByVec(vec, Vector3(powVal));
}


//=============================================================================
//	４要素ベクトル
//=============================================================================

//-----------------------------------------------------------------------------
//!	デフォルトコンストラクタ
//-----------------------------------------------------------------------------
Vector4::Vector4() {}

//-----------------------------------------------------------------------------
//! コンストラクタ(xyzw指定)
//-----------------------------------------------------------------------------
Vector4::Vector4(f32 x, f32 y, f32 z, f32 w)
: _x(x)
, _y(y)
, _z(z)
, _w(w)
{}

//-----------------------------------------------------------------------------
//!	コンストラクタ(ベクトル指定)
//-----------------------------------------------------------------------------
Vector4::Vector4(const Vector3 v, f32 w)
: _x(v._x)
, _y(v._y)
, _z(v._z)
, _w(w)
{}

//-----------------------------------------------------------------------------
//! コンストラクタ(カラー)
//-----------------------------------------------------------------------------
Vector4::Vector4(const Color color)
{
	// 0.0f~1.0fにコンバート
	_x = LinearInterpolation(0.0f, 1.0f, color._r, 255);
	_y = LinearInterpolation(0.0f, 1.0f, color._g, 255);
	_z = LinearInterpolation(0.0f, 1.0f, color._b, 255);
	_w = LinearInterpolation(0.0f, 1.0f, color._a, 255);
}

//-----------------------------------------------------------------------------
//! ベクトルの変換
//! v' ← this * m;
//!	@param	[in]	m	ベクトル変換のための行列
	//!	@return	変換後のベクトル
//-----------------------------------------------------------------------------
Vector4	Vector4::transform(const Matrix& m)
{
	Vector4 result;

	result._x = (_x * m._m[0]._x) + (_y * m._m[1]._x) + (_z * m._m[2]._x) + (_w * m._m[3]._x );
	result._y = (_x * m._m[0]._y) + (_y * m._m[1]._y) + (_z * m._m[2]._y) + (_w * m._m[3]._y );
	result._z = (_x * m._m[0]._z) + (_y * m._m[1]._z) + (_z * m._m[2]._z) + (_w * m._m[3]._z );
	result._w = (_x * m._m[0]._w) + (_y * m._m[1]._w) + (_z * m._m[2]._w) + (_w * m._m[3]._w );

	return result;
}
//=============================================================================
//	END OF FILE
//=============================================================================