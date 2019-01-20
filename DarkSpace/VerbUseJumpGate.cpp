/*
	VerbUseJumpGate.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "VerbUseJumpGate.h"
#include "NounShip.h"
#include "NounFighter.h"
#include "NounTrail.h"
#include "GadgetJD.h"
#include "NounField.h"
#include "GameContext.h"

//-------------------------------------------------------------------------------

IMPLEMENT_FACTORY( VerbUseJumpGate, Verb );

BEGIN_PROPERTY_LIST( VerbUseJumpGate, Verb )
	ADD_TRANSMIT_PROPERTY( m_pGate );
END_PROPERTY_LIST();

VerbUseJumpGate::VerbUseJumpGate()
{}

VerbUseJumpGate::VerbUseJumpGate( Noun * pUser, NounJumpGate * pGate ) : m_pGate( pGate )
{
	attachVerb( pUser );
}

//----------------------------------------------------------------------------

Verb::Priority VerbUseJumpGate::priority() const
{
	return CRITICAL;
}

Verb::Scope VerbUseJumpGate::scope() const
{
	return LOCAL;
}

bool VerbUseJumpGate::client() const
{
	return true;
}

bool VerbUseJumpGate::queue() const
{
	return false;
}

//----------------------------------------------------------------------------

bool VerbUseJumpGate::canAttach( Noun * pNoun )
{
	return pNoun != NULL;
}

void VerbUseJumpGate::onExecute()
{
	Noun * pUser = target();
	if ( validate( pUser ) && validate( m_pGate ) )
	{
		WorldContext::Ref pContext = context();

		bool bAllyShip = false;

		if ( WidgetCast<NounShip>( pUser ) )
		{
			NounShip * pShip = (NounShip *)pUser;

			bAllyShip = pShip->testFlags( NounShip::FLAG_AI_ALLY );
			// turn off jumpdrive if active
			if ( pShip->jumpDrive() )
				pShip->jumpDrive()->clearJump();
				
			// remove any interdictor fields that would leave other ships stuck
			for(int i=0;i<pShip->childCount();i++)
			{
				
				NounGadget * pGadget = WidgetCast<NounGadget>( pShip->child(i) );
				if ( pGadget != NULL && pGadget->type() == NounGadget::INTERDICTOR )
				{
					if ( pGadget->useActive() )
						pGadget->use( tick(), NULL, false );
					pGadget->release();
				}
			}				
			
			// clear interdictor flag, we don't want the ship to become stuck
			pShip->clearFlags( NounShip::FLAG_JUMP_DISABLED );

			// clear safe zone flag, we don't want weapons to be stuck either
			pShip->clearFlags(NounShip::FLAG_IN_SAFE_ZONE);
			
			// clear command, if it was to follow into this jumpgate
			if ( pShip->command() == NounShip::FOLLOW && pShip->commandTarget() == m_pGate )
				pShip->setCommand( NounShip::NOCOMMAND, NULL );
		}

		//TRACE( CharString().format("VerbUseJumpGate::execute, context = %p, noun = %s", context(), pUser->name() ) );
		if (! m_pGate->isRemoteServer() )
		{
			// This is a local destination, find the object or detach the object...
			Noun * pDestination = m_pGate->destinationNoun();
			if ( validate( pDestination ) )
			{
				NodeZone * pZone = pDestination->zone();
				ASSERT( pZone );
				NodeZone * pUserZone = pUser->zone();
				ASSERT( pUserZone );

				Matrix33 frame( pDestination->frame() );
				Vector3 position( pDestination->position() );
				position += frame.k * (pDestination->radius() + pUser->radius()) * 1.25f;

				if ( WidgetCast<NounShip>( pUser ) )
				{
					NounShip * pShip = (NounShip *)pUser;

					pShip->setHeading( atan2( frame.k.x, frame.k.z ) );
					// set the objects position
					pShip->setPosition( position );
					// set the last position as well, or object will collide with objects between this gate and the other
					pShip->setLastPosition( position );
					//pShip->unlockDynamics( tick() );
				}
				else
				{
					// set the objects position
					pUser->setPosition( position );
					// set the last position as well, or object will collide with objects between this gate and the other
					pUser->setLastPosition( position );
				}

				pUser->invalidateWorld();

				// transfer the target to the correct zone if necessary, don't update it's position
				if ( pUserZone != pZone )
					pUserZone->transferNoun( pUser, pZone, false );

				// if destination is a jumpgate, create a use effect
				if ( pContext->isClient() && WidgetCast<NounJumpGate>( pDestination ) )
				{
					((NounJumpGate *)pDestination)->createUseEffect();
					
					// prevent "trailjumping"
					if( WidgetCast<NounShip>( pUser ) ||
						WidgetCast<NounFighter>( pUser ) )
					{
						// Prevent "trailjump" for ships and fighters
						for(int i=0;i<pUser->childCount();i++)
						{
							NounTrail * pTrail = WidgetCast<NounTrail>( pUser->child(i) );
							if ( pTrail != NULL )
								pTrail->reset();
						}
					}

				}
			}
			else 
			{
				// home gate, ship is leaving the world

				// NOTE: We don't let ships with the FLAG_AI_ALLY use home gates, since we've had some complaits
				// from players who are fighting near jumpgates losing their ally ships.
				if ( !bAllyShip && pContext->isServer() )
					pUser->setDetach();
			}
		}
		else
		{
			// remote destintation, notify the user and detach the user after...
			pContext->user()->onServerTransfer( pUser, m_pGate->address(), 
				m_pGate->port(), m_pGate->destination() );
			pUser->setDetach();
		}

		// create the use effect
		if ( pContext->isClient() )
			m_pGate->createUseEffect();
	}
}

//----------------------------------------------------------------------------
//EOF
