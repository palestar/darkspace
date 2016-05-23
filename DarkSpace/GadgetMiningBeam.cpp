/*
	GadgetMiningBeam.cpp
	(c)1999 Palestar Development, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Display/PrimitiveTriangleFan.h"
#include "Display/PrimitiveSetTransform.h"
#include "CargoResource.h"
#include "NounAsteroid.h"
#include "NounPlanet.h"
#include "NounDropTarget.h"
#include "GameContext.h"
#include "GadgetMiningBeam.h"
#include "resource.h"
#include "VerbGadgetOff.h"

//----------------------------------------------------------------------------

const float MINE_PLANET_RES_SEC = Constant( "MINE_PLANET_RES_SEC", 1.0f );
const float MINE_ASTEROID_RES_SEC = Constant( "MINE_ASTEROID_RES_SEC", 2.0f );

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetMiningBeam, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetMiningBeam, 4366055366513706451LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetMiningBeam, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Target, TICKS_PER_SECOND );
END_PROPERTY_LIST();

GadgetMiningBeam::GadgetMiningBeam() : m_fMined( 0.0f ), m_nMineTick( 0 )
{}

//----------------------------------------------------------------------------

void GadgetMiningBeam::render( RenderContext &context, 
					const Matrix33 & frame, 
					const Vector3 & position )
{
	NounGadget::render( context, frame, position );

	// render the beam
	if ( m_Target.valid() )
	{
		// render the use effect
		Scene * pUseEffect = useEffect();
		if ( pUseEffect != NULL )
			pUseEffect->render( context, frame, position );

		Vector3 positionVS( context.worldToView( position ) );
		if (! context.sphereVisible( positionVS, range() ) )
			return;

		Vector3 direction( m_Target->worldPosition() - position );
		Vector3 head( position );
		Vector3 tail( head + direction );

		// calculate the material wrap
		float h = 0.05f;
		float w = 10.0f;

		Material * pTracerMaterial = tracerMaterial();
		if ( pTracerMaterial != NULL )
		{
			h = pTracerMaterial->height();
			w = pTracerMaterial->width();
			Material::push( context, pTracerMaterial );
		}
		else
			Material::push( context, Color(255,0,0), true, PrimitiveMaterial::ADDITIVE );

		float u = (head - tail).magnitude() / w;

		const Vector3	N( 0,0, 0);
		const Vector3	Y( 0, h, 0 );
		const Vector3	X( h, 0, 0 );
		const Color		HC( 255,255,255,255 );
		const Color		TC( 255,255,255,32 );

		VertexL beamY[4] = 
		{
			VertexL( head + Y, N, HC, u, 0.0f ),
			VertexL( tail + Y, N, TC, 0.0f, 0.0f ),
			VertexL( tail - Y, N, TC, 0.0f, 1.0f ),
			VertexL( head - Y, N, HC, u, 1.0f ),
		};
		VertexL beamX[4] = 
		{
			VertexL( head + X, N, HC, u, 0.0f ),
			VertexL( tail + X, N, TC, 0.0f, 0.0f ),
			VertexL( tail - X, N, TC, 0.0f, 1.0f ),
			VertexL( head - X, N, HC, u, 1.0f ),
		};
		
		DisplayDevice * pDisplay = context.device();
		ASSERT( pDisplay );

		context.pushIdentity();
		PrimitiveTriangleFanDL::push( pDisplay, 4, beamY );
		PrimitiveTriangleFanDL::push( pDisplay, 4, beamX );
	}
}

//----------------------------------------------------------------------------

void GadgetMiningBeam::release()
{
	NounGadget::release();
	m_Target = NULL;
}

void GadgetMiningBeam::simulate( dword nTick )
{
	// deactivate mining beam if we cloak while the beam is active
	if ( WidgetCast<NounShip>( parent() ) && ((NounShip *)parent())->testFlags( NounShip::FLAG_CLOAKED ) && useActive() )
		m_Target = NULL;
		
	if ( nTick >= m_nMineTick )
	{
		m_nMineTick = nTick + (TICKS_PER_SECOND * calculateModifier( MT_MINING, true ));

		Noun * pTarget = m_Target;
		if ( pTarget != NULL && WidgetCast<NounDropTarget>( pTarget ) )
			pTarget = ((NounDropTarget *)pTarget)->planet();

		if ( pTarget != NULL )
		{
			if ( WidgetCast<NounPlanet>( pTarget ) )
				mineResources( MINE_PLANET_RES_SEC * mineScale() );
			else if ( WidgetCast<NounAsteroid>( pTarget ) )
				mineResources( MINE_ASTEROID_RES_SEC * mineScale() );
		}
	}

	NounGadget::simulate( nTick );
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetMiningBeam::type() const
{
	return MINING_BEAM;
}

dword GadgetMiningBeam::hotkey() const
{
	return 'G';
}

bool GadgetMiningBeam::usable( Noun * pTarget, bool shift ) const
{
	if ( m_Target.valid() )
		return true;	// we can always turn the beam off
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;

	if ( WidgetCast<NounShip>( parent() ) && ((NounShip *)parent())->testFlags( NounShip::FLAG_CLOAKED ) )
		return false;			// weapons are disabled
	if ( WidgetCast<NounDropTarget>( pTarget ) )
		pTarget = ((NounDropTarget *)pTarget)->planet();
	if ( WidgetCast<NounPlanet>( pTarget ) == NULL &&
		WidgetCast<NounAsteroid>( pTarget ) == NULL )
		return false;	// mining beam can only be used on planets and asteroids

	Vector3 position( pTarget->worldPosition() );
	if ( (position - worldPosition()).magnitude() > range() )
		return false;	// out of range

	// finally check the facing of the weapon
	return( checkFacing( position ) );
}

bool GadgetMiningBeam::useActive() const
{
	return m_Target.valid();
}

//----------------------------------------------------------------------------

void GadgetMiningBeam::use( dword when, Noun * pTarget, bool shift )
{
	if ( m_Target.valid() )
	{
		NounGadget::use( when, pTarget, shift );
		m_Target = NULL;
	}
	else
		m_Target = pTarget;

	message( CharString().format( "<color;ffffff>Tactical: %s %s.", name(), useActive() ? "Active" : "Inactive" ) ); 
	m_fMined = 0.0f;
	m_nMineTick = when;
}

int	GadgetMiningBeam::useEnergy( dword nTick, int energy )
{
	Noun * pTarget = m_Target;
	if ( WidgetCast<NounDropTarget>( pTarget ) )
		pTarget = ((NounDropTarget *)pTarget)->planet();

	if ( pTarget != NULL )
	{
		energy -= energyCost();
		if ( isLocal() )
		{
			if ( (energy <= 0 || destroyed()) )
			{
				Verb::Ref( new VerbGadgetOff( this ) );
			}
			else 
			{
				Vector3 position( pTarget->worldPosition() );
				if ( (worldPosition() - position).magnitude() > range() || !checkFacing( position ) )
					Verb::Ref( new VerbGadgetOff( this ) );
			}
		}
	}

	return energy;
}

bool GadgetMiningBeam::updateLogic()
{
	if ( useActive() )
		return true;		// early out if not ready to fire or currently in use...

	if ( WidgetCast<NounShip>( parent() ) )
	{
		NounShip * pShip = (NounShip *)parent();

		Noun * pTarget = pShip->commandTarget();
		if (! pTarget )
			return true;	// no current target
		if ( usable( pTarget, false ) )
			pShip->useGadget( this, pTarget, false );

		return true;
	}

	return false;
}

//----------------------------------------------------------------------------

void GadgetMiningBeam::mineResources( float fAmount )
{
	if ( isServer() )
	{
		m_fMined += fAmount;
		if ( m_fMined >= 1.0f )
		{
			Noun * pParent = WidgetCast<Noun>( parent() );
			ASSERT( pParent );

			for(int i=0;i<pParent->childCount();i++)
			{
				CargoResource * pCheck = WidgetCast<CargoResource>( pParent->child(i) );
				if ( pCheck != NULL && pCheck->quantity() < pCheck->maxQuantity() )
				{
					int fTransfer = Min<int>( pCheck->maxQuantity() - pCheck->quantity(), m_fMined );
					m_fMined -= fTransfer;

					pCheck->setQuantity( pCheck->quantity() + fTransfer );
					//pCheck->resync();
				}
			}

			if ( m_fMined >= 1.0f )
			{
				if ( WidgetCast<NounShip>( pParent ) )
				{
					if ( ((NounShip *)pParent)->availableCargo() > 0 )
					{
						CargoResource::Ref pCargo = new CargoResource();
						pCargo->setQuantity( m_fMined );
						
						// attach the new object to the parent now
						pParent->attachNode( pCargo );
					}
				}

				m_fMined = 0.0f;
			}
		}
	}
}

//----------------------------------------------------------------------------
// EOF
