/*
	ViewModerate.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWMODERATE_H
#define VIEWMODERATE_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"
#include "Interface/WindowObserve.h"
#include "Interface/GameDocument.h"



//----------------------------------------------------------------------------

class ViewModerate : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewModerate >	Ref;

	// Construction
	ViewModerate();

	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );

	// {{BEGIN_DATA
	WindowText *				m_pTextStatus;
	WindowEdit *				m_pChatEdit;
	NodeWindow *				m_pChatEditWindow;
	WindowList *				m_pListObjects;
	WindowList *				m_pListServers;
	WindowList *				m_pListClients;
	WindowObserve *				m_pObserveWindow;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onListObjects(const Message & msg);
	bool				onButtonRefresh(const Message & msg);
	bool				onButtonExit(const Message & msg);
	bool				onChatEdit(const Message & msg);
	bool				onButtonConnect(const Message & msg);
	bool				onButtonBan(const Message & msg);
	bool				onButtonKick(const Message & msg);
	// END_MSG}}

private:
	void				updateLists();

	float				m_UpdateTime;

	GameClient::ServerStatus
						m_Status;
	Array< MetaClient::Server >
						m_Servers;
	Array< Noun::wRef >
						m_Objects;

	dword				m_nLastUpdate;
	dword				m_nLastBytesRecv;
	dword				m_nLastBytesSent;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
