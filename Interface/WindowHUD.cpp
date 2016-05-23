/*
	WindowHUD.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Interface/WindowHUD.h"
#include "Display/PrimitiveLineList.h"
#include "Display/PrimitiveLineStrip.h"
#include "Display/PrimitiveTriangleFan.h"
#include "Display/PrimitiveMaterial.h"
#include "Math/Helpers.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowHUD, NodeWindow );
REGISTER_FACTORY_KEY( WindowHUD, 4210285268953107469 );

WindowHUD::WindowHUD()
{
	m_ActionTime = 0;
	m_MapOffset.set( 0, 0 );
	m_MapVelocity.set( 0, 0 );
	m_Flare = 1.0f;
}

//----------------------------------------------------------------------------

void WindowHUD::onUpdate( float t )
{
	NodeWindow::onUpdate( t );
}

const float ANIM_TIME = 0.25f;
const int	MAX_LINE_SEGMENTS = 9;

void WindowHUD::onRender( RenderContext & context,const RectInt & window )
{}


//----------------------------------------------------------------------------
//EOF
