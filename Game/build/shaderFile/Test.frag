// simple.frag

#version 330 core

uniform sampler2D		BaseTexture; 
uniform sampler2D		SpecularMap;
uniform sampler2D		NormalMap;
uniform sampler2DShadow	ShadowMap;

uniform float			texRepeat;		// テクスチャのリピート数
uniform	float			gSplitCount;	// カスケードの分割数

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


// gl_FragColorと同じ
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

//#define	pow	fastPow	// powが見つかったら高速powに置き換え

//-----------------------------------------------------------------------------
// 0〜1クランプ
//-----------------------------------------------------------------------------
float	saturate(float x)
{
	return clamp(x, 0.0, 1.0);
}

////-----------------------------------------------------------------------------
//// フレネル近似
////!	@param	[in]	NdotL			法線とライトベクトルとの内積
////!	@param	[in]	fresnelBias		フレネルバイアス値
////!	@param	[in]	fresnelPower	フレネル強度
////-----------------------------------------------------------------------------
//float	fresnel(float NdotL, float fresnelBias, float fresnelPower)
//{
//	float	facing = (1.0 - NdotL);
//
//	return max( fresnelBias + (1.0 - fresnelBias) * pow(facing, fresnelPower), 0.0);
//}


//---------------------------------------------------------------------------
// フレネル反射(Schlickの近似式)
//! @param	[in]	NdotV	内積結果(cosθ)
//! @param	[in]	f0		基本反射率(フレネルバイアス)
//---------------------------------------------------------------------------
float	fresnel(float NdotV, float f0)
{
	//return f0 + (1 - f0) * pow(1.0 - NdotV, 5.0);
	// ↓[高速化近似]
	return f0 + (1.0 - f0) * exp(-8.35 * NdotV);
}


////-----------------------------------------------------------------------------
//// 鏡面反射光 : Reflectance (Specular)
////　　　  ＼               ／
////	　InDir  ＼  normal ／outDir
////              ＼↑ ／
////  ---------------+----------------
////	正規化Blinn-Phong
////-----------------------------------------------------------------------------
//float BlinnPhong(
// in  vec3  normal,
// in  vec3  halfVector,
// in  float specPow,
// in  vec3  eyeDir
// )
//{
//	// 入射方向
//	//vec3 inDir  = normalize(toFragPosition - lightPos);
//	vec3 inDir  = normalize(shaderInput.lightDir);
//	// 反射方向
//	vec3 outDir = reflect(inDir, normal);
//	// 反射方向と視線方向の内積
//	float NdotL = dot(-outDir, eyeDir);
//	// 反射量計算
//	float reflectancePower = pow( max(dot(normal, halfVector), 0), specPow );
//	// 正規化
//	reflectancePower *= 1.0 / ( (specPow + 2.0) / (2.0 * PI) );
//	// フレネル反射
//	float	fresnelReflection = fresnel(NdotL, 0.5, 4.0);
//	// 反射率計算
//	float	reflectance = reflectancePower * fresnelReflection;
//	return  reflectance;
//}

