//===========================================================================
//!
//!	@file	gmQuaternion.h
//!	@brief	クォータニオン(四元数)
//!	@note	[参考URL] http://marupeke296.com/DXG_No10_Quaternion.html
//!
//!	クォータニオンを活用することでXYZ軸中心の回転だけでなく、任意軸中心の回転が可能になる。
//!
//!	【特徴】
//!	◎回転を最短距離で補間する	球面線形補間( SLERP : Spherical Linear intERPolation )
//!	◎乗算の計算量が行列と比較して軽い。
//!
//!	クォータニオン ←→ 行列 の相互変換が可能。
//!	クォータニオンは回転情報のみを持つため、平行移動量やスケール値は別途持つ必要がある。
//!
//===========================================================================
#pragma once

//===========================================================================
//! クォータニオン (四元数)
//===========================================================================
class Quaternion
{
public:
	//-------------------------------------------------------------
	//!	@name 初期化
	//-------------------------------------------------------------
	//@{

	//!	コンストラクタ
	Quaternion(void) {}
	//!	コンストラクタ
	Quaternion(float x, float y, float z, float w) : _x(x), _y(y), _z(z), _w(w) {}
	//!	コンストラクタ
	Quaternion(const Vector3& i, float r): _x(i._x), _y(i._y), _z(i._z), _w(r) {}
	//! コンストラクタ（３つのオイラー角指定）
	Quaternion(const Vector3& angle)
	{
		float	cosX_2 = cosf(angle._x * 0.5f);
		float	cosY_2 = cosf(angle._y * 0.5f);
		float	cosZ_2 = cosf(angle._z * 0.5f);

		float	sinX_2 = sinf(angle._x * 0.5f);
		float	sinY_2 = sinf(angle._y * 0.5f);
		float	sinZ_2 = sinf(angle._z * 0.5f);

		_x = sinX_2 * cosY_2 * cosZ_2 - cosX_2 * sinY_2 * sinZ_2;
		_y = cosX_2 * sinY_2 * cosZ_2 + sinX_2 * cosY_2 * sinZ_2;
		_z = cosX_2 * cosY_2 * sinZ_2 - sinX_2 * sinY_2 * cosZ_2;
		_w = cosX_2 * cosY_2 * cosZ_2 + sinX_2 * sinY_2 * sinZ_2;
	} 
	
	//@}
	//-------------------------------------------------------------
	//!	@name 代入
	//-------------------------------------------------------------
	//@{

	//! 代入
	Quaternion&			operator = (const Quaternion& q)
	{
		_x = q._x;
		_y = q._y;
		_z = q._z;
		_w = q._w;
		return *this;
	}

	//@}
	//-------------------------------------------------------------
	//!	@name 演算
	//-------------------------------------------------------------
	//@{

	//! 加算
	const Quaternion	operator + (const Quaternion& q) const	
	{
		return Quaternion(_x + q._x, _y + q._y, _z + q._z, _w + q._w);
	}

	//! 減算
	const Quaternion	operator - (const Quaternion& q) const	
	{
		return Quaternion(_x - q._x, _y - q._y, _z - q._z, _w - q._w);
	}

	//! 乗算
	const Quaternion	operator * (const Quaternion& q) const	
	{
		return Quaternion(
			_y * q._z - _z * q._y + _w * q._x + _x * q._w,
			_z * q._x - _x * q._z + _w * q._y + _y * q._w,
			_x * q._y - _y * q._x + _w * q._z + _z * q._w,
			_w * q._w - Vector3::dot( Vector3(_x, _y, _z), Vector3(q._x, q._y, q._z))
		);
	}

	// 除算
	const Quaternion	operator / (const Quaternion& q) const	
	{
		return *this * q.inverse();
	}

	const Quaternion	operator * (float scale) const
	{
		return Quaternion(_x * scale, _y * scale, _z * scale, _w * scale);
	}

