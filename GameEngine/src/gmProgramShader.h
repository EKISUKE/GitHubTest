//-----------------------------------------------------------------------------
//!
//!	@file	gmProgramShader.h
//!	@brief	シェーダプログラム
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	シェーダプログラム
//-----------------------------------------------------------------------------
class ProgramShader
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{
	//! コンストラクタ
	ProgramShader();
	
	//! デストラクタ
	virtual ~ProgramShader();

	//! シェーダプログラムの初期化
	//!	@param	[in]	vertex		バーテックスシェーダ
	//!	@param	[in]	fragment	フラグメントシェーダ
	bool		createProgram(GLuint& vertex, GLuint& fragment);

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! UniformBlockと渡した番号を紐付け
	//!	@param	[in]	blockName		UniformBlockの名前
	//!	@param	[in]	bindingPoint	バインド番号
	bool		BindUniformBlockPoint(GM_CSTR blockName, GLuint& bindingPoint);

	//! シェーダプログラムの取得
	GLuint		&getShaderProgram() { return _myProgram; }

	//@}
private:
	
	//! シェーダリンクエラーの出力
	static void printProgramInfoLog(GLuint program);

private:
	GLuint			_myProgram;	//!< 適用用のプログラム
	//! シェーダコンパイルのログバッファ 
	static const s32 MAX_SHADER_LOG_SIZE = 1024;
	static char s_logBuffer[MAX_SHADER_LOG_SIZE];
};


//=============================================================================
// END OF FILE
//=============================================================================