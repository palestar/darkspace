/*
	GameDll.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Standard/Dll.h"

//----------------------------------------------------------------------------

#undef DLL

#ifdef GAME_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT

#ifdef _DEBUG
#pragma comment(lib,"DarkSpaceD.lib") 
#else
#pragma comment(lib,"DarkSpace.lib") 
#endif

#endif

//----------------------------------------------------------------------------
// EOF
