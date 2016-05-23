/*
	ShipEnhancements.cpp
	(c)2008 Palestar Inc, Jeff Everett
*/

#include "ShipEnhancements.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( MinorVelocity, NounEnhancement );
IMPLEMENT_FACTORY( MinorThrust, NounEnhancement );
IMPLEMENT_FACTORY( MinorTurn, NounEnhancement );
IMPLEMENT_FACTORY( MinorArmor, NounEnhancement );
IMPLEMENT_FACTORY( MinorShield, NounEnhancement );
IMPLEMENT_FACTORY( MinorEwarRange, NounEnhancement );
IMPLEMENT_FACTORY( MinorEwarStrength, NounEnhancement );
IMPLEMENT_FACTORY( MinorEwarEnergy, NounEnhancement );

IMPLEMENT_FACTORY( LimitedVelocity, NounEnhancement );
IMPLEMENT_FACTORY( LimitedThrust, NounEnhancement );
IMPLEMENT_FACTORY( LimitedTurn, NounEnhancement );
IMPLEMENT_FACTORY( LimitedArmor, NounEnhancement );
IMPLEMENT_FACTORY( LimitedShield, NounEnhancement );
IMPLEMENT_FACTORY( LimitedEwarRange, NounEnhancement );
IMPLEMENT_FACTORY( LimitedEwarStrength, NounEnhancement );
IMPLEMENT_FACTORY( LimitedEwarEnergy, NounEnhancement );

IMPLEMENT_FACTORY( StandardVelocity, NounEnhancement );
IMPLEMENT_FACTORY( StandardThrust, NounEnhancement );
IMPLEMENT_FACTORY( StandardTurn, NounEnhancement );
IMPLEMENT_FACTORY( StandardArmor, NounEnhancement );
IMPLEMENT_FACTORY( StandardShield, NounEnhancement );
IMPLEMENT_FACTORY( StandardEwarRange, NounEnhancement );
IMPLEMENT_FACTORY( StandardEwarStrength, NounEnhancement );
IMPLEMENT_FACTORY( StandardEwarEnergy, NounEnhancement );

IMPLEMENT_FACTORY( ImprovedVelocity, NounEnhancement );
IMPLEMENT_FACTORY( ImprovedThrust, NounEnhancement );
IMPLEMENT_FACTORY( ImprovedTurn, NounEnhancement );
IMPLEMENT_FACTORY( ImprovedArmor, NounEnhancement );
IMPLEMENT_FACTORY( ImprovedShield, NounEnhancement );
IMPLEMENT_FACTORY( ImprovedEwarRange, NounEnhancement );
IMPLEMENT_FACTORY( ImprovedEwarStrength, NounEnhancement );
IMPLEMENT_FACTORY( ImprovedEwarEnergy, NounEnhancement );

IMPLEMENT_FACTORY( EnhancedVelocity, NounEnhancement );
IMPLEMENT_FACTORY( EnhancedThrust, NounEnhancement );
IMPLEMENT_FACTORY( EnhancedTurn, NounEnhancement );
IMPLEMENT_FACTORY( EnhancedArmor, NounEnhancement );
IMPLEMENT_FACTORY( EnhancedShield, NounEnhancement );
IMPLEMENT_FACTORY( EnhancedEwarRange, NounEnhancement );
IMPLEMENT_FACTORY( EnhancedEwarStrength, NounEnhancement );
IMPLEMENT_FACTORY( EnhancedEwarEnergy, NounEnhancement );

IMPLEMENT_FACTORY( AdvancedVelocity, NounEnhancement );
IMPLEMENT_FACTORY( AdvancedThrust, NounEnhancement );
IMPLEMENT_FACTORY( AdvancedTurn, NounEnhancement );
IMPLEMENT_FACTORY( AdvancedArmor, NounEnhancement );
IMPLEMENT_FACTORY( AdvancedShield, NounEnhancement );
IMPLEMENT_FACTORY( AdvancedEwarRange, NounEnhancement );
IMPLEMENT_FACTORY( AdvancedEwarStrength, NounEnhancement );
IMPLEMENT_FACTORY( AdvancedEwarEnergy, NounEnhancement );

IMPLEMENT_FACTORY( MakkarShipBooster, NounEnhancement );
IMPLEMENT_FACTORY( ShipProtector, NounEnhancement );
IMPLEMENT_FACTORY( ShipProtector2, NounEnhancement );
IMPLEMENT_FACTORY( ShipProtector3, NounEnhancement );
IMPLEMENT_FACTORY( ShipProtector4, NounEnhancement );
IMPLEMENT_FACTORY( ShipProtector5, NounEnhancement );
IMPLEMENT_FACTORY( ArmorOverhaul, NounEnhancement );
IMPLEMENT_FACTORY( StealthModule, NounEnhancement );


