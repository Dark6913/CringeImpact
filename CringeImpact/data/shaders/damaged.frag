#version 110

uniform sampler2D texture;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	float brightness = (pixel.r + pixel.b + pixel.g) / 3.0;
	pixel = vec4(brightness, 0, 0, pixel.a);

	gl_FragColor = pixel;
}