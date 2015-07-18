// Texture.frag




uniform sampler2D texture;

varying vec4 position;
varying vec3 cam;
varying vec3 normal;
varying vec3 lightvec;

void main (void)
{

	vec4 resultColor;

	vec4 color     = texture2DProj(texture, gl_TexCoord[0]);

	vec3 light	  = normalize((gl_LightSource[0].position * position.w - gl_LightSource[0].position.w * position).xyz);
	
	vec3 fnormal = normalize(normal);
	
	// �g�U���˗������߂�(�����o�[�g)
	float diffuse = max(dot(light, fnormal), 0.0);
	
	//if( diffuse <= 0 ){
	//	resultColor = 	gl_LightSource[0].diffuse * diffuse * color
	//				+	gl_LightSource[0].ambient * color;
	//
	//}else{
	//
	//	//---- ���ʔ��˂̎���
	//	// ���̍��W�̋t�x�N�g���𐳋K�����āA�����x�N�g�����쐬
	//	vec3 view	  = normalize(position.xyz);
	//
	//	vec3 reflection = normalize(light-view);
    //    // ���ʔ��˗������߂�
	//	float specular = pow(max(dot(normal, reflection), 0.0), gl_FrontMaterial.shininess);
    //    resultColor = gl_LightSource[0].diffuse  * diffuse * color
	//				+ gl_FrontLightProduct[0].specular	   * specular
	//				+ gl_LightSource[0].ambient			   * color;
	//
	//}

	//---- ���ʔ��˂̎���
	// ���̍��W�̋t�x�N�g���𐳋K�����āA�����x�N�g�����쐬
	vec3 view	  = -normalize(position.xyz);
	// ���ԃx�N�g�����쐬
	vec3 halfway  =  normalize(light + view);
	// ���ʔ��˗������߂�
	float specular = pow(max(dot(normal, halfway), 0.0), gl_FrontMaterial.shininess);
	
	//gl_FragColor  = gl_LightSource[0].diffuse  * gl_FrontMaterial.diffuse  * diffuse
	//			  + gl_LightSource[0].specular * gl_FrontMaterial.specular * specular
	//			  + gl_LightSource[0].ambient  * gl_FrontMaterial.ambient;
	//
	
	//gl_FragColor = gl_FrontLightProduct[0].diffuse  * diffuse
	//			 + gl_FrontLightProduct[0].specular * specular
	//			 + gl_FrontLightProduct[0].ambient;
	
	
	
	//gl_FragColor   = color * gl_LightSource[0].diffuse * diffuse
	//			   + gl_FrontLightProduct[0].specular  * specular
	//			   + color * gl_LightSource[0].ambient;

	gl_FragColor   = gl_LightSource[0].diffuse * diffuse * color
					+ gl_FrontLightProduct[0].specular
					+	gl_LightSource[0].ambient * color;
	


	

	//gl_FragColor = resultColor;
}