/*
	NounStar.cpp
	(c)1999 PaleStar, Richard Lyle
*/


#include "Debug/Assert.h"
#include "resource.h"
#include "NounStar.h"
#include "NounShip.h"
#include "NounPlanet.h"
#include "VerbDestroyShip.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

const dword		STAR_UPDATE = TICKS_PER_SECOND * 5;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounStar, NounBody );
REGISTER_FACTORY_KEY( NounStar, 4145260826753541950LL );

BEGIN_PROPERTY_LIST( NounStar, NounBody )
	ADD_TRANSMIT_PROPERTY( m_Name );
END_PROPERTY_LIST();

NounStar::NounStar() : m_nCheckFactionTick( 0 )
{
	m_nNodeFlags |= NF_AMBIENT|NF_NOSHADOW;
}

//----------------------------------------------------------------------------

void NounStar::render( RenderContext & context, const Matrix33 & frame, const Vector3 & position )
{
	bool occlusion = false;
	if ( Noun::context() != NULL )
	{
		Vector3 positionVS( context.worldToView( position ) );
		if ( positionVS.z > 1.0f )
		{
			// test for occlusion from the viewer position, if blocked don't render the flare
			Vector3 destination( position );
			Vector3 origin( context.position() );
			Vector3 delta( destination - origin );
			float distance = delta.magnitude();

			if ( distance < 25000.0f )
			{
				// check for occlusion
				Array< GameContext::NounCollision > check;
				if ( Noun::context()->proximityCheck( origin, distance, check, CLASS_KEY(NounPlanet) ) )
				{
					for(int i=0;i<check.size();i++)
					{
						Noun * pCheck = check[i].pNoun;
						// planets are the only object that will occlude the lens flare from a star
						if ( WidgetCast<NounPlanet>( pCheck ) )
						{
							// object is close, final check
							Vector3 intersect;
							if ( pCheck->intersect( origin, delta, intersect ) )
							{
								occlusion = true;
								break;
							}
						}
					}
				}
			}
			else 
				occlusion = true;	// star is too far away!
		}
		else
			occlusion = true;	// star is behind the viewer
	}

	// set the render bits
	dword previousBits = context.bits();
	context.setBits( occlusion ? 0 : RENDER_FLARE );
	// render the star
	NounBody::render( context, frame, position );
	// restore the previous render bits
	context.setBits( previousBits );
}

//----------------------------------------------------------------------------

CharString NounStar::displayName(bool a_bAllowTags) const
{
	// TODO: we may want to incorporate some sort of config.ini flag so we
	// can select whether we want this display scheme for singular servers
	CharString sName = name();
	if (serverName() != "")
		sName += CharString().format("\n%s", serverName());

	return sName;
}

NounType NounStar::nounType() const
{
	return TYPE_LANDMARK;
}

bool NounStar::collidable() const
{
	return true;
}

bool NounStar::canOrbit() const
{
	return true;
}

float NounStar::baseSignature() const
{
	return radius() * 1000.0f;
}

//----------------------------------------------------------------------------

const int CHECK_FACTION_TIME = (TICKS_PER_SECOND * 35);

void NounStar::simulate( dword nTick )
{
	if ( isServer() )
	{
		// check which faction/fleet controls this most planets in this star system
		if ( nTick >= m_nCheckFactionTick )
		{
			GameContext * pContext = (GameContext *)context();
			ASSERT( pContext );

			m_nCheckFactionTick = nTick + CHECK_FACTION_TIME;

			// find the top faction in this star system
			Hash< dword, int > factionPlanets;

			int topFactionId = 0;
			int topFactionPlanets = 0;

			for(int i=0;i<m_Planets.size();i++)
			{
				NounPlanet * pPlanet = m_Planets[ i ];

				int nFactionId = pPlanet->factionId();
				if (! factionPlanets.find( nFactionId ).valid() )
					factionPlanets[ nFactionId ] = 0;

				int & nCount = factionPlanets[ nFactionId ];
				if ( ++nCount > topFactionPlanets )
				{
					topFactionId = nFactionId;
					topFactionPlanets = nCount;
				}
			}

			if ( topFactionId > 0 )
			{
				int myTeamId = pContext->factionTeam( topFactionId );
				if ( myTeamId != teamId() )
				{
					const GameContext::Team & team = pContext->team( myTeamId );

					// post global chat messages
					globalChat( CharString().format( "<color;ffffff>NEWS: %s is now under %s control.</color>", name(), team.name.cstr() ) );
					// set the teamId on this star
					setTeamId( myTeamId );
					// force this noun to get resent to all clients..
					resync();
				}
			}
		}
	}

	NounBody::simulate( nTick );
}

void NounStar::collision( Noun * pCollide, const Vector3 & intersect )
{
	if ( WidgetCast<NounShip>( pCollide ) )
	{
		NounShip * pShip = (NounShip *)pCollide;
		if ( pShip->isLocal() && !pShip->isDestroyed() )
		{
			if ( (worldPosition() - pShip->worldPosition()).magnitude() < radius() )
				Verb::Ref( new VerbDestroyShip( pShip, this, true ) );		// ship to close, destroy them with no mercy
		}
	}
}

bool NounStar::postInitialize()
{
	if (! Noun::postInitialize() )
		return false;

	if ( m_NounContext.valid() )
	{
		// pre-load assets..
		m_NounContext->scene( false );

		// adjust hull of stars to be 1/8 the scene size..
		m_Hull = m_Hull *  0.125f;
		m_fRadius = m_Hull.radius();
		m_fRadius2 = m_fRadius * m_fRadius;
	}

	return true;
}

bool NounStar::canShipCollide() const
{
	return true;
}

void NounStar::addPlanet( NounPlanet * pPlanet )
{
	// only add this planet to the array if it's not already registered
	m_Planets.pushUnique( pPlanet );
}

void NounStar::removePlanet( NounPlanet * pPlanet )
{
	// remove this planet from the array
	m_Planets.removeSearch( pPlanet );
}

//----------------------------------------------------------------------------
// EOF
