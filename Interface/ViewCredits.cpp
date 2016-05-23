/*
	ViewCredits.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Interface/ViewCredits.h"
#include "Interface/GameDocument.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewCredits, WindowView::View );
REGISTER_FACTORY_KEY( ViewCredits, 4127865904577885681 );

ViewCredits::ViewCredits()
{
	// Construct your view class
}

//----------------------------------------------------------------------------

void ViewCredits::onActivate()
{
	//{{BEGIN_DATA_INIT
	//END_DATA_INIT}}

	// called when this view is created
}

void ViewCredits::onDeactivate()
{
	// called before this view is destroyed
}

void ViewCredits::onUpdate( float t )
{
	// implement view updating
}

bool ViewCredits::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 1507228051, onButtonOkay);
	//END_MSG_MAP}}

	return false;
}

void ViewCredits::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

void ViewCredits::onRender( RenderContext & context, const RectInt & window )
{
	// render this view, window is in screen space
}

bool ViewCredits::onCursorMove( const RectInt & window, 
						const PointInt & position,		// window and position are local coordinates
						const PointInt & delta )
{
	return false;
}

bool ViewCredits::onLeftDown( const RectInt & window, 
						const PointInt & position )
{
	return false;
}

bool ViewCredits::onLeftUp( const RectInt & window, 
						const PointInt & position )
{
	return false;
}

bool ViewCredits::onRightDown( const RectInt & window,
						const PointInt & position )
{
	return false;
}

bool ViewCredits::onRightUp( const RectInt & window,
						const PointInt & position )
{
	return false;
}

//----------------------------------------------------------------------------


bool ViewCredits::onButtonOkay(const Message & msg)
{
	((GameDocument *)document())->setScene( "Main" );
	return true;
}

