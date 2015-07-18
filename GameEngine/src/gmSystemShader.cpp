//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemShader.h
//!	@brief	シェーダ管理システム
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "stdafx.h"

//-----------------------------------------------------------------------------
// 静的変数の宣言
//-----------------------------------------------------------------------------
ProgramShader SystemShader::_program[SHADER_MAX];		//!< 適用用のプログラム

//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
SystemShader::SystemShader()
: _matrixBindingPoint(1)
, _currentProgram	 (SHADER_TEXTURE)
, _nullTexture		 (nullptr)
, _nullSpecular		 (nullptr)
, _nullNormal		 (nullptr)
{
	SAFE_NEW(_nullTexture);
	_nullTexture->loadAndTexImage2D("../Image/nullTexture.jpg");
	SAFE_NEW(_nullSpecular);
	_nullSpecular->loadAndTexImage2D("../Image/nullSpecular.jpg");
	SAFE_NEW(_nullNormal);
	_nullNormal->loadAndTexImage2D("../Image/nullNormal.jpg");

}
	
//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
SystemShader::~SystemShader()
{
	SAFE_DELETE(_nullTexture);
	SAFE_DELETE(_nullSpecular);
	SAFE_DELETE(_nullNormal);
}


//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool SystemShader::init()
{
	char buf[255];

	//! GLSLの初期化
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		return false;
	}

	sprintf_s(buf, "OpenGLVersion : (%s)", glGetString(GL_VERSION));

	sprintf_s(buf, "OpenGLVersion : (%s)", glGetString(GL_SHADING_LANGUAGE_VERSION));


	VertexShader	allVertex;
	allVertex.LoadAndCompile("Test.vert");
	FragmentShader	allFrag;
	allFrag.LoadAndCompile("Test.frag");

	//---- デバッグ描画
	VertexShader	normalVertex;
	//normalVertex.LoadAndCompile("noTexture.vert");
	normalVertex.LoadAndCompile("debugDraw.vert");
	FragmentShader	normalFrag;
	//normalFrag.LoadAndCompile("noTexture.frag");
	normalFrag.LoadAndCompile("debugDraw.frag");

	//---- シャドウ書き込み用
	VertexShader	shadowVertex;
	shadowVertex.LoadAndCompile("Shadow.vert");
	FragmentShader	shadowFrag;
	shadowFrag.LoadAndCompile("Shadow.frag");

	//---- テクスチャデバッグ用
	VertexShader	debugVertex;
	debugVertex.LoadAndCompile("debugTexture.vert");
	FragmentShader	debugFrag;
	debugFrag.LoadAndCompile("debugTexture.frag");

	//---- キューブマップ描画用
	VertexShader	drawCubeMapVertex;
	drawCubeMapVertex.LoadAndCompile("drawCubeMap.vert");
	FragmentShader	drawCubeMapFrag;
	drawCubeMapFrag.LoadAndCompile("drawCubeMap.frag");

	//---- 通常描画用(テクスチャあり)
	VertexShader	standardVertex;
	standardVertex.LoadAndCompile("standard.vert");
	FragmentShader	standardFrag;
	standardFrag.LoadAndCompile("standard.frag");

	//---- 通常描画用(テクスチャなし)
	FragmentShader  standardColorFrag;
	standardColorFrag.LoadAndCompile("standardColor.frag");

	//---- 2D描画用
	VertexShader	filterVertex;
	filterVertex.LoadAndCompile("Filter.vert");
	FragmentShader	filterFrag;
	filterFrag.LoadAndCompile("Filter.frag");

	//---- モーションブラー用
	VertexShader	motionBlurVertex;
	motionBlurVertex.LoadAndCompile("MotionBlur.vert");
	FragmentShader	motionBlurFrag;
	motionBlurFrag.LoadAndCompile("MotionBlur.frag");

	//---- ブルーム用ぼかしフィルター
	VertexShader	gaussian3x3_V_Vertex;
	gaussian3x3_V_Vertex.LoadAndCompile("Gaussian3x3_V.vert");
	VertexShader	gaussian3x3_H_Vertex;
	gaussian3x3_H_Vertex.LoadAndCompile("Gaussian3x3_H.vert");
	FragmentShader	gaussian3x3_Frag;
	gaussian3x3_Frag.LoadAndCompile("Gaussian3x3.frag");
	//---- ブルームフィルター
	FragmentShader	bloomGlareFrag;
	bloomGlareFrag.LoadAndCompile("BloomGlare.frag");

	//---- 減算フィルター
	FragmentShader	subtractFrag;
	subtractFrag.LoadAndCompile("Subtract.frag");

	//---- トーンマッピング用
	VertexShader	getLuminanceVertex;
	getLuminanceVertex.LoadAndCompile("GetLuminance.vert");
	FragmentShader	getLuminanceFrag;
	getLuminanceFrag.LoadAndCompile("GetLuminance.frag");

	VertexShader	luminanceReductionVertex;
	luminanceReductionVertex.LoadAndCompile("LuminanceReduction.vert");
	FragmentShader	luminanceReductionFrag;
	luminanceReductionFrag.LoadAndCompile("LuminanceReduction.frag");

	FragmentShader	lightAdaptationFrag;
	lightAdaptationFrag.LoadAndCompile("LightAdaptation.frag");

	FragmentShader	toneMappingFinalFrag;
	toneMappingFinalFrag.LoadAndCompile("ToneMappingFinal.frag");

	//---- グレア用
	VertexShader	glareFilterVertex;
	glareFilterVertex.LoadAndCompile("GlareFilter.vert");
	FragmentShader	glareFilterFrag;
	glareFilterFrag.LoadAndCompile("GlareFilter.frag");

	//---- レンズフレア用
	FragmentShader	lensFlareFrag;
	lensFlareFrag.LoadAndCompile("LensFlare.frag");
	FragmentShader	lensDirtStarFrag;
	lensDirtStarFrag.LoadAndCompile("AddLensEffects.frag");

	VertexShader	anamolFlareFilterVertex;
	anamolFlareFilterVertex.LoadAndCompile("AnamolphicFlareFilter.vert");
	FragmentShader	anamolFlareFilterFrag;
	anamolFlareFilterFrag.LoadAndCompile("AnamolphicFlareFilter.frag");

	VertexShader	ExpandBlurVertex;
	ExpandBlurVertex.LoadAndCompile("ExpandBlur.vert");
	FragmentShader	ExpandBlurFrag;
	ExpandBlurFrag.LoadAndCompile("ExpandBlur.frag");


	// 標準描画
	if( !InitShader(SHADER_STANDARD,			standardVertex,				standardFrag) )				return false;
	getGLError();
	// 標準描画(テクスチャなし)
	if( !InitShader(SHADER_COLOR,				standardVertex,				standardColorFrag) )		return false;
	getGLError();
	// 通常描画用
	if( !InitShader(SHADER_NORMAL,				normalVertex,				normalFrag) )				return false;
	getGLError();
	// すべてのエフェクト用
	if( !InitShader(SHADER_TEXTURE,				allVertex,					allFrag) )					return false;
	getGLError();
	// シャドウ書き込み用
	if( !InitShader(SHADER_SHADOW,				shadowVertex,				shadowFrag) )				return false;
	getGLError();
	// デバッグ描画用
	if( !InitShader(SHADER_DEBUG,				debugVertex,				debugFrag) )				return false;
	getGLError();
	// キューブマップ描画用
	if( !InitShader(SHADER_DRAWCUBEMAP,			drawCubeMapVertex,			drawCubeMapFrag) )			return false;
	getGLError();
	// ポストエフェクト用(ミップマップ付き)
	if( !InitShader(SHADER_FILTER,				filterVertex,				filterFrag) )				return false;
	getGLError();
	// 水平ブラー
	if( !InitShader(SHADER_GAUSSIAN_3X3_H,		gaussian3x3_H_Vertex,		gaussian3x3_Frag) )			return false;
	getGLError();
	// 垂直ブラー
	if( !InitShader(SHADER_GAUSSIAN_3X3_V,		gaussian3x3_V_Vertex,		gaussian3x3_Frag) )			return false;
	getGLError();
	// 減算
	if( !InitShader(SHADER_SUBTRACT,			filterVertex,				subtractFrag) )				return false;
	getGLError();
	// ブルームグレア
	if( !InitShader(SHADER_BLOOM_GLARE,			filterVertex,				bloomGlareFrag) )			return false;
	getGLError();
	// 最初の輝度を生成
	if( !InitShader(SHADER_GET_LUMINANCE,		getLuminanceVertex,			getLuminanceFrag) )			return false;
	getGLError();
	// 縮小していく
	if( !InitShader(SHADER_LUMINANCE_REDUCTION,	luminanceReductionVertex,	luminanceReductionFrag) )	return false;
	getGLError();
	// 明順応 / 暗順応
	if( !InitShader(SHADER_LIGHT_ADAPTATION,	filterVertex,				lightAdaptationFrag) )		return false;
	getGLError();
	// トーンマッピング最終
	if( !InitShader(SHADER_TONEMAPPING_FINAL,	filterVertex,				toneMappingFinalFrag) )		return false;
	getGLError();
	// グレア
	if( !InitShader(SHADER_GLARE,				glareFilterVertex,			glareFilterFrag) )			return false;
	getGLError();
	// モーションブラー
	if( !InitShader(SHADER_MOTION_BLUR,			motionBlurVertex,			motionBlurFrag) )			return false;
	getGLError();
	// 2D描画用(テクスチャあり)
	if( !InitShader(SHADER_PUT_TEXTURE,			filterVertex,				standardFrag) )				return false;
	getGLError();
	// 2D描画用(テクスチャなし)
	if( !InitShader(SHADER_RENDER_2D,			filterVertex,				standardColorFrag) )		return false;
	getGLError();
	// レンズフレア
	if( !InitShader(SHADER_LENS_FLARE,			filterVertex,				lensFlareFrag) )			return false;
	getGLError();
	// レンズ汚れ、スターバースト
	if( !InitShader(SHADER_LENS_DIRT_STAR,		filterVertex,				lensDirtStarFrag) )			return false;
	getGLError();
	// アナモルフィックのフレア
	if( !InitShader(SHADER_ANAMOLPHIC_FLARE,	anamolFlareFilterVertex,	anamolFlareFilterFrag) )	return false;
	getGLError();
	// 4点0.25フェッチ拡大ブラー
	if( !InitShader(SHADER_EXPAND_BLUR,			ExpandBlurVertex,			ExpandBlurFrag) )			return false;

	getGLError();
	
	
	


	////! シェーダプログラムの初期化
	//for( s32 i=0; i<SHADER_MAX; i++ )
	//{
	//	if( !InitShader( (SHADER_MODE)(i) ) ) return false;
	//}

	// UniformBlock初期化
	InitUniformBlock();

	// コンテキスト初期化
	InitContext();

	_changeShader = false;

	return true;
}

