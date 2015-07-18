// MotionBlur.frag
// ���[�V�����u���[
#version 330

// �f���o���̐F
layout(location = 0) out vec4 fragColor;

uniform sampler2D	BaseTexture;
uniform float		gMipLod;	// �~�b�v�i��

// �o�[�e�b�N�X����󂯎��l
in Data{
	vec2	_uv;
	vec4	_uv1;
	vec4	_uv2;
	vec4	_uv3;
}shaderInput;

void main()
{
	vec2	uv1 = shaderInput._uv;
	vec2	uv2 = shaderInput._uv1.xy;
	vec2	uv3 = shaderInput._uv1.zw;
	vec2	uv4 = shaderInput._uv2.xy;
	vec2	uv5 = shaderInput._uv2.zw;
	vec2	uv6 = shaderInput._uv3.xy;
	vec2	uv7 = shaderInput._uv3.zw;

	fragColor  = textureLod(BaseTexture, uv1, gMipLod) * (1.0/7.0);
	fragColor += textureLod(BaseTexture, uv2, gMipLod) * (1.0/7.0);
	fragColor += textureLod(BaseTexture, uv3, gMipLod) * (1.0/7.0);
	fragColor += textureLod(BaseTexture, uv4, gMipLod) * (1.0/7.0);
	fragColor += textureLod(BaseTexture, uv5, gMipLod) * (1.0/7.0);
	fragColor += textureLod(BaseTexture, uv6, gMipLod) * (1.0/7.0);
	fragColor += textureLod(BaseTexture, uv7, gMipLod) * (1.0/7.0);
}