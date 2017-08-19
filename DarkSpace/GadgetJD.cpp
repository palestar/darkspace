/*
    GadgetJD.cpp
    (c)2000 Palestar Inc, Richard Lyle
*/


#include "Debug/Assert.h"
#include "GadgetJD.h"
#include "VerbJumpAbort.h"
#include "GameContext.h"
#include "Resource.h"
#include "StructureDefense.h"
#include "VerbGadgetOff.h"

//----------------------------------------------------------------------------

const dword GADGETJD_STEP_RATE = TICKS_PER_SECOND * 3;

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( GadgetJD, NounGadget ); 
REGISTER_FACTORY_KEY( GadgetJD, 4346018582041573986 );

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetJD, NounGadget );
END_PROPERTY_LIST();

GadgetJD::GadgetJD() : m_nLastFleetId( -1 )
{}

//----------------------------------------------------------------------------

void GadgetJD::render( RenderContext &context, 
                const Matrix33 & frame, 
                const Vector3 & position )
{
    if ( active() )
    {
        Scene * pUseEffect = useEffect();
        if ( pUseEffect != NULL )
            pUseEffect->render( context, frame, position );
    }
}

//----------------------------------------------------------------------------

void GadgetJD::release()
{
    NounGadget::release();
    if ( m_pField.valid() )
        m_pField->setLink( NULL );
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
            m_nLastFleetId = teamId();

            m_pField = new FieldInterdictor;
            m_pField->setName( "Jump Distruption Field" );
            m_pField->setPosition( zonePosition() );
            m_pField->setFieldRadius( range() );
            m_pField->setLink( this );

            zone()->attachNoun( m_pField );
        }

        energy -= energyCost();
        if ( (energy < 0 || destroyed() || isCloaked() || isSafeZone()) && isLocal() )
            Verb::Ref( new VerbGadgetOff( this ) );
    }
    else if ( m_pField.valid() )
    {
        m_pField->setLink( NULL );
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
            pStructure->useGadget( NULL, this );        // turn on ECM 
        else if ( !pStructure->active() && useActive() )
            pStructure->useGadget( NULL, this );        // turn off ECM

        return true;
    }

    return true;
}

//----------------------------------------------------------------------------
//EOF
