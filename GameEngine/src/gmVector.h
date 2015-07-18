//-----------------------------------------------------------------------------
//!
//!	@file	gmVector.h
//!	@brief	�x�N�g�����Z
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once


//---- �v���g�^�C�v�錾
class Degree;
class Radian;
//-----------------------------------------------------------------------------
//! �p�x (degree 0�x�`360�x)
//-----------------------------------------------------------------------------
class Degree
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Degree() {}

	//! �R���X�g���N�^(f32���ړn��)
	explicit Degree(f32 degree);

	//! �R���X�g���N�^(radian��degree�ϊ�)
	Degree(const Radian& radian);

	//! �R�s�[�R���X�g���N�^
	Degree(const Degree& degree) { _degree = degree._degree; }


	//@}
	//-------------------------------------------------------------
	//! @name ���Z�q�I�[�o�[���[�h
	//-------------------------------------------------------------
	//@{

	//! float�փL���X�g
	operator	f32() const { return _degree; }

	//! ��Z
	Degree		operator * (f32 n) const { return Degree(_degree * n); }
	//! ���Z
	Degree&		operator += (Degree n) { _degree += n._degree; return *this; }
	//! ���Z
	Degree&		operator -= (Degree n) { _degree -= n._degree; return *this; }
	//! ��Z	
	Degree&		operator *= (f32 n) { _degree *= n; return *this; }

	//@}
public:
	f32			_degree;
};

//-----------------------------------------------------------------------------
//! �p�x (radian 0�`2��)
//-----------------------------------------------------------------------------
class Radian
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Radian() {}

	//! �R���X�g���N�^(f32���ړn��)
	explicit Radian(f32 radian);

	//! �R���X�g���N�^(degree��radian�ϊ�)
	Radian(const Degree& degree);

	//! �R�s�[�R���X�g���N�^
	Radian(const Radian& radian) { _radian = radian._radian; }

	//@}
	//-------------------------------------------------------------
	//! @name �^�X�N�֐�
	//-------------------------------------------------------------
	//@{

	//! float�փL���X�g
	operator	f32() const { return _radian; }

	//! ���Z
	Radian		operator + (f32 n) const { return Radian(_radian + n); }
	//! ��Z
	Radian		operator * (f32 n) const { return Radian(_radian * n); }
	
	//! ���Z
	const Radian operator += (f32 n)    { _radian += n; return *this; }

	//! ���Z
	const Radian operator += (Radian n) { _radian += n._radian; return *this; }


	//@}
public:
	f32			_radian;

};

//---- �v���g�^�C�v�錾
struct Matrix;
struct Vector4;

//-----------------------------------------------------------------------------
//! �R�v�f�x�N�g���\����
//-----------------------------------------------------------------------------
struct Vector3
{

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Vector3();

	//! �R���X�g���N�^
	Vector3(f32 val);

	//! �R���X�g���N�^
	Vector3(f32 x, f32 y, f32 z);

	//! �R�s�[�R���X�g���N�^
	Vector3(const Vector4& v);

	//@}
	//-------------------------------------------------------------
	//! @name ���Z�q�I�[�o�[���[�h
	//-------------------------------------------------------------
	//@{

	//! �L���X�g
	operator Float3& () const { return *(Float3*)&_x; }

	//! �z��A�N�Z�X (const)
	//!	@param	[in]	index	�A�N�Z�X�ԍ�	0=x  1=y  2=z
	f32		operator [] (u32 index) const { return (&_x)[index]; }

	// ���Z
	//!	@param	[in]	v	����̃x�N�g��
	Vector3	operator + ( const Vector3& v ) const;

	// ���Z
	//!	@param	[in]	v	����̃x�N�g��
	Vector3 operator - ( const Vector3& v ) const;

	// ��Z
	//!	@param	[in]	v	����̃x�N�g��
	Vector3 operator * ( const Vector3& v ) const;

	// �X�J����Z
	//!	@param	[in]	s	�g�嗦
	Vector3 operator * ( float s ) const;


	//! ���Z
	const Vector3& operator += (const Vector3& v);
	
	//! ���Z
	const Vector3& operator -= (const Vector3& v);
	
	//! ��Z
	const Vector3& operator *= (const Vector3& v);
	
	//! �X�J����Z
	const Vector3& operator *= (f32 s);

	//! ���̒l
	Vector3 operator + () const;

	//! ���̒l
	Vector3 operator - () const;

