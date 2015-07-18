//-----------------------------------------------------------------------------
//!
//!	@file	gmTypedef.h
//!	@brief	�^�錾
//!	@author Yuki Ishigaki
//!
//-----------------------------------------------------------------------------
#pragma once
#pragma message(" + gmTypedef.h")

typedef   signed char		s8;			//!<  8bit�������萮��
typedef	unsigned char		u8;			//!<  8bit�����Ȃ�����
//typedef			 char		c8;			//!<  8bit����

typedef   signed short		s16;		//!< 16bit�������萮��
typedef	unsigned short		u16;		//!< 16bit�����Ȃ�����

typedef   signed int		s32;		//!< 32bit�������萮��
typedef	unsigned int		u32;		//!< 32bit�����Ȃ�����

typedef   signed long long	s64;		//!< 64bit�������萮��
typedef	unsigned long long	u64;		//!< 64bit�����Ȃ�����

typedef float				f32;		//!< �P���x���������_��
typedef double				f64;		//!< �{���x���������_��


/*
#define SAFE_RELEASE( PTR_ )		\
	if( PTR_ != NULL ) {			\
		PTR_->Release();			\
		PTR_ = NULL;				\
	}								\
*/

//---- GM�֘A
typedef char*		GM_STR;
typedef const char*	GM_CSTR;
typedef char		GM_CHAR;


//-----------------------------------------------------------------------------
//!	Float2
//-----------------------------------------------------------------------------
struct Float2
{
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{
	//! �f�t�H���g�R���X�g���N�^
	Float2() {}

	//! �R���X�g���N�^
	Float2(f32 x, f32 y) : _x(x), _y(y) {}

	//@}

	//---- �����o�ϐ�
	f32		_x;
	f32		_y;
};

//-----------------------------------------------------------------------------
//!	Float3
//-----------------------------------------------------------------------------
struct Float3 
{
	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Float3() {}
	//! �R���X�g���N�^
	Float3(f32 x, f32 y, f32 z) : _x(x), _y(y), _z(z) {}

	//@}

	//---- �����o�ϐ�
	f32		_x;
	f32		_y;
	f32		_z;
};

//-----------------------------------------------------------------------------
//!	�J���[�\����
//-----------------------------------------------------------------------------
struct Color
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{

	//! �f�t�H���g�R���X�g���N�^
	Color() {}

	//! �R���X�g���N�^(rgba�l�w��)
	Color(u8 r, u8 g, u8 b, u8 a = 255)
	: _r(r), _g(g), _b(b), _a(a) {}

	//@}

	//-------------------------------------------------------------
	//! @name �ÓI�Ȓ萔
	//-------------------------------------------------------------
	//@{

	static Color		BLACK;		//!< ��
	static Color		BLUE;		//!< ��
	static Color		RED;		//!< ��
	static Color		MAGENTA;	//!< �}�[���^
	static Color		GREEN;		//!< ��
	static Color		CYAN;		//!< �V�A��
	static Color		YELLOW;		//!< ��
	static Color		WHITE;		//!< ��

	static Color		LTBLUE;		//!< ���邢��
	static Color		LTRED;		//!< ���邢��
	static Color		LTMAGENTA;	//!< ���邢�}�[���^
	static Color		LTGREEN;	//!< ���邢��
	static Color		LTCYAN;		//!< ���邢�V�A��
	static Color		LTYELLOW;	//!< ���邢��
	static Color		LTGRAY;		//!< ���邢�O���[

	static Color		DKBLUE;		//!< �Â���
	static Color		DKRED;		//!< �Â���
	static Color		DKMAGENTA;	//!< �Â��}�[���^
	static Color		DKGREEN;	//!< �Â���
	static Color		DKCYAN;		//!< �Â��V�A��
	static Color		DKYELLOW;	//!< �Â���
	static Color		DKGRAY;		//!< �Â��O���[

	static Color		NONE;		//!< ���F

	//@}
public:
	//---- �����o�ϐ�
	u8	_r;
	u8	_g;
	u8	_b;
	u8	_a;
};


//-----------------------------------------------------------------------------
//! �T�C�Y�\����(�e���v���[�g)
//-----------------------------------------------------------------------------
template<typename T>
struct Size
{
	//! �f�t�H���g�R���X�g���N�^
	Size()		   : _w(0), _h(0){}
	//! �R���X�g���N�^
	Size(T w, T h) : _w(w), _h(h){}

	//! ���Z�q�I�[�o�[���[�h
	Size& operator / (Size s)
	{
		return Size(_w / s._w, _h / s._h);
	}

	T		_w;
	T		_h;
};


//-----------------------------------------------------------------------------
//!	���S�Ȋm�ہi�Q�[���G���W���I�u�W�F�N�g�j
//-----------------------------------------------------------------------------
template <typename T>
void SAFE_NEW(T*& p)
{
	if( p== nullptr ) {
		p = new T();
	}
}


//-----------------------------------------------------------------------------
//!	���S�ȉ���i�Q�[���G���W���I�u�W�F�N�g�j
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
//!	���S�ȉ��
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
//! ���b�Z�[�W�t���f�o�b�O�A�T�[�V����
//!	�y�g�p���@�zGM_ASSERT(����, "���b�Z�[�W")
//!	�y��z GM_ASSERT(p != NULL, "�|�C���^��NULL�ł��B")
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

	//! �f�t�H���g�R���X�g���N�^
	Vector4() {}
	//! �R���X�g���N�^
	Vector4(f32 x, f32 y, f32 z, f32 w) : _x(x), _y(y), _z(z), _w(w) {}

};


GM_ALIGN16 struct Matrix
{
	Vector4		_m[4];

	//! �f�t�H���g�R���X�g���N�^
	Matrix() {}

	//! ������
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

	//! �y�����S�̉�]
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


// �\���̓��̃����o�ϐ��̐擪����̃I�t�Z�b�g�l���v�Z
//	�y�g�p���@�zu32 offset = GM_OFFSET_OF(Vector3, _y);
#define GM_OFFSET_OF( STRUCT_, MEMBER_VALUE_ )		(size_t)(&((STRUCT_*)0)->MEMBER_VALUE_)





//=============================================================================
// END OF FILE
//=============================================================================
