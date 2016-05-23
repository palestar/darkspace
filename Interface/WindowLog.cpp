/*
	WindowLog.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "Interface/WindowLog.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowLog, WindowText );
REGISTER_FACTORY_KEY( WindowLog, 4240986808537713705 );

WindowLog::WindowLog() : m_LastChatCount( 0 )
{}

const int CHAT_BUFFER_SIZE = 4096;
const int MAX_MESSAGES = 256;

void WindowLog::onUpdate( float t )
{
	WindowText::onUpdate( t );

	GameClient * pClient = ((GameDocument *)document())->client();
	ASSERT( pClient );

	if ( pClient->chatCount() != m_LastChatCount )
	{
		if ( pClient->chatCount() < m_LastChatCount )
			m_LastChatCount = 0;
		m_LastChatCount = Max( pClient->chatCount() - MAX_MESSAGES, m_LastChatCount );

		// append messages to chat log
		CharString log = text();
		for(;m_LastChatCount<pClient->chatCount();m_LastChatCount++)
		{
			log += pClient->chat( m_LastChatCount );
			log += "\n";
		}

		// keep the log under a managable length
		int length = log.length();
		while( length > CHAT_BUFFER_SIZE )
		{
			log.right( log.length() - (log.find( '\n' ) + 1) );
			length = log.length();
		}
		setText( log );
	}

	// scroll the parent frame to the last line of text
	NodeWindow * pParent = WidgetCast<NodeWindow>( parent() );
	if ( pParent != NULL && pParent->visible() )
	{
		// display the last line of text
		if ( (pParent->flags() & NodeWindow::SCROLLING) == 0 )
		{
			SizeInt content = pParent->contentSize();
			SizeInt visible = pParent->window().size();
			if ( content.height > visible.height )
				pParent->setOffset( PointInt( 0, content.height - visible.height) );
		}
	}
}

//----------------------------------------------------------------------------
//EOF
