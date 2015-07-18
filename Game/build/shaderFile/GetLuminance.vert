// GetLuminance.vert
// 輝度情報の最初の生成
#version 330

in vec4			position;
in vec4			color;
in vec2			uv;

uniform vec4	gTextureSize;

out Data {
	vec4	_uv1;
	vec4	_uv2;
}shaderOutput;

void main()
{
	gl_Position				  = position;

	// テクスチャサイズの逆数
	vec2	invTextureSize = gTextureSize.zw;

	//---- Output
	shaderOutput._uv1.xy = uv + vec2(-0.5, -0.5) * invTextureSize;
	shaderOutput._uv1.zw = uv + vec2( 0.5, -0.5) * invTextureSize;
	shaderOutput._uv2.xy = uv + vec2(-0.5,  0.5) * invTextureSize;
	shaderOutput._uv2.zw = uv + vec2( 0.5,  0.5) * invTextureSize;
}