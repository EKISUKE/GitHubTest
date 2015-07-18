//-----------------------------------------------------------------------------
//!
//!	@file	GLFont.cpp
//!	@brief	�����\���p
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
GLFont::GLFont(wchar_t *fontname, s32 size)
{
	Initialize(fontname, size);
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
GLFont::~GLFont(){
	glDeleteLists(_list, 255);
	_list   = 0;
}

//-----------------------------------------------------------------------------
//! ������
//-----------------------------------------------------------------------------
bool GLFont::Initialize(wchar_t* fontname, s32 size)
{
	 Hfont = CreateFontW(
						 size,						// �t�H���g����
						 0,							// ������
						 0,							// �e�L�X�g�̊p�x
						 0,							// �x�[�X���C���Ƃ����Ƃ̊p�x
						 FW_REGULAR,				// �t�H���g�̑���
						 FALSE,						// �C�^���b�N��
						 FALSE,						// �A���_�[���C��
						 FALSE,						// �ł�������
						 SHIFTJIS_CHARSET,			// �����Z�b�g
						 OUT_DEFAULT_PRECIS,		// �o�͐��x
						 CLIP_DEFAULT_PRECIS,		// �N���b�s���O���x
						 ANTIALIASED_QUALITY,		// �o�͕i��
						 FIXED_PITCH | FF_MODERN,	// �s�b�`�ƃt�@�~���[
						 fontname);					// ���̖�

	 Hdc = wglGetCurrentDC();
	 SelectObject(Hdc, Hfont);
	 return true;
}

//-----------------------------------------------------------------------------
//! ���C�h������̕`��
//-----------------------------------------------------------------------------
void  GLFont::DrawStringW(s32 x, s32 y, s32 z, wchar_t* format, ...)
{
	SelectObject(Hdc, Hfont);
	wchar_t buf[256];
	va_list ap;
	int	Length = 0;
	_list = 0;

	// �|�C���^��NULL�̏ꍇ�͏I��
	if( format == NULL )
	{
		return;
	}

	// ������ϊ�
	va_start(ap, format);
	vswprintf_s(buf, format, ap);
	va_end(ap);
	
	Length = wcslen(buf);
	_list   = glGenLists(Length);


	

	for( s32 i=0; i<Length; i++ )
	{
		// �����̃r�b�g�}�b�v���f�B�X�v���C���X�g�Ƃ��č쐬
		wglUseFontBitmapsW(Hdc, buf[i], 1, _list + (DWORD)i );
		/*glNewList(_list + i, GL_COMPILE);
		
		glEndList();*/
	}

	
	glDisable(GL_LIGHTING);		// �����F���o�����߂Ɍ����𖳌���
	// �`��ʒu�̐ݒ�
	glRasterPos3i(x, y, z);
	// �f�B�X�v���C���X�g�ŕ`��
	for( s32 i=0; i<Length; i++ )
	{
		glCallList( _list+i);
	}
	glEnable(GL_LIGHTING);		// ������L����
	// �f�B�X�v���C���X�g�j��
	glDeleteLists(_list, Length);
	_list   = 0;
	//Length  = 0;
}

//=============================================================================
//	END OF FILE
//=============================================================================