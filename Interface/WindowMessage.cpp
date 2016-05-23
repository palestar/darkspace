/*
	WindowMessage.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Standard/Limits.h"
#include "Interface/WindowMessage.h"

//----------------------------------------------------------------------------

WindowMessage::WindowMessage( NodeWindow * pParent, const char * pMessage ) : WindowText()
{
	ASSERT( pParent );

	setText( pMessage );
	setContext( pParent->context() );

	float baseTime = 0.0f;
	for(int i=0;i<pParent->childCount();i++)
		if ( WidgetCast<WindowMessage>( pParent->child(i) ) )
			baseTime = Max( baseTime, ((WindowMessage *)pParent->child(i))->m_Time );
	
	float messageTime = Min( (m_Text.length() * 0.1f) + 1.0f, 15.0f );
	
	m_Window = RectInt( PointInt(0,0), SizeInt( 0,0 ) );
	m_Flags = VISIBLE | PARENT_WIDTH | NOCLIP;
	m_Time = Min( baseTime + messageTime, 30.0f );	
	m_Alpha = 1.0f;

	pParent->attachNode( this );
}

void WindowMessage::onUpdate( float t )
{
	m_Time -= t;

	if ( m_Time < 0.0f )
		destroy();
	else if ( m_Time < 1.0f )
		m_Alpha = m_Time;
}

//----------------------------------------------------------------------------
//EOF
