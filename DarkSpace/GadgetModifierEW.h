#ifndef GADGET_MODIFIER_EW_H 
#define GADGET_MODIFIER_EW_H

#include "Constants.h"
#include "NounGadget.h"
#include "NounShip.h"
#include "GameDll.h"

class DLL GadgetModifierEW : public NounGadget
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	GadgetModifierEW();

	Type 			type() const;
	dword 			hotkey() const;

	virtual CharString					useTip(Noun * pTarget, bool shift) const;
	bool								usable(Noun * pTarget, bool shift) const;
	void								use(dword when, Noun * pTarget, bool shift);
	bool								useActive() const;
	int									useEnergy(dword nTick, int energy);

	virtual int							energyCost() const = 0;
	virtual float						strength() const = 0;
	virtual float						range() const = 0;

	void								release();
	bool								active() const;

protected:
	NounShip::wRef						m_Target;
	Array< ModifierType >				m_Modifiers;
};

inline bool GadgetModifierEW::active() const {
	return (flags() & FLAG_ACTIVE) != 0;
}

#endif