/*
	GadgetJumpDrive.cpp
	(c)1999 Palestar Development, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Math/Helpers.h"
#include "Gui3d/InterfaceContext.h"
#include "NounShip.h"
#include "SceneryEffect.h"
#include "GadgetJumpDrive.h"
#include "VerbJump.h"
#include "VerbJumpBegin.h"
#include "VerbJumpAbort.h"
#include "VerbBreakOrbit.h"
#include "NounJumpTarget.h"
#include "resource.h"
#include "GameContext.h"

//! How much does the ships velocity affect how close we jump to our target...
static Constant JUMP_DISTANCE_VELOCITY( "JUMP_DISTANCE_VELOCITY", 30.0f );
//! How much faster does the jump drive recharge when out of combat... default 30% @ 1.3 x
static Constant OUT_OF_COMBAT_JUMP_DRIVE_BONUS( "OUT_OF_COMBAT_JUMP_DRIVE_BONUS", 1.3f );

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetJumpDrive, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetJumpDrive, 4099650247218117266LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetJumpDrive, NounGadget );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Energy );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Fuel );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_JumpTime );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_JumpVelocity );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_JumpTarget );
END_PROPERTY_LIST();

GadgetJumpDrive::GadgetJumpDrive() : m_Energy( 0 ), m_JumpTime( 0 ), m_Fuel( 0 ), m_JumpVelocity( 0.0f ),
	m_FuelUse( 0.0f ), m_FuelFactor( 1.0f ), m_EnergyFactor( 1.0f )
{}

//----------------------------------------------------------------------------

bool GadgetJumpDrive::read( const InStream & input )
{
	if (! NounGadget::read( input ) )
		return false;
	if ( jumping() && !m_JumpTarget.valid() )
		clearFlags( FLAG_JUMPING );
	return true;
}

//----------------------------------------------------------------------------

void GadgetJumpDrive::render( RenderContext &context, 
				const Matrix33 & frame, 
				const Vector3 & position )
{
	NounGadget::render( context, frame, position );

	if ( isClient() )
	{
		GameContext * pContext = gameContext();

		NounShip * pShip = WidgetCast<NounShip>( parent() );
		if ( pShip != NULL )
		{
			if ( pShip->isLocal() )
			{
				// render the jump effect
				if ( engaged() )
				{
					Scene * pProjEffect = projEffect();
					if ( pProjEffect != NULL )
						pProjEffect->render( context, frame, position );

					GameContext::sm_bJumpEffect = true;
					pContext->getStarField().setJumpActive( true );
					pContext->getStarField().setParticleCount( 300 );
				}
				else if ( GameContext::sm_bJumpEffect )
				{
					pContext->getStarField().setJumpActive( false );
					pContext->getStarField().setParticleCount( 150 );
					GameContext::sm_bJumpEffect = false;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------

void GadgetJumpDrive::setup()
{
	NounGadget::setup();

	setFlags( FLAG_JUMPING|FLAG_ENGAGED, false );

	m_JumpTime = 0;
	m_Fuel = maxFuel();

	calcluateFactors();
	m_Energy = energyCostMass();
}

void GadgetJumpDrive::initialize()
{
	NounGadget::initialize();

	// do this only if we are already attached to a ship
	if ( WidgetCast<NounShip>( parent() ) )
		calcluateFactors();
}

void GadgetJumpDrive::release()
{
	NounGadget::release();
	m_JumpTarget = NULL;
}

const float MAX_JUMP_VELOCITY = 1.0f;
const float JUMP_OFFSET = 250.0f;
const int	BEGIN_JUMP_OUT_EFFECT = TICKS_PER_SECOND;

inline bool HeadingEpsilon( float v1, float v2 )
{
	return fabs( v1 - v2 ) >= (PI / 360);
}

void GadgetJumpDrive::simulate( dword nTick )
{
	if ( engaged() || jumping() )
	{
		NounShip * pShip = WidgetCast<NounShip>( parent() );
		if (! pShip )
			return;
		dword nElapsed = nTick - tick();

		// update the energy and fuel factors
		calcluateFactors();

		if ( engaged() )
		{
			// if the drive is engaged and the cloak flag is applied after the jump due to latency, we want to abort the jump
			if ( isLocal() && pShip->testFlags( NounShip::FLAG_JUMP_DISABLED|NounShip::FLAG_CLOAKED ) )
				Verb::Ref( new VerbJumpAbort( pShip ) );

			// secondary fix so we check for activation of cloaking device prior to FLAG_CLOAKED being applied
			if (  isLocal() && pShip->gadgetCloak() != NULL )
				if ( pShip->gadgetCloak()->active() )
					Verb::Ref( new VerbJumpAbort( pShip ) );
				
			// check to see if ship is despawning, cancel the despawn if it is
			if( pShip->despawning() )
				pShip->cancelDespawn();
				
			// subtract fuel cost
			m_FuelUse += fuelCostMass() * nElapsed;
			if ( m_FuelUse > 1.0f )
			{
				int used = m_FuelUse;

				m_FuelUse -= used;
				m_Fuel = Max( m_Fuel - used, 0 );
				if ( m_Fuel <= 0 )
					m_JumpTime = 0;
			}

			// subtract jump time
			m_JumpTime -= nElapsed;
			if ( m_JumpTime <= 0 )
			{
				if ( isLocal() )
				{
					pShip->setPosition( pShip->position() + (m_JumpVelocity * (nElapsed + m_JumpTime)) );
					pShip->invalidateWorld();
				}

				// jump has ended!
				if ( context()->isClient() )
					createHitEffect();

				// clear the jumping and engaged flags
				clearFlags( FLAG_JUMPING|FLAG_ENGAGED );
				// enable dynamics, command, and control
				pShip->clearFlags( NounShip::FLAG_JUMPING|NounShip::FLAG_DYNAMICS_DISABLED|
					NounShip::FLAG_COMMAND_DISABLED|NounShip::FLAG_CONTROL_DISABLED );
				// notify the user that a jump is completed
				gameContext()->gameUser()->onJump( pShip );
				// lastly, post a message
				message( CharString().format( "<color;ffffff>Helm: Jump complete!" ) );
			}
			else if ( isLocal() )
			{
				pShip->setPosition( pShip->position() + (m_JumpVelocity * nElapsed) );
				pShip->invalidateWorld();
			}
		}
		else if ( jumping() )
		{
			if ( isLocal() )
			{
				// continue to check if jump can be made, abort jump if drive becomes unusable while aligning... 
				// reasons can be turning on cloak or entering a jump disruption field for example.
				if ( canJump( m_JumpTarget, false ) )
				{
					// get the jump delta 
					Vector3 jump( m_JumpTarget->worldPosition() - pShip->worldPosition() );
					jump.y = 0.0f;		// keep jumps on the playing field, don't allow 3D jumps yet.. ;)

					// get the jump heading from the jump delta
					float jumpHeading = atan2( jump.x, jump.z );
					// get the heading delta
					float delta = AngleDelta( NormalizeAngle( pShip->heading() ), jumpHeading );

					m_JumpTime = (int)(fabs( delta ) / (pShip->maxYaw() * TICK_DURATION_S));
					if ( m_JumpTime > 0 )
					{
						// enable ship controls
						pShip->clearFlags( NounShip::FLAG_CONTROL_DISABLED );
						// turn the ship towards the jump target, keep the same velocity
						pShip->controlHeading( jumpHeading );
						// re-disable ship controls
						pShip->addFlags( NounShip::FLAG_CONTROL_DISABLED );
					}
					else
					{
						if ( pShip->inOrbit() )
							Verb::Ref( new VerbBreakOrbit( pShip ) );

						// heading correct, engage jump drive
						float distance = jump.magnitude();
						distance -= (m_JumpTarget->radius() + pShip->radius()) * 2.0f;		// adjust the distance, so we don't get too close
						distance -= pShip->velocity() * JUMP_DISTANCE_VELOCITY;				// adjust the distance, so fast moving ships have time to slow if needed

						if ( distance < 1.0f )
							distance = 1.0f;

						float vt = ( velocity() * calculateModifier( MT_JUMPSPEED ) ) * TICK_DURATION_S;
						// calculate the jump time in ticks
						dword jumpTime = (dword)(distance / vt);
						// get the jump velocity
						jump.normalize();
						jump *= vt;

						Verb::Ref( new VerbJump( pShip, jumpTime, jump ) );
						// clear the jumping flag, so we don't issue a second VerbJump()
						clearFlags( FLAG_JUMPING );
					}
				}
				else
				{
					// drive is no longer usable on target... abort jump.
					Verb::Ref( new VerbJumpAbort( pShip ) );
					// clear the jumping flag so we don't keep sending VerbJumpAbort()
					clearFlags( FLAG_JUMPING|FLAG_ENGAGED );
				}
			}
		}
	}

	NounGadget::simulate( nTick );
}

//----------------------------------------------------------------------------

GadgetJumpDrive::Type GadgetJumpDrive::type() const
{
	return DRIVE_JUMP;
}

NounGadget::EnergyClass GadgetJumpDrive::energyClass() const
{
	return ENERGY_CLASS_JUMP;
}

dword GadgetJumpDrive::hotkey() const
{
	return( 'J' );
}

CharString GadgetJumpDrive::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip;
	if ( shift )
		return "Emergency Jump.";	
	// usage information
	float fMod = calculateModifier( MT_JUMPSPEED );
	tip += CharString().format("\nMax Distance:<X;100>%.0fgu @ %.0fgu/s", maxRange(), velocity() * fMod );
	float fCharge = ( ( energyCharge() * TICKS_PER_SECOND ) * damageRatioInv() ) * calculateModifier( MT_JUMPCOOLDOWN );
	float fOOCCharge = fCharge * OUT_OF_COMBAT_JUMP_DRIVE_BONUS;
	tip += CharString().format("\nRecharge Time:<X;100>%.1fs (Out of Combat: %.1fs)", energyCostMass() / fCharge, energyCostMass() / fOOCCharge );
	tip += CharString().format("\nEnergy Usage:<X;100>%.1f", energyCostMass() / 1000.0f );		
	
	return tip;
}

CharString GadgetJumpDrive::status() const
{
	return CharString().format("%d", m_Fuel);
}

float GadgetJumpDrive::addSignature() const
{
	return engaged() ? 50.0f : 0.0f;
}

bool GadgetJumpDrive::needReload( Noun * pWhom ) const
{
	return fuel() < maxFuel();
}

bool GadgetJumpDrive::usable( Noun * pTarget, bool shift ) const
{
	if ( engaged() || jumping() )
		return true;		// always allow abort and stop
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	return canJump( pTarget, shift );
}

int GadgetJumpDrive::usableWhen() const
{
	return Min<int>( energyCostMass() > 0 ? (100 - ((m_Energy * 100) / energyCostMass())) : 0, NounGadget::usableWhen() );
}

bool GadgetJumpDrive::useActive() const
{
	return engaged() || jumping();
}

void GadgetJumpDrive::use( dword when, Noun * pTarget, bool bShift )
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	ASSERT( pShip );

	if ( engaged() || jumping() )
	{
		// abort a jump
		if ( isLocal() )
			Verb::Ref( new VerbJumpAbort( pShip ) );
	}
	else if ( bShift )
	{
		// emergency jump
		if ( isLocal() )
		{
			if ( pShip->inOrbit() )
				Verb::Ref( new VerbBreakOrbit( pShip ) );

			dword jumpTime = ((rand() % 5) + 5) * TICKS_PER_SECOND;

			Vector3 jumpVelocity( sin( pShip->heading() ), 0, cos( pShip->heading() ) );
			jumpVelocity *= velocity() * TICK_DURATION_S;	

			Verb::Ref( new VerbJump( pShip, jumpTime, jumpVelocity ) );
		}
	}
	else if ( pTarget != NULL )
	{
		NounGadget::use( when, pTarget, bShift );

		if ( isLocal() )
			Verb::Ref( new VerbJumpBegin( pShip, pTarget ) );
	}
}

int GadgetJumpDrive::useEnergy( dword nTick, int energy )
{
	if (! engaged() )
	{
		// charge the drives
		if ( m_Energy < energyCostMass() )
		{
			int maxCharge = (int)(damageRatioInv() * energyCharge());
			int charge = Min( Min( maxCharge, energyCostMass() - m_Energy), energy );

			energy -= charge;
			m_Energy += Max( (int)(charge * calculateModifier( MT_JUMPCOOLDOWN )), 1 ) * OUT_OF_COMBAT_JUMP_DRIVE_BONUS;
		}
	}

	return energy;
}

bool GadgetJumpDrive::reload( Noun * pWhom )
{
	m_Fuel = Clamp( m_Fuel + refuelRate(), 0, maxFuel() );
	return m_Fuel < maxFuel();
}

//----------------------------------------------------------------------------

void GadgetJumpDrive::beginJump( Noun * pTarget )
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	ASSERT( pShip );

	// disable updateCommand() and ignore all ship control calls, ship is under control of the jump drive
	pShip->addFlags( NounShip::FLAG_CONTROL_DISABLED|NounShip::FLAG_COMMAND_DISABLED );
	// set the jump target
	m_JumpTarget = pTarget;
	// set the jump flag
	addFlags( FLAG_JUMPING );

	if ( isLocal() && pShip->inOrbit() )
		Verb::Ref( new VerbBreakOrbit( pShip ) );
}

void GadgetJumpDrive::doJump( dword when, dword jumpTime, const Vector3 & jump )
{
	NounShip * pShip = (NounShip *)parent();
	ASSERT( pShip );

	// create jumping effect
	if ( context()->isClient() )
		createUseEffect( false );

	// set the engaged flag
	addFlags( FLAG_ENGAGED );
	// clear the jumping flag
	clearFlags( FLAG_JUMPING );
	// set the jump time and velocity
	m_JumpTime = jumpTime;
	m_JumpVelocity = jump;
	// use all banked energy
	m_Energy = 0;
	// set the tick of the jump drive, so the ship position will be updated correctly 
	setTick( when );

	// disabled ship dynamics, command, and control while in jump
	pShip->addFlags( NounShip::FLAG_JUMPING|NounShip::FLAG_DYNAMICS_DISABLED|NounShip::FLAG_COMMAND_DISABLED|NounShip::FLAG_CONTROL_DISABLED );
	pShip->setHeading( atan2( jump.x, jump.z) );

	//TRACE( CharString().format("GadgetJumpDrive::doJump, when = %u, jumpTime = %u, position = %s", when, jumpTime, CharString( pShip->worldPosition() )) );

	// lastly send message that the jump drive is engaged
	message( CharString().format( "<color;ffffff>Helm: Engaging Jump Drive!" ) );
}

void GadgetJumpDrive::abortJump( dword when )
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if ( pShip != NULL )
	{
		// get the lag
		int lag = when - tick();
		// enable ship controls again
		pShip->clearFlags( NounShip::FLAG_JUMPING|NounShip::FLAG_DYNAMICS_DISABLED|NounShip::FLAG_COMMAND_DISABLED|NounShip::FLAG_CONTROL_DISABLED );

		// create jump out effect if engaged
		if ( engaged() )
		{
			// back up ship position if needed
			if ( lag < 0 )
			{
				if ( isLocal() )
				{
					// undo position changes by this jump drive, note that lag is a negative number
					pShip->setPosition( pShip->position() + (m_JumpVelocity * lag) ); 
					pShip->invalidateWorld();
				}
			}

			// display jump out effect
			if ( context()->isClient() )
				createHitEffect();
			
			message( "<color;ffffff>Helm: Jump aborted!" );
		}
		else
			message( "<color;ffffff>Helm: Jump Alignment Aborted!" );

		// clear jump drive flags
		clearFlags( FLAG_JUMPING|FLAG_ENGAGED );
		m_JumpTime = 0;
	}
}

void GadgetJumpDrive::clearJump()
{
	NounShip * pShip = (NounShip *)parent();
	ASSERT( pShip );

	// clear flags on ship
	pShip->clearFlags( NounShip::FLAG_JUMPING|NounShip::FLAG_DYNAMICS_DISABLED|NounShip::FLAG_COMMAND_DISABLED|NounShip::FLAG_CONTROL_DISABLED );
	// clear flags on this device
	clearFlags( FLAG_JUMPING|FLAG_ENGAGED );
	m_JumpTime = 0;
}

//----------------------------------------------------------------------------

bool GadgetJumpDrive::canJump( Noun * pTarget, bool shift ) const
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if ( pShip == NULL )
		return false;		// jump drive not attached to a ship
	if ( pShip->gadgetCloak() != NULL )
		if ( pShip->gadgetCloak()->active() )
			return false;
	if ( pShip->testFlags( NounShip::FLAG_JUMP_DISABLED|NounShip::FLAG_CLOAKED ) )
		return false;		// can't jump, drive disabled
	if ( damageRatio() > 0.75f )		// don't allow jump if drive is more than 50% damaged
		return false;
	if ( m_Energy < energyCostMass() )
		return false;		// still charging
	if ( m_Fuel <= 0 )
		return false;		// no fuel
	if ( shift )
		return true;		// allow emergency jump
	if ( pTarget == NULL )
		return false;		// standard jump requires a valid target object
	
	float distance = (worldPosition() - pTarget->worldPosition()).magnitude();
	if ( distance > maxRange() )
		return false;		// out of range

	return true;
}

void GadgetJumpDrive::calcluateFactors()
{
	Noun * pParent = WidgetCast<Noun>( parent() );
	if ( pParent != NULL )
	{
		// update the fuel and energy use factors, the mass of the ship may change dynamically
		m_FuelFactor = pParent->mass() / 10.0f;
		m_EnergyFactor = pParent->mass() / 10.0f;
	}
}

void GadgetJumpDrive::createHitEffect()
{
	NounShip * pShip = (NounShip *)parent();
	ASSERT( pShip );

	// create warp out effect
	if ( context()->isClient() && hitEffect() != NULL )
	{
		SceneryEffect * pEffect = new SceneryEffect;
		pEffect->setNounContext( new NounContext(hitEffect()) );
		pEffect->setOrientation( pShip->orientation() );
		pEffect->setPosition( pShip->worldPosition() );

		if (! pEffect->setSegment( "Effect" ) )
			pEffect->setLife( 15.0f );

		context()->attachNoun( pEffect );
	}
}

//----------------------------------------------------------------------------
// EOF
