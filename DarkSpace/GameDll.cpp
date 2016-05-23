/*
	GameDll.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

//----------------------------------------------------------------------------


#include "Standard/Library.h"
#include "Standard/String.h"

#if defined(_WIN32)

#include <windows.h>

//----------------------------------------------------------------------------

#ifdef _DEBUG
Library		LIB_GADGETS( "GadgetsD.dll", true );
Library		LIB_INTERFACE( "InterfaceD.dll" );
#else
Library		LIB_GADGETS( "Gadgets.dll", true );
Library		LIB_INTERFACE( "Interface.dll" );
#endif

extern "C" int WINAPI 
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch( dwReason )
	{
	case DLL_PROCESS_ATTACH:
		StringBase::addStringTable( hInstance );
		break;
	case DLL_PROCESS_DETACH:
		StringBase::deleteStringTable( hInstance );
		break;
	}

	return 1;  
}

#else

#ifdef _DEBUG
Library		LIB_GADGETS( "libGadgetsD.so", true );
#else
Library		LIB_GADGETS( "libGadgets.so", true );
#endif

#endif

//----------------------------------------------------------------------------
//EOF
