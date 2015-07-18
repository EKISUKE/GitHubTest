//-----------------------------------------------------------------------------
//!
//!	@file	gmShaderObjectBase.cpp
//!	@brief	�V�F�[�_���
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
ShaderObjectBase::ShaderObjectBase()
{
}
	
//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
ShaderObjectBase::~ShaderObjectBase()
{
}

//-----------------------------------------------------------------------------
// �ÓI�ϐ��̐錾
//-----------------------------------------------------------------------------
char ShaderObjectBase::s_logBuffer[MAX_SHADER_LOG_SIZE];		//!< �G���[�ۑ��p


//-----------------------------------------------------------------------------
//! �V�F�[�_�̓ǂݍ���
//!	@param	[in]	filePath
//-----------------------------------------------------------------------------
bool ShaderObjectBase::readShaderSource(GM_CSTR fileName)
{
	errno_t err;
	FILE*	fp;
	int		size;
	const GLchar* source;

	string	filePath = SHADER_PATH;

	filePath += fileName;

	// �t�@�C�����J��
	if((err = fopen_s(&fp, filePath.c_str(), "rb")) != 0 )
	{
		MessageBox(NULL, "file is not found!!\n", NULL, MB_OK);
		return false;
	}

	//! �t�@�C���T�C�Y�̎擾
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	//! �V�F�[�_�v���O�����̓ǂݍ��ݗ̈���m��
	if( (source = (GLchar*)malloc(size)) == nullptr )
	{
		MessageBox(NULL, "Memory is not enough for this file\n", NULL, MB_OK);
		fclose(fp);
		return false;
	}

	//! �t�@�C����擪����ǂݍ���
	fseek(fp, 0, SEEK_SET);
	int ret = fread((void*)source, 1, size, fp) != (size_t)size;


	//! �V�F�[�_�I�u�W�F�N�g�Ƀv���O�������Z�b�g
	if(ret){
		MessageBox(NULL, "Could not read file\n", NULL, MB_OK);
	}else{
		glShaderSource(myShader, 1, &source, &size);
	}

	//! �V�F�[�_�ǂݍ��ݗ̈�̊J��
	free((void*)source);
	fclose(fp);

	return true;
}


//-----------------------------------------------------------------------------
//! �V�F�[�_�R���p�C���G���[�̏o��
//-----------------------------------------------------------------------------
void ShaderObjectBase::printShaderInfoLog(GLuint shader)
{
	int logSize;
	int length;

	/* ���O�̒����́A�Ō��NULL�������܂� */
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &logSize);


	if (logSize > 1)
	{
		glGetShaderInfoLog(shader, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
		
		// �G���[�ł͂Ȃ������烁�b�Z�[�W�{�b�N�X�͂����Ȃ�
		if ((strcmp(s_logBuffer, "No errors.\n") != 0)) {
			stringstream str;
			str << "Shader Info Log";
			str << s_logBuffer;
			str << "\n";

			MessageBoxA(NULL, str.str().c_str(), NULL, MB_OK);
		}
	}
}

//=============================================================================
// END OF FILE
//=============================================================================