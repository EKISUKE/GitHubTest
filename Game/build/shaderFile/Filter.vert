// Filter.vert
// ポストエフェクトフィルター
#version 420

//#extension GL_ARB_shading_language_include : require
//
//#include "common/utils.glsl"	// フルパスならインクルードできる


in vec4		position;
in vec4		color;
in vec4		emissiveColor;
in vec2		uv;

out Data {
	vec4	diffuseColor;
	vec4	emissiveColor;
	vec2	uv;
}shaderOutput;

void main()
{
	gl_Position				   = position;
	//---- Output			   
	shaderOutput.uv			   = uv;
	shaderOutput.diffuseColor  = color;
	shaderOutput.emissiveColor = emissiveColor;

	//shaderOutput.emissiveColor = vec4(1.0, 1.0, 1.0, 1.0);
}