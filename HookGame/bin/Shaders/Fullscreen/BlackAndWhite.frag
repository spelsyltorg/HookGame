
uniform sampler2D albedo; 
uniform float intensity; 

void main() 
{ 
	vec4 texColor = texture2D(albedo, gl_TexCoord[0].xy); 
	texColor.a = 1.0;
	float allColors = texColor.r + texColor.g + texColor.b; 
	allColors = allColors / 3.0;
	gl_FragColor = vec4(allColors, allColors, allColors, 1.0); 
}