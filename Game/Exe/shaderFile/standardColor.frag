// standardColor.frag
// �W���`��(�e�N�X�`���Ȃ�)

#version 330

// �f���o���̐F
layout(location = 0) out vec4 fragColor;

// �o�[�e�b�N�X����󂯎��l
in Data{
	vec4	diffuseColor;
	vec2	uv;
}shaderInput;

void main()
{
	fragColor = shaderInput.diffuseColor;
}