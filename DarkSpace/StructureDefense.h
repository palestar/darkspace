/*
	StructureDefense.h

	This structure defends the planet from orbital attack
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef STRUCTUREDEFENSE_H
#define STRUCTUREDEFENSE_H

#include "NounProjectile.h"
#include "NounStructure.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL StructureDefense : public NounStructure
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Construction
	StructureDefense();

	// Noun interface
	void				initialize();
	void				simulate( dword nTick );
	// NounStructure interface
	virtual int			sortId() const;
	virtual int			groupId() const;
	virtual int			maxDamage() const;					// how much damage can this structure take
	virtual int			buildTechnology() const;			// technology level required to build
	virtual int			buildTime() const;					// time in seconds to build this structure
	virtual int			buildCost() const;					// cost to build this structure
	virtual int			repairRate() const;					// damaged repaired per second

	virtual int			workers() const;
	virtual int			power() const;
	virtual int			technology() const;
	virtual int			defense() const;					// damage per second against enemy ships
	virtual float		protection() const;					// percentage damage reduction against attacks on this planet

	CharString			status() const;						// status string
	Color				color() const;

	bool				canUpgrade( NounStructure * pStructure ) const;

	// Accessors
	int					shielding() const;					// get the shielding status

	// Mutators
	void				useGadget( Noun * pTarget, NounGadget * pGadget );
	void				flushUse();

	// Helpers
	static bool			isTargeted( NounPlanet * pPlanet, 
							Noun * pProj );

protected:
	// non-serialized
	int					m_Shielding;
	int					m_nUseGadgets;

	Noun::wRef			m_pUseTarget;
	Array< NounGadget * > 	
						m_Use;
};

//----------------------------------------------------------------------------

inline int StructureDefense::shielding() const
{
	return m_Shielding;
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
