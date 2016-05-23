/*
	NounSmartProjectile.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "World/NounTarget.h"
#include "NounSmartProjectile.h"
#include "GameContext.h"

#define ENABLE_PROJECTILE_INTERCEPT		1

//----------------------------------------------------------------------------

Constant SMART_PROJECTILE_AUTO_TARGET_RANGE( "SMART_PROJECTILE_AUTO_TARGET_RANGE", 500.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounSmartProjectile, NounProjectile );
REGISTER_FACTORY_KEY( NounSmartProjectile, 4296314670275078591LL );

BEGIN_PROPERTY_LIST( NounSmartProjectile, NounProjectile )
	//ADD_TRANSMIT_PROPERTY( m_fTurnRate );
	//ADD_TRANSMIT_PROPERTY( m_fVelocity );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fYaw );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_fPitch );
END_PROPERTY_LIST();

NounSmartProjectile::NounSmartProjectile() : m_fTurnRate( 0 ), m_fVelocity( 0 ), m_fYaw( 0 ), m_fPitch( 0 ), m_fRoll( 0 ), m_nLogicTick( 0 )
{}

//----------------------------------------------------------------------------

void NounSmartProjectile::initialize()
{
	NounProjectile::initialize();

	if ( m_pWeapon.valid() )
	{
		m_fTurnRate = m_pWeapon->turnRate();
		m_fVelocity = m_pWeapon->projectileVelocity();

		if ( m_pWeapon->projectileAutoTarget() || !m_pWeapon->needsTarget() )
			addFlags( FLAG_AUTO_TARGET );
		if ( m_pWeapon->isMine() )
			addFlags( FLAG_CAN_STOP );
	}
}

void NounSmartProjectile::simulate( dword nTick )
{
	if ( nTick >= m_nLogicTick )
	{
		m_nLogicTick = nTick + TICKS_PER_SECOND;

		Noun * pTarget = m_pTarget;
		if ( validateTarget( pTarget ) )
		{
#if ENABLE_PROJECTILE_INTERCEPT
			Vector3 vTargetPosition( pTarget->worldPosition() );
			Vector3 vTargetVelocity( pTarget->worldVelocity() );

			float fOvershoot = 0.0f;
			if ( m_pWeapon.valid() )
				fOvershoot = Max( m_pWeapon->interceptMaxRadius() - pTarget->radius(), 0.0f );

			Vector3 vIntercept;
			NounProjectile::intercept( vTargetPosition, vTargetVelocity, vIntercept, fOvershoot );

			Vector3 vDelta( vIntercept - worldPosition() );
			vDelta.normalize();

			control( vDelta );
#else
			Vector3 vDelta( pTarget->worldPosition() - worldPosition() );
			vDelta.normalize();

			control( vDelta );
#endif
		}
		else
		{
			if ( flags() & FLAG_AUTO_TARGET )
			{
				// auto-target 
				Array< GameContext::NounCollision > nouns;
				if ( context()->proximityCheck( worldPosition(), SMART_PROJECTILE_AUTO_TARGET_RANGE, nouns, CLASS_KEY(NounShip) ) )
				{
					float fClosestRange = 0.0f;
					for(int i=0;i<nouns.size();i++)
					{
						NounShip * pShip = WidgetCast<NounShip>( nouns[i].pNoun );
						if ( pShip == NULL || isFriend( pShip ) || !validateTarget( pShip ) )
							continue;	// skip object

						if ( !validateTarget( pTarget ) || nouns[i].fDistance < fClosestRange )
						{
							pTarget = pShip;
							fClosestRange = nouns[i].fDistance;
						}
					}

					// set the new target, may be NULL...
					m_pTarget = pTarget;
				}
			}
			else
				m_nLife = 0;		// target was lost
		}
	}

	if ( validateTarget( m_pTarget ) )
	{
		Vector3 vOrientation( orientation() );
		
		float fPitch = Clamp( m_fPitch, -m_fTurnRate, m_fTurnRate ) * TICK_DURATION_S;
		m_fPitch -= fPitch;
		vOrientation.x += fPitch;

		float fYaw = Clamp( m_fYaw, -m_fTurnRate, m_fTurnRate ) * TICK_DURATION_S;
		m_fYaw -= fYaw;
		vOrientation.y += fYaw;

		setOrientation( vOrientation );
		setVelocity( m_Frame.k * m_fVelocity );
	}
	else
	{
		// no target, decay the velocity and stop the projectile (mine)
		if ( flags() & FLAG_CAN_STOP )
			setVelocity( velocity() - (velocity() / 32.0f) );
	}

	// move the projectile
	NounProjectile::simulate( nTick );
}

void NounSmartProjectile::control( const Vector3 & toward )
{
	// toward should be normalized!
	float fNewYaw = atan2( toward.x, toward.z );
	float fNewPitch = -asin( toward.y );
	
	m_fYaw = AngleDelta( orientation().m_fYaw, fNewYaw );
	m_fPitch = AngleDelta( orientation().m_fPitch, fNewPitch );
}

//----------------------------------------------------------------------------
// EOF

