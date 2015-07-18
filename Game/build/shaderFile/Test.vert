#version 330 core
//#pragma option(nvidia)

//uniform mat4	gMatWorld;
//uniform mat4	gViewMatrix;
//uniform mat4	gProjMatrix;	

layout(std140)uniform Matrices{
	mat4 gMatWorld;
	mat4 gViewMatrix;
	mat4 gProjMatrix;
};


in vec3		lightPos;
in vec3		eyePos;
in vec4		position;
in vec4		blendIndices;
in vec4		blendWeight;
in vec3		normal;
in vec4		diffuseColor;
in vec3		specularColor;
in vec4		emissiveColor;
in float	specularPower;

in vec2	uv;

uniform mat4	gSkinMatrix[110];


out Data{

	vec3  skinPos;
	vec3  normal;
	vec3  lightDir;
	vec3  eyeDir;
	vec3  halfVector;
	vec2  uv;
	vec4  vPos;
	vec4  diffuseColor;
	vec3  specularColor;
	vec4  emissiveColor;
	float specularPower;
	float fogCoord;
	mat3  normalMatrix;

}shaderOutput;



//===========================================================================
// �S�E�F�C�g�X�L�j���O�A�j���[�V����
//===========================================================================
void Animation_4Skin(
	in		vec4	position,
	out		vec3	skinPosition,
	in		vec4	blendIndices,
	in		vec4	blendWeight,
	inout	vec3	normal
) {

	//blendWeight = normalize(blendWeight);

	vec4 indexVector = blendIndices;
	blendWeight.w = 1.0 - dot(blendWeight.xyz, vec3(1.0, 1.0, 1.0));

	skinPosition = vec3(0,0,0);
	
	vec3	skinNormal = vec3(0,0,0);
	if(blendWeight.x == 0) {
		skinPosition = position.xyz;
		skinNormal   = normal;
		return;
	}

	
	

	for( int i=0; i<4; i++ ) {

		int index = int(indexVector.x);
		skinPosition += vec3( position * gSkinMatrix[index] * blendWeight.x );
		skinNormal   += vec3( vec4(normal, 0) * gSkinMatrix[index] * blendWeight.x );
		indexVector = indexVector.yzwx;
		blendWeight = blendWeight.yzwx;

		if(blendWeight.x == 0 ) {
			break;
		}

	}
	
	normal = normalize(skinNormal);
}	

//-----------------------------------------------------------------------------
// �^���W�F���g�s��̍쐬
//-----------------------------------------------------------------------------
mat3 createTangentMatrix(in vec3 normal, in mat3 n_mat)
{
	
	
	// Z�����̃x�N�g��
	vec3 axisZ	 = vec3(0.0, 0.0, -1.0);
	// ���̃x�N�g���Ɩ@���̊O�ς�X���������߂�(�ڐ�)
	//vec3 tangent = cross(normal, axisZ);
	vec3 tangent;
	vec3 v1 = cross(normal, vec3(0.0,  0.0, -1.0));
	vec3 v2 = cross(normal, vec3(0.0, -1.0,  0.0));

	if( length(v1) > length(v2) )
	{
		tangent = v1;
	}else{
		tangent = v2;
	}

	// ���ʂ��O�Ȃ�X������vec3(0,1,0)
	//if(length(tangent) == 0.0){tangent = vec3(0.0, -1.0, 0.0); }
	
	

	// ���[���h��Ԃɕϊ�
	//vec3 t = vec3( mv * vec4(tangent, 0.0) );
	//vec3 n = vec3( mv * vec4(normalize(normal),0.0) );

	vec3 t = normalize( vec3( n_mat * tangent ) );
	vec3 n = normalize( vec3( n_mat * normal ) );
	
	// �@���ۑ�
	normal = normalize(n);

	// Y���������߂�(�]�@��)
	vec3 b = cross(n, t);
	// �^���W�F���g�s��쐬
	mat3 tangentMat = mat3(t, b, n);
	return tangentMat;
}

