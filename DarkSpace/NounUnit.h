/*
	NounUnit.h

	This is the base class for a ground unit
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNUNIT_H
#define NOUNUNIT_H

#include "Network/NetworkTypes.h"

#include "NounPlanet.h"
#include "NounBody.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class NounShip;
class NounGadget;

class DLL NounUnit : public NounBody
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// Types
	typedef WidgetReference< NounUnit >		wRef;
	
	enum Order
	{
		NOORDER,	// no current order

		ATTACK,		// attack other enemy units (default order)
		RAZE,		// attack enemy structures
		HOLD,		// move to target position and hold
		MOVE,		// move to target position then enter attack mode
	};

	// Construction
	NounUnit();

	// BaseNode interface
	void				onAttached();
	void				preRender( RenderContext &context, 
							const Matrix33 & frame, 
							const Vector3 & position );
	// Noun interface
	NounType			nounType() const;
	bool				enableOrbit() const;
	float				baseSignature() const;
	bool				isCargo() const;
	bool				canDamage( dword type ) const;

	void				initialize();
	void				release();
	void				simulate( dword nTick );

	void				inflictDamage( dword nWhen, Noun * pFrom, 
							int damage, dword type, 
							const Vector3 & direction );

	// NounGame interface
	CharString			status() const;

	// Accessors
	virtual int			maxDamage() const = 0;			// maximum unit damage
	virtual int			attackRange() const = 0;		// attack range in hexes
	virtual int			attackDamage() const = 0;		// 
	virtual dword		damageType() const = 0;			// what kind of damage does this unit inflict
	virtual int			healAmount() const = 0;			// how many units does the unit heal by
	virtual float		sensor() const = 0;				// unit sensor rating
	virtual float		view() const = 0;				// unit view rating
	virtual int			buildTechnology() const = 0;	// technology on the planet required to build this structure
	virtual int			buildTime() const = 0;			// how many seconds does it take to build this unit
	virtual int			buildCost() const = 0;			// cost in resources to build unit
	virtual bool		shipCaptureBonus() const = 0;	// does this unit get a bonus to ship capturing

	s16					hex() const;
	int					damage() const;
	float				experience() const;
	NounUnit *			leader() const;					// our leader unit
	Order				order() const;					// get the current unit order
	Noun *				target() const;					// current unit target
	Noun *				home() const;

	// Mutators
	void				disband( Noun * pWho );			// called by VerbDisband()
	void				destroy( Noun * pKiller );		// called by VerbDestroyUnit()

	void				setHex( u16 nHex,				// hex number to move unit onto..
							bool bMove = false );		// if true, then unit will interpolate to new hex
	void				updatePosition();
	void				setDamage( int damage );
	void				setExperience( float exp );
	void				setLeader( NounUnit * pLeader );
	void				setOrder( Order order );		// set the current unit order
	void				setTarget( Noun * pTarget );	// set unit target
	void				setHome( Noun * pHome );


	// Helpers
	float				damageRatio() const;
	float				damageRatioInv() const;
	Material *			icon() const;					// get unit icon
	
	void				addExperience( float amount );

	// static
	static const char *
						experienceText( float experience );
	static const char *	
						orderText( Order order );

	// update the positions of all units on the given planet and hex
	static bool			updateHexPositions( NounPlanet * pPlanet, 
							u16 nHex, bool bMove = false );		// updates all units in the given hex

protected:
	// Data
	u16					m_nHex;					// our current planet hex
	u16					m_Damage;				// current unit strength
	bf8					m_Experience;			// current unit experience, 0.0 - 1.0
	u8					m_Order;				// unit standing order

	// non-serialized
	Noun::wRef			m_pTarget;				// unit target
	Noun::wRef			m_pHome;				// who is our home
	NounUnit::wRef		m_pLeader;				// unit leader
	dword				m_nUpdateUnit;
	int					m_CaptureDelay;

	bool				m_bMoving;				// true if unit it moving to it's new hex
	Vector3				m_vMoveTarget;			// target position

	// Mutators
	void				updateUserId();
	void				updateLeader();
	bool				validateTarget( Noun * pTarget );
	int					moveToTarget( Noun * pTarget, int within );
	void				attackTarget( Noun * pTarget );
	NounUnit *			findUnitTarget();
	NounStructure *		findStructureTarget();
	NounGadget *		findRandomGadget( NounShip * pShip );
};

IMPLEMENT_RAW_STREAMING( NounUnit::Order );

//----------------------------------------------------------------------------

inline s16 NounUnit::hex() const
{
	return m_nHex;
}

inline int NounUnit::damage() const
{
	return m_Damage;
}

inline float NounUnit::experience() const
{
	return m_Experience;
}

inline NounUnit * NounUnit::leader() const
{
	return m_pLeader;
}

inline NounUnit::Order NounUnit::order() const
{
	return (NounUnit::Order)m_Order;
}

inline Noun * NounUnit::target() const
{
	return m_pTarget;
}

inline Noun * NounUnit::home() const
{
	return m_pHome;
}

//----------------------------------------------------------------------------

inline float NounUnit::damageRatio() const
{
	return float( damage() ) / maxDamage();
}

inline float NounUnit::damageRatioInv() const
{
	return 1.0f - damageRatio();
}

//----------------------------------------------------------------------------

inline void NounUnit::addExperience( float amount )
{
	setExperience( experience() + amount );
}

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
//EOF
