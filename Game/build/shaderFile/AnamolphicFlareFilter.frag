// AnamolphicFlareFilter.frag
// アナモルフィックなどの垂直ブラー用

#version 420

// 吐き出しの色
layout(location = 0) out vec4 fragColor;

uniform sampler2D	BaseTexture;
uniform float		gMipLod;			 // ミップ段数
uniform float		gAttenuation = 0.99; // 減衰係数


// バーテックスから受け取る値
in Data{
	vec4	_uv12;
	vec4	_uv34;
	vec4	_uv56;
	vec4	_uv78;
	vec4	_invUV12;
	vec4	_invUV34;
	vec4	_invUV56;
	vec4	_invUV78;
	float	_stride;
}shaderInput;

void main()
{
	//---- 通常のUVオフセット
	vec2	uv1	   = shaderInput._uv12.xy;
	vec2	uv2	   = shaderInput._uv12.zw;
	vec2	uv3	   = shaderInput._uv34.xy;
	vec2	uv4	   = shaderInput._uv34.zw;
	vec2	uv5	   = shaderInput._uv56.xy;
	vec2	uv6	   = shaderInput._uv56.zw;
	vec2	uv7	   = shaderInput._uv78.xy;
	vec2	uv8	   = shaderInput._uv78.zw;
	//---- マイナスのオフセット
	vec2	invUV1 = shaderInput._invUV12.xy;
	vec2	invUV2 = shaderInput._invUV12.zw;
	vec2	invUV3 = shaderInput._invUV34.xy;
	vec2	invUV4 = shaderInput._invUV34.zw;
	vec2	invUV5 = shaderInput._invUV56.xy;
	vec2	invUV6 = shaderInput._invUV56.zw;
	vec2	invUV7 = shaderInput._invUV78.xy;
	vec2	invUV8 = shaderInput._invUV78.zw;

	float attenuation = gAttenuation;	// 減衰係数
	float stride = shaderInput._stride;

	fragColor  = textureLod(BaseTexture, uv1,	 gMipLod) * pow(attenuation, 0 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, uv2,	 gMipLod) * pow(attenuation, 1 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, uv3,	 gMipLod) * pow(attenuation, 2 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, uv4,	 gMipLod) * pow(attenuation, 3 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, uv5,	 gMipLod) * pow(attenuation, 4 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, uv6,	 gMipLod) * pow(attenuation, 5 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, uv7,	 gMipLod) * pow(attenuation, 6 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, uv8,	 gMipLod) * pow(attenuation, 7 * stride) * (1.0/16.0);

	fragColor += textureLod(BaseTexture, invUV1, gMipLod) * pow(attenuation, 0 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, invUV2, gMipLod) * pow(attenuation, 1 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, invUV3, gMipLod) * pow(attenuation, 2 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, invUV4, gMipLod) * pow(attenuation, 3 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, invUV5, gMipLod) * pow(attenuation, 4 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, invUV6, gMipLod) * pow(attenuation, 5 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, invUV7, gMipLod) * pow(attenuation, 6 * stride) * (1.0/16.0);
	fragColor += textureLod(BaseTexture, invUV8, gMipLod) * pow(attenuation, 7 * stride) * (1.0/16.0);

}