// LightAdaptation.frag
// 明順応 / 暗順応
#version 330

// 吐き出しの色
layout(location = 0) out vec4 fragColor;
// テクスチャ
uniform sampler2D BaseTexture;
// テクスチャ
uniform sampler2D BaseTexture2;
// ミップマップ番号
uniform float	  gMipLod;

// バーテックスから受け取る値
in Data{
	vec4	diffuseColor;
	vec2	uv;
}shaderInput;

void main()
{
	// BaseTexture  : 集計テクスチャ ( gMipLodでmip指定されている, LuminanceReduction の結果 )
	// BaseTexture2 : 順応テクスチャ ( 1x1のテクスチャ )
	vec2	info = textureLod(BaseTexture, shaderInput.uv, gMipLod).rg;

	// 現在の順応テクスチャ
	vec2	adaptation = texture(BaseTexture2, shaderInput.uv).rg;

	// どのくらい変化するか
	adaptation += (info - adaptation) * 0.02;

	// 結果の値をテクスチャとして保存
	fragColor = vec4(adaptation, 1, 1);
}