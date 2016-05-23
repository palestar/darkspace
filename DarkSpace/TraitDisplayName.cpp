/**
	@file TraitDisplayName.cpp

	(c)2012 Palestar Inc
	@author Richard Lyle @date 11/9/2012 4:17:44 PM
*/

#include "TraitDisplayName.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( TraitDisplayName, Trait );

BEGIN_PROPERTY_LIST( TraitDisplayName, Trait )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_sDisplayName );
END_PROPERTY_LIST();


//---------------------------------------------------------------------------------------------------
//EOF
