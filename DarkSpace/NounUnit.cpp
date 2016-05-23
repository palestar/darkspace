/*
	NounUnit.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "VerbCaptureShip.h"
#include "VerbDestroyGadget.h"
#include "VerbMoveUnit.h"
#include "VerbDestroyUnit.h"
#include "VerbOrderUnit.h"
#include "VerbUnload.h"
#include "NounPlanet.h"
#include "NounShip.h"
#include "NounCargo.h"
#include "NounUnit.h"
#include "GameContext.h"
#include "resource.h"
#include "TraitCargo.h"

//----------------------------------------------------------------------------

const int UNIT_UPDATE_TIME = TICKS_PER_SECOND * 5;
const int UNIT_CAPTURE_DELAY = (TICKS_PER_SECOND * 60) / UNIT_UPDATE_TIME;

// how much XP is gained for each attack by a unit
static Constant UNIT_EXP_GAIN( "UNIT_EXP_GAIN", 0.05f );
// how much is subtracted from the roll for a ship capture based on the unit's experience level
static Constant UNIT_SHIP_CAPTURE_XP_BONUS( "UNIT_SHIP_CAPTURE_XP_BONUS", 10 );
// spacing between stacked nouns
static Constant	HEX_STACK_SPACE( "HEX_STACK_SPACE", 0.75f );		
// how fast to units move (gu/s)
static Constant UNIT_MOVE_SPEED( "UNIT_MOVE_SPEED", 5.0f );
// how much extra attack units that have a bonus to ship capture do to enemy units on ships when boarding
static Constant SHIP_CAPTURE_ATTACK_BONUS( "SHIP_CAPTURE_ATTACK_BONUS", 1.25f);

//----------------------------------------------------------------------------

int TYPE_CAPTURE_CHANCE[] =
{
	0,	// UNKNOWN,
	10,	// FIGHTER,
	10,	// ENGINEER,
	10,	// SUPPLY,
	7,	// TRANSPORT,
	10,	// SCOUT,
	7,	// FRIGATE,
	5,	// DESTROYER,
	2,	// CRUISER,
	1,	// DREAD,
	0,	// STATION,
	2,	// PLATFORM,
	2,	// PLATFORM_SUPPLY
};

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( NounUnit, NounBody ); 
REGISTER_FACTORY_KEY( NounUnit, 4221754442509816190LL );

BEGIN_ABSTRACT_PROPERTY_LIST( NounUnit, NounBody );
	CLEAR_PROPERTY_FLAGS( m_Position, PF_TRANSMIT );		// don't transmit, our position is set by our hex...
	CLEAR_PROPERTY_FLAGS( m_vOrientation, PF_TRANSMIT );	
	ADD_PROPERTY_FLAGS( m_nUserId, PF_TRANSMIT );			// retain the user who dropped this unit, so we can award them credit...
	// keep the PF_TRANSMIT flag turned on, because when a unit gets wrapped by a NounPod object, it only saves the PF_TRANSMIT properties...
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nHex );
	ADD_REPLICATED_PROPERTY( m_Damage, TICKS_PER_SECOND * 5 );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Experience );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Order );
END_PROPERTY_LIST();

NounUnit::NounUnit() : 
	m_nHex( 0xffff ), 
	m_Damage( 0 ),
	m_Experience( 0 ), 
	m_CaptureDelay( -1 ), 
	m_Order( ATTACK ), 
	m_nUpdateUnit( 0 ),
	m_bMoving( false ),
	m_vMoveTarget( Vector3::ZERO )
{
	//attachTrait( new TraitCargo() );
}

//----------------------------------------------------------------------------

void NounUnit::onAttached()
{
	NounBody::onAttached();

	// initialize our update timer
	m_nUpdateUnit = tick() + seed( UNIT_UPDATE_TIME );
	
	// notify friendly ship owner they have been boarded
	if ( WidgetCast<NounShip>( parent() ) && isEnemy( (NounShip *)parent() ) && shipCaptureBonus() )
		((NounShip *)parent())->message( CharString( "<color;0000ff>Security: Enemy raiding party attaching to our ship; prepare to repel boarders!</color>" ) );
	if ( WidgetCast<NounShip>( parent() ) && isEnemy( (NounShip *)parent() ) )
		((NounShip *)parent())->message( CharString( "<color;0000ff>Security: Enemy units have boarded!</color>" ) );
	
	// update our actual position in 3D space ...
	setHex( m_nHex, false );
	// update our user ID
	updateUserId();
}

void NounUnit::preRender( RenderContext &context, 
						const Matrix33 & frame, 
						const Vector3 & position )
{
	// don't render within the game
}

//----------------------------------------------------------------------------

NounType NounUnit::nounType() const
{
	return TYPE_LOCAL_OBJECT;
}

bool NounUnit::enableOrbit() const
{
	return false;
}

float NounUnit::baseSignature() const
{
	return 1.0f;
}

bool NounUnit::isCargo() const
{
	return true;
}

bool NounUnit::canDamage( dword type ) const
{
	return (type & (DAMAGE_BIO|DAMAGE_KINETIC|DAMAGE_ENERGY)) != 0;
}

void NounUnit::initialize()
{
	NounBody::initialize();
	setHex( m_nHex, false );
	updateUserId();
}

void NounUnit::release()
{
	NounBody::release();
	m_pTarget = NULL;
	m_pHome = NULL;
}

void NounUnit::simulate( dword nTick )
{
	if (! isSleeping() )
	{
		if ( m_bMoving )
		{
			Vector3 vDelta( m_vMoveTarget - position() );
			float fDistance = vDelta.magnitude();

			if ( fDistance > EPSILON )
			{
				// keep delta under our maximum speed
				if ( fDistance > UNIT_MOVE_SPEED )
					vDelta *= UNIT_MOVE_SPEED / fDistance;
				// move unit
				setPosition( position() + (vDelta * TICK_DURATION_S) );
			}
			else
			{
				// arrived at destination..
				setPosition( m_vMoveTarget );
				m_bMoving = false;
			}
		}

		if ( nTick >= m_nUpdateUnit )
		{
			// increment the next time we will update...
			m_nUpdateUnit = nTick + UNIT_UPDATE_TIME;
			// heal unit if damaged
			if ( damage() > 0 && isFriend( WidgetCast<Noun>( parent() ) ) )
				setDamage( damage() - healAmount() );

			// call the combat timer on client and server (bit of duplication going on here
			// but we save on replicating OOC timer data
			if ( WidgetCast<NounShip>( parent() ) != NULL )
			{
				NounShip * pShip = (NounShip *)parent();
				if ( isEnemy( pShip ) )
					pShip->setOutOfCombat();
			}

			// all unit logic occurs server side only
			if ( isServer() )
			{
				updateLeader();

				if ( WidgetCast<NounShip>( parent() ) != NULL )
				{
					NounShip * pShip = (NounShip *)parent();
					ASSERT( pShip );

					// make sure the home is set for this unit
					bool isShipFriend = isFriend( pShip );
					if ( isShipFriend )
						m_pHome = pShip;
					
					// attack current target
					if ( validateTarget( m_pTarget ) && isEnemy( m_pTarget ) )
						attackTarget( m_pTarget );
					else
						m_pTarget = findUnitTarget();

					// if we have no defending unit and we are onboard an enemy ship start the capture process..
					if (! m_pTarget.valid() && !isShipFriend )
					{
						if ( m_CaptureDelay > 0 )
						{
							m_CaptureDelay--;
							if ( m_CaptureDelay == 0 )
							{
								// If ship is a monster or admin, unit is just destroyed!
								if ( !pShip->isMonster() && (pShip->flags() & NounShip::FLAG_ADMIN) == 0 )
								{
									// ship is NOT a monster or admin
									
									int nCaptureRoll = (rand() % 100) - (m_Experience * UNIT_SHIP_CAPTURE_XP_BONUS);
									int nSecondChance = 0;

									if( shipCaptureBonus() )
									{
										//infantry gets a second roll and the lower win is used
										nSecondChance = (rand() % 100) - (m_Experience * UNIT_SHIP_CAPTURE_XP_BONUS);
										nCaptureRoll = nSecondChance < nCaptureRoll ? nSecondChance : nCaptureRoll;
									}
									// adjust capture roll by any capture defense..
									nCaptureRoll += 100 * (pShip->calculateModifier( MT_CAPTURE_DEFENSE) - 1.0f);

									if (nCaptureRoll <= TYPE_CAPTURE_CHANCE[ pShip->type() ] )
									{
										// ship has been captured
										gameContext()->gameUser()->onCaptureShip( this, pShip );
										// set the teamId now so multiple units don't generate multiple verbs
										pShip->setTeamId( teamId() );
										pShip->setUserId( 0 );	

										Verb::Ref( new VerbCaptureShip( pShip, teamId(), this ) );	
										m_CaptureDelay = -1;
									}
									else
									{
										if( shipCaptureBonus() )
										{
											//Destroy an extra gadget
											NounGadget * pDestroy = findRandomGadget( pShip );
											if ( pDestroy != NULL && !pDestroy->destroyed() )
												Verb::Ref( new VerbDestroyGadget( pDestroy, this ) );
										}
							
										// ship not captured, destroy a device then
										NounGadget * pDestroy = findRandomGadget( pShip );
										if ( pDestroy != NULL && !pDestroy->destroyed() )
											Verb::Ref( new VerbDestroyGadget( pDestroy, this ) );

									}
								}

								setDetach();
							}
						}
						else 
						{
							// start the capture timer..
							m_CaptureDelay = UNIT_CAPTURE_DELAY;
						}
					}
					else
						m_CaptureDelay = -1;		// reset the capture counter
				}
				else if ( WidgetCast<NounPlanet>( parent() ) != NULL )
				{
					NounPlanet * pPlanet = (NounPlanet *)parent();
					ASSERT( pPlanet );

					switch( m_Order )
					{
					case NOORDER:
					case ATTACK:	// attack enemy units
						{
							// check our current target
							if ( validateTarget( m_pTarget ) && !isFriend( m_pTarget ) )
							{
								// move towards target
								if ( moveToTarget( m_pTarget, attackRange() ) <= attackRange() )
									attackTarget( m_pTarget );	// attack target when in range
							}
							else
							{
								// look for new target
								m_pTarget = findUnitTarget();
							}
						}
						break;
					case RAZE:		// attack enemy structures
						{
							if ( validateTarget( m_pTarget ) && !isFriend( pPlanet ) )
							{
								if ( moveToTarget( m_pTarget, attackRange() ) <= attackRange() )
									attackTarget( m_pTarget );
							}
							else
							{
								// look for new target
								if(pPlanet->structureCount() > 0 && !isFriend( pPlanet ) )	
									m_pTarget = findStructureTarget();
								else
									setOrder(ATTACK);
							}
						}
						break;
					case HOLD:		// move to target position, attack any enemy units within range
						{
							moveToTarget( m_pTarget, 0 );

							// check for a unit target
							NounUnit * pUnitTarget = findUnitTarget();
							if ( pUnitTarget != NULL && moveToTarget( pUnitTarget, INT_MAX ) <= attackRange() )
								attackTarget( pUnitTarget );
						}
						break;
					case MOVE:		// move to target position 
						{
							if ( moveToTarget( m_pTarget, 0 ) <= 0 )
								setOrder( ATTACK );		// go back to attack order once at target
						}
						break;
					}
				}
				else
				{
					// unit not attached to planet or ship, die in vacuum of space!
					setDetach();
				}
			}	// end if server()
		}
	}

	NounBody::simulate( nTick );
}

void NounUnit::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	ASSERT( context() != NULL );

	// don't allow more damage than possible
	int possible = maxDamage() - m_Damage;
	if ( damage > possible )
		damage = possible;

	m_Damage = Clamp( m_Damage + damage, 0, maxDamage() );

	if ( pFrom != NULL )
	{
		float ratio = float( damage ) / maxDamage();
		if ( isFriend( pFrom ) )
			gameContext()->gameUser()->onFriendlyFire( pFrom, ratio );
		else
			gameContext()->gameUser()->onPlanetsDamaged( pFrom, ratio );
	}

	// check for unit destroyed
	if ( isServer() && m_Damage >= maxDamage() )
	{
		// destroy the unit
		Verb::Ref( new VerbDestroyUnit( this, pFrom ) );
		// detach from the world
		setDetach();
	}
}

//----------------------------------------------------------------------------

CharString NounUnit::status() const
{
	CharString sTargetInfo( NounBody::status() );
	sTargetInfo += CharString().format("%d%%/%s/%d\n", int( damageRatioInv() * 100), experienceText( m_Experience ), attackDamage() );
	sTargetInfo += CharString().format("Fleet: %s\n", teamName());
	sTargetInfo += CharString().format("Order: %s %s\n", NounUnit::orderText( order() ), m_pTarget.valid() ? m_pTarget->name() : "" );

	return sTargetInfo;
}

Material * NounUnit::icon() const
{
	return WidgetCast<Material>( nounContext()->resource( "ICON" ) );
}

//----------------------------------------------------------------------------

const char * NounUnit::orderText( Order order )
{
	static const char * ORDER_TEXT[] =
	{
		"NONE",
		"ATTACK",
		"RAZE",
		"HOLD",
		"MOVE"
	};
	if ( order >= 0 && order < sizeof(ORDER_TEXT)/sizeof(ORDER_TEXT[0]) )
		return ORDER_TEXT[ order ];
	return "?";
}

const char * NounUnit::experienceText( float experience )
{
	if ( experience > 0.9f )
		return "Legendary";
	if ( experience > 0.8f )
		return "Heroic";
	if ( experience > 0.7f )
		return "Elite";
	if ( experience > 0.6f )
		return "Hardened";
	if ( experience > 0.5f )
		return "Experienced";
	if ( experience > 0.4f )
		return "Skilled";
	if ( experience > 0.3f )
		return "Trained";
	if ( experience > 0.2f )
		return "Rookie";
	if ( experience > 0.1f )
		return "Recruit";
	return "Conscript";
	
}

bool NounUnit::updateHexPositions( NounPlanet * pPlanet, u16 nHex, bool bMove /*= false*/ )
{
	if (! pPlanet )
		return false;
	if (! pPlanet->isHexValid( nHex ) )
		return false;		// don't crash at the very least!

	Vector3 vPosition = pPlanet->hex( nHex ).position;
	Vector3 vOffset = vPosition;
	vOffset.normalize();

	const float STACK_SPACE = HEX_STACK_SPACE;

	// note, we update the position of ALL NounUnit objects sharing the same hex...
	float fOffset = STACK_SPACE;
	for(int i=0;i<pPlanet->childCount();++i)
	{
		NounUnit * pSibling = WidgetCast<NounUnit>( pPlanet->child(i) );
		if (! pSibling || pSibling->hex() != nHex )
			continue;		
		
		if ( bMove )
		{
			pSibling->m_bMoving = true;
			pSibling->m_vMoveTarget = vPosition + (vOffset * fOffset );
		}
		else
		{
			pSibling->setPosition( vPosition + (vOffset * fOffset) );
		}
		fOffset += STACK_SPACE;
	}

	return true;
}

