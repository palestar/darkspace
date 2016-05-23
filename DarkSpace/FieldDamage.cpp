/*
	FieldDamage.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/


#include "FieldDamage.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( FieldDamage, NounField );
REGISTER_FACTORY_KEY( FieldDamage, 4775406032264294020LL );

BEGIN_PROPERTY_LIST( FieldDamage, NounField )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fLife );
	ADD_PROPERTY( m_nInflictDamage );
	ADD_BITS_PROPERTY( m_nInflictDamageType );
		ADD_BIT_OPTION( m_nInflictDamageType, DAMAGE_KINETIC );
		ADD_BIT_OPTION( m_nInflictDamageType, DAMAGE_ENERGY );
		ADD_BIT_OPTION( m_nInflictDamageType, DAMAGE_PSI );
		ADD_BIT_OPTION( m_nInflictDamageType, DAMAGE_EMP );
		ADD_BIT_OPTION( m_nInflictDamageType, DAMAGE_BIO );
		ADD_BIT_OPTION( m_nInflictDamageType, DAMAGE_ELF );
		ADD_BIT_OPTION( m_nInflictDamageType, DAMAGE_PLANET );
END_PROPERTY_LIST();

FieldDamage::FieldDamage() : m_fLife( 0.0f ), m_nInflictDamage( 1 ), m_nInflictDamageType( DAMAGE_ENERGY )
{}

//----------------------------------------------------------------------------

void FieldDamage::simulate( dword nTick )
{
	NounField::simulate( nTick );

	if ( m_fLife > 0.0f )
	{
		float fElapsed = TICK_DURATION_S * (nTick - tick());

		m_fLife -= fElapsed;
		if ( m_fLife <= 0.0f )
			setDetach();
	}
}

//----------------------------------------------------------------------------

void FieldDamage::onEnterField( Noun * pNoun )
{}

void FieldDamage::onInsideField( Noun * pNoun )
{
	Noun * pZoneParent = zoneParent();

	NounGame * pNounGame = WidgetCast<NounGame>( pNoun );
	if ( pNounGame != NULL && pNounGame != pZoneParent && pNounGame->canDamage( m_nInflictDamageType ) )
	{
		Vector3 vDirection( worldPosition() - pNounGame->worldPosition() );
		float fDistance = vDirection.magnitude();
		vDirection *= 1.0f / fDistance;		// normalize the direction

		float fDamageRatio = 1.0f - (fDistance / m_fFieldRadius);
		if ( fDamageRatio < 0.0f )
			fDamageRatio = 0.0f;		// don't allow negative ratios

		int nInflict = (fDamageRatio * ((m_nInflictDamage * TICK_DURATION_S))) + 1;

		// send the damage verb
		if ( nInflict > 0 )
		{
			pNounGame->inflictDamage( tick(), pZoneParent, nInflict, m_nInflictDamageType, 
				pNounGame->worldFrame() * vDirection );
		}
	}
}

void FieldDamage::onLeaveField( Noun * pNoun )
{}

//----------------------------------------------------------------------------
//EOF
