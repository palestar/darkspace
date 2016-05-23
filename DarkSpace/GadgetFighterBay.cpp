/*
	GadgetFighterBay.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "GadgetFighterBay.h"
#include "GameContext.h"
#include "resource.h"

//----------------------------------------------------------------------------

Constant MAX_ZONE_FIGHTERS( "MAX_ZONE_FIGHTERS", 128 );

struct RomanDigit
{
	const char *		m_psString;
	unsigned int		m_nValue;
};

static const RomanDigit ROMAN_DIGITS[]=
{
	{"M",  1000},
	{"CM",  900},
	{"D",   500},
	{"CD",  400},
	{"C",   100},
	{"XC",   90},
	{"L",    50},
	{"XL",   40},
	{"X",    10},
	{"IX",    9},
	{"V",     5},
	{"IV",    4},
	{"I",     1},
};


static CharString GenerateRomanNumerals( unsigned int a_nNumber )
{
	CharString digits;
	for(int i=0; a_nNumber > 0 && i < sizeof(ROMAN_DIGITS)/sizeof(ROMAN_DIGITS[0]);++i)
	{
		while( ROMAN_DIGITS[i].m_nValue <= a_nNumber )
		{
			digits += ROMAN_DIGITS[i].m_psString;
			a_nNumber -= ROMAN_DIGITS[i].m_nValue;
		}
	}

	return digits;
}

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetFighterBay, GadgetWeapon ); 
REGISTER_FACTORY_KEY( GadgetFighterBay, 4387377698697536802LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetFighterBay, GadgetWeapon );
END_PROPERTY_LIST();

//---------------------------------------------------------------------------------------------------

bool GadgetFighterBay::postInitialize()
{
	if (! GadgetWeapon::postInitialize() )
		return false;

	if ( m_NounContext.valid() )
	{
		// pre-load assets..
		m_NounContext->resource( "FIGHTER", false );
	}

	return true;
}

//---------------------------------------------------------------------------------------------------

bool GadgetFighterBay::usable( Noun * pTarget, bool shift ) const
{
	NounShip * pParent = WidgetCast<NounShip>( parentNoun() );
	if ( pParent != NULL && (pParent->flags() & NounShip::FLAG_CLOAKED) != 0 )
		return false;		// device is not usable while cloaked, even if shift is down..

	return GadgetWeapon::usable( pTarget, shift );
}

const float		MAX_RECALL_RANGE = 10000.0f;

void GadgetFighterBay::use( dword when, Noun * pTarget, bool shift)
{
	if ( shift )
	{
		bool bValidTarget = validateTarget( pTarget );
		
		// change target/recall all fighters
		Array< GameContext::NounCollision > nouns;
		if ( context()->proximityCheck( worldPosition(), MAX_RECALL_RANGE, nouns, CLASS_KEY(NounFighter)) )
		{
			for(int i=0;i<nouns.size();i++)
			{
				NounFighter * pFighter = (NounFighter *)nouns[i].pNoun;
				if ( pFighter->weapon() == this )
				{
					if ( bValidTarget && pTarget != pFighter->target() )
					{
						pFighter->setTarget( pTarget );
						message( CharString().format( "<color;ffffff>Command: %s changing target to %s.", pFighter->name(), pTarget->name() ) );	
					}
					else
					{
						pFighter->setTarget( NULL );
						message( CharString().format("<color;ffffff>Command: %s returning to base.", pFighter->name() ) );
					}
				}
			}
		}

	}
	else
	{	
		// launch a fighter
		GadgetWeapon::use( when, pTarget, shift );
	}
}

NounProjectile * GadgetFighterBay::projectileFactory()
{
	// get the fighter template
	Scene * pScene = WidgetCast<Scene>( nounContext()->resource( "FIGHTER" ) );
	if ( pScene == NULL )
		return NULL;
	if ( pScene->node()->childCount() < 1 )
		return NULL;

	// find the NounFighter object in the scene 
	NounFighter * pFighter = WidgetCast<NounFighter>( pScene->node()->child( 0 ) );
	if ( pFighter != NULL )
	{
		// return a copy of the fighter object!
		NounFighter * pCopy = (NounFighter *)pFighter->copy();
		// load all ammo
		pCopy->setup();

		static int nFighterNumber = 1;
		// give the fighter a unique name
		pCopy->setName( CharString().format( "%s %s", pCopy->name(), GenerateRomanNumerals( nFighterNumber++ ).cstr() ) );

		return pCopy;
	}

	return NULL;		// no fighter found in scene
}

//----------------------------------------------------------------------------
//EOF
