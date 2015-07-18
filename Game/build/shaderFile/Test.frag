// simple.frag

#version 330 core

uniform sampler2D		BaseTexture; 
uniform sampler2D		SpecularMap;
uniform sampler2D		NormalMap;
uniform sampler2DShadow	ShadowMap;

uniform float			texRepeat;		// �e�N�X�`���̃��s�[�g��
uniform	float			gSplitCount;	// �J�X�P�[�h�̕�����

uniform mat4			depthBiasMVP[3];
uniform float			farDist[3];

in Data{

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

} shaderInput;


// gl_FragColor�Ɠ���
layout(location = 0) out vec4 fragColor;

#define PI						3.14159265359

#define gamma					1.0/2.0



float	fastPow(float  x, float  y) { return exp(log(x) * y); }
vec2	fastPow(vec2   x, vec2   y) { return exp(log(x) * y); }
vec3	fastPow(vec3   x, vec3   y) { return exp(log(x) * y); }
vec4	fastPow(vec4   x, vec4   y) { return exp(log(x) * y); }


vec2	fastPow(vec2 x, float y) { return exp(log(x) * y); }
vec3	fastPow(vec3 x, float y) { return exp(log(x) * y); }
vec4	fastPow(vec4 x, float y) { return exp(log(x) * y); }

//#define	pow	fastPow	// pow�����������獂��pow�ɒu������

//-----------------------------------------------------------------------------
// 0�`1�N�����v
//-----------------------------------------------------------------------------
float	saturate(float x)
{
	return clamp(x, 0.0, 1.0);
}

////-----------------------------------------------------------------------------
//// �t���l���ߎ�
////!	@param	[in]	NdotL			�@���ƃ��C�g�x�N�g���Ƃ̓���
////!	@param	[in]	fresnelBias		�t���l���o�C�A�X�l
////!	@param	[in]	fresnelPower	�t���l�����x
////-----------------------------------------------------------------------------
//float	fresnel(float NdotL, float fresnelBias, float fresnelPower)
//{
//	float	facing = (1.0 - NdotL);
//
//	return max( fresnelBias + (1.0 - fresnelBias) * pow(facing, fresnelPower), 0.0);
//}


//---------------------------------------------------------------------------
// �t���l������(Schlick�̋ߎ���)
//! @param	[in]	NdotV	���ό���(cos��)
//! @param	[in]	f0		��{���˗�(�t���l���o�C�A�X)
//---------------------------------------------------------------------------
float	fresnel(float NdotV, float f0)
{
	//return f0 + (1 - f0) * pow(1.0 - NdotV, 5.0);
	// ��[�������ߎ�]
	return f0 + (1.0 - f0) * exp(-8.35 * NdotV);
}


////-----------------------------------------------------------------------------
//// ���ʔ��ˌ� : Reflectance (Specular)
////�@�@�@  �_               �^
////	�@InDir  �_  normal �^outDir
////              �_�� �^
////  ---------------+----------------
////	���K��Blinn-Phong
////-----------------------------------------------------------------------------
//float BlinnPhong(
// in  vec3  normal,
// in  vec3  halfVector,
// in  float specPow,
// in  vec3  eyeDir
// )
//{
//	// ���˕���
//	//vec3 inDir  = normalize(toFragPosition - lightPos);
//	vec3 inDir  = normalize(shaderInput.lightDir);
//	// ���˕���
//	vec3 outDir = reflect(inDir, normal);
//	// ���˕����Ǝ��������̓���
//	float NdotL = dot(-outDir, eyeDir);
//	// ���˗ʌv�Z
//	float reflectancePower = pow( max(dot(normal, halfVector), 0), specPow );
//	// ���K��
//	reflectancePower *= 1.0 / ( (specPow + 2.0) / (2.0 * PI) );
//	// �t���l������
//	float	fresnelReflection = fresnel(NdotL, 0.5, 4.0);
//	// ���˗��v�Z
//	float	reflectance = reflectancePower * fresnelReflection;
//	return  reflectance;
//}

//-----------------------------------------------------------------------------
// �I�[�����l�C���[
//-----------------------------------------------------------------------------
float DIFFUSE_OrenNayar(float roughness, vec3 N, vec3 V, vec3 L)
{
	 // calculate intermediary values
	float NdotL = dot(N, L);
	float NdotV = dot(N, V);

    float angleVN = acos(NdotV);
    float angleLN = acos(NdotL);
    
    float alpha = max(angleVN, angleLN);
    float beta  = min(angleVN, angleLN);
	
	float gammaFactor = dot(V - N * NdotV, L - N * NdotL);
    
    float roughnessSquared = roughness * roughness;
    
    // calculate A and B
    float A = 1.0 - 0.5 * (roughnessSquared / (roughnessSquared + 0.57));

    float B = 0.45 * (roughnessSquared / (roughnessSquared + 0.09));
 
    float C = sin(alpha) * tan(beta);
    
    // put it all together
    float L1 = max(0.0, NdotL) * (A + B * max(0.0, gammaFactor) * C);
    
    // get the final color 
    return L1;
}