//---------------------------------------------------------------------------------------------------

// Support for old class names which didn't follow the naming standard, do NOT add any new ehancements 
// in here.
REGISTER_FACTORY_ALIAS( MinorVelocity, NounEnhancement_MinorVelocity );
REGISTER_FACTORY_ALIAS( MinorThrust, NounEnhancement_MinorThrust );
REGISTER_FACTORY_ALIAS( MinorTurn, NounEnhancement_MinorTurn );
REGISTER_FACTORY_ALIAS( MinorArmor, NounEnhancement_MinorArmor );
REGISTER_FACTORY_ALIAS( MinorShield, NounEnhancement_MinorShield );

REGISTER_FACTORY_ALIAS( LimitedVelocity, NounEnhancement_LimitedVelocity );
REGISTER_FACTORY_ALIAS( LimitedThrust, NounEnhancement_LimitedThrust );
REGISTER_FACTORY_ALIAS( LimitedTurn, NounEnhancement_LimitedTurn );
REGISTER_FACTORY_ALIAS( LimitedArmor, NounEnhancement_LimitedArmor );
REGISTER_FACTORY_ALIAS( LimitedShield, NounEnhancement_LimitedShield );

REGISTER_FACTORY_ALIAS( StandardVelocity, NounEnhancement_StandardVelocity );
REGISTER_FACTORY_ALIAS( StandardThrust, NounEnhancement_StandardThrust );
REGISTER_FACTORY_ALIAS( StandardTurn, NounEnhancement_StandardTurn );
REGISTER_FACTORY_ALIAS( StandardArmor, NounEnhancement_StandardArmor );
REGISTER_FACTORY_ALIAS( StandardShield, NounEnhancement_StandardShield );

REGISTER_FACTORY_ALIAS( ImprovedVelocity, NounEnhancement_ImprovedVelocity );
REGISTER_FACTORY_ALIAS( ImprovedThrust, NounEnhancement_ImprovedThrust );
REGISTER_FACTORY_ALIAS( ImprovedTurn, NounEnhancement_ImprovedTurn );
REGISTER_FACTORY_ALIAS( ImprovedArmor, NounEnhancement_ImprovedArmor );
REGISTER_FACTORY_ALIAS( ImprovedShield, NounEnhancement_ImprovedShield );

REGISTER_FACTORY_ALIAS( EnhancedVelocity, NounEnhancement_EnhancedVelocity );
REGISTER_FACTORY_ALIAS( EnhancedThrust, NounEnhancement_EnhancedThrust );
REGISTER_FACTORY_ALIAS( EnhancedTurn, NounEnhancement_EnhancedTurn );
REGISTER_FACTORY_ALIAS( EnhancedArmor, NounEnhancement_EnhancedArmor );
REGISTER_FACTORY_ALIAS( EnhancedShield, NounEnhancement_EnhancedShield );

REGISTER_FACTORY_ALIAS( AdvancedVelocity, NounEnhancement_AdvancedVelocity );
REGISTER_FACTORY_ALIAS( AdvancedThrust, NounEnhancement_AdvancedThrust );
REGISTER_FACTORY_ALIAS( AdvancedTurn, NounEnhancement_AdvancedTurn );
REGISTER_FACTORY_ALIAS( AdvancedArmor, NounEnhancement_AdvancedArmor );
REGISTER_FACTORY_ALIAS( AdvancedShield, NounEnhancement_AdvancedShield );

REGISTER_FACTORY_ALIAS( MakkarShipBooster, NounEnhancement_MakkarShipBooster );
REGISTER_FACTORY_ALIAS( ShipProtector, NounEnhancement_ShipProtector );
REGISTER_FACTORY_ALIAS( ShipProtector2, NounEnhancement_ShipProtectorTwo );
REGISTER_FACTORY_ALIAS( ShipProtector3, NounEnhancement_ShipProtectorThree );
REGISTER_FACTORY_ALIAS( ShipProtector4, NounEnhancement_ShipProtectorFour );
REGISTER_FACTORY_ALIAS( ShipProtector5, NounEnhancement_ShipProtectorFive );
REGISTER_FACTORY_ALIAS( ArmorOverhaul, NounEnhancement_ArmorOverhaul );

//---------------------------------------------------------------------------------------------------
// EOF
