// Texture.vert
 
varying vec4 position;
varying vec3 cam;
varying vec3 normal;
varying vec3 lightvec;

void main(void)
{
	position	= gl_ModelViewMatrix  * gl_Vertex;
	//gl_Position = gl_ProjectionMatrix * position;
	//gl_Position	  = ftransform();
	cam			= normalize(position.xyz);
	normal		= normalize(gl_NormalMatrix * gl_Normal);
	lightvec	= normalize(gl_LightSource[0].position.xyz - position);
	
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

	
	gl_Position	  = ftransform();
}