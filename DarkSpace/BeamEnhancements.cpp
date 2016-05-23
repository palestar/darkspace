/**
	@file BeamEnhancements.cpp
	
	(c)2009 Palestar Inc
	@author Richard Lyle @date 5/6/2010 9:45:54 PM
*/

#include "BeamEnhancements.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( MinorBeamAccelerator, NounEnhancement );
IMPLEMENT_FACTORY( MinorBeamCooler, NounEnhancement );
IMPLEMENT_FACTORY( MinorBeamMultiplexer, NounEnhancement );
IMPLEMENT_FACTORY( MinorBeamCondenser, NounEnhancement );

IMPLEMENT_FACTORY( LimitedBeamAccelerator, NounEnhancement );
IMPLEMENT_FACTORY( LimitedBeamCooler, NounEnhancement );
IMPLEMENT_FACTORY( LimitedBeamMultiplexer, NounEnhancement );
IMPLEMENT_FACTORY( LimitedBeamCondenser, NounEnhancement );

IMPLEMENT_FACTORY( StandardBeamAccelerator, NounEnhancement );
IMPLEMENT_FACTORY( StandardBeamCooler, NounEnhancement );
IMPLEMENT_FACTORY( StandardBeamMultiplexer, NounEnhancement );
IMPLEMENT_FACTORY( StandardBeamCondenser, NounEnhancement );

IMPLEMENT_FACTORY( ImprovedBeamAccelerator, NounEnhancement );
IMPLEMENT_FACTORY( ImprovedBeamCooler, NounEnhancement );
IMPLEMENT_FACTORY( ImprovedBeamMultiplexer, NounEnhancement );
IMPLEMENT_FACTORY( ImprovedBeamCondenser, NounEnhancement );

IMPLEMENT_FACTORY( EnhancedBeamAccelerator, NounEnhancement );
IMPLEMENT_FACTORY( EnhancedBeamCooler, NounEnhancement );
IMPLEMENT_FACTORY( EnhancedBeamMultiplexer, NounEnhancement );
IMPLEMENT_FACTORY( EnhancedBeamCondenser, NounEnhancement );

IMPLEMENT_FACTORY( AdvancedBeamAccelerator, NounEnhancement );
IMPLEMENT_FACTORY( AdvancedBeamCooler, NounEnhancement );
IMPLEMENT_FACTORY( AdvancedBeamMultiplexer, NounEnhancement );
IMPLEMENT_FACTORY( AdvancedBeamCondenser, NounEnhancement );

//---------------------------------------------------------------------------------------------------

// Support for old class names which didn't follow the naming standard.
REGISTER_FACTORY_ALIAS( MinorBeamAccelerator, NounEnhancement_MinorBeamAccelerator );
REGISTER_FACTORY_ALIAS( MinorBeamCooler, NounEnhancement_MinorBeamCooler );
REGISTER_FACTORY_ALIAS( MinorBeamMultiplexer, NounEnhancement_MinorBeamMultiplexer );
REGISTER_FACTORY_ALIAS( MinorBeamCondenser, NounEnhancement_MinorBeamCondenser );

REGISTER_FACTORY_ALIAS( LimitedBeamAccelerator, NounEnhancement_LimitedBeamAccelerator );
REGISTER_FACTORY_ALIAS( LimitedBeamCooler, NounEnhancement_LimitedBeamCooler );
REGISTER_FACTORY_ALIAS( LimitedBeamMultiplexer, NounEnhancement_LimitedBeamMultiplexer );
REGISTER_FACTORY_ALIAS( LimitedBeamCondenser, NounEnhancement_LimitedBeamCondenser );

REGISTER_FACTORY_ALIAS( StandardBeamAccelerator, NounEnhancement_StandardBeamAccelerator );
REGISTER_FACTORY_ALIAS( StandardBeamCooler, NounEnhancement_StandardBeamCooler );
REGISTER_FACTORY_ALIAS( StandardBeamMultiplexer, NounEnhancement_StandardBeamMultiplexer );
REGISTER_FACTORY_ALIAS( StandardBeamCondenser, NounEnhancement_StandardBeamCondenser );

REGISTER_FACTORY_ALIAS( ImprovedBeamAccelerator, NounEnhancement_ImprovedBeamAccelerator );
REGISTER_FACTORY_ALIAS( ImprovedBeamCooler, NounEnhancement_ImprovedBeamCooler );
REGISTER_FACTORY_ALIAS( ImprovedBeamMultiplexer, NounEnhancement_ImprovedBeamMultiplexer );
REGISTER_FACTORY_ALIAS( ImprovedBeamCondenser, NounEnhancement_ImprovedBeamCondenser );

REGISTER_FACTORY_ALIAS( EnhancedBeamAccelerator, NounEnhancement_EnhancedBeamAccelerator );
REGISTER_FACTORY_ALIAS( EnhancedBeamCooler, NounEnhancement_EnhancedBeamCooler );
REGISTER_FACTORY_ALIAS( EnhancedBeamMultiplexer, NounEnhancement_EnhancedBeamMultiplexer );
REGISTER_FACTORY_ALIAS( EnhancedBeamCondenser, NounEnhancement_EnhancedBeamCondenser );

REGISTER_FACTORY_ALIAS( AdvancedBeamAccelerator, NounEnhancement_AdvancedBeamAccelerator );
REGISTER_FACTORY_ALIAS( AdvancedBeamCooler, NounEnhancement_AdvancedBeamCooler );
REGISTER_FACTORY_ALIAS( AdvancedBeamMultiplexer, NounEnhancement_AdvancedBeamMultiplexer );
REGISTER_FACTORY_ALIAS( AdvancedBeamCondenser, NounEnhancement_AdvancedBeamCondenser );

//---------------------------------------------------------------------------------------------------
//EOF
