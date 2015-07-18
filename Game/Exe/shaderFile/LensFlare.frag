// LensFlare.frag
// レンズフレア

//#extension GL_ARB_shading_language_include : require

//#include "common/define.glsl"

#version 330

// 元の画像
uniform sampler2D BaseTexture;
// レンズカラー
uniform sampler1D lensColorTex;

// ゴーストのサンプル数
uniform int		uSamples	= 8;
// 分散
uniform float	uDispersal  = 0.3;
// HALOの幅
uniform float	uHaloWidth  = 0.47;
// 歪み
uniform float	uDistortion = 1.0;

in Data{
	vec4	diffuseColor;
	vec2	uv;
}shaderInput;

// 吐き出しの色
layout(location = 0) out vec4 fragColor;


// レンズの色の歪みを取得する
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

	// 歪み
	vec3 distortion = vec3(-texelSize.x * uDistortion, 0.0, texelSize.x * uDistortion);

	// ゴースト計算して求める
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

	
	// HALOを計算して求める
	// HALOベクトル
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