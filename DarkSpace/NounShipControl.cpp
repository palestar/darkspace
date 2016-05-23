/*
	NounShipControl.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Debug/Trace.h"
#include "Debug/Profile.h"
#include "Math/Helpers.h"
#include "System/Messages.h"
#include "World/NounZoneTarget.h"
#include "VerbBreakOrbit.h"
#include "VerbEnterOrbit.h"
#include "GadgetBeamWeapon.h"
#include "GadgetShield.h"
#include "NounStructure.h"
#include "NounNebula.h"
#include "NounShip.h"
#include "NounJumpGate.h"
#include "GameContext.h"
#include "resource.h"

#include <float.h>
#include <math.h>

//----------------------------------------------------------------------------

static Constant			USE_JUMP_DISTANCE( "USE_JUMP_DISTANCE", 2000.0f );					// when moving to a target, at what range do we use the jump drives
static Constant			CHECK_JUMP_INTERVAL( "CHECK_JUMP_INTERVAL", 10.0f );				// how often to check for a jump collision
static Constant			Y_DECAY_PERCENT( "Y_DECAY_PERCENT", 0.02f );						// how fastt to decay the Y to zero

static const float		MAX_SHIP_ROLL = PI / 4;												// maximum roll when turning
static const float		MIN_SHIP_ROLL = -MAX_SHIP_ROLL;
const int				SHIP_SYNC_RATE = TICKS_PER_SECOND * 30;								// how often is a VerbSyncShip() sent 
const int				SHIP_UNLOCK_RATE = TICKS_PER_SECOND * 5;							// how often to call unlockDynamics() 

//----------------------------------------------------------------------------

// define to non-zero to enable ship pitching when changing Z
#define ENABLE_SHIP_PITCH		0

//----------------------------------------------------------------------------

inline bool HeadingEpsilon( float v1, float v2 )
{
	return fabs( v1 - v2 ) >= (PI / 360);
}

inline bool VelocityEpsilon( float v1, float v2 )
{
	return fabs( v1 - v2 ) >= 0.05f;
}

//----------------------------------------------------------------------------

bool NounShip::controlHeadingVelocity( float fHeading, float fVelocity )
{
	if ( flags() & FLAG_CONTROL_DISABLED )
		return false;		// ship control is disabled
	// now set the new heading and velocity
	m_fLastSetHeading = m_fSetHeading = NormalizeAngle( fHeading );
	m_fLastSetVelocity = m_fSetVelocity = fVelocity;
	m_fLastYaw = m_fYaw = 0.0f;

	return true;
}

bool NounShip::controlYawVelocity( float fYaw, float fVelocity )
{
	if ( flags() & FLAG_CONTROL_DISABLED )
		return false;		// ship control is disabled
	// now set the new heading and velocity
	m_fLastSetHeading = m_fSetHeading = m_fHeading;
	m_fLastSetVelocity = m_fSetVelocity = Clamp( fVelocity, 0.0f, maxVelocity());
	m_fLastYaw = m_fYaw = Clamp( fYaw, -1.0f, 1.0f );

	return true;
}

bool NounShip::controlVelocity( float fVelocity )
{
	if ( flags() & FLAG_CONTROL_DISABLED )
		return false;		// ship control is disabled
	// now set the new velocity
	m_fLastSetVelocity = m_fSetVelocity = Clamp( fVelocity, 0.0f, maxVelocity());

	return true;
}

bool NounShip::controlHeading( float fHeading )
{
	if ( flags() & FLAG_CONTROL_DISABLED )
		return false;		// ship control is disabled

	// now set the new heading
	m_fLastSetHeading = m_fSetHeading = NormalizeAngle( fHeading );
	m_fLastYaw = m_fYaw = 0.0f;

	return true;
}

bool NounShip::controlYaw( float fYaw )
{
	if ( flags() & FLAG_CONTROL_DISABLED )
		return false;		// ship control is disabled

	// now set the new yaw
	m_fLastYaw = m_fYaw = Clamp( fYaw, -1.0f, 1.0f);
	m_fLastSetHeading = m_fSetHeading = m_fHeading;

	return true;
}

//----------------------------------------------------------------------------

bool NounShip::canAlpha( Noun * pTarget )
{
	for(int i=0;i<childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
		if ( pGadget != NULL && pGadget->hotkey() == HK_SPACE && pGadget->usable( pTarget, false ) )
			return true;
	}
	return false;
}

// use all weapons linked to the spacebar
bool NounShip::verbAlpha( Noun * pTarget )
{
	bool bWeaponUsed = false;
	for(int i=0;i<childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
		if ( pGadget != NULL && pGadget->hotkey() == HK_SPACE )
		{
			useGadget( pGadget, pTarget, false );
			bWeaponUsed = true;
		}
	}

	return bWeaponUsed;
}

bool NounShip::canBomb( Noun * pTarget )
{
	for(int i=0;i<childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
		if ( pGadget != NULL && pGadget->type() == NounGadget::WEAPON_BOMB && pGadget->usable( pTarget, false ) )
			return true;
	}
	return false;
}

bool NounShip::verbBomb( Noun * pTarget )
{
	bool bWeaponUsed = false;
	for(int i=0;i<childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( child(i) );
		if ( pGadget != NULL && pGadget->type() == NounGadget::WEAPON_BOMB )
		{
			useGadget( pGadget, pTarget, false );
			bWeaponUsed = true;
		}
	}

	return bWeaponUsed;
}

void NounShip::useGadget( NounGadget * pGadget, Noun * pTarget, bool shift )
{
	// check that the ship is attached to the context, and that the gadget belongs to this ship
	if ( pGadget != NULL && context() != NULL && pGadget->parent() == this )		
	{
		if ( m_UseGadgets.size() > 0 && (pTarget != m_UseTarget || shift != m_UseShift) )
			useGadgets();	// flush the current use command now

		m_UseGadgets.push( pGadget );
		m_UseTarget = pTarget;
		m_UseShift = shift;
	}
}

bool NounShip::togglePointDefense()
{
	if (! isLocal() )
		return false;

	// determine the state of the beam weapons
	bool bState = false;
	
	Array< GadgetBeamWeapon * > weapons;
	for(int i=0;i<m_Gadgets.size();i++)
	{
		GadgetBeamWeapon * pBeam = WidgetCast<GadgetBeamWeapon>( m_Gadgets[i] );
		if ( pBeam != NULL && pBeam->allowPointDefense() )
		{
			bState |= pBeam->pointDefense();
			weapons.push( pBeam );
		}
	}

	for(int j=0;j<weapons.size();j++)
	{
		GadgetBeamWeapon * pBeam = weapons[j];
		if ( bState )
		{
			if ( pBeam->pointDefense() )
				useGadget( pBeam, NULL, true );		
		}
		else
		{
			if ( !pBeam->pointDefense() )
				useGadget( pBeam, NULL, true );
		}
	}

	return !bState;
}

int NounShip::toggleShields()
{
	if (! isLocal() )
		return false;
	bool bState = false;

	for(int i=0;i<m_Shields.size();i++)
		bState |= m_Shields[i]->active();

	for(int j=0;j<m_Shields.size();j++)
	{
		GadgetShield * pShield = m_Shields[j];
		if ( bState && pShield->active() )
			useGadget( pShield, NULL, true );
		else if ( !bState && !pShield->active() )
			useGadget( pShield, NULL, true );
	}

	return m_Shields[0]->useMode();
}

//----------------------------------------------------------------------------

const int MAX_MOVE_DEPTH = 5;

NounShip::MoveResult NounShip::moveToTarget( Noun * a_pTarget, float a_fOutside, float a_fWithin, float a_fSpeed, 
											bool a_bSlowToTarget, bool a_bForceJump, int a_nDepth /*= 0*/ )
{
	if (! isLocal() )
		return MOVE_ERROR;	
	if (! inLockedZone() )
		return MOVE_ERROR;		// ship is not in a locked zone
	if ( flags() & FLAG_CONTROL_DISABLED )
		return MOVE_ERROR;		// ship control is disabled

	Vector3 vMyPosition( worldPosition() );
	Vector3 vTargetPosition( a_pTarget->worldPosition() );

	// check if our target is a friendly ship, if so jump to their same jump target if they are jumping..
	if ( WidgetCast<NounShip>( a_pTarget ) && isFriend( a_pTarget ) )
	{
		// if our friendly ship is jumping, then we want to move to their same jump destionation..
		GadgetJumpDrive * pMyDrive = jumpDrive();
		GadgetJumpDrive * pFollowDrive = ((NounShip *)a_pTarget)->jumpDrive();
		if ( pMyDrive != NULL && pFollowDrive != NULL && (pFollowDrive->engaged() || pFollowDrive->jumping()) )
		{
			Noun * pJumpTarget = pFollowDrive->jumpTarget();
			if ( pJumpTarget != NULL )
			{
				vTargetPosition = pJumpTarget->worldPosition();
				a_pTarget = pJumpTarget;
			}
		}
	}

	// firstly, check for collisions with planets/projectiles.. if this ship is taking evasive action this will return true 
	// and override any current move targets..
	if ( avoidCollision() )
		return MOVE_MOVING;

	Vector3 vDelta( vTargetPosition - vMyPosition );
	float fDistance = vDelta.magnitudeXZ() - a_fOutside;

	// check the distance to the target, if we are within range then our move is done..
	if ( fDistance < a_fWithin )
		return MOVE_DONE;

	bool bNoPath = false;
	if ( a_nDepth < MAX_MOVE_DEPTH ) 
	{
		NounJumpGate * pGate = findJumpGate( a_pTarget );
		if ( pGate != NULL )
		{
			// we found a gate that will get us to our destination faster, so head towards the gate...
			return moveToTarget( pGate, 0.0f, 0.0f, 1.0f, false, false, a_nDepth + 1 );
		}

		GadgetJumpDrive * pJumpDrive = jumpDrive();
		if ( (a_bForceJump || fDistance > USE_JUMP_DISTANCE) && pJumpDrive != NULL )
		{
			if ( pJumpDrive->jumping() || pJumpDrive->engaged() )
				return MOVE_MOVING;

			if ( pJumpDrive->usable( NULL, true ) )
			{
				if ( fDistance < pJumpDrive->maxRange() )
				{
					float r2 = radius() * 2.0f;

					CollisionInfo info;
					if ( checkForCollision( a_pTarget, &info ) )
					{
						Vector3 vAvoid;
						calculateAvoidVector( info.m_pCollide, vAvoid );

						NounZoneTarget::Ref pMoveTarget( new NounZoneTarget( vAvoid, context() ) );
						// call this routine recursively
						return moveToTarget( pMoveTarget, 0.0f, 0.0f, 1.0f, a_bSlowToTarget, a_bForceJump, a_nDepth + 1 );
					}
					else
					{
						// clear jump path to our target, jump to the target
						useGadget( pJumpDrive, a_pTarget, false );
						// send the verb now, otherwise the code may attempt to do it again
						useGadgets();

						return MOVE_MOVING;
					}
				} // if ( distance < pJumpDrive->maxRange() )
				else
				{
					// ship is out of jump range... 
					// TODO: Check fuel and send a supply distress signal if low...
				}
			} // pJumpDrive->usable( NULL, true )	
		} // if ( distance > USE_JUMP_DISTANCE && a_nDepth < MAX_JUMP_DEPTH && pJumpDrive != NULL )

  		CollisionInfo info;
		if ( checkForCollision( a_pTarget, &info ) )
		{
			Vector3 vAvoid;
			calculateAvoidVector( info.m_pCollide, vAvoid );

			// move to the new position
			NounZoneTarget::Ref pMoveTarget( new NounZoneTarget( vAvoid, context() ) );
			// call this routine recursively
			return moveToTarget( pMoveTarget, a_fOutside, a_fWithin, a_fSpeed, a_bSlowToTarget, a_bForceJump, a_nDepth + 1 );
		}

	} // if ( a_nDepth < MAX_MOVE_DEPTH ) 
	else
		bNoPath = true;

	//----------------------------------------------------------------------------
	
	// reduce the distance from target by the within distance..
	fDistance -= a_fWithin;
	// get the normalized delta, calculate the correct heading from that value
	Vector3 vNormalizedDelta( ~vDelta );
	// get the heading
	float fHeading = atan2( vNormalizedDelta.x, vNormalizedDelta.z );
	// calculate yaw delta
	float fYawDelta = AngleDelta( NormalizeAngle( m_fHeading ), NormalizeAngle( fHeading ) );
	// check the velocity
	float fAcceleration = maxAcceleration();

	if (! bNoPath )
	{
		if ( m_fVelocity > 0.0f )
		{
			// calculate time until destination
			float t = fDistance / m_fVelocity;
			// calculate how long it will take to stop
			float t2 = m_fVelocity / (fAcceleration * 2.0f);

			//TRACE( CharString().format("distance = %.1f, velocity = %.1f, a = %.1f, t = %.1f, t2 = %.1f", distance, m_fVelocity, acceleration, t, t2) );

			if ( t2 < t )
			{
				//TRACE( "NounShip::pushMove, t2 < t" );
				if ( fabs( fYawDelta ) < PI4 )
					controlHeadingVelocity( fHeading, maxVelocity() * a_fSpeed ); 
				else
					controlHeading( fHeading ); 
			}
			else
			{
				//TRACE( "NounShip::pushMove, t2 > t" );
				if ( a_bSlowToTarget )
					controlVelocity( 0.0f );			// begin stopping
			}
		}
		else
		{
			if ( fabs( fYawDelta ) < PI4 )
				controlHeadingVelocity( fHeading, maxVelocity() * a_fSpeed ); //Min( m_fVelocity + acceleration,  ) );
			else
				controlHeading( fHeading );								// turn in place towards target
		}
	}
	else
	{
		// no path to target, just turn towards our target but come to a complete stop..
		controlHeadingVelocity( fHeading, 0.0f );
	}

	return MOVE_MOVING;
}