//-----------------------------------------------------------------------------
//! 解放
//-----------------------------------------------------------------------------
void SystemShader::cleanup()
{
}

//-----------------------------------------------------------------------------
//! シェーダプログラムの初期化
//! シェーダプログラムの初期化
//!	@param	[in]	mode	シェーダのモード
//!	@param	[in]	vertex	バーテックスシェーダ
//!	@param	[in]	vertex	フラグメントシェーダ
//-----------------------------------------------------------------------------
bool SystemShader::InitShader(SHADER_MODE mode, VertexShader& vertex, FragmentShader& frag)
{
	if( !_program[mode].createProgram(vertex.getShader(), frag.getShader()) ) return false;
	return true;
}

//-----------------------------------------------------------------------------
//! UniformBlockの初期化
//-----------------------------------------------------------------------------
bool SystemShader::InitUniformBlock()
{
	//---- 使うシェーダにUniformBlockをバインドする
	_program[SHADER_TEXTURE].BindUniformBlockPoint("Matrices", _matrixBindingPoint);
	_program[SHADER_SHADOW].BindUniformBlockPoint("Matrices", _matrixBindingPoint);
	_program[SHADER_DRAWCUBEMAP].BindUniformBlockPoint("Matrices", _matrixBindingPoint);
	_program[SHADER_STANDARD].BindUniformBlockPoint("Matrices", _matrixBindingPoint);
	_program[SHADER_COLOR].BindUniformBlockPoint("Matrices", _matrixBindingPoint);

	GLsizei dataSize = ( sizeof(Matrix) * 3 );

	// バッファ確保
	glGenBuffers(1, &_uniformBuffer);
	// 紐付け
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBuffer);
	// データ転送領域確保
	glBufferData(GL_UNIFORM_BUFFER, dataSize, nullptr, GL_DYNAMIC_DRAW);
	// バッファとバインドポイントをひも付け
	glBindBufferBase(GL_UNIFORM_BUFFER, _matrixBindingPoint, _uniformBuffer);


	// 紐付け開放
	glBindBuffer(GL_UNIFORM_BUFFER, 0 );

	
	return true;
}