	const Quaternion	operator / (float scale) const
	{
		float	invScale = 1.0f / scale;
		return Quaternion(_x * invScale, _y * invScale, _z * invScale, _w * invScale);
	}

	const Quaternion	operator -	(void) const
	{
		return Quaternion(-_x, -_y, -_z, -_w);
	}
	
	const Quaternion&	operator += (const Quaternion& q)
	{
		_x += q._x;
		_y += q._y;
		_z += q._z;
		_w += q._w;
		return *this;
	}

	const Quaternion&	operator -= (const Quaternion &q)		
	{
		_x -= q._x;
		_y -= q._y;
		_z -= q._z;
		_w -= q._w;
		return *this;
	}

	const Quaternion&	operator *= (const Quaternion &q)		
	{	
		*this = *this * q;
		return *this;
	}
	
	const Quaternion&	operator *= (float scale)			
	{
		_x *= scale;
		_y *= scale;
		_z *= scale;
		_w *= scale;
		return *this;
	}

	const Quaternion&	operator /= (float scale)			
	{
		float	invScale = scale;
		_x *= invScale;
		_y *= invScale;
		_z *= invScale;
		_w *= invScale;
		return *this;
	}

	//! 長さを取得
	float			length(void) const
	{
		return sqrtf( lengthSq() );
	}

	//! 長さの二乗を取得
	float			lengthSq(void) const
	{
		return _x * _x + _y * _y + _z * _z + _w * _w;
	}

	//! 正規化
	Quaternion	normalize(void) const
	{
		return *this / length();
	}

	//! 共役
	Quaternion	conjugate(void) const
	{
		return Quaternion(-_x, -_y, -_z, _w);
	}

	//! 逆クォータニオン
	Quaternion	inverse(void) const
	{
		return conjugate() / lengthSq();
	}
/*	
	//! returns the logarithm of a quaternion = v*a where q = [cos(a),v*sin(a)]
	quaternion log() const
	{
		float a = (float)acos(s);
		float sina = (float)sin(a);
		quaternion ret;

		ret.s = 0;
		if (sina > 0)
		{
			ret.v.x = a*v.x/sina;
			ret.v.y = a*v.y/sina;
			ret.v.z = a*v.z/sina;
		} else {
			ret.v.x= ret.v.y= ret.v.z= 0;
		}
		return ret;
	}

	//! returns e^quaternion = exp(v*a) = [cos(a),vsin(a)]
	quaternion exp() const
	{
		float a = (float)v.length();
		float sina = (float)sin(a);
		float cosa = (float)cos(a);
		quaternion ret;

		ret.s = cosa;
		if (a > 0)
		{
			ret.v.x = sina * v.x / a;
			ret.v.y = sina * v.y / a;
			ret.v.z = sina * v.z / a;
		} else {
			ret.v.x = ret.v.y = ret.v.z = 0;
		}
		return ret;
	}

*/

	//!	クォータニオン同士の内積
	static inline float	dot(const Quaternion& q1, const Quaternion& q2) 
	{
		return q1._x * q2._x + q1._y * q2._y + q1._z * q2._z + q1._w * q2._w;
	}




