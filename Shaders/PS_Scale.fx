//-----------------------------------------------------------------------------
// File: PS_Scale.fx
// Desc: This will scale the pixels by a scalar, used by the blur effect to darken the previous frame.
// (c)2010 Palestar 
//-----------------------------------------------------------------------------

// globals
sampler2D 	sScene : register(s0);
float		fScale = 0.95f;

float4 Scale( in float2 t : TEXCOORD0 ) : COLOR0
{
    return tex2D( sScene, t ) * fScale, 1.0f;
}

float4 ps_main( in float2 t : TEXCOORD0 ) : COLOR0
{
	return Scale( t );
}
