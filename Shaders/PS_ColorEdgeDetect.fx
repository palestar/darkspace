// PS_ColorEdgeDetect.fx

sampler2D DiffuseTexture;

float2 PixelKernel[4] =
{
    { 0,  1},
    { 1,  0},
    { 0, -1},
    {-1,  0}
};

float2 TexelKernel[4]
<
	string ConvertPixelsToTexels = "PixelKernel";
>;

//-----------------------------------------------------------------------------
// Pixel Shader: PostProcessPS
// Desc: Performs post-processing effect that detects and highlights edges.
//-----------------------------------------------------------------------------
float3 LuminanceConv = { 0.2125f, 0.7154f, 0.0721f };

float4 ColorEdges( in float2 Tex : TEXCOORD0 ) : COLOR0
{
    float4 Orig = tex2D( DiffuseTexture, Tex );

    float4 Sum = 0;

    for( int i = 0; i < 4; i++ )
        Sum += ( abs( Orig - tex2D( DiffuseTexture, Tex + TexelKernel[i] ) ) - 0.5f ) * 1.2 + 0.5f;

    return saturate( dot( LuminanceConv, Sum ) ) * 5;
}

float4 ps_main( in float2 Tex : TEXCOORD0 ) : COLOR0
{
	return ColorEdges( Tex );
}
