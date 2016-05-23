//-----------------------------------------------------------------------------
// File: Default.fx
// Desc: Medusa Uber Shader - This shader replaced the old fixed-function pipeline but supporting direction and point lights and adding shadow map support.
// this shader is a single light shader, so geometry needs to be rendered for each light if you want more than onelight.
// (c)2010 Palestar 
//-----------------------------------------------------------------------------

#ifdef USE_SHADOW_MAP_W
#define SHADOW_EPSILON			0.001f
#else
#define SHADOW_EPSILON			5.0f
#endif

// light types, this should match the D3DLIGHTTYPE enum..
#define LIGHT_POINT				1
#define LIGHT_SPOT				2
#define LIGHT_DIRECTIONAL		3

// GLOBALS
float4x4 	mWorld;				// model space to world transform
float4x4	mView;				// world to view transform
float4x4	mProj;				// view to projection transform
float3		vCameraPos;			// position of the camera in world space

float4x4	mLightView;			// world to light view space transform
float4x4	mLightProj;			// view to light proj space transform

// the diffuse texture
bool		bEnableDiffuse = false;
sampler2D	sDiffuse : register(s0);
// the lightmap texture
bool		bEnableLightMap = false;
sampler2D	sLightMap : register(s1);
// the bumpmap texture
bool		bEnableBumpMap = false;
sampler2D	sBumpMap : register(s2);
float		fBumpDepth = 1.20f;

// shadow map
bool		bEnableShadowMap = false;
sampler2D 	sShadowMap : register(s7);
float2		szShadowMap = float2( 2048, 2048 );
float		fShadowDistance = 850.0f;

// Material properties
float4		vMatDiffuse =		// modulated with the diffuse light
								float4( 1.0f, 1.0f, 1.0f, 1.0f );
float4		vMatSpecular =		// modulated with the specular light
								float4( 1.0f, 1.0f, 1.0f, 1.0f );
float		fMatSpecularPower =	// specular strength
								0.0f;		
float4		vMatAmbient =		// modulated with ambient light
								float4( 1.0f, 1.0f, 1.0f, 1.0f );
float4		vMatEmissive =		// light emitted by the material..
								float4( 0.0f, 0.0f, 0.0f, 1.0f );

bool		bEnableAmbient = true;
// global ambient light added to all pixels
float4		vGlobalAmbient =	float4( 0.1f, 0.1f, 0.1f, 1.0f );

// Ambient light, all pixels will have atleast this much light.. this also controls the alpha value from the pixel
// shader if a diffuse texture is not provided.
int			nLightType = LIGHT_DIRECTIONAL;
float4		vLightDiffuse = 	// diffuse lightxx
								float4( 1.0f, 1.0f, 1.0f, 1.0f );
float4		vLightSpecular =	// specular light
								float4( 0.0f, 0.0f, 0.0f, 0.0f );
float3		vLightAmbient =		// ambient light 
								float3( 1.0f, 1.0f, 1.0f );
										
float3		vLightPosition =	// position of our light in world space
								float3( 0.0f, 0.0f, 0.0f );
// direction of light in world space for directional/spot lights								
float3		vLightDirection =	float3( 0.0f, 0.0f, 1.0f );
// calculated from the vertex position to the vLightPosition
float3		vLightDelta;
float		fLightDistance;
// attenuation of the point/spot light, D = Distance, 1, D, D^2
float3		vAttenuation =		float3( 1.0f, 1000.0f, 1000.0f * 1000.0f );
// 
float3		vSpot;				// cos(theta/2), cos(phi/2), falloff 


//---------------------------------------------------------------------------------------------------

struct VS_INPUT
{
    float4 vPosition    : POSITION0;		// The incoming, model space, coordinate
    float3 vNormal		: NORMAL0;			// vertex normal
    float2 vUV			: TEXCOORD0;		// texture UV
};
    
struct VS_OUTPUT
{
    float4 vPosition	: POSITION0;        // projected position of vertex
    float2 vUV			: TEXCOORD0;		// texture UV
    float3 vNormal		: TEXCOORD1;		
    float3 vTangent		: TEXCOORD2;
    float3 vBinormal	: TEXCOORD3;
    float4 vWorldPos	: TEXCOORD4;		// world position of vertex
    float4 vLightPos	: TEXCOORD5;		// shadow map projection
};

VS_OUTPUT vs_main30( in VS_INPUT v )
{
	VS_OUTPUT rv;
    rv.vWorldPos	= mul( v.vPosition, mWorld );	// to world space
	rv.vPosition	= mul( rv.vWorldPos, mView );	// to view space
	rv.vPosition	= mul( rv.vPosition, mProj );	// to screen space
    rv.vUV			= v.vUV;						// pass texture coordinate through
    rv.vNormal		= mul( v.vNormal, (float3x3)mWorld );

	// TODO: pre-calculate the tangent & binormal vectors in C++ rather than on the fly in the graphics hardware
	if ( bEnableBumpMap )
	{    
		float3 c1 = cross( v.vNormal, float3( 0.0, 0.0, 1.0 ) );
		float3 c2 = cross( v.vNormal, float3( 0.0, 1.0, 0.0 ) );
		if ( length( c1 ) > length( c2 ) )
			rv.vTangent = normalize( c1 );
		else
			rv.vTangent = normalize( c2 );
		rv.vBinormal = normalize( cross( v.vNormal, rv.vTangent ) );
		
		rv.vTangent = mul( rv.vTangent, (float3x3)mWorld );
		rv.vBinormal = mul( rv.vBinormal, (float3x3)mWorld );
	}
		
    if ( bEnableShadowMap )
    {
		rv.vLightPos	= mul( rv.vWorldPos, mLightView );
		rv.vLightPos	= mul( rv.vLightPos, mLightProj );
	}
	
	return rv;
}

