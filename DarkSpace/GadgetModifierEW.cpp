#include "NounShip.h"
#include "GadgetModifierEW.h"
#include "VerbGadgetOff.h"

IMPLEMENT_ABSTRACT_FACTORY(GadgetModifierEW, NounGadget);
BEGIN_ABSTRACT_PROPERTY_LIST(GadgetModifierEW, NounGadget);
	ADD_TRANSMIT_UPDATE_PROPERTY(m_Target);
	ADD_REPLICATED_PROPERTY(m_Flags, TICKS_PER_SECOND);
END_PROPERTY_LIST();

GadgetModifierEW::GadgetModifierEW()
{
}

NounGadget::Type GadgetModifierEW::type() const
{
	return SPECIAL_OFFENSIVE;
}

dword GadgetModifierEW::hotkey() const
{
	return 'L';
}

CharString GadgetModifierEW::useTip(Noun * pTarget, bool shift) const
{
	CharString tip;
	tip += CharString().format("\nRange:<X;100>%.0fgu", range() * calculateModifier( MT_EWAR_RANGE ));
	tip += CharString().format("\nEnergy Cost:<X;100>%.1f", energyCost() * calculateModifier( MT_EWAR_ENERGY, true));
	if (active() && m_Target.valid())
		tip += CharString().format("\nTarget:<X;100>%s", m_Target->displayName(false));
	return tip;
}

bool GadgetModifierEW::usable(Noun * pTarget, bool shift) const
{
	if (active())
		return true;	// always allow it to be turned off...
	if (!NounGadget::usable(pTarget, shift))
		return false;

	NounShip *pTargetShip = WidgetCast<NounShip>(pTarget);

	if (!pTargetShip)
		return false;

	if (((NounShip *)pTarget)->isDestroyed())
		return false;

	if (!isEnemy(pTargetShip))
		return false;

	if (pTargetShip->testFlags( NounShip::FLAG_CLOAKED|NounShip::FLAG_IN_SAFE_ZONE ) )
		return false;

	if (active() && m_Target != pTargetShip)
		return false;

	float distance = (worldPosition() - pTarget->worldPosition()).magnitude();
	if (distance > range() * calculateModifier(MT_EWAR_RANGE))
		return false;        // target out of range

	return true;
}

bool GadgetModifierEW::useActive() const
{
	return active();
}

void GadgetModifierEW::use(dword when, Noun * pTarget, bool shift)
{
	if (active() && m_Target.valid())
	{
		NounGadget::use(when, pTarget, shift);

		for(ModifierType modifier : modifiersType())
			m_Target->addModifier(modifier, strength());

		if (isServer())
		{
			setFlags(FLAG_ACTIVE, false);
			message( CharString().format( "<color;ffffff>Tactical: %s Inactive on %s", name(), m_Target->name() ), true );
		}
		m_Target = NULL;
	}
	else
	{
		NounShip *pShipTarget = WidgetCast<NounShip>(pTarget);
		if (!shift && pShipTarget != NULL)
		{
			m_Target = pShipTarget;

			for (ModifierType modifier : modifiersType())
				m_Target->subtractModifier(modifier, strength());

			if (isServer())
			{
				setFlags(FLAG_ACTIVE, true);
				message(CharString().format("<color;ffffff>Tactical: %s Active on %s", name(), m_Target->name()), true);
			}
		}
	}
}
int GadgetModifierEW::useEnergy( dword nTick, int energy )
{
	if ( active() )
	{
		bool bTurnOff = true;

		if ( m_Target.valid() && !m_Target->isDestroyed() && !m_Target->testFlags( NounShip::FLAG_CLOAKED|NounShip::FLAG_IN_SAFE_ZONE ) )
		{
			int nCost = energyCost() * calculateModifier( MT_EWAR_ENERGY, true);
			float fDistance = (worldPosition() - m_Target->worldPosition()).magnitude();
			float fRange = range() * calculateModifier( MT_EWAR_RANGE );

			if ( fDistance <= fRange && energy >= nCost )
			{
				bTurnOff = false;

				energy -= nCost;
			}
		}

		if ( bTurnOff && isLocal() )
	        Verb::Ref( new VerbGadgetOff( this ) );
	}

	return energy;
}

void GadgetModifierEW::release()
{
	NounGadget::release();
	if(isServer())
		setFlags(FLAG_ACTIVE, false);

	if(m_Target.valid())
	{
		for (ModifierType modifier : modifiersType())
			m_Target->addModifier(modifier, strength());

		m_Target = NULL;
	}
}