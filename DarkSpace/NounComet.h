/*
	NounComet.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNCOMET_H
#define NOUNCOMET_H

#include "NounAsteroid.h"

//----------------------------------------------------------------------------

class NounComet : public NounAsteroid
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	NounComet();
	// Noun interface
	void			simulate( dword nTick );
};

#endif

//----------------------------------------------------------------------------
//EOF
