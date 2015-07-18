// AddLensEffects.frag
// レンズに汚れ、スターバーストを追加する

//#version 420
#version 330

// レンズフレア画像
uniform sampler2D BaseTexture;
// レンズ汚れ
uniform sampler2D lensDirtTex;
// スターバースト
uniform sampler2D lensStarTex;
// カメラの回転角度行列
uniform mat4 lensStarMatrix;	
// ミップマップ番号
uniform float	  gMipLod;



in Data{
	vec4	diffuseColor;
	vec2	uv;
}shaderInput;

// 吐き出しの色
layout(location = 0) out vec4 fragColor;


void main() { 

	vec2 texCoord = shaderInput.uv;

	// レンズ汚れ色取得
	vec4 lensMod = texture(lensDirtTex, texCoord);
	// スターバーストのUV
	vec2 lensStarTexcoord = (lensStarMatrix * vec4(texCoord, 1.0, 1.0)).xy;
	// レンズ汚れ+スターバーストの色
	//lensMod  = texture(lensStarTex, lensStarTexcoord);
	lensMod  += texture(lensStarTex, lensStarTexcoord);

	fragColor = textureLod(BaseTexture, texCoord, gMipLod) * 10.0f;

	fragColor  *= lensMod;
}