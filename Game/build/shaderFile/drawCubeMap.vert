// drawCubeMap.vert
// �L���[�u�}�b�v�`��p(SkyBox�Ȃ�)

#version 330

in vec4		position; 

out vec3	texCoord;

//---- �s��
layout(std140) uniform Matrices {
	mat4	gMatWorld;
	mat4	gViewMatrix;
	mat4	gProjMatrix;
};

void main(void)
{
	// ���W�ϊ�
	mat4 toWorldMat = gProjMatrix * gViewMatrix * gMatWorld;
	gl_Position		= vec4( toWorldMat * position );
	texCoord		= position.xyz;
}