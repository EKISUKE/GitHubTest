//-----------------------------------------------------------------------------
//!
//!	@file	gmSystem.h
//!	@brief	汎用的関数群
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include"stdafx.h"

//-----------------------------------------------------------------------------
//! ラジアンに変換
//-----------------------------------------------------------------------------
f32		TO_RADIAN(f32	deg)
{
	return deg * math::PI / 180.0f;
}

//-----------------------------------------------------------------------------
//!	デグリーに変換
//-----------------------------------------------------------------------------
f32		TO_DEGREE(f32	rad)
{
	return rad * 180.0f / math::PI;
}


//-----------------------------------------------------------------------------
//! 線形補間(frameの値はSからEの間の値に収まる)
//! P' = (E - S) * f / maxf + S
//! @param	[in] start		最小値(始点) 上記のSに当たる
//! @param	[in] end		最大値(終点) 上記のEに当たる
//! @param	[in] frame		現在の値	 上記のfに当たる
//!	@param	[in] maxFrame	値の最大値	 上記のmaxfに当たる
//-----------------------------------------------------------------------------
f32 LinearInterpolation( f32 start , f32 end , f32 frame, f32 maxFrame){
    return ((long)(end - start) * frame / maxFrame + start);
}



//---------------------------------------------------------------------------
//!	ブレンドモードを設定
//!	@param	[in]	mode	ブレンドモード
//---------------------------------------------------------------------------
void setBlendMode(BLEND_MODE mode)
{
	//-------------------------------------------------------------
	// 半透明ブレンド ON/OFF
	//-------------------------------------------------------------
	if( mode == BM_NONE ) {		//---- OFF
		glDisable(GL_BLEND);
	}
	else {						//---- ON
		glEnable(GL_BLEND);
	}

	//----------------------------------------------------------------
	// ブレンドモードの合成比率の設定
	//----------------------------------------------------------------
	switch( mode ) 
	{
		case BM_NONE:
			break;
		case BM_BLEND_NOALPHA:
		{
			glBlendFunc(GL_ONE, GL_ZERO);
			break;
		}
		case BM_BLEND_ALPHA:
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
		case BM_INVERSE:
		{
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
			break;
		}
		case BM_ADD_NOALPHA:
		{
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		}
		case BM_ADD_ALPHA:
		{
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}
		case BM_SUB_NOALPHA:
		{
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ZERO);
			glBlendFunc(GL_ONE, GL_ONE);
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ZERO);
			break;
		}
		case BM_SUB_ALPHA:
		{
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ZERO);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ZERO);
			break;
		}
		case BM_MULTI_NOALPHA:
		{
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		}
		case BM_MULTI_ALPHA:
		{
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
			break;
		}
		case BM_SCREEN_NOALPHA:
		{
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
			break;
		}
		case BM_DIV_NOALPHA:
		{
			glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_COLOR);
			break;
		}
		case BM_INVISIVLE:
		{
			glBlendFunc(GL_DST_COLOR, GL_ONE);
			break;
		}
		case BM_DARKNESS:
		{
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);

			break;
		}

			
	}
}

//-----------------------------------------------------------------------------
//! テクスチャを2D表示
//!	@param	[in]	pTexture	テクスチャのポインタ
//!	@param	[in]	w			描画の幅
//!	@param	[in]	h			描画の高さ
//!	@param	[in]	x			描画のX座標
//!	@param	[in]	y			描画のY座標
//-----------------------------------------------------------------------------
void drawTexture2D(Texture* pTexture, f32 w, f32 h, f32 x, f32 y)
{
	Texture::set(pTexture);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x,y);

		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x+w,y);

		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x,y+h);

		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x+w,y+h);

	glEnd();

	Texture::set(NULL);
}

