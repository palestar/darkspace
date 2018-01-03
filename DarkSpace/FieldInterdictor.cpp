/*
    FieldInterdictor.cpp
    (c)2004 Palestar Inc, Richard Lyle
*/


#include "Debug/Profile.h"
#include "FieldInterdictor.h"
#include "GadgetJD.h"
#include "NounShip.h"
#include "VerbJumpAbort.h"
#include "Resource.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( FieldInterdictor, NounField );
REGISTER_FACTORY_KEY( FieldInterdictor, 4635880574829179581 );

BEGIN_PROPERTY_LIST( FieldInterdictor, NounField )
END_PROPERTY_LIST();

FieldInterdictor::FieldInterdictor()
{}

//----------------------------------------------------------------------------

void FieldInterdictor::simulate( dword nTick )
{
    // verify that our GadgeJD device is linked to this field, this removes orphaned fields
    if ( WidgetCast<GadgetJD>( link() ) )
        if ( ((GadgetJD *)link())->field() != this )
            setLink( NULL );
    NounField::simulate( nTick );
}

//----------------------------------------------------------------------------

void FieldInterdictor::onEnterField( Noun * pNoun )
{}

void FieldInterdictor::onInsideField( Noun * pNoun )
{
    if ( m_pLink.valid() && WidgetCast<NounShip>( pNoun ) )
    {
        NounShip * pShip = (NounShip *)pNoun;

        // disable any non-friendly ships inside the field...
	    if ( ! m_pLink->isFriend( pShip ) )
        {
            if ( !pShip->testFlags( NounShip::FLAG_JUMP_DISABLED ) )    // no need to disable, if already disabled
            {
                pShip->message( "<color;ffffff>Helm: Jump drives disabled by interdictor.." );
                pShip->addFlags( NounShip::FLAG_JUMP_DISABLED );

                if ( server() )
                {
                    // abort the jump if any
                    if ( pShip->jumpDrive() != NULL && (pShip->jumpDrive()->engaged() || pShip->jumpDrive()->jumping()) )
                        Verb::Ref( new VerbJumpAbort( pShip ) );
                }
            }
        }
        /*
        else if ( pShip->testFlags( NounShip::FLAG_JUMP_DISABLED ) )
        {
            // ship is no longer an enemy ship, enable jump drives..
            pShip->clearFlags( NounShip::FLAG_JUMP_DISABLED );
        }
        */
    }
}

void FieldInterdictor::onLeaveField( Noun * pNoun )
{
    if ( WidgetCast<NounShip>( pNoun ) )
    {
        NounShip * pShip = (NounShip *)pNoun;
        // ship has left interdictor field, enable jump drive
        pShip->clearFlags( NounShip::FLAG_JUMP_DISABLED );
    }
}

//----------------------------------------------------------------------------
//EOF
