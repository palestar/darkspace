/*
	Port.h

	Port object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef NOUNCONTEXTPORT_H
#define NOUNCONTEXTPORT_H

#include "Standard/Dll.h"
#include "Standard/Array.h"
#include "World/NounContext.h"
#include "World/Noun.h"
#include "Tools/ResourcerDoc/Port.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef NOUNCONTEXTPORT_DLL
#define DLL		DLL_EXPORT
#else
#define DLL		DLL_IMPORT
#endif

//-------------------------------------------------------------------------------

class DLL NounContextPort : public Port
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	typedef Reference<NounContextPort>	Ref;

	// Construction
	NounContextPort();

	// Widget Interface
	bool						read( const InStream & input );
	// Port Interface
	bool						autoLoad();									// enable autoload for this resource
	void						dependencies( DependentArray & dep );		// get the depdendencies
	void						translate( StringArray & strings );
	Resource::Ref 				createResource();							// create the resource 

	bool						importFile( const char * fileName );		// import from file
	void						shellOpen() const;							// open the source object through a shell program

	NounContext *				initialize( NounContext * pContext );

	// Types
	typedef NounContext::DataArray	DataArray;
	typedef NounContext::Drop		Drop;

	// Data
	CharString					m_Name;
	CharString					m_Description;
	ClassKey					m_Class;
	ClassKey					m_Type;
	CharString					m_Scene;
	float						m_Mass;
	bool						m_bHullOverride;
	BoxHull						m_Hull;
	dword						m_Flags;
	DataArray					m_Data;

	// DEPRECATED
	struct KeyPort : public PropertyClass
	{
		DECLARE_PROPERTY_LIST();

		CharString				m_sKey;
		CharString				m_sPort;

		KeyPort & operator=( const KeyPort & copy )
		{
			m_sKey = copy.m_sKey;
			m_sPort = copy.m_sPort;
			return *this;
		}
	};

	Array< KeyPort >			m_Resources;
	Array< Drop::Ref >			m_Drops;
};

//---------------------------------------------------------------------------------------------------

inline const InStream & operator>>( const InStream & input, NounContextPort::KeyPort & res )
{
	input >> res.m_sKey;
	input >> res.m_sPort;
	return input;
}

inline const OutStream & operator<<( const OutStream & output, const NounContextPort::KeyPort & res )
{
	output << res.m_sKey;
	output << res.m_sPort;
	return output;
}

//---------------------------------------------------------------------------------------------------

#endif

//-------------------------------------------------------------------------------
// EOF
