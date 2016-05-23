/**
	@file FactionInfo.cpp

	(c)2013 Palestar Inc
	@author Jack Wallace @date 7/21/2013
*/

#include "FactionInfo.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( FactionInfo, NounContext::Data );

BEGIN_PROPERTY_LIST( FactionInfo, NounContext::Data );
	ADD_PROPERTY( m_nFactionInfo );
END_PROPERTY_LIST();

//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
//EOF