//----------------------------------------------------------------------------

void NounUnit::disband( Noun * pWho )
{
	if ( isServer() )
	{
		setDetach();
	}
}

void NounUnit::destroy( Noun * pKiller )
{
	setDetach();
}

void NounUnit::setHex( u16 nHex, bool bMove /*= false*/ )
{
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( parent() );
	if ( pPlanet != NULL )
	{
		u16 nLastHex = m_nHex;
		m_nHex = nHex;

		if ( pPlanet != NULL && !pPlanet->isHexValid( m_nHex ) )
			m_nHex = pPlanet->findHex( position(), false );

		if ( nLastHex != m_nHex )
			updateHexPositions( pPlanet, nLastHex, bMove );
		updateHexPositions( pPlanet, m_nHex, bMove );
	}
	else
	{
		m_nHex = 0xffff;
		setPosition( Vector3::ZERO );
	}
}

void NounUnit::updatePosition()
{
	setHex( m_nHex, false );
}

void NounUnit::setDamage( int damage )
{
	m_Damage = Clamp( damage, 0, maxDamage() );
}

void NounUnit::setExperience( float exp )
{
	m_Experience = Clamp( exp, 0.0f, 1.0f );
}

void NounUnit::setLeader( NounUnit * pLeader )
{
	m_pLeader = pLeader;
}

