//-----------------------------------------------------------------------------
//!
//!	@file	gmProgramShader.cpp
//!	@brief	�V�F�[�_�v���O����
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
ProgramShader::ProgramShader()
{
}
	
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
ProgramShader::~ProgramShader()
{
}

//-----------------------------------------------------------------------------
// �ÓI�ϐ��̐錾
//-----------------------------------------------------------------------------
char ProgramShader::s_logBuffer[MAX_SHADER_LOG_SIZE];		//!< �G���[�ۑ��p


//-----------------------------------------------------------------------------
//! �V�F�[�_�v���O�����̏�����
//!	@param	[in]	vertex		�o�[�e�b�N�X�V�F�[�_
//!	@param	[in]	fragment	�t���O�����g�V�F�[�_
//-----------------------------------------------------------------------------
bool ProgramShader::createProgram(GLuint& vertex, GLuint& fragment)
{
	//! �V�F�[�_�v���O�����̃����N���ʂ𓾂�ϐ�
	GLint linked;
	
	//! �v���O�����I�u�W�F�N�g�̍쐬
	_myProgram = glCreateProgram();
	
	//! �V�F�[�_�I�u�W�F�N�g�̃V�F�[�_�v���O�����ւ̓o�^
	glAttachShader(_myProgram, vertex);
	glAttachShader(_myProgram, fragment);

	//! Color���p�C�v���C���ɐݒ�
	//glBindFragDataLocation(myProgram, 0, "fragColor");

	//! �V�F�[�_�v���O�����̃����N
	glLinkProgram(_myProgram);
	glGetProgramiv(_myProgram, GL_LINK_STATUS, &linked);
	printProgramInfoLog(_myProgram);
	if(linked == GL_FALSE) {
		MessageBox(NULL, "Link error.\n", NULL, MB_OK);
		return false;
	}
	
	return true;
}

//-----------------------------------------------------------------------------
//! UniformBlock�Ɠn�����ԍ���R�t��
//!	@param	[in]	blockName		UniformBlock�̖��O
//!	@param	[in]	bindingPoint	�o�C���h�ԍ�
//-----------------------------------------------------------------------------
bool ProgramShader::BindUniformBlockPoint(GM_CSTR blockName, GLuint& bindingPoint)
{
	// blockName��UniformBlock�̃C���f�b�N�X�擾
	GLuint blockIndex = glGetUniformBlockIndex(_myProgram, blockName);
	if( blockIndex == -1 ) return false;
	// �n���ꂽ�ԍ���UniformBlock��R�t��
	glUniformBlockBinding(_myProgram, blockIndex, bindingPoint);
	
	// �G���[�擾
	getGLError();

	return true;
}




//-----------------------------------------------------------------------------
//! �V�F�[�_�����N�G���[�̏o��
//-----------------------------------------------------------------------------
void ProgramShader::printProgramInfoLog(GLuint program)
{
	int logSize;
	int length;

	/* ���O�̒����́A�Ō��NULL�������܂� */
	glGetProgramiv(program, GL_INFO_LOG_LENGTH , &logSize);

	if (logSize > 1)
	{
		glGetProgramInfoLog(program, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
		// �G���[�ł͂Ȃ������烁�b�Z�[�W�{�b�N�X�͂����Ȃ�
		if ((strcmp(s_logBuffer, "No errors.\n") != 0)) {
			stringstream str;
			str << "Program Info Log\n";
			str << s_logBuffer;
			str << "\n";
			MessageBoxA(NULL, str.str().c_str(), NULL, MB_OK);
		}
	}
}
//=============================================================================
// END OF FILE
//=============================================================================