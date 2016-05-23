/*
	GadgetTractorBeam.cpp
	(c)1999 Palestar Development, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Display/PrimitiveSetTransform.h"
#include "Display/PrimitiveTriangleFan.h"
#include "NounShip.h"
#include "NounCargo.h"
#include "NounUnit.h"
#include "NounAsteroid.h"
#include "ShipPlatform.h"
#include "NounJumpGate.h"
#include "NounNebula.h"
#include "Constants.h"
#include "VerbBreakOrbit.h"
#include "GadgetTractorBeam.h"
#include "GameContext.h"
#include "resource.h"
#include "VerbGadgetOff.h"

//---------------------------------------------------------------------------------------------------

static Constant TRACTOR_MASS_ENERGY_COST( "TRACTOR_MASS_ENERGY_COST", 0.1f );

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetTractorBeam, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetTractorBeam, 4135734184578657498LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetTractorBeam, NounGadget );
	ADD_REPLICATED_PROPERTY( m_Target, TICKS_PER_SECOND );
	ADD_REPLICATED_PROPERTY( m_vOffset, TICKS_PER_SECOND );
END_PROPERTY_LIST();

GadgetTractorBeam::GadgetTractorBeam() : m_vOffset( true )
{}

//----------------------------------------------------------------------------

void GadgetTractorBeam::render( RenderContext &context, 
					const Matrix33 & frame, 
					const Vector3 & position )
{
	// render the beam
	if ( m_Target.valid() )
	{
		// render the use effect
		Scene * pUseEffect = useEffect();
		if ( pUseEffect != NULL )
			pUseEffect->render( context, frame, position );
		
		Vector3 positionVS( context.worldToView( position ) );
		if (! context.sphereVisible( positionVS, length() ) )
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

void GadgetTractorBeam::release()
{
	NounGadget::release();
	m_Target = NULL;
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetTractorBeam::type() const
{
	return TRACTOR_BEAM;
}

dword GadgetTractorBeam::hotkey() const
{
	return 'T';
}

bool GadgetTractorBeam::usable( Noun * pTarget, bool shift ) const
{
	if ( m_Target.valid() )
		return true;	// we can always turn the beam off
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	if ( isCloaked() )
		return false;

	return validateTarget( pTarget );
}

bool GadgetTractorBeam::useActive() const
{
	return m_Target.valid();
}

//----------------------------------------------------------------------------

void GadgetTractorBeam::use( dword when, Noun * pTarget, bool shift )
{
	if (! m_Target.valid() && pTarget )
	{
		NounGadget::use( when, pTarget, shift );

		m_Target = (NounBody *)pTarget;
		m_vOffset = pTarget->worldPosition() - worldPosition();
		m_vOffset.y = 0.0f;
	}
	else
		m_Target = NULL;

	message( CharString().format( "<color;ffffff>Tactical: %s %s.", name(), useActive() ? "Active" : "Inactive" ) ); 
}

int	GadgetTractorBeam::useEnergy( dword nTick, int energy )
{
	bool bTurnOff = false;

	if ( m_Target.valid() && m_Target->isLocal() )
	{
		Noun * pParent = WidgetCast<Noun>( parent() );
		ASSERT( pParent );
		NounBody * pTarget = m_Target;
		ASSERT( pTarget );

		// prevent using a tractor beam on a jumping ship...
		if ( WidgetCast<NounShip>( pTarget ) )
		{
			NounShip * pTargetShip = (NounShip *)pTarget;
			if ( pTargetShip->jumpDrive() && pTargetShip->jumpDrive()->engaged() )
				bTurnOff = true;
		}

		// make sure the velocity is set in the ateroid, so if we turn off the tractor beam the asteroid will keep going..
		if ( WidgetCast<NounAsteroid>( pTarget ) )
			((NounAsteroid *)pTarget)->setVelocity( worldVelocity() );

		Vector3 vTargetPosition = worldPosition() + m_vOffset;
		Vector3 vDelta = vTargetPosition - pTarget->worldPosition();
		float fMoveDistance = vDelta.magnitude();
		float fTargetMass = pTarget->mass() - mass();
		if ( fTargetMass < 1.0f )
			fTargetMass = 1.0f;
		
		energy -= (fTargetMass * fMoveDistance * energyCost() );		// need to add falloff here?
		if ( (energy <= 0 || destroyed() || !validateTarget( pTarget ) || isCloaked()) )
			bTurnOff = true;

		if (! bTurnOff )
		{
			pTarget->setWorldPosition( vTargetPosition );

			// break the orbit of the target object
			if ( pTarget != NULL && pTarget->inOrbit() )
				Verb::Ref( new VerbBreakOrbit( pTarget ) );
		}
	}

	if ( bTurnOff )
		Verb::Ref( new VerbGadgetOff( this ) );

	return energy;
}

//----------------------------------------------------------------------------

bool GadgetTractorBeam::validateTarget( Noun * pTarget ) const
{
	if (! NounGame::validateTarget( pTarget ) )
		return false;
	if ( pTarget == parent() )
		return false;	// don't allow a ship to tractor themselves..
	if ( pTarget->zone() == NULL )
		return false;	// object is not in the zone..
	if ( WidgetCast<NounBody>( pTarget ) == NULL && WidgetCast<NounCargo>( pTarget ) == NULL )
		return false;	// we can only lock onto objects with mass or resources
	if ( WidgetCast<NounJumpGate>( pTarget ) != NULL )
		return false;	// don't allow jumpgates to be moved
	if ( WidgetCast<NounPlanet>( pTarget ) != NULL )
		return false;	// don't allow a planet to be tractored out of orbit
	if ( WidgetCast<NounUnit>( pTarget ) != NULL )
		return false;	// don't allow units to be tractored off planets
	if ( WidgetCast<NounNebula>( pTarget ) != NULL )
		return false;	// don't allow nebulas to be moved
	if ( WidgetCast<NounShip>( pTarget ) != NULL && WidgetCast<ShipPlatform>( pTarget ) == NULL )
		return false;	// only ship that can be tractored are platforms

	float fRange = length() * calculateModifier( MT_TRACTOR );
	Vector3 position( pTarget->worldPosition() );
	if ( (position - worldPosition()).magnitude() > fRange )
		return false;	// check the range

	// finally check the facing of the weapon
	return( checkFacing( position ) );
}

//----------------------------------------------------------------------------
// EOF
