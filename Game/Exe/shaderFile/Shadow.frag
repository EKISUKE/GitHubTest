#version 330 core
 
// Ouput data
layout(location = 0) out vec4 fragmentdepth;

in vec4	vPos;

void main(){


	float depth = vPos.z / vPos.w;
	depth = depth * 0.5 + 0.5;

	float moment1 = depth;
	float moment2 = depth * depth;

	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25 *(dx*dx+dy*dy);


	//fragmentdepth = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);

	fragmentdepth = vec4(moment1, moment2, 0.0, 1.0 );

	//fragmentdepth.rb = blendWeightForColor.rb;

	//fragmentdepth.ra = vec2(1.0);
}