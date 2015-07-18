// Gaussian3x3_H.vert
// 水平ブラー(ガウシアン3x3)
#version 330

in vec4			position;
in vec4			color;
in vec2			uv;

uniform vec4	gTextureSize;

out Data {
	vec4	_diffuseColor;
	vec4	_uv1;
	vec2	_uv2;
}shaderOutput;

void main()
{
	vec2 invTextureSize = gTextureSize.zw;

	gl_Position				  = position;

	//---- Output
	shaderOutput._diffuseColor	= color;
	shaderOutput._uv1.xy		= uv;
	shaderOutput._uv1.zw		= uv + vec2(-1, 0) * invTextureSize;
	shaderOutput._uv2.xy		= uv + vec2(+1, 0) * invTextureSize;
	
}