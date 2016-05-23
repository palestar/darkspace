/**
	@file ShipEnhancements.h
	@brief TODO
	
	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/6/2010 9:41:25 PM
*/

#ifndef SHIPENHANCEMENTS_H
#define SHIPENHANCEMENTS_H

#include "NounEnhancement.h"

//---------------------------------------------------------------------------------------------------

class MinorVelocity : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorVelocity()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_VELOCITY, 1 ) );
	}

	const char * getName() const 
	{ 
		return "Minor Engine Tuner";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipvelocity.prt"; 
	}
};

class MinorThrust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorThrust()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_THRUST, 1 ) );
	}

	const char * getName() const 
	{ 
		return "Minor Engine Booster";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipthrust.prt"; 
	}
};

class MinorTurn : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorTurn()
	{
		m_Modifiers.push_back( new ShipModifier( MT_TURNRATE, 1 ) );
	}

	const char * getName() const 
	{ 
		return "Minor Manuevering Jets";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipturn.prt"; 
	}
};

class MinorArmor : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorArmor()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE, 1 ) );
	}

	const char * getName() const 
	{ 
		return "Minor Defense Upgrade";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shiparmor.prt"; 
	}
};

class MinorShield : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorShield()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE_RECHARGE, 1 ) );
	}

	const char * getName() const 
	{ 
		return "Minor Defense Power";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
	}
};

class MinorEwarRange : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorEwarRange()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_RANGE, 1) );
	}

	const char * getName() const
	{
		return "Minor Signal Modulator";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
};

class MinorEwarStrength : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorEwarStrength()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_STRENGTH, 1) );
	}

	const char * getName() const
	{
		return "Minor Signal Amplifier";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
};

class MinorEwarEnergy : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorEwarEnergy()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_ENERGY, 1) );
	}

	const char * getName() const
	{
		return "Minor Signal Dynamo";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
};

// Limited Ship ---------------------------------------------------------------//

class LimitedVelocity : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedVelocity()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_VELOCITY, 2 ) );
	}

	const char * getName() const 
	{ 
		return "Limited Engine Tuner";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipvelocity.prt"; 
	}
};

class LimitedThrust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedThrust()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_THRUST, 2 ) );
	}

	const char * getName() const 
	{ 
		return "Limited Engine Booster";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipthrust.prt"; 
	}
};

class LimitedTurn : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedTurn()
	{
		m_Modifiers.push_back( new ShipModifier( MT_TURNRATE, 2 ) );
	}

	const char * getName() const 
	{ 
		return "Limited Manuevering Jets";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipturn.prt"; 
	}
};

class LimitedArmor : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedArmor()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE, 2 ) );
	}

	const char * getName() const 
	{ 
		return "Limited Defense Upgrade";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shiparmor.prt"; 
	}
};

class LimitedShield : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedShield()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE_RECHARGE, 2 ) );
	}

	const char * getName() const 
	{ 
		return "Limited Defense Power";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
	}
};

class LimitedEwarRange : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedEwarRange()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_RANGE, 2) );
	}

	const char * getName() const
	{
		return "Limited Signal Modulator";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
};

class LimitedEwarStrength : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedEwarStrength()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_STRENGTH, 2) );
	}

	const char * getName() const
	{
		return "Limited Signal Amplifier";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
};

class LimitedEwarEnergy : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedEwarEnergy()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_ENERGY, 2) );
	}

	const char * getName() const
	{
		return "Limited Signal Dynamo";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
};

// Standard Ship ---------------------------------------------------------------//

class StandardVelocity : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardVelocity()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_VELOCITY, 3 ) );
	}

	const char * getName() const 
	{ 
		return "Standard Engine Tuner";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipvelocity.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 100; 
	}
	u8 maxDamage() const 
	{ 
		return 35; 
	}
};

class StandardThrust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardThrust()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_THRUST, 3 ) );
	}

	const char * getName() const 
	{ 
		return "Standard Engine Booster";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipthrust.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 100; 
	}
	u8 maxDamage() const 
	{ 
		return 35; 
	}
};

class StandardTurn : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardTurn()
	{
		m_Modifiers.push_back( new ShipModifier( MT_TURNRATE, 3 ) );
	}

	const char * getName() const 
	{ 
		return "Standard Manuevering Jets";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipturn.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 100; 
	}
	u8 maxDamage() const 
	{ 
		return 35; 
	}
};

class StandardArmor : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardArmor()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE, 3 ) );
	}

	const char * getName() const 
	{ 
		return "Standard Defense Upgrade";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shiparmor.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 100; 
	}
	u8 maxDamage() const 
	{ 
		return 35; 
	}
};

class StandardShield : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardShield()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE_RECHARGE, 3 ) );
	}

	const char * getName() const 
	{ 
		return "Standard Defense Power";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 100; 
	}
	u8 maxDamage() const 
	{ 
		return 35; 
	}
};

class StandardEwarRange : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardEwarRange()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_RANGE, 3) );
	}

	const char * getName() const
	{
		return "Standard Signal Modulator";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 100; 
	}
	u8 maxDamage() const 
	{ 
		return 35; 
	}
};

