// standard.frag
// 標準描画
#version 420

// 吐き出しの色
layout(location = 0) out vec4 fragColor;
// テクスチャ
uniform sampler2D BaseTexture;
// テクスチャ
uniform sampler2D BaseTexture2;

// バーテックスから受け取る値
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