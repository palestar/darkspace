/*
	NounBeacon.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Constants.h"
#include "NounBeacon.h"
#include "NounShip.h"
#include "resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounBeacon, NounGame );
REGISTER_FACTORY_KEY( NounBeacon, 4310006587792664725LL );

BEGIN_PROPERTY_LIST( NounBeacon, NounGame )
	ADD_TRANSMIT_PROPERTY( m_GadgetBeacon );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Life );
	ADD_TRANSMIT_PROPERTY( m_BaseSignature );
END_PROPERTY_LIST();

NounBeacon::NounBeacon() : m_Life( 0 )
{}

NounBeacon::NounBeacon( const Vector3 & position, GadgetBeacon * pGadget ) 
	: m_GadgetBeacon( pGadget ), m_Life( pGadget->beaconLife() ), m_BaseSignature( pGadget->beaconSignature() )
{
	setNounContext( pGadget->nounContext() );
	setPosition( position );
	setName( CharString( "BEACON" ) );
}

//----------------------------------------------------------------------------

void NounBeacon::onAttached()
{
	Noun::onAttached();

	if ( m_GadgetBeacon.valid() )
		m_GadgetBeacon->onProjectileActive( this );
}

void NounBeacon::onDetaching()
{
	Noun::onDetaching();

	if ( m_GadgetBeacon.valid() )
		m_GadgetBeacon->onProjectileDead( this );
}

void NounBeacon::render( RenderContext &context, 
				const Matrix33 & frame, 
				const Vector3 & position )
{
	Noun::render( context, frame, position );

	if (! m_pHitEffect.valid() && !(m_pHitEffect = WidgetCast<Scene>( nounContext()->resource( "HIT" ))) )
		return;
	
	float fContextAlpha = context.alpha();
	context.setAlpha( 1.0f );
	m_pHitEffect->render( context, frame, position );
	context.setAlpha( fContextAlpha);
}

//----------------------------------------------------------------------------

NounType NounBeacon::nounType() const
{
	return TYPE_LOCAL_OBJECT;
}

bool NounBeacon::canBeDetected() const
{
	return true;
}

float NounBeacon::baseSignature() const
{
	return m_BaseSignature;
}

void NounBeacon::release()
{
	Noun::release();
	m_GadgetBeacon = NULL;
}

void NounBeacon::simulate( dword nTick ) 
{
	// get our parent noun
	NounGame * pParent = WidgetCast<NounGame>( parentNoun() );
	if ( pParent != NULL && m_Life > 0 )
	{
		// add signature to parent, making it more detectable
		pParent->addSignature( nTick, baseSignature() );
		--m_Life;
	}
	else
	{
		// end of life or not attached to a noun, detach from the world
		setDetach();
	}

	Noun::simulate( nTick );
}

//----------------------------------------------------------------------------

NounBeacon * NounBeacon::findBeacon( Noun * pNoun, Noun * pFrom )
{
	for(int i=0;i<pNoun->childCount();i++)
	{
		NounBeacon * pBeacon = WidgetCast<NounBeacon>( pNoun->child(i) );
		if ( pBeacon != NULL && pBeacon->owner() == pFrom )
			return pBeacon;	
	}

	return NULL;
}

//----------------------------------------------------------------------------
//EOF
