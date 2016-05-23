/*
	NounNebula.h
	(c)1999 Palestar Development, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Constants.h"
#include "NounNebula.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

Constant			NEBULA_NATURAL_ECM( "NEBULA_NATURAL_ECM", 6.0f );
const dword			NEBULA_UPDATE = TICKS_PER_SECOND * 5;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounNebula, NounBody );
REGISTER_FACTORY_KEY( NounNebula, 4047286620084539939LL );

BEGIN_PROPERTY_LIST( NounNebula, NounBody )
	ADD_TRANSMIT_PROPERTY( m_Name );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Children );
END_PROPERTY_LIST();

NounNebula::NounNebula() : m_nUpdateAffected( 0 )
{
	m_nNodeFlags |= NF_NOSHADOW;
}
 
//----------------------------------------------------------------------------

NounType NounNebula::nounType() const
{
	return TYPE_LANDMARK;
}

void NounNebula::release()
{
	NounBody::release();
	// release references to other objects
	m_Affect.release();
}

void NounNebula::simulate( dword nTick )
{
	if ( nTick >= m_nUpdateAffected  )
	{
		m_nUpdateAffected = nTick + NEBULA_UPDATE;
		m_Affect.release();

		// affect the ECM of all ships within range
		Array< GameContext::NounCollision > nouns;
		if ( context()->proximityCheck( worldPosition(), radius(), nouns ) )
		{
			for(int i=0;i<nouns.size();i++)
			{
				NounGame * pAffect = WidgetCast<NounGame>( nouns[i].pNoun );
				if ( pAffect != NULL && pAffect->enableECM() )
					m_Affect.push( pAffect );
			}
		}
	}

	// modify the ECM on the affected objects
	for(int i=0;i<m_Affect.size();++i)
	{
		NounGame::Ref pAffect = m_Affect[ i ];
		if ( pAffect.valid() )
			pAffect->addSignature( nTick, -NEBULA_NATURAL_ECM );
	}

	NounBody::simulate( nTick );
}


//----------------------------------------------------------------------------
// EOF