void NounUnit::setOrder( NounUnit::Order order )
{
	m_Order = order;
}

void NounUnit::setTarget( Noun * pTarget )
{
	m_pTarget = pTarget;
}

void NounUnit::setHome( Noun * pHome )
{
	m_pHome = pHome;
}

//---------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------

void NounUnit::updateUserId()
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if ( pShip && isFriend( pShip ) )
		setUserId( pShip->userId() );
}

void NounUnit::updateLeader()
{
	if ( !validateTarget( m_pLeader ) || m_pLeader->order() != order() || m_pLeader->key() > key() )
	{
		// find a new leader
		m_pLeader = NULL;

		Noun * pParent = parentNoun();
		if ( pParent != NULL )
		{
			NounUnit * pLeader = NULL;
			for(int i=0;i<pParent->childCount();i++)
			{
				Noun * pChild = WidgetCast<Noun>( pParent->child(i) );
				if ( pChild != this && WidgetCast<NounUnit>( pChild ) != NULL && isFriend( pChild ) )
				{
					NounUnit * pUnit = (NounUnit *)pChild;

					if ( pUnit->order() != order() )
						continue;		// skip, doesn't have the same order as this unit..
					if ( WidgetCast<NounPlanet>( pParent ) && order() == HOLD && pUnit->target() != target() )
						continue;		// if holding a position, leader must have the same target as well...
					if ( pUnit->key() < key() )
						pLeader = pUnit;
				}
			}

			m_pLeader = pLeader;
		}
	}
}

