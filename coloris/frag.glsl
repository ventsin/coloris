// biliniar interpolation fragment shader

uniform vec4 color0;
uniform vec4 color1;
uniform vec4 color2;
uniform vec4 color3;

varying vec2 uv;

vec4 mixf(in vec4 v1, in vec4 v2, in float a)
{
    vec4 result;
    result.x = v1.x * v1.x * (1 - a) + v2.x * v2.x * a;
    result.y = v1.y * v1.y  * (1 - a) + v2.y * v2.y * a;
    result.z = v1.z * v1.z  * (1 - a) + v2.z * v2.z * a;
    result.w = v1.w * v1.w  * (1 - a) + v2.w * v2.w * a;

    result.x = sqrt(result.x);
    result.y = sqrt(result.y);
    result.z = sqrt(result.z);
    result.w = sqrt(result.w);
   
    return result;
}

void main()
{
	//remember to swtich colorC with colorD eventually
	vec4 color = mix(mix(color0, color1, uv.x), mix(color2, color3, uv.x), uv.y);
	gl_FragColor = color;
}