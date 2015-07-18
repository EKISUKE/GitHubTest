//-----------------------------------------------------------------------------
//!
//!	@file	gmVertexArrayObject.h
//!	@brief	頂点フォーマット
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! 通常描画(アニメーション付)VAOフォーマット
//-----------------------------------------------------------------------------
struct NormalContext
{
	GLuint	_positionLocation;
	GLuint	_blendIndicesLocation;
	GLuint	_blendWeightLocation;
	GLuint	_normalLocation;
	GLuint	_uvLocation;
	GLuint	_specularPowerLocation;		
	//! プログラムハンドルから初期化
	void	init(GLuint program);
};

//-----------------------------------------------------------------------------
//! シャドウ用VAOフォーマット
//-----------------------------------------------------------------------------
struct ShadowContext
{
	GLuint	_positionLocation;
	GLuint	_blendIndicesLocation;
	GLuint	_blendWeightLocation;
	//! プログラムハンドルから初期化
	void	init(GLuint program);
};

//-----------------------------------------------------------------------------
//! フィルター用VAOフォーマット
//-----------------------------------------------------------------------------
struct FilterContext
{
	GLuint	_positionLocation;
	GLuint	_uvLocation;
	GLuint	_diffuseColorLocation;
	GLuint	_emissiveColorLocation;
	//! プログラムハンドルから初期化
	void	init(GLuint program);
};

//-----------------------------------------------------------------------------
//! VAOクラス
//-----------------------------------------------------------------------------
class VertexArrayObject
{
public:
	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{

	//! コンストラクタ
	VertexArrayObject();
	//! デストラクタ
	virtual ~VertexArrayObject();

	//! VAO作成
	void	genVAO();
	//! VAO削除
	void	delVAO();

	//@}
	//-------------------------------------------------------------
	//! @name 開始・終了
	//-------------------------------------------------------------
	//@{

	

	//! コンテキストの登録開始
	//void	beginRegistContext(GLuint& _vbo, GLuint& ibo);
	void	beginRegistContext();
	//! コンテキストの登録終了
	void	endRegistContext();
	//! 描画開始
	void	beginRender();
	//! 描画終了
	void	endRender();	

	//@}
private:
	GLuint	_vao;	
};




//=============================================================================
// END OF FILE
//=============================================================================