//-----------------------------------------------------------------------------
//! バックバッファ描画
//!	@param	[in]	backBuffer	バックバッファテクスチャ
//-----------------------------------------------------------------------------
void drawBackBuffer(Texture* backBuffer)
{

	//-------------------------------------------------------------
	// 幅と高さを自動で取得
	//-------------------------------------------------------------
	//---- テクスチャの幅と高さ
	Size<s32> texSize = backBuffer->getSize();

	glPushAttrib(GL_VIEWPORT_BIT);
	// 書き込みサイズをテクスチャの範囲に設定
	glViewport(0, 0, texSize._w, texSize._h);
	//-------------------------------------------------------------
	// シェーダ定数(Shader Constant)の設定
	//-------------------------------------------------------------
	//---- シェーダの変更
	// シェーダを変更
	GmShader()->changeShader(SystemShader::SHADER_PUT_TEXTURE);
	
	//---- テクスチャ
	GmShader()->bindTexture("BaseTexture", backBuffer->getColorBuffer(), 0);

	//-------------------------------------------------------------
	// 描画 (Draw)
	//-------------------------------------------------------------
	static GLuint VB;
	static bool	Initialized = false;
	if( !Initialized ) {
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, nullptr, GL_DYNAMIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(FilterVertex) * 4, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Initialized = true;
	}

	const FilterVertex v[4] = {
		{ Float3(-1.0f, -1.0f, 0.0f), Color(255,255,255,255), Color::NONE, Float2(0.0f, 0.0f) },
		{ Float3(+1.0f, -1.0f, 0.0f), Color(255,255,255,255), Color::NONE, Float2(1.0f, 0.0f) },
		{ Float3(-1.0f, +1.0f, 0.0f), Color(255,255,255,255), Color::NONE, Float2(0.0f, 1.0f) },
		{ Float3(+1.0f, +1.0f, 0.0f), Color(255,255,255,255), Color::NONE, Float2(1.0f, 1.0f) },
	};

	/*const Vertex	v[4] = {
		{ Float3(-1.0f, -1.0f, 0.0f), Color(255,255,255,255), Float2(0.0f, 0.0f) },
		{ Float3(+1.0f, -1.0f, 0.0f), Color(255,255,255,255), Float2(1.0f, 0.0f) },
		{ Float3(-1.0f, +1.0f, 0.0f), Color(255,255,255,255), Float2(0.0f, 1.0f) },
		{ Float3(+1.0f, +1.0f, 0.0f), Color(255,255,255,255), Float2(1.0f, 1.0f) }
	};*/

	//---- 描画
	//glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	// 頂点バッファバインド
	glBindBuffer(GL_ARRAY_BUFFER, VB);
// データ転送
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FilterVertex) * 4, v);


	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _position));
	GmShader()->setAndEnableVertexAttribute("uv"			, 2, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _uv));
	GmShader()->setAndEnableVertexAttribute("color"			, 4, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _diffuseColor));
	GmShader()->setAndEnableVertexAttribute("emissiveColor"	, 4, GL_FLOAT, sizeof(FilterVertex), (GLvoid*)offsetof(FilterVertex, _emissiveColor));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("uv");
	GmShader()->DisableVertexAttribute("color");
	GmShader()->DisableVertexAttribute("emissiveColor");

	GmShader()->unbindTexture();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 元に戻す
	//glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GmShader()->beginPrevShader();
	glPopAttrib();
}


//-----------------------------------------------------------------------------
//! シャドウテクスチャデバッグ描画
//!	@param	[in]	textureID	テクスチャのハンドル
//!	@param	[in]	x			描画のX座標
//!	@param	[in]	y			描画のY座標
//-----------------------------------------------------------------------------
void debugShadowTexture(GLuint textureID, s32 x, s32 y)
{
	glPushAttrib(GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);


	int shadH  = GmShader()->getCurrentShaderPorgram();
	int handle = glGetUniformLocation(shadH, "quadTexture");
	glUniform1i(handle, 0);
	glViewport(x, y, 300, 300);

	GLfloat vertex_data[] =
	{
		-1,-1, 0,
		 1,-1, 0,
		 1, 1, 0,
		-1, 1, 0,
	};

	/*glBegin(GL_QUADS);
	glVertex3f(-1,-1, 0);
	glVertex3f( 1,-1, 0);
	glVertex3f( 1, 1, 0);
	glVertex3f(-1, 1, 0);
	glEnd();*/

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertex_data);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_DEPTH_TEST);
//	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glPopAttrib();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);
}

