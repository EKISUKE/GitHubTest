//-----------------------------------------------------------------------------
//!
//!	@file	GLFont.cpp
//!	@brief	文字表示用
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#include "Library.h"


//-----------------------------------------------------------------------------
//! コンストラクタ
//-----------------------------------------------------------------------------
GLFont::GLFont(wchar_t *fontname, s32 size)
{
	Initialize(fontname, size);
}

//-----------------------------------------------------------------------------
//! デストラクタ
//-----------------------------------------------------------------------------
GLFont::~GLFont(){
	glDeleteLists(_list, 255);
	_list   = 0;
}

//-----------------------------------------------------------------------------
//! 初期化
//-----------------------------------------------------------------------------
bool GLFont::Initialize(wchar_t* fontname, s32 size)
{
	 Hfont = CreateFontW(
						 size,						// フォント高さ
						 0,							// 文字幅
						 0,							// テキストの角度
						 0,							// ベースラインとｘ軸との角度
						 FW_REGULAR,				// フォントの太さ
						 FALSE,						// イタリック体
						 FALSE,						// アンダーライン
						 FALSE,						// 打ち消し線
						 SHIFTJIS_CHARSET,			// 文字セット
						 OUT_DEFAULT_PRECIS,		// 出力精度
						 CLIP_DEFAULT_PRECIS,		// クリッピング精度
						 ANTIALIASED_QUALITY,		// 出力品質
						 FIXED_PITCH | FF_MODERN,	// ピッチとファミリー
						 fontname);					// 書体名

	 Hdc = wglGetCurrentDC();
	 SelectObject(Hdc, Hfont);
	 return true;
}

//-----------------------------------------------------------------------------
//! ワイド文字列の描画
//-----------------------------------------------------------------------------
void  GLFont::DrawStringW(s32 x, s32 y, s32 z, wchar_t* format, ...)
{
	SelectObject(Hdc, Hfont);
	wchar_t buf[256];
	va_list ap;
	int	Length = 0;
	_list = 0;

	// ポインタがNULLの場合は終了
	if( format == NULL )
	{
		return;
	}

	// 文字列変換
	va_start(ap, format);
	vswprintf_s(buf, format, ap);
	va_end(ap);
	
	Length = wcslen(buf);
	_list   = glGenLists(Length);


	

	for( s32 i=0; i<Length; i++ )
	{
		// 文字のビットマップをディスプレイリストとして作成
		wglUseFontBitmapsW(Hdc, buf[i], 1, _list + (DWORD)i );
		/*glNewList(_list + i, GL_COMPILE);
		
		glEndList();*/
	}

	
	glDisable(GL_LIGHTING);		// 文字色を出すために光源を無効化
	// 描画位置の設定
	glRasterPos3i(x, y, z);
	// ディスプレイリストで描画
	for( s32 i=0; i<Length; i++ )
	{
		glCallList( _list+i);
	}
	glEnable(GL_LIGHTING);		// 光源を有効化
	// ディスプレイリスト破棄
	glDeleteLists(_list, Length);
	_list   = 0;
	//Length  = 0;
}

//=============================================================================
//	END OF FILE
//=============================================================================