/*
	FieldSafeZone.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef FIELDSAFEZONE_H
#define FIELDSAFEZONE_H

#include "NounField.h"

//----------------------------------------------------------------------------

class FieldSafeZone : public NounField
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< FieldSafeZone >				Ref;
	typedef WidgetReference< FieldSafeZone >		wRef;

	// Construction
	FieldSafeZone();

	// Noun interface
	void		simulate( dword nTick );					

	// NounField interface
	void		onEnterField( Noun * pNoun );
	void		onInsideField( Noun * pNoun );
	void		onLeaveField( Noun * pNoun );
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
