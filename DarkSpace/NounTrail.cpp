/*
	NounTrail.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "NounTrail.h"
#include "Constants.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_NOKEY_FACTORY( NounTrail, NounGame );
REGISTER_FACTORY_KEY( NounTrail, 4160086826159486992LL );

BEGIN_PROPERTY_LIST( NounTrail, NounGame )
	CLEAR_PROPERTY_FLAGS( m_Flags, PF_UPDATE );
	CLEAR_PROPERTY_FLAGS( m_Position, PF_UPDATE );
	CLEAR_PROPERTY_FLAGS( m_vOrientation, PF_UPDATE );
END_PROPERTY_LIST();

NounTrail::NounTrail() : m_bDetachOnEnd( false ), m_bUpdate( false )
{}

NounTrail::NounTrail( const Trail & a_Copy ) : m_Trail( a_Copy ), 
	m_bDetachOnEnd( true ), m_bUpdate( false )
{}

//----------------------------------------------------------------------------

void NounTrail::render( RenderContext &context, 
	const Matrix33 & frame, 
	const Vector3 & position )
{
	// call our base class
	Noun::render( context, frame, position );

	// moved the update into the Render so that we can be sure our worldPosition() is correctly adjusted..
	if ( m_bUpdate )
	{
		// update the trail
		m_Trail.updateTrail( worldPosition() );
		if ( m_bDetachOnEnd && m_Trail.isEnded() )
			setDetach();
		m_bUpdate = false;
	}

	// render the trail
	m_Trail.renderTrail( context, frame, position, position );
}

//----------------------------------------------------------------------------

NounType NounTrail::nounType() const
{
	return TYPE_EFFECT;
}

bool NounTrail::postInitialize()
{
	if (! Noun::postInitialize() )
		return false;
	if (! nounContext()->resource( "TRAIL", false ) )
		return false;		// wait for the trail material to be loaded..

	resetTrail();
	return true;
}

void NounTrail::simulate( dword nTick )
{
	Noun::simulate( nTick );
	m_bUpdate = true;		// flag for update, next call to render() will update the trail..
}

void NounTrail::onWorldTransform( const Vector3 & transform )
{
	Noun::onWorldTransform( transform );
	// update the trail so it appears in the same location
	m_Trail.offsetTrail( transform );
}

//---------------------------------------------------------------------------------------------------

void NounTrail::resetTrail()
{
	m_Trail.setTrailMaterial( WidgetCast<Material>( 
		nounContext()->resource( "TRAIL" ) ) );
}

void NounTrail::CopyTrailsIntoZone( BaseNode * pNode )
{
	if ( pNode != NULL )
	{
		// recurse into child nodes..
		for(int i=0;i<pNode->childCount();++i)
			CopyTrailsIntoZone( pNode->child(i) );

		if ( WidgetCast<NounTrail>( pNode ) )
		{
			NounTrail * pTrail = (NounTrail *)pNode;

			NounTrail * pTrailCopy = new NounTrail( pTrail->trail() );
			pTrailCopy->setPosition( pTrail->worldPosition() );
			pTrailCopy->setFrame( pTrail->worldFrame() );
			pTrail->context()->attachNoun( pTrailCopy );
		}
	}
}

//----------------------------------------------------------------------------
//EOF
