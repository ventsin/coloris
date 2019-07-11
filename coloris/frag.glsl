// biliniar interpolation fragment shader

uniform vec4 color0;
uniform vec4 color1;
uniform vec4 color2;
uniform vec4 color3;

varying vec2 uv;

void main()
{
	vec4 color = mix(mix(color0, color1, uv.x), mix(color2, color3, uv.x), uv.y);
	color.a = 1.0;
	gl_FragColor = color;
}