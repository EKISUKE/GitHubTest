//-----------------------------------------------------------------------------
//!
//!	@file	gmVertexArrayObject.cpp
//!	@brief	頂点フォーマット
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//---- 頂点フォーマット実装

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
// VAOクラスの実装
//=============================================================================


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
VertexArrayObject::VertexArrayObject()
{
	
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
VertexArrayObject::~VertexArrayObject()
{
}

//-----------------------------------------------------------------------------
//! VAO作成
//-----------------------------------------------------------------------------
void VertexArrayObject::genVAO()
{
	// VAOの作成
	glGenVertexArrays(1, &_vao);
}
//-----------------------------------------------------------------------------
//! VAO削除
//-----------------------------------------------------------------------------
void VertexArrayObject::delVAO()
{
	// VAOの削除
	glDeleteVertexArrays(1, &_vao);
}

//-----------------------------------------------------------------------------
//! コンテキストの登録開始
//-----------------------------------------------------------------------------
void VertexArrayObject::beginRegistContext()
{
	glBindVertexArray(_vao);
}

//-----------------------------------------------------------------------------
//! コンテキストの登録終了
//-----------------------------------------------------------------------------
void VertexArrayObject::endRegistContext()
{
	glBindVertexArray(0);
}

//-----------------------------------------------------------------------------
//! 描画開始
//-----------------------------------------------------------------------------
void VertexArrayObject::beginRender()
{
	glBindVertexArray(_vao);
}

//-----------------------------------------------------------------------------
//! 描画終了
//-----------------------------------------------------------------------------
void VertexArrayObject::endRender()
{
	glBindVertexArray(0);
}

//=============================================================================
// END OF FILE
//=============================================================================