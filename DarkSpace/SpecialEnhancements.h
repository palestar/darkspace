/**
	@file SpecialEnhancements.h
	@brief TODO
	
	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/6/2010 9:41:44 PM
*/

#ifndef SPECIALENHANCEMENTS_H
#define SPECIALENHANCEMENTS_H

#include "NounEnhancement.h"

//---------------------------------------------------------------------------------------------------

class JumpFuelModifier : public EnhancementModifier
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	//! Construction
	JumpFuelModifier( int a_nPercent = 10 ) : m_nPercent( a_nPercent )
	{}

	//! EnhancementModifier interface
	virtual CharString	getDescription( NounShip * a_pShip );
	virtual bool		canAttach( NounShip * a_pShip );
	virtual bool		onApply( NounShip * a_pShip );
	virtual void		onRemove( NounShip * a_pShip );

protected:
	//! Data
	int					m_nPercent;
};

class JumpDriveRefill : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	JumpDriveRefill()
	{
		m_Modifiers.push_back(new JumpFuelModifier(100));
	}

	virtual const char * getName() const 
	{ 
		return "JumpDrive Fuel Refill";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_jumpdriverefill.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 250; 
	}
	virtual ApplyType applyType() const 
	{ 
		return AT_USE;
	}
};

//---------------------------------------------------------------------------------------------------

class ShipEnergyModifier : public EnhancementModifier
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	//! Construction
	ShipEnergyModifier( int a_nPercent = 10 ) : m_nPercent( a_nPercent )
	{}

	//! EnhancementModifier interface
	virtual CharString	getDescription( NounShip * a_pShip );
	virtual bool		canAttach( NounShip * a_pShip );
	virtual bool		onApply( NounShip * a_pShip );
	virtual void		onRemove( NounShip * a_pShip );

protected:
	//! Data
	int					m_nPercent;
};

class EnergyRefill : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnergyRefill()
	{
		m_Modifiers.push_back(new ShipEnergyModifier(50));
	}

	const char * getName() const 
	{ 
		return "Energy Recharger MkI";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_energyrefuel50.prt";
	}
	bool canBuy() const
	{
		 return true;
	}	
	int buyValue() const 
	{ 
		return 250; 
	}
	ApplyType applyType() const 
	{ 
		return AT_USE;
	}
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
};

//---------------------------------------------------------------------------------------------------

// This modifier spawns a ship of the given type to act as a ally for the target ship.
class SpawnShipModifier : public EnhancementModifier
{
public:
	DECLARE_WIDGET_CLASS();

	//! Construction
	SpawnShipModifier( NounShip::Type eType = NounShip::SCOUT ) : m_eType( eType )
	{}

	//! EnhancementModifier interface
	virtual CharString	getDescription( NounShip * a_pShip );
	virtual bool		canAttach( NounShip * a_pShip );
	virtual bool		onApply( NounShip * a_pShip );
	virtual void		onRemove( NounShip * a_pShip );

protected:
	//! Data
	NounShip::Type		m_eType;
};

class ScoutAlly : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ScoutAlly()
	{
		m_Modifiers.push_back(new SpawnShipModifier( NounShip::SCOUT ));
	}

	const char * getName() const 
	{ 
		return "Scout Ally";
	}
	virtual const char * getIconName() const
	{
		return "interface/navigation/scout.prt";
	}
	bool canBuy() const
	{
		 return true;
	}	
	int buyValue() const 
	{ 
		return 50; 
	}
	ApplyType applyType() const 
	{ 
		return AT_USE;
	}
	Rarity rarity()  const 
	{
		return RARITY_COMMON;
	}
};

class FrigateAlly : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	FrigateAlly()
	{
		m_Modifiers.push_back(new SpawnShipModifier( NounShip::FRIGATE ));
	}

	const char * getName() const 
	{ 
		return "Frigate Ally";
	}
	virtual const char * getIconName() const
	{
		return "interface/navigation/frigate.prt";
	}
	bool canBuy() const
	{
		 return true;
	}	
	int buyValue() const 
	{ 
		return 100; 
	}
	ApplyType applyType() const 
	{ 
		return AT_USE;
	}
	Rarity rarity()  const 
	{
		return RARITY_COMMON;
	}
};

