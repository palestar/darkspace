/*
	GadgetJD.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "GadgetJD.h"
#include "VerbJumpAbort.h"
#include "GameContext.h"
#include "resource.h"
#include "StructureDefense.h"
#include "VerbGadgetOff.h"

//----------------------------------------------------------------------------

const dword GADGETJD_STEP_RATE = TICKS_PER_SECOND * 3;

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetJD, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetJD, 4346018582041573986LL );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetJD, NounGadget );
	//ADD_TRANSMIT_UPDATE_PROPERTY( m_Children );
END_PROPERTY_LIST();

GadgetJD::GadgetJD() : m_nLastFleetId( -1 ), m_fFieldScale( 1.0f )
{}

//----------------------------------------------------------------------------

void GadgetJD::render( RenderContext &context, 
				const Matrix33 & frame, 
				const Vector3 & position )
{	
	Scene * pUseEffect = useEffect();
	if ( active() && pUseEffect != NULL )
		pUseEffect->render( context, Matrix33::IDENTITY * m_fFieldScale, position );		
}

//----------------------------------------------------------------------------

void GadgetJD::initialize()
{
	NounGadget::initialize();

	m_pField = NULL;
	NounField::removeAllFields( this );
}

void GadgetJD::release()
{
	NounGadget::release();
}

//----------------------------------------------------------------------------

NounGadget::Type GadgetJD::type() const
{
	return INTERDICTOR;
}

dword GadgetJD::hotkey() const
{
	return 'I';
}

float GadgetJD::addSignature() const
{
	return active() ? 9.0f : 0.0f;
}

bool GadgetJD::usable( Noun * pTarget, bool shift ) const
{
	if ( active() )
		return true;
	if (! NounGadget::usable( pTarget, shift ) )
		return false;
	if ( destroyed() )
		return false;
	if ( isCloaked() || isSafeZone() )
		return false;
	return true;
}

bool GadgetJD::useActive() const
{
	return active();
}

void GadgetJD::use( dword when, Noun * pTarget, bool shift)
{
	if ( active() )
		NounGadget::use( when, pTarget, shift );

	// set the device active
	setFlags( FLAG_ACTIVE, !active() );
	message( CharString().format( "<color;ffffff>Tactical: %s %s.", name(), active() ? "Active" : "Inactive" ) ); 
}

int GadgetJD::useEnergy( dword nTick, int energy )
{
	if ( active() )
	{
		if ( !m_pField.valid() || m_nLastFleetId != teamId() )
		{
			// if the old field is still here due to ownership change, we need to null it before we setup a new one
			if ( m_pField.valid() )
			{
				detachNode( m_pField );
				m_pField = NULL;		
			}
			m_nLastFleetId = teamId();

			m_pField = new FieldInterdictor;
			m_pField->setName( "Jump Disruption Field" );

			float fRange = range();

			if ( WidgetCast<StructureDefense>( parent() ) )
			{
				NounPlanet * pPlanet = ((StructureDefense *)parent())->planet();
				m_fFieldScale = ( range() + pPlanet->radius() ) / range();
				m_pField->setFieldRadius( fRange * m_fFieldScale );			
				
				attachNode( m_pField );
				m_pField->setWorldPosition( pPlanet->worldPosition() );
			}
			else
			{
				m_fFieldScale = 1.0f;
				m_pField->setFieldRadius( fRange );			
				attachNode( m_pField );
			}
		}

		energy -= energyCost();
		if ( (energy < 0 || destroyed() || isCloaked() || isSafeZone()) && isLocal() )
			Verb::Ref( new VerbGadgetOff( this ) );
	}
	else if ( m_pField.valid() )
	{
		detachNode( m_pField );
		m_pField = NULL;
	}

	return energy;
}

bool GadgetJD::updateLogic()
{
	if ( WidgetCast<NounShip>( parent() ) )
	{
		NounShip * pShip = (NounShip *)parent();
		if ( !useActive() )
			pShip->useGadget( this, NULL, false );
		
		return true;
	}
	else if ( WidgetCast<StructureDefense>( parent() ) )
	{
		StructureDefense * pStructure = (StructureDefense *)parent();
		if ( pStructure->active() && !useActive() )
			pStructure->useGadget( NULL, this );
		else if ( !pStructure->active() && useActive() )
			pStructure->useGadget( NULL, this );

		return true;
	}

	return true;
}

float GadgetJD::addMass() const
{
	return active() ? 4.0f : 2.0f;
}

//----------------------------------------------------------------------------
//EOF
