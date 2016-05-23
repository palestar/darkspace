/*
	LayoutTargetButtons.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTTARGETBUTTONS_H
#define LAYOUTTARGETBUTTONS_H

#include "Gui3d/WindowLayout.h"
#include "Gui3d/WindowText.h"
#include "World/Noun.h"
#include "DarkSpace/NounDropTarget.h"
#include "DarkSpace/NounStructure.h"
#include "DarkSpace/StructureFactory.h"
#include "DarkSpace/StructureBarracks.h"

//----------------------------------------------------------------------------

class LayoutTargetButtons : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
						LayoutTargetButtons();
	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();		
	void				onUpdate( float t );

private:
	// Data
	Noun::wRef			m_LastTarget;
};

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
//EOF
