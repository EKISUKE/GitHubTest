//-----------------------------------------------------------------------------
//!
//!	@file	gmMatrix.h
//!	@brief	行列演算
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	４ｘ４行列
//-----------------------------------------------------------------------------
struct Matrix
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! デフォルトコンストラクタ
	Matrix();

	//! コンストラクタ
	Matrix(f32 m11, f32 m12, f32 m13, f32 m14,
		   f32 m21, f32 m22, f32 m23, f32 m24,
		   f32 m31, f32 m32, f32 m33, f32 m34,
		   f32 m41, f32 m42, f32 m43, f32 m44);

	//@}
	//-------------------------------------------------------------
	//! @name 演算
	//-------------------------------------------------------------
	//@{

	//! 乗算合成
	//! @param	[in]	m	相手側の行列
	//!	@return	合成語の行列
	//
	//	return ← this * m
	Matrix		operator * (const Matrix& m) const;

	//@}
	//-------------------------------------------------------------
	//! @name 行列取得
	//-------------------------------------------------------------
	//@{

	//! 回転角度取得
	Matrix		getRotate() const;

	//@}
	//-------------------------------------------------------------
	//! @name 行列生成
	//-------------------------------------------------------------
	//@{

	//! 逆行列(直行行列用の高速版)
	Matrix		inverseFast();

	//! 転置行列
	Matrix		transpose();

	//!	平行移動する行列
	//! @param	[in]	v	移動方向
	//!	@return 平行移動行列
	static Matrix	translate(const Vector3& v);


	//! スケーリング
	//!	@param	[in]	s	XYZスケール値
	//!	@return	スケーリング行列
	static Matrix	scale(const Vector3& s);
	

	//!	Ｙ軸中心の回転
	//!	@param	[in]	radian	回転角度
	//!	@return 回転行列
	static Matrix	rotateY(const Radian& radian);
	
	//! Ｚ軸中心の回転
	//!	@param	[in]	radian	回転角度
	//!	@return	回転行列
	static Matrix	rotateZ(const Radian& radian);


	//!	任意軸中心の回転
	//!	@param	[in]	v		回転中心の軸
	//!	@param	[in]	radian	回転角度
	//!	@return 回転行列
	static Matrix	rotateAxis(const Vector3& v, const Radian& radian);



	//! 姿勢行列(LookAtMatrix)の作成
	//!	@param	[in]	position	カメラの位置
	//!	@param	[in]	lookAt		注視点
	//!	@param	[in]	worldUp		世界の上方向のベクトル
	//!	@return	姿勢行列
	static Matrix lookAt(const Vector3& position,
						const Vector3& lookAt,
						const Vector3& worldUp = Vector3(0.0f, 1.0f, 0.0f));

	//! ビュー行列の生成
	//!	@param	[in]	position	カメラの位置
	//!	@param	[in]	lookAt		注視点
	//!	@param	[in]	worldUp		世界の上方向のベクトル
	//!	@return ビュー行列
	static Matrix	view(
						const Vector3& position,
						const Vector3& lookAt,
						const Vector3& worldUp = Vector3(0.0f, 1.0f, 0.0f));

	//! 投影行列の生成
	//!	@param	[in]	fov			画角(ラジアン)
	//!	@param	[in]	aspect		アスペクト比 (幅÷高さ)
	//!	@param	[in]	nearZ		近クリップ面Z
	//!	@param	[in]	farZ		遠クリップ面Z
	//!	@return 投影行列
	static Matrix	projection(const Degree& fov, f32 aspect, f32 nearZ, f32 farZ);

	//! 正射影行列の作成
	//!	@param	[in]	Left		クリッピングの左座標
	//!	@param	[in]	Right		クリッピングの右座標
	//!	@param	[in]	Bottom		クリッピングの下座標
	//!	@param	[in]	Top			クリッピングの上座標
	//!	@param	[in]	Near		近クリップ面
	//!	@param	[in]	Far			遠クリップ面
	//!	@return 正射投影行列
	static Matrix	ortho(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 Near, f32 Far);

	//@}

	static const Matrix	ZERO;		//!< ゼロ行列
	static const Matrix	IDENTITY;	//!< 単位行列
public:
	//---- ４要素 × ４行
	//			+====+====+====+====+
	//	Vector4	| _x | _y | _z | _w |	 _m[0]  
	//			+====+====+====+====+
	//	Vector4	| _x | _y | _z | _w |	 _m[1]  
	//			+====+====+====+====+
	//	Vector4	| _x | _y | _z | _w |	 _m[2]  
	//			+====+====+====+====+
	//	Vector4	| _x | _y | _z | _w |	 _m[3]  
	//			+====+====+====+====+
	Vector4		_m[4];

};

//=============================================================================
//	END OF FILE
//=============================================================================
