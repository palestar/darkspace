/*
	DefaultLootTables.h

	List of all default loot tables used for ships that have no specific drops
	(c)2008 Palestar Inc, Jeff Everett
*/

#ifndef DEFAULTLOOTTABLES_H
#define DEFAULTLOOTTABLES_H

class DefaultLootTables
{
public:
	//! Types
	struct LootInfo : public Referenced
	{
		typedef Reference< LootInfo >		Ref;

		LootInfo(ClassKey a_key, int a_chance) : m_classKey(a_key), m_nChance(a_chance) 
		{}

		ClassKey	m_classKey;
		int			m_nChance;
	};
	typedef std::vector< LootInfo::Ref >		LootVector;
	typedef std::map< NounShip::Type, LootVector >	LootMap;

	static void					initialize();
	static LootMap &			getLootMap();
	static const ClassKey &		selectLoot(NounShip * a_pShip);

protected:
	static LootInfo *			selectItem(LootVector & a_list );
};

#endif
