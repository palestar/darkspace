/*
	GadgetDropPod.h

	Gadget that creates an infantry unit in a pod for boarding
*/

#ifndef GADGETDROPPOD_H
#define GADGETDROPPOD_H

#include "NounGadget.h"
#include "NounUnit.h"
#include "GameDll.h"


//---------------------

class DLL GadgetDropPod : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	//constructor
	GadgetDropPod();

	// Noun interface
	void					setup();
	void					initialize();
	dword					hotkey() const;

	// NounGadget interface
	Type					type() const;										
	EnergyClass				energyClass() const;

	bool					usable( Noun * pTarget, bool shift ) const;			
	void					use( dword when, Noun * pTarget, bool shift);		
	int						useEnergy( dword nTick,int energy );
	void					loadUnit();						//load the unit up and stick it into m_pUnit
	bool					updateLogic();					//AI Usage
	int						usableWhen();					

	//accessors
	virtual int		energyNeeded() const = 0;		// how much energy is needed
	virtual int		chargeRate() const = 0;			// how fast does this weapon charge
	virtual float	range() const = 0;				// range of this weapon
	virtual bool	canDropOnPlanet() const = 0;	// can this device drop infantry onto a planet?

private:

	// Data
	int				m_Energy;

	// non-serialized
	dword			m_nEnergyTick;
	float			m_fChargeRate;
	Scene::Ref		m_pBuild;
	NounUnit *		m_pUnit;

};


#endif

