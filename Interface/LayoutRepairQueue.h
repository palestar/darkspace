/*
	LayoutRepairQueue.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef LAYOUTREPAIRQUEUE_H
#define LAYOUTREPAIRQUEUE_H

#include "Gui3d/WindowButton.h"
#include "Gui3d/WindowLayout.h"



//----------------------------------------------------------------------------

class LayoutRepairQueue : public WindowLayout
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	class ButtonGadget : public WindowButton
	{
	public:
		// Construction
							ButtonGadget();
		// NodeWindow interface
		void				onRender( RenderContext & context,
								const RectInt & window );		// client render, window is in screen coordinates
		// WindowButton interface
		void				onButtonUp();
		// Mutators
		void				setGadget( NounGadget * pGadget );
	private:
		NounGadget::wRef	m_rGadget;
	};

	// Construction
	LayoutRepairQueue();

	// NodeInterfaceClient interface
	void				onActivate();
	void				onDeactivate();		
	void				onUpdate( float t );

};

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
