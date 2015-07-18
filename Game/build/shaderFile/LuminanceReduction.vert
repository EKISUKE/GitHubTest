// LuminanceReduction.vert
// トーンマッピング 縮小(1パスで1/4)
#version 330

in vec4		position;
in vec4		color;
in vec2		uv;

uniform vec4	gTextureSize;

out Data {
	vec4	_uv0;
	vec4	_uv1;
	vec4	_uv2;
	vec4	_uv3;
	vec4	_uv4;
	vec4	_uv5;
	vec4	_uv6;
	vec4	_uv7;
	vec4	_uv8;
	vec4	_uv9;
}shaderOutput;


void main()
{
	gl_Position				  = position;

	// テクスチャサイズの逆数
	vec2	invTextureSize = gTextureSize.zw;

#if 0
	//---- Output
	shaderOutput._uv0.xy = uv + vec2(0,0) * invTextureSize;
	shaderOutput._uv0.zw = uv + vec2(1,0) * invTextureSize;
	shaderOutput._uv1.xy = uv + vec2(2,0) * invTextureSize;
	shaderOutput._uv1.zw = uv + vec2(3,0) * invTextureSize;

	shaderOutput._uv2.xy = uv + vec2(0,1) * invTextureSize;
	shaderOutput._uv2.zw = uv + vec2(1,1) * invTextureSize;
	shaderOutput._uv3.xy = uv + vec2(2,1) * invTextureSize;
	shaderOutput._uv3.zw = uv + vec2(3,1) * invTextureSize;


	shaderOutput._uv4.xy = uv + vec2(0,2) * invTextureSize;
	shaderOutput._uv4.zw = uv + vec2(1,2) * invTextureSize;
	shaderOutput._uv5.xy = uv + vec2(2,2) * invTextureSize;
	shaderOutput._uv5.zw = uv + vec2(3,2) * invTextureSize;
	

	shaderOutput._uv6.xy = uv + vec2(0,3) * invTextureSize;
	shaderOutput._uv6.zw = uv + vec2(1,3) * invTextureSize;
	shaderOutput._uv7.xy = uv + vec2(2,3) * invTextureSize;
	shaderOutput._uv7.zw = uv + vec2(3,3) * invTextureSize;

	
#else
	
	
	shaderOutput._uv0.xy = uv + vec2(-2,0) * invTextureSize;
	shaderOutput._uv0.zw = uv + vec2(-1,0) * invTextureSize;
	shaderOutput._uv1.xy = uv + vec2( 0,0) * invTextureSize;
	shaderOutput._uv1.zw = uv + vec2( 1,0) * invTextureSize;
	shaderOutput._uv2.xy = uv + vec2( 2,0) * invTextureSize;

	shaderOutput._uv2.zw = uv + vec2(-2,1) * invTextureSize;
	shaderOutput._uv3.xy = uv + vec2(-1,1) * invTextureSize;
	shaderOutput._uv3.zw = uv + vec2( 0,1) * invTextureSize;
	shaderOutput._uv4.xy = uv + vec2( 1,1) * invTextureSize;
	shaderOutput._uv4.zw = uv + vec2( 2,1) * invTextureSize;
	
	shaderOutput._uv5.xy = uv + vec2(-2,2) * invTextureSize;
	shaderOutput._uv5.zw = uv + vec2(-1,2) * invTextureSize;
	shaderOutput._uv6.xy = uv + vec2( 0,2) * invTextureSize;
	shaderOutput._uv6.zw = uv + vec2( 1,2) * invTextureSize;
	shaderOutput._uv7.xy = uv + vec2( 2,2) * invTextureSize;
	
	shaderOutput._uv7.zw = uv + vec2(-2,3) * invTextureSize;
	shaderOutput._uv8.xy = uv + vec2(-1,3) * invTextureSize;
	shaderOutput._uv8.zw = uv + vec2( 0,3) * invTextureSize;
	shaderOutput._uv9.xy = uv + vec2( 1,3) * invTextureSize;
	shaderOutput._uv9.zw = uv + vec2( 2,3) * invTextureSize;

#endif
}