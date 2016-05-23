/**
	@file BeamEnhancements.h
	@brief TODO
	
	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/6/2010 9:45:39 PM
*/

#ifndef BEAMENHANCEMENTS_H
#define BEAMENHANCEMENTS_H

#include "NounEnhancement.h"

//---------------------------------------------------------------------------------------------------

class MinorBeamAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorBeamAccelerator()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_RANGE,1 ) );
	}

	const char * getName() const 
	{ 
		return "Minor Beam Accelerator";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamrange.prt";
	}
};

class MinorBeamCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorBeamCooler()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 1 ) );
	}

	const char * getName() const 
	{ 
		return "Minor Beam Cooler";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamenergy.prt";
	}
};

class MinorBeamMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorBeamMultiplexer()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 1 ) );
	}

	const char * getName() const 
	{ 
		return "Minor Beam Multiplexer";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamdamage.prt";
	}
};

class MinorBeamCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	MinorBeamCondenser()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 1 ) );
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 1 ) );
	}

	const char * getName() const 
	{ 
		return "Minor Beam Condenser";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamcooldown.prt";
	}
};


// Limited Beam ---------------------------------------------------------------//

class LimitedBeamAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedBeamAccelerator()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_RANGE,2 ) );
	}

	const char * getName() const 
	{ 
		return "Limited Beam Accelerator";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamrange.prt";
	}
};

class LimitedBeamCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedBeamCooler()	
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 2 ) );
	}

	const char * getName() const 
	{ 
		return "Limited Beam Cooler";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamenergy.prt";
	}
};

class LimitedBeamMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedBeamMultiplexer()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 2 ) );
	}

	const char * getName() const 
	{ 
		return "Limited Beam Multiplexer";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamdamage.prt";
	}
};

class LimitedBeamCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	LimitedBeamCondenser()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 2 ) );
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 1 ) );
	}

	const char * getName() const 
	{ 
		return "Limited Beam Condenser";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamcooldown.prt";
	}
};


// Standard Beam ---------------------------------------------------------------//

class StandardBeamAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardBeamAccelerator()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_RANGE, 3 ) );
	}

	const char * getName() const 
	{ 
		return "Standard Beam Accelerator";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamrange.prt";
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

class StandardBeamCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardBeamCooler()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 3 ) );
	}

	const char * getName() const 
	{ 
		return "Standard Beam Cooler";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamenergy.prt";
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

class StandardBeamMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardBeamMultiplexer()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 3 ) );
	}

	const char * getName() const 
	{ 
		return "Standard Beam Multiplexer";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamdamage.prt";
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

class StandardBeamCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	StandardBeamCondenser()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 2 ) );
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 2 ) );
	}

	const char * getName() const 
	{ 
		return "Standard Beam Condenser";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamcooldown.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 110; 
	};
	u8 maxDamage() const 
	{ 
		return 35; 
	}
};

// Improved Beam ---------------------------------------------------------------//

class ImprovedBeamAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedBeamAccelerator()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_RANGE, 4 ) );
	}

	const char * getName() const 
	{ 
		return "Improved Beam Accelerator";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamrange.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 200; 
	};
	u8 maxDamage() const 
	{ 
		return 30; 
	}
};

class ImprovedBeamCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedBeamCooler()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 4 ) );
	}

	const char * getName() const 
	{ 
		return "Improved Beam Cooler";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamenergy.prt";
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

class ImprovedBeamMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedBeamMultiplexer()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 4 ) );
	}

	const char * getName() const 
	{ 
		return "Improved Beam Multiplexer";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamdamage.prt";
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

class ImprovedBeamCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	ImprovedBeamCondenser()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 3 ) );
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 2 ) );
	}

	const char * getName() const 
	{ 
		return "Improved Beam Condenser";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamcooldown.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 220; 
	}
	u8 maxDamage() const 
	{ 
		return 30; 
	}
};

// Enhanced Beam ---------------------------------------------------------------//

class EnhancedBeamAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedBeamAccelerator()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_RANGE, 5 ) );
	}

	const char * getName() const 
	{ 
		return "Enhanced Beam Accelerator";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamrange.prt";
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

class EnhancedBeamCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedBeamCooler()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 5 ) );
	}

	const char * getName() const 
	{ 
		return "Enhanced Beam Cooler";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamenergy.prt";
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

class EnhancedBeamMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedBeamMultiplexer()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 5 ) );
	}

	const char * getName() const 
	{ 
		return "Enhanced Beam Multiplexer";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamdamage.prt";
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

class EnhancedBeamCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	EnhancedBeamCondenser()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 3 ) );
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 3 ) );
	}

	const char * getName() const 
	{ 
		return "Enhanced Beam Condenser";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamcooldown.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 440; 
	}
	u8 maxDamage() const 
	{ 
		return 25; 
	}
};

// Advanced Beam ---------------------------------------------------------------//

class AdvancedBeamAccelerator : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedBeamAccelerator()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_RANGE, 6 ) );
	}

	const char * getName() const 
	{ 
		return "Advanced Beam Accelerator";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamrange.prt";
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
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
};

class AdvancedBeamCooler : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedBeamCooler()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 6 ) );
	}

	const char * getName() const 
	{ 
		return "Advanced Beam Cooler";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamenergy.prt";
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
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
};

class AdvancedBeamMultiplexer : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedBeamMultiplexer()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 6 ) );
	}

	const char * getName() const 
	{ 
		return "Advanced Beam Multiplexer";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamdamage.prt";
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
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
};

class AdvancedBeamCondenser : public NounEnhancement
{
public:
	DECLARE_WIDGET_CLASS();

	AdvancedBeamCondenser()
	{
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_ENERGY, 4 ) );
		m_Modifiers.push_back( new ShipModifier( MT_BEAM_DAMAGE, 3 ) );
	}

	const char * getName() const 
	{ 
		return "Advanced Beam Condenser";
	}
	const char * getIconName() const
	{
		return "interface/icons/enh_beamcooldown.prt";
	}
	bool canBuy() const
	{
		 return true;
	}
	int buyValue() const 
	{ 
		return 880; 
	}
	u8 maxDamage() const 
	{ 
		return 20; 
	}
	Rarity rarity()  const 
	{
		return RARITY_RARE;
	}
	dword requirementFlags() const 
	{
		return REQ_ANY;
	} 
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
