/**
	@file ShipFighter.h
	@brief This is a specialized type of ship that is used to represent a wing of fighters. The number
	of fighters rendered by this object is an indication of the damage level of this ship. 

	* This ship can't be repaired.
	* This objects uses the NounFighter class as a sub-object to render the individual fighters.

	(c)2009 Palestar Inc
	@author Richard Lyle @date 7/27/2010 9:39:34 PM
*/

#ifndef SHIPFIGHTER_H
#define SHIPFIGHTER_H

//---------------------------------------------------------------------------------------------------

class ShipFighter : public NounShip
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< ShipFighter >					Ref;
	typedef WidgetReference< ShipFighter >				wRef;

	enum Formation {
		F_OFFENSIVE,
		F_DEFENSIVE,
		F_BALANCED
	};

	//! Accessors
	Formation			formation() const;					// returns the current formation of the fighters
	int					fighterCount() const;				// returns the number of fighters based on damage
	Noun *				fighter( int n ) const;				// returns a pointer to the fighter object
	int					maxFighterCount() const;			// returns the maximum number of fighters

	//! Mutators
	void				setFormation( Formation a_eFormation );
	void				setMaxFighterCount( int a_nMax );

protected:
	//! Data
	u8					m_eFormation;
	u8					m_nMaxFighters;

	//! non-serialized
	Array< Noun::Ref >	m_Fighters;
};

#endif

//---------------------------------------------------------------------------------------------------
//EOF
