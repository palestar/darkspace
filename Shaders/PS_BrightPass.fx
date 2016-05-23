//-----------------------------------------------------------------------------
// File: PS_BrightPass.fx
// Desc: This pixel shader darkens dark colors and lightens light colors.
// (c)2010 Palestar 
//-----------------------------------------------------------------------------

// Globals
sampler2D 	sDiffuse : register(s0);
float 		fLuminance = 0.06f;

// Constants
static const float fMiddleGray = 0.18f;
static const float fWhiteCutoff = 0.8f;

//-----------------------------------------------------------------------------
// Pixel Shader: BrightPass
// Desc: Perform a high-pass filter on the source texture
//-----------------------------------------------------------------------------
float4 BrightPass( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float3 ColorOut = tex2D( sDiffuse, Tex );

    ColorOut *= fMiddleGray / ( fLuminance + 0.001f );
    ColorOut *= ( 1.0f + ( ColorOut / ( fWhiteCutoff * fWhiteCutoff ) ) );
    ColorOut -= 5.0f;

    ColorOut = max( ColorOut, 0.0f );

    ColorOut /= ( 10.0f + ColorOut );

    return float4( ColorOut, 1.0f );
}

float4 ps_main20( in float2 Tex : TEXCOORD0 ) : COLOR0
{
	return BrightPass( Tex );
}

