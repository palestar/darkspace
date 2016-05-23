/**
	@file NounGame.h
	@brief Base class for all Game Noun objects.

	(c)2006 Palestar Inc
	@author Richard Lyle @date 4/29/2007 4:45:14 PM
*/

#ifndef NOUNGAME_H
#define NOUNGAME_H

#include "World/Noun.h"
#include "GameDll.h"

class GameContext;

//---------------------------------------------------------------------------------------------------

/*
	The following enumeration contains all the possible types interactions that can occur between two nouns. 

	The Noun class implements a generic interface for noun interactions... a Noun can now specify what interactions it considers
	acceptable through the canInteract() virtual, and performs those interactions with the interact() virtual function. This is
	intended to be a low-level interface between nouns.
*/

enum NounInteraction
{
	INTERACT_USE,				// Generic interactions
	INTERACT_BUILD,
	INTERACT_REPAIR,
	
	INTERACT_ORBIT,				// can we orbit
	INTERACT_ENTER_ORBIT,		// can we enter orbit now
	INTERACT_LOAD,				// can we load the object

	// add interactions 
	LAST_INTERACTION			// leave at end
};

//---------------------------------------------------------------------------------------------------

class DLL NounGame : public Noun
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< NounGame >			Ref;
	typedef WidgetReference< NounGame >		wRef;
	typedef TargetReference< NounGame >		tRef;

	// Construction
	NounGame();

	// Noun interface
	virtual CharString		displayName( bool a_bAllowTags ) const;	
	virtual void			update();

	// Accessors
	GameContext *			gameContext() const;
	float					signature() const;				// this objects current signature

	bool					inOrbit() const;				// are we currently in orbit
	Noun *					orbiting() const;				// object we are orbiting
	Noun *					orbitRoot() const;				// this normally returns the primary star of a system
	const char *			starSystem() const;				// returns the name of the star system if any of this object
	const char *			serverName() const;				// returns the name of the server this object is in

	// get the target status of this object, tag support is assumed.
	virtual CharString		status() const;					

	virtual bool			canInteract( NounInteraction nInteraction, 
								Noun * pWith ) const;

	virtual bool			canDamage( dword type ) const;	// can this object be damaged by the specified damage type
	virtual bool			canOrbit() const;				// can another object orbit this one
	virtual bool			canShipCollide() const;			// return true if this object should be avoided by ships

	virtual bool			canBeDetected() const;			// can this object be detected
	virtual bool			enableECM() const;				// can ECM/ECCM affect the signature of this object
	virtual bool			enablePD() const;				// is this a point defense target (i.e. missiles, bombs)
	virtual float			baseSignature() const;			// what is the default signature for this noun (by default the radius)
	virtual float			viewDetectScale() const;		// this affects the canDetect() function by scaling the radius of the object to determine the visibile range.
	virtual float			visibility() const;				// how visible is this object

	virtual bool			isCargo() const;				// can this object be loaded/unloaded onto a ship
	virtual bool			canAttachCargo( 
								Noun * pCargo ) const;		// can the specified cargo be attached to this noun, used by findCargoHold()

	virtual int				resourceCount() const;			// returns the total number of resources available...
	virtual int				useResources( int nWanted );	// use resources of this object, returns the amount actually used...

	dword					detectionFlags() const;			// get the detection flags
	bool					isDetected( int nFactionId ) const;				// returns true if this object is detected by the provided faction
	bool					isDetected( const Noun * pNoun ) const;		// returns true if this object is detected by the noun

	bool					canDetect( Noun * pNoun,
								float sensor,				// at what distance can we detect 1.0 of signature
								float view ) const;			// at what distance can we see a object with a radius of 1 
	bool					validateTarget( const Noun * pNoun ) const;	// returns true if this object has detected the noun

	// depreciated
	virtual bool			enableOrbit() const;			// can this object orbit another

	// Mutators
	void					addSignature( dword when, 
								float sig );				// add signature to this noun
	void					resetSignature();				// finalize the current signature, and reset to the baseSignature()
	void					setDetectionFlags( dword nFlags );

	virtual void			interact( NounInteraction nInteraction,
								Noun * pWith, int nTicks = 1 );
	virtual void			inflictDamage( dword nWhen,
								Noun * pFrom, int damage, dword type, 
								const Vector3 & direction );	// called by VerbDamage()

	// Static
	static Noun *			findCargoHold( Noun * pNoun, Noun * pCargo );

protected:
	// Data
	dword					m_DetectionFlags;				// which factions can detect this object
	// emitSignature()
	float					m_Signature;					// objects current signature
	dword					m_SignatureTick;
	float					m_LastSignature;				// set when resetSignature() is called
};

//---------------------------------------------------------------------------------------------------

inline GameContext * NounGame::gameContext() const
{
	return (GameContext *)context();
}

inline float NounGame::signature() const
{
	return m_LastSignature;
}

inline dword NounGame::detectionFlags() const
{
	return m_DetectionFlags;
}

inline bool NounGame::isDetected( int nFactionId ) const
{
	return canBeDetected() && (m_DetectionFlags & (1 << nFactionId)) != 0;
}

inline bool NounGame::isDetected( const Noun * pNoun ) const
{
	return pNoun != NULL && canBeDetected() ? isDetected( pNoun->factionId() ) : false;
}

inline float NounGame::viewDetectScale() const
{
	return 1.0f;
}

//---------------------------------------------------------------------------------------------------


#endif

//---------------------------------------------------------------------------------------------------
//EOF
