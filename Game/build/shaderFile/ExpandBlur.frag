// ExpandBlur.frag
// 拡大ブラー(4点0.25フェッチ)
#version 420

// 吐き出しの色
layout(location = 0) out vec4 fragColor;
// テクスチャ
uniform sampler2D BaseTexture;
// ミップマップ番号
uniform float	  gMipLod;

// バーテックスから受け取る値
in Data{
	vec4	_diffuseColor;
	vec4	_uv[2];
}shaderInput;

void main()
{
	vec2 uv1 = shaderInput._uv[0].xy;
	vec2 uv2 = shaderInput._uv[0].zw;
	vec2 uv3 = shaderInput._uv[1].xy;
	vec2 uv4 = shaderInput._uv[1].zw;

	fragColor  = textureLod(BaseTexture, uv1, gMipLod) * (0.25);
	fragColor += textureLod(BaseTexture, uv2, gMipLod) * (0.25);
	fragColor += textureLod(BaseTexture, uv3, gMipLod) * (0.25);
	fragColor += textureLod(BaseTexture, uv4, gMipLod) * (0.25);

	
	
}
