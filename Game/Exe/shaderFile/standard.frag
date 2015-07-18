// standard.frag
// �W���`��
#version 330

// �f���o���̐F
layout(location = 0) out vec4 fragColor;
// �e�N�X�`��
uniform sampler2D BaseTexture;
// �e�N�X�`��
uniform sampler2D BaseTexture2;

// �o�[�e�b�N�X����󂯎��l
in Data{
	vec4	diffuseColor;
	vec2	uv;
}shaderInput;

void main()
{
	fragColor = texture(BaseTexture, shaderInput.uv);
	fragColor *= shaderInput.diffuseColor;
}