bool NounShip::updateDynamics( dword nTick )
{
	PROFILE_FUNCTION();

	bool bClientDynamics = isClient();

	if (!m_bDynamicsEnabled )
	{
		m_nDynamicsTick = nTick;
		return false;
	}
	else if ( nTick <= m_nDynamicsTick )
	{
		if ( m_nDynamicsTick > nTick )
			m_nDynamicsTick = nTick;		// dynamics tick should never be ahead of our current tick
		return true;
	}

	if (! isLocal() )
	{
		float fMaxVelocity = maxVelocity();
		float fMaxAcceleration = maxAcceleration() * TICK_DURATION_S;

		// for non-local ships, update their velocity based on their set velocity.. so we can properly
		// calculate drive energy generation on the server and other clients..
		while( m_nDynamicsTick < nTick )
		{
			float fAcceleration = Clamp( m_fSetVelocity - m_fVelocity, -fMaxAcceleration, fMaxAcceleration );
			m_fVelocity = Clamp( m_fVelocity + fAcceleration, 0.0f, fMaxVelocity );

			++m_nDynamicsTick;
		}
		m_fHeading = atan2( m_Frame.k.x, m_Frame.k.z );

		return false;
	}

	int nTicks = nTick - m_nDynamicsTick;
	if ( inOrbit() )
	{
		if ( m_fSetVelocity > 0.0f && isLocal() )
			Verb::Ref( new VerbBreakOrbit( this ) );
	}

	// This updates our Y plane which is based on a ship to ship collision list..
	updateShipPlane();

	if ( (flags() & FLAG_DYNAMICS_DISABLED) )
	{
		if ( bClientDynamics )
		{
			// if dynamics is disabled, decay the roll and y position if needed
			while( m_nDynamicsTick < nTick )
			{
				// decay the roll
				if ( enableRollDynamics() )
				{
					float fRollMass = mass() + 4.0f;
					if ( fRollMass > 0.0f )
					{
						if ( m_fRoll > EPSILON )			// prevent m_fRoll becoming denormalized (#DEN)
							m_fRoll -= m_fRoll / fRollMass;		
						else
							m_fRoll = 0.0f;
					}
				}

				// decay the Y position to our plane..
				m_Position.y += (m_fPlane - m_Position.y) * Y_DECAY_PERCENT;

				++m_nDynamicsTick;
			}
#if ENABLE_SHIP_PITCH
			float fElapsed = TICK_DURATION_S * nTicks;
			m_fPitch -= m_fPitch * fElapsed;		// decay any pitch
#endif
			// update the ship frame
			setOrientation( Vector3( m_fPitch, m_fHeading, -m_fRoll ) );
		}
	}
	else
	{
		float fMaxVelocity = maxVelocity();
		float fMaxYaw = maxYaw() * TICK_DURATION_S;
		float fMaxAcceleration = maxAcceleration() * TICK_DURATION_S;
		float fStaticYaw = m_fYaw * fMaxYaw;

		bool bInOrbit = inOrbit();
		while( m_nDynamicsTick < nTick )
		{
			// adjust the set heading by the current yaw
			m_fSetHeading += fStaticYaw;

			// move heading towards the set heading
			float fTurn = Clamp( AngleDelta( NormalizeAngle( m_fHeading ), NormalizeAngle( m_fSetHeading ) ), -fMaxYaw, fMaxYaw );
			if ( bClientDynamics && enableRollDynamics() )
			{
				m_fRoll = Clamp( m_fRoll - fTurn, MIN_SHIP_ROLL, MAX_SHIP_ROLL );
				float fRollMass = mass() + 4.0f;
				if ( fRollMass > 0.0f )
					m_fRoll -= m_fRoll / fRollMass;		// decay the roll
				else
					m_fRoll = 0.0f;
			}
			// update the heading
			m_fHeading = NormalizeAngle( m_fHeading + fTurn );

			// get the acceleration
			float fAcceleration = Clamp( m_fSetVelocity - m_fVelocity, -fMaxAcceleration, fMaxAcceleration );
			// update the velocity
			m_fVelocity = Clamp( m_fVelocity + fAcceleration, 0.0f, fMaxVelocity );
			
			// move the ships position
			if ( m_fVelocity > 0.0f )
				m_Position += Vector3( sinf( m_fHeading ), 0.0f, cosf( m_fHeading) ) * (m_fVelocity * TICK_DURATION_S);
			// decay the Y position to our plane..
			m_Position.y += (m_fPlane - m_Position.y) * Y_DECAY_PERCENT;

			++m_nDynamicsTick;
		}

#if ENABLE_SHIP_PITCH
		if ( bClientDynamics )
		{
			float fElapsed = TICK_DURATION_S * nTicks;
			// update the pitch...
			m_fPitch = Clamp( m_fPitch + (((m_LastPosition.y - m_Position.y) * fElapsed) * 0.5f), -PI4, PI4 );
			m_fPitch -= m_fPitch * fElapsed;		// decay any pitch
		}
#endif

		setOrientation( Vector3( m_fPitch, m_fHeading, -m_fRoll ) );
		// recalculate our world position now and all child objects..
		invalidateWorld();
	}

#if defined(_WIN32)
	if ( _isnan( m_Position.x ) )
		m_Position.x = 0.0f;
	if ( _isnan( m_Position.y ) )
		m_Position.y = 0.0f;
	if ( _isnan( m_Position.z ) )
		m_Position.z = 0.0f;
#else
	if ( isnan( m_Position.x ) )
		m_Position.x = 0.0f;
	if ( isnan( m_Position.y ) )
		m_Position.y = 0.0f;
	if ( isnan( m_Position.z ) )
		m_Position.z = 0.0f;
#endif

	return true;
}

