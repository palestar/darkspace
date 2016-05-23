/**
	@file TierInfo.h
	
	(c)2013 Palestar Inc
	@author Jack Wallace @date 7/21/2013
*/

#ifndef TIERINFO_H
#define TIERINFO_H

#include "World/NounContext.h"
#include "GameDll.h"

//---------------------------------------------------------------------------------------------------

class DLL TierInfo : public NounContext::Data
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();
	
	typedef Reference< TierInfo >		Ref;

	TierInfo() : m_nTier( 0 )
	{}

	int					m_nTier;
};


#endif

//---------------------------------------------------------------------------------------------------
//EOF