class StandardEwarStrength : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardEwarStrength()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_STRENGTH, 3) );
	}

	const char * getName() const
	{
		return "Standard Signal Amplifier";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 100; 
	}
	u8 maxDamage() const 
	{ 
		return 35; 
	}
};

class StandardEwarEnergy : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardEwarEnergy()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_ENERGY, 3) );
	}

	const char * getName() const
	{
		return "Standard Signal Dynamo";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 100; 
	}
	u8 maxDamage() const 
	{ 
		return 35; 
	}
};

// Improved Ship ---------------------------------------------------------------//

class ImprovedVelocity : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedVelocity()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_VELOCITY, 4 ) );
	}

	const char * getName() const 
	{ 
		return "Improved Engine Tuner";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipvelocity.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 200; 
	}
	u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedThrust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedThrust()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_THRUST, 4 ) );
	}

	const char * getName() const 
	{ 
		return "Improved Engine Booster";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipthrust.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 200; 
	}
	u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedTurn : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedTurn()
	{
		m_Modifiers.push_back( new ShipModifier( MT_TURNRATE, 4 ) );
	}

	const char * getName() const 
	{ 
		return "Improved Manuevering Jets";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipturn.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 200; 
	}
	u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedArmor : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedArmor()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE, 4 ) );
	}

	const char * getName() const 
	{ 
		return "Improved Defense Upgrade";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shiparmor.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 200; 
	}
	u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedShield : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedShield()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE_RECHARGE, 4 ) );
	}

	const char * getName() const 
	{ 
		return "Improved Defense Power";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 200; 
	}
	u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedEwarRange : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedEwarRange()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_RANGE, 4) );
	}

	const char * getName() const
	{
		return "Improved Signal Modulator";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 200; 
	}
	u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedEwarStrength : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedEwarStrength()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_STRENGTH, 4) );
	}

	const char * getName() const
	{
		return "Improved Signal Amplifier";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 200; 
	}
	u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedEwarEnergy : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedEwarEnergy()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_ENERGY, 4) );
	}

	const char * getName() const
	{
		return "Improved Signal Dynamo";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 200; 
	}
	u8 maxDamage() const 
	{ 
		return 30; 
	}
};

// Enhanced Ship ---------------------------------------------------------------//

class EnhancedVelocity : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedVelocity()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_VELOCITY, 5 ) );
	}

	const char * getName() const 
	{ 
		return "Enhanced Engine Tuner";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipvelocity.prt"; 
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

class EnhancedThrust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedThrust()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_THRUST, 5 ) );
	}

	const char * getName() const 
	{ 
		return "Enhanced Engine Booster";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipthrust.prt"; 
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

class EnhancedTurn : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedTurn()
	{
		m_Modifiers.push_back( new ShipModifier( MT_TURNRATE, 5 ) );
	}

	const char * getName() const 
	{ 
		return "Enhanced Manuevering Jets";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipturn.prt"; 
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

class EnhancedArmor : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedArmor()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE, 5 ) );
	}

	const char * getName() const 
	{ 
		return "Enhanced Defense Upgrade";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shiparmor.prt"; 
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

class EnhancedShield : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedShield()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE_RECHARGE, 5 ) );
	}

	const char * getName() const 
	{ 
		return "Enhanced Defense Power";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
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

class EnhancedEwarRange : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedEwarRange()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_RANGE, 5) );
	}

	const char * getName() const
	{
		return "Enhanced Signal Modulator";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
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

class EnhancedEwarStrength : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedEwarStrength()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_STRENGTH, 5) );
	}

	const char * getName() const
	{
		return "Enhanced Signal Amplifier";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
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

class EnhancedEwarEnergy : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedEwarEnergy()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_ENERGY, 5) );
	}

	const char * getName() const
	{
		return "Enhanced Signal Dynamo";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
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


// Advanced Ship ---------------------------------------------------------------//

class AdvancedVelocity : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedVelocity()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_VELOCITY, 6 ) );
	}

	const char * getName() const 
	{ 
		return "Advanced Engine Tuner";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipvelocity.prt"; 
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
	Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedThrust : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedThrust()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DRIVE_THRUST, 6 ) );
	}

	const char * getName() const 
	{ 
		return "Advanced Engine Booster";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipthrust.prt"; 
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
	Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedTurn : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedTurn()
	{
		m_Modifiers.push_back( new ShipModifier( MT_TURNRATE, 6 ) );
	}

	const char * getName() const 
	{ 
		return "Advanced Manuevering Jets";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipturn.prt"; 
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
	Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedArmor : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedArmor()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE, 6 ) );
	}

	const char * getName() const 
	{ 
		return "Advanced Defense Upgrade";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shiparmor.prt"; 
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
	Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedShield : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedShield()
	{
		m_Modifiers.push_back( new ShipModifier( MT_DEFENSE_RECHARGE, 6 ) );
	}

	const char * getName() const 
	{ 
		return "Advanced Defense Power";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
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
	Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedEwarRange : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedEwarRange()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_RANGE, 6) );
	}

	const char * getName() const
	{
		return "Advanced Signal Modulator";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
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
	Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedEwarStrength : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedEwarStrength()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_STRENGTH, 6) );
	}

	const char * getName() const
	{
		return "Advanced Signal Amplifier";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
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
	Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

class AdvancedEwarEnergy : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedEwarEnergy()
	{
		m_Modifiers.push_back( new ShipModifier( MT_EWAR_ENERGY, 6) );
	}

	const char * getName() const
	{
		return "Advanced Signal Dynamo";
	}

	const char * getIconName() const
	{
		return "interface/icons/enh_ewar.prt";
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
	Rarity rarity() const 
	{
		return RARITY_RARE;
	}
};

//---------------------------------------------------------------------------------------------------

class MakkarShipBooster : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MakkarShipBooster()
	{
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,5));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,5));
		m_Modifiers.push_back(new ShipModifier(MT_TURNRATE,5));
		m_Modifiers.push_back(new ShipModifier(MT_DEFENSE,-2));
	}

	const char * getName() const 
	{ 
		return "Makkar Ship Booster";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipvelocity.prt"; 
	}
	dword requirementFlags() const 
	{
		return REQ_DESTROYER|REQ_CRUISER|REQ_DREAD|REQ_STATION;
	} 
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 800; 
	}
	virtual bool isUnique() const
	{
		return true;
	}
};

