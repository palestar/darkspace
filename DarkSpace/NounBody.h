/*
	NounBody.h - This class is depreciated
	(c)1999 PaleStar Development, Richard Lyle
*/

#ifndef NOUN_BODY_H
#define NOUN_BODY_H

#include "NounGame.h"
#include "Constants.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounBody : public NounGame		// DEPRECIATED!
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounBody >				Ref;
	typedef WidgetReference< NounBody >			wRef;
	typedef WeakReference< NounBody >			WeakRef;

	// Construction
	NounBody();

	//! Noun interface
	virtual NounType		nounType() const;		
	//! NounGame interface
	virtual bool			enableOrbit() const;
	virtual bool			canBeDetected() const;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
