// BloomGlare.frag
// �u���[���O���A
#version 330

// �f���o���̐F
layout(location = 0) out vec4 fragColor;
// �e�N�X�`��
uniform sampler2D BaseTexture;

// �o�[�e�b�N�X����󂯎��l
in Data{
	vec4	diffuseColor;
	vec4	emissiveColor;
	vec2	uv;
}shaderInput;

uniform float brightness;

void main()
{
	fragColor  = textureLod(BaseTexture, shaderInput.uv, 1) * 0.25;
	fragColor += textureLod(BaseTexture, shaderInput.uv, 2) * 0.50;
	fragColor += textureLod(BaseTexture, shaderInput.uv, 3) * 0.75;
	fragColor += textureLod(BaseTexture, shaderInput.uv, 4);
	fragColor += textureLod(BaseTexture, shaderInput.uv, 5);
	fragColor += textureLod(BaseTexture, shaderInput.uv, 6);
	fragColor *= (1.0/6.0) * brightness;
}