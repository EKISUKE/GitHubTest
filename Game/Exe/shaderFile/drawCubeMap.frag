// drawCubeMap.frag
// �L���[�u�}�b�v�`��p(SkyBox�Ȃ�)
#version 330

in vec3 texCoord;

uniform float	intensity;

layout(location = 0) out vec4 fragColor;
uniform samplerCube cubeMap;


void main(void)
{
	fragColor	   = texture(cubeMap, texCoord);

	// ���ɕϊ����Ă݂�

	/*fragColor.r = min( fragColor.r * pow(2, 255 * (fragColor.a - 0.5)), 2000.0 );
	fragColor.g = min( fragColor.g * pow(2, 255 * (fragColor.a - 0.5)), 2000.0 );
	fragColor.b = min( fragColor.b * pow(2, 255 * (fragColor.a - 0.5)), 2000.0 );*/

	//fragColor.rgb *= vec3(intensity);
	//fragColor.a   *= vec3(intensity);
	//fragColor *= vec4(intensity);


	//fragColor *= vec4(1000);
}