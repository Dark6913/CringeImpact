#version 110

uniform vec2 position;
uniform vec2 size;
uniform vec4 radiuses;
uniform vec2 resolution;

void main()
{ 
	vec2 coord = vec2(gl_FragCoord.x, resolution.y - gl_FragCoord.y);
	vec4 pixel = vec4(gl_Color.rgb, 0);

	if ((coord.x >= position.x && coord.x <= position.x + size.x &&
		coord.y >= position.y && coord.y <= position.y + size.y) &&
		!((coord.x >= position.x && coord.x <= position.x + radiuses[0] && coord.y >= position.y && coord.y <= position.y + radiuses[0] &&
		length(coord - position - radiuses[0]) > radiuses[0]) ||
		(coord.x >= position.x + size.x - radiuses[1] && coord.x <= position.x + size.x && coord.y >= position.y && coord.y <= position.y + radiuses[1] &&
		length(coord - position - vec2(size.x, 0) + vec2(radiuses[1], -radiuses[1])) > radiuses[1]) ||
		(coord.x >= position.x && coord.x <= position.x + radiuses[3] && coord.y >= position.y + size.y - radiuses[3] && coord.y <= position.y + size.y &&
		length(coord - position - vec2(0, size.y) - vec2(radiuses[3], -radiuses[3])) > radiuses[3]) ||
		(coord.x >= position.x + size.x - radiuses[2] && coord.x <= position.x + size.x && coord.y >= position.y + size.y - radiuses[2] && coord.y <= position.y + size.y &&
		length(coord - position - size + radiuses[2]) > radiuses[2])))
	{
		pixel.a = 1.0;
	}

    gl_FragColor =  pixel;
}