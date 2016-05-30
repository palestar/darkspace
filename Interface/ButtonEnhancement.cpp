/*
	WindowEnhancement.h

	(c)2008 Palestar, Jeff Everett
*/

#include "Debug/Assert.h"
#include "Render3D/NodeLight.h"
#include "Render3D/NodeSound.h"
#include "Gui3d/NodeWindow.h"
#include "Gui3D/WindowButton.h"
#include "Interface/ButtonEnhancement.h"
#include "Interface/GameDocument.h"
#include "DarkSpace/NounShip.h"
#include "DarkSpace/NounCargo.h"
#include "DarkSpace/NounStructure.h"
#include "DarkSpace/NounDropTarget.h"
#include "DarkSpace/NounBeacon.h"
#include "Interface/WindowEnhancement.h"
#include "DarkSpace/NounEnhancement.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ButtonEnhancement, WindowButton )

ButtonEnhancement::ButtonEnhancement()
{}

void ButtonEnhancement::onActivate()
{}

//----------------------------------------------------------------------------

void ButtonEnhancement::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender(context,window);
}


void ButtonEnhancement::onButtonUp()
{
	if(m_pEnhancementWindow->getSelected() == this)
		m_pEnhancementWindow->setSelected(NULL);
	else
		m_pEnhancementWindow->setSelected(this);
	WindowButton::onButtonUp();
}


//----------------------------------------------------------------------------
// EOF
