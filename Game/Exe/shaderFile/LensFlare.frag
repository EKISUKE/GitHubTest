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

in Data{
	vec4	diffuseColor;
	vec2	uv;
}shaderInput;

// �f���o���̐F
layout(location = 0) out vec4 fragColor;


// �����Y�̐F�̘c�݂��擾����
vec4 textureDistorted(
	in sampler2D tex,
	in vec2		 uv,
	in vec2		 direction,
	in vec3		 distortion
) {
	return vec4(
		texture(tex, uv + direction * distortion.r).r,
		texture(tex, uv + direction * distortion.g).g,
		texture(tex, uv + direction * distortion.b).b,
		1.0
	);
}

void main() { 
	vec2 flipUV		= -shaderInput.uv + vec2(1.0);
	vec2 texelSize	= 1.0 / vec2(textureSize(BaseTexture, 0));

	vec2 ghostVec	= (vec2(0.5) - flipUV) * uDispersal;

	// �c��
	vec3 distortion = vec3(-texelSize.x * uDistortion, 0.0, texelSize.x * uDistortion);

	// �S�[�X�g�v�Z���ċ��߂�
	vec4 result = vec4(0.0);

	for(int i=0; i<uSamples; ++i) {
		vec2	offset = fract(flipUV + ghostVec * float(i));

		float	weight = length(vec2(0.5) - offset) / length(vec2(0.5));
		weight = pow(1.0 - weight, 3.5f);

		//result += texture(BaseTexture, offset)  * weight;

		result += textureDistorted(
			BaseTexture,
			offset,
			normalize(ghostVec),
			distortion
		) * weight;

		//result = vec4(weight);
	}

	
	result *= texture( lensColorTex, length(vec2(0.5) - flipUV ) / length(vec2(0.5)) );

	
	// HALO���v�Z���ċ��߂�
	// HALO�x�N�g��
	vec2 haloVec = normalize(ghostVec) * uHaloWidth;
	float weight = length(vec2(0.5) - fract(flipUV + haloVec))	/ length(vec2(0.5));
	weight = pow(1.0 - weight, 1.0);
	result += textureDistorted(
		BaseTexture,
		fract(flipUV + haloVec),
		normalize(ghostVec),
		distortion
	) * weight;


	fragColor = result;

}