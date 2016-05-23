/**
	@file AdminEnhancements.cpp

	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/6/2010 8:20:53 PM
*/

#include "AdminEnhancements.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( MavericksUltimateBuild, NounEnhancement );
IMPLEMENT_FACTORY( MavericksDefense, NounEnhancement );
IMPLEMENT_FACTORY( MavericksWeaponDamage, NounEnhancement );
IMPLEMENT_FACTORY( PantheonJumpCore, NounEnhancement );

REGISTER_FACTORY_ALIAS( MavericksUltimateBuild, NounEnhancement_MavericksUltimateBuild );
REGISTER_FACTORY_ALIAS( MavericksDefense, NounEnhancement_MavericksDefense );
REGISTER_FACTORY_ALIAS( MavericksWeaponDamage, NounEnhancement_MaverickWeaponDamage );
REGISTER_FACTORY_ALIAS( PantheonJumpCore, NounEnhancement_PantheonJumpCore );

//---------------------------------------------------------------------------------------------------
//EOF
