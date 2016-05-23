/*
	MonsterEnergy.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Profile.h"
#include "MonsterEnergy.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( MonsterEnergy, NounShip );
REGISTER_FACTORY_KEY( MonsterEnergy, 4254148423148532207LL );

BEGIN_PROPERTY_LIST( MonsterEnergy, NounShip )
END_PROPERTY_LIST();

MonsterEnergy::MonsterEnergy() : m_nLogicTick( 0 )
{}

//----------------------------------------------------------------------------

const int UPDATE_LOGIC_RATE = TICKS_PER_SECOND * 10;

void MonsterEnergy::simulate( dword nTick )
{
	// logic updates server side and only if monster is not controlled by a player
	if ( isLocal() && nTick >= m_nLogicTick )
	{
		m_nLogicTick = nTick + UPDATE_LOGIC_RATE;

		//PROFILE_START( "MonsterEnergy::simulate( dword nTick ) - logic" );
		if ( order() == NOORDER || (rand() % 100 ) < 10 )
		{
			// make a list of possible places to follow
			Array< Noun * > follow;

			// update the contact list
			Array< GameContext::NounCollision > contacts;
			context()->proximityCheck( worldPosition(), 100000.0f, contacts, CLASS_KEY(NounShip) );

			for(int i=0;i<contacts.size();i++)
			{
				Noun * pContact = contacts[ i ].pNoun;
				if ( WidgetCast<MonsterEnergy>( pContact ) )
					continue;						// don't follow self or other energy entities
				if ( WidgetCast<NounShip>( pContact ) )
					follow.push( pContact );		// follow other ships
			}

			if ( follow.size() > 0 )
				verbOrder( MOVE, follow[ rand() % follow.size() ] );
		}
		//PROFILE_END();
	}

	NounShip::simulate( nTick );
}

void MonsterEnergy::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	// 1/4 damage if any kind of energy weapon is used!
	if ( type & DAMAGE_ENERGY )
		damage = -(damage / 4);

	NounShip::inflictDamage( nWhen, pFrom, damage, type, direction );
}

//----------------------------------------------------------------------------
//EOF
