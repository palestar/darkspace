/*
	LayoutMessages.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "Interface/LayoutMessages.h"
#include "Interface/WindowMessage.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutMessages, WindowLayout );
REGISTER_FACTORY_KEY( LayoutMessages, 4240981718836307720 );

LayoutMessages::LayoutMessages() : m_LastChatCount( 0 )
{}

//----------------------------------------------------------------------------

const int MAX_MESSAGES = 16;

void LayoutMessages::onActivate()
{
	WindowLayout::onActivate();

	GameClient * pClient = ((GameDocument *)document())->client();
	ASSERT( pClient );

	// any new messages since the last time we were active
	if ( pClient->chatCount() != m_LastChatCount )
	{
		if ( pClient->chatCount() < m_LastChatCount )
		{
			detachAllNodes();
			m_LastChatCount = 0;
		}

		// determine how many new and current messages
		int messages = (pClient->chatCount() - m_LastChatCount) + childCount();
		// remove current messages if we are above the maximum
		while( messages > MAX_MESSAGES && childCount() > 0 )
			detachNode( 0 );		// detach the first and oldest child

		// redo the layout of the messages
		doLayout();
	}
}

void LayoutMessages::onDeactivate()
{
	WindowLayout::onDeactivate();
}

void LayoutMessages::onUpdate( float t )
{
	WindowLayout::onUpdate( t );

	GameClient * pClient = ((GameDocument *)document())->client();
	ASSERT( pClient );

	// update ship messages
	if ( pClient->chatCount() != m_LastChatCount )
	{
		if ( pClient->chatCount() < m_LastChatCount )
		{
			detachAllNodes();
			m_LastChatCount = 0;
		}

		// keep a limit on the number of messages to display
		m_LastChatCount = Max( pClient->chatCount() - MAX_MESSAGES, m_LastChatCount );
		
		for(;m_LastChatCount<pClient->chatCount();m_LastChatCount++)
			new WindowMessage( this, pClient->chat( m_LastChatCount ) );

		doLayout();
	}

	// keep the incoming messages within the clipping area
	if ( childCount() > 0 && contentSize().height > window().height() )
	{
		for(int i=0;i<childCount();i++)
			if ( WidgetCast<WindowMessage>( child(i) ) )
			{
				((WindowMessage *)child(i))->setTime( Min( ((WindowMessage *)child(i))->time(), 1.0f ) );
				break;
			}
	}
}

//----------------------------------------------------------------------------
//EOF
