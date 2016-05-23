/*
	GadgetScript.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "Resource/Collection.h"
#include "GadgetScript.h"
#include "GameContext.h"
#include "resource.h"


IMPLEMENT_FACTORY( GadgetScript, NounGadget );
REGISTER_FACTORY_KEY( GadgetScript, 4392433842104907699LL );

BEGIN_PROPERTY_LIST( GadgetScript, NounGadget )
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Mode );
END_PROPERTY_LIST();

GadgetScript::GadgetScript() : m_Mode( 0 )
{}

//----------------------------------------------------------------------------

void GadgetScript::initialize()
{
	NounGadget::initialize();
	
	// load scripts from the noun context
	loadScripts();
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetScript::type() const
{
	return UNKNOWN;
}

dword GadgetScript::hotkey() const
{
	return 0; 
}

CharString GadgetScript::status() const
{
	return CharString().format("%u", m_Mode);
}

CharString GadgetScript::description() const
{
	Text * pText = getScript();
	if ( pText != NULL )
		return pText->text();

	return "?";
}

int GadgetScript::useModeCount() const
{
	return m_Scripts.size();
}

int GadgetScript::useMode() const
{
	return m_Mode;
}

bool GadgetScript::useTarget() const
{
	return true;
}

bool GadgetScript::usable( Noun * pTarget, bool shift ) const
{
	if ( getScript() != NULL )
		return true;
	return false;
}

void GadgetScript::useMode( int mode )
{
	m_Mode = Clamp( mode, 0, m_Scripts.size() );
}

void GadgetScript::use( dword when, Noun * pTarget, bool shift )
{
	Text * pText = getScript();
	if ( pText != NULL )
	{
		message( "Running script..." );
		context()->runScript( UniqueNumber(), "GadgetScript", CharString( pText->text() ) );
	}
}

//----------------------------------------------------------------------------

void GadgetScript::loadScripts()
{
	// release any previous scripts
	m_Scripts.release();

	// grab the collection pointer from our noun context
	Collection::Link collection = WidgetCast<Collection>( nounContext()->resource( "SCRIPTS" ) );
	if ( collection.valid() )
	{
		for(int i=0;i<collection->resourceCount();i++)
		{
			Resource * pResource = collection->resource(i);
			if ( WidgetCast<Text>( pResource ) )
				m_Scripts.push( (Text *)pResource );
		}
	}
}

//----------------------------------------------------------------------------

Text * GadgetScript::getScript() const
{
	if ( m_Mode < m_Scripts.size() )
		return m_Scripts[ m_Mode ];
	return NULL;
}

//----------------------------------------------------------------------------
// EOF
