// Gaussian3x3.frag
// �K�E�V�A��3x3
#version 330

// �f���o���̐F
layout(location = 0) out vec4 fragColor;
// �e�N�X�`��
uniform sampler2D BaseTexture;
// �~�b�v�}�b�v�ԍ�
uniform float	  gMipLod;

// �o�[�e�b�N�X����󂯎��l
in Data{
	vec4	_diffuseColor;
	vec4	_uv1;
	vec2	_uv2;
}shaderInput;

void main()
{
	vec2 uv1 = shaderInput._uv1.xy;
	vec2 uv2 = shaderInput._uv1.zw;
	vec2 uv3 = shaderInput._uv2.xy;

	fragColor  = textureLod(BaseTexture, uv2, gMipLod) * (1.0f/4.0f);
	fragColor += textureLod(BaseTexture, uv1, gMipLod) * (2.0f/4.0f);
	fragColor += textureLod(BaseTexture, uv3, gMipLod) * (1.0f/4.0f);
	
}
