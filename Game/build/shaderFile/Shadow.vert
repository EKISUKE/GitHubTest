// Shadow.vert
// シャドウ書き込み用シェーダ

#version 330 core
 
//---- 行列
layout(std140) uniform Matrices {
	mat4	gMatWorld;
	mat4	gViewMatrix;
	mat4	gProjMatrix;
};

uniform mat4	gSkinMatrix[110];

in vec4		position; 
in vec4		blendIndices;
in vec4		blendWeight;

out vec4	vPos;

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;


//===========================================================================
// ４ウェイトスキニングアニメーション
//===========================================================================
void Animation_4Skin(
	in		vec4	position,
	out		vec3	skinPosition,
	in		vec4	blendIndices,
	in		vec4	blendWeight) 
{

	//blendWeight = normalize(blendWeight);

	vec4 indexVector = blendIndices;
	blendWeight.w = 1.0 - dot(blendWeight.xyz, vec3(1.0, 1.0, 1.0));

	skinPosition = vec3(0,0,0);
	
	if(blendWeight.x == 0.0) {
		skinPosition = position.xyz;
		return;
	}


	for( int i=0; i<4; i++ ) {
	
		int index = int(indexVector.x);
		skinPosition += vec3( position * gSkinMatrix[index] * blendWeight.x );
		indexVector = indexVector.yzwx;
		blendWeight = blendWeight.yzwx;
	
		if(blendWeight.x == 0 ) {
			break;
		}
	}
}	


void main(){
	
	//---- アニメーション
	vec3	skinPosition;
	Animation_4Skin(position, skinPosition, blendIndices, blendWeight);

	// 座標変換（ベクトルに行列を乗算）
	mat4 transMat = depthMVP * gMatWorld;
	
	// 座標変換（ベクトルに行列を乗算）
	gl_Position	  = vec4( transMat * vec4(skinPosition, 1) );

	vPos		  = gl_Position;
}