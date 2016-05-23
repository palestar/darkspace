
/*
	NounEnhancement.cpp
	(c)2008 Palestar Inc, Jeff Everett
*/


#include "Debug/Assert.h"
#include "Constants.h"
#include "NounShip.h"
#include "NounPlanet.h"
#include "StructurePort.h"
#include "GameContext.h"
#include "NounEnhancement.h"
#include "GameProfile.h"

static Constant REMOVE_ENH_PERCENT_COST( "REMOVE_ENH_PERCENT_COST", 0.2f );

//---------------------------------------------------------------------------------------------------

bool				NounEnhancement::sm_bInitialized = false;
NounEnhancement::EnhancmentList
					NounEnhancement::sm_BuyList;

const Color			COLOR_COMMON(15,217,26,255);
const Color			COLOR_RARE (108,108,255,255);
const Color			COLOR_ULTRA_RARE (193,0,193,255);

IMPLEMENT_ABSTRACT_FACTORY( NounEnhancement, NounGame );

BEGIN_ABSTRACT_PROPERTY_LIST( NounEnhancement, NounGame )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nDamage );
END_PROPERTY_LIST();

//---------------------------------------------------------------------------------------------------

NounEnhancement::NounEnhancement() : m_nDamage( 0 ), m_bApplied( false )
{}

NounEnhancement::~NounEnhancement() 
{}

//---------------------------------------------------------------------------------------------------

void NounEnhancement::onAttached()
{
	NounGame::onAttached();
	applyModifiers();
}

void NounEnhancement::onDetaching()
{
	removeModifiers();
	NounGame::onDetaching();
}

//---------------------------------------------------------------------------------------------------

NounType NounEnhancement::nounType() const
{
	return TYPE_OBJECT;
}

void NounEnhancement::initialize()
{
	NounGame::initialize();
	loadIcon();
	applyModifiers();
}

//---------------------------------------------------------------------------------------------------

Material * NounEnhancement::icon() const
{
	return m_pIcon;
}

Color NounEnhancement::color() const
{
	Rarity r = rarity();
	if(r == RARITY_RARE)
		return COLOR_RARE;
	else if(r == RARITY_ULTRA_RARE)
		return COLOR_ULTRA_RARE;
	return COLOR_COMMON;
}

CharString NounEnhancement::status() const
{
	// only the requirements are displayed in teh description since it is used by tool tips
	CharString sDesc;
	sDesc.format("<large><color;%6.6x>%s</color></large>",color().RGB(),getName());

	NounShip * pShip = WidgetCast<NounShip>( parent() );
	for(size_t i=0;i<m_Modifiers.size();i++)
	{
		EnhancementModifier * pModifier = m_Modifiers[i];
		if ( pModifier != NULL )
			sDesc += CharString("\n  ") + pModifier->getDescription( pShip );
	}

	unsigned int flags = requirementFlags();
	if ( (flags & REQ_ANY) != 0 && (flags & REQ_ANY) != REQ_ANY )
	{
		sDesc += "\n<color;ffffffff>  Ships: <color;ff50ff50>";
		if( flags & REQ_ENGINEER )
			sDesc += "Engineer ";
		if( flags & REQ_SUPPLY)
			sDesc += "Supply ";
		if( flags & REQ_TRANSPORT)
			sDesc += "Transport ";
		if( flags & REQ_SCOUT)
			sDesc += "Scout ";
		if( flags & REQ_FRIGATE)
			sDesc += "Frigate ";
		if( flags & REQ_DESTROYER)
			sDesc += "Destroyer ";
		if( flags & REQ_CRUISER)
			sDesc += "Cruiser ";
		if( flags & REQ_DREAD)
			sDesc += "Dreadnaught ";
		if( flags & REQ_STATION)
			sDesc += "Station ";
		sDesc += "</color>";
	}

	if( flags & REQ_SUBSCRIBER)
		sDesc += "\n<color;ff00ff00>  SUBSCRIBER</color>";
	if(isUnique())
		sDesc += "\n<color;ff0000ff>  UNIQUE</color>";
	if( flags & REQ_ADMIN)
		sDesc += "\n<color;ff00ff00>  ADMIN</color>";

	if ( maxDamage() > 0 )
	{
		sDesc += CharString().format("\n<color;%s>  DURABILITY: %u/%u</color>", 
			(damage() < (maxDamage() / 2)) ? "ffffffff" : "ff0000ff", 
			maxDamage() - damage(), maxDamage() );
	}

	return sDesc;
}

//---------------------------------------------------------------------------------------------------

const char * NounEnhancement::getIconName() const
{
	return "interface/icons/team.prt";
}

NounEnhancement::ApplyType NounEnhancement::applyType() const
{
	return AT_ATTACH;
}

dword NounEnhancement::requirementFlags() const
{
	return REQ_ANY;
}

NounEnhancement::Rarity NounEnhancement::rarity() const
{
	return RARITY_COMMON;
}

bool NounEnhancement::isUnique() const
{
	return false;
}

bool NounEnhancement::canBuy() const
{
	return false;
}

int NounEnhancement::buyValue() const
{
	return 0;
}

u8 NounEnhancement::maxDamage() const
{
	return 0;
}

//---------------------------------------------------------------------------------------------------

bool NounEnhancement::incrementDamage()
{
	if ( maxDamage() != 0 && m_nDamage < maxDamage() )
	{
		++m_nDamage;
		if ( m_nDamage >= maxDamage() )
			return true;
	}

	return false;
}

void NounEnhancement::setDamage( u8 a_nDamage )
{
	m_nDamage = a_nDamage;
}