class DestroyerAlly : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	DestroyerAlly()
	{
		m_Modifiers.push_back(new SpawnShipModifier( NounShip::DESTROYER ));
	}

	const char * getName() const 
	{ 
		return "Destroyer Ally";
	}
	virtual const char * getIconName() const
	{
		return "interface/navigation/destroyer.prt";
	}
	bool canBuy() const
	{
		 return true;
	}	
	int buyValue() const 
	{ 
		return 200; 
	}
	ApplyType applyType() const 
	{ 
		return AT_USE;
	}
	Rarity rarity()  const 
	{
		return RARITY_COMMON;
	}
};

class SupplyAlly : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	SupplyAlly()
	{
		m_Modifiers.push_back(new SpawnShipModifier( NounShip::SUPPLY ));
	}

	const char * getName() const 
	{ 
		return "Supply Ally";
	}
	virtual const char * getIconName() const
	{
		return "interface/navigation/supply.prt";
	}
	bool canBuy() const
	{
		 return true;
	}	
	int buyValue() const 
	{ 
		return 300; 
	}
	ApplyType applyType() const 
	{ 
		return AT_USE;
	}
	Rarity rarity()  const 
	{
		return RARITY_COMMON;
	}
};

class CruiserAlly : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	CruiserAlly()
	{
		m_Modifiers.push_back(new SpawnShipModifier( NounShip::CRUISER ));
	}

	const char * getName() const 
	{ 
		return "Cruiser Ally";
	}
	virtual const char * getIconName() const
	{
		return "interface/navigation/cruiser.prt";
	}
	bool canBuy() const
	{
		 return true;
	}	
	int buyValue() const 
	{ 
		return 400; 
	}
	ApplyType applyType() const 
	{ 
		return AT_USE;
	}
	Rarity rarity()  const 
	{
		return RARITY_COMMON;
	}
};

class DreadAlly : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	DreadAlly()
	{
		m_Modifiers.push_back(new SpawnShipModifier( NounShip::DREAD ));
	}

	const char * getName() const 
	{ 
		return "Dreadnaught Ally";
	}
	virtual const char * getIconName() const
	{
		return "interface/navigation/dread.prt";
	}
	bool canBuy() const
	{
		 return true;
	} 	
	int buyValue() const 
	{ 
		return 800; 
	}
	ApplyType applyType() const 
	{ 
		return AT_USE;
	}
	Rarity rarity()  const 
	{
		return RARITY_COMMON;
	}
};

class StationAlly : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StationAlly()
	{
		m_Modifiers.push_back(new SpawnShipModifier( NounShip::STATION ));
	}

	const char * getName() const 
	{ 
		return "Station Ally";
	}
	virtual const char * getIconName() const
	{
		return "interface/navigation/station.prt";
	}
	int buyValue() const 
	{ 
		return 1600; 
	}
	ApplyType applyType() const 
	{ 
		return AT_USE;
	}
	Rarity rarity()  const 
	{
		return RARITY_COMMON;
	}
};

//---------------------------------------------------------------------------------------------------

class UpgradedBuildDrones : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	UpgradedBuildDrones()
	{
		m_Modifiers.push_back(new ShipModifier(MT_BUILD_SPEED,10));
	}
	const char * getName() const 
	{ 
		return "Upgraded Build Drones";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_buildicon.prt";
	}
	bool canBuy() const
	{
		 return true;
	}	
	int buyValue() const 
	{ 
		return 1500; 
	}
	u8 maxDamage() const 
	{ 
		return 20; 
	}
	dword requirementFlags() const 
	{
		return REQ_ENGINEER;
	} 
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
};

//---------------------------------------------------------------------------------------------------

class TrailColorModifier : public EnhancementModifier
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	//! Types
	enum TrailColors {
		TRAIL_PURPLE				= 0,
		TRAIL_GREEN					= 1,
		TRAIL_RED					= 2,
		TRAIL_BLUE					= 3,
		TRAIL_WHITE					= 4,
		TRAIL_RAINBOW				= 5,
		TRAIL_PINK					= 6,
	};


	//! Construction
	TrailColorModifier( TrailColors a_eColor = TRAIL_PURPLE ) : m_eColor( a_eColor )
	{}

	//! EnhancementModifier interface
	virtual CharString	getDescription( NounShip * a_pShip );
	virtual bool		canAttach( NounShip * a_pShip );
	virtual bool		onApply( NounShip * a_pShip );
	virtual void		onRemove( NounShip * a_pShip );