//-----------------------------------------------------------------------------
//! コンテキストの初期化
//-----------------------------------------------------------------------------
bool SystemShader::InitContext()
{
	_normalContext.init(_program[SHADER_TEXTURE].getShaderProgram());
	_shadowContext.init(_program[SHADER_SHADOW].getShaderProgram());
	_filterContext.init(_program[SHADER_PUT_TEXTURE].getShaderProgram());

	return true;
}

//-----------------------------------------------------------------------------
//! シェーダの開始
//-----------------------------------------------------------------------------
void SystemShader::beginShader()
{
	if( _currentProgram == SHADER_NONE )
	{
		endShader();
	}else {
		//! シェーダプログラムの適用
		glUseProgram(_program[_currentProgram].getShaderProgram());
	}
}

//-----------------------------------------------------------------------------
//! ひとつ前のシェーダの開始
//-----------------------------------------------------------------------------
void SystemShader::beginPrevShader()
{
	// 変更フラグがOFFなら処理しない
	if( !_changeShader ) return;
	// 今のプログラムとして上書き
	_currentProgram = _prevProgram;
	beginShader();
	// 変更フラグをOFFにする
	_changeShader = false;
}

//-----------------------------------------------------------------------------
//! シェーダの終了
//-----------------------------------------------------------------------------
void SystemShader::endShader()
{
	setPass(SHADER_NONE);
	//! シェーダプログラムの適用
	glUseProgram(GL_NONE);

}