//-----------------------------------------------------------------------------
// ���K��Blinn-phong
//-----------------------------------------------------------------------------
float SPECULAR_BlinnPhong(float roughness, float NdotH)
{
	float	bias		  = 0.005;		// 0���Z������邽��
	float	specularPower = 2.0 / saturate(roughness * roughness + bias) - 2.0;
	//specularPower		  = 0.0;			// 0.0���Ɩ��Ȃ����Asaturate�ł͉�ʂ��^���ÂɂȂ�
	specularPower = saturate(specularPower);
	// ���K��
	float	factor		  = (specularPower + 2.0) * (1.0 / (2.0 * PI));
	return  pow(max(0, NdotH), specularPower) * factor;
}

float sqr(float x) { return x * x; }

//--------------------------------------------------------------
// GGX (Trowbridge-Reitz) NDF
//--------------------------------------------------------------
float SPECULAR_GGX(float roughness, float NdotH)
{
	float CosSquared = NdotH * NdotH;
	float TanSquared = (1 - CosSquared) / CosSquared;
	return (1.0 / PI) * sqr(roughness / (CosSquared * (roughness*roughness + TanSquared)));
}

//-----------------------------------------------------------------------------
// �ގ��̐F : Albedo�̌v�Z
//-----------------------------------------------------------------------------
vec4 calcAlbedo()
{
	vec4 texColor = texture(BaseTexture, shaderInput.uv * texRepeat);
	vec4 albedo = pow(texColor, vec4(2.2));		// �K���}�␳�����ގ��̐F
	vec4 materialColor = vec4(1.0, 1.0, 1.0, 1.0);
	return albedo *= materialColor;				// �ގ��̐F
}

//-----------------------------------------------------------------------------
// Specular�̌v�Z
//-----------------------------------------------------------------------------
vec3 calcSpecular()
{
	vec3 specColor     = vec3( texture(SpecularMap, shaderInput.uv * texRepeat) );
	return specColor;				// �ގ��̐F
}

float calcRoughness()
{
	return saturate(1.0 - calcSpecular().r);
}

//-----------------------------------------------------------------------------
// Normal�̌v�Z
//-----------------------------------------------------------------------------
vec3 calcNormal()
{
	// * 2.0f - 1.0f ��texture�ŏo�Ă���0.0f�`1.0f��-1.0f�`1.0f�ɂ��邽��
	vec3 normalColor = normalize(texture(NormalMap, shaderInput.uv * texRepeat).rgb * 2.0 - 1.0 );
	//vec3 normalGamma = pow(normalColor, vec3(2.2));
	return normalize(normalColor);					// �ގ��̐F
}


//-----------------------------------------------------------------------------
// �e�e���W���̌v�Z
//-----------------------------------------------------------------------------
float calcShadowCoef()
{
	vec4 skinPosition = vec4(shaderInput.skinPos, 1.0);

	int index = 0;

	float fragCoord = shaderInput.fogCoord;

	// �����ɂ���Ďg���e�N�X�`���ƍs���ύX����
	if( fragCoord < farDist[0] ) {
		index = 0;
	}else if ( fragCoord < farDist[1] ) {
		index = 1;
	}else if ( fragCoord < farDist[2] ) {
		index = 2;
	}

	// �������W(UV)
	vec4 shadowCoord = vec4( depthBiasMVP[index] * skinPosition );
	// ���������Ŋ��邱�Ƃ�UV��Ή�������
	shadowCoord.x = (shadowCoord.x / gSplitCount);
	// �ԍ��ɂ���ĎQ�ƃe�N�X�`�����Ⴄ�̂�Ή�������
	shadowCoord.x += (1.0 / gSplitCount) * index;


	// �K�E�V�A��
	float shadowCoef = textureProj( ShadowMap, shadowCoord ) * 0.25;
	shadowCoef += textureProjOffset( ShadowMap, shadowCoord, ivec2(-1, -1) ) * 0.0625;
	shadowCoef += textureProjOffset( ShadowMap, shadowCoord, ivec2(-1,  0) ) * 0.125;
	shadowCoef += textureProjOffset( ShadowMap, shadowCoord, ivec2(-1, +1) ) * 0.0625;
	shadowCoef += textureProjOffset( ShadowMap, shadowCoord, ivec2( 0, -1) ) * 0.125;
	shadowCoef += textureProjOffset( ShadowMap, shadowCoord, ivec2( 0, +1) ) * 0.125;
	shadowCoef += textureProjOffset( ShadowMap, shadowCoord, ivec2( 1,  0) ) * 0.125;
	shadowCoef += textureProjOffset( ShadowMap, shadowCoord, ivec2( 1,  1) ) * 0.0625;

	return shadowCoef;
}

