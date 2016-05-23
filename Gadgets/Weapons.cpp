/*
	Weapons.cpp
	(c)2011 Palestar Inc, Richard Lyle
	Modified by Robert Kelford, Feb 2011
*/

//---------------------------------------------------------------------------------------------------

// weapons

//Torps
#include "Gadgets/WeaponFusionTorpedo.h"
#include "Gadgets/WeaponAMtorpedo.h"
#include "Gadgets/WeaponProtonTorpedo.h"
#include "Gadgets/WeaponEtorpedo.h"
#include "Gadgets/WeaponSABOT.h"
#include "Gadgets/WeaponIonTorpedo.h"
#include "Gadgets/WeaponElfTorpedo.h"
#include "Gadgets/WeaponAMtorpedoFighter.h"
#include "Gadgets/WeaponProtonTorpedoFighter.h"
#include "Gadgets/WeaponSABOT2.h"
#include "Gadgets/WeaponTKTorpedo.h"

//Tier 3
#include "Gadgets/WeaponNeutroniumTorpedo.h"
#include "Gadgets/WeaponQST.h"
#include "Gadgets/WeaponVarianceTorpedo.h"

//Siege Torps
#include "Gadgets/WeaponNeutroniumSiegeTorpedo.h"
#include "Gadgets/WeaponQSST.h"
#include "Gadgets/WeaponVarianceSiegeTorpedo.h"

//Cannons
#include "Gadgets/WeaponParticleCannon.h"
#include "Gadgets/WeaponParticleCannonFighter.h"
#include "Gadgets/WeaponParticleCannonHV.h"
#include "Gadgets/WeaponEMPCannon.h"
#include "Gadgets/WeaponEMPCannonHV.h"
#include "Gadgets/WeaponRailgun.h"
#include "Gadgets/WeaponRailgunFighter.h"
#include "Gadgets/WeaponRailgunHV.h"
#include "Gadgets/WeaponPsiCannon.h"
#include "Gadgets/WeaponPsiCannonFighter.h"
#include "Gadgets/WeaponPsiCannonHV.h"
#include "Gadgets/WeaponPlasmaCannon.h"
#include "Gadgets/WeaponPlasmaCannonHV.h"
#include "Gadgets/WeaponGauss.h"
#include "Gadgets/WeaponGaussHV.h"
#include "Gadgets/WeaponAresCannon.h"
#include "Gadgets/WeaponWibbleCannon.h"
#include "Gadgets/WeaponWibbleCannonHV.h"
#include "Gadgets/WeaponDMC.h"
#include "Gadgets/WeaponValenceAutocannon.h"
#include "Gadgets/WeaponValenceAutocannonHV.h"
#include "Gadgets/WeaponSpreadShot.h"

//Tier 3
#include "Gadgets/WeaponHeavyMassDriver.h"
#include "Gadgets/WeaponIonCannon.h"
#include "Gadgets/WeaponStellarIncinerator.h"



//Mines and bombs
#include "Gadgets/WeaponTNmine.h"
#include "Gadgets/WeaponAMmine.h"
#include "Gadgets/WeaponTKBomb.h"
#include "Gadgets/WeaponEMPmine.h"
#include "Gadgets/WeaponBBBomb.h"
#include "Gadgets/WeaponFurybomb.h"
#include "Gadgets/WeaponQSbomb.h"
#include "Gadgets/WeaponBPCbomb.h"
#include "Gadgets/WeaponMiRVbomb.h"
#include "Gadgets/WeaponNeutronbomb.h"
#include "Gadgets/WeaponPurifierBomb.h"


//Missiles
#include "Gadgets/WeaponIEmissile.h"
#include "Gadgets/WeaponIEMissile2.h"
#include "Gadgets/WeaponARmissile.h"
#include "Gadgets/WeaponARMissile2.h"
#include "Gadgets/WeaponPCmissile.h"
#include "Gadgets/WeaponPSGmissile.h"
#include "Gadgets/WeaponPSGMissile2.h"
#include "Gadgets/WeaponASMissile.h"
#include "Gadgets/WeaponGuardianMissile.h"
#include "Gadgets/WeaponPSMissile.h"
#include "Gadgets/WeaponShredderMissile.h"
#include "Gadgets/WeaponShredderMissile2.h"
#include "Gadgets/WeaponPsiwaveSeekerMissile.h"
#include "Gadgets/WeaponShockMissile.h"
#include "Gadgets/WeaponEMPMissile.h"
#include "Gadgets/WeaponEMPMissileHV.h"

// Tier 1 Missiles (Scouts/Frigate)
#include "Gadgets/WeaponAntiRadarMissile.h"
#include "Gadgets/WeaponPeregrineMissile.h"
#include "Gadgets/WeaponSparrowMissile.h"
#include "Gadgets/WeaponPasseridaeMissileLauncher.h"