//-----------------------------------------------------------------------------
//! 四角形の描画
//-----------------------------------------------------------------------------
void drawRect(f32 x, f32 y, f32 w, f32 h, const Color& color)
{
	Size<s32> screenSize  = GmWindow()->getResolution();

	f32 x1 = (x + w) * ( 2.0f / screenSize._w) - 1.0f;
	f32 y1 = (y	+ h) * (-2.0f / screenSize._h) + 1.0f;

	f32 x2 = (x    ) * ( 2.0f / screenSize._w) - 1.0f;
	f32 y2 = (y    ) * (-2.0f / screenSize._h) + 1.0f;

	static GLuint VB;
	static bool Initialized = false;
	if( !Initialized ) {
		glGenBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, nullptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		Initialized = true;
	}

	// 頂点の作成
	const Vertex v[4] = {
		{ Float3( x1,  y1, 0.0f),  color },
		{ Float3( x2,  y1, 0.0f),  color },
		{ Float3( x1,  y2, 0.0f),  color },
		{ Float3( x2,  y2, 0.0f),  color },
	};

	//! カラーシェーダーに変更
	GmShader()->changeShader(SystemShader::SHADER_RENDER_2D);

	//---- 描画
	setBlendMode(BM_BLEND_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);

	// 頂点バッファバインド
	glBindBuffer(GL_ARRAY_BUFFER, VB);
	// データ転送
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * 4, v);

	GmShader()->setAndEnableVertexAttribute("position"		, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _position));
	GmShader()->setAndEnableVertexAttribute("color"			, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, _color));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	GmShader()->DisableVertexAttribute("position");
	GmShader()->DisableVertexAttribute("color");

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 元に戻す
	setBlendMode(BM_NONE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_DEPTH_TEST);
	GmShader()->beginPrevShader();
	
}


//-----------------------------------------------------------------------------
//! FPS取得
//-----------------------------------------------------------------------------
float	getGLFps()
{
	f32 resultFps = 0;
	static s32 GLframe;
    static u32 GLtimebase;
    u32		   GLtimenow;
 
    GLframe++;
    GLtimenow = glutGet(GLUT_ELAPSED_TIME);
 
    if (GLtimenow - GLtimebase > 1000) {
        resultFps = GLframe * 1000.0f / (GLtimenow - GLtimebase);
        GLtimebase = GLtimenow;
        GLframe = 0;
    }
	return resultFps;
}

//-----------------------------------------------------------------------------
//! 文字列表示
//-----------------------------------------------------------------------------
void render_string(float x, float y, std::string const& str)
{
    float z = 0.0f;
    glRasterPos3f(x, y, z);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned char*)str.c_str());
}


//-----------------------------------------------------------------------------
//! 点の描画
//-----------------------------------------------------------------------------
void DebugPointRender(Vector3& pos)
{
	/* 点にアンチエリアシング処理を行う */
	glEnable(GL_POINT_SMOOTH);

	/* アンチエリアシング処理の品質を最高にする */
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	
	/* アルファブレンディングを有効にする */
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	setBlendMode(BM_ADD_ALPHA);

	glPointSize(10);
	glPushMatrix();
		glBegin(GL_POINTS);
		glVertex3fv((GLfloat*)&pos);
		glEnd();
	glPopMatrix();

	//glDisable(GL_BLEND);
	setBlendMode(BM_NONE);
	glDisable(GL_POINT_SMOOTH);
}


