// debugTexture.frag
// �f�o�b�O�p�V�F�[�_�[(�f�o�b�O���������p�Ɏ��R�Ɏg����V�F�[�_�[)
#version 330

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
	vec2	uv;
}shaderInput;

void main()
{
	//fragColor = texture(BaseTexture, shaderInput.uv);
	fragColor = textureLod(BaseTexture, shaderInput.uv, gMipLod);
	//fragColor = shaderInput.diffuseColor;
}