bool NounUnit::validateTarget( Noun * pTarget )
{
	if ( pTarget == NULL )
		return false;

	if ( WidgetCast<NounDropTarget>( pTarget ) )
	{
		NounDropTarget * pDrop = (NounDropTarget *)pTarget;
		if ( pDrop->planet() != parent() )
			return false;			// not on the same planet
	}
	else
	{
		// structures / units
		if ( pTarget->zone() == NULL )
			return false;			// object no longer attached to the universe
		if ( pTarget->parent() != parent() )
			return false;			// not on the same planet/ship or been destroyed
		if ( WidgetCast<NounGadget>( pTarget ) )
			if ( ((NounGadget *)pTarget)->destroyed() )
				return false;		// gadget is already destroyed
	}

	return true;
}

int NounUnit::moveToTarget( Noun * pTarget, int nWithin )
{
	if (! isServer() )
		return INT_MAX;		// server-side function only!
	if (! pTarget )
		return INT_MAX;		// no target pointer provided
	NounPlanet * pPlanet = WidgetCast<NounPlanet>( parent() );
	if ( pPlanet == NULL )
		return 0;			// on ship or other parent, always in range on ships

	int nStart = hex();
	if ( nStart < 0 )
		return INT_MAX;		// we are not on the planet

	int nEnd = -1;
	if ( WidgetCast<NounUnit>( pTarget ) )
		nEnd = ((NounUnit *)pTarget)->hex();
	else if ( WidgetCast<NounStructure>( pTarget ) )
		nEnd = ((NounStructure *)pTarget)->hex();
	else if ( WidgetCast<NounDropTarget>( pTarget ) )
		nEnd = ((NounDropTarget *)pTarget)->hex();

	if ( nEnd < 0 )
		return INT_MAX;		// target not on planet
	if ( nStart == nEnd )
		return 0;			// already at correct position

	// find a path to the target
	NounPlanet::Path * pPath = pPlanet->pathMap()->findPath( nStart, nEnd );
	// get the current distance to the target
	int nDistance = pPath->pathCount();
	// if the distance is outside our attack distance, then move to the next hex
	if ( nDistance > nWithin && nDistance > 0 )
	{
		// move the unit
		Verb::Ref( new VerbMoveUnit( this, pPath->path( 0 ) ) );
	}

	return nDistance;
}

