// LuminanceReduction.frag
// トーンマッピング 縮小(1パスで1/4)
#version 330

// 吐き出しの色
layout(location = 0) out vec4 fragColor;
// テクスチャ
uniform sampler2D BaseTexture;
// ミップマップ番号
uniform float	  gMipLod;

// バーテックスから受け取る値
in Data{
	vec4	_uv0;
	vec4	_uv1;
	vec4	_uv2;
	vec4	_uv3;
	vec4	_uv4;
	vec4	_uv5;
	vec4	_uv6;
	vec4	_uv7;
	vec4	_uv8;
	vec4	_uv9;
}shaderInput;



void main()
{
	vec2 uv01 = shaderInput._uv0.xy;
	vec2 uv02 = shaderInput._uv0.zw;
	vec2 uv03 = shaderInput._uv1.xy;
	vec2 uv04 = shaderInput._uv1.zw;
	vec2 uv05 = shaderInput._uv2.xy;
	vec2 uv06 = shaderInput._uv2.zw;
	vec2 uv07 = shaderInput._uv3.xy;
	vec2 uv08 = shaderInput._uv3.zw;
	vec2 uv09 = shaderInput._uv4.xy;
	vec2 uv10 = shaderInput._uv4.zw;
	vec2 uv11 = shaderInput._uv5.xy;
	vec2 uv12 = shaderInput._uv5.zw;
	vec2 uv13 = shaderInput._uv6.xy;
	vec2 uv14 = shaderInput._uv6.zw;
	vec2 uv15 = shaderInput._uv7.xy;
	vec2 uv16 = shaderInput._uv7.zw;
	vec2 uv17 = shaderInput._uv8.xy;
	vec2 uv18 = shaderInput._uv8.zw;
	vec2 uv19 = shaderInput._uv9.xy;
	vec2 uv20 = shaderInput._uv9.zw;


	//---- 輝度取得
	vec2 i01 = textureLod(BaseTexture, uv01, gMipLod).rg;
	vec2 i02 = textureLod(BaseTexture, uv02, gMipLod).rg;
	vec2 i03 = textureLod(BaseTexture, uv03, gMipLod).rg;
	vec2 i04 = textureLod(BaseTexture, uv04, gMipLod).rg;
	vec2 i05 = textureLod(BaseTexture, uv05, gMipLod).rg;
	vec2 i06 = textureLod(BaseTexture, uv06, gMipLod).rg;
	vec2 i07 = textureLod(BaseTexture, uv07, gMipLod).rg;
	vec2 i08 = textureLod(BaseTexture, uv08, gMipLod).rg;
	vec2 i09 = textureLod(BaseTexture, uv09, gMipLod).rg;
	vec2 i10 = textureLod(BaseTexture, uv10, gMipLod).rg;
	vec2 i11 = textureLod(BaseTexture, uv11, gMipLod).rg;
	vec2 i12 = textureLod(BaseTexture, uv12, gMipLod).rg;
	vec2 i13 = textureLod(BaseTexture, uv13, gMipLod).rg;
	vec2 i14 = textureLod(BaseTexture, uv14, gMipLod).rg;
	vec2 i15 = textureLod(BaseTexture, uv15, gMipLod).rg;
	vec2 i16 = textureLod(BaseTexture, uv16, gMipLod).rg;
	vec2 i17 = textureLod(BaseTexture, uv17, gMipLod).rg;
	vec2 i18 = textureLod(BaseTexture, uv18, gMipLod).rg;
	vec2 i19 = textureLod(BaseTexture, uv19, gMipLod).rg;
	vec2 i20 = textureLod(BaseTexture, uv20, gMipLod).rg;

	
	//---- 最大輝度を取得
	float	maxIntensity = max ( 
								max( 
									max(
										max( max(i01, i02), max(i03, i04) ),
										max( max(i05, i06), max(i07, i08) )
									),
									max(
										max( max(i09, i10), max(i11, i12) ),
										max( max(i13, i14), max(i15, i16) )
									)
								),
								max( max(i17, i18), max(i19, i20) )
							).r;

	//---- 平均輝度を取得
	vec2	average = (i01 + i02 + i03 + i04 +
					   i05 + i06 + i07 + i08 + 
					   i09 + i10 + i11 + i12 +
					   i13 + i14 + i15 + i16 +
					   i17 + i18 + i19 + i20) * (1.0/20.0f);



	fragColor.r  = maxIntensity;
	fragColor.g  = average.g;
	fragColor.ba = vec2(1.0);
}