class ShipProtector : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ShipProtector()
	{
		m_Modifiers.push_back(new ShipModifier(MT_DEFENSE,7));
	}

	const char * getName() const 
	{ 
		return "Benchford Buish Ship Protector";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shiparmor.prt"; 
	}
	bool isUnique() const 
	{
		return true;
	}
	dword requirementFlags() const 
	{
		return REQ_DESTROYER|REQ_CRUISER|REQ_DREAD|REQ_STATION;
	} 
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 1000; 
	}
};

class ShipProtector2 : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ShipProtector2()
	{
		m_Modifiers.push_back(new ShipModifier(MT_DEFENSE,9));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,-6));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,-6));
	}

	const char * getName() const 
	{ 
		return "Zlarr's Defense";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shiparmor.prt"; 
	}
	bool isUnique() const 
	{
		return true;
	}
	dword requirementFlags() const 
	{
		return REQ_DESTROYER|REQ_CRUISER|REQ_DREAD;
	} 
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 1250; 
	}
};

class ShipProtector3 : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ShipProtector3()
	{
		m_Modifiers.push_back(new ShipModifier(MT_DEFENSE,4));
	}

	const char * getName() const 
	{ 
		return "Magnetron Shield MkI";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 1500; 
	}
	bool isUnique() const 
	{
		return true;
	}
	dword requirementFlags() const 
	{
		return REQ_SCOUT|REQ_TRANSPORT|REQ_ENGINEER;
	}
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
};

class ShipProtector4 : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ShipProtector4()
	{
		m_Modifiers.push_back(new ShipModifier(MT_DEFENSE,5));
	}

	const char * getName() const 
	{ 
		return "Magnetron Shield MkII";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 2000; 
	}
	bool isUnique() const 
	{
		return true;
	}
	dword requirementFlags() const 
	{
		return REQ_SCOUT|REQ_TRANSPORT|REQ_ENGINEER;
	}
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
};

class ShipProtector5 : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ShipProtector5()
	{
		m_Modifiers.push_back(new ShipModifier(MT_DEFENSE,7));
	}

	const char * getName() const 
	{ 
		return "Magnetron Shield MkIII";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 2500; 
	}
	bool isUnique() const 
	{
		return true;
	}
	dword requirementFlags() const 
	{
		return REQ_SCOUT|REQ_TRANSPORT|REQ_ENGINEER;
	}
	Rarity rarity()  const 
	{
		return RARITY_ULTRA_RARE;
	}
};

class ArmorOverhaul : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ArmorOverhaul()
	{
		m_Modifiers.push_back(new ShipModifier(MT_DEFENSE,6));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,-4));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,-4));
	}

	const char * getName() const 
	{ 
		return "Rogen's Armor Overhaul";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shiparmor.prt"; 
	}
	bool isUnique() const 
	{
		return true;
	}
	dword requirementFlags() const 
	{
		return REQ_SCOUT|REQ_FRIGATE|REQ_DESTROYER|REQ_CRUISER;
	} 
	Rarity rarity()  const 
	{
		return RARITY_ULTRA_RARE;
	}
};

class StealthModule : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StealthModule()
	{
		m_Modifiers.push_back(new ShipModifier(MT_CLOAK,15));
		m_Modifiers.push_back(new ShipModifier(MT_SIGNATURE,15));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_VELOCITY,-5));
		m_Modifiers.push_back(new ShipModifier(MT_DRIVE_THRUST,-10));
	}

	const char * getName() const 
	{ 
		return "Stealth Module";
	}
	const char * getIconName() const 
	{ 
		return "interface/icons/enh_shipshield.prt"; 
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 2500; 
	}
	bool isUnique() const 
	{
		return true;
	}
	dword requirementFlags() const 
	{
		return 0;
	} 
	Rarity rarity()  const 
	{
		return RARITY_ULTRA_RARE;
	}
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
