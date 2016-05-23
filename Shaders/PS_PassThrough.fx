// PS_PassThrough.fx

sampler2D sDiffuse : register(s0);

struct PS_INPUT
{
    float3 t		: TEXCOORD0;
    float4 c		: COLOR0;
};
    
struct PS_OUTPUT
{
    float4 c		: COLOR0;
};

PS_OUTPUT ps_main( in PS_INPUT p )
{
	PS_OUTPUT output;
    output.c = p.c * tex2D( sDiffuse, p.t );
	return output;
}
