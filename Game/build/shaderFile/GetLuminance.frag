// GetLuminance.frag
// 輝度情報の最初の生成
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
	vec4	_uv1;
	vec4	_uv2;
}shaderInput;

void main()
{
	vec2	uv1 = shaderInput._uv1.xy;
	vec2	uv2 = shaderInput._uv1.zw;
	vec2	uv3 = shaderInput._uv2.xy;
	vec2	uv4 = shaderInput._uv2.zw;

	vec3	RGBtoY = vec3(0.299, 0.587, 0.114);	// RGB→YUV変換

	//---- YUVに変換
	float	intensity1 = dot( RGBtoY, textureLod(BaseTexture, uv1, gMipLod).rgb );
	float	intensity2 = dot( RGBtoY, textureLod(BaseTexture, uv2, gMipLod).rgb );
	float	intensity3 = dot( RGBtoY, textureLod(BaseTexture, uv3, gMipLod).rgb );
	float	intensity4 = dot( RGBtoY, textureLod(BaseTexture, uv4, gMipLod).rgb );

	

	//---- 最大輝度を取得
	float	maxIntensity = max( max(intensity1, intensity2), max(intensity3, intensity4));

	//---- 平均輝度を取得
	float	averageIntensity;
	const float EPSILON = 0.000001;	// 0を割るのを防ぐため
	// a^x = b → x = loga(b) 
	// ( a = 2.71828 18284 59045 23536 02874 71352 
	// というネイピア数(自然対数の底 普段は e と表記される) )
	averageIntensity  = log(intensity1 + EPSILON) / 4;
	averageIntensity += log(intensity2 + EPSILON) / 4;
	averageIntensity += log(intensity3 + EPSILON) / 4;
	averageIntensity += log(intensity4 + EPSILON) / 4;

	fragColor.r  = maxIntensity;
	fragColor.g  = averageIntensity;
	fragColor.ba = vec2(1.0);
}