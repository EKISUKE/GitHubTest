// standard.frag
// �W���`��
#version 420

// �f���o���̐F
layout(location = 0) out vec4 fragColor;
// �e�N�X�`��
uniform sampler2D BaseTexture;
// �e�N�X�`��
uniform sampler2D BaseTexture2;

// �o�[�e�b�N�X����󂯎��l
in Data{
	vec4	diffuseColor;
	vec4	emissiveColor;
	vec2	uv;
}shaderInput;

void main()
{
	vec4 ambient = texture(BaseTexture, shaderInput.uv);
	fragColor		 = ambient;
	fragColor.rgb	*= shaderInput.diffuseColor.rgb;
	fragColor.rgb   += shaderInput.emissiveColor.rgb;

}