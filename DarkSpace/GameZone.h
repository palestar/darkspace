/**
	@file GameZone.h
	@brief TODO

	(c)2006 Palestar Inc
	@author Richard Lyle @date 10/18/2006 8:29:57 PM
*/

#ifndef GAMEZONE_H
#define GAMEZONE_H

#include "World/NodeZone.h"
#include "GameDll.h"

//---------------------------------------------------------------------------------------------------

class DLL GameZone : public NodeZone
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	GameZone();

	// Accessors
	int					planetCount() const;						// number of planets in this zone
	int					shipCount() const;							// number of ships in this zone
	int					factionPlanets( int factionId ) const;		// number of planets owned by this faction
	int					factionHome() const;						// which faction has a home jumpgate in this system

protected:
	// Data
	int					m_nPlanetCount;
	int					m_nShipCount;

	// NodeZone interface
	virtual bool		hookNouns( BaseNode::Ref pNode );				// attach all nouns to update chain
	virtual void		unhookNouns( BaseNode::Ref pNode );			// detach all nouns from update chain
};

//---------------------------------------------------------------------------------------------------

inline int GameZone::planetCount() const
{
	return m_nPlanetCount;
}

inline int GameZone::shipCount() const
{
	return m_nShipCount;
}

//---------------------------------------------------------------------------------------------------

#endif

//---------------------------------------------------------------------------------------------------
//EOF