//-----------------------------------------------------------------------------
//! シェーダプログラム変更
//!	@param	[in]	mode	シェーダモード
//-----------------------------------------------------------------------------
void SystemShader::changeShader(SHADER_MODE mode)
{
	// 同じものなので処理しない
	if( _currentProgram == mode ) return;
	// チェンジフラグON
	_changeShader = true;
	// 新しいシェーダを設定
	setPass(mode);
	// シェーダを開始
	beginShader();
}



//-----------------------------------------------------------------------------
//! シェーダの種類設定
//!	@param	[in]	mode	シェーダモード
//-----------------------------------------------------------------------------
void SystemShader::setPass(SHADER_MODE mode)
{
	_prevProgram	= _currentProgram;
	_currentProgram = mode;
}

//=============================================================================
// Uniform系設定
//=============================================================================

//-----------------------------------------------------------------------------
//! マトリックスデータを転送
//!	@param	[in]	data		行列データ
//!	@param	[in]	dataSize	データサイズ
//-----------------------------------------------------------------------------
void SystemShader::setUniformMatrixBuffer(Matrix* data, GLsizei dataSize)
{

	// 紐付け
	glBindBuffer(GL_UNIFORM_BUFFER, _uniformBuffer);
	
	glBufferSubData(GL_UNIFORM_BUFFER, 0, dataSize, data);

	// 紐付け開放
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	
}