// Tier 2 Missiles (Destroyers/Cruisers)
#include "Gadgets/WeaponRaptorMissile.h"
#include "Gadgets/WeaponIonTrackerMissile.h"
#include "Gadgets/WeaponPhoenixMissile.h"

// Tier 3 Missiles (Dreadnoughts/Stations)
#include "Gadgets/WeaponLinearDriveMissile.h"
#include "Gadgets/WeaponProtonCruiseMissile.h"
#include "Gadgets/WeaponHarpexMissile.h"

// K'Luth Missiles (self scaling)
#include "Gadgets/WeaponPsionicMissile.h"
#include "Gadgets/WeaponShroudMissile.h"

#include "Gadgets/WeaponTrebuchetSiegeMissile.h"

// beam weapons
#include "Gadgets/WeaponCL.h"
#include "Gadgets/WeaponCLH.h"
#include "Gadgets/WeaponCLPlanet.h"
#include "Gadgets/WeaponDSbeam.h"
#include "Gadgets/WeaponDisruptor.h"
#include "Gadgets/WeaponDisruptorA.h"
#include "Gadgets/WeaponDeathBeam.h"
#include "Gadgets/WeaponFLUXbeam.h"
#include "Gadgets/MouthBeam.h"
#include "Gadgets/WeaponELF.h"
#include "Gadgets/WeaponFEP.h"
#include "Gadgets/WeaponFEPO.h"
#include "Gadgets/WeaponCLICC.h"
#include "Gadgets/WeaponCLHICC.h"

// Point-defense Beams
#include "Gadgets/WeaponPulseBeam.h"
#include "Gadgets/WeaponAmplifiedPulseBeam.h"
#include "Gadgets/WeaponRapidCycleDisruptor.h"
#include "Gadgets/WeaponPsiFocusedDisruptor.h"
#include "Gadgets/WeaponLonginusDefenseBeam.h"
#include "Gadgets/WeaponCLER.h"
#include "Gadgets/WeaponFighterBeam.h"

// area weapons
#include "Gadgets/WeaponFluxWave.h"

// Legacy weapons
#include "Gadgets/WeaponMPA.h"
#include "Gadgets/WeaponHAMmine.h"
#include "Gadgets/WeaponHEMPmine.h"
#include "Gadgets/WeaponHTNmine.h"
#include "Gadgets/WeaponMassPulseCannon.h"
#include "Gadgets/WeaponAntiShipMissile.h"
#include "Gadgets/WeaponAmplifiedCoilBeam.h"
#include "Gadgets/WeaponIonizedBeamCannon.h"
#include "Gadgets/WeaponTKCannon.h"
#include "Gadgets/WeaponPSIBeam.h"
#include "Gadgets/WeaponCL500.h"
#include "Gadgets/WeaponCL1000.h"
#include "Gadgets/WeaponCL2000.h"
#include "Gadgets/WeaponIPbeam.h"
#include "Gadgets/WeaponIPSbeam.h"
#include "Gadgets/WeaponCL300.h"

#include "DarkSpace/GameContext.h"

//----------------------------------------------------------------------------

// weapons
IMPLEMENT_LIGHT_FACTORY( WeaponRailgun, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponRailgunFighter, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponRailgunHV, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponSABOT, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponSABOT2, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponGauss, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponGaussHV, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponParticleCannon, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponParticleCannonFighter, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponParticleCannonHV, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponEMPCannon, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponEMPCannonHV, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPsiCannon, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPsiCannonFighter, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPsiCannonHV, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPlasmaCannon, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPlasmaCannonHV, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponMPA, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponFusionTorpedo, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponIEmissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponIEMissile2, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponTNmine, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponARmissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponARMissile2, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponMiRVbomb, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponEMPmine, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPCmissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponProtonTorpedo, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponBPCbomb, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPSGmissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPSGMissile2, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponAMmine, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponAMtorpedo, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponQSbomb, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponNeutronBomb, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPurifierBomb, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponQST, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponAntiShipMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponAresCannon, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponBBBomb, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponFuryBomb, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponIonCannon, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponASMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponGuardianMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPSMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponShredderMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponShredderMissile2, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPsiwaveSeekerMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponShockMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponTKBomb, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponEtorpedo, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponStellarIncinerator, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponWibbleCannon, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponWibbleCannonHV, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponDMC, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponIonTorpedo, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponElfTorpedo, GadgetWeapon);
IMPLEMENT_LIGHT_FACTORY( WeaponHeavyMassDriver, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponNeutroniumTorpedo, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponVarianceTorpedo, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponAMtorpedoFighter, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponProtonTorpedoFighter, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponValenceAutocannon, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponValenceAutocannonHV, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponSpreadShot, GadgetSpreadWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponTKTorpedo, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponEMPMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponEMPMissileHV, GadgetWeapon );

