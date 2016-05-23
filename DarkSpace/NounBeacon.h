/*
	NounBeacon.h

	This object is attached as a child object by GadgetBeacon to a ship or planet
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNBEACON_H
#define NOUNBEACON_H

#include "GadgetBeacon.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounBeacon : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounBeacon >		Ref;

	// Construction
	NounBeacon();
	NounBeacon( const Vector3 & position, 
		GadgetBeacon * pGadget );		

	// BaseNode interface
	void				onAttached();
	void				onDetaching();
	void				render( RenderContext &context, 
							const Matrix33 & frame, 
							const Vector3 & position );

	// Noun interface
	NounType			nounType() const;				
	bool				canBeDetected() const;			// can this object be detected
	float				baseSignature() const;

	void				release();
	void				simulate( dword nTick );

	// Accessors
	Noun *				owner() const;					// which zone parent object attached this beacon

	// Static
	static NounBeacon *	findBeacon( Noun * pNoun, Noun * pFrom );

private:
	// Data
	GadgetBeacon::wRef	m_GadgetBeacon;
	int					m_Life;
	float				m_BaseSignature;

	// non-serialized
	Scene::Link			m_pHitEffect;
};

//----------------------------------------------------------------------------

inline Noun * NounBeacon::owner() const
{
	return m_GadgetBeacon.valid() ? m_GadgetBeacon->zoneParent() : NULL;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
