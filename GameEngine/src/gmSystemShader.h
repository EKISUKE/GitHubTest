//-----------------------------------------------------------------------------
//!
//!	@file	gmSystemShader.h
//!	@brief	シェーダ管理システム
//!	@author YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

// シェーダファイルまでのパス
#define SHADER_PATH	"shaderFile/"

//-----------------------------------------------------------------------------
//!	シェーダ管理
//-----------------------------------------------------------------------------
class SystemShader
{
public:
	//! シェーダの種類
	enum SHADER_MODE
	{
		SHADER_STANDARD,
		SHADER_COLOR,
		SHADER_NORMAL,
		SHADER_TEXTURE,
		SHADER_SHADOW,
		SHADER_DEBUG,
		SHADER_DRAWCUBEMAP,
		SHADER_FILTER,
		SHADER_GAUSSIAN_3X3_H,
		SHADER_GAUSSIAN_3X3_V,
		SHADER_SUBTRACT,
		SHADER_BLOOM_GLARE,
		SHADER_GET_LUMINANCE,
		SHADER_LUMINANCE_REDUCTION,
		SHADER_LIGHT_ADAPTATION,
		SHADER_TONEMAPPING_FINAL,
		SHADER_GLARE,
		SHADER_MOTION_BLUR,
		SHADER_PUT_TEXTURE,
		SHADER_RENDER_2D,
		SHADER_LENS_FLARE,
		SHADER_LENS_DIRT_STAR,
		SHADER_ANAMOLPHIC_FLARE,
		SHADER_EXPAND_BLUR,

		SHADER_MAX,
		SHADER_NONE,
	};


	//-------------------------------------------------------------
	//! @name 初期化
	//-------------------------------------------------------------
	//@{
private:

	//! コンストラクタ
	SystemShader();
	//! コピーコンストラクタ
	SystemShader(SystemShader& src){}

	//! デストラクタ
	virtual ~SystemShader();
public:
	//! 初期化
	bool		init();

	//! 解放
	void		cleanup();

	//! シェーダプログラムの初期化
	//!	@param	[in]	mode	シェーダのモード
	//!	@param	[in]	vertex	バーテックスシェーダ
	//!	@param	[in]	vertex	フラグメントシェーダ
	bool		InitShader(SHADER_MODE mode, VertexShader& vertex, FragmentShader& frag);
	//! UniformBlockの初期化
	bool		InitUniformBlock();
	//! コンテキストの初期化
	bool		InitContext();

	//@}
	//-------------------------------------------------------------
	//! @name 開始・終了・変更
	//-------------------------------------------------------------
	//@{

	//! シェーダの開始
	void		beginShader();
	//! ひとつ前のシェーダの開始
	void		beginPrevShader();
	//! シェーダの終了
	void		endShader();

	//! シェーダプログラム変更
	//!	@param	[in]	mode	シェーダモード
	void		changeShader(SHADER_MODE mode);

	//@}
	//-------------------------------------------------------------
	//! @name 取得・設定
	//-------------------------------------------------------------
	//@{

	//! インスタンスの取得
	static SystemShader* getInstance() {
		static SystemShader	_myInstance;
		return &_myInstance;
	}

	//! シェーダプログラムの取得
	//!	@param	[in]	mode	シェーダモード
	GLuint				getShaderProgram( SHADER_MODE mode ) { return _program[mode].getShaderProgram(); }
	//! 現在のシェーダプログラム取得
	GLuint				getCurrentShaderPorgram() const { return _program[_currentProgram].getShaderProgram(); }
	//! ひとつ前のシェーダプログラム取得
	GLuint				getPrevShaderProgram()    const { return _program[_prevProgram].getShaderProgram(); }
	//! マトリックスのバインドポイントを取得
	GLuint				getMatrixBindingPoint()   const { return _matrixBindingPoint; }
	//! 通常描画のコンテキスト取得
	NormalContext		getNormalContext()	      const { return _normalContext; }
	//! シャドウのコンテキスト取得
	ShadowContext		getShadowContext()		  const { return _shadowContext; }
	//! フィルターのコンテキスト取得
	FilterContext		getFilterContext()		  const { return _filterContext; }
	//! ヌルテクスチャのハンドル取得(ディフューズ)
	GLuint				getNullTexHandle()		  const { return _nullTexture->getTextureHandle(); }
	//! ヌルテクスチャのハンドル取得(スペキュラ)
	GLuint				getNullSpecHandle()		  const { return _nullSpecular->getTextureHandle(); }
	//! ヌルテクスチャのハンドル取得(ノーマル)
	GLuint				getNullNormHandle()		  const { return _nullNormal->getTextureHandle(); }

	//! シェーダの種類設定
	//!	@param	[in]	mode	シェーダモード
	void				setPass(SHADER_MODE mode);
	

	//---- Uniform系設定

	//! マトリックスデータを転送
	//!	@param	[in]	data		行列データ
	//!	@param	[in]	dataSize	データサイズ
	void				setUniformMatrixBuffer(Matrix* data, GLsizei dataSize);
	
	//! Floatのユニフォーム変数を設定
	//!	@param	[in]	name	変数名	
	//!	@param	[in]	data	データ
	void				setUniform1f(const GLchar* name, GLfloat data);

	//! Floatのユニフォーム変数を設定
	//!	@param	[in]	name	変数名	
	//!	@param	[in]	data	データ
	void				setUniform1fv(const GLchar* name, GLfloat* data, GLsizei count = 1);

