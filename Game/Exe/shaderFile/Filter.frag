// Filter.frag
// ポストエフェクトフィルター
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

void main()
{
	fragColor      = textureLod(BaseTexture, shaderInput.uv, gMipLod);
	fragColor.rgb *= shaderInput.diffuseColor.rgb;
}