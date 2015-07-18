//-----------------------------------------------------------------------------
//!
//!	@file	gmVertexShader.h
//!	@brief	頂点シェーダ
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//!	頂点シェーダ
//-----------------------------------------------------------------------------
class VertexShader : public ShaderObjectBase
{
public:

	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	VertexShader();
	
	//! デストラクタ
	virtual ~VertexShader();

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