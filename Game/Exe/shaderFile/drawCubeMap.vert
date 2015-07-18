// drawCubeMap.vert
// キューブマップ描画用(SkyBoxなど)

#version 330

in vec4		position; 

out vec3	texCoord;

//---- 行列
layout(std140) uniform Matrices {
	mat4	gMatWorld;
	mat4	gViewMatrix;
	mat4	gProjMatrix;
};

void main(void)
{
	// 座標変換
	mat4 toWorldMat = gProjMatrix * gViewMatrix * gMatWorld;
	gl_Position		= vec4( toWorldMat * position );
	texCoord		= position.xyz;
}