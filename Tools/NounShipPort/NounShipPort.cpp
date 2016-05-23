// NounShipPort.cpp : Defines the initialization routines for the DLL.
//

#define NOUNSHIPPORT_DLL
#include "stdafx.h"
#include <afxdllx.h>

#include "Port.h"
#include "Resource.h"
#include "DarkSpace/GameContext.h"

static AFX_EXTENSION_MODULE NounShipPortDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	static HICON						Icon;

	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//TRACE0("NOUNSHIPPORT.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(NounShipPortDLL, hInstance))
			return 0;
		new CDynLinkLibrary(NounShipPortDLL);

		Icon = LoadIcon( hInstance, MAKEINTRESOURCE(IDR_PORT) );

		// register with ScenePort
		BaseNodePort::registerPort( CLASS_KEY(NounShipPort), Icon );

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//TRACE0("NOUNSHIPPORT.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(NounShipPortDLL);

		// unregister port
		BaseNodePort::unregisterPort( CLASS_KEY(NounShipPort) );

		DeleteObject(Icon);
	}
	return 1;   // ok
}
