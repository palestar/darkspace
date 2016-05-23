/*
	GadgetsDll.h
	(c)1999 PaleStar, Richard Lyle
*/

#include "Standard/Dll.h"

//-------------------------------------------------------------------------------

#undef DLL

#ifdef GADGETS_DLL
#define DLL			DLL_EXPORT
#else
#define DLL			DLL_IMPORT
#endif

//----------------------------------------------------------------------------
// EOF
