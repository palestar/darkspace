// ChildFrame.cpp : implementation file
//

#define UNIVERSECONTEXTPORT_DLL
#include "stdafx.h"
#include "ChildFrame.h"

#include "resource.h"

extern AFX_EXTENSION_MODULE g_PortDLL;

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

CChildFrame::CChildFrame()
{
}

CChildFrame::~CChildFrame()
{
}


BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	// we want to load a specific menu from this DLL
	HINSTANCE hInstOld = AfxGetResourceHandle();
	AfxSetResourceHandle(g_PortDLL.hModule);

	if (!m_Menu.LoadMenu(IDR_PORT))
	{
		// restore the old resource chain and return error
		AfxSetResourceHandle(hInstOld);
		return FALSE;
	}
	AfxSetResourceHandle(hInstOld); // restore the old resource chain

	// when the list output frame window is active, use this menu
	m_hMenuShared = m_Menu.m_hMenu;
	
	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}
