/*
	Gadgets.cpp

	Gadget factories
	(c)200 Palestar, Richard Lyle
*/

#define GADGETS_DLL

// drives (engines)
#include "Gadgets/DriveAFE.h"
#include "Gadgets/DrivePFE.h"
#include "Gadgets/DriveIE.h"
#include "Gadgets/DrivePIE.h"
#include "Gadgets/DriveAME.h"
#include "Gadgets/Innards.h"
#include "Gadgets/DriveSE.h"
#include "Gadgets/DriveHVIE.h"
#include "Gadgets/DriveHVPIE.h"
#include "Gadgets/DriveHVPFE.h"
#include "Gadgets/DriveHVAFE.h"
#include "Gadgets/DriveHVAME.h"
#include "Gadgets/DriveHVSE.h"

// jump drives
#include "Gadgets/JumpDriveTachyon.h"
#include "Gadgets/JumpDriveAntiMatter.h"
#include "Gadgets/JumpDriveHMA.h"
#include "Gadgets/JumpDriveSE.h"
#include "Gadgets/JumpDriveOverdrive.h"

// shields
#include "Gadgets/ShieldReactive.h"
#include "Gadgets/ShieldActive.h"
#include "Gadgets/ShieldKluthPT.h"
#include "Gadgets/ShieldAuxillary.h"
#include "Gadgets/ShieldPlanetary.h"
#include "Gadgets/ShieldSkirmish.h"
#include "Gadgets/ShieldGhost.h"

// armor
#include "Gadgets/Armor5000.h"
#include "Gadgets/Armor10000.h"
#include "Gadgets/ArmorOrganic.h"
#include "Gadgets/ArmorReflective.h"
#include "Gadgets/ArmorStandard.h"
#include "Gadgets/ArmorFighter.h"
#include "Gadgets/ArmorAblative.h"
#include "Gadgets/ArmorChitin.h"
#include "Gadgets/ArmorMolecular.h"
#include "Gadgets/ArmorComposite.h"
#include "Gadgets/ArmorDusk.h"
#include "Gadgets/ArmorStealth.h"


// special

#include "Gadgets/TractorBeam300.h"
#include "Gadgets/MiningBeam.h"
#include "Gadgets/Cloak100.h"
#include "Gadgets/ECCM1.h"
#include "Gadgets/ECM1.h"
#include "Gadgets/ECM2.h"
#include "Gadgets/ECCM2.h"
#include "Gadgets/NarrowBandTECM.h"
#include "Gadgets/NarrowBandTECCM.h"
#include "Gadgets/FocusedTECM.h"
#include "Gadgets/FocusedTECCM.h"
#include "Gadgets/LongWaveECM.h"
#include "Gadgets/LongWaveECCM.h"
#include "Gadgets/TacticalECM.h"
#include "Gadgets/TacticalECCM.h"
#include "Gadgets/LocalECM.h"
#include "Gadgets/ReactorAux.h"
#include "Gadgets/Reactor500.h"
#include "Gadgets/Reactor1000.h"
#include "Gadgets/Reactor1500.h"
#include "Gadgets/Beacon100.h"
#include "Gadgets/ELF750.h"
#include "Gadgets/EMP500.h"
#include "Gadgets/JD1000.h"
#include "Gadgets/PulseShield.h"
#include "Gadgets/JumpGate1.h"
#include "Gadgets/JumpGate2.h"
#include "Gadgets/SupplyDrones.h"
#include "Gadgets/SupplyDronesLight.h"
#include "Gadgets/SupplyDronesPlanetary.h"
#include "Gadgets/SupplyDronesStation.h"
#include "Gadgets/Scanner1.h"
#include "Gadgets/Scanner2.h"
#include "Gadgets/ScannerDetectCloak.h"
#include "Gadgets/JD2K.h"
#include "Gadgets/DropPod.h"
#include "Gadgets/DropPodMI.h"

// fighters
#include "Gadgets/FighterBay.h"
#include "Gadgets/FighterBayAssault.h"
#include "Gadgets/FighterBayBomber.h"
#include "Gadgets/FighterBayInterceptor.h"
#include "Gadgets/FighterBayScout.h"
#include "Gadgets/FighterBayTorpedo.h"

// hull 
#include "Gadgets/AutoRepair.h"
#include "Gadgets/NaniteInjector.h"
#include "Gadgets/DamageControl.h"

// planets
#include "Gadgets/PlanetArid.h"
#include "Gadgets/PlanetBarren.h"
#include "Gadgets/PlanetGas.h"
#include "Gadgets/PlanetIce.h"
#include "Gadgets/PlanetInferno.h"
#include "Gadgets/PlanetMoon.h"
#include "Gadgets/PlanetOcean.h"
#include "Gadgets/PlanetTerran.h"
#include "Gadgets/PlanetHome.h"

