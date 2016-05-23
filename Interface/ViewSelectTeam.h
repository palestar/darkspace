/*
	ViewSelectTeam.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWSELECTTEAM_H
#define VIEWSELECTTEAM_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"

//----------------------------------------------------------------------------

class ViewSelectTeam : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewSelectTeam >	Ref;

	// Construction
						ViewSelectTeam();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	// {{BEGIN_DATA
	NodeWindow *		m_pObserveWindow;
	WindowButton *		m_pButtonOkay;
	WindowList *		m_pTeamList;
	WindowText *		m_pTeamDescription;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onSelectTeam(const Message & msg);
	bool				onButtonCancel(const Message & msg);
	bool				onButtonOkay(const Message & msg);
	// END_MSG}}

private:
	// Data
	Array< GameContext::Team >
						m_Teams;

	int					m_SelectedTeam;
	float				m_UpdateTeamList;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
