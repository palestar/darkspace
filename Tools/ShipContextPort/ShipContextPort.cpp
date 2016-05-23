// ShipContextPort.cpp : Defines the initialization routines for the DLL.
//

#define SHIPCONTEXTPORT_DLL
#include "stdafx.h"
#include <afxdllx.h>

#include "Port.h"
#include "PortView.h"
#include "ChildFrame.h"
#include "Resource.h"

//-------------------------------------------------------------------------------

AFX_EXTENSION_MODULE						g_PortDLL = { NULL, NULL };

//-------------------------------------------------------------------------------

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	static HICON						Icon;

	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//TRACE0("SHIPCONTEXTPORT.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(g_PortDLL, hInstance))
			return 0;
		new CDynLinkLibrary(g_PortDLL);

		Icon = LoadIcon( hInstance, MAKEINTRESOURCE(IDR_PORT) );

		// register with resourcer
		Port::registerPort( CLASS_KEY( ShipContextPort ), Icon );
		//Port::registerImportExt( CLASS_KEY( ShipContextPort ), ".XXX" );	// TODO: For each supported extension call registerPortExtension()
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//TRACE0("SHIPCONTEXTPORT.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(g_PortDLL);

		// unregister with resourcer
		Port::unregisterPort( CLASS_KEY( ShipContextPort ) );
		//Port::unregisterImportExt( ".XXX" );						// TODO: release each register extension

		DeleteObject(Icon);
	}

	return 1;   // ok
}
