// NounStructurePort.cpp : Defines the initialization routines for the DLL.
//

#define NOUNSTRUCTUREPORT_DLL
#include "stdafx.h"
#include <afxdllx.h>

#include "Port.h"
#include "Resource.h"
#include "DarkSpace/GameContext.h"


static AFX_EXTENSION_MODULE NounStructurePortDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	static HICON						Icon;

	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//TRACE0("NOUNSTRUCTUREPORT.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(NounStructurePortDLL, hInstance))
			return 0;
		new CDynLinkLibrary(NounStructurePortDLL);

		Icon = LoadIcon( hInstance, MAKEINTRESOURCE(IDR_PORT) );

		// register with ScenePort
		BaseNodePort::registerPort( CLASS_KEY(NounStructurePort), Icon );

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		//TRACE0("NOUNSTRUCTUREPORT.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(NounStructurePortDLL);

		// unregister port
		BaseNodePort::unregisterPort( CLASS_KEY(NounStructurePort) );

		DeleteObject(Icon);
	}
	return 1;   // ok
}