void createTBN(out vec3 t, out vec3 b, out vec3 n, in vec3 normal, in mat3 normalMatrix)
{
	vec3 tangent;
	vec3 v1 = cross(normal, vec3(0.0,  0.0, -1.0));
	vec3 v2 = cross(normal, vec3(0.0, -1.0,  0.0));

	if( length(v1) > length(v2) )
	{
		tangent = v1;
	}else{
		tangent = v2;
	}
	n = normalize(normalMatrix * normal);
	t = normalize(normalMatrix * tangent);
	b = cross(n, t);
}


// ���_�V�F�[�_�[ 4Weight�X�L�j���O
void main(void)
{

	//---- �A�j���[�V����
	vec3	skinPosition;
	shaderOutput.normal = normalize(normal);
	Animation_4Skin(position, skinPosition, blendIndices, blendWeight, shaderOutput.normal);

	//---- ���[���h��Ԃɕϊ�
	mat4 toWorldMat = gProjMatrix * gViewMatrix * gMatWorld;
	//mat4 toWorldMat = debugMatrix;
	mat4 modelView  = gViewMatrix * gMatWorld;
	// �m�[�}���}�g���b�N�X
	//mat3 normalMat  = mat3( transpose(inverse(modelView)) );
	mat3 normalMat  = mat3( gMatWorld );

	
	// ���C�g����
	vec3 lightDir   =  normalize(lightPos);
	//vec3 lightDir   =  normalize(lightPos - gl_Position.xyz);
	// ��������
	vec3 vertPos    = vec3( gMatWorld * vec4(skinPosition,1) );

	vec3 eyeDir	    =  normalize(vertPos - eyePos);
	//vec3 eyeDir	    =  normalize(-gl_Position.xyz);
	//vec3 eyeDir	    =  normalize(eyePos - gl_Position.xyz);

	vertPos = vec3( gViewMatrix * gMatWorld * vec4(skinPosition,1) );

	// �n�[�t�x�N�g��
	//vec3 halfVector  = normalize( -vertPos + lightDir );
	vec3 halfVector  = normalize( vertPos + lightDir );
	//vec3 halfVector  = normalize( gl_Position.xyz + lightDir );


	// ���W�ϊ��i�x�N�g���ɍs�����Z�j
	gl_Position			  = vec4( toWorldMat * vec4(skinPosition, 1) );
	shaderOutput.vPos	  = gl_Position;
	shaderOutput.fogCoord = gl_Position.z;

	//---- �^���W�F���g��Ԃɕϊ�
#if 1
	// �^���W�F���g�s��
	mat3 tangentMat		  = createTangentMatrix(shaderOutput.normal, normalMat);
	mat3 worldToTangent	  = inverse(tangentMat);
	//mat3 worldToTangent = tangentMat;
	// ���C�g����
	shaderOutput.lightDir = normalize(worldToTangent * lightDir);
	// ��������
	shaderOutput.eyeDir	  = normalize(worldToTangent * eyeDir);
	// �n�[�t�x�N�^�[
	//shaderOutput.halfVector = normalize(shaderOutput.eyeDir + shaderOutput.lightDir);
	shaderOutput.halfVector = normalize(worldToTangent * halfVector);

#else 
	shaderOutput.normal			 = normalize( gMatWorld * shaderOutput.normal );
	shaderOutput.lightDir		 = normalize( lightDir );
	shaderOutput.eyeDir			 = normalize( eyeDir );
	shaderOutput.halfVector		 = normalize( halfVector );
	shaderOutput.normalMatrix	 = normalMat;
#endif 

	//---- �t���O�����g�V�F�[�_�ɏ��]��
	shaderOutput.uv				= uv;
	shaderOutput.diffuseColor	= diffuseColor;
	shaderOutput.specularColor  = specularColor;
	shaderOutput.emissiveColor	= emissiveColor;
	shaderOutput.specularPower	= specularPower;
	shaderOutput.skinPos		= skinPosition;
//	shaderOutput.shadowCoord	= vec4( depthBiasMVP * vec4(skinPosition, 1) );
}