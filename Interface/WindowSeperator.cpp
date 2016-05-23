/*
	WindowSeperator.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Standard/Constant.h"
#include "Interface/WindowSeperator.h"

//----------------------------------------------------------------------------

Constant DEFAULT_SEPERATOR_HEIGHT( "DEFAULT_SEPERATOR_HEIGHT", 1.0f );

//----------------------------------------------------------------------------

WindowSeperator::WindowSeperator()
{
	m_Flags |= NOCLIP | PARENT_WIDTH;		// turn off clipping, make this window width the same as our parent
	m_Style |= USER1;						// don't allow the base class to resize this window
	m_Alpha = 0;

	setSeperator( DEFAULT_SEPERATOR_HEIGHT );
}

//----------------------------------------------------------------------------

void WindowSeperator::setSeperator( int size, const char * pText /*= NULL*/ )
{
	// horizontal seperator
	setWindow( RectInt( PointInt(0,0), SizeInt( 1, size )) );
	// set the text
	setText( pText != NULL ? pText : "" );
}

//----------------------------------------------------------------------------
//EOF
