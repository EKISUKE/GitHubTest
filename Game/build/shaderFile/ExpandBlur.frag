// ExpandBlur.frag
// �g��u���[(4�_0.25�t�F�b�`)
#version 420

// �f���o���̐F
layout(location = 0) out vec4 fragColor;
// �e�N�X�`��
uniform sampler2D BaseTexture;
// �~�b�v�}�b�v�ԍ�
uniform float	  gMipLod;

// �o�[�e�b�N�X����󂯎��l
in Data{
	vec4	_diffuseColor;
	vec4	_uv[2];
}shaderInput;

void main()
{
	vec2 uv1 = shaderInput._uv[0].xy;
	vec2 uv2 = shaderInput._uv[0].zw;
	vec2 uv3 = shaderInput._uv[1].xy;
	vec2 uv4 = shaderInput._uv[1].zw;

	fragColor  = textureLod(BaseTexture, uv1, gMipLod) * (0.25);
	fragColor += textureLod(BaseTexture, uv2, gMipLod) * (0.25);
	fragColor += textureLod(BaseTexture, uv3, gMipLod) * (0.25);
	fragColor += textureLod(BaseTexture, uv4, gMipLod) * (0.25);

	
	
}