//-----------------------------------------------------------------------------
// GLのエラーを検出
//-----------------------------------------------------------------------------
bool getGLError()
{

#if 1
	GLenum error = glGetError();

	switch( error )
	{
		case GL_NO_ERROR:			// 0
			return true;
			break;
		case GL_INVALID_ENUM:		// 1280 
			MessageBoxA(NULL, "有効なenumではありません(GL_INVALID_ENUM)", "GLエラー", MB_OK);
			break;
		case GL_INVALID_VALUE:		// 1281
			MessageBoxA(NULL, "有効な値ではありません(GL_INVALID_VALUE)", "GLエラー", MB_OK);
			break;
		case GL_INVALID_OPERATION:	// 1282
			MessageBoxA(NULL, "有効なオペレーションではありません(GL_INVALID_OPERATION)", "GLエラー", MB_OK);
			break;
		case GL_STACK_OVERFLOW:		// 1283
			MessageBoxA(NULL, "スタック最大を超えました(GL_STACK_OVERFLOW)", "GLエラー", MB_OK);
			break;
		case GL_STACK_UNDERFLOW:	// 1284
			MessageBoxA(NULL, "スタック最小を下回りました(GL_STACK_UNDERFLOW)", "GLエラー", MB_OK);
			break;
		case GL_OUT_OF_MEMORY:		// 1285
			MessageBoxA(NULL, "メモリ外アクセスをしました(GL_OUT_OF_MEMORY)", "GLエラー", MB_OK);
			break;
		default:
			MessageBoxA(NULL, "UNKNOWN", "GLエラー", MB_OK);
			break;
	}

	assert(false);
#endif
	return false;
}


namespace common {

//-----------------------------------------------------------------------------
//! int -> float変換
//!	@param	[in]	val		変換元
//-----------------------------------------------------------------------------
f32	IntToFloat(u32 val)
{
	return  (f32)val * (1.0f / 255.0f);
}

//-----------------------------------------------------------------------------
//! Byte -> float変換
//!	@param	[in]	val		変換元
//-----------------------------------------------------------------------------
f32	ByteToFloat(u8 val)
{
	return  (f32)val * (1.0f / 255.0f);
}

//-----------------------------------------------------------------------------
//! 32bitfloat -> 16bitfloat変換
//!	@param	[in]	val		変換元
// 参照URL : http://www.platinumgames.co.jp/programmer_blog/?p=484
//-----------------------------------------------------------------------------
u16 Float32to16(f32 val)
{
	
	FloatComp comp;
	// floatを代入して
	comp.f = val;

	//-------------------------------------------------------------
	// 各部を分離 + 仮数部を圧縮
	//-------------------------------------------------------------
	// 符号部
	s32	sign		= (comp.u >> 16) & 0x8000;	// 16ビット右シフトして最上位bitを取得する
	// 指数部
	s32	exponent	= (comp.u >> 23) & 0x00ff;	// 23bit右シフトして下位8bitを取得する
	// 仮数部(圧縮も行う)
	s32	significand	= (comp.u >> 13) & 0x03ff;	// 13bit右シフトして下位13bitを削り、下位10bitを取得する


	//-------------------------------------------------------------
	// 指数部の決定
	//-------------------------------------------------------------
	// float の計算
	// 2.5 = (-1)^符号部 * 2^(指数部-バイアス(127)) * (1 + 仮数部(0.25));
	exponent = exponent - 127;	// 今は32bitの状態なので-127して2の何乗なのか求める
	exponent = exponent + 15;	// 15(バイアス値)を足して16bitでの指数部にする

	//-------------------------------------------------------------
	// 合成
	//-------------------------------------------------------------
	// 指数部は10bitシフトして上位5bit取得それ以外は普通に合成
	return sign | ( (exponent << 10) & 0x7c00 ) | significand;
}



}	//	namespace common