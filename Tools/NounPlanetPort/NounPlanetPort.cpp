// NounPlanetPort.cpp : Defines the initialization routines for the DLL.
//

#define NOUNPLANETPORT_DLL
#include "stdafx.h"
#include <afxdllx.h>

#include "Port.h"
#include "Resource.h"
#include "DarkSpace/GameContext.h"

static AFX_EXTENSION_MODULE NounPlanetPortDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	static HICON						Icon;

	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//TRACE0("NOUNPLANETPORT.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(NounPlanetPortDLL, hInstance))
			return 0;
		new CDynLinkLibrary(NounPlanetPortDLL);

		Icon = LoadIcon( hInstance, MAKEINTRESOURCE(IDR_PORT) );

		// register with ScenePort
		BaseNodePort::registerPort( CLASS_KEY(NounPlanetPort), Icon );

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//TRACE0("NOUNPLANETPORT.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(NounPlanetPortDLL);

		// unregister port
		BaseNodePort::unregisterPort( CLASS_KEY(NounPlanetPort) );

		DeleteObject(Icon);
	}
	return 1;   // ok
}