void NounShip::updateShipPlane()
{
	float fLastPlane = m_fPlane;
	m_fPlane = 0.0f;

	Vector3 vMyPosition( worldPosition() );

	ShipCollisionList::iterator iCollision = m_ShipCollisions.begin();
	while( iCollision != m_ShipCollisions.end() )
	{
		NounShip * pShip = *iCollision;
		if ( pShip != NULL && pShip != this )
		{
			Vector3 vDelta( vMyPosition - pShip->worldPosition() );
			vDelta.y = 0.0f;		// zero our the Y axis

			float fDistance = vDelta.magnitude();
			float fInner = radius() + pShip->radius();
			if ( fDistance < fInner )
			{
				// smaller ships go above larger ships.. when ships have the same mass, then we compare key's
				if ( mass() < pShip->mass() || (mass() == pShip->mass() && key() < pShip->key()) )
				{
					// move this ship up from the other ships plane, unless we're already at a higher plane..
					float fTop = pShip->hull().max.y;
					m_fPlane = Max( pShip->m_fPlane + fTop, m_fPlane );
				}

				// collision still valid, continue..
				++iCollision;
				continue;
			}
		}

		// collision is no longer valid, remove it from the list..
		m_ShipCollisions.erase( iCollision++ );
	}

	CTRACE( m_fPlane != fLastPlane, CharString().format( "Ship %p plane changed from %.2f to %.2f", this, fLastPlane, m_fPlane ) );
}

//----------------------------------------------------------------------------
//EOF
