// Subtract.frag
// ���Z
#version 330

// �f���o���̐F
layout(location = 0) out vec4 fragColor;
// �e�N�X�`��
uniform sampler2D BaseTexture;
// �~�b�v�}�b�v�ԍ�
uniform float	  gMipLod;

// �o�[�e�b�N�X����󂯎��l
in Data{
	vec4	diffuseColor;
	vec4	emissiveColor;
	vec2	uv;
}shaderInput;

// 臒l
uniform float		gThreshold;

uniform float		scale;

void main()
{
	fragColor = textureLod(BaseTexture, shaderInput.uv, gMipLod) - gThreshold;

	fragColor *= vec4(scale);

	// �}�C�i�X�΍�
	fragColor	= max(fragColor, vec4(0.0));
}