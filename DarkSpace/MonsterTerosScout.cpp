/*
	MonsterTerosScout.cpp
	(c)2008 Palestar Inc, Jeff Everett

	All of the Teros Scout ship AI definitions
*/


#include "Debug/Profile.h"
#include "MonsterTerosScout.h"
#include "NounPlanet.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( MonsterTerosScout, MonsterTeros );
IMPLEMENT_FACTORY( MonsterTerosScout2, MonsterTeros );
IMPLEMENT_FACTORY( MonsterTerosScout3, MonsterTeros );
IMPLEMENT_FACTORY( MonsterTerosScout4, MonsterTeros );

BEGIN_PROPERTY_LIST( MonsterTerosScout, MonsterTeros )
END_PROPERTY_LIST();
BEGIN_PROPERTY_LIST( MonsterTerosScout2, MonsterTeros )
END_PROPERTY_LIST();
BEGIN_PROPERTY_LIST( MonsterTerosScout3, MonsterTeros )
END_PROPERTY_LIST();
BEGIN_PROPERTY_LIST( MonsterTerosScout4, MonsterTeros )
END_PROPERTY_LIST();

MonsterTerosScout::MonsterTerosScout() 
{}
MonsterTerosScout2::MonsterTerosScout2() 
{}
MonsterTerosScout3::MonsterTerosScout3() 
{}
MonsterTerosScout4::MonsterTerosScout4() 
{}

//----------------------------------------------------------------------------

const int UPDATE_LOGIC_RATE = TICKS_PER_SECOND * 10;

void MonsterTerosScout::simulate( dword nTick )
{
	bool bUseBaseSimulate = false;
	if(bUseBaseSimulate)
	{
		// do some special logic then call NounShip::simulate
		NounShip::simulate(nTick);
	}
	else
	{
		MonsterTeros::simulate( nTick );
	}
}

void MonsterTerosScout2::simulate( dword nTick )
{
	MonsterTeros::simulate( nTick );
}


void MonsterTerosScout3::simulate( dword nTick )
{
	MonsterTeros::simulate( nTick );
}


void MonsterTerosScout4::simulate( dword nTick )
{
	MonsterTeros::simulate( nTick );
}

//----------------------------------------------------------------------------
//EOF
