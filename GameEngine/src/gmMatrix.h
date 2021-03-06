//-----------------------------------------------------------------------------
//!
//!	@file	gmMatrix.h
//!	@brief	sńZ
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	SSsń
//-----------------------------------------------------------------------------
struct Matrix
{
public:
	//-------------------------------------------------------------
	//! @name úť
	//-------------------------------------------------------------
	//@{

	//! ftHgRXgN^
	Matrix();

	//! RXgN^
	Matrix(f32 m11, f32 m12, f32 m13, f32 m14,
		   f32 m21, f32 m22, f32 m23, f32 m24,
		   f32 m31, f32 m32, f32 m33, f32 m34,
		   f32 m41, f32 m42, f32 m43, f32 m44);

	//@}
	//-------------------------------------------------------------
	//! @name Z
	//-------------------------------------------------------------
	//@{

	//! ćZŹ
	//! @param	[in]	m	č¤Ěsń
	//!	@return	ŹęĚsń
	//
	//	return Š this * m
	Matrix		operator * (const Matrix& m) const;

	//@}
	//-------------------------------------------------------------
	//! @name sńćž
	//-------------------------------------------------------------
	//@{

	//! ń]pxćž
	Matrix		getRotate() const;

	//@}
	//-------------------------------------------------------------
	//! @name sńśŹ
	//-------------------------------------------------------------
	//@{

	//! tsń(źssńpĚŹĹ)
	Matrix		inverseFast();

	//! ]usń
	Matrix		transpose();

	//!	˝sÚŽˇésń
	//! @param	[in]	v	ÚŽűü
	//!	@return ˝sÚŽsń
	static Matrix	translate(const Vector3& v);


	//! XP[O
	//!	@param	[in]	s	XYZXP[l
	//!	@return	XP[Osń
	static Matrix	scale(const Vector3& s);
	

	//!	x˛SĚń]
	//!	@param	[in]	radian	ń]px
	//!	@return ń]sń
	static Matrix	rotateY(const Radian& radian);
	
	//! y˛SĚń]
	//!	@param	[in]	radian	ń]px
	//!	@return	ń]sń
	static Matrix	rotateZ(const Radian& radian);


	//!	CÓ˛SĚń]
	//!	@param	[in]	v		ń]SĚ˛
	//!	@param	[in]	radian	ń]px
	//!	@return ń]sń
	static Matrix	rotateAxis(const Vector3& v, const Radian& radian);



	//! p¨sń(LookAtMatrix)ĚěŹ
	//!	@param	[in]	position	JĚĘu
	//!	@param	[in]	lookAt		_
	//!	@param	[in]	worldUp		˘EĚăűüĚxNg
	//!	@return	p¨sń
	static Matrix lookAt(const Vector3& position,
						const Vector3& lookAt,
						const Vector3& worldUp = Vector3(0.0f, 1.0f, 0.0f));

	//! r[sńĚśŹ
	//!	@param	[in]	position	JĚĘu
	//!	@param	[in]	lookAt		_
	//!	@param	[in]	worldUp		˘EĚăűüĚxNg
	//!	@return r[sń
	static Matrix	view(
						const Vector3& position,
						const Vector3& lookAt,
						const Vector3& worldUp = Vector3(0.0f, 1.0f, 0.0f));

	//! esńĚśŹ
	//!	@param	[in]	fov			ćp(WA)
	//!	@param	[in]	aspect		AXyNgä (ł)
	//!	@param	[in]	nearZ		ßNbvĘZ
	//!	@param	[in]	farZ		NbvĘZ
	//!	@return esń
	static Matrix	projection(const Degree& fov, f32 aspect, f32 nearZ, f32 farZ);

	//! łËesńĚěŹ
	//!	@param	[in]	Left		NbsOĚśŔW
	//!	@param	[in]	Right		NbsOĚEŔW
	//!	@param	[in]	Bottom		NbsOĚşŔW
	//!	@param	[in]	Top			NbsOĚăŔW
	//!	@param	[in]	Near		ßNbvĘ
	//!	@param	[in]	Far			NbvĘ
	//!	@return łËesń
	static Matrix	ortho(f32 Left, f32 Right, f32 Bottom, f32 Top, f32 Near, f32 Far);

	//@}

	static const Matrix	ZERO;		//!< [sń
	static const Matrix	IDENTITY;	//!< PĘsń
public:
	//---- Svf ~ Ss
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