#if 0
void main (void)
{

	//-------------------------------------------------------------
	// �@�� : Normal
	//-------------------------------------------------------------
	vec3		normalColor = calcNormal();
	// �@��
	//vec3		normal	   = normalize( shaderInput.normal );
	vec3		normal	   = normalize( normalColor );
	// �����̕���
	vec3		lightDir   = normalize( shaderInput.lightDir );

	// �����̕���
	vec3		eyeDir	   = normalize( shaderInput.eyeDir );

	// �n�[�t�x�N�g��
	vec3		halfVector = normalize(shaderInput.halfVector);
	

	// �����̐F�ݒ�
	vec4		lightColor = vec4(1.0, 1.0, 0.95, 1.0);	// �J���[ (�u����)
	//vec4		lightColor = vec4(0.5, 0.6, 0.8, 1.0);
	

	//-------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------
	//float		distance            = length( lightPos - toFragPosition );
	float		distance            = length( lightDir );
	// ���������́w�����̂Q��ɔ����x����
	float		distanceAttenuation = 1.0 / (distance * distance);

lightColor *= 5.0;
//lightColor *= distanceAttenuation;

	

	//-------------------------------------------------------------
	// �g�U���� : Diffuse
	// ���K��Lambert
	//-------------------------------------------------------------
	float		diffuse      = max( dot(lightDir, normal), 0.0) * (1.0/PI);	// �f�B�t���[�Y

	//-------------------------------------------------------------
	// �ގ��̐F : Albedo
	//-------------------------------------------------------------
	vec4		albedo = calcAlbedo();
	//-------------------------------------------------------------
	// ���ʔ��ˌ� : Reflectance (Specular)
	//-------------------------------------------------------------
	float		reflectance;
	if( diffuse > 0.0 ){
		//reflectance = BlinnPhong(normal, halfVector, 9.0f, eyeDir);	
		reflectance = BlinnPhong(normal, halfVector, shaderInput.specularPower, eyeDir);	
	}else{
		reflectance = 0.0;
	}

	//-------------------------------------------------------------
	// ���� : Ambient
	//-------------------------------------------------------------
	vec3		ambient = vec3(0.25, 0.25, 0.25);

	//-------------------------------------------------------------
	// ������ : Emissive
	//-------------------------------------------------------------
	vec3		emissive = vec3(0.0, 0.0, 0.0);

	//-------------------------------------------------------------
	// �e�e���W�� : ShadowCoef
	//-------------------------------------------------------------
	float		shadowCoef = calcShadowCoef();

	
	// �G�l���M�[�ۑ��̖@����K�p
	{
		// �ŏ��̓J���[�̐F
		vec4	energy = lightColor;
		
		float	reflectanceFactor = 1.0;
		// �X�y�L�����̐F(SpecularMap)
		vec3	specularColor = calcSpecular();
		// �X�y�L�����[��
		vec3	specularEnergy = energy.rgb * reflectance * reflectanceFactor * specularColor;
		// �G�l���M�[�������
		energy.rgb -= specularEnergy; 
		// �f�B�t���[�Y��
		vec4	diffuseEnergy = energy * albedo * diffuse;

		vec4 calcColor = vec4(1.0);
		
		calcColor = diffuseEnergy + vec4(specularEnergy, 0.0);
		calcColor *= shadowCoef;
	
		fragColor = calcColor;
	}

	// �ʂ̌���
	fragColor += albedo * vec4(ambient, 1.0);

	// �\�����ʂ����j�A��ԂɂȂ�悤�ɋt�K���}�␳
	// �t�K���}�␳�͖{���͕`��̈�ԍŌ�Ń|�X�g�t�B���^�őS��ʂōs�Ȃ��B
	//fragColor.rgb = pow( fragColor.rgb, vec3(gamma) );


 
	//---- �J�X�P�[�h�p�̐F�ύX
	
	//float fragCoord = shaderInput.fogCoord;
	//vec3  maskColor = vec3(1.0);
	//
	//// �����ɂ���Ďg���e�N�X�`���ƍs���ύX����
	//if( fragCoord < farDist[0] ) {
	//	maskColor = vec3(0.0, 0.0, 0.3);
	//}else if ( fragCoord < farDist[1] ) {
	//	maskColor = vec3(0.0, 0.3, 0.0);
	//}else if ( fragCoord < farDist[2] ) {
	//	maskColor = vec3(0.3, 0.0, 0.0);
	//}
	//fragColor.rgb += maskColor;


	//---- �t�H�O
	float fogC = abs(shaderInput.fogCoord);
	float fog  = exp(-0.0005 * 0.0005 * fogC * fogC);
	fog  = clamp(fog, 0.0, 1.0);

	fragColor.rgb = mix(vec3(0.8, 0.8, 0.8), fragColor.rgb, fog);

	// half����a��1.0�𒴂���ƈႤ�Ƃ��ɏ������ނ���
	fragColor.a = clamp(fragColor.a, 0.0, 1.0);

}


