//-----------------------------------------------------------------------------
//!
//!	@file	gmFragmentShader.cpp
//!	@brief	フラグメントシェーダ
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
FragmentShader::FragmentShader()
{
}
	
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
FragmentShader::~FragmentShader()
{
	//! シェーダオブジェクトの削除
	glDeleteShader(myShader);
}


//-----------------------------------------------------------------------------
//! シェーダプログラムの初期化
//!	@param	[in]	filePath
//-----------------------------------------------------------------------------
bool FragmentShader::LoadAndCompile(GM_CSTR filePath)
{
	//! シェーダプログラムのコンパイル/リンク結果を得る変数
	GLint compiled;
	
	//! シェーダオブジェクトの作成
	myShader = glCreateShader(GL_FRAGMENT_SHADER);
	if( !readShaderSource(filePath) ) return false;

	//! バーテックスシェーダのソースプログラムのコンパイル
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