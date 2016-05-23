//-----------------------------------------------------------------------------
// File: ShadowMap.fx
// Desc: This vertex/pixel shaders will create a shadow map from a rendered scene.
// (c)2010 Palestar 
//-----------------------------------------------------------------------------

#define USE_SHADOW_MAP_W		0

// globals
float4x4 	mWorld;
float4x4	mLightView;
float4x4	mProj;

//-----------------------------------------------------------------------------
// Vertex Shader: VertShadow
// Desc: Process vertex for the shadow map
//-----------------------------------------------------------------------------
void vs_main( float4 Pos : POSITION,
                 float3 Normal : NORMAL,
                 out float4 oPos : POSITION,
                 out float2 Depth : TEXCOORD0 )
{
    //
    // Compute the projected coordinates
    //
	oPos = mul( Pos, mWorld );			// transform vert from model to view space
    oPos = mul( oPos, mLightView );		// transform from view space to view space of our light
    oPos = mul( oPos, mProj );			// transform from view to projection space

    //
    // Store z and w in our spare texcoord
    //
    Depth.xy = oPos.zw;
}  

//-----------------------------------------------------------------------------
// Pixel Shader: PixShadow
// Desc: Process pixel for the shadow map
//-----------------------------------------------------------------------------
float4 ps_main( float2 Depth : TEXCOORD0 ) : COLOR
{
    //
    // Depth is z / w
    //
#if USE_SHADOW_MAP_W
    return Depth.x / Depth.y;
#else
	return Depth.x;
#endif
}