void NounUnit::attackTarget( Noun * pTarget )
{
	// calculate damage
	int damage = RandomFloat( 0.25f, 1.0f) * attackDamage();
	// add bonus damage based on experience
	damage += experience() * attackDamage();
	if( WidgetCast<NounShip>( parent() ) != NULL && shipCaptureBonus() )
	{
		//This unit is on a ship and has bonus to shipcapturing!
		damage *= SHIP_CAPTURE_ATTACK_BONUS;
	}
	// in range, attack the target
	NounGame * pDamage = WidgetCast<NounGame>( pTarget );
	if ( pDamage != NULL )
		pDamage->inflictDamage( tick(), this, damage, damageType(), Vector3( 0.0f ) );
	// give this unit experience
	addExperience( UNIT_EXP_GAIN );
}

NounUnit * NounUnit::findUnitTarget()
{
	if ( validateTarget( m_pLeader ) )
		return WidgetCast<NounUnit>( m_pLeader->target() );			// if we have a leader, return our leaders target...

	Noun * pParent = parentNoun();
	if ( pParent == NULL )
		return NULL;

	NounUnit * pTarget = NULL;
	float targetRange = 5000.0f;

	for(int i=0;i<pParent->childCount();i++)
	{
		Noun * pChild = (Noun *)pParent->child(i);
		if ( WidgetCast<NounUnit>( pChild ) && !isFriend( pChild ) && canDetect( pChild, sensor(), view()) )
		{
			float range = (pChild->position() - position()).magnitude();
			if ( range < targetRange )
			{
				pTarget = (NounUnit *)pChild;
				targetRange = range;
			}
		}
	}

	return pTarget;
}

