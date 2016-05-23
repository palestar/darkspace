/*
	Helpers.h

	Misc template routines used by the interface code
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef INTERFACE_HELPERS_H
#define INTERFACE_HELPERS_H

#include "Gui3d/NodeWindow.h"

//----------------------------------------------------------------------------

template<class T>
T * GetButton( NodeWindow * pParent, int n )
{
	if ( n >= 0 && n < pParent->childCount())
	{
		T * pButton = dynamic_cast<T *>( pParent->child(n) );
		if ( pButton != NULL )
			return pButton;
	}

	// create a new button, and insert it at this desired position
	T * pButton = new T;
	pParent->insertNode( n, pButton );

	return pButton;
}

template<class T>
int GetNouns( Noun * pCheck, Array<T *> & nouns )
{
	if ( dynamic_cast<T *>( pCheck ) != NULL )
		nouns.push( (T *)pCheck );

	// recurse into child nodes
	for(int i=0;i<pCheck->childCount();i++)
		GetNouns<T>( (Noun *)pCheck->child(i), nouns );

	return nouns.size();
}

template<class T>
void CullContacts( Noun * pCenter, int maxContacts, Array< T * > & contacts )
{
	// trim planets down if it exceeds our maximum number of contacts
	while( contacts.size() > maxContacts )
	{
		// find the object with the greatest distance and remove it from the list
		float cullRange = 0.0f;
		int cullIndex = -1;

		for(int i=0;i<contacts.size();i++)
		{
			float distance = (pCenter->worldPosition() - contacts[i]->worldPosition()).magnitude2();
			if ( distance > cullRange )
			{
				cullIndex = i;
				cullRange = distance;
			}
		}
		
		if ( cullIndex < 0 )
			break;		// failed to find an object to cull, exit while loop

		contacts.removeSwap( cullIndex );
	}
}

#endif

//----------------------------------------------------------------------------
//EOF
