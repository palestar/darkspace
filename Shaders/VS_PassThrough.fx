// VS_PassThrough.fx

// globals
float4x4 	mWorld;
float4x4	mView;
float4x4	mProj;

struct VS_INPUT
{
    float3 position     : POSITION;			// The incoming, model space, coordinate
    float3 normal		: NORMAL;
    float2 uv			: TEXCOORD0;
};
    
struct VS_OUTPUT
{
    float4 position		: POSITION;         // Projection space vertex position
    float2 uv			: TEXCOORD0;
    float4 diffuse		: COLOR;            // Colour to be interpolated    
};

VS_OUTPUT vs_main( in VS_INPUT v )
{
	VS_OUTPUT output;
    output.position		= mul( float4( v.position, 1.0f ), mWorld );		// transform to world space
	output.position		= mul( output.position, mView );					// transform to view space
	output.position		= mul( output.position, mProj );					// transform to screen space
    output.uv			= v.uv;
    output.diffuse		= float4(1,1,1,1);
	return output;
}
