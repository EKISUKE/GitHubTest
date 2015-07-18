//-----------------------------------------------------------------------------
//!
//!	@file	gmVertexArrayObject.cpp
//!	@brief	���_�t�H�[�}�b�g
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//---- ���_�t�H�[�}�b�g����

void NormalContext::init(GLuint program)
{
	_positionLocation		= glGetAttribLocation(program, "position");
	_blendIndicesLocation	= glGetAttribLocation(program, "blendIndices");
	_blendWeightLocation	= glGetAttribLocation(program, "blendWeight");
	_normalLocation			= glGetAttribLocation(program, "normal");
	_uvLocation				= glGetAttribLocation(program, "uv");
	_specularPowerLocation	= glGetAttribLocation(program, "specularPower");
}

void ShadowContext::init(GLuint program)
{
	_positionLocation		= glGetAttribLocation(program, "position");
	_blendIndicesLocation	= glGetAttribLocation(program, "blendIndices");
	_blendWeightLocation	= glGetAttribLocation(program, "blendWeight");
}

void FilterContext::init(GLuint program)
{
	_positionLocation		= glGetAttribLocation(program, "position");
	_uvLocation				= glGetAttribLocation(program, "uv");
	_diffuseColorLocation	= glGetAttribLocation(program, "color");
	_emissiveColorLocation	= glGetAttribLocation(program, "emissiveColor");
}



//=============================================================================
// VAO�N���X�̎���
//=============================================================================


//-----------------------------------------------------------------------------
//! �R���X�g���N�^
//-----------------------------------------------------------------------------
VertexArrayObject::VertexArrayObject()
{
	
}

//-----------------------------------------------------------------------------
//! �f�X�g���N�^
//-----------------------------------------------------------------------------
VertexArrayObject::~VertexArrayObject()
{
}

//-----------------------------------------------------------------------------
//! VAO�쐬
//-----------------------------------------------------------------------------
void VertexArrayObject::genVAO()
{
	// VAO�̍쐬
	glGenVertexArrays(1, &_vao);
}
//-----------------------------------------------------------------------------
//! VAO�폜
//-----------------------------------------------------------------------------
void VertexArrayObject::delVAO()
{
	// VAO�̍폜
	glDeleteVertexArrays(1, &_vao);
}

//-----------------------------------------------------------------------------
//! �R���e�L�X�g�̓o�^�J�n
//-----------------------------------------------------------------------------
void VertexArrayObject::beginRegistContext()
{
	glBindVertexArray(_vao);
}

//-----------------------------------------------------------------------------
//! �R���e�L�X�g�̓o�^�I��
//-----------------------------------------------------------------------------
void VertexArrayObject::endRegistContext()
{
	glBindVertexArray(0);
}

//-----------------------------------------------------------------------------
//! �`��J�n
//-----------------------------------------------------------------------------
void VertexArrayObject::beginRender()
{
	glBindVertexArray(_vao);
}

//-----------------------------------------------------------------------------
//! �`��I��
//-----------------------------------------------------------------------------
void VertexArrayObject::endRender()
{
	glBindVertexArray(0);
}

//=============================================================================
// END OF FILE
//=============================================================================