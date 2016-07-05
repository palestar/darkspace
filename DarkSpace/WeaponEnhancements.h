/**
	@file WeaponEnhancements.h
	@brief TODO
	
	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/6/2010 9:41:08 PM
*/

#ifndef WEAPONENHANCEMENTS_H
#define WEAPONENHANCEMENTS_H

#include "NounEnhancement.h"

//---------------------------------------------------------------------------------------------------

class MinorWeaponAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorWeaponAccelerator()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_RANGE, 1 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Minor Weapon Accelerator";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponrange.prt";
	}
};

class MinorWeaponCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorWeaponCooler()
	{
		//Changed all "cooler" enhancements to use MT_WEAPON_ENERGY as that is what actually
		//decreases the cooldown time and MT_WEAPON_COOLDOWN does nothing.
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_COOLDOWN, 1 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 1 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Minor Weapon Cooler";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponcooldown.prt";
	}
};

class MinorWeaponMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorWeaponMultiplexer()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 1 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Minor Weapon Multiplexer";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weapondamage.prt";
	}
};

class MinorWeaponCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorWeaponCondenser()
	{
		//Changed all "condenser" enhancements to match the same modifier arrangement as beam condensers...
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 1 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 1 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Minor Weapon Condenser";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponenergy.prt";
	}
};

// Limited Weapon ---------------------------------------------------------------//

class LimitedWeaponAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedWeaponAccelerator()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_RANGE, 2 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Limited Weapon Accelerator";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponrange.prt";
	}
};

class LimitedWeaponCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedWeaponCooler()
	{
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_COOLDOWN, 2 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 2 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Limited Weapon Cooler";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponcooldown.prt";
	}
};

class LimitedWeaponMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedWeaponMultiplexer()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 2 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Limited Weapon Multiplexer";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weapondamage.prt";
	}
};

class LimitedWeaponCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedWeaponCondenser()
	{
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 2 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 2 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 1 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Limited Weapon Condenser";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponenergy.prt";
	}
};

// Standard Weapon ---------------------------------------------------------------//

class StandardWeaponAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardWeaponAccelerator()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_RANGE, 3 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Standard Weapon Accelerator";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponrange.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 100; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 35; 
	}
};

class StandardWeaponCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardWeaponCooler()
	{
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_COOLDOWN, 3 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 3 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Standard Weapon Cooler";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponcooldown.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 100; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 35; 
	}
};

class StandardWeaponMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardWeaponMultiplexer()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 3 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Standard Weapon Multiplexer";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weapondamage.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 100; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 35; 
	}
};

class StandardWeaponCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardWeaponCondenser()
	{
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 3 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 2 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 2 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Standard Weapon Condenser";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponenergy.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 100; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 35; 
	}
};

// Improved Weapon ---------------------------------------------------------------//

class ImprovedWeaponAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedWeaponAccelerator()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_RANGE, 4 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Improved Weapon Accelerator";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponrange.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 200; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedWeaponCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedWeaponCooler()
	{
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_COOLDOWN, 4 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 4 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Improved Weapon Cooler";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponcooldown.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 200; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedWeaponMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedWeaponMultiplexer()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 4 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Improved Weapon Multiplexer";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weapondamage.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 200; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedWeaponCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedWeaponCondenser()
	{
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 4 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 3 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 2 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Improved Weapon Condenser";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponenergy.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 200; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 30; 
	}
};

// Enhanced Weapon ---------------------------------------------------------------//

class EnhancedWeaponAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedWeaponAccelerator()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_RANGE, 5 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Enhanced Weapon Accelerator";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponrange.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 400; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 25; 
	}
};

class EnhancedWeaponCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedWeaponCooler()
	{
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_COOLDOWN, 5 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 5 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Enhanced Weapon Cooler";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponcooldown.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 400; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 25; 
	}
};

class EnhancedWeaponMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedWeaponMultiplexer()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 5 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Enhanced Weapon Multiplexer";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weapondamage.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 400; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 25; 
	}
};

class EnhancedWeaponCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedWeaponCondenser()
	{
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 5 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 3 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 3 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Enhanced Weapon Condenser";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponenergy.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 400; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 25; 
	}
};

