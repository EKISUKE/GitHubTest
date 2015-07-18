// LensFlare.frag
// �����Y�t���A

//#extension GL_ARB_shading_language_include : require

//#include "common/define.glsl"

#version 330

// ���̉摜
uniform sampler2D BaseTexture;
// �����Y�J���[
uniform sampler1D lensColorTex;

// �S�[�X�g�̃T���v����
uniform int		uSamples	= 8;
// ���U
uniform float	uDispersal  = 0.3;
// HALO�̕�
uniform float	uHaloWidth  = 0.47;
// �c��
uniform float	uDistortion = 1.0;

uniform bool	isAnamolphic = false;

in Data{
	vec4	diffuseColor;
	vec4	emissiveColor;
	vec2	uv;
}shaderInput;

// �f���o���̐F
layout(location = 0) out vec4 fragColor;


// �S�[�X�g(����)�̐F���擾����
vec3 textureDistorted(
	in sampler2D tex,			// ���̃e�N�X�`��
	in vec2		 uv,			// UV���W
	in vec2		 direction,		// �ړ�����(�S�[�X�g�̃I�t�Z�b�g)
	in vec3		 distortion		// �c�݁A�����␳
) {
	return vec3(
		texture(tex, uv + direction * distortion.r).r,
		texture(tex, uv + direction * distortion.g).g,
		texture(tex, uv + direction * distortion.b).b
	);
	
}


void main() { 
	// �e�N�X�`�����W�𔽓]
	vec2 flipUV		= -shaderInput.uv + vec2(1.0);
	// �e�N�Z���̃T�C�Y(�e�N�X�`���̃T�C�Y�̋t��)
	vec2 texelSize	= 1.0 / vec2(textureSize(BaseTexture, 0));

	// ���_
	vec2 centerPos = vec2(0.5);
	// �T���v����
	int	 sample = uSamples;
	// �A�i�����t�B�b�N�Ȃ���Ώ̂Ȃ̂Ō��_��Y��ł_��Y�ɂ��킹��
	// �T���v���͈��̂�(���]�����邾��)
	if(isAnamolphic) {
		flipUV.y	= shaderInput.uv.y;
		centerPos.y = flipUV.y;
		sample		= 4;
	}
	// ���_�����ւ̃x�N�g��
	vec2 ghostVec = (centerPos - flipUV) * uDispersal;

	// �c��
	vec3 distortion = vec3(-texelSize.x * uDistortion, 0.0, texelSize.x * uDistortion);

	// �S�[�X�g�v�Z���ċ��߂�
	vec4 result = vec4(0.0);
	// �T���v���������_�����ւ̃x�N�g������S�[�X�g���쐬
	for (int i=0; i<sample; ++i) {
		// fract�ŏ����_�������o���ČJ��Ԃ�(TexParamerter��GL_REPEAT�ł�����)
		//vec2	offset = fract(flipUV + ghostVec * float(i) * 7);
		vec2	offset = fract(flipUV + ghostVec * float(i));

		float	weight = length(vec2(0.5) - offset) / length(vec2(0.5));
		weight = isAnamolphic ? 1.0 : pow(1.0 - weight, 3.5f);

		//result += texture(BaseTexture, offset)  * weight;

		result.rgb += textureDistorted(
								BaseTexture,
								offset,
								normalize(ghostVec),
								distortion
								) * weight;

		//result = vec4(weight);
	}

	// ���F�̂悤�Ȃ��̂�����
	if(isAnamolphic == false)
	{
		result *= texture( lensColorTex, length(vec2(0.5) - flipUV ) / length(vec2(0.5)) );
	}

	
	// HALO���v�Z���ċ��߂�
	// HALO�x�N�g��
	// �A�i�����t�B�b�N����Ȃ������珈������
	if(isAnamolphic == false) {
		vec2 haloVec = normalize(ghostVec) * uHaloWidth;
		float weight = length(vec2(0.5) - fract(flipUV + haloVec))	/ length(vec2(0.5));
		weight = pow(1.0 - weight, 1.0);
		result.rgb += textureDistorted(
								BaseTexture,
								fract(flipUV + haloVec),
								normalize(ghostVec),
								distortion
								) * weight;

	}

	fragColor = result;

	//float blue = (fragColor.r + fragColor.g + fragColor.b) * (1.0/3.0);
	float blue = max( max(fragColor.r,  fragColor.g), fragColor.b );

	//fragColor.rgb = vec3(0.0, 0.0, blue);
	fragColor.rgb *= 10.0f;

}