	//@}
	//-------------------------------------------------------------
	//!	@name 変換
	//-------------------------------------------------------------
	//@{
/*
	//! casting to a 4x4 isomorphic matrix for right multiplication with vector
	operator matrix4() const
	{			
		return matrix4(s,  -v.x, -v.y,-v.z,
				v.x,  s,  -v.z, v.y,
				v.y, v.z,    s,-v.x,
				v.z,-v.y,  v.x,   s);
	}
	*/
	//! 変換( クォータニオン → 回転行列 )
	Matrix	toRotateMatrix(void) const
	{
		GM_ASSERT(0.9999f < length() && length() < 1.0001f, "Quarternion is not normalized");		
		return Matrix(
			1.0f - 2.0f * (_y * _y + _z * _z),        2.0f * (_x * _y - _w * _z),        2.0f * (_x * _z + _w * _y), 0.0f,
			       2.0f * (_x * _y + _w * _z), 1.0f - 2.0f * (_x * _x + _z * _z),        2.0f * (_y * _z - _w * _x), 0.0f,   
			       2.0f * (_x * _z - _w * _y),        2.0f * (_y * _z + _w * _x), 1.0f - 2.0f * (_x * _x + _y * _y), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	//! 変換( 回転行列 → クォータニオン )
	//! @note 参考URL http://marupeke296.com/DXG_No58_RotQuaternionTrans.html
	bool	fromRotateMatrix(const Matrix& m)
	{
		// 要素の最大成分を求める
		float	element[4];	// 0:X, 1:Y, 2:Z, 3:W
		element[0] =  m._m[0]._x - m._m[1]._y - m._m[2]._z + 1.0f;
		element[1] = -m._m[0]._x + m._m[1]._y - m._m[2]._z + 1.0f;
		element[2] = -m._m[0]._x - m._m[1]._y + m._m[2]._z + 1.0f;
		element[3] =  m._m[0]._x + m._m[1]._y + m._m[2]._z + 1.0f;

		unsigned int	index = 0;
		for( signed int i=1; i<4; ++i ) {
			if( element[i] <= element[index] ) continue;
			index = i;
		}

		if( element[index] < 0.0f ) {
			return false;				// 変換失敗
		}

		// 最大要素の値を求める
		float		v     = sqrtf( element[index] ) * 0.5f;
		float		scale = 0.25f / v;

		switch ( index ) {
		case 0: //---- X
			_x = v;
			_y = (m._m[0]._y + m._m[1]._x) * scale;
			_z = (m._m[2]._x + m._m[0]._z) * scale;
			_w = (m._m[1]._z - m._m[2]._y) * scale;
			break;
		case 1: //---- Y
			_x = (m._m[0]._y + m._m[1]._x) * scale;
			_y = v;
			_z = (m._m[1]._z + m._m[2]._y) * scale;
			_w = (m._m[2]._x - m._m[0]._z) * scale;
			break;
		case 2: //---- Z
			_x = (m._m[2]._x + m._m[0]._z) * scale;
			_y = (m._m[1]._z + m._m[2]._y) * scale;
			_z = v;
			_w = (m._m[0]._y - m._m[1]._x) * scale;
			break;
		case 3: //---- W
			_x = (m._m[1]._z - m._m[2]._y) * scale;
			_y = (m._m[2]._x - m._m[0]._z) * scale;
			_z = (m._m[0]._y - m._m[1]._x) * scale;
			_w = v;
			break;
		}

		*this = conjugate();

		return true;
	}
/*

	//! converts from a normalized axis - angle pair rotation to a quaternion
	static inline quaternion from_axis_angle(const vector3f &axis, float angle)
	{ return quaternion(cosf(angle/2), axis*sinf(angle/2)); }

	//! returns the axis and angle of this unit quaternion
	void to_axis_angle(vector3f &axis, float &angle) const
	{
		angle = acosf(s);

		// pre-compute to save time
		float sinf_theta_inv = 1.0/sinf(angle);

		// now the vector
		axis.x = v.x*sinf_theta_inv;
		axis.y = v.y*sinf_theta_inv;
		axis.z = v.z*sinf_theta_inv;

		// multiply by 2
		angle*=2;
	}
*/
	//! 変換( クォータニオン→オイラー角 )
	Vector3	toEulerAngles(bool homogenous=true) const
	{
		float		sqX = _x * _x;
		float		sqY = _y * _y;
		float		sqZ = _z * _z;
		float		sqW = _w * _w;

		Vector3	euler;
		if( homogenous ) {
			euler._x = atan2f( 2.0f * (_x * _y + _z * _w),  sqX - sqY - sqZ + sqW);    		
			euler._y = asinf (-2.0f * (_x * _z - _y * _w));
			euler._z = atan2f( 2.0f * (_y * _z + _x * _w), -sqX - sqY + sqZ + sqW);    
		} else {
			euler._x = atan2f( 2.0f * (_z * _y + _x * _w), 1.0f - 2.0f * (sqX + sqY));
			euler._y = asinf (-2.0f * (_x * _z - _y * _w));
			euler._z = atan2f( 2.0f * (_x * _y + _z * _w), 1.0f - 2.0f * (sqY + sqZ));
		}
		return euler;
	}

	//! クォータニオンでベクトルを回転変換
	//!	@param	[in]	v	変換するベクトル
	//!	@return	回転後のベクトル
	Vector3	rotate(const Vector3& v) const
	{   
		Quaternion	r(*this * Quaternion(v, 0.0f) * conjugate());
		return Vector3(r._x, r._y, r._z);
	}

	//@}
	//-------------------------------------------------------------
	//!	@name 補間
	//-------------------------------------------------------------
	//@{

	//! 線形補間
	static Quaternion	lerp(const Quaternion& q1, const Quaternion& q2, float t) 
	{
		return (q1 * (1.0f - t) + q2 * t).normalize();
	}

	//!	球面線形補間( SLERP : Spherical Linear intERPolation )
	//!	q1→q2への回転を最短距離で補間します
	//!	@param	[in]	q1	クォータニオン1
	//!	@param	[in]	q2	クォータニオン2
	//!	@param	[in]	t	補間パラメータt (0.0～1.0)
	static Quaternion	slerp(const Quaternion& q1, const Quaternion& q2, float t) 
	{
		Quaternion q3;
		float		dot = Quaternion::dot(q1, q2);

		if( dot < 0.0f ) {
			dot = -dot;
			q3  = -q2;
		}
		else {
			q3 = q2;
		}
		
		if( dot < 0.95f ) {
			float	angle = acosf(dot);
			return (q1 * sinf( angle * (1.0f - t) ) + q3 * sinf( angle * t )) / sinf(angle);
		} else {
			// もし角度が小さかったら線形補間で対応する
			return lerp(q1, q3, t);
		}
	}
/*
	//! This version of slerp, used by squad, does not check for theta > 90.
	static quaternion slerpNoInvert(const quaternion &q1, const quaternion &q2, float t) 
	{
		float dot = quaternion::dot(q1, q2);

		if (dot > -0.95f && dot < 0.95f)
		{
			float angle = acosf(dot);			
			return (q1*sinf(angle*(1-t)) + q2*sinf(angle*t))/sinf(angle);
		} else  // if the angle is small, use linear interpolation								
			return lerp(q1,q2,t);			
	}

	//! spherical cubic interpolation
	static quaternion squad(const quaternion &q1,const quaternion &q2,const quaternion &a,const quaternion &b,float t)
	{
		quaternion c= slerpNoInvert(q1,q2,t),
			       d= slerpNoInvert(a,b,t);		
		return slerpNoInvert(c,d,2*t*(1-t));
	}

	//! Shoemake-Bezier interpolation using De Castlejau algorithm
	static quaternion bezier(const quaternion &q1,const quaternion &q2,const quaternion &a,const quaternion &b,float t)
	{
		// level 1
		quaternion q11= slerpNoInvert(q1,a,t),
				q12= slerpNoInvert(a,b,t),
				q13= slerpNoInvert(b,q2,t);		
		// level 2 and 3
		return slerpNoInvert(slerpNoInvert(q11,q12,t), slerpNoInvert(q12,q13,t), t);
	}

	//! Given 3 quaternions, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
	static quaternion spline(const quaternion &qnm1,const quaternion &qn,const quaternion &qnp1)
	{
		quaternion qni(qn.s, -qn.v);	
		return qn * (( (qni*qnm1).log()+(qni*qnp1).log() )/-4).exp();
	}
*/


	//@}


	static const Quaternion		IDENTITY;		//!< 単位クォータニオン(0,0,0,1)

public:



	float			_x;
	float			_y;
	float			_z;
	float			_w;
	



};

//===========================================================================
//	END OF FILE
//===========================================================================