	//! Float2のユニフォーム変数を設定
	//!	@param	[in]	name	変数名	
	//!	@param	[in]	data	データ
	//!	@param	[in]	count	データの個数
	void				setUniform2fv(const GLchar* name, GLfloat* data, GLsizei count = 1);

	//! Float3のユニフォーム変数を設定
	//!	@param	[in]	name	変数名	
	//!	@param	[in]	data	データ
	//!	@param	[in]	count	データの個数
	void				setUniform3fv(const GLchar* name, GLfloat* data, GLsizei count = 1);

	//! Float4のユニフォーム変数を設定
	//!	@param	[in]	name	変数名	
	//!	@param	[in]	data	データ
	//!	@param	[in]	count	データの個数
	void				setUniform4fv(const GLchar* name, GLfloat* data, GLsizei count = 1);

	//! intのユニフォーム変数を設定
	//!	@param	[in]	name	変数名	
	//!	@param	[in]	data	データ
	void				setUniform1i(const GLchar* name, GLint data);


	//! 行列を設定
	//!	@param	[in]	name		変数名	
	//!	@param	[in]	dataCount	行列の数
	//!	@param	[in]	transpose	転置するかどうか
	//!	@param	[in]	data		データ
	void				setMatrix4x4(const GLchar* name, GLuint dataCount, GLboolean transpose, GLfloat* data);
	
	//! テクスチャ設定
	//!	@param	[in]	name		変数名	
	//!	@param	[in]	texHandle	テクスチャのハンドル
	//!	@param	[in]	texNum		有効化するテクスチャの番号(GL_TEXTURE0←なら0)
	void				bindTexture(const GLchar* name, GLuint texHandle, GLuint texNum, GLenum target = GL_TEXTURE_2D);


	//! テクスチャ設定解除
	//!	@param	[in]	target		アンバインドするターゲット
	void				unbindTexture(GLenum target = GL_TEXTURE_2D);

	//---- in(attrib)系設定


	//! バーテックス変数を設定する(1fv)
	//!	@param	[in]	name	変数名	
	//!	@param	[in]	data	データ
	void				setVertex1fv(const GLchar* name, GLfloat* data);

	//! バーテックス変数を設定する(2fv)
	//!	@param	[in]	name	変数名	
	//!	@param	[in]	data	データ
	void				setVertex2fv(const GLchar* name, GLfloat* data);

	//! バーテックス変数を設定する(4fv)
	//!	@param	[in]	name	変数名	
	//!	@param	[in]	data	データ
	void				setVertex4fv(const GLchar* name, GLfloat* data);

	//! バーテックス変数を設定する(3fv)
	//!	@param	[in]	name	変数名	
	//!	@param	[in]	data	データ
	void				setVertex3fv(const GLchar* name, GLfloat* data);
	
	//! バーテックス変数を設定し有効にする
	//!	@param	[in]	name	変数名
	//!	@param	[in]	size	データひとつ分の個数
	//!	@param	[in]	size	データタイプ
	//!	@param	[in]	size	データひとつごとのスライド量
	//!	@param	[in]	offset	データの開始位置
	void				setAndEnableVertexAttribute(const GLchar* name, GLint size, GLenum type, GLsizei stride, GLvoid* offset);

	//! バーテックス変数を設定し有効にする(ハンドルで)
	//!	@param	[in]	handle	変数のハンドル
	//!	@param	[in]	size	データひとつ分の個数
	//!	@param	[in]	size	データタイプ
	//!	@param	[in]	size	データひとつごとのスライド量
	//!	@param	[in]	offset	データの開始位置
	void				setAndEnableVertexAttributeWithHandle(GLint handle, GLint size, GLenum type, GLsizei stride, GLvoid* offset);
	
	//! 設定を無効にする
	//!	@param	[in]	name	変数名
	void				DisableVertexAttribute(const GLchar* name);
	
	//! 設定を無効にする(ハンドルで)
	//!	@param	[in]	name	変数名
	void				DisableVertexAttributeWithHandle(GLint handle);
	
	

	//@}
private:

	static ProgramShader	_program[SHADER_MAX];
	SHADER_MODE				_currentProgram;					//!< 現在使用中のプログラム
	SHADER_MODE				_prevProgram;						//!< ひとつ前のシェーダプログラム
	GLuint					_matrixBindingPoint;				//!< 行列のバイトポイント
	GLuint					_uniformBuffer;						//!< バッファデータ
	bool					_changeShader;						//!< シェーダチェンジしてるかどうか

	NormalContext			_normalContext;						//!< 通常描画用コンテキスト
	ShadowContext			_shadowContext;						//!< シャドウ用コンテキスト
	FilterContext			_filterContext;						//!< フィルター用コンテキスト
	Texture*				_nullTexture;						//!< ヌルテクスチャ(ディフューズ)
	Texture*				_nullSpecular;						//!< ヌルテクスチャ(スペキュラ)
	Texture*				_nullNormal;						//!< ヌルテクスチャ(ノーマル)
};

//-----------------------------------------------------------------------------
// アクセスインターフェイス
//-----------------------------------------------------------------------------
__forceinline
SystemShader*	GmShader()
{
	return SystemShader::getInstance();
}


//=============================================================================
// END OF FILE
//=============================================================================