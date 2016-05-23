/*
	MonsterCrystal.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Profile.h"
#include "MonsterCrystal.h"
#include "NounPlanet.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( MonsterCrystal, NounShip );
REGISTER_FACTORY_KEY( MonsterCrystal, 4254148416883685662LL );

BEGIN_PROPERTY_LIST( MonsterCrystal, NounShip )
END_PROPERTY_LIST();

MonsterCrystal::MonsterCrystal() : m_nLogicTick( 0 )
{}

//----------------------------------------------------------------------------

const int UPDATE_LOGIC_RATE = TICKS_PER_SECOND * 10;

void MonsterCrystal::simulate( dword nTick )
{
	// logic updates server side and only if monster is not controlled by a player
	if ( isLocal() && nTick >= m_nLogicTick )
	{
		m_nLogicTick = nTick + UPDATE_LOGIC_RATE;

		//PROFILE_START( "MonsterCrystal::simulate( dword nTick ) - updating logic" );
		if ( order() == ATTACK )
		{
			if ( WidgetCast<NounPlanet>( orderTarget() ) )
			{
				// check for the planet population at zero
				if ( ((NounPlanet *)orderTarget())->population() <= 0 )
					verbOrder( NOORDER, NULL );
			}
			else
				verbOrder( NOORDER, NULL );
		}

		if ( order() != ATTACK )
		{
			// make a list of possible places to attack
			Array< Noun * > planets;
			NounPlanet *	pAttack = NULL;
			int				population = 0;

			// update the contact list
			Array< GameContext::NounCollision > contacts;
			context()->proximityCheck( worldPosition(), 100000.0f, contacts, CLASS_KEY(NounPlanet) );

			for(int i=0;i<contacts.size();i++)
			{
				Noun * pContact = contacts[i].pNoun;
				if ( WidgetCast<NounPlanet>( pContact ) )
				{
					planets.push( pContact );
					if ( ((NounPlanet *)pContact)->population() > population )
					{
						pAttack = (NounPlanet *)pContact;
						population = ((NounPlanet *)pContact)->population();
					}
				}
			}

			if ( pAttack != NULL )
				verbOrder( ATTACK, pAttack );
			else if ( planets.size() > 0 )
				verbOrder( MOVE, planets[ rand() % planets.size() ] );
		}
		//PROFILE_END();
	}

	NounShip::simulate( nTick );
}


//----------------------------------------------------------------------------
//EOF
