/*
	Structures.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/

#include "Factory/FactoryTypes.h"
#include "StructureDefensePD1.h"
#include "StructureDefensePD2.h"
#include "StructureDefenseMix1.h"
#include "StructureDefenseMix2.h"
#include "StructureDefenseShip1.h"
#include "StructureDefenseShip2.h"
#include "StructureDepot.h"
#include "StructureDome.h"
#include "StructureFarm.h"
#include "StructureFarm2.h"
#include "StructureFarm3.h"
#include "StructureInterdictor.h"
#include "StructureLab.h"
#include "StructureLab2.h"
#include "StructureLab3.h"
#include "StructureMine2.h"
#include "StructureMine3.h"
#include "StructurePower.h"
#include "StructurePower2.h"
#include "StructurePower3.h"
#include "StructureSensor.h"
#include "StructureShield.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( StructureDome, NounStructure );
REGISTER_FACTORY_KEY( StructureDome, 4358655243067392312LL );

IMPLEMENT_FACTORY( StructureFarm, NounStructure );
REGISTER_FACTORY_KEY( StructureFarm, 4358657273399954133LL );
IMPLEMENT_FACTORY( StructureFarm2, StructureFarm );
REGISTER_FACTORY_KEY( StructureFarm2, 4358656418277519855LL );
IMPLEMENT_FACTORY( StructureFarm3, StructureFarm );
REGISTER_FACTORY_KEY( StructureFarm3, 4625197461833557930LL );


IMPLEMENT_FACTORY( StructureLab, NounStructure );
REGISTER_FACTORY_KEY( StructureLab, 4358652871579652080LL );
IMPLEMENT_FACTORY( StructureLab2, StructureLab );
REGISTER_FACTORY_KEY( StructureLab2, 4626264166535343145LL );
IMPLEMENT_FACTORY( StructureLab3, StructureLab );
REGISTER_FACTORY_KEY( StructureLab3, 4626264115237277372LL );

IMPLEMENT_FACTORY( StructureMine2, StructureMine );
REGISTER_FACTORY_KEY( StructureMine2, 4358653480912281105LL );
IMPLEMENT_FACTORY( StructureMine3, StructureMine );
REGISTER_FACTORY_KEY( StructureMine3, 4626281300141920244LL );

IMPLEMENT_FACTORY( StructurePower, NounStructure );
REGISTER_FACTORY_KEY( StructurePower, 4358670436516558872LL );
IMPLEMENT_FACTORY( StructurePower2, StructurePower );
REGISTER_FACTORY_KEY( StructurePower2, 4625213283425213696LL );
IMPLEMENT_FACTORY( StructurePower3, StructurePower );
REGISTER_FACTORY_KEY( StructurePower3, 4625213059273481618LL );

IMPLEMENT_FACTORY( StructureDefensePD1, StructureDefense );
REGISTER_FACTORY_KEY( StructureDefensePD1, 4626226597766694059LL );
IMPLEMENT_FACTORY( StructureDefensePD2, StructureDefense );
REGISTER_FACTORY_KEY( StructureDefensePD2, 4626227516323566368LL );
IMPLEMENT_FACTORY( StructureDefenseMix1, StructureDefense );
IMPLEMENT_FACTORY( StructureDefenseMix2, StructureDefense );
IMPLEMENT_FACTORY( StructureDefenseShip1, StructureDefense );
IMPLEMENT_FACTORY( StructureDefenseShip2, StructureDefense );
IMPLEMENT_FACTORY( StructureDepot, StructureDefense );
REGISTER_FACTORY_KEY( StructureDepot, 4252216246685448146LL );
IMPLEMENT_FACTORY( StructureInterdictor, StructureDefense );
REGISTER_FACTORY_KEY( StructureInterdictor, 4330400532656178242LL );
IMPLEMENT_FACTORY( StructureSensor, StructureDefense );
REGISTER_FACTORY_KEY( StructureSensor, 4330444123067646117LL );
IMPLEMENT_FACTORY( StructureShield, StructureDefense );
REGISTER_FACTORY_KEY( StructureShield, 4252189630951100054LL );

//----------------------------------------------------------------------------
//EOF