#else

void main()
{
	//-------------------------------------------------------------
	// �@�� : Normal
	//-------------------------------------------------------------
	vec3		normalColor = calcNormal();
	// �@��
	//vec3		normal	   = normalize( shaderInput.normal );
	vec3		normal	   = normalize( normalColor );
	// �����̕���
	vec3		lightDir   = normalize( shaderInput.lightDir );

	// �����̕���
	vec3		eyeDir	   = normalize( shaderInput.eyeDir );

	// �n�[�t�x�N�g��
	vec3		halfVector = normalize(shaderInput.halfVector);
	

	// �����̐F�ݒ�
	vec3		lightColor = vec3(1.0, 1.0, 0.95) * 5.0;	// �J���[ (�u����)
	
	//float		roughness = calcRoughness();
	float		roughness = 0.1;
	float		metalness = 0.8;

	//-------------------------------------------------------------
	// ��������
	//-------------------------------------------------------------
	//float		distance            = length( lightPos - toFragPosition );
	float		distance            = length( lightDir );
	// ���������́w�����̂Q��ɔ����x����
	float		distanceAttenuation = 1.0 / (distance * distance);


	//-------------------------------------------------------------
	// �g�U���� : Diffuse
	// ���K��Lambert
	//-------------------------------------------------------------
	//vec3		diffuse = lightColor * DIFFUSE_Lambert(normal, L);
	vec3		diffuse = lightColor * DIFFUSE_OrenNayar(roughness, normal, eyeDir, lightDir);

	//-------------------------------------------------------------
	// �ގ��̐F : Albedo
	//-------------------------------------------------------------
	vec4		albedo = calcAlbedo();
	//-------------------------------------------------------------
	// ���ʔ��ˌ� : Reflectance (Specular)
	//-------------------------------------------------------------
	//vec3		specular = lightColor * SPECULAR_BlinnPhong(saturate(roughness+0.004), dot(normal, halfVector));
	vec3		specular = lightColor * SPECULAR_GGX	   (saturate(roughness+0.004), dot(normal, halfVector));

	// �����x�[�X�ł͔��˗�0.04���
	// �����̓X�y�L�����[�ɒ��F������
	vec3		specularColor = mix(vec3(0.04), albedo.rgb, metalness);

	//-------------------------------------------------------------
	// ���� : Ambient
	//-------------------------------------------------------------
	vec3		ambient = vec3(0.25, 0.25, 0.25);

	//-------------------------------------------------------------
	// ������ : Emissive
	//-------------------------------------------------------------
	vec3		emissive = vec3(0.0, 0.0, 0.0);

	//-------------------------------------------------------------
	// �e�e���W�� : ShadowCoef
	//-------------------------------------------------------------
	float		shadowCoef = calcShadowCoef();


	// �t���l����
	float	NdotV		   = saturate(dot(normal, eyeDir));
	float	fresnelFactor  = fresnel(NdotV, saturate(metalness));
	vec3    diffuseFactor  = vec3(1.0 - fresnelFactor);
	vec3    specularFactor = vec3(fresnelFactor);
	//vec3  specularFactor = vec3(1.0);

	vec3 diffuseEnergy  = albedo.rgb	* diffuse  * diffuseFactor;
	vec3 specularEnergy = specularColor * specular * specularFactor;

	
	vec3 calcColor = diffuseEnergy + specularEnergy;
	
	// �e����	
	calcColor *= shadowCoef;
	
	fragColor = vec4(calcColor, 1.0);

	// �ʂ̌���
	fragColor += albedo * vec4(ambient, 1.0);
	
	
	// �\�����ʂ����j�A��ԂɂȂ�悤�ɋt�K���}�␳
	// �t�K���}�␳�͖{���͕`��̈�ԍŌ�Ń|�X�g�t�B���^�őS��ʂōs�Ȃ��B
	//fragColor.rgb = pow( fragColor.rgb, vec3(gamma) );
	
	
	//---- �t�H�O
	float fogC = abs(shaderInput.fogCoord);
	float fog  = exp(-0.0005 * 0.0005 * fogC * fogC);
	fog  = clamp(fog, 0.0, 1.0);
	
	fragColor.rgb = mix(vec3(0.8, 0.8, 0.8), fragColor.rgb, fog);

	// half����a��1.0�𒴂���ƈႤ�Ƃ��ɏ������ނ���
	fragColor.a = clamp(fragColor.a, 0.0, 1.0);

}
#endif