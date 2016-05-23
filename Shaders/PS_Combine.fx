//-----------------------------------------------------------------------------
// File: PS_Combine.fx
// Desc: Used to combine two textures, used by the bloom & blur effects.
// (c)2010 Palestar 
//-----------------------------------------------------------------------------

// globals
sampler2D 	sScene : register(s0);
sampler2D 	sAdd : register(s1);
float		fScale = 1.0f;

float4 Combine( in float2 t : TEXCOORD0 ) : COLOR0
{
    float3 ColorOrig = tex2D( sScene, t );
    ColorOrig += tex2D( sAdd, t ) * fScale;

    return float4( ColorOrig, 1.0f );
}

float4 ps_main( in float2 t : TEXCOORD0 ) : COLOR0
{
	return Combine( t );
}
