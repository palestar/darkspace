/**
	@file Drop.cpp

	(c)2009 Palestar Inc
	@author Richard Lyle @date 6/24/2011 11:10:39 PM
*/

#include "DropData.h"

//---------------------------------------------------------------------------------------------------

IMPLEMENT_FACTORY( DropData, NounContext::Data );

BEGIN_PROPERTY_LIST( DropData, NounContext::Data );
	ADD_CLASSKEY_PROPERTY( m_nDropClassKey, NounEnhancement );
	ADD_PROPERTY( m_nBaseAmount );
	ADD_PROPERTY( m_nRandomAmount );
	ADD_PROPERTY( m_nChance );
END_PROPERTY_LIST();

//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
//EOF