//-----------------------------------------------------------------------------
// オーレンネイヤー
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
// 正規化Blinn-phong
//-----------------------------------------------------------------------------
float SPECULAR_BlinnPhong(float roughness, float NdotH)
{
	float	bias		  = 0.005;		// 0除算を避けるため
	float	specularPower = 2.0 / saturate(roughness * roughness + bias) - 2.0;
	//specularPower		  = 0.0;			// 0.0だと問題ないが、saturateでは画面が真っ暗になる
	specularPower = saturate(specularPower);
	// 正規化
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
// 材質の色 : Albedoの計算
//-----------------------------------------------------------------------------
vec4 calcAlbedo()
{
	vec4 texColor = texture(BaseTexture, shaderInput.uv * texRepeat);
	vec4 albedo = pow(texColor, vec4(2.2));		// ガンマ補正した材質の色
	vec4 materialColor = vec4(1.0, 1.0, 1.0, 1.0);
	return albedo *= materialColor;				// 材質の色
}

//-----------------------------------------------------------------------------
// Specularの計算
//-----------------------------------------------------------------------------
vec3 calcSpecular()
{
	vec3 specColor     = vec3( texture(SpecularMap, shaderInput.uv * texRepeat) );
	return specColor;				// 材質の色
}

float calcRoughness()
{
	return saturate(1.0 - calcSpecular().r);
}

//-----------------------------------------------------------------------------
// Normalの計算
//-----------------------------------------------------------------------------
vec3 calcNormal()
{
	// * 2.0f - 1.0f はtextureで出てくる0.0f〜1.0fを-1.0f〜1.0fにするため
	vec3 normalColor = normalize(texture(NormalMap, shaderInput.uv * texRepeat).rgb * 2.0 - 1.0 );
	//vec3 normalGamma = pow(normalColor, vec3(2.2));
	return normalize(normalColor);					// 材質の色
}


//-----------------------------------------------------------------------------
// 影影響係数の計算
//-----------------------------------------------------------------------------
float calcShadowCoef()
{
	vec4 skinPosition = vec4(shaderInput.skinPos, 1.0);

	int index = 0;

	float fragCoord = shaderInput.fogCoord;

	// 距離によって使うテクスチャと行列を変更する
	if( fragCoord < farDist[0] ) {
		index = 0;
	}else if ( fragCoord < farDist[1] ) {
		index = 1;
	}else if ( fragCoord < farDist[2] ) {
		index = 2;
	}

	// 光源座標(UV)
	vec4 shadowCoord = vec4( depthBiasMVP[index] * skinPosition );
	// 分割数分で割ることでUVを対応させる
	shadowCoord.x = (shadowCoord.x / gSplitCount);
	// 番号によって参照テクスチャが違うのを対応させる
	shadowCoord.x += (1.0 / gSplitCount) * index;


	// ガウシアン
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
	// 法線 : Normal
	//-------------------------------------------------------------
	vec3		normalColor = calcNormal();
	// 法線
	//vec3		normal	   = normalize( shaderInput.normal );
	vec3		normal	   = normalize( normalColor );
	// 光源の方向
	vec3		lightDir   = normalize( shaderInput.lightDir );

	// 視線の方向
	vec3		eyeDir	   = normalize( shaderInput.eyeDir );

	// ハーフベクトル
	vec3		halfVector = normalize(shaderInput.halfVector);
	

	// 光源の色設定
	vec4		lightColor = vec4(1.0, 1.0, 0.95, 1.0);	// カラー (蛍光灯)
	//vec4		lightColor = vec4(0.5, 0.6, 0.8, 1.0);
	

	//-------------------------------------------------------------
	// 距離減衰
	//-------------------------------------------------------------
	//float		distance            = length( lightPos - toFragPosition );
	float		distance            = length( lightDir );
	// 距離減衰は『距離の２乗に反比例』する
	float		distanceAttenuation = 1.0 / (distance * distance);

lightColor *= 5.0;
//lightColor *= distanceAttenuation;

	

	//-------------------------------------------------------------
	// 拡散反射 : Diffuse
	// 正規化Lambert
	//-------------------------------------------------------------
	float		diffuse      = max( dot(lightDir, normal), 0.0) * (1.0/PI);	// ディフューズ

	//-------------------------------------------------------------
	// 材質の色 : Albedo
	//-------------------------------------------------------------
	vec4		albedo = calcAlbedo();
	//-------------------------------------------------------------
	// 鏡面反射光 : Reflectance (Specular)
	//-------------------------------------------------------------
	float		reflectance;
	if( diffuse > 0.0 ){
		//reflectance = BlinnPhong(normal, halfVector, 9.0f, eyeDir);	
		reflectance = BlinnPhong(normal, halfVector, shaderInput.specularPower, eyeDir);	
	}else{
		reflectance = 0.0;
	}

	//-------------------------------------------------------------
	// 環境光 : Ambient
	//-------------------------------------------------------------
	vec3		ambient = vec3(0.25, 0.25, 0.25);

	//-------------------------------------------------------------
	// 自発光 : Emissive
	//-------------------------------------------------------------
	vec3		emissive = vec3(0.0, 0.0, 0.0);

	//-------------------------------------------------------------
	// 影影響係数 : ShadowCoef
	//-------------------------------------------------------------
	float		shadowCoef = calcShadowCoef();

	
	// エネルギー保存の法則を適用
	{
		// 最初はカラーの色
		vec4	energy = lightColor;
		
		float	reflectanceFactor = 1.0;
		// スペキュラの色(SpecularMap)
		vec3	specularColor = calcSpecular();
		// スペキュラー項
		vec3	specularEnergy = energy.rgb * reflectance * reflectanceFactor * specularColor;
		// エネルギーから引く
		energy.rgb -= specularEnergy; 
		// ディフューズ項
		vec4	diffuseEnergy = energy * albedo * diffuse;

		vec4 calcColor = vec4(1.0);
		
		calcColor = diffuseEnergy + vec4(specularEnergy, 0.0);
		calcColor *= shadowCoef;
	
		fragColor = calcColor;
	}

	// 別の光源
	fragColor += albedo * vec4(ambient, 1.0);

	// 表示結果がリニア空間になるように逆ガンマ補正
	// 逆ガンマ補正は本来は描画の一番最後でポストフィルタで全画面で行なう。
	//fragColor.rgb = pow( fragColor.rgb, vec3(gamma) );


 
	//---- カスケード用の色変更
	
	//float fragCoord = shaderInput.fogCoord;
	//vec3  maskColor = vec3(1.0);
	//
	//// 距離によって使うテクスチャと行列を変更する
	//if( fragCoord < farDist[0] ) {
	//	maskColor = vec3(0.0, 0.0, 0.3);
	//}else if ( fragCoord < farDist[1] ) {
	//	maskColor = vec3(0.0, 0.3, 0.0);
	//}else if ( fragCoord < farDist[2] ) {
	//	maskColor = vec3(0.3, 0.0, 0.0);
	//}
	//fragColor.rgb += maskColor;


	//---- フォグ
	float fogC = abs(shaderInput.fogCoord);
	float fog  = exp(-0.0005 * 0.0005 * fogC * fogC);
	fog  = clamp(fog, 0.0, 1.0);

	fragColor.rgb = mix(vec3(0.8, 0.8, 0.8), fragColor.rgb, fog);

	// halfだとaが1.0を超えると違うとこに書き込むため
	fragColor.a = clamp(fragColor.a, 0.0, 1.0);

}


#else

void main()
{
	//-------------------------------------------------------------
	// 法線 : Normal
	//-------------------------------------------------------------
	vec3		normalColor = calcNormal();
	// 法線
	//vec3		normal	   = normalize( shaderInput.normal );
	vec3		normal	   = normalize( normalColor );
	// 光源の方向
	vec3		lightDir   = normalize( shaderInput.lightDir );

	// 視線の方向
	vec3		eyeDir	   = normalize( shaderInput.eyeDir );

	// ハーフベクトル
	vec3		halfVector = normalize(shaderInput.halfVector);
	

	// 光源の色設定
	vec3		lightColor = vec3(1.0, 1.0, 0.95) * 5.0;	// カラー (蛍光灯)
	
	//float		roughness = calcRoughness();
	float		roughness = 0.1;
	float		metalness = 0.8;

	//-------------------------------------------------------------
	// 距離減衰
	//-------------------------------------------------------------
	//float		distance            = length( lightPos - toFragPosition );
	float		distance            = length( lightDir );
	// 距離減衰は『距離の２乗に反比例』する
	float		distanceAttenuation = 1.0 / (distance * distance);


	//-------------------------------------------------------------
	// 拡散反射 : Diffuse
	// 正規化Lambert
	//-------------------------------------------------------------
	//vec3		diffuse = lightColor * DIFFUSE_Lambert(normal, L);
	vec3		diffuse = lightColor * DIFFUSE_OrenNayar(roughness, normal, eyeDir, lightDir);

	//-------------------------------------------------------------
	// 材質の色 : Albedo
	//-------------------------------------------------------------
	vec4		albedo = calcAlbedo();
	//-------------------------------------------------------------
	// 鏡面反射光 : Reflectance (Specular)
	//-------------------------------------------------------------
	//vec3		specular = lightColor * SPECULAR_BlinnPhong(saturate(roughness+0.004), dot(normal, halfVector));
	vec3		specular = lightColor * SPECULAR_GGX	   (saturate(roughness+0.004), dot(normal, halfVector));

	// 物理ベースでは反射率0.04が基準
	// 金属はスペキュラーに着色がある
	vec3		specularColor = mix(vec3(0.04), albedo.rgb, metalness);

	//-------------------------------------------------------------
	// 環境光 : Ambient
	//-------------------------------------------------------------
	vec3		ambient = vec3(0.25, 0.25, 0.25);

	//-------------------------------------------------------------
	// 自発光 : Emissive
	//-------------------------------------------------------------
	vec3		emissive = vec3(0.0, 0.0, 0.0);

	//-------------------------------------------------------------
	// 影影響係数 : ShadowCoef
	//-------------------------------------------------------------
	float		shadowCoef = calcShadowCoef();


	// フレネル項
	float	NdotV		   = saturate(dot(normal, eyeDir));
	float	fresnelFactor  = fresnel(NdotV, saturate(metalness));
	vec3    diffuseFactor  = vec3(1.0 - fresnelFactor);
	vec3    specularFactor = vec3(fresnelFactor);
	//vec3  specularFactor = vec3(1.0);

	vec3 diffuseEnergy  = albedo.rgb	* diffuse  * diffuseFactor;
	vec3 specularEnergy = specularColor * specular * specularFactor;

	
	vec3 calcColor = diffuseEnergy + specularEnergy;
	
	// 影処理	
	calcColor *= shadowCoef;
	
	fragColor = vec4(calcColor, 1.0);

	// 別の光源
	fragColor += albedo * vec4(ambient, 1.0);
	
	
	// 表示結果がリニア空間になるように逆ガンマ補正
	// 逆ガンマ補正は本来は描画の一番最後でポストフィルタで全画面で行なう。
	//fragColor.rgb = pow( fragColor.rgb, vec3(gamma) );
	
	
	//---- フォグ
	float fogC = abs(shaderInput.fogCoord);
	float fog  = exp(-0.0005 * 0.0005 * fogC * fogC);
	fog  = clamp(fog, 0.0, 1.0);
	
	fragColor.rgb = mix(vec3(0.8, 0.8, 0.8), fragColor.rgb, fog);

	// halfだとaが1.0を超えると違うとこに書き込むため
	fragColor.a = clamp(fragColor.a, 0.0, 1.0);

}
#endif