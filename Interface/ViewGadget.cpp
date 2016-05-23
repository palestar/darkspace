/*
	ViewGadget.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Interface/ViewGadget.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewGadget, WindowView::View );
REGISTER_FACTORY_KEY( ViewGadget, 4218019884878194369 );

ViewGadget::ViewGadget()
{
	// Construct your view class
}

ViewGadget::~ViewGadget()
{
	// implement destruction
}

//----------------------------------------------------------------------------

void ViewGadget::onActivate()
{
	//{{BEGIN_DATA_INIT
	m_pGadgetRender = WidgetCast<NodeWindow>( window()->findNode( "GadgetRender" ) );
	m_pAutoText = WidgetCast<WindowText>( window()->findNode( "AutoText" ) );
	m_pRepairText = WidgetCast<WindowText>( window()->findNode( "RepairText" ) );
	m_pAutoPanel = WidgetCast<NodeWindow>( window()->findNode( "AutoPanel" ) );
	m_pRepairPanel = WidgetCast<NodeWindow>( window()->findNode( "RepairPanel" ) );
	m_pGadgetDescription = WidgetCast<WindowText>( window()->findNode( "GadgetDescription" ) );
	//END_DATA_INIT}}

	// called when this view is created
}

void ViewGadget::onDeactivate()
{
	// called before this view is destroyed
}

void ViewGadget::onUpdate( float t )
{
	// implement view updating
}

bool ViewGadget::onMessage( const Message & msg )
{
	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONUP, 992988836, onButtonAutoAttack);
	MESSAGE_MAP( WB_BUTTONUP, 1477311301, onButtonAutoDefend);
	MESSAGE_MAP( WB_BUTTONUP, 2542746716, onButtonAutoOff);
	MESSAGE_MAP( WB_BUTTONUP, 3883543142, onButtonRepairQueue);
	MESSAGE_MAP( WB_BUTTONUP, 920750390, onButtonRepairLast);
	MESSAGE_MAP( WB_BUTTONUP, 2981508253, onButtonRepairNow);
	MESSAGE_MAP( WB_BUTTONUP, 582488779, onButtonClose);
	//END_MSG_MAP}}

	return false;
}

void ViewGadget::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

//----------------------------------------------------------------------------


bool ViewGadget::onButtonClose(const Message & msg)
{
	return true;
}

bool ViewGadget::onButtonRepairNow(const Message & msg)
{
	return true;
}

bool ViewGadget::onButtonRepairLast(const Message & msg)
{
	return true;
}

bool ViewGadget::onButtonRepairQueue(const Message & msg)
{
	return true;
}

bool ViewGadget::onButtonAutoOff(const Message & msg)
{
	return true;
}

bool ViewGadget::onButtonAutoDefend(const Message & msg)
{
	return true;
}

bool ViewGadget::onButtonAutoAttack(const Message & msg)
{
	return true;
}

