//-----------------------------------------------------------------------------
//!
//!	@file	gmFragmentShader.cpp
//!	@brief	�t���O�����g�V�F�[�_
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
FragmentShader::FragmentShader()
{
}
	
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
FragmentShader::~FragmentShader()
{
	//! �V�F�[�_�I�u�W�F�N�g�̍폜
	glDeleteShader(myShader);
}


//-----------------------------------------------------------------------------
//! �V�F�[�_�v���O�����̏�����
//!	@param	[in]	filePath
//-----------------------------------------------------------------------------
bool FragmentShader::LoadAndCompile(GM_CSTR filePath)
{
	//! �V�F�[�_�v���O�����̃R���p�C��/�����N���ʂ𓾂�ϐ�
	GLint compiled;
	
	//! �V�F�[�_�I�u�W�F�N�g�̍쐬
	myShader = glCreateShader(GL_FRAGMENT_SHADER);
	if( !readShaderSource(filePath) ) return false;

	//! �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�����̃R���p�C��
	glCompileShader(myShader);
	glGetShaderiv(myShader, GL_COMPILE_STATUS, &compiled);


	printShaderInfoLog(myShader);
	if (compiled == GL_FALSE) {
		
		MessageBox(NULL, "Compile error in fragment shader.", NULL, MB_OK);
		return false;
	}
	return true;
}

//=============================================================================
// END OF FILE
//=============================================================================