class EnhancedWeaponAmmo : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedWeaponAmmo()
	{
		m_Modifiers.push_back( new ShipModifier( MT_WEAPON_AMMO, 5 ) );
	}

	const char * getName() const 
	{ 
		return "Enhanced Weapon Rack";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_weaponrange.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}	
	int buyValue() const 
	{ 
		return 400; 
	}
	u8 maxDamage() const 
	{ 
		return 25; 
	}
};

// Advanced Weapon ---------------------------------------------------------------//

class AdvancedWeaponAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedWeaponAccelerator()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_RANGE, 6 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Advanced Weapon Accelerator";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponrange.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 800; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 20; 
	}
	virtual Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedWeaponCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedWeaponCooler()
	{
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_COOLDOWN, 6 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 6 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Advanced Weapon Cooler";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponcooldown.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 800; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 20; 
	}
	virtual Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedWeaponMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedWeaponMultiplexer()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 6 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Advanced Weapon Multiplexer";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weapondamage.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 800; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 20; 
	}
	virtual Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedWeaponCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedWeaponCondenser()
	{
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 6 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY, 4 ) );
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 3 ) );
	}
	virtual const char * getName() const 
	{ 
		return "Advanced Weapon Condenser";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weaponenergy.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	virtual int buyValue() const 
	{ 
		return 800; 
	}
	virtual u8 maxDamage() const 
	{ 
		return 20; 
	}
	virtual Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedWeaponAmmo : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedWeaponAmmo()
	{
		m_Modifiers.push_back( new ShipModifier( MT_WEAPON_AMMO, 10 ) );
	}

	const char * getName() const 
	{ 
		return "Advanced Weapon Rack";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_weaponrange.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}	
	int buyValue() const 
	{ 
		return 800; 
	}
	u8 maxDamage() const 
	{ 
		return 20; 
	}
	virtual Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

//---------------------------------------------------------------------------------------------------

class ZlygsWeaponAugmentation : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ZlygsWeaponAugmentation()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_RANGE,-10));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_RANGE,-10));
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE, 5));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_DAMAGE, 5));
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY,5));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_ENERGY,5));
	}

	virtual const char * getName() const 
	{ 
		return "Zlyg's Weapon Augmentation";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weapondamage.prt";
	}
	virtual bool isUnique() const 
	{
		return true;
	}
	virtual dword requirementFlags() const 
	{
		return REQ_DESTROYER|REQ_CRUISER|REQ_DREAD|REQ_STATION;
	} 
	virtual Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
};


class ThariWeaponBooster : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ThariWeaponBooster()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_RANGE,15));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_RANGE,15));
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE,-5));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_DAMAGE,-5));
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_COOLDOWN,5));
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY,5));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_ENERGY,5));
	}

	virtual const char * getName() const 
	{ 
		return "Thari's Weapon Booster";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weapondamage.prt";
	}
	virtual bool isUnique() const 
	{
		return true;
	}
	virtual Rarity rarity()  const 
	{
		return RARITY_ULTRA_RARE;
	}
};

class BuccaneerWeapon : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	BuccaneerWeapon()
	{
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_DAMAGE,5));
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_RANGE,5));
		//m_Modifiers.push_back(new ShipModifier(MT_WEAPON_COOLDOWN,5));
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY,5));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_ENERGY,-5));
	}

	virtual const char * getName() const 
	{ 
		return "Buccaneer's Weapon Expatiation";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_weapondamage.prt";
	}
	virtual bool isUnique() const 
	{
		return true;
	}
	virtual dword requirementFlags() const 
	{
		return REQ_SCOUT|REQ_FRIGATE|REQ_DESTROYER;
	} 
	virtual Rarity rarity() const 
	{
		return RARITY_ULTRA_RARE;
	}
};

class PrivateerBeam : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	PrivateerBeam()
	{
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_DAMAGE,5));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_RANGE,5));
		m_Modifiers.push_back(new ShipModifier(MT_BEAM_ENERGY,5));
		m_Modifiers.push_back(new ShipModifier(MT_WEAPON_ENERGY,-5));
	}

	virtual const char * getName() const 
	{ 
		return "Privateer's Beam Polarization";
	}
	virtual const char * getIconName() const
	{
		return "interface/icons/enh_beamdamage.prt";
	}
	virtual bool isUnique() const 
	{
		return true;
	}
	virtual dword requirementFlags() const 
	{
		return REQ_SCOUT|REQ_FRIGATE|REQ_DESTROYER;
	} 
	virtual Rarity rarity() const 
	{
		return RARITY_ULTRA_RARE;
	}
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