protected:
	//! Data
	TrailColors			m_eColor;
};
IMPLEMENT_RAW_STREAMING( TrailColorModifier::TrailColors );

class DeepPurpleExhaust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	DeepPurpleExhaust()
	{
		m_Modifiers.push_back(new TrailColorModifier(TrailColorModifier::TRAIL_PURPLE));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,6));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,3));
		m_Modifiers.push_back(new ShipModifier(MT_TURNRATE,4));
	}

	virtual const char * getName() const 
	{ 
		return "Deep Purple Exhaust";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_trail.prt";
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 2500; 
	}
	virtual Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	virtual bool isUnique() const 
	{ 
		return true; 
	}
};

class ForestGreenExhaust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ForestGreenExhaust()
	{
		m_Modifiers.push_back(new TrailColorModifier(TrailColorModifier::TRAIL_GREEN));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,6));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,3));
		m_Modifiers.push_back(new ShipModifier(MT_TURNRATE,4));
	}

	virtual const char * getName() const 
	{ 
		return "Forest Green Exhaust";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_trail.prt";
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 2500; 
	}
	virtual Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	virtual bool isUnique() const 
	{ 
		return true; 
	}
};

class RedHotExhaust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	RedHotExhaust()
	{
		m_Modifiers.push_back(new TrailColorModifier(TrailColorModifier::TRAIL_RED));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,6));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,3));
		m_Modifiers.push_back(new ShipModifier(MT_TURNRATE,4));
	}

	virtual const char * getName() const 
	{ 
		return "Red Hot Exhaust";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_trail.prt";
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 2500; 
	}
	virtual Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	virtual bool isUnique() const 
	{ 
		return true; 
	}
};

class DarkBlueExhaust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	DarkBlueExhaust()
	{
		m_Modifiers.push_back(new TrailColorModifier(TrailColorModifier::TRAIL_BLUE));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,6));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,3));
		m_Modifiers.push_back(new ShipModifier(MT_TURNRATE,4));
	}

	virtual const char * getName() const 
	{ 
		return "Dark Blue Exhaust";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_trail.prt";
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 2500; 
	}
	virtual Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	virtual bool isUnique() const 
	{ 
		return true; 
	}
};

class WhiteExhaust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	WhiteExhaust()
	{
		m_Modifiers.push_back(new TrailColorModifier(TrailColorModifier::TRAIL_WHITE));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,6));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,3));
		m_Modifiers.push_back(new ShipModifier(MT_TURNRATE,4));
	}

	virtual const char * getName() const 
	{ 
		return "White Exhaust";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_trail.prt";
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 2500; 
	}
	virtual Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	virtual bool isUnique() const 
	{ 
		return true; 
	}
};

class RainbowExhaust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	RainbowExhaust()
	{
		m_Modifiers.push_back(new TrailColorModifier(TrailColorModifier::TRAIL_RAINBOW));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,7));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,5));
		m_Modifiers.push_back(new ShipModifier(MT_TURNRATE,5));
	}

	virtual const char * getName() const 
	{ 
		return "Rainbow Exhaust";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_trail.prt";
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 6000; 
	}
	virtual Rarity rarity()  const 
	{
		return RARITY_ULTRA_RARE;
	}
	virtual bool isUnique() const 
	{ 
		return true; 
	}
};

class DeepPinkGradExhaust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	DeepPinkGradExhaust()
	{
		m_Modifiers.push_back(new TrailColorModifier(TrailColorModifier::TRAIL_PINK));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,6));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,3));
		m_Modifiers.push_back(new ShipModifier(MT_TURNRATE,4));
	}

	virtual const char * getName() const 
	{ 
		return "Deep Pink Gradient Exhaust";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_trail.prt";
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 3000; 
	}
	virtual Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	virtual bool isUnique() const 
	{ 
		return true; 
	}
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
