// Subtract.frag
// 減算
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
	vec4	emissiveColor;
	vec2	uv;
}shaderInput;

// 閾値
uniform float		gThreshold;

uniform float		scale;

void main()
{
	fragColor = textureLod(BaseTexture, shaderInput.uv, gMipLod) - gThreshold;

	fragColor *= vec4(scale);

	// マイナス対策
	fragColor	= max(fragColor, vec4(0.0));
}