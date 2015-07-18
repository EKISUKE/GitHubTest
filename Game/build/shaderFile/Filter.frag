// Filter.frag
// ポストエフェクトフィルター
#version 420

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
	vec4	emissiveColor;
	vec2	uv;
}shaderInput;

void main()
{
	vec4 ambient = textureLod(BaseTexture, shaderInput.uv, gMipLod);
	fragColor = ambient;
	fragColor.rgb *= shaderInput.diffuseColor.rgb;
}