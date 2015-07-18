// standardColor.frag
// 標準描画(テクスチャなし)

#version 330

// 吐き出しの色
layout(location = 0) out vec4 fragColor;

// バーテックスから受け取る値
in Data{
	vec4	diffuseColor;
	vec2	uv;
}shaderInput;

void main()
{
	fragColor = shaderInput.diffuseColor;
}