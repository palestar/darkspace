/*
	NounJumpGate.h

	This object moves other objects from one location to another or detaches them from the world if no destination is provided.

	Gates can have duration and intervals, which can control when the gate is active or inactive. If a duration is provided, but
	no interval is set, then the gate is detached from the world when deactivated. if an interval is provided, which is always
	greater than the duration, then the gate will reactivate once the interval time has elapsed and remain active for the duration
	again which is useful for timed wormholes, etc..

	(c)1999 PaleStar, Richard Lyle
*/

#ifndef NOUN_JUMP_GATE_H
#define NOUN_JUMP_GATE_H

#include "NounBody.h"
#include "NounShip.h"
#include "FieldSafeZone.h"
#include "Constants.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class GameContext;		// forward declare

class DLL NounJumpGate : public NounBody
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounJumpGate >				Ref;
	typedef WidgetReference< NounJumpGate >			wRef;
	
	//! Static
	static bool				sm_bBuildMode;				// if true, then jump gates will not try to link to their destination yet

	// Construction
	NounJumpGate();

	// Widget interface
	bool					read( const InStream & input );
	// Node interface
	void					render( RenderContext &context, 
								const Matrix33 & frame, 
								const Vector3 & position );

	// Noun interface
	NounType				nounType() const;	
	bool					collidable() const;
	
	void					initialize();
	void					release();
	void					simulate( dword nTick );

	void					collision( Noun * pCollide,
								 const Vector3 & intersect );

	// NounGate interface
	virtual CharString		status() const;					// get the target status of this object

	// Accessors
	bool					active() const;					// is the gate active
	bool					autoActivate() const;			// does the gate automatically go active/inactive
	bool					isLocalDestination() const;		// returns true if gate is linked to an object in the same context
	bool					isRemoteServer() const;			// returns true if this is a remote server destination
	const char *			address() const;				// returns the address of the remote server
	int						port() const;					// returns the port of the remote server

	const char *			destination() const;			// returns the name of the destination
	Noun *					destinationNoun() const;		// returns a pointer to the destination object
	const char *			description() const;			// description of this JG
	bool					safeZone() const;				// does this gate have a safe zone
	float					safeZoneDistance() const;		// radius of that safe zone
	
	bool					duration() const;		
	dword					durationTicks() const;
	bool					interval() const;
	dword					intervalTicks() const;

	FieldSafeZone *			field() const;
	bool					destinationInvalid() const;		// returns true if destination couldn't be found

	// Mutators
	void					setActive( bool bActive );	// turn gate on/off
	void					setAutoActivate( bool a_bAutoActivate );
	void					setRemoteServer( const char * a_pAddress, int a_nPort );
	void					clearRemoteServer();
	void					setDestination( const char * pName );
	void					setDestination( Noun * pNoun );
	void					setDescription( const char * pDescription );
	void					setSafeZone( bool bSafeZone, float fDistance = 1000.0f );

	void					setDuration( bool bDuration, dword nDuration );
	void					setInterval( bool bInterval, dword nInterval );

	bool					updateDestination( bool bForce = false );

	// Helpers
	Scene *					useEffect() const;
	void					createUseEffect();			// creates a white flash when this gate is used
	
protected:
	bool					m_bActive;					// is the gate active
	bool					m_bAutoActivate;			// if true, then this gate is only active if the destination is valid
	bool					m_bRemoteServer;			// if true, then client will connect to a new server when this gate is hit..
	CharString				m_sAddress;					// IP address of remote server
	int						m_nPort;					// port of remote server

	CharString				m_sDestination;				// name of the destination node..
	Noun::wRef				m_pDestination;				// node of our destination 
	bool					m_bSafeZone;				// if true, any ships within the safe zone distance have weapons disabled
	float					m_fSafeZoneDistance;		// set the safe zone distance
	FieldSafeZone::wRef		m_pSafeZone;

	bool					m_bDuration;				// if true, this gate is only active for the duration, then detached if no interval is set
	dword					m_nDuration;				// number of ticks the gate is active
	bool					m_bInterval;				
	dword					m_nInterval;
	
	dword					m_nGateTick;				// current tick for use by duration/interval

	// non-serialized
	dword					m_nLastUse;
	bool					m_bDestinationInvalid;		// set to true if the destination noun can't be found
};

//----------------------------------------------------------------------------

inline bool NounJumpGate::active() const
{
	return m_bActive;
}

inline bool NounJumpGate::autoActivate() const
{
	return m_bAutoActivate;
}

inline bool NounJumpGate::isLocalDestination() const
{
	return m_pDestination.valid();
}

inline bool NounJumpGate::isRemoteServer() const
{
	return m_bRemoteServer;
}

inline const char * NounJumpGate::address() const
{
	return m_sAddress;
}

inline int NounJumpGate::port() const
{
	return m_nPort;
}

inline const char * NounJumpGate::destination() const
{
	return m_sDestination;
}

inline Noun * NounJumpGate::destinationNoun() const
{
	return m_pDestination;
}

inline bool NounJumpGate::safeZone() const
{
	return m_bSafeZone;
}

inline float NounJumpGate::safeZoneDistance() const
{
	return m_fSafeZoneDistance;
}

inline bool NounJumpGate::duration() const
{
	return m_bDuration;
}

inline dword NounJumpGate::durationTicks() const
{
	return m_nDuration;
}

inline bool NounJumpGate::interval() const
{
	return m_bInterval;
}

inline dword NounJumpGate::intervalTicks() const
{
	return m_nInterval;
}

inline FieldSafeZone * NounJumpGate::field() const
{
	return m_pSafeZone;
}

inline bool	NounJumpGate::destinationInvalid() const
{
	return m_bDestinationInvalid;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
