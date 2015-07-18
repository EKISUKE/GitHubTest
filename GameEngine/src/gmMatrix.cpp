//-----------------------------------------------------------------------------
//!
//!	@file	Matrix.cpp
//!	@brief	行列演算
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"


//! ゼロ行列
const Matrix Matrix::ZERO
(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f
);

//! 単位行列
const Matrix Matrix::IDENTITY
(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

//-----------------------------------------------------------------------------
//!	デフォルトコンストラクタ
//-----------------------------------------------------------------------------
Matrix::Matrix()
{
}

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
Matrix::Matrix(f32 m11, f32 m12, f32 m13, f32 m14,
			   f32 m21, f32 m22, f32 m23, f32 m24,
			   f32 m31, f32 m32, f32 m33, f32 m34,
			   f32 m41, f32 m42, f32 m43, f32 m44)
{
	_m[0] = Vector4(m11, m12, m13, m14);
	_m[1] = Vector4(m21, m22, m23, m24);
	_m[2] = Vector4(m31, m32, m33, m34);
	_m[3] = Vector4(m41, m42, m43, m44);
}

//-----------------------------------------------------------------------------
//! 乗算合成
//! @param	[in]	m	相手側の行列
//!	@return	合成語の行列
//
//	return ← this * m
//-----------------------------------------------------------------------------
Matrix	Matrix::operator * (const Matrix& m) const
{
	Matrix	result;

	result._m[0]._x = (_m[0]._x * m._m[0]._x) + (_m[0]._y * m._m[1]._x) + (_m[0]._z * m._m[2]._x) + (_m[0]._w * m._m[3]._x);
	result._m[0]._y = (_m[0]._x * m._m[0]._y) + (_m[0]._y * m._m[1]._y) + (_m[0]._z * m._m[2]._y) + (_m[0]._w * m._m[3]._y);
	result._m[0]._z = (_m[0]._x * m._m[0]._z) + (_m[0]._y * m._m[1]._z) + (_m[0]._z * m._m[2]._z) + (_m[0]._w * m._m[3]._z);
	result._m[0]._w = (_m[0]._x * m._m[0]._w) + (_m[0]._y * m._m[1]._w) + (_m[0]._z * m._m[2]._w) + (_m[0]._w * m._m[3]._w);

	result._m[1]._x = (_m[1]._x * m._m[0]._x) + (_m[1]._y * m._m[1]._x) + (_m[1]._z * m._m[2]._x) + (_m[1]._w * m._m[3]._x);
	result._m[1]._y = (_m[1]._x * m._m[0]._y) + (_m[1]._y * m._m[1]._y) + (_m[1]._z * m._m[2]._y) + (_m[1]._w * m._m[3]._y);
	result._m[1]._z = (_m[1]._x * m._m[0]._z) + (_m[1]._y * m._m[1]._z) + (_m[1]._z * m._m[2]._z) + (_m[1]._w * m._m[3]._z);
	result._m[1]._w = (_m[1]._x * m._m[0]._w) + (_m[1]._y * m._m[1]._w) + (_m[1]._z * m._m[2]._w) + (_m[1]._w * m._m[3]._w);

	result._m[2]._x = (_m[2]._x * m._m[0]._x) + (_m[2]._y * m._m[1]._x) + (_m[2]._z * m._m[2]._x) + (_m[2]._w * m._m[3]._x);
	result._m[2]._y = (_m[2]._x * m._m[0]._y) + (_m[2]._y * m._m[1]._y) + (_m[2]._z * m._m[2]._y) + (_m[2]._w * m._m[3]._y);
	result._m[2]._z = (_m[2]._x * m._m[0]._z) + (_m[2]._y * m._m[1]._z) + (_m[2]._z * m._m[2]._z) + (_m[2]._w * m._m[3]._z);
	result._m[2]._w = (_m[2]._x * m._m[0]._w) + (_m[2]._y * m._m[1]._w) + (_m[2]._z * m._m[2]._w) + (_m[2]._w * m._m[3]._w);

	result._m[3]._x = (_m[3]._x * m._m[0]._x) + (_m[3]._y * m._m[1]._x) + (_m[3]._z * m._m[2]._x) + (_m[3]._w * m._m[3]._x);
	result._m[3]._y = (_m[3]._x * m._m[0]._y) + (_m[3]._y * m._m[1]._y) + (_m[3]._z * m._m[2]._y) + (_m[3]._w * m._m[3]._y);
	result._m[3]._z = (_m[3]._x * m._m[0]._z) + (_m[3]._y * m._m[1]._z) + (_m[3]._z * m._m[2]._z) + (_m[3]._w * m._m[3]._z);
	result._m[3]._w = (_m[3]._x * m._m[0]._w) + (_m[3]._y * m._m[1]._w) + (_m[3]._z * m._m[2]._w) + (_m[3]._w * m._m[3]._w);

	return result;
}

//-----------------------------------------------------------------------------
//! 回転角度取得
//-----------------------------------------------------------------------------
Matrix Matrix::getRotate() const
{
	return Matrix(
		_m[0]._x, _m[0]._y, _m[0]._z, 0.0f,
		_m[1]._x, _m[1]._y, _m[1]._z, 0.0f,
		_m[3]._x, _m[3]._y, _m[3]._z, 0.0f,
			0.0f,	  0.0f,		0.0f, 1.0f
	);
}

//-----------------------------------------------------------------------------
//! 逆行列(直行行列用の高速版)
//-----------------------------------------------------------------------------
Matrix Matrix::inverseFast()
{
	Matrix result;

	//---- 回転成分は(3x3)は転置
	result._m[0]._x = _m[0]._x;
	result._m[0]._y = _m[1]._x;
	result._m[0]._z = _m[2]._x;
	result._m[0]._w = 0.0f;

	result._m[1]._x = _m[0]._y;
	result._m[1]._y = _m[1]._y;
	result._m[1]._z = _m[2]._y;
	result._m[1]._w = 0.0f;

	result._m[2]._x = _m[0]._z;
	result._m[2]._y = _m[1]._z;
	result._m[2]._z = _m[2]._z;
	result._m[2]._w = 0.0f;

	// 平行移動成分は転置回転した行列をかける
	Vector3 position = _m[3];
	result._m[3]._x  = Vector3::dot(position, Vector3(_m[0]) ) * -1.0f;
	result._m[3]._y  = Vector3::dot(position, Vector3(_m[1]) ) * -1.0f;
	result._m[3]._z  = Vector3::dot(position, Vector3(_m[2]) ) * -1.0f;
	result._m[3]._w  = 1.0f;


	return result;
}

//-----------------------------------------------------------------------------
//! 転置行列
//-----------------------------------------------------------------------------
Matrix Matrix::transpose()
{
	Matrix result;

	for(s32 i=0; i<4; ++i) {
		for(s32 j=0; j<4; ++j) {
			result._m[i][j] = _m[j][i];
		}
	}
	return result;
}

//-----------------------------------------------------------------------------
//!	平行移動する行列
//! @param	[in]	v	移動方向
//!	@return 平行移動行列
//-----------------------------------------------------------------------------
Matrix	Matrix::translate(const Vector3& v)
{
	return Matrix( 
				  1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  v._x, v._y, v._z, 1.0f
	);
}

//-----------------------------------------------------------------------------
//! スケーリング
//!	@param	[in]	s	XYZスケール値
//!	@return	スケーリング行列
//-----------------------------------------------------------------------------
Matrix Matrix::scale(const Vector3& s)
{
	return Matrix(
		s._x, 0.0f, 0.0f, 0.0f,
		0.0f, s._y, 0.0f, 0.0f,
		0.0f, 0.0f, s._z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

//-----------------------------------------------------------------------------
//!	Ｙ軸中心の回転
//!	@param	[in]	radian	回転角度
//!	@return 回転行列
//-----------------------------------------------------------------------------
Matrix	Matrix::rotateY(const Radian& radian)
{
	Matrix result;

	// sinとcosを求める
	f32	s = sinf(radian);
	f32 c = cosf(radian);

	// 回転行列の生成
	result._m[0] = Vector4(	  c,  0.0f,   -s,  0.0f);
	result._m[1] = Vector4(0.0f,  1.0f, 0.0f,  0.0f);
	result._m[2] = Vector4(	  s,  0.0f,    c,  0.0f);
	result._m[3] = Vector4(0.0f,  0.0f, 0.0f,  1.0f);

	return result;
}

//-----------------------------------------------------------------------------
//! Ｚ軸中心の回転
//!	@param	[in]	radian	回転角度
//!	@return	回転行列
//-----------------------------------------------------------------------------
Matrix Matrix::rotateZ(const Radian& radian)
{
	Matrix	result;

	float	s = sinf(radian);
	float	c = cosf(radian);

	result._m[0] = Vector4(   c,    s, 0.0f, 0.0f);
	result._m[1] = Vector4(  -s,    c, 0.0f, 0.0f);
	result._m[2] = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	result._m[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	
	return result;
}

//-----------------------------------------------------------------------------
//!	任意軸中心の回転
//!	@param	[in]	v		回転中心の軸
//!	@param	[in]	radian	回転角度
//!	@return 回転行列
//-----------------------------------------------------------------------------
Matrix	Matrix::rotateAxis(const Vector3& v, const Radian& radian)
{
	Matrix result;

	Vector3 axis = v.normalize();
	
	// それぞれの軸座標,sinとcosを求める
	f32		x = axis._x;
	f32		y = axis._y;
	f32		z = axis._z;
	f32		s = sinf(radian);
	f32		c = cosf(radian);

	// 回転行列の生成
	result._m[0] = Vector4(x*x*(1.0f-c) + c	  , x*y*(1.0f-c) - z*s  , z*x*(1.0f-c) + y*s , 0.0f);
	result._m[1] = Vector4(x*y*(1.0f-c) + z*s , y*y*(1.0f-c) + c    , y*z*(1.0f-c) - x*s , 0.0f);
	result._m[2] = Vector4(z*x*(1.0f-c) - y*s , y*z*(1.0f-c) + x*s  , z*z*(1.0f-c) + c   , 0.0f);
	result._m[3] = Vector4(				 0.0f ,				   0.0f ,				0.0f , 1.0f);

	return result;

}

//-----------------------------------------------------------------------------
//! 姿勢行列(LookAtMatrix)の作成
//!	@param	[in]	position	カメラの位置
//!	@param	[in]	lookAt		注視点
//!	@param	[in]	worldUp		世界の上方向のベクトル
//!	@return	姿勢行列
//-----------------------------------------------------------------------------
Matrix Matrix::lookAt(
	const Vector3& position,
	const Vector3& lookAt,
	const Vector3& worldUp)
{
	//-------------------------------------------------------------
	// 姿勢行列を生成
	//-------------------------------------------------------------

	//---- Z軸
	Vector3 axisZ = (position - lookAt).normalize();

	//---- X軸
	Vector3 axisX = Vector3::cross(worldUp, axisZ).normalize();

	//---- Y軸
	Vector3 axisY = Vector3::cross(axisZ, axisX).normalize();
	
	Matrix result;
	//---- 行列へセット		 xyz		w
	result._m[0] = Vector4(axisX	 , 0.0f);
	result._m[1] = Vector4(axisY	 , 0.0f);
	result._m[2] = Vector4(axisZ	 , 0.0f);
	result._m[3] = Vector4(position  , 1.0f);

	/*f32 dot1 = -Vector3::dot(axisX, position);
	f32 dot2 = -Vector3::dot(axisY, position);
	f32 dot3 = -Vector3::dot(axisZ, position);
	result._m[3] = Vector4(dot1, dot2, dot3, 1.0f);*/

	return result;
}

//-----------------------------------------------------------------------------
//! ビュー行列の生成
//!	@param	[in]	position	カメラの位置
//!	@param	[in]	lookAt		注視点
//!	@param	[in]	worldUp		世界の上方向のベクトル
//!	@return ビュー行列
//-----------------------------------------------------------------------------
Matrix Matrix::view(
	const Vector3& position,
	const Vector3& lookAt,
	const Vector3& worldUp)
{

	/*D3DXMatrixLookAtRH(
		(D3DXMATRIX *)&result,
		(D3DXVECTOR3*)&position,
		(D3DXVECTOR3*)&lookAt,
		(D3DXVECTOR3*)&worldUp);*/

	Matrix	result;

	//---- Z軸
	Vector3 axisZ = (position - lookAt).normalize();

	//---- X軸
	Vector3 axisX = Vector3::cross(worldUp, axisZ).normalize();

	//---- Y軸
	Vector3 axisY = Vector3::cross(axisZ, axisX).normalize();

	
	//-------------------------------------------------------------
	// ビュー行列を生成
	//-------------------------------------------------------------
	result._m[0]	= Vector4(axisX._x, axisY._x, axisZ._x , 0.0f);
	result._m[1]	= Vector4(axisX._y, axisY._y, axisZ._y , 0.0f);
	result._m[2]	= Vector4(axisX._z, axisY._z, axisZ._z , 0.0f);

	result._m[3]._x = Vector3::dot(position, axisX) * -1.0f;
	result._m[3]._y = Vector3::dot(position, axisY) * -1.0f;
	result._m[3]._z = Vector3::dot(position, axisZ) * -1.0f;
	result._m[3]._w = 1.0f;

	return result;
}

//-----------------------------------------------------------------------------
//! 投影行列の生成
//!	@param	[in]	fov			画角(ラジアン)
//!	@param	[in]	aspect		アスペクト比 (幅÷高さ)
//!	@param	[in]	nearZ		近クリップ面Z
//!	@param	[in]	farZ		遠クリップ面Z
//!	@return 投影行列
//-----------------------------------------------------------------------------
Matrix Matrix::projection(const Degree& fov, f32 aspect, f32 nearZ, f32 farZ)
{
	// cos(θ) = 1/tan(θ)
	f32	f =	1.0f / tanf( Radian(fov) * 0.5f );

	return Matrix
	(
		f/aspect	  , 0.0f,                  0.0f               ,  0.0f,
		    0.0f      ,  f  ,                  0.0f               ,  0.0f,
			0.0f      , 0.0f, (   farZ + nearZ    )/(nearZ - farZ), -1.0f,
			0.0f      , 0.0f, (2.0f * farZ * nearZ)/(nearZ - farZ),  0.0f
	);
}

//-----------------------------------------------------------------------------
//! 正射影行列の作成
//!	@param	[in]	Left		クリッピングの左座標
//!	@param	[in]	Right		クリッピングの右座標
//!	@param	[in]	Bottom		クリッピングの下座標
//!	@param	[in]	Top			クリッピングの上座標
//!	@param	[in]	Near		近クリップ面
//!	@param	[in]	Far			遠クリップ面
//!	@return 正射投影行列
//-----------------------------------------------------------------------------
Matrix Matrix::ortho(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 Near, f32 Far)
{
	f32 RL = Right - Left;
	f32 TB = Top - Bottom;
	f32 FN = Far - Near;
	f32 NF = Near - Far;
	f32 tx = -(Right + Left) / RL;
	f32 ty = -(Top + Bottom) / TB;
	f32 tz = -(Far + Near)   / FN;
	
	return Matrix
	(
	  2.0f / RL,		 0.0f,		 0.0f, 0.0f,
	       0.0f,	2.0f / TB,	     0.0f, 0.0f,
	       0.0f,		 0.0f, -2.0f / FN, 0.0f,
	         tx,		   ty,		   tz, 1.0f
	);
	

}



//=============================================================================
//	END OF FILE
//=============================================================================