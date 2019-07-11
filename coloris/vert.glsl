// simple vertex shader

varying vec2 uv;

void main()
{
	uv = gl_MultiTexCoord0.xy;
	gl_Position    = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor  = gl_Color;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
}