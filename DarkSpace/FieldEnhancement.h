/**
	@file FieldEnhancement.h
	@brief This field adds ehancement modifiers to all ships in range and removes those enhancements if the ships
	leave this field. 

	(c)2009 Palestar Inc
	@author Richard Lyle @date 6/23/2011 8:00:07 PM
*/

#ifndef FIELDENHANCEMENT_H
#define FIELDENHANCEMENT_H

#include "NounField.h"
#include "EnhancementModifier.h"
#include "GameDll.h"

//---------------------------------------------------------------------------------------------------

class DLL FieldEnhancement : public NounField
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef Reference< FieldEnhancement >			Ref;
	typedef WidgetReference< FieldEnhancement >		wRef;

	// Construction
	FieldEnhancement();

	// Noun interface
	void		initialize();
	void		simulate( dword nTick );							// update noun forward in time by one tick
	// NounField interface
	void		onEnterField( Noun * pNoun );
	void		onInsideField( Noun * pNoun );
	void		onLeaveField( Noun * pNoun );

	//! Accessors
	bool		isFriendsOnly() const;								// if true, then modifiers are applied to friendly ships only (default), if false then applied to enemy ships only
	int			modifierCount() const;
	EnhancementModifier * 
				modifier(int n) const;

	//! Mutators
	void		setFriendsOnly( bool a_bFriendsOnly );
	void		addModifier( EnhancementModifier * a_pModifier );
	void		removeModifier( EnhancementModifier * a_pModifier );
	void		clearModifiers();

protected:
	//! Data
	bool		m_bFriendsOnly;
	Array< EnhancementModifier::Ref >
				m_Modifiers;
};

//---------------------------------------------------------------------------------------------------

inline bool FieldEnhancement::isFriendsOnly() const
{
	return m_bFriendsOnly;
}

inline int FieldEnhancement::modifierCount() const
{
	return m_Modifiers.size();
}

inline EnhancementModifier * FieldEnhancement::modifier(int n) const
{
	return m_Modifiers[ n ];
}

//---------------------------------------------------------------------------------------------------

#endif

//---------------------------------------------------------------------------------------------------
//EOF
