/*
	NounEnhancement.h
	(c)2008 Palestar Inc, Jeff Everett
*/

#ifndef NOUNENHANCEMENT_H
#define NOUNENHANCEMENT_H

#include "NounGame.h"
#include "NounShip.h"
#include "EnhancementModifier.h"

#include <vector>

#include "GameDll.h"

//---------------------------------------------------------------------------------------------------

class DLL NounEnhancement : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	enum Constants {
		MAX_ENHANCEMENTS_PER_SHIP = 12,		// what is the most enhancements a single ship can have
	};

	typedef ModifierType			Type;

	enum ApplyType
	{
		AT_ATTACH,
		AT_USE
	};

	enum RequirementFlag {
		REQ_SUBSCRIBER				= 0x00000001,	
		REQ_ENGINEER				= 0x00000100,					
		REQ_SUPPLY					= 0x00000200,					
		REQ_TRANSPORT				= 0x00000400,					
		REQ_SCOUT					= 0x00000800,					
		REQ_FRIGATE					= 0x00001000,					
		REQ_DESTROYER				= 0x00002000,					
		REQ_CRUISER					= 0x00004000,					
		REQ_DREAD					= 0x00008000,					
		REQ_STATION					= 0x00010000,
		REQ_ADMIN					= 0x10000000,

		REQ_ANY						= REQ_ENGINEER|REQ_SUPPLY|REQ_TRANSPORT|REQ_SCOUT|REQ_FRIGATE|REQ_DESTROYER|REQ_CRUISER|REQ_DREAD|REQ_STATION,
	};

	enum Rarity
	{
		RARITY_COMMON		= 1,
		RARITY_RARE			= 2,
		RARITY_ULTRA_RARE	= 3,
	};

	typedef std::vector< EnhancementModifier::Ref >		ModifiedVector;

	typedef WidgetReference< NounEnhancement >	wRef;
	typedef Reference<NounEnhancement>			Ref;
	typedef std::vector< Ref >					EnhancmentList;

	//! Construction
	NounEnhancement();
	virtual ~NounEnhancement();

	//! BaseNode interface
	virtual void			onAttached();
	virtual void			onDetaching();
	//! Noun interface
	virtual NounType		nounType() const;
	virtual void			initialize();

	//! NounGame interface
	virtual CharString		status() const;				// status string displayed when targeted

	//! Accessors
	Material *				icon() const;
	Color					color() const;				// color of enhancement as displayed on it's icon & title text 
	u8						damage() const;				// current amount of damage on this enahancement
	bool					isActive() const;			// returns true if damage() < maxDamage() || maxDamage() == 0

	int						modifierCount() const;
	EnhancementModifier *	modifier( int n ) const;

	//! Interface
	virtual const char *	getName() const = 0;		// name of this enhancement
	virtual const char *	getIconName() const;		// get the icon resource name
	virtual ApplyType		applyType() const;			// apply type
	virtual dword			requirementFlags() const;	// what are the requirement flags for this enhancement
	virtual Rarity			rarity()  const;			// how rare is this ehancement, used to determine color
	virtual bool			isUnique() const;			// if true, then only one of this enhancement type can be on a ship at once
	virtual bool			canBuy() const;				// if true, then item can be bought in the store
	virtual int				buyValue() const;			// how much does this item cost in credits
	virtual u8				maxDamage() const;			// maximum damage of this enhancement, 0 if indestructable

	//! Mutators
	bool					incrementDamage();			// +1 to damage, returns true if destroyed
	void					setDamage( u8 a_nDamage );	// set damage to specific level
	int						calculateRemoveCost();		// how many credits does it cost to remove this enhancement

	virtual bool			canUse( NounShip * a_pShip );	// check if this enhancment can be used on the given ship
	virtual bool			use( NounShip *a_pShip );	// use this enhancement on the given ship, this may attach this object onto the ship
	virtual void			applyModifiers( bool bForce = false );			// invoked by initialize() & onAttached() apply our modifiers to our parent
	virtual void			removeModifiers();			// invoked by onDetaching() to remove our modifiers from our parent

	void					loadIcon();					// load our icon

	//! Static
	static void				staticInitialize();

	static bool				sm_bInitialized;
	static EnhancmentList	sm_BuyList;

protected:
	//! Data
	u8						m_nDamage;
	ModifiedVector			m_Modifiers;
	
	//! non-serialized
	bool					m_bApplied;
	Material::Link			m_pIcon;
};

//---------------------------------------------------------------------------------------------------

inline u8 NounEnhancement::damage() const
{
	return m_nDamage;
}

inline bool NounEnhancement::isActive() const
{
	return maxDamage() == 0 || damage() < maxDamage();
}

inline int NounEnhancement::modifierCount() const
{
	return (int)m_Modifiers.size();
}

inline EnhancementModifier * NounEnhancement::modifier( int n ) const
{
	if ( n >= 0 && n < (int)m_Modifiers.size() )
		return m_Modifiers[ n ];
	return NULL;
}

//---------------------------------------------------------------------------------------------------

#endif

