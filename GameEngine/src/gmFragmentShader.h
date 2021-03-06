//-----------------------------------------------------------------------------
//!
//!	@file	gmFragmentShader.h
//!	@brief	フラグメントシェーダ
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	フラグメントシェーダ
//-----------------------------------------------------------------------------
class FragmentShader : public ShaderObjectBase
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	FragmentShader();
	
	//! デストラクタ
	virtual ~FragmentShader();

	//@}
	//-------------------------------------------------------------
	//! @name タスク関数
	//-------------------------------------------------------------
	//@{

	//! シェーダプログラム読み込み
	//!	@param	[in]	filePath
	virtual bool		LoadAndCompile(GM_CSTR filePath);
	
	//@}
};

//=============================================================================
// END OF FILE
//=============================================================================