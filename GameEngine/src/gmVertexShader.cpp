//-----------------------------------------------------------------------------
//!
//!	@file	gmVertexShader.cpp
//!	@brief	���_�V�F�[�_
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
VertexShader::VertexShader()
{
}
	
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
VertexShader::~VertexShader()
{
	//! �V�F�[�_�I�u�W�F�N�g�̍폜
	glDeleteShader(myShader);
}



//-----------------------------------------------------------------------------
//! �V�F�[�_�v���O�����̏�����
//!	@param	[in]	filePath
//-----------------------------------------------------------------------------
bool VertexShader::LoadAndCompile(GM_CSTR filePath)
{
	//! �V�F�[�_�v���O�����̃R���p�C��/�����N���ʂ𓾂�ϐ�
	GLint compiled;
	
	//! �V�F�[�_�I�u�W�F�N�g�̍쐬
	myShader = glCreateShader(GL_VERTEX_SHADER);

	//! �V�F�[�_�̃\�[�X�v���O�����̓ǂݍ���
	if( !readShaderSource(filePath) ) return false;

	//! �o�[�e�b�N�X�V�F�[�_�̃\�[�X�v���O�����̃R���p�C��
	glCompileShader(myShader);
	glGetShaderiv(myShader, GL_COMPILE_STATUS, &compiled);

	printShaderInfoLog(myShader);
	if (compiled == GL_FALSE) {
		MessageBox(NULL, "Compile error in vertex shader.", NULL, MB_OK);
		return false;
	}

	return true;
}

//=============================================================================
// END OF FILE
//=============================================================================