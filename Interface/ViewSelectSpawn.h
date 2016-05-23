/*
	ViewSelectSpawn.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWSELECTSPAWN_H
#define VIEWSELECTSPAWN_H

#include "System/Messages.h"
#include "Render3d/Material.h"
#include "Gui3d/WindowView.h"
#include "Game/Noun.h"

//----------------------------------------------------------------------------

class ViewSelectSpawn : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewSelectSpawn >	Ref;

	// Construction
	ViewSelectSpawn();

	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	// {{BEGIN_DATA
	WindowList *				m_pListSpawn;
	WindowEdit *				m_pChatEdit;
	NodeWindow *				m_pChatEditWindow;
	WindowText *				m_pTextSpawn;
	WindowButton *				m_pButtonOkay;
	NodeWindow *				m_pObserveWindow;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onListSpawn(const Message & msg);
	bool				onChatEdit(const Message & msg);
	bool				onButtonCancel(const Message & msg);
	bool				onButtonOkay(const Message & msg);
	// END_MSG}}

private:
	// Mutators
	void				onSelectSpawn( Noun * pSpawn );
	void				selectSpawn();
	void				updateSpawnList();

	// Data
	Noun::wRef			m_rSelectedSpawn;
	float				m_UpdateSpawnList;
	Array< Noun::wRef >	m_Spawn;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
