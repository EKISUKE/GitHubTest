// standard.frag
// 標準描画
#version 330

// 吐き出しの色
layout(location = 0) out vec4 fragColor;
// テクスチャ
uniform sampler2D BaseTexture;
// テクスチャ
uniform sampler2D BaseTexture2;

// バーテックスから受け取る値
in Data{
	vec4	diffuseColor;
	vec2	uv;
}shaderInput;

void main()
{
	fragColor = texture(BaseTexture, shaderInput.uv);
	fragColor *= shaderInput.diffuseColor;
}