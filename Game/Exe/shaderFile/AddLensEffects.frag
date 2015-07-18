// AddLensEffects.frag
// �����Y�ɉ���A�X�^�[�o�[�X�g��ǉ�����

//#version 420
#version 330

// �����Y�t���A�摜
uniform sampler2D BaseTexture;
// �����Y����
uniform sampler2D lensDirtTex;
// �X�^�[�o�[�X�g
uniform sampler2D lensStarTex;
// �J�����̉�]�p�x�s��
uniform mat4 lensStarMatrix;	
// �~�b�v�}�b�v�ԍ�
uniform float	  gMipLod;



in Data{
	vec4	diffuseColor;
	vec2	uv;
}shaderInput;

// �f���o���̐F
layout(location = 0) out vec4 fragColor;


void main() { 

	vec2 texCoord = shaderInput.uv;

	// �����Y����F�擾
	vec4 lensMod = texture(lensDirtTex, texCoord);
	// �X�^�[�o�[�X�g��UV
	vec2 lensStarTexcoord = (lensStarMatrix * vec4(texCoord, 1.0, 1.0)).xy;
	// �����Y����+�X�^�[�o�[�X�g�̐F
	//lensMod  = texture(lensStarTex, lensStarTexcoord);
	lensMod  += texture(lensStarTex, lensStarTexcoord);

	fragColor = textureLod(BaseTexture, texCoord, gMipLod) * 10.0f;

	fragColor  *= lensMod;
}