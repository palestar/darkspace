/*
	GadgetSpawn.h
	(c)1999 Palestar Development, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "GadgetSpawn.h"
#include "CargoGadget.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( GadgetSpawn, NounGadget );
REGISTER_FACTORY_KEY( GadgetSpawn, 4390541822318883224LL );

BEGIN_PROPERTY_LIST( GadgetSpawn, NounGadget )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Mode );
END_PROPERTY_LIST();

GadgetSpawn::GadgetSpawn() : m_Mode( 0 )
{}

//----------------------------------------------------------------------------

void GadgetSpawn::initialize()
{
	NounGadget::initialize();

	// load objects we can spawn
	loadSpawns();
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetSpawn::type() const
{
	return UNKNOWN;
}

dword GadgetSpawn::hotkey() const
{
	return 'N';
}

CharString GadgetSpawn::status() const
{
	return CharString().format("%u", m_Mode);
}

CharString GadgetSpawn::description() const
{
	Noun * pSpawn = getSpawn();
	if ( pSpawn != NULL && pSpawn->nounContext() != NULL )
		return CharString().format("<color;ffffff>%s\n<color;808080>%s", pSpawn->nounContext()->name(), 
			pSpawn->nounContext()->description() );

	return "?";
}

int GadgetSpawn::useModeCount() const
{
	return m_Spawn.size();
}

int GadgetSpawn::useMode() const
{
	return m_Mode;
}

bool GadgetSpawn::useTarget() const
{
	return true;
}

bool GadgetSpawn::usable( Noun * pTarget, bool shift ) const
{
	if ( getSpawn() != NULL )
		return true;
	return false;
}

void GadgetSpawn::useMode( int mode )
{
	m_Mode = Clamp( mode, 0, m_Spawn.size() );
}

void GadgetSpawn::use( dword when, Noun * pTarget, bool shift )
{
	Noun * pSpawn = getSpawn();
	if ( pSpawn != NULL )
	{
		message( CharString().format( "SPAWN: %s", pSpawn->name()) );

		// this gadget is used on the server side only
		if ( isServer() )
		{
			// create a copy of the object
			pSpawn = WidgetCast<Noun>( pSpawn->copy() );
			ASSERT( pSpawn );

			// set the object tick to the same as when the gadget was used
			pSpawn->setTick( when );
			pSpawn->setup();

			// convert into a cargo item if it's a gadget
			if ( WidgetCast<NounGadget>( pSpawn ) )
			{
				CargoGadget * pCargo = new CargoGadget( (NounGadget *)pSpawn );
				delete pSpawn;

				pSpawn = pCargo;
			}
	
			if ( pTarget != NULL )
			{
				Vector3 offset( RandomFloat(-1,1), 0, RandomFloat(-1,1) );
				offset.normalize();

				float r = (pTarget->radius() + pSpawn->radius()) * 1.1f;
				pSpawn->setPosition( pTarget->worldPosition() + (offset * r) );
			}
			else
				pSpawn->setPosition( worldPosition() );

			context()->attachNoun( pSpawn );

			// put ships under ai control
			if ( WidgetCast<NounShip>( pSpawn ) )
			{
				((NounShip *)pSpawn)->setTeamId( parentBody()->teamId() );
				((NounShip *)pSpawn)->setUserId( 0 ); 
			}
		}
	}
}

//----------------------------------------------------------------------------

void GadgetSpawn::loadSpawns()
{
	m_Spawn.release();

	Collection::Link collection = WidgetCast<Collection>( nounContext()->resource( "SPAWN" ) );
	if ( collection.valid() )
	{
		for(int i=0;i<collection->resourceCount();i++)
		{
			Resource * pResource = collection->resource(i);
			if ( WidgetCast<Scene>( pResource ) )
			{
				BaseNode * pRoot = ((Scene *)pResource)->node();
				ASSERT( pRoot );

				for(int j=0;j<pRoot->childCount();j++)
				{
					Noun * pNoun = WidgetCast<Noun>( pRoot->child(j) );
					if ( pNoun != NULL )
						m_Spawn.push( pNoun );
				}
			}
			else if ( WidgetCast<NounContext>( pResource ) )
			{
				Noun * pNoun = ((NounContext *)pResource)->createNoun();

				// convert gadgets into a cargo item
				if ( WidgetCast<NounGadget>( pNoun ) )
				{
					CargoGadget * pCargo = new CargoGadget( (NounGadget *)pNoun );
					delete pNoun;

					pNoun = pCargo;
				}
				
				m_Spawn.push( pNoun );
			}
			else if ( WidgetCast<NounTemplate>( pResource ) )
				m_Spawn.push( ((NounTemplate *)pResource)->noun() );
		}
	}
}

//----------------------------------------------------------------------------

Noun * GadgetSpawn::getSpawn() const
{
	if ( m_Mode < m_Spawn.size() )
		return m_Spawn[ m_Mode ];
	return NULL;
}

//----------------------------------------------------------------------------
// EOF
