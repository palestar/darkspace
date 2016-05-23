/*
	ViewConnectServer.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWCONNECTSERVER_H
#define VIEWCONNECTSERVER_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"



//----------------------------------------------------------------------------

class ViewConnectServer : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewConnectServer >	Ref;

	// Construction
						ViewConnectServer();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	// {{BEGIN_DATA
	WindowBar *				m_pProgress;
	WindowText *			m_pStatus;
	// END_DATA}}

	// {{BEGIN_MSG
	// END_MSG}}

private:
	// Data
	Array< BaseNode::Ref >	
						m_PreCache;
	int					m_PreCacheIndex;
	float				m_fPreCacheTime;
	bool				m_bCacheInitialized;
	bool				m_bLoginPending;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
