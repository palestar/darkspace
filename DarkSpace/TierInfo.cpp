/**
	@file TierInfo.cpp

	(c)2013 Palestar Inc
	@author Jack Wallace @date 7/21/2013
*/

#include "TierInfo.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( TierInfo, NounContext::Data );

BEGIN_PROPERTY_LIST( TierInfo, NounContext::Data );
	ADD_PROPERTY( m_nTier );
END_PROPERTY_LIST();

//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
//EOF
