/*
	InterfaceDll.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Standard/Dll.h"

//-------------------------------------------------------------------------------

#undef DLL

#ifdef INTERFACE_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT
#endif

//----------------------------------------------------------------------------
// EOF

