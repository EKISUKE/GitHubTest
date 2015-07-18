//-----------------------------------------------------------------------------
//!
//!	@file	gmProgramShader.cpp
//!	@brief	シェーダプログラム
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
ProgramShader::ProgramShader()
{
}
	
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
ProgramShader::~ProgramShader()
{
}

//-----------------------------------------------------------------------------
// 静的変数の宣言
//-----------------------------------------------------------------------------
char ProgramShader::s_logBuffer[MAX_SHADER_LOG_SIZE];		//!< エラー保存用


//-----------------------------------------------------------------------------
//! シェーダプログラムの初期化
//!	@param	[in]	vertex		バーテックスシェーダ
//!	@param	[in]	fragment	フラグメントシェーダ
//-----------------------------------------------------------------------------
bool ProgramShader::createProgram(GLuint& vertex, GLuint& fragment)
{
	//! シェーダプログラムのリンク結果を得る変数
	GLint linked;
	
	//! プログラムオブジェクトの作成
	_myProgram = glCreateProgram();
	
	//! シェーダオブジェクトのシェーダプログラムへの登録
	glAttachShader(_myProgram, vertex);
	glAttachShader(_myProgram, fragment);

	//! Colorをパイプラインに設定
	//glBindFragDataLocation(myProgram, 0, "fragColor");

	//! シェーダプログラムのリンク
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
//! UniformBlockと渡した番号を紐付け
//!	@param	[in]	blockName		UniformBlockの名前
//!	@param	[in]	bindingPoint	バインド番号
//-----------------------------------------------------------------------------
bool ProgramShader::BindUniformBlockPoint(GM_CSTR blockName, GLuint& bindingPoint)
{
	// blockNameのUniformBlockのインデックス取得
	GLuint blockIndex = glGetUniformBlockIndex(_myProgram, blockName);
	if( blockIndex == -1 ) return false;
	// 渡された番号とUniformBlockを紐付け
	glUniformBlockBinding(_myProgram, blockIndex, bindingPoint);
	
	// エラー取得
	getGLError();

	return true;
}




//-----------------------------------------------------------------------------
//! シェーダリンクエラーの出力
//-----------------------------------------------------------------------------
void ProgramShader::printProgramInfoLog(GLuint program)
{
	int logSize;
	int length;

	/* ログの長さは、最後のNULL文字も含む */
	glGetProgramiv(program, GL_INFO_LOG_LENGTH , &logSize);

	if (logSize > 1)
	{
		glGetProgramInfoLog(program, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
		// エラーではなかったらメッセージボックスはださない
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