// Filter.frag
// �|�X�g�G�t�F�N�g�t�B���^�[
#version 420

// �f���o���̐F
layout(location = 0) out vec4 fragColor;
// �e�N�X�`��
uniform sampler2D BaseTexture;
// �e�N�X�`��
uniform sampler2D BaseTexture2;
// �~�b�v�}�b�v�ԍ�
uniform float	  gMipLod;

// �o�[�e�b�N�X����󂯎��l
in Data{
	vec4	diffuseColor;
	vec4	emissiveColor;
	vec2	uv;
}shaderInput;

void main()
{
	vec4 ambient = textureLod(BaseTexture, shaderInput.uv, gMipLod);
	fragColor = ambient;
	fragColor.rgb *= shaderInput.diffuseColor.rgb;
}