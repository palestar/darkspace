/*
	MonsterGafin.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Profile.h"
#include "MonsterGafin.h"
#include "NounPlanet.h"
#include "NounNebula.h"
#include "VerbCommandShip.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( MonsterGafin, NounShip );
REGISTER_FACTORY_KEY( MonsterGafin, 4254148411458712735LL );

MonsterGafin::MonsterGafin() : m_DamageCaused( 0 ), m_nLogicTick( 0 )
{}

//----------------------------------------------------------------------------

const int UPDATE_LOGIC_RATE = TICKS_PER_SECOND * 10;

void MonsterGafin::simulate( dword nTick )
{
	// logic updates server side and only if monster is not controlled by a player
	if ( isLocal() && nTick >= m_nLogicTick )
	{
		m_nLogicTick = nTick + UPDATE_LOGIC_RATE;

		//PROFILE_START( "MonsterGafin::simulate( dword nTick ) - logic" );

		// attack any ships that attack us firstly
		if ( m_LastAttacker.valid() && validateTarget( m_LastAttacker ) )
		{
			// attack this target then
			verbCommand( ENGAGE, m_LastAttacker );
			//setOrder( ATTACK, m_LastAttacker );
			m_LastAttacker = NULL;
			m_DamageCaused = 0;
		}

		if ( order() == NOORDER || (rand() % 100 ) < 5 )
		{
			// make a list of possible places to attack
			Array< Noun * > defend;

			for(int i=0;i<contactCount();i++)
			{
				Noun * pContact = contact( i );
				if ( WidgetCast<NounNebula>( pContact ) )
					defend.push( pContact );		// defend nebulas
				else if ( WidgetCast<MonsterGafin>( pContact ) )
				{
					// don't make a circular dependency with orders, otherwise they will fly in a straight line out to nowhere 
					MonsterGafin * pCheck = (MonsterGafin *)pContact;
					while( pCheck != NULL )
					{
						if ( pCheck == this )
							break;		// stop, this is us
						pCheck = WidgetCast<MonsterGafin>( pCheck->orderTarget() );
					}

					if ( pCheck == NULL )
						defend.push( pContact );		// defend other gafin's
				}
				else if ( WidgetCast<NounPlanet>( pContact ) )
					if ( ((NounPlanet *)pContact)->flags() & NounPlanet::FLAG_HYDROGEN )
						defend.push( pContact );	// defend planets with hydrogen

			}

			if ( defend.size() > 0 )
				verbOrder( DEFEND, defend[ rand() % defend.size() ] );
		}
		//PROFILE_END();
	}

	NounShip::simulate( nTick );
}

void MonsterGafin::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	if ( damage > m_DamageCaused )
	{
		m_DamageCaused = damage;
		m_LastAttacker = pFrom;
	}

	NounShip::inflictDamage( nWhen, pFrom, damage, type, direction );
}

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( MonsterGafin2, MonsterGafin );

MonsterGafin2::MonsterGafin2()
{}

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( MonsterGafin3, MonsterGafin );

MonsterGafin3::MonsterGafin3()
{}

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( MonsterGafin4, MonsterGafin );

MonsterGafin4::MonsterGafin4()
{}

//----------------------------------------------------------------------------
//EOF
