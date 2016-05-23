//-----------------------------------------------------------------------------
// File: ShadowScene.fx
// Desc: This shader creates the shadow map texture that is drawn over the top of the scene to apply shadows
// (c)2010 Palestar 
//-----------------------------------------------------------------------------

#define SHADOW_EPSILON			1.0f
#define ENABLE_SHADOW_LERP		0
#define DARKNESS_SCALE			0.5f

sampler2D 	sShadowMap : register(s0);

float4x4 	mWorld;				// model space to world transform
float4x4	mView;				// world to view transform
float4x4	mProj;				// view to projection transform
float4x4	mLightView;			// view transform for light
float4x4	mLightProj;			// projection for light

float2		szShadowMap 		= { 1024, 1024 };
float		fSceneScale			= 1.0f; // / 50000.0f;
float		fShadowScale		= 0.5f;
float		fShadowDistance		= 1000.0f;		// how far do shadows project..

//---------------------------------------------------------------------------------------------------

void vs_main( float4 iPos : POSITION,
                out float4 oPos : POSITION,
                out float4 vPosLight : TEXCOORD0 )
{
	float4 vWorldPos = mul( iPos, mWorld );				// to world space
 	oPos = mul( vWorldPos, mView );						// to view space
    oPos = mul( oPos, mProj );							// to screen space
	oPos.z -= 0.01f;									// push towards camera just a little, so the shadow geometry is rendered in front of the actual geometry so it doesn't z-fight
	float4 vLightView = mul( vWorldPos, mLightView );	// to light view space
	vPosLight = mul( vLightView, mLightProj );			// to light screen space
}

//---------------------------------------------------------------------------------------------------

float4 ps_main( float4 vPosLight : TEXCOORD0 ) : COLOR
{
	// to texture space
	vPosLight.xy *= 0.5;
	vPosLight.xy /= vPosLight.w;
	vPosLight.xy += float2( 0.5, 0.5 );
	vPosLight.y = 1.0f - vPosLight.y;

	float fDarkAmount = 0.0f;
	
	float fZ = vPosLight.z; // / vPosLight.w; 
#if !ENABLE_SHADOW_LERP
	// Transform from screen space to texture space.
	float fSMZ = tex2D( sShadowMap, vPosLight.xy );
	if ( fSMZ > 1.0f )
	{
		fSMZ += SHADOW_EPSILON;
		if ( fSMZ < fZ )
		{
			float fDistance = fZ - fSMZ;
			float fDistanceScale = 1.0f - (fDistance / fShadowDistance);
			if ( fDistanceScale > 0.0f )
				fDarkAmount = fDistanceScale;
		}
	}
#else
	// transform to texel space
	float2 szTexel = 1.0f / szShadowMap;
	float2 fPos = frac( vPosLight.xy * szShadowMap );
	// offset our sample position by 0.5 a texel..
	vPosLight.xy = vPosLight.xy - (szTexel * 0.5f);
	
	float s[4];
	s[0] = tex2D( sShadowMap, vPosLight.xy );
	s[1] = tex2D( sShadowMap, vPosLight.xy + float2(szTexel.x, 0) );
	s[2] = tex2D( sShadowMap, vPosLight.xy + float2(0, szTexel.y) );
	s[3] = tex2D( sShadowMap, vPosLight.xy + szTexel );
	
	float r1 = lerp( s[0], s[1], fPos.x );
	float r2 = lerp( s[2], s[3], fPos.x );
	float fSMZ = lerp( r2, r1, fPos.y );
	if ( fSMZ > 1.0f )
	{
		fSMZ += SHADOW_EPSILON;
		if ( fSMZ < fZ )
		{
			float fDistance = fZ - fSMZ;
			float fDistanceScale = 1.0f - (fDistance / fShadowDistance);
			if ( fDistanceScale > 0.0f )
				fDarkAmount = fDistanceScale;
		}
	}
#endif		
	return float4(0,0,0,fDarkAmount * fShadowScale * DARKNESS_SCALE);
}

