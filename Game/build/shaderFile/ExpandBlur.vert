// ExpandBlur.vert
// 拡大ブラー(4点0.25フェッチ)
#version 420

in vec4		position;
in vec4		color;
in vec2		uv;

uniform vec4	gTextureSize;
uniform float	gUVOffset = 0.5f;

out Data {
	vec4	_diffuseColor;
	vec4	_uv[2];
}shaderOutput;
void main()
{
	vec2 invTextureSize = gTextureSize.zw;

	gl_Position			= position;
	float uvOffset		= gUVOffset;
	//---- Output
	shaderOutput._diffuseColor	= color;
	shaderOutput._uv[0].xy		= uv + vec2(-uvOffset, -uvOffset) * invTextureSize;
	shaderOutput._uv[0].zw		= uv + vec2(+uvOffset, -uvOffset) * invTextureSize;
	shaderOutput._uv[1].xy		= uv + vec2(-uvOffset, +uvOffset) * invTextureSize;
	shaderOutput._uv[1].zw		= uv + vec2(+uvOffset, +uvOffset) * invTextureSize;

}