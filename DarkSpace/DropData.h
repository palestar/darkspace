/**
	@file DropData.h
	@brief Loot information for the Noun.
	
	(c)2009 Palestar Inc
	@author Richard Lyle @date 6/24/2011 11:20:02 PM
*/

#ifndef DROPDATA_H
#define DROPDATA_H

#include "World/NounContext.h"
#include "GameDll.h"

//---------------------------------------------------------------------------------------------------

class DLL DropData : public NounContext::Data
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	typedef Reference< DropData >		Ref;

	DropData() : m_nDropClassKey( (qword)0 ), m_nBaseAmount( 0 ), m_nRandomAmount( 0 ), m_nChance( 0 )
	{}

	ClassKey			m_nDropClassKey;
	int					m_nBaseAmount;
	int					m_nRandomAmount;
	int					m_nChance;
};


#endif

//---------------------------------------------------------------------------------------------------
//EOF
