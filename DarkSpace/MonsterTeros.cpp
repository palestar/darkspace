/*
	MonsterTerosScout.cpp
	(c)2008 Palestar Inc, Jeff Everett
*/


#include "Debug/Profile.h"
#include "MonsterTeros.h"
#include "NounPlanet.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( MonsterTeros, NounShip );

BEGIN_PROPERTY_LIST( MonsterTeros, NounShip )
END_PROPERTY_LIST();

MonsterTeros::MonsterTeros() : m_nLogicTick( 0 )
{}

//----------------------------------------------------------------------------

const int UPDATE_LOGIC_RATE = TICKS_PER_SECOND * 10;

void MonsterTeros::simulate( dword nTick )
{	
	// logic updates server side and only if monster is not controlled by a player
	if ( isLocal() && nTick >= m_nLogicTick )
	{
		m_nLogicTick = nTick + UPDATE_LOGIC_RATE;

		PROFILE_START( "MonsterTerosScout::simulate( dword nTick ) - updating logic" );

		if ( order() == NOORDER || (rand() % 100 ) < 10 )
		{
			// make a list of possible places to follow
			Array< Noun * > follow;
			Array<Noun *> planets;

			// update the contact list
			Array< GameContext::NounCollision > contacts;
			// grab the ships near us
			context()->proximityCheck( worldPosition(), 100000.0f, contacts, CLASS_KEY(NounShip) );
			// grab the planets near us
			context()->proximityCheck( worldPosition(), 100000.0f, contacts, CLASS_KEY(NounPlanet) );

			for(int i=0;i<contacts.size();i++)
			{
				Noun * pContact = contacts[ i ].pNoun;
				if ( WidgetCast<MonsterTeros>( pContact ) )
					continue;						// don't follow self or others of our kind

				if ( WidgetCast<NounShip>( pContact ) )
					follow.push( pContact );		// follow other ships

				if( WidgetCast<NounPlanet>( pContact ) ) // add planets
					planets.push(pContact);
			}


			if ( follow.size() > 0 )
			{
				// attack a ship
				verbOrder( ATTACK, follow[ rand() % follow.size() ] );
			}
			else if(planets.size() > 0)
			{
				// nothing to attack so just find a planet to go to
				verbOrder( MOVE,planets[rand() % planets.size()] );
			}
		}
		PROFILE_END();
	}
	
	NounShip::simulate( nTick );
}


//----------------------------------------------------------------------------
//EOF
