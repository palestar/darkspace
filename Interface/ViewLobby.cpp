/*
	ViewLobby.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "ViewLobby.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewLobby, WindowView::View );
REGISTER_FACTORY_KEY( ViewLobby, 4701347204354899329 );

ViewLobby::ViewLobby()
{
	// Construct your view class
}

//----------------------------------------------------------------------------

void ViewLobby::onActivate()
{
	//{{BEGIN_DATA_INIT
	//END_DATA_INIT}}

	// called when this view is created
}

void ViewLobby::onDeactivate()
{
	// called before this view is destroyed
}

void ViewLobby::onUpdate( float t )
{
	// implement view updating
}

bool ViewLobby::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	//END_MSG_MAP}}

	return false;
}

void ViewLobby::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

//----------------------------------------------------------------------------