int NounEnhancement::calculateRemoveCost()
{
#if 1
	return 0;
#else
	// removal cost is some % of buy value, defined by a constant loaded from the constants.ini file..
	int nRemovalCost =  buyValue() * REMOVE_ENH_PERCENT_COST;
	// damage to the gadget reduces it's costs to remove..
	float fRemoveMod = 0.0f;
	if ( maxDamage() > 0 )
		fRemoveMod = (float)damage() / maxDamage();		// 0.0 - 1.0f scalar
	if ( fRemoveMod > 0.0f )
		nRemovalCost -= nRemovalCost * fRemoveMod;

	return nRemovalCost;
#endif
}

bool NounEnhancement::canUse( NounShip * a_pShip )
{
	if ( applyType() == AT_ATTACH )
	{
		// check if we have enough enhancement slots or not..
		if ( a_pShip->enhancementCount() >= a_pShip->maxEnhancements() )
			return false;
	}

	// ask each modifier if they can be attached onto this ship, they may reject it..
	for(size_t i=0;i<m_Modifiers.size();++i)
		if (! m_Modifiers[i]->canAttach( a_pShip ) )
			return false;

	unsigned int flags = requirementFlags();

	// is this enhancement unique?
	if(isUnique())
	{
		for(int i=0;i<a_pShip->childCount();i++)
			if( a_pShip->child(i)->factory()->classKey() == factory()->classKey() )
				return false;
	}

	if( (flags & REQ_ADMIN) != 0 && (a_pShip->flags() & NounShip::FLAG_ADMIN) == 0 )
		return false;
	if( (flags & REQ_SUBSCRIBER) != 0 && (a_pShip->flags() & NounShip::FLAG_SUBSCRIBER) == 0 )
		return false;

	if ( (flags & REQ_ANY) != 0 )
	{
		switch( a_pShip->type() )
		{
		case NounShip::ENGINEER:
			return (flags & REQ_ENGINEER) != 0;
		case NounShip::SUPPLY:
			return (flags & REQ_SUPPLY) != 0;
		case NounShip::TRANSPORT:
			return (flags & REQ_TRANSPORT) != 0;
		case NounShip::SCOUT:
			return (flags & REQ_SCOUT) != 0;
		case NounShip::FRIGATE:
			return (flags & REQ_FRIGATE) != 0;
		case NounShip::DESTROYER:
			return (flags & REQ_DESTROYER) != 0;
		case NounShip::CRUISER:
			return (flags & REQ_CRUISER) != 0;
		case NounShip::DREAD:
			return (flags & REQ_DREAD) != 0;
		case NounShip::STATION:
			return (flags & REQ_STATION) != 0;
		}

		return false;
	}

	return true;
}

bool NounEnhancement::use( NounShip *a_pShip )
{
	if (! canUse( a_pShip ) )
		return false;

	// attaching onto the ship will call onAttached() and apply our modifiers
	a_pShip->attachNode( this );
	if (! m_bApplied )
	{
		// detach from ship since it failed to apply, this will keep the enhancement usable..
		a_pShip->detachNode( this );
		return false;
	}

	// if this is a use enhancement, then flag it to be detached...
	if ( applyType() == AT_USE )
		setDetach();

	return true;
}

void NounEnhancement::applyModifiers( bool bForced /*= false*/ )
{
	if ((!m_bApplied || bForced) && (maxDamage() == 0 || damage() < maxDamage()) )
	{
		NounShip * pParent = WidgetCast<NounShip>( parent() );
		if ( pParent != NULL )
		{
			for(size_t i=0;i<m_Modifiers.size();++i)
				if (! m_Modifiers[i]->onApply( pParent ) )
				{
					// failed to apply modifier i, remove all modifiers that have already been applied..
					for( size_t k=0;k<i;++k)
						m_Modifiers[i]->onRemove( pParent );
					return;
				}
		}
		m_bApplied = true;
	}
}

void NounEnhancement::removeModifiers()
{
	if ( m_bApplied )
	{
		NounShip * pParent = WidgetCast<NounShip>( parent() );
		if ( pParent != NULL )
		{
			for(size_t i=0;i<m_Modifiers.size();++i)
				m_Modifiers[i]->onRemove( pParent );
		}
		m_bApplied = false;
	}
}

void NounEnhancement::loadIcon()
{
	if (! loadResource( getIconName(), m_pIcon ) )
		TRACE( "Failed to load ehancement icon!" );
}

//---------------------------------------------------------------------------------------------------

void NounEnhancement::staticInitialize()
{
	if(sm_bInitialized)
		return;

	// enumerate all classes derived from NounEnhancement that have a buyValue() > 0 and
	// add them to the sm_BuyList static variable... this is enumerated when the buy window 
	// is up on the ship selection screen.
	std::list< ClassKey > classes;
	classes.push_back( CLASS_KEY( NounEnhancement ) );

	while( classes.size() > 0 )
	{
		ClassKey k( classes.front() );
		classes.pop_front();

		int nTypes = Factory::typeCount( k );
		for(int i=0;i<nTypes;++i)
			classes.push_back( Factory::type( k, i ) );

		Widget::Ref pWidget = Factory::createWidget( k );
		NounEnhancement::Ref pEnhancement = WidgetCast<NounEnhancement>( pWidget );
		if ( pEnhancement && pEnhancement->canBuy() )
			sm_BuyList.push_back( pEnhancement );
	}

	sm_bInitialized = true;
}

//---------------------------------------------------------------------------------------------------
// EOF
