//-----------------------------------------------------------------------------
//!
//!	@file	gmSystem.cpp
//!	@brief	汎用的関数群
//!	@author	YukiIshigaki
//!
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
//! ラジアンに変換
//-----------------------------------------------------------------------------
f32		TO_RADIAN(f32	deg);

//-----------------------------------------------------------------------------
//!	デグリーに変換
//-----------------------------------------------------------------------------
f32		TO_DEGREE(f32	rad);

////! 2D描画用
//void view2D();
//
////! 3D描画用
//void view3D();

//-----------------------------------------------------------------------------
//! 線形補間
//! P' = (E - S) * f / maxf + S
//! @param	[in] start		最小値(始点) 上記のSに当たる
//! @param	[in] end		最大値(終点) 上記のEに当たる
//! @param	[in] frame		現在の値	 上記のfに当たる
//!	@param	[in] maxFrame	値の最大値	 上記のmaxfに当たる
//-----------------------------------------------------------------------------
f32 LinearInterpolation( f32 start , f32 end , f32 frame, f32 maxFrame);

//! FPS取得
float getGLFps();

//! 文字列表示
void render_string(float x, float y, std::string const& str);


//-----------------------------------------------------------------------------
//!	ブレンドモード (Blend Mode)
//-----------------------------------------------------------------------------
enum BLEND_MODE {
	BM_NONE,			//!< 合成なし(OFF)
	BM_BLEND_NOALPHA,	//!< 通常ブレンド(アルファなし)
	BM_BLEND_ALPHA,		//!< 通常ブレンド(アルファあり)
	BM_INVERSE,			//!< 反転
	BM_ADD_NOALPHA,		//!< 加算ブレンド(アルファなし)
	BM_ADD_ALPHA,		//!< 加算ブレンド(アルファあり)
	BM_SUB_NOALPHA,		//!< 減算(アルファなし)
	BM_SUB_ALPHA,		//!< 減算(アルファあり)
	BM_MULTI_NOALPHA,	//!< 乗算(アルファなし)
	BM_MULTI_ALPHA,		//!< 乗算(アルファあり)
	BM_SCREEN_NOALPHA,	//!< スクリーン(アルファなし)
	BM_DIV_NOALPHA,		//!< 除算(アルファなし)
	BM_INVISIVLE,		//!< 透明
	BM_DARKNESS,		//!< ダークネス
	BM_TEST,			//!< テスト用


};

//-----------------------------------------------------------------------------
//!	ブレンドモードを設定
//!	@param	[in]	mode	ブレンドモード
//-----------------------------------------------------------------------------
void setBlendMode(BLEND_MODE mode);

//-----------------------------------------------------------------------------
//! 点の描画
//!	@param	[in]	pos		点の座標
//-----------------------------------------------------------------------------
void DebugPointRender(Vector3& pos);


class Texture;

//! バックバッファ描画
//!	@param	[in]	backBuffer	バックバッファテクスチャ
void drawBackBuffer(Texture* backBuffer);

//! シャドウテクスチャデバッグ描画
//!	@param	[in]	textureID	テクスチャのハンドル
//!	@param	[in]	x			描画のX座標
//!	@param	[in]	y			描画のY座標
void debugShadowTexture(GLuint textureID, s32 x, s32 y);

//! 四角形の描画
void drawRect(f32 x, f32 y, f32 w, f32 h, const Color& color);

//-----------------------------------------------------------------------------
//! 名前と一致するかどうか
//!	マクロ意味 #x : xを文字列にする	is##x : isとxをつなげる
//-----------------------------------------------------------------------------
#define DEF_COL_NAME(x)								\
	static char* myName = #x;						\
													\
bool	is##x(Collision* obj)						\
{													\
	if( strcmp(obj->getName(), myName ) == 0 ){		\
		return true;								\
	}												\
	return false;									\
}				

#define DEF_COL_NAME_HEADER(x)						\
bool	is##x(Collision* obj);

//-----------------------------------------------------------------------------
//! 名前と一致するかどうか(攻撃用)
//!	マクロ意味 #x : xを文字列にする	is##x : isとxをつなげる
//-----------------------------------------------------------------------------
#define DEF_COL_ATTACK_NAME(x)							\
	static char* myAttackName = #x;						\
														\
bool	is##x(Collision* obj)							\
{														\
	if( strcmp(obj->getName(), myAttackName ) == 0 ){	\
		return true;									\
	}													\
	return false;										\
}				

#define DEF_COL_ATTACK_NAME_HEADER(x)					\
bool	is##x(Collision* obj);



//-----------------------------------------------------------------------------
//!	アサートメッセージ付き
//-----------------------------------------------------------------------------
#define ASSERTMSG(IF_, MSG_)	assert( IF_ && MSG_ )


//-----------------------------------------------------------------------------
// GLのエラーを検出
//-----------------------------------------------------------------------------
bool getGLError();


namespace math{

	//! 円周率π
	static const f32	PI = 3.14159265f;

	template <typename T>
	T	getSameValofFour(T search, T a, T b, T c, T d)
	{
		if( search == abs(a) ){
			return a;
		}else if( search == abs(b) ){
			return b;
		}else if( search == abs(c) ){
			return c;
		}else{
			return d;
		}
	}


}	// namespace math



namespace common {

	struct FloatComp {
		union {
			f32 f;	// フロートの値
			u32	u;	// ビットでの値 
		};
	};

	//-----------------------------------------------------------------------------
	//! int -> float変換
	//!	@param	[in]	val		変換元
	//-----------------------------------------------------------------------------
	f32	IntToFloat(u32 val);

	//! Byte -> float変換
	//!	@param	[in]	val		変換元
	f32			ByteToFloat(u8 val);

	//! 32bitfloat -> 16bitfloat変換
	//!	@param	[in]	val				変換元
	// 参照URL : http://www.platinumgames.co.jp/programmer_blog/?p=484
	u16			Float32to16(f32 val);

}	// namespace common