// units
#include "Gadgets/UnitInfantry.h"
#include "Gadgets/UnitHeavyInfantry.h"
#include "Gadgets/UnitRaidingParty.h"
#include "Gadgets/UnitArmor.h"
#include "Gadgets/UnitAutomaton.h"
#include "Gadgets/UnitSpy.h"

#include "DarkSpace/GameContext.h"

//---------------------------------------------------------------------------------------------------

#include "Standard/Dll.h"

// DLL export a version number so that a .lib will be generated for this DLL
DLL_EXPORT dword	GADGET_DLL_VERSION = 0x00010000;

//----------------------------------------------------------------------------

// drives
IMPLEMENT_LIGHT_FACTORY( DriveAFE, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DrivePFE, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DriveIE, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DrivePIE, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DriveAME, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( Innards, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DriveSE, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DriveHVIE, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DriveHVPIE, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DriveHVPFE, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DriveHVAFE, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DriveHVAME, GadgetDrive );
IMPLEMENT_LIGHT_FACTORY( DriveHVSE, GadgetDrive );

// jump drives
IMPLEMENT_LIGHT_FACTORY( JumpDriveTachyon, GadgetJumpDrive );
IMPLEMENT_LIGHT_FACTORY( JumpDriveAntiMatter, GadgetJumpDrive );
IMPLEMENT_LIGHT_FACTORY( JumpDriveHMA, GadgetJumpDrive );
IMPLEMENT_LIGHT_FACTORY( JumpDriveSE, GadgetJumpDrive );
IMPLEMENT_LIGHT_FACTORY( JumpDriveOverdrive, GadgetJumpDrive );

// shields
IMPLEMENT_LIGHT_FACTORY( ShieldReactive, GadgetShield );
IMPLEMENT_LIGHT_FACTORY( ShieldActive, GadgetShield );
IMPLEMENT_LIGHT_FACTORY( ShieldKluthPT, GadgetShield );
IMPLEMENT_LIGHT_FACTORY( ShieldAuxillary, GadgetShield );
IMPLEMENT_LIGHT_FACTORY( ShieldPlanetary, GadgetShield );
IMPLEMENT_LIGHT_FACTORY( ShieldSkirmish, GadgetShield );
IMPLEMENT_LIGHT_FACTORY( ShieldGhost, GadgetShield );

// armor
IMPLEMENT_LIGHT_FACTORY( Armor5000, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( Armor10000, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( ArmorOrganic, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( ArmorReflective, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( ArmorStandard, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( ArmorFighter, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( ArmorAblative, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( ArmorChitin, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( ArmorMolecular, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( ArmorComposite, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( ArmorDusk, GadgetArmor );
IMPLEMENT_LIGHT_FACTORY( ArmorStealth, GadgetArmor );

// special
IMPLEMENT_LIGHT_FACTORY( TractorBeam300, GadgetTractorBeam  );
IMPLEMENT_LIGHT_FACTORY( MiningBeam, GadgetMiningBeam  );
IMPLEMENT_LIGHT_FACTORY( Cloak100, GadgetCloak  );
IMPLEMENT_LIGHT_FACTORY( ECM1, GadgetECM );
IMPLEMENT_LIGHT_FACTORY( ECM2, GadgetECM  );
IMPLEMENT_LIGHT_FACTORY( LongWaveECM, GadgetECM );
IMPLEMENT_LIGHT_FACTORY( NarrowBandTECM, GadgetTECM );
IMPLEMENT_LIGHT_FACTORY( FocusedTECM, GadgetTECM );
IMPLEMENT_LIGHT_FACTORY( ECCM1, GadgetECCM );
IMPLEMENT_LIGHT_FACTORY( ECCM2, GadgetECCM  );
IMPLEMENT_LIGHT_FACTORY( LongWaveECCM, GadgetECCM );
IMPLEMENT_LIGHT_FACTORY( NarrowBandTECCM, GadgetTECCM );
IMPLEMENT_LIGHT_FACTORY( FocusedTECCM, GadgetTECCM );
IMPLEMENT_LIGHT_FACTORY( LocalECM, GadgetECM );
IMPLEMENT_LIGHT_FACTORY( TacticalECCM, GadgetECCM );
IMPLEMENT_LIGHT_FACTORY( TacticalECM, GadgetECM );

IMPLEMENT_LIGHT_FACTORY( ReactorAux, GadgetReactor  );
IMPLEMENT_LIGHT_FACTORY( Reactor500, GadgetReactor  );
IMPLEMENT_LIGHT_FACTORY( Reactor1000, GadgetReactor  );
IMPLEMENT_LIGHT_FACTORY( Reactor1500, GadgetReactor  );
IMPLEMENT_LIGHT_FACTORY( Scanner1, GadgetScanner );
IMPLEMENT_LIGHT_FACTORY( Scanner2, GadgetScanner  );
IMPLEMENT_LIGHT_FACTORY( Beacon100, GadgetBeacon );
IMPLEMENT_LIGHT_FACTORY( ELF750, GadgetELF );
IMPLEMENT_LIGHT_FACTORY( EMP500, GadgetEMP );
IMPLEMENT_LIGHT_FACTORY( JD1000, GadgetJD );
IMPLEMENT_LIGHT_FACTORY( JD2K, GadgetJD );
IMPLEMENT_LIGHT_FACTORY( PulseShield, GadgetPulseShield );
IMPLEMENT_LIGHT_FACTORY( JumpGate1, GadgetJumpGate );
IMPLEMENT_LIGHT_FACTORY( JumpGate2, GadgetJumpGate );
IMPLEMENT_LIGHT_FACTORY( SupplyDrones, GadgetReload );
IMPLEMENT_LIGHT_FACTORY( SupplyDronesLight, GadgetReload );
IMPLEMENT_LIGHT_FACTORY( SupplyDronesPlanetary, GadgetAreaReload );
IMPLEMENT_LIGHT_FACTORY( SupplyDronesStation, GadgetAreaReload );
IMPLEMENT_LIGHT_FACTORY( ScannerDetectCloak, GadgetScanner );
IMPLEMENT_LIGHT_FACTORY( DropPod, GadgetDropPod );
IMPLEMENT_LIGHT_FACTORY( DropPodMI, GadgetDropPod );

// fighterbay
IMPLEMENT_LIGHT_FACTORY( FighterBay, GadgetFighterBay );
IMPLEMENT_LIGHT_FACTORY( FighterBayAssault, GadgetFighterBay );
IMPLEMENT_LIGHT_FACTORY( FighterBayBomber, GadgetFighterBay );
IMPLEMENT_LIGHT_FACTORY( FighterBayInterceptor, GadgetFighterBay );
IMPLEMENT_LIGHT_FACTORY( FighterBayScout, GadgetFighterBay );
IMPLEMENT_LIGHT_FACTORY( FighterBayTorpedo, GadgetFighterBay );

// hull
IMPLEMENT_LIGHT_FACTORY( AutoRepair, GadgetAutoRepair );
IMPLEMENT_LIGHT_FACTORY( NaniteInjector, GadgetAutoRepair );
IMPLEMENT_LIGHT_FACTORY( DamageControl, GadgetAutoRepair );

// planets
IMPLEMENT_LIGHT_FACTORY( PlanetArid, NounPlanet  );
IMPLEMENT_LIGHT_FACTORY( PlanetBarren, NounPlanet  );
IMPLEMENT_LIGHT_FACTORY( PlanetGas, NounPlanet  );
IMPLEMENT_LIGHT_FACTORY( PlanetIce, NounPlanet  );
IMPLEMENT_LIGHT_FACTORY( PlanetInferno, NounPlanet  );
IMPLEMENT_LIGHT_FACTORY( PlanetMoon, NounPlanet  );
IMPLEMENT_LIGHT_FACTORY( PlanetOcean, NounPlanet  );
IMPLEMENT_LIGHT_FACTORY( PlanetTerran, NounPlanet  );
IMPLEMENT_LIGHT_FACTORY( PlanetHome, NounPlanet  );

// units
IMPLEMENT_LIGHT_FACTORY( UnitInfantry, NounUnit  );
IMPLEMENT_LIGHT_FACTORY( UnitHeavyInfantry, NounUnit  );
IMPLEMENT_LIGHT_FACTORY( UnitArmor, NounUnit  );
IMPLEMENT_LIGHT_FACTORY( UnitSpy, NounUnit  );
IMPLEMENT_LIGHT_FACTORY( UnitRaidingParty, NounUnit );
IMPLEMENT_LIGHT_FACTORY( UnitAutomaton, NounUnit );

//---------------------------------------------------------------------------------------------------
// Deprecated - These are here so old data can still be loaded, any new classes do not need to use these macros.

// drives
REGISTER_FACTORY_KEY( DriveAFE, 4099437050842420384LL );
REGISTER_FACTORY_KEY( DrivePFE, 4040955759932336910LL );
REGISTER_FACTORY_KEY( DriveIE, 4099440049324635072LL );
REGISTER_FACTORY_KEY( DriveAME, 4099440766107387844LL );
REGISTER_FACTORY_KEY( Innards, 4342293670845237182LL );
REGISTER_FACTORY_KEY( DriveSE, 4782300370914723402LL );

// jump drives
REGISTER_FACTORY_KEY( JumpDriveTachyon, 4047036734213217210LL );
REGISTER_FACTORY_KEY( JumpDriveAntiMatter, 4099648382958298885LL );
REGISTER_FACTORY_KEY( JumpDriveHMA, 4774562357411634960LL );
REGISTER_FACTORY_KEY( JumpDriveSE, 4786841056657545120LL );

// shields
REGISTER_FACTORY_KEY( ShieldReactive, 4099710586210818492LL );
REGISTER_FACTORY_KEY( ShieldActive, 4100729283535768340LL );
REGISTER_FACTORY_KEY( ShieldAuxillary, 4741077577743448457LL );
REGISTER_FACTORY_KEY( ShieldPlanetary, 4741182737349824262LL );

// armor
REGISTER_FACTORY_KEY( Armor5000, 4137183805467668710LL );
REGISTER_FACTORY_KEY( Armor10000, 4137184072960010671LL );
REGISTER_FACTORY_KEY( ArmorOrganic, 4741194432622789330LL );
REGISTER_FACTORY_KEY( ArmorReflective, 4741194366626993662LL );
REGISTER_FACTORY_KEY( ArmorStandard, 4740791600288790661LL );
REGISTER_FACTORY_KEY( ArmorFighter, 4774091781379863514LL );
REGISTER_FACTORY_KEY( ArmorAblative, 4792593785145316180LL );

// special
REGISTER_FACTORY_KEY( TractorBeam300, 4135777198391030506LL );
REGISTER_FACTORY_KEY( MiningBeam, 4366059357601997137LL );
REGISTER_FACTORY_KEY( Cloak100, 4137096860817805254LL );
REGISTER_FACTORY_KEY( ECM1, 4775931665948831196LL );
REGISTER_FACTORY_KEY( ECM2, 4137144019805459920LL );
REGISTER_FACTORY_KEY( ECCM1, 4775931932204972441LL );
REGISTER_FACTORY_KEY( ECCM2, 4137157418652216769LL );
REGISTER_FACTORY_KEY( ReactorAux, 4749602348983090335LL );
REGISTER_FACTORY_KEY( Reactor500, 4251764431379214566LL );
REGISTER_FACTORY_KEY( Reactor1000, 4251764500703397611LL );
REGISTER_FACTORY_KEY( Reactor1500, 4251764549146232752LL );
REGISTER_FACTORY_KEY( Scanner1, 4775931461259618855LL );
REGISTER_FACTORY_KEY( Scanner2, 4311140094512607959LL );
REGISTER_FACTORY_KEY( Beacon100, 4310006558958019275LL );
REGISTER_FACTORY_KEY( ELF750, 4368370409085292595LL );
REGISTER_FACTORY_KEY( EMP500, 4311149524743672923LL );
REGISTER_FACTORY_KEY( JD1000, 4311166577810181077LL );
REGISTER_FACTORY_KEY( PulseShield, 4311169534862082401LL );
REGISTER_FACTORY_KEY( JumpGate1, 4626290765862839774LL );
REGISTER_FACTORY_KEY( JumpGate2, 4626290792489382544LL );
REGISTER_FACTORY_KEY( SupplyDrones, 4137236340100448318LL );
REGISTER_FACTORY_KEY( ScannerDetectCloak, 4805600880967813014LL );

// fighterbay
REGISTER_FACTORY_KEY( FighterBay, 4296410260161268789LL );
REGISTER_FACTORY_KEY( FighterBayAssault, 4741470902762295561LL );
REGISTER_FACTORY_KEY( FighterBayBomber, 4741470767613930685LL );
REGISTER_FACTORY_KEY( FighterBayInterceptor, 4741470824232499553LL );
REGISTER_FACTORY_KEY( FighterBayScout, 4775932548272137036LL );

// hull
REGISTER_FACTORY_KEY( AutoRepair, 4298545049075442893LL );

// planets
REGISTER_FACTORY_KEY( PlanetArid, 4145260978820732942LL );
REGISTER_FACTORY_KEY( PlanetBarren, 4145260929316094409LL );
REGISTER_FACTORY_KEY( PlanetGas, 4145260926434848870LL );
REGISTER_FACTORY_KEY( PlanetIce, 4145260940782158578LL );
REGISTER_FACTORY_KEY( PlanetInferno, 4145260899127508098LL );
REGISTER_FACTORY_KEY( PlanetOcean, 4145260917919533662LL );
REGISTER_FACTORY_KEY( PlanetTerran, 4145260910666259647LL );

// units
REGISTER_FACTORY_KEY( UnitInfantry, 4221756500576490230LL );
REGISTER_FACTORY_KEY( UnitHeavyInfantry, 4626883208034310163LL );
REGISTER_FACTORY_KEY( UnitArmor, 4221757389705300747LL );
REGISTER_FACTORY_KEY( UnitSpy, 4221757382449108538LL );


//---------------------------------------------------------------------------------------------------
// EOF
