//-----------------------------------------------------------------------------
//!
//!	@file	gmShaderObjectBase.cpp
//!	@brief	シェーダ基底
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
ShaderObjectBase::ShaderObjectBase()
{
}
	
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
ShaderObjectBase::~ShaderObjectBase()
{
}

//-----------------------------------------------------------------------------
// 静的変数の宣言
//-----------------------------------------------------------------------------
char ShaderObjectBase::s_logBuffer[MAX_SHADER_LOG_SIZE];		//!< エラー保存用


//-----------------------------------------------------------------------------
//! シェーダの読み込み
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

	// ファイルを開く
	if((err = fopen_s(&fp, filePath.c_str(), "rb")) != 0 )
	{
		MessageBox(NULL, "file is not found!!\n", NULL, MB_OK);
		return false;
	}

	//! ファイルサイズの取得
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	//! シェーダプログラムの読み込み領域を確保
	if( (source = (GLchar*)malloc(size)) == nullptr )
	{
		MessageBox(NULL, "Memory is not enough for this file\n", NULL, MB_OK);
		fclose(fp);
		return false;
	}

	//! ファイルを先頭から読み込む
	fseek(fp, 0, SEEK_SET);
	int ret = fread((void*)source, 1, size, fp) != (size_t)size;


	//! シェーダオブジェクトにプログラムをセット
	if(ret){
		MessageBox(NULL, "Could not read file\n", NULL, MB_OK);
	}else{
		glShaderSource(myShader, 1, &source, &size);
	}

	//! シェーダ読み込み領域の開放
	free((void*)source);
	fclose(fp);

	return true;
}


//-----------------------------------------------------------------------------
//! シェーダコンパイルエラーの出力
//-----------------------------------------------------------------------------
void ShaderObjectBase::printShaderInfoLog(GLuint shader)
{
	int logSize;
	int length;

	/* ログの長さは、最後のNULL文字も含む */
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &logSize);


	if (logSize > 1)
	{
		glGetShaderInfoLog(shader, MAX_SHADER_LOG_SIZE, &length, s_logBuffer);
		
		// エラーではなかったらメッセージボックスはださない
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