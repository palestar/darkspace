/*
	TextMission.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"

#include "Interface/WindowMission.h"

//----------------------------------------------------------------------------

Constant UPDATE_MISSION_WINDOW( "UPDATE_MISSION_TEXT", 5.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowMission, NodeWindow );

WindowMission::WindowMission()
{}

//----------------------------------------------------------------------------

void WindowMission::onActivate()
{
	NodeWindow::onActivate();
}

void WindowMission::onUpdate( float t )
{
	NodeWindow::onUpdate( t );

	if ( visible() )
	{
		if ( activeTime() > UPDATE_MISSION_WINDOW )
		{

			setActiveTime( 0 );
		}
	}
}

//----------------------------------------------------------------------------
//EOF
