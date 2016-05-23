// PS_VertBlur.fx

// Globals
sampler2D 	sDiffuse : register(s0);
int 		nDiffuseWidth = 320;
int 		nDiffuseHeight = 180;
float 		fVertScale = 1.5f;

static const int VERT_BLUR_SIZE = 13;

float2 PixelKernel[VERT_BLUR_SIZE] =
{
    { 0, -6 },
    { 0, -5 },
    { 0, -4 },
    { 0, -3 },
    { 0, -2 },
    { 0, -1 },
    { 0,  0 },
    { 0,  1 },
    { 0,  2 },
    { 0,  3 },
    { 0,  4 },
    { 0,  5 },
    { 0,  6 },
};

static const float BlurWeights[VERT_BLUR_SIZE] = 
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};

//-----------------------------------------------------------------------------
// Pixel Shader: HorizontalBlur
// Desc: Blurs the image vertically
//-----------------------------------------------------------------------------
float4 VertBlur( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Color = 0;
	float2 DiffuseSize = { 1.0f / nDiffuseWidth, 1.0f / nDiffuseHeight };
	
    for (int i = 0; i < VERT_BLUR_SIZE; i++)
    {    
		float2 fOffset = PixelKernel[i] * DiffuseSize;
        Color += tex2D( sDiffuse, (Tex + fOffset) ) * BlurWeights[i] * fVertScale;
    }

    return Color;
}

float4 ps_main( in float2 Tex : TEXCOORD0 ) : COLOR0
{
	return VertBlur( Tex );
}

