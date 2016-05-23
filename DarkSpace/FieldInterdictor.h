/*
	FieldInterdictor.h
	(c)2004 Palestar Inc, Richard Lyle
*/

#ifndef FIELDINTERDICTOR_H
#define FIELDINTERDICTOR_H

#include "NounField.h"

//----------------------------------------------------------------------------

class FieldInterdictor : public NounField
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< FieldInterdictor >			Ref;
	typedef WidgetReference< FieldInterdictor >		wRef;

	// Construction
	FieldInterdictor();

	// Noun interface
	void		simulate( dword nTick );							// update noun forward in time by one tick

	// NounField interface
	void		onEnterField( Noun * pNoun );
	void		onInsideField( Noun * pNoun );
	void		onLeaveField( Noun * pNoun );
};

//----------------------------------------------------------------------------

#endif

//----------------------------------------------------------------------------
//EOF
