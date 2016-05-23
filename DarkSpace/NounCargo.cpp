/*
	NounCargo.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Standard/Limits.h"
#include "VerbLoad.h"
#include "NounShip.h"
#include "NounCargo.h"
#include "StructurePort.h"
#include "GameContext.h"
#include "SceneryEffect.h"

//----------------------------------------------------------------------------

const char *	CARGO_NC = "Misc/Resource Container/NC_Resource.prt";

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounCargo, NounGame );
REGISTER_FACTORY_KEY( NounCargo, 4335706563441832149LL );

BEGIN_PROPERTY_LIST( NounCargo, NounGame )
	ADD_REPLICATED_PROPERTY( m_Quantity, TICKS_PER_SECOND );
END_PROPERTY_LIST();

NounCargo::NounCargo() : m_Quantity( 1 )
{
	NounContext::Link pNounContext;
	if ( loadResource( CARGO_NC, pNounContext ) )
		setNounContext( pNounContext );
}

//----------------------------------------------------------------------------

void NounCargo::preRender( RenderContext &context, 
					const Matrix33 & frame, 
					const Vector3 & position )
{
	BaseNode * pParent = parent();
	if ( WidgetCast<NounShip>( pParent ) != NULL ||
		WidgetCast<NounPlanet>( pParent ) != NULL ||
		WidgetCast<StructurePort>( pParent ) != NULL )
	{
		// don't render when attached to the planet or a ship
		return;
	}

	Noun::preRender( context, frame, position );
}

//----------------------------------------------------------------------------

NounType NounCargo::nounType() const
{
	return TYPE_LOCAL_OBJECT;
}

bool NounCargo::collidable() const
{
	return isRootNoun();	
}

bool NounCargo::canDamage( dword type ) const
{
	return (type & (DAMAGE_ENERGY|DAMAGE_KINETIC)) != 0;
}

bool NounCargo::canBeDetected() const
{
	return true;
}

bool NounCargo::isCargo() const
{
	return true;
}

bool NounCargo::postInitialize()
{
	if (! Noun::postInitialize() )
		return false;

	if ( m_NounContext.valid() )
	{
		// pre-load assets...
		m_NounContext->resource( "EXPLOSION", false );
	}

	return true;
}

void NounCargo::simulate( dword nTick )
{
	if ( isServer() )
	{
		if ( m_Quantity <= 0 )
			setDetach();
	}

	Noun::simulate( nTick );
}

void NounCargo::collision( Noun * pCollide, const Vector3 & intersect )
{}

void NounCargo::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	m_Quantity -= damage;
	if ( m_Quantity <= 0 )
		explode();
}

//----------------------------------------------------------------------------

int NounCargo::quantity() const
{
	return m_Quantity;
}

int NounCargo::durability() const
{
	return 50;
}

int NounCargo::value() const
{
	return 0;
}

int NounCargo::size() const
{
	return 1;
}

int NounCargo::maxQuantity() const
{
	return 1000;
}

void NounCargo::setQuantity( int quantity )
{
	m_Quantity = quantity; //Clamp( quantity, 0, maxQuantity() );
}

CharString NounCargo::status() const
{
	return NounGame::status();
}

void NounCargo::explode()
{
	// create explosion client side only
	if ( context()->isClient() )
	{
		Scene * pExplosion = WidgetCast<Scene>( nounContext()->resource( "EXPLOSION" ) );
		if ( pExplosion != NULL )
		{
			// create the hit effect
			SceneryEffect * pHit = new SceneryEffect;
			pHit->setNounContext( new NounContext( pExplosion ) );
			pHit->setPosition( worldPosition() );

			if (! pHit->setSegment( "Effect" ) )
				pHit->setLife( 5.0f );

			context()->attachNoun( pHit );
		}
	}

	setDetach();
}

//----------------------------------------------------------------------------
//EOF
