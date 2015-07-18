// Gaussian3x3.frag
// ガウシアン3x3
#version 330

// 吐き出しの色
layout(location = 0) out vec4 fragColor;
// テクスチャ
uniform sampler2D BaseTexture;
// ミップマップ番号
uniform float	  gMipLod;

// バーテックスから受け取る値
in Data{
	vec4	diffuseColor;
	vec4	uv1;
	vec2	uv2;
}shaderInput;

void main()
{
	vec2 uv1 = shaderInput.uv1.xy;
	vec2 uv2 = shaderInput.uv1.zw;
	vec2 uv3 = shaderInput.uv2.xy;

	fragColor  = textureLod(BaseTexture, uv2, gMipLod) * (1.0f/4.0f);
	fragColor += textureLod(BaseTexture, uv1, gMipLod) * (2.0f/4.0f);
	fragColor += textureLod(BaseTexture, uv3, gMipLod) * (1.0f/4.0f);
	
}
