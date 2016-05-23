/*
	NounSmartProjectile.h
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNSMARTPROJECTILE_H
#define NOUNSMARTPROJECTILE_H

#include "NounProjectile.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounSmartProjectile : public NounProjectile
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< NounSmartProjectile >	wRef;

	enum  {
		FLAG_AUTO_TARGET	= 0x00000004,			// if no target, project will attempt to find a new target automatically..
		FLAG_CAN_STOP		= 0x00000008,			// if the target is lost, does this projectile slow to a stop
	};

	// Construction
	NounSmartProjectile();

	// Noun interface
	void					initialize();
	void					simulate( dword nTick );

protected:
	// Mutators
	void					control( const Vector3 & toward );
	// Data
	float					m_fTurnRate;
	float					m_fVelocity;
	float					m_fYaw, m_fPitch;

	// non-serialized
	float					m_fRoll;
	dword					m_nLogicTick;
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
