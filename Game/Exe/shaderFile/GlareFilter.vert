// GlareFilter.vert
#version 330

in vec4		position;
in vec2		uv;

uniform vec2	gDir2D;
uniform vec4	gTextureSize;
uniform float	gStride;


out Data {
	vec4	_uv12;
	vec4	_uv34;
	vec4	_uv56;
	vec4	_uv78;
	float	_stride;
}shaderOutput;


void main()
{

	gl_Position		= position;

	// テクスチャサイズの逆数
	vec2	invTextureSize = gTextureSize.zw;
	
	float	stride	= gStride;

	//---- Output
	shaderOutput._uv12.xy	= uv + gDir2D * 0 * stride * invTextureSize;
	shaderOutput._uv12.zw	= uv + gDir2D * 1 * stride * invTextureSize;

	shaderOutput._uv34.xy	= uv + gDir2D * 2 * stride * invTextureSize;
	shaderOutput._uv34.zw	= uv + gDir2D * 3 * stride * invTextureSize;
													   
	shaderOutput._uv56.xy	= uv + gDir2D * 4 * stride * invTextureSize;
	shaderOutput._uv56.zw	= uv + gDir2D * 5 * stride * invTextureSize;
													   
	shaderOutput._uv78.xy	= uv + gDir2D * 6 * stride * invTextureSize;
	shaderOutput._uv78.zw	= uv + gDir2D * 7 * stride * invTextureSize;
	
	shaderOutput._stride	= gStride;
}