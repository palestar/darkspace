/*
	Trail.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Display/PrimitiveTriangleStrip.h"
#include "Display/PrimitiveSetTransform.h"
#include "Constants.h"
#include "Trail.h"

//----------------------------------------------------------------------------

float			Trail::sm_fTrailScalar = 1.0f;
float			Trail::sm_fTrailFarClip = 500.0f;
float			Trail::sm_fTrailFadeDistance = 100.0f;	

//----------------------------------------------------------------------------

Constant	TRAIL_UPDATE_DOT( "TRAIL_UPDATE_DOT", 0.0001f );
Constant	DEFAULT_TRAIL_RATE( "DEFAULT_TRAIL_RATE", 1.0f );
Constant	DEFAULT_TRAIL_LIFE( "DEFAULT_TRAIL_LIFE", 4.0f );

//----------------------------------------------------------------------------


Trail::Trail()
{
	m_TrailUpdate = DEFAULT_TRAIL_RATE;
	m_TrailRadius = 0.0f;
	m_TrailLife = DEFAULT_TRAIL_LIFE * TICKS_PER_SECOND;
	m_TrailRate = DEFAULT_TRAIL_RATE * TICKS_PER_SECOND;
}

Trail::Trail( const Trail & a_Copy ) :
	m_TrailMaterial( a_Copy.m_TrailMaterial ),
	m_TrailLife( a_Copy.m_TrailLife ),
	m_TrailRate( a_Copy.m_TrailRate ),
	m_TrailUpdate( a_Copy.m_TrailUpdate ),
	m_TrailRadius( a_Copy.m_TrailRadius ),
	m_Trail( a_Copy.m_Trail )
{}

//----------------------------------------------------------------------------

void Trail::updateTrail( const Vector3 & worldPosition )
{
	const int MAX_LIFE = Max<int>( sm_fTrailScalar * m_TrailLife, 1 );

	Vector3 vLine[ 2 ];
	int nSegments = 0;

	// update the segments
	SegmentList::iterator iSegment = m_Trail.begin();
	while( iSegment != m_Trail.end() )
	{
		Segment & segment = *iSegment;
		vLine[ 0 ] = vLine[ 1 ];
		vLine[ 1 ] = segment.position;
		nSegments++;

		if ( --segment.life <= 0 )
			m_Trail.erase( iSegment++ );
		else
			++iSegment;
	}

	// Check if we need to force an update due to the angle of the line exceeding TRAIL_UPDAYE_DOT
	bool bForceUpdate = false;
	if ( nSegments >= 2 && worldPosition != vLine[1] )
	{
		Vector3 vDelta[ 2 ];
		vDelta[ 0 ] = vLine[1] - vLine[0];		
		vDelta[ 0 ].normalize();
		vDelta[ 1 ] = worldPosition - vLine[1];	
		vDelta[ 1 ].normalize();

		float fDot = vDelta[ 0 ] | vDelta[ 1 ];
		if ( fabs(fDot - 1.0f) > TRAIL_UPDATE_DOT )
			bForceUpdate = true;
	}

	// add new segments to the trail
	m_TrailUpdate++;
	if ( bForceUpdate || m_TrailUpdate > m_TrailRate )
	{
		m_TrailUpdate = 0;

		Segment newSegment;
		newSegment.life = MAX_LIFE;
		newSegment.position = worldPosition;

		m_Trail.push_back( newSegment );

		// update the trail radius
		m_TrailRadius = 0.0f;
		for( SegmentList::iterator iSegment = m_Trail.begin(); iSegment != m_Trail.end(); ++iSegment )
		{
			Segment & segment = *iSegment;

			float distance = (newSegment.position - segment.position).magnitude2();
			if ( distance > m_TrailRadius )
				m_TrailRadius = distance;
		}

		m_TrailRadius = sqrt( m_TrailRadius );
	}
}

void Trail::renderTrail( RenderContext &context, const Matrix33 & frame, 
	const Vector3 & position, const Vector3 & vHead )
{
	if (! m_TrailMaterial.valid() )
		return;
	// simple visiblity check
	Vector3 positionVS( context.worldToView( position ) );
	if (! context.sphereVisible( positionVS, m_TrailRadius ) )
		return;
	if ( positionVS.z > sm_fTrailFarClip )
		return;		// trail is beyond the far clipping plane...

	int nTrailSize = m_Trail.size();
	int nVerts = (nTrailSize + 1) * 2;
	if ( nVerts < 4 )
		return;

	float fBaseAlpha = Clamp<float>( 1.0f - ((positionVS.z - (sm_fTrailFarClip - sm_fTrailFadeDistance)) / sm_fTrailFadeDistance), 0.0f, 1.0f );

	DisplayDevice * pDisplay = context.display();
	ASSERT( pDisplay );

	// create the triangle strip primitive
	PrimitiveTriangleStripDL::Ref pYStrip = PrimitiveTriangleStripDL::create( pDisplay, nVerts );
	PrimitiveTriangleStripDL::Ref pXStrip = PrimitiveTriangleStripDL::create( pDisplay, nVerts );

	// lock the primitives
	VertexL * pVertY = pYStrip->lock();
	VertexL * pVertX = pXStrip->lock();
	

	const Vector3 vNormal( 0, 0, 0 );
	const Matrix33 mTrail( context.viewToWorld( Matrix33::IDENTITY ) );
	const Vector3 vX( mTrail.i * m_TrailMaterial->height() );
	const Vector3 vY( mTrail.j * m_TrailMaterial->height() );

	const float MAX_LIFE = Max<float>( sm_fTrailScalar * m_TrailLife, 1.0f );
	const int	BASE_ALPHA = context.alpha() * fBaseAlpha * 255;
	const Color DIFFUSE( 255,255,255, BASE_ALPHA );

	pVertY->position = vHead + vY;
	pVertY->normal = vNormal;
	pVertY->diffuse = DIFFUSE;
	pVertY->u = 0.0f;
	pVertY->v = 0.0f;
	pVertY++;

	pVertY->position = vHead - vY;
	pVertY->normal = vNormal;
	pVertY->diffuse = DIFFUSE;
	pVertY->u = 0.0f;
	pVertY->v = 1.0f;
	pVertY++;

	pVertX->position = vHead + vX;
	pVertX->normal = vNormal;
	pVertX->diffuse = DIFFUSE;
	pVertX->u = 0.0f;
	pVertX->v = 0.0f;
	pVertX++;

	pVertX->position = vHead - vX;
	pVertX->normal = vNormal;
	pVertX->diffuse = DIFFUSE;
	pVertX->u = 0.0f;
	pVertX->v = 1.0f;
	pVertX++;

	// don't alpha fade the first segment as it actually creates a flicker in the head of the trail
	bool bFirstSegment = true;

	// start at the end, since thats actually the beginning of the trail
	SegmentList::reverse_iterator iSegment = m_Trail.rbegin();
	while( iSegment != m_Trail.rend() )
	{
		Segment & segment = *iSegment;
		++iSegment;

		// get the UV coordinates for this segment
		const float fT = float( segment.life ) / MAX_LIFE;
		const float fU = Clamp( (1.0f - fT), 0.05f, 1.0f );
		const Color DIFFUSE( 255, 255, 255, bFirstSegment ? BASE_ALPHA : (fT * BASE_ALPHA)  );
		
		pVertY->position = segment.position + vY;
		pVertY->normal = vNormal;
		pVertY->diffuse = DIFFUSE;
		pVertY->u = fU;
		pVertY->v = 0.0f;
		pVertY++;

		pVertY->position = segment.position - vY;
		pVertY->normal = vNormal;
		pVertY->diffuse = DIFFUSE;
		pVertY->u = fU;
		pVertY->v = 1.0f;
		pVertY++;

		pVertX->position = segment.position + vX;
		pVertX->normal = vNormal;
		pVertX->diffuse = DIFFUSE;
		pVertX->u = fU;
		pVertX->v = 0.0f;
		pVertX++;

		pVertX->position = segment.position - vX;
		pVertX->normal = vNormal;
		pVertX->diffuse = DIFFUSE;
		pVertX->u = fU;
		pVertX->v = 1.0f;
		pVertX++;

		bFirstSegment = false;
	}

	pYStrip->unlock();
	pXStrip->unlock();
	
	// set the material
	context.push( m_TrailMaterial );
	// set the transform line strip from worldspace to viewspace
	context.pushIdentity();
	context.push( pYStrip );
	context.push( pXStrip );
}

void Trail::renderProjectileTrail( RenderContext &context, const Matrix33 & frame, 
						const Vector3 & position, const Vector3 & vHead )
{
	if (! m_TrailMaterial.valid() )
		return;
	// simple visiblity check
	Vector3 positionVS( context.worldToView( position ) );
	if (! context.sphereVisible( positionVS, m_TrailRadius ) )
		return;

	int nTrailSize = m_Trail.size();
	int nVerts = (nTrailSize + 1) * 2;
	if ( nVerts < 4 )
		return;

	DisplayDevice * pDisplay = context.display();
	ASSERT( pDisplay );

	// create the triangle strip primitive
	PrimitiveTriangleStripDL::Ref pYStrip = PrimitiveTriangleStripDL::create( pDisplay, nVerts );
	// lock the primitives
	VertexL * pVert = pYStrip->lock();

	const Vector3 vNormal( 0, 0, 0 );
	const Matrix33 mTrail( context.viewToWorld( Matrix33::IDENTITY ) );
	const Vector3 vX( mTrail.i * m_TrailMaterial->height() );
	const dword BASE_ALPHA = context.alpha() * 255;
	const Color DIFFUSE( 255,255,255, BASE_ALPHA );

	pVert->position = vHead + vX;
	pVert->normal = vNormal;
	pVert->diffuse = DIFFUSE;
	pVert->u = 0.0f;
	pVert->v = 0.0f;
	pVert++;

	pVert->position = vHead - vX;
	pVert->normal = vNormal;
	pVert->diffuse = DIFFUSE;
	pVert->u = 0.0f;
	pVert->v = 1.0f;
	pVert++;

	// calculate the maximum life of a segment, this is used to calculate the UV coordinates for the segments
	const float MAX_LIFE = Max<float>( sm_fTrailScalar * m_TrailLife, 1.0f );

	// start at the end, since thats actually the beginning of the trail
	SegmentList::reverse_iterator iSegment = m_Trail.rbegin();
	while( iSegment != m_Trail.rend() )
	{
		Segment & segment = *iSegment;
		++iSegment;

		// get the UV coordinates for this segment
		const float fT = float( segment.life ) / MAX_LIFE;
		const float fU = Clamp( 1.0f - fT, 0.05f, 0.95f );
		const Color DIFFUSE( 255, 255, 255, fT * BASE_ALPHA );

		pVert->position = segment.position + vX;
		pVert->normal = vNormal;
		pVert->diffuse = DIFFUSE;
		pVert->u = fU;
		pVert->v = 0.0f;
		pVert++;

		pVert->position = segment.position - vX;
		pVert->normal = vNormal;
		pVert->diffuse = DIFFUSE;
		pVert->u = fU;
		pVert->v = 1.0f;
		pVert++;
	}

	pYStrip->unlock();

	context.push( m_TrailMaterial );
	context.pushIdentity();
	context.push( pYStrip );
}

void Trail::offsetTrail( const Vector3 & add )
{
	for( SegmentList::iterator iSegment = m_Trail.begin();
		iSegment != m_Trail.end(); ++iSegment )
	{
		Segment & segment = *iSegment;
		segment.position += add;
	}
}

//----------------------------------------------------------------------------
//EOF
