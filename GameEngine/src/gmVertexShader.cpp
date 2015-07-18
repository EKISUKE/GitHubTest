//-----------------------------------------------------------------------------
//!
//!	@file	gmVertexShader.cpp
//!	@brief	頂点シェーダ
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
VertexShader::VertexShader()
{
}
	
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
VertexShader::~VertexShader()
{
	//! シェーダオブジェクトの削除
	glDeleteShader(myShader);
}



//-----------------------------------------------------------------------------
//! シェーダプログラムの初期化
//!	@param	[in]	filePath
//-----------------------------------------------------------------------------
bool VertexShader::LoadAndCompile(GM_CSTR filePath)
{
	//! シェーダプログラムのコンパイル/リンク結果を得る変数
	GLint compiled;
	
	//! シェーダオブジェクトの作成
	myShader = glCreateShader(GL_VERTEX_SHADER);

	//! シェーダのソースプログラムの読み込み
	if( !readShaderSource(filePath) ) return false;

	//! バーテックスシェーダのソースプログラムのコンパイル
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