// GlareFilter.frag
// グレアフィルター
#version 330

// 吐き出しの色
layout(location = 0) out vec4 fragColor;

uniform sampler2D	BaseTexture;
uniform float		gMipLod;	// ミップ段数

// バーテックスから受け取る値
in Data{
	vec4	_uv12;
	vec4	_uv34;
	vec4	_uv56;
	vec4	_uv78;
	float	_stride;
}shaderInput;

void main()
{
	vec2	uv1 = shaderInput._uv12.xy;
	vec2	uv2 = shaderInput._uv12.zw;
	vec2	uv3 = shaderInput._uv34.xy;
	vec2	uv4 = shaderInput._uv34.zw;
	vec2	uv5 = shaderInput._uv56.xy;
	vec2	uv6 = shaderInput._uv56.zw;
	vec2	uv7 = shaderInput._uv78.xy;
	vec2	uv8 = shaderInput._uv78.zw;

	const float attenuation = 0.99;	// 減衰係数
	float stride = shaderInput._stride;

	fragColor  = textureLod(BaseTexture, uv1, gMipLod) * pow(attenuation, 0 * stride) * (1.0/8.0);
	fragColor += textureLod(BaseTexture, uv2, gMipLod) * pow(attenuation, 1 * stride) * (1.0/8.0);
	fragColor += textureLod(BaseTexture, uv3, gMipLod) * pow(attenuation, 2 * stride) * (1.0/8.0);
	fragColor += textureLod(BaseTexture, uv4, gMipLod) * pow(attenuation, 3 * stride) * (1.0/8.0);
	fragColor += textureLod(BaseTexture, uv5, gMipLod) * pow(attenuation, 4 * stride) * (1.0/8.0);
	fragColor += textureLod(BaseTexture, uv6, gMipLod) * pow(attenuation, 5 * stride) * (1.0/8.0);
	fragColor += textureLod(BaseTexture, uv7, gMipLod) * pow(attenuation, 6 * stride) * (1.0/8.0);
}