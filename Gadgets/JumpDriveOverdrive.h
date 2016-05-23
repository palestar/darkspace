/*
	JumpDriveOverdrive.h
	(c)2000 Palestar Development, Richard Lyle
*/

#ifndef JUMP_DRIVE_OVERDRIVE_H
#define JUMP_DRIVE_OVERDRIVE_H

#include "DarkSpace/GadgetJumpDrive.h"



//----------------------------------------------------------------------------

class JumpDriveOverdrive : public GadgetJumpDrive
{
public:
	DECLARE_WIDGET_CLASS();

	// NounGadget interface
	int	maxDamage() const
	{
		return 37000 + (level() * 875);
	}
	Type type() const
	{
		return DRIVE_JUMP;
	}
	int addValue() const
	{
		return 12000;
	}
	int buildTechnology() const
	{
		return 25;
	}
	int	buildCost() const
	{
		return 250;
	}
	dword buildFlags() const
	{
		return NounPlanet::FLAG_METALS;
	}
	int	buildTime() const
	{
		return 150;
	}

	// GadgetJumpDrive interface
	int energyCost() const
	{
		return 1200 + (level() * 5);
	}
	int energyCharge() const
	{
		return 1;
	}
	int	maxFuel() const
	{
		return 600 + (level() * 150);
	}
	int refuelRate() const
	{
		return 100;
	}
	int fuelCost() const
	{
		return 1;
	}
	float velocity() const
	{	
		return (90.0f - (level() * 10.0f));
	}
	float thrust() const
	{
		return 150.0f + (level() * 5.0f);
	}
	dword hotkey() const
	{
		return( 'O' );
	}

};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
