//-----------------------------------------------------------------------------
//!
//!	@file	Color.h
//!	@brief	�J���[�\����
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	@struct �J���[�\����
//-----------------------------------------------------------------------------
struct Color
{
public:
	//! �f�t�H���g�R���X�g���N�^
	Color() {}

	//! �R���X�g���N�^(rgba�l�w��)
	Color(unsigned char r,
		  unsigned char g,
		  unsigned char b,
		  unsigned char a = 255)
	: _r(r), _g(g), _b(b), _a(a) {}

	//-------------------------------------------------------------
	// �ÓI�Ȓ萔
	//-------------------------------------------------------------
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
public:
	unsigned char	_r;
	unsigned char	_g;
	unsigned char	_b;
	unsigned char	_a;
};

//===========================================================================
//	END OF FILE
//===========================================================================
