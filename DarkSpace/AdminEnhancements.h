/**
	@file AdminEnhancements.h
	@brief Admin-only enhancements.

	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/6/2010 8:20:05 PM
*/

#ifndef ADMINENHANCEMENTS_H
#define ADMINENHANCEMENTS_H

#include "NounEnhancement.h"

//---------------------------------------------------------------------------------------------------
class PantheonJumpCore : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	PantheonJumpCore()
	{
		m_Modifiers.push_back( new ShipModifier( MT_JUMPCOOLDOWN, 1000 ) ); 	// 1000% increase
		m_Modifiers.push_back( new ShipModifier( MT_JUMPSPEED, 500 ) ); 	// 500% increase
	}

	virtual const char * getName() const 
	{ 
		return "Pantheon Jump Core";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_shipshield.prt";
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ADMIN;
	} 
	virtual Rarity rarity()  const 
	{
		return RARITY_ULTRA_RARE;
	}
};

class MavericksWeaponDamage : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MavericksWeaponDamage()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 100000));		// 100,000% increase!
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_DAMAGE, 100000));
	}

	virtual const char * getName() const 
	{ 
		return "Maverick's Ultimate Weapon Multiplexer";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_beamdamage.prt";
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ADMIN;
	} 
	virtual Rarity rarity()  const 
	{
		return RARITY_ULTRA_RARE;
	}
};

class MavericksUltimateBuild : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MavericksUltimateBuild()
	{
		m_Modifiers.push_back(new ShipModifier(MT_BUILD_SPEED,100));
	}

	virtual const char * getName() const 
	{ 
		return "Mavericks Ultimate Build Computer";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_buildicon.prt";
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ADMIN;
	} 
	virtual Rarity rarity()  const 
	{
		return RARITY_ULTRA_RARE;
	}
};

class MavericksDefense : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MavericksDefense()
	{
		m_Modifiers.push_back(new ShipModifier(MT_DAMAGE_REDUCTION,100));
		m_Modifiers.push_back(new ShipModifier(MT_CAPTURE_DEFENSE,100));
	}

	virtual const char * getName() const 
	{ 
		return "Maverick's Ultimate Defense";
	}
	virtual const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
	}
	virtual bool isUnique() const 
	{
		return true;
	}
	virtual dword requirementFlags() const 
	{
		return REQ_ADMIN;
	} 
	virtual Rarity rarity()  const 
	{
		return RARITY_ULTRA_RARE;
	}
};

class DragonHeart : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	DragonHeart()
	{
		m_Modifiers.push_back(new ShipModifier(MT_JUMPCOOLDOWN, 500));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY, 250));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST, 250));
		m_Modifiers.push_back(new ShipModifier(MT_TURNRATE, 100));
		m_Modifiers.push_back(new ShipModifier(MT_DAMAGE_REDUCTION, 20));
		m_Modifiers.push_back(new ShipModifier(MT_BUILD_SPEED, 50));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_DAMAGE, 50));
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 50));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_RANGE, 50));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_ENERGY, 50));
	}

	virtual const char * getName() const
	{
		return "The Dragon's Heart";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_shipshield.prt";
	}
	virtual dword requirementFlags() const
	{
		return REQ_ADMIN;
	}
	virtual Rarity rarity()  const
	{
		return RARITY_ULTRA_RARE;
	}
};

//---------------------------------------------------------------------------------------------------

#endif

//---------------------------------------------------------------------------------------------------
//EOF
