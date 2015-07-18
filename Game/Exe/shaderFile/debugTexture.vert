// debugTexture.vert
// �f�o�b�O�p�V�F�[�_�[(�f�o�b�O���������p�Ɏ��R�Ɏg����V�F�[�_�[)
#version 330

in vec4		position;
in vec4		color;
in vec2		uv;

out Data {
	vec4	diffuseColor;
	vec2	uv;
}shaderOutput;

void main()
{
	gl_Position				  = position;
	//---- Output
	shaderOutput.uv			  = uv;
	shaderOutput.diffuseColor = color;
}