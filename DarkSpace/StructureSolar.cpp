/*
	StructureSolar.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounStar.h"
#include "GameContext.h"
#include "StructureSolar.h"

//----------------------------------------------------------------------------

const float MAX_STAR_RANGE = 300000.0f;
const float MAX_POWER = 100.0f;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( StructureSolar, StructurePower );
REGISTER_FACTORY_KEY( StructureSolar, 4358662172126761340LL );

BEGIN_PROPERTY_LIST( StructureSolar, StructurePower )
	ADD_PROPERTY( m_Power );
END_PROPERTY_LIST();

StructureSolar::StructureSolar() : m_nUpdateTick( 0 )
{
	m_Power = 0;
}

//----------------------------------------------------------------------------

void StructureSolar::simulate( dword nTick )
{
	if ( nTick >= m_nUpdateTick )
	{
		m_nUpdateTick = nTick + (TICKS_PER_SECOND * 60);

		NounPlanet * pPlanet = planet();
		ASSERT( pPlanet );

		// determine the power output
		m_Power = 0;
		// get my current world position
		Vector3		myPosition( worldPosition() );

		// get all nouns within the specified range
		Array< GameContext::NounCollision > nouns;
		context()->proximityCheck( myPosition, MAX_STAR_RANGE, nouns, CLASS_KEY(NounStar) );

		// enumerate all the stars close to this solar generator
		for(int i=0;i<nouns.size();i++)
		{
			NounStar * pStar = WidgetCast<NounStar>( nouns[i].pNoun );
			if ( pStar != NULL && nouns[i].fDistance < MAX_STAR_RANGE )
			{
				// invert the range, so it's higher the closer the planet
				float fRange = MAX_STAR_RANGE - nouns[i].fDistance;

				Vector3 lightDirection( pStar->worldPosition() - myPosition );
				lightDirection.normalize();

				Vector3 surfaceDirection( myPosition - pPlanet->worldPosition() );
				surfaceDirection.normalize();

				float dot = surfaceDirection | lightDirection;
				if ( dot > 0 )
					m_Power  += ((fRange * MAX_POWER) / MAX_STAR_RANGE) * dot;
			}
		}
	}

	NounStructure::simulate( nTick );
}

//----------------------------------------------------------------------------

int StructureSolar::sortId() const
{
	return 1;
}

int StructureSolar::groupId() const
{
	return 4;
}

int StructureSolar::maxDamage() const
{
	return 52920;
}

int StructureSolar::buildTechnology() const
{
	return 10;
}

int	StructureSolar::buildTime() const
{
	return 50;
}

int	StructureSolar::buildCost() const
{
	return 200;
}

int	StructureSolar::repairRate() const
{
	return 200;
}

Color StructureSolar::color() const
{
	return BLUE;
}

int	StructureSolar::workers() const
{
	return 1;
}

int	StructureSolar::power() const
{
	return m_Power * getTechPercentage();
}

int StructureSolar::technology() const
{
	return 0;
}

//----------------------------------------------------------------------------
//EOF
