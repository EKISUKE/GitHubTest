// debugTexture.vert
// デバッグ用シェーダー(デバッグしたい時用に自由に使えるシェーダー)
#version 330

in vec4		position;
in vec4		color;
in vec2		uv;

out Data {
	vec4	diffuseColor;
	vec2	uv;
}shaderOutput;

void main()
{
	gl_Position				  = position;
	//---- Output
	shaderOutput.uv			  = uv;
	shaderOutput.diffuseColor = color;
}