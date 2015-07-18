// LightAdaptation.frag
// ������ / �Ï���
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
	// BaseTexture  : �W�v�e�N�X�`�� ( gMipLod��mip�w�肳��Ă���, LuminanceReduction �̌��� )
	// BaseTexture2 : �����e�N�X�`�� ( 1x1�̃e�N�X�`�� )
	vec2	info = textureLod(BaseTexture, shaderInput.uv, gMipLod).rg;

	// ���݂̏����e�N�X�`��
	vec2	adaptation = texture(BaseTexture2, shaderInput.uv).rg;

	// �ǂ̂��炢�ω����邩
	adaptation += (info - adaptation) * 0.02;

	// ���ʂ̒l���e�N�X�`���Ƃ��ĕۑ�
	fragColor = vec4(adaptation, 1, 1);
}