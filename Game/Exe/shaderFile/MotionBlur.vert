// MotionBlur.vert
#version 330

in vec4		position;
in vec2		uv;

uniform vec2	gDir2D;
uniform vec4	gTextureSize;
uniform float	gStride;


out Data {
	vec2	_uv;
	vec4	_uv1;
	vec4	_uv2;
	vec4	_uv3;
}shaderOutput;


void main()
{

	gl_Position		= position;

	// テクスチャサイズの逆数
	vec2	invTextureSize = gTextureSize.zw;
	
	float	stride	= gStride;

	//---- Output
	shaderOutput._uv.xy			  = uv + gDir2D *  0 * stride * invTextureSize;
													   
	shaderOutput._uv1.xy		  = uv + gDir2D * -1 * stride * invTextureSize;
	shaderOutput._uv1.zw		  = uv + gDir2D * +1 * stride * invTextureSize;
													   
	shaderOutput._uv2.xy		  = uv + gDir2D * -2 * stride * invTextureSize;
	shaderOutput._uv2.zw		  = uv + gDir2D * +2 * stride * invTextureSize;
													   
	shaderOutput._uv3.xy		  = uv + gDir2D * -3 * stride * invTextureSize;
	shaderOutput._uv3.zw		  = uv + gDir2D * +3 * stride * invTextureSize;


	
}