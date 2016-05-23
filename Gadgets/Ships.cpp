/*
	Ships.cpp

	Ship factories
	(c)2008 Palestar, Richard Lyle
*/

#include "ShipScout.h"
#include "ShipFrigate.h"
#include "ShipDestroyer.h"
#include "ShipCruiser.h"
#include "ShipDread.h"
#include "ShipDread2.h"
#include "ShipTransport.h"
#include "ShipEngineer.h"
#include "ShipStation.h"
#include "ShipSupply.h"
#include "ShipOrb.h"
#include "ShipNode.h"

#include "PlatformSupply.h"
#include "PlatformDefense.h"
#include "PlatformSensor.h"

#include "DarkSpace/GameContext.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_LIGHT_FACTORY( ShipScout,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipFrigate,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipDestroyer,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipCruiser,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipDread,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipDread2,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipTransport,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipEngineer,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipStation,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipSupply,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipOrb,  NounShip  );
IMPLEMENT_LIGHT_FACTORY( ShipNode,  NounShip  );

IMPLEMENT_LIGHT_FACTORY( PlatformSupply,  ShipPlatform  );
IMPLEMENT_LIGHT_FACTORY( PlatformDefense,  ShipPlatform  );
IMPLEMENT_LIGHT_FACTORY( PlatformSensor,  ShipPlatform  );

//---------------------------------------------------------------------------------------------------

REGISTER_FACTORY_KEY( ShipScout, 4134481056980778456LL );
REGISTER_FACTORY_KEY( ShipFrigate, 4134482421166913763LL );
REGISTER_FACTORY_KEY( ShipDestroyer, 4134492530746159390LL );
REGISTER_FACTORY_KEY( ShipCruiser, 4134492549914846131LL );
REGISTER_FACTORY_KEY( ShipDread, 4134492572795831926LL );
REGISTER_FACTORY_KEY( ShipDread2, 4808274212439059536LL );
REGISTER_FACTORY_KEY( ShipTransport, 4134492600475277960LL );
REGISTER_FACTORY_KEY( ShipEngineer, 4298831235611907573LL );
REGISTER_FACTORY_KEY( ShipStation, 4134492580337929012LL );
REGISTER_FACTORY_KEY( ShipSupply, 4225865236091586780LL );
REGISTER_FACTORY_KEY( ShipOrb, 4390533631090757001LL );

//---------------------------------------------------------------------------------------------------
// EOF
