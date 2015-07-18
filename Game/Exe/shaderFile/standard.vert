// standard.vert
#version 330

layout(std140)uniform Matrices{
	mat4 gMatWorld;
	mat4 gViewMatrix;
	mat4 gProjMatrix;
};

in vec4		position;
in vec4		diffuseColor;
in vec3		specularColor;
in vec4		emissiveColor;
in float	specularPower;
in vec2		uv;

out Data {
	vec4	diffuseColor;
	vec2	uv;
}shaderOutput;

void main()
{
	// ç¿ïWïœä∑
	mat4 toWorldMat		= gProjMatrix * gViewMatrix * gMatWorld;
	gl_Position			= vec4( toWorldMat * position );

	//---- Output
	shaderOutput.uv			  = uv;
	shaderOutput.diffuseColor = diffuseColor;
}