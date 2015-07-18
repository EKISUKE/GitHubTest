// ToneMappingFinal.frag
// トーンマッピング最終
#version 330

// 吐き出しの色
layout(location = 0) out vec4 fragColor;
// テクスチャ
uniform sampler2D BaseTexture;
// テクスチャ
uniform sampler2D BaseTexture2;
// ミップマップ番号
uniform float	  gMipLod;

// バーテックスから受け取る値
in Data{
	vec4	diffuseColor;
	vec2	uv;
}shaderInput;


// 参照
// http://www.cg.tuwien.ac.at/~ginko/Hable_John_Uncharted2_HDRLighting.pdf
// Note: E/F = Toe Angle


//vec3 A = vec3( 0.15 );	// Shoulder Strength
//vec3 B = vec3( 0.50 );	// Linear Strength
//vec3 C = vec3( 0.10 );	// Lenear Angle
//vec3 D = vec3( 0.20 );	// Toe Strength
//vec3 E = vec3( 0.02 );	// Toe Numberator
//vec3 F = vec3( 0.30 );	// Toe Denominator
//vec3 W = vec3( 11.2 );	// Linear White

vec3 A = vec3( 0.22 );	// Shoulder Strength
vec3 B = vec3( 0.30 );	// Linear Strength
vec3 C = vec3( 0.10 );	// Lenear Angle
vec3 D = vec3( 0.20 );	// Toe Strength
vec3 E = vec3( 0.01 );	// Toe Numberator
vec3 F = vec3( 0.30 );	// Toe Denominator
vec3 W = vec3( 11.2 );	// Lenear White



//void main()
//{
//	fragColor = textureLod(BaseTexture, shaderInput.uv, gMipLod);
//
//	//---- RGB→YCbCrに変換する行列
//	const vec3 RGBtoY  = vec3( +0.29900, +0.58700, +0.11400 );
//	const vec3 RGBtoCb = vec3( -0.16874, -0.33126, +0.50000 );
//	const vec3 RGBtoCr = vec3( +0.50000, -0.41869, -0.08131 );
//
//	//---- YCbCr→RGBに変換する行列
//	const vec3 YCbCrtoR = vec3( +1.00000, +0.00000, +1.40200 );
//	const vec3 YCbCrtoG = vec3( +1.00000, -0.34414, -0.71414 );
//	const vec3 YCbCrtoB = vec3( +1.00000, +1.77200, +0.00000 );
//	
//	// 1x1サイズまで縮小した情報テクスチャ
//	vec4 info = textureLod(BaseTexture, vec2(0.5, 0.5), gMipLod);
//
//	// 実際のHDR画像
//	vec3 texel = textureLod(BaseTexture2, shaderInput.uv, 0).rgb;
//
//	float coeff		   = 0.18 * exp( -info.g );	// 輝度の平均値の基準輝度
//	float maxIntensity = coeff * info.r;		// 輝度の最大値
//
//	// YCbCr系に変換
//	vec3	YCbCr;
//	YCbCr.y = dot( RGBtoCb, texel );
//	YCbCr.z = dot( RGBtoCr, texel );
//
//	// 輝度のトーンカーブ補正(Y成分)
//	float lum = coeff * dot( RGBtoY, texel );
//
//	//YCbCr.x = lum;
//	YCbCr.x = lum * (1.0 + lum/(maxIntensity * maxIntensity)) / (1.0+lum);
//
//	// RGB系にして出力
//	fragColor.r = dot( YCbCrtoR, YCbCr );
//	fragColor.g = dot( YCbCrtoG, YCbCr );
//	fragColor.b = dot( YCbCrtoB, YCbCr );
//
//	fragColor.a = 1.0f;
//
//	//fragColor.rgb = vec3(info.g);
//}


// Filmic Tonemapping
vec3 Uncharted2Tonemap(vec3 x)
{
	return ((x * (A*x + C*B) + D*E)/(x * (A*x + B) + D*F)) - E/F;
}

#if 1

void main()
{
	// 1x1サイズまで縮小した情報テクスチャ(YUV状態)
	vec4 info = textureLod(BaseTexture, vec2(0.5, 0.5), gMipLod);

	float adaptationScale = info.r;

	// 元のHDR画像
	vec3	texColor = textureLod(BaseTexture2, shaderInput.uv, gMipLod).rgb;
	//texColor		 = pow( texColor, vec3(2.2) );
	//texColor		*= vec3(16);	 
	//texColor		*= vec3(adaptationScale);


	// F(x) = ((x * (A*x + C*B) + D*E)/(x * (A*x + B) + D*F)) - E/F;
	// FinalColor = F(LinearColor)/F(LinearWhite)
	float	ExposureBias = 2.0;

	// F(LinearColor)
	vec3	curr		= Uncharted2Tonemap( vec3(ExposureBias) * texColor );
	// 1.0 / F(LinearWhite)
	vec3	whiteScale	= vec3( 1.0 / Uncharted2Tonemap(W) );

	// LDR状態の画像(Final Color)
	vec3	color		= curr * whiteScale;
	// 逆ガンマ補正
	//vec3	retColor	= pow(color, vec3( 1/2.2 ));
	vec3	retColor	= color;

	//fragColor.rgb = min(vec3(1.0), fragColor.rgb);

	// リニア→sRGB
	fragColor = vec4(retColor, 1.0);

}

#else

void main()
{

	// 1x1サイズまで縮小した情報テクスチャ
	vec4 info = textureLod(BaseTexture, vec2(0.5, 0.5), gMipLod);


	float adaptationScale = info.r;

	// 元のHDR画像
	vec3	texColor = textureLod(BaseTexture2, shaderInput.uv, 0).rgb;
	//texColor		 = pow(texColor, vec3(2.2));
	//texColor		*= vec3(adaptationScale);

	//vec3 retColor = pow(texColor, vec3(1/2.2));

	vec3 retColor = texColor;

	// リニア→sRGB
	fragColor = vec4(retColor, 1.0);
}

#endif