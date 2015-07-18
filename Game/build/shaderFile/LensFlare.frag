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

uniform bool	isAnamolphic = false;

in Data{
	vec4	diffuseColor;
	vec4	emissiveColor;
	vec2	uv;
}shaderInput;

// 吐き出しの色
layout(location = 0) out vec4 fragColor;


// ゴースト(虚像)の色を取得する
vec3 textureDistorted(
	in sampler2D tex,			// 元のテクスチャ
	in vec2		 uv,			// UV座標
	in vec2		 direction,		// 移動方向(ゴーストのオフセット)
	in vec3		 distortion		// 歪み、虚像補正
) {
	return vec3(
		texture(tex, uv + direction * distortion.r).r,
		texture(tex, uv + direction * distortion.g).g,
		texture(tex, uv + direction * distortion.b).b
	);
	
}


void main() { 
	// テクスチャ座標を反転
	vec2 flipUV		= -shaderInput.uv + vec2(1.0);
	// テクセルのサイズ(テクスチャのサイズの逆数)
	vec2 texelSize	= 1.0 / vec2(textureSize(BaseTexture, 0));

	// 原点
	vec2 centerPos = vec2(0.5);
	// サンプル数
	int	 sample = uSamples;
	// アナモルフィックなら線対称なので原点のYを打つ点のYにあわせる
	// サンプルは一回のみ(反転させるだけ)
	if(isAnamolphic) {
		flipUV.y	= shaderInput.uv.y;
		centerPos.y = flipUV.y;
		sample		= 4;
	}
	// 原点方向へのベクトル
	vec2 ghostVec = (centerPos - flipUV) * uDispersal;

	// 歪み
	vec3 distortion = vec3(-texelSize.x * uDistortion, 0.0, texelSize.x * uDistortion);

	// ゴースト計算して求める
	vec4 result = vec4(0.0);
	// サンプル数分原点方向へのベクトルからゴーストを作成
	for (int i=0; i<sample; ++i) {
		// fractで小数点だけ取り出して繰り返す(TexParamerterをGL_REPEATでも同じ)
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

	// 虹色のようなものをつける
	if(isAnamolphic == false)
	{
		result *= texture( lensColorTex, length(vec2(0.5) - flipUV ) / length(vec2(0.5)) );
	}

	
	// HALOを計算して求める
	// HALOベクトル
	// アナモルフィックじゃなかったら処理する
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