	//---- ��r���Z
	//! ������
	bool	operator == (const Vector3& v);
	//! �������Ȃ�
	bool	operator != (const Vector3& v);


	//@}
	//-------------------------------------------------------------
	//! @name �ϊ��֐�
	//-------------------------------------------------------------
	//@{

	//! �x�N�g���̒���
	f32		length() const;

	//! �x�N�g���̒����̓��(������r�p)
	f32		squareLength() const;

	//! ���K��
	//! @return ���K���ς݃x�N�g��
	Vector3		normalize() const;

	//! �x�N�g���̕ϊ�
	//! v' �� this * m;
	//!	@param	[in]	m	�x�N�g���ϊ��̂��߂̍s��
	//!	@return	�ϊ���̃x�N�g��
	Vector3		transform(const Matrix& m);

	//! ����
	//!	@param	[in]	a	�x�N�g��A
	//!	@param	[in]	b	�x�N�g��B
	//! @return ���ό���
	static f32		dot(const Vector3& a, const Vector3& b);


	//! �O��
	//!	@param	[in]	a	�x�N�g��A
	//!	@param	[in]	b	�x�N�g��B
	//! @return �O�ό���
	static Vector3	cross(const Vector3& a, const Vector3& b);

	//! �ׂ���
	//!	@param	[in]	vec		�ׂ��挳�x�N�g��
	//! @param	[in]	powVec	�ׂ��搔�x�N�g��
	static Vector3	powByVec(const Vector3&	vec, const Vector3& powVec);


	//! �ׂ���
	//!	@param	[in]	vec		�ׂ��挳�x�N�g��
	//! @param	[in]	powVal	�ׂ��搔
	static Vector3	powByVal(const Vector3&	vec, const float& powVal);

	//@}

	// ��{�����o�[
	f32		_x;
	f32		_y;
	f32		_z;
};


//-----------------------------------------------------------------------------
//! �S�v�f�x�N�g���\����
//-----------------------------------------------------------------------------
struct Vector4
{
	
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//!	�f�t�H���g�R���X�g���N�^
	Vector4();

	//! �R���X�g���N�^(xyzw�w��)
	Vector4(f32 x, f32 y, f32 z, f32 w);

	//!	�R���X�g���N�^(�x�N�g���w��)
	Vector4(const Vector3 v, f32 w);

	//! �R���X�g���N�^(�J���[)
	Vector4(const Color color);

	//@}
	//-------------------------------------------------------------
	//! @name �ϊ��֐�
	//-------------------------------------------------------------
	//@{

	//! �x�N�g���̕ϊ�
	//! v' �� this * m;
	//!	@param	[in]	m	�x�N�g���ϊ��̂��߂̍s��
	//!	@return	�ϊ���̃x�N�g��
	Vector4		transform(const Matrix& m);

	//@}
	//-------------------------------------------------------------
	//! @name ���Z�q�I�[�o�[���[�h
	//-------------------------------------------------------------
	//@{

	//! �z��A�N�Z�X (const)
	//!	@param	[in]	index	�A�N�Z�X�ԍ�	0=x  1=y  2=z	3=w
//	f32		operator [] (u32 index) const { return (&_x)[index]; }
	f32&	operator [] (u32 index)  { return (&_x)[index]; }


	//@}
	// ��{�����o�[
	f32		_x;
	f32		_y;
	f32		_z;
	f32		_w;
};

//-----------------------------------------------------------------------------
//! ���_
//-----------------------------------------------------------------------------
struct Vertex {
	Float3		_position;		//!< xyz���W
	Vector4		_color;			//!< �J���[
	Float2		_uv;			//!< �e�N�X�`�����WUV
	Float3		_normal;		//!< �@��
};

//-----------------------------------------------------------------------------
//! �t�B���^�[�p���_�\����
//-----------------------------------------------------------------------------
struct FilterVertex {
	Float3		_position;		//!< xyz���W
	Vector4		_diffuseColor;	//!< �f�B�t���[�Y�J���[
	Vector4		_emissiveColor;	//!< ���Ȕ����J���[
	Float2		_uv;			//!< �e�N�X�`�����WUV
};

//-----------------------------------------------------------------------------
//! ���_�o�b�t�@�R���e�L�X�g
//-----------------------------------------------------------------------------
struct VertexContext {
	Vector3		_pos;
	Float2		_uv;
	Vector4		_color;
};


//=============================================================================
//	END OF FILE
//=============================================================================