//-----------------------------------------------------------------------------
//! Floatのユニフォーム変数を設定
//!	@param	[in]	name	変数名	
//!	@param	[in]	data	データ
//-----------------------------------------------------------------------------
void SystemShader::setUniform1f(const GLchar* name, GLfloat data)
{
	if(_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	glUniform1f(handle, data);
}

//-----------------------------------------------------------------------------
//! Floatのユニフォーム変数を設定
//!	@param	[in]	name	変数名	
//!	@param	[in]	data	データ
//-----------------------------------------------------------------------------
void SystemShader::setUniform1fv(const GLchar* name, GLfloat* data, GLsizei count)
{
	if(_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	glUniform1fv(handle, count, data);
}

//-----------------------------------------------------------------------------
//! Float2のユニフォーム変数を設定
//!	@param	[in]	name	変数名	
//!	@param	[in]	data	データ
//!	@param	[in]	count	データの個数
//-----------------------------------------------------------------------------
void SystemShader::setUniform2fv(const GLchar* name, GLfloat* data, GLsizei count)
{
	if(_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	glUniform2fv(handle, count, data);
}

//-----------------------------------------------------------------------------
//! Float3のユニフォーム変数を設定
//!	@param	[in]	name	変数名	
//!	@param	[in]	data	データ
//!	@param	[in]	count	データの個数
//-----------------------------------------------------------------------------
void SystemShader::setUniform3fv(const GLchar* name, GLfloat* data, GLsizei count)
{
	if(_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	glUniform3fv(handle, count, data);
}

//-----------------------------------------------------------------------------
//! Float4のユニフォーム変数を設定
//!	@param	[in]	name	変数名	
//!	@param	[in]	data	データ
//!	@param	[in]	count	データの個数
//-----------------------------------------------------------------------------
void SystemShader::setUniform4fv(const GLchar* name, GLfloat* data, GLsizei count)
{
	if(_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	glUniform4fv(handle, count, data);
}

//-----------------------------------------------------------------------------
//! intのユニフォーム変数を設定
//!	@param	[in]	name	変数名	
//!	@param	[in]	data	データ
//-----------------------------------------------------------------------------
void SystemShader::setUniform1i(const GLchar* name, GLint data)
{
	if (_currentProgram == SHADER_NONE) return;
	GLint handle = glGetUniformLocation(_program[_currentProgram].getShaderProgram(), name);
	if (handle == -1) return;
	glUniform1i(handle, data);
}

//-----------------------------------------------------------------------------
//! 行列を設定
//!	@param	[in]	name		変数名	
//!	@param	[in]	dataCount	行列の数
//!	@param	[in]	transpose	転置するかどうか
//!	@param	[in]	data		データ
//-----------------------------------------------------------------------------
void SystemShader::setMatrix4x4(const GLchar* name, GLuint dataCount, GLboolean transpose, GLfloat* data)
{
	if(_currentProgram == SHADER_NONE) return;

	GLint shaderHandle = _program[_currentProgram].getShaderProgram();
	// ハンドル取得
	int handle = glGetUniformLocation(shaderHandle, name);
	if( handle == -1 ) return;
	// 行列転送
	glUniformMatrix4fv(handle, dataCount, transpose,  data);
}

//-----------------------------------------------------------------------------
//! テクスチャ設定
//!	@param	[in]	name		変数名	
//!	@param	[in]	texHandle	テクスチャのハンドル
//!	@param	[in]	texNum		有効化するテクスチャの番号(GL_TEXTURE0←なら0)
//-----------------------------------------------------------------------------
void SystemShader::bindTexture(const GLchar* name, GLuint texHandle, GLuint texNum, GLenum target)
{
	if(_currentProgram == SHADER_NONE) return;

	GM_ASSERT(texNum <= 31, "テクスチャの有効番号を越えています");

	GLint shaderHandle = _program[_currentProgram].getShaderProgram();
	// ハンドル取得
	int handle = glGetUniformLocation(shaderHandle, name);
	if( handle == -1 ) return;
	glActiveTexture(GL_TEXTURE0 + texNum);
	// テクスチャ転送
	glBindTexture(target, texHandle);
	// ハンドルに番号をひも付け
	glUniform1i(handle, texNum);
}

//-----------------------------------------------------------------------------
//! テクスチャ設定解除
//!	@param	[in]	target		アンバインドするターゲット
//-----------------------------------------------------------------------------
void SystemShader::unbindTexture(GLenum target)
{
	if(_currentProgram == SHADER_NONE) return;

	glActiveTexture(GL_TEXTURE0);
	// テクスチャ転送
	glBindTexture(target, 0);
	
}


//=============================================================================
// in(attrib)系設定
//=============================================================================

//-----------------------------------------------------------------------------
//! バーテックス変数を設定する(1fv)
//!	@param	[in]	name	変数名	
//!	@param	[in]	data	データ
//-----------------------------------------------------------------------------
void SystemShader::setVertex1fv(const char* name, GLfloat* data)
{
	if(_currentProgram == SHADER_NONE) return;

	// ハンドル取得
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	// バーテックス変数転送
	glVertexAttrib1fv(handle, data);
}


//-----------------------------------------------------------------------------
//! バーテックス変数を設定する(2fv)
//!	@param	[in]	name	変数名	
//!	@param	[in]	data	データ
//-----------------------------------------------------------------------------
void SystemShader::setVertex2fv(const char* name, GLfloat* data)
{
	if(_currentProgram == SHADER_NONE) return;

	// ハンドル取得
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	// バーテックス変数転送
	glVertexAttrib2fv(handle, data);
}

//-----------------------------------------------------------------------------
//! バーテックス変数を設定する(3fv)
//!	@param	[in]	name	変数名	
//!	@param	[in]	data	データ
//-----------------------------------------------------------------------------
void SystemShader::setVertex3fv(const char* name, GLfloat* data)
{
	if(_currentProgram == SHADER_NONE) return;

	// ハンドル取得
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	// バーテックス変数転送
	glVertexAttrib3fv(handle, data);
}


//-----------------------------------------------------------------------------
//! バーテックス変数を設定する(4fv)
//!	@param	[in]	name	変数名	
//!	@param	[in]	data	データ
//-----------------------------------------------------------------------------
void SystemShader::setVertex4fv(const GLchar* name, GLfloat* data)
{
	if(_currentProgram == SHADER_NONE) return;

	// ハンドル取得
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	// バーテックス変数転送
	glVertexAttrib4fv(handle, data);
}



//-----------------------------------------------------------------------------
//! バーテックス変数を設定し有効にする
//!	@param	[in]	name	変数名
//!	@param	[in]	size	データひとつ分の個数
//!	@param	[in]	size	データタイプ
//!	@param	[in]	size	データひとつごとのスライド量
//!	@param	[in]	offset	データの開始位置
//-----------------------------------------------------------------------------
void SystemShader::setAndEnableVertexAttribute(const GLchar* name, GLint size, GLenum type, GLsizei stride, GLvoid* offset)
{
	if(_currentProgram == SHADER_NONE) return;
	// ハンドル取得
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
//	getGLError();
	// データ転送と有効化
	setAndEnableVertexAttributeWithHandle(handle, size, type, stride, offset);
//	getGLError();
}
//-----------------------------------------------------------------------------
//! バーテックス変数を設定し有効にする(ハンドルで)
//!	@param	[in]	handle	変数のハンドル
//!	@param	[in]	size	データひとつ分の個数
//!	@param	[in]	size	データタイプ
//!	@param	[in]	size	データひとつごとのスライド量
//!	@param	[in]	offset	データの開始位置
//-----------------------------------------------------------------------------
void SystemShader::setAndEnableVertexAttributeWithHandle(GLint handle, GLint size, GLenum type, GLsizei stride, GLvoid* offset)
{
	if( handle == -1 ) return;
	if(_currentProgram == SHADER_NONE) return;
	// バーテックス配列を転送
	glVertexAttribPointer(handle, size, type, GL_FALSE, stride, offset);
	// バーテックス配列を有効化
	glEnableVertexAttribArray(handle);

//	getGLError();
}

//-----------------------------------------------------------------------------
//! 設定を無効にする
//!	@param	[in]	name	変数名
//-----------------------------------------------------------------------------
void SystemShader::DisableVertexAttribute(const GLchar* name)
{
	if(_currentProgram == SHADER_NONE) return;

	// ハンドル取得
	GLint handle = glGetAttribLocation(_program[_currentProgram].getShaderProgram(), name);
	if( handle == -1 ) return;
	// 設定したバーテックス配列の無効化
	DisableVertexAttributeWithHandle(handle);
}

//-----------------------------------------------------------------------------
//! 設定を無効にする(ハンドルで)
//!	@param	[in]	name	変数名
//-----------------------------------------------------------------------------
void SystemShader::DisableVertexAttributeWithHandle(GLint handle)
{
	if( handle == -1 ) return;
	if(_currentProgram == SHADER_NONE) return;
	// 設定したバーテックス配列の無効化
	glDisableVertexAttribArray(handle);
}




//=============================================================================
// END OF FILE
//=============================================================================