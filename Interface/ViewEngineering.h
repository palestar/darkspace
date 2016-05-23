/*
	ViewEngineering.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWENGINEERING_H
#define VIEWENGINEERING_H

#include "System/Messages.h"
#include "Display/Types.h"
#include "Gui3d/WindowView.h"
#include "Game/CargoGadget.h"
#include "Game/NounGadget.h"



//----------------------------------------------------------------------------

class ViewEngineering : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewEngineering >	Ref;

	// Construction
						ViewEngineering();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );

	void				onRender( RenderContext & context, 
							const RectInt & window );
private:
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

	// {{BEGIN_DATA
	NodeWindow *				m_pRepairQueue;
	NodeWindow *				m_pLayoutRepair;
	WindowText *				m_pShipStatus;
	// END_DATA}}

	// {{BEGIN_MSG
	// END_MSG}}

	// Data
	float				m_UpdateStatus;
	bool				m_Adjust;
	Vector3				m_AdjustPosition;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
