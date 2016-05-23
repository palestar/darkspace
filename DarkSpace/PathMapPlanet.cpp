/*
	PathMapPlanet.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Standard/Constant.h"
#include "Render3D/NodeTransform.h"
#include "PathMapPlanet.h"

//----------------------------------------------------------------------------

Constant PLANET_HEX_SIZE( "PLANET_HEX_SIZE", 6.0f );
Constant PLANET_MIN_THETA( "PLANET_MIN_THETA", 4.0f );
Constant PLANET_MAX_THETA( "PLANET_MAX_THETA", 12.0f );
Constant PLANET_MIN_PHI( "PLANET_MIN_PHI", 8.0f );
Constant PLANET_MAX_PHI( "PLANET_MAX_PHI", 20.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( PathMapPlanet, PathMap );
REGISTER_FACTORY_KEY( PathMapPlanet, 4552676905201229260LL );

BEGIN_PROPERTY_LIST( PathMapPlanet, PathMap )
END_PROPERTY_LIST();

void PathMapPlanet::onAttached()
{
	release();

	BaseNode * pParent = parent();
	if ( pParent != NULL )
	{
		// get the world hull of our parent
		BoxHull hull( ::NodeTransform::worldHull( pParent ) );

		float radius = Max( Max( hull.max.x - hull.min.x, 
			hull.max.y - hull.min.y), 
			hull.max.z - hull.min.z);
		radius = radius * 0.5f;									// convert from diameter to radius

		// how many hexes down the Y axis of the planet (theta)
		int thetaHexes = Clamp<float>( (radius * PI2) / PLANET_HEX_SIZE, PLANET_MIN_THETA, PLANET_MAX_THETA );
		float thetaStep = PI2 / thetaHexes;
		float theta = -PI4;

		int phiHexes = Clamp<float>( ((2.0f * PI * radius) * cos( theta )) / PLANET_HEX_SIZE, PLANET_MIN_PHI, PLANET_MAX_PHI );
		float phiStep = (PI * 2.0f) / phiHexes;

		int hexOffset = 0;
		for(int i=0;i<thetaHexes;i++)
		{
			float cosTheta = cos( theta );
			float sinTheta = sin( theta );
			float phi = -PI; 

			for(int j=0;j<phiHexes;j++)
			{
				PathMap::Node newNode;
				newNode.id = hexOffset + j;
				newNode.position = Vector3( sin(phi) * cosTheta, sinTheta, cos(phi) * cosTheta ) * radius;
				newNode.links.allocate( 4 );
				newNode.links[ 0 ] = (i > 0) ? (hexOffset - phiHexes + j) : -1;						// up
				newNode.links[ 1 ] = (j < (phiHexes - 1)) ? (hexOffset + j + 1) : hexOffset;		// right
				newNode.links[ 2 ] = (i < (thetaHexes - 1)) ? (hexOffset + phiHexes + j) : -1;		// down
				newNode.links[ 3 ] = (j > 0) ? (hexOffset + j - 1) : (hexOffset + phiHexes - 1);	// left

				addNode( newNode );

				phi += phiStep;
			}

			hexOffset += phiHexes;
			theta += thetaStep;
		}
	}
}

//----------------------------------------------------------------------------
//EOF