NounStructure * NounUnit::findStructureTarget()
{
	if ( validateTarget( m_pLeader ) )
		return WidgetCast<NounStructure>( m_pLeader->target() );			// if we have a leader, return our leaders target...

	Noun * pParent = parentNoun();
	if ( pParent == NULL )
		return NULL;

	NounStructure * pTarget = NULL;
	float targetRange = 5000.0f;

	for(int i=0;i<pParent->childCount();i++)
	{
		Noun * pChild = (Noun *)pParent->child(i);
		if ( WidgetCast<NounStructure>( pChild ) && !isFriend( pChild ) && canDetect( pChild, sensor(), view()) )
		{
			float range = (pChild->position() - position()).magnitude();
			if ( range < targetRange )
			{
				pTarget = (NounStructure *)pChild;
				targetRange = range;
			}
		}
	}

	return pTarget;
}

NounGadget * NounUnit::findRandomGadget( NounShip * pShip )
{
	Array< NounGadget * > gadgets;
	for(int i=0;i<pShip->childCount();i++)
		if ( WidgetCast<NounGadget>( pShip->child(i) ) )
			gadgets.push( (NounGadget *)pShip->child(i) );

	return gadgets.size() > 0 ? gadgets[ rand() % gadgets.size() ] : NULL;
}

//----------------------------------------------------------------------------
//EOF
