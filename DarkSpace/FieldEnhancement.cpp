/**
	@file FieldEnhancement.cpp

	(c)2009 Palestar Inc
	@author Richard Lyle @date 6/23/2011 8:01:28 PM
*/

#include "Debug/Profile.h"
#include "FieldEnhancement.h"
#include "NounShip.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( FieldEnhancement, NounField );

BEGIN_PROPERTY_LIST( FieldEnhancement, NounField )
	ADD_TRANSMIT_PROPERTY( m_bFriendsOnly );
	ADD_TRANSMIT_PROPERTY( m_Modifiers );
	ADD_TRANSMIT_PROPERTY( m_fFieldRadius );
END_PROPERTY_LIST();

FieldEnhancement::FieldEnhancement() : m_bFriendsOnly( true )
{}

//----------------------------------------------------------------------------

void FieldEnhancement::initialize()
{
	// remove any invalid modifiers..
	for(int i=0;i<m_Modifiers.size();++i)
		if (! m_Modifiers[i].valid() )
		{
			LOG_ERROR( "FieldEnhancement", "Removing invalid modifier %d", i );
			m_Modifiers.removeSwap( i-- );
		}
}

void FieldEnhancement::simulate( dword nTick )
{
	NounField::simulate( nTick );
}

//----------------------------------------------------------------------------

void FieldEnhancement::onEnterField( Noun * pNoun )
{
	if ( ( m_bFriendsOnly && isFriend( pNoun ) ) 
		|| ( !m_bFriendsOnly && isEnemy( pNoun ) ) )
	{
		if ( WidgetCast<NounShip>( pNoun ) )
		{
			for(int i=0;i<m_Modifiers.size();++i)
				m_Modifiers[i]->onApply( (NounShip *)pNoun );
		}
		// TODO: else if ( WidgetCast<NounPlanet>( pNoun ) )
	}
}

void FieldEnhancement::onInsideField( Noun * pNoun )
{}

void FieldEnhancement::onLeaveField( Noun * pNoun )
{
	if ( ( m_bFriendsOnly && isFriend( pNoun ) ) 
		|| ( !m_bFriendsOnly && isEnemy( pNoun ) ) )
	{
		if ( WidgetCast<NounShip>( pNoun ) )
		{
			for(int i=0;i<m_Modifiers.size();++i)
				m_Modifiers[i]->onRemove( (NounShip *)pNoun );
		}
	}
}

//---------------------------------------------------------------------------------------------------

void FieldEnhancement::setFriendsOnly( bool a_bFriendsOnly )
{
	m_bFriendsOnly = a_bFriendsOnly;
}

void FieldEnhancement::addModifier( EnhancementModifier * a_pModifier )
{
	m_Modifiers.push( a_pModifier );
}

void FieldEnhancement::removeModifier( EnhancementModifier * a_pModifier )
{
	m_Modifiers.removeSearchSwap( a_pModifier );
}

void FieldEnhancement::clearModifiers()
{
	m_Modifiers.release();
}

//---------------------------------------------------------------------------------------------------
//EOF