// Siege Torps
IMPLEMENT_LIGHT_FACTORY( WeaponQSST, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponNeutroniumSiegeTorpedo, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponVarianceSiegeTorpedo, GadgetWeapon );

// Tier 1 Missiles (Scouts/Frigate)
IMPLEMENT_LIGHT_FACTORY( WeaponAntiRadarMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPeregrineMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponSparrowMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPasseridaeMissileLauncher, GadgetSpreadWeapon);

// Tier 2 Missiles (Destroyers/Cruisers)
IMPLEMENT_LIGHT_FACTORY( WeaponRaptorMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponIonTrackerMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPhoenixMissile, GadgetWeapon );

// Tier 3 Missiles (Dreadnoughts/Stations)
IMPLEMENT_LIGHT_FACTORY( WeaponLinearDriveMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponProtonCruiseMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponHarpexMissile, GadgetWeapon );

// K'Luth Missiles (self scaling)
IMPLEMENT_LIGHT_FACTORY( WeaponPsionicMissile, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponShroudMissile, GadgetWeapon );

IMPLEMENT_LIGHT_FACTORY( WeaponTrebuchetSiegeMissile, GadgetWeapon );

// beam weapons
IMPLEMENT_LIGHT_FACTORY( WeaponCL, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponCLH, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponCLPlanet, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponDSbeam, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponDisruptor, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponDisruptorA, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponDeathBeam, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponFLUXbeam, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( MouthBeam, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponELF, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPSIBeam, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponFEP, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponFEPO, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponCLICC, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponCLHICC, GadgetBeamWeapon );

// point-defense beams
IMPLEMENT_LIGHT_FACTORY( WeaponPulseBeam, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponAmplifiedPulseBeam, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponRapidCycleDisruptor, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponPsiFocusedDisruptor, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponLonginusDefenseBeam, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponCLER, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponFighterBeam, GadgetBeamWeapon );

// area weapons
IMPLEMENT_LIGHT_FACTORY( WeaponFluxWave, GadgetAreaWeapon );

// legacy weapons
IMPLEMENT_LIGHT_FACTORY( WeaponAmplifiedCoilBeam, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponIonizedBeamCannon, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponTKCannon, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponCL1000, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponCL2000, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponIPbeam, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponIPSbeam, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponCL300, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponCL500, GadgetBeamWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponHAMmine, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponHEMPmine, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponHTNmine, GadgetWeapon );
IMPLEMENT_LIGHT_FACTORY( WeaponMassPulseCannon, GadgetWeapon );

//---------------------------------------------------------------------------------------------------

// DEPRECIATE FACTORY METHOD... this is here to just support loading old data using the old key generation method.
REGISTER_FACTORY_KEY( WeaponRailgun, 4099399420416827088LL );
REGISTER_FACTORY_KEY( WeaponSABOT, 4099401530004079802LL );
REGISTER_FACTORY_KEY( WeaponGauss, 4099400956198072530LL );
REGISTER_FACTORY_KEY( WeaponParticleCannon, 4045840264442462183LL );
REGISTER_FACTORY_KEY( WeaponEMPCannon, 4368334562977486480LL );
REGISTER_FACTORY_KEY( WeaponPsiCannon, 4099397981448677052LL );
REGISTER_FACTORY_KEY( WeaponMPA, 4099398937773268912LL );
REGISTER_FACTORY_KEY( WeaponFusionTorpedo, 4042551160603183550LL );
REGISTER_FACTORY_KEY( WeaponIEmissile, 4099403920919425570LL );
REGISTER_FACTORY_KEY( WeaponTNmine, 4099402733246481883LL );
REGISTER_FACTORY_KEY( WeaponARmissile, 4099403442956352290LL );
REGISTER_FACTORY_KEY( WeaponMiRVbomb, 4099410246903071496LL );
REGISTER_FACTORY_KEY( WeaponEMPmine, 4099409136567132462LL );
REGISTER_FACTORY_KEY( WeaponPCmissile, 4099407265333522532LL );
REGISTER_FACTORY_KEY( WeaponProtonTorpedo, 4114513447988027654LL );
REGISTER_FACTORY_KEY( WeaponBPCbomb, 4099406619945537306LL );
REGISTER_FACTORY_KEY( WeaponPSGmissile, 4099406255724965188LL );
REGISTER_FACTORY_KEY( WeaponAMmine, 4099414716842935840LL );
REGISTER_FACTORY_KEY( WeaponAMtorpedo, 4099411772369944389LL );
REGISTER_FACTORY_KEY( WeaponQSbomb, 4311209624888631297LL );
REGISTER_FACTORY_KEY( WeaponNeutronBomb, 4741184969614620176LL );
REGISTER_FACTORY_KEY( WeaponPhoenixMissile, 4741185064992012554LL );
REGISTER_FACTORY_KEY( WeaponPurifierBomb, 4741185822134339167LL );
REGISTER_FACTORY_KEY( WeaponQST, 4741185427081512208LL );
REGISTER_FACTORY_KEY( WeaponAntiShipMissile, 4741189370380132557LL );
REGISTER_FACTORY_KEY( WeaponAresCannon, 4741189426613637508LL );
REGISTER_FACTORY_KEY( WeaponBBBomb, 4741188992119569465LL );
REGISTER_FACTORY_KEY( WeaponFuryBomb, 4741195866304579292LL );
REGISTER_FACTORY_KEY( WeaponMassPulseCannon, 4741198284249083508LL );
REGISTER_FACTORY_KEY( WeaponIonCannon, 4745102530872666213LL );
REGISTER_FACTORY_KEY( WeaponASMissile, 4745099888689418843LL );
REGISTER_FACTORY_KEY( WeaponGuardianMissile, 4745099693559031500LL );
REGISTER_FACTORY_KEY( WeaponPSMissile, 4745099730896468899LL );
REGISTER_FACTORY_KEY( WeaponShredderMissile, 4745100092957560045LL );
REGISTER_FACTORY_KEY( WeaponPsiwaveSeekerMissile, 4774070694222275540LL );
REGISTER_FACTORY_KEY( WeaponShockMissile, 4745100101405946768LL );
REGISTER_FACTORY_KEY( WeaponTKBomb, 4745098793041179783LL );
REGISTER_FACTORY_KEY( WeaponHAMmine, 4745095885040941113LL );
REGISTER_FACTORY_KEY( WeaponHEMPmine, 4745096256917942714LL );
REGISTER_FACTORY_KEY( WeaponHTNmine, 4745096326716405287LL );
REGISTER_FACTORY_KEY( WeaponEtorpedo, 4314513417988047654LL );

// beam weapons
REGISTER_FACTORY_KEY( WeaponIPbeam, 4045169659392845258LL );
REGISTER_FACTORY_KEY( WeaponIPSbeam, 4099419296416481518LL );
REGISTER_FACTORY_KEY( WeaponCL300, 4099427908330392140LL );
REGISTER_FACTORY_KEY( WeaponCL500, 4099427924273123938LL );
REGISTER_FACTORY_KEY( WeaponCL, 4741695514411283455LL );
REGISTER_FACTORY_KEY( WeaponCLH, 4741695439256909203LL );
REGISTER_FACTORY_KEY( WeaponCL1000, 4293675832064222509LL );
REGISTER_FACTORY_KEY( WeaponCL2000, 4293675714608755623LL );
REGISTER_FACTORY_KEY( WeaponDSbeam, 4099428882975882134LL );
REGISTER_FACTORY_KEY( WeaponDisruptor, 4741715658864134285LL );
REGISTER_FACTORY_KEY( WeaponDisruptorA, 4741715820079104667LL );
REGISTER_FACTORY_KEY( WeaponDeathBeam, 4330777244047070123LL );
REGISTER_FACTORY_KEY( WeaponFLUXbeam, 4099430246369708123LL );
REGISTER_FACTORY_KEY( MouthBeam, 4342293685871948888LL );
REGISTER_FACTORY_KEY( WeaponELF, 4368333855832657842LL );
REGISTER_FACTORY_KEY( WeaponPulseBeam, 4373450812475706924LL );
REGISTER_FACTORY_KEY( WeaponAmplifiedCoilBeam, 4741190174816895799LL );
REGISTER_FACTORY_KEY( WeaponIonizedBeamCannon, 4741195379153378762LL );
REGISTER_FACTORY_KEY( WeaponStellarIncinerator, 4742270873106470774LL );
REGISTER_FACTORY_KEY( WeaponTKCannon, 4745088699564917180LL );
REGISTER_FACTORY_KEY( WeaponPSIBeam, 4745103542970732060LL );

// area weapons
REGISTER_FACTORY_KEY( WeaponFluxWave, 4373537908980069595LL );

//---------------------------------------------------------------------------------------------------
//EOF