//---------------------------------------------------------------------------------------------------

float calculateLightAmount( in VS_OUTPUT input )
{
	float fLightAmount = 4.0f;
	float2 vShadowUV = ((input.vLightPos.xy * 0.5f) / input.vLightPos.w) + float2( 0.5f, 0.5f );
	vShadowUV.y = 1.0f - vShadowUV.y;

	float2 vTexel = 1.0f / szShadowMap;
	
	float y = -0.5f;
	while( y <= 0.5f )
	{
		float x = -0.5f;
		while( x <= 0.5f )
		{
			float fSMZ = tex2D( sShadowMap, vShadowUV + (float2(x,y) * vTexel) );
			if ( fSMZ > 1.0f )
			{
				fSMZ += SHADOW_EPSILON;
				if ( fSMZ < input.vLightPos.z )
				{
					float fDistance = input.vLightPos.z - fSMZ;
					float fDistanceScale = 1.0f - (fDistance / fShadowDistance);
					if ( fDistanceScale > 0.0f )
						fLightAmount -= fDistanceScale;
				}
			}
			
			x += 1.0f;
		}
		y += 1.0f;
	}
	
	fLightAmount /= 4.0f;
		
	return saturate( fLightAmount );
}

float4 applyDiffuseSpecular( in float fLightAmount, in VS_OUTPUT input )
{
	// Phong relfection is ambient + light-diffuse + spec highlights.
	// I = Ia*ka*Oda + fatt*Ip[kd*Od(N.L) + ks(R.V)^n]
	// Ref: http://www.whisqu.se/per/docs/graphics8.htm

	float4 vPixel = 0.0f;
	if ( nLightType == LIGHT_POINT ) 
	{
		vLightDelta = input.vWorldPos.xyz - vLightPosition.xyz;
		vLightDirection = normalize( vLightDelta );
		fLightDistance = length( vLightDelta );
		fLightAmount *= 1.0f / (vAttenuation.x + (vAttenuation.y * fLightDistance));
	}

	float fDiffuseLight = fLightAmount;
	fDiffuseLight *= dot(input.vNormal, -vLightDirection);
	
	if ( bEnableDiffuse )
	{
		float4 vTexel = tex2D( sDiffuse, input.vUV );
		vPixel.xyz += (vTexel.xyz * vMatDiffuse.xyz * vLightDiffuse.xyz) * fDiffuseLight;
		vPixel.xyz += (vTexel.xyz * vMatEmissive.xyz);
		vPixel.w = vTexel.w * vMatDiffuse.w;		// modulate our alpha from the diffuse texture with our material alpha
	}
	else
	{
		vPixel.xyz += (vMatDiffuse.xyz * vLightDiffuse.xyz) * fDiffuseLight;
		vPixel.xyz += vMatEmissive.xyz;
		vPixel.w = vMatDiffuse.w;
	}
	
	if ( bEnableAmbient )
	{
		float fDiffuseLightInv = (1.0f - fDiffuseLight) * 0.5f;
		vPixel.xyz += vGlobalAmbient * fDiffuseLightInv;
	}
	
	if ( fMatSpecularPower > 0.0f )
	{
		// Using Blinn half angle modification for perofrmance over correctness
		float3 h = normalize(normalize(vCameraPos - input.vWorldPos) - vLightDirection);		
		float fSpecularLight = fLightAmount;
		fSpecularLight *= pow(saturate(dot(h, input.vNormal)), fMatSpecularPower);		

		if ( fSpecularLight > 0.0f )
			vPixel.xyz += (vMatSpecular.xyz * vLightSpecular.xyz) * fSpecularLight;
	}

	return vPixel;
}

float3 getBumpedNormal( in VS_OUTPUT input )
{
	float3 vBump = fBumpDepth * (tex2D( sBumpMap, input.vUV) - (0.5, 0.5, 0.5));
	float3 vBumpNormal = input.vNormal + (vBump.x * input.vTangent + vBump.y * input.vBinormal);
	vBumpNormal = normalize( vBumpNormal );
	
	return vBumpNormal;
}

//---------------------------------------------------------------------------------------------------

float4 ps_main30( in VS_OUTPUT input ) : COLOR0
{
	// always start with the ambient color, including it's alpha value..
	float fLightAmount = 1.0f;
	if ( bEnableShadowMap )
		fLightAmount = calculateLightAmount( input );
	if ( bEnableBumpMap )
		input.vNormal = getBumpedNormal( input );
		
	float4 vPixel = applyDiffuseSpecular( fLightAmount, input );
	if ( bEnableLightMap )
		vPixel.xyz += tex2D( sLightMap, input.vUV ).xyz * 2.0f;
	
	return saturate( vPixel );
}

//---------------------------------------------------------------------------------------------------
// EOF




