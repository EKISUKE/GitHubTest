//-----------------------------------------------------------------------------
//!
//!	@file	gmProgramShader.h
//!	@brief	�V�F�[�_�v���O����
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	�V�F�[�_�v���O����
//-----------------------------------------------------------------------------
class ProgramShader
{
public:

	//-------------------------------------------------------------
	//! @name ������
	//-------------------------------------------------------------
	//@{
	//! �R���X�g���N�^
	ProgramShader();
	
	//! �f�X�g���N�^
	virtual ~ProgramShader();

	//! �V�F�[�_�v���O�����̏�����
	//!	@param	[in]	vertex		�o�[�e�b�N�X�V�F�[�_
	//!	@param	[in]	fragment	�t���O�����g�V�F�[�_
	bool		createProgram(GLuint& vertex, GLuint& fragment);

	//@}
	//-------------------------------------------------------------
	//! @name �擾�E�ݒ�
	//-------------------------------------------------------------
	//@{

	//! UniformBlock�Ɠn�����ԍ���R�t��
	//!	@param	[in]	blockName		UniformBlock�̖��O
	//!	@param	[in]	bindingPoint	�o�C���h�ԍ�
	bool		BindUniformBlockPoint(GM_CSTR blockName, GLuint& bindingPoint);

	//! �V�F�[�_�v���O�����̎擾
	GLuint		&getShaderProgram() { return _myProgram; }

	//@}
private:
	
	//! �V�F�[�_�����N�G���[�̏o��
	static void printProgramInfoLog(GLuint program);

private:
	GLuint			_myProgram;	//!< �K�p�p�̃v���O����
	//! �V�F�[�_�R���p�C���̃��O�o�b�t�@ 
	static const s32 MAX_SHADER_LOG_SIZE = 1024;
	static char s_logBuffer[MAX_SHADER_LOG_SIZE];
};


//=============================================================================
// END OF FILE
//=============================================================================