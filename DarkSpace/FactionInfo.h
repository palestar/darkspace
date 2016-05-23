/**
	@file FactionInfo.h
	
	(c)2013 Palestar Inc
	@author Jack Wallace @date 7/21/2013
*/

#ifndef FACTIONINFO_H
#define FACTIONINFO_H

#include "World/NounContext.h"
#include "GameDll.h"

//---------------------------------------------------------------------------------------------------

class DLL FactionInfo : public NounContext::Data
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();
	
	typedef Reference< FactionInfo >		Ref;

	FactionInfo() : m_nFactionInfo( -1 )
	{}

	int					m_nFactionInfo;
};


#endif

//---------------------------------------------------------------------------------------------------
//EOF
