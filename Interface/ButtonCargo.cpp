/*
	ButtonCargo.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "System/Keyboard.h"
#include "Display/PrimitiveLineStrip.h"
#include "Render3D/NodeLight.h"
#include "Render3D/NodeSound.h"
#include "Interface/ButtonCargo.h"
#include "Interface/GameDocument.h"
#include "DarkSpace/NounUnit.h"
#include "DarkSpace/NounCargo.h"
#include "DarkSpace/CargoGadget.h"
#include "DarkSpace/CargoEnhancement.h"
#include "DarkSpace/VerbApplyEnhancement.h"

//----------------------------------------------------------------------------

const SizeInt CARGO_BUTTON_SIZE( 32, 24);

//----------------------------------------------------------------------------

ButtonCargo::ButtonCargo()
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL | SHOW_HOTKEY | LOCK_ICON_SIZE;
	m_Alpha = 0.75f;
	m_HotKey = 'U';

	setWindow( RectInt( PointInt(0,0), CARGO_BUTTON_SIZE ) );
	setEnable( false );
}

//----------------------------------------------------------------------------

void ButtonCargo::onUpdate( float t )
{
	WindowButton::onUpdate( t );

	if ( enabled() )
	{
		Noun * pCargo = m_Cargo;
		if (! pCargo )
			return;

		if ( WidgetCast<CargoEnhancement>( pCargo ) )
		{
			setTip(((CargoEnhancement *)pCargo)->status());
			m_HotKey = 0;
			return;
		}

		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );
		NounShip * pShip = pDoc->ship();
		if (! pShip )
			return;

		bool validAction = false;

		Noun * pTarget = pDoc->target();
		if ( pTarget != NULL )
		{
			if ( pShip->canUnload( pTarget, pCargo ) )
			{
				setTip( CharString().format( "Unload %s onto %s", 
					pCargo->name(), pTarget->name() ) );
				setIconColor( WHITE );

				validAction = true;
				m_HotKey = 'U';
			}
		}

		if ( !validAction )
		{
			if ( WidgetCast<NounUnit>( pCargo ) )
				setTip( CharString().format("%s %s", pCargo->name(), ((NounUnit *)pCargo)->status() ) );
			else
				setTip( pCargo->name() );
			setIconColor( GREY );
			m_HotKey = 0;
		}
	}
}

void ButtonCargo::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	if ( enabled() )
	{
		Noun * pCargo = m_Cargo;
		ASSERT( pCargo );

		// display white box around cargo if current target
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );

		if ( pDoc->target() == pCargo && (pDoc->tick() % 10) < 6 )
			renderGlow( context );

		if ( WidgetCast<CargoEnhancement>( pCargo ) )
		{
			// show the durability if an enhancement..
			CargoEnhancement * pCargoEnh = (CargoEnhancement *)pCargo;

			Font * pFont = windowStyle()->font();
			ASSERT( pFont );

			NounEnhancement * pEnhancement = pCargoEnh->enhancement();
			if ( pEnhancement != NULL )
			{
				int nMaxDamage = pEnhancement->maxDamage();
				if ( nMaxDamage > 0 )
				{
					WideString sQuantity;
					sQuantity.format( "%d/%d", nMaxDamage - pCargoEnh->damage(), nMaxDamage );

					SizeInt stringSize( pFont->size( sQuantity ) );
					PointInt stringPos( window.m_Right - stringSize.width, window.top );
					Font::push( context.display(), pFont, stringPos, sQuantity, WHITE );
				}
			}

			if ( ((CargoEnhancement *)pCargo)->quantity() <= 0 )
				destroy();
		}
		else if ( WidgetCast<NounCargo>( pCargo ) )
		{
			// draw the quantity if cargo
			Font * pFont = windowStyle()->font();
			ASSERT( pFont );

			WideString sQuantity;
			sQuantity.format( "%d", ((NounCargo *)pCargo)->quantity() );

			SizeInt stringSize( pFont->size( sQuantity ) );
			PointInt stringPos( window.m_Right - stringSize.width, window.top );
			Font::push( context.display(), pFont, stringPos, sQuantity, WHITE );

			if ( ((NounCargo *)pCargo)->quantity() <= 0 )
				destroy();
		}
		else if ( WidgetCast<NounUnit>( pCargo ) )
		{
			NounUnit * pUnit = (NounUnit *)pCargo;

			// draw the unit health on the button
			Font * pFont = windowStyle()->font();
			ASSERT( pFont );

			WideString sHealth;
			sHealth.format( "%d%%", 100 - ((pUnit->damage() * 100) / pUnit->maxDamage()) );

			PointInt ptHealth( window.right - pFont->size( sHealth ).width, window.top );
			Font::push( context.display(), pFont, ptHealth, sHealth, WHITE );

			// display damage bar
			if ( pUnit->damage() > 0 )
			{
				float damage = 1.0f - (pUnit->damage() / pUnit->maxDamage());
				RectInt bar( window.m_Left, window.m_Bottom + 1, 
					window.m_Right - (window.width() * (1.0f - damage)), window.m_Bottom + 3 );
				RectFloat barUV(0,0,1,1);

				Color barColor( 255 * (1.0f - damage), 255 * damage,0,255 );
				PrimitiveMaterial::push( context.display(), PrimitiveMaterial::NONE );
				PrimitiveWindow::push( context.display(), bar, barUV, barColor );
			}
		}
	}
}

bool ButtonCargo::onCursorMove( const RectInt & window, const PointInt & position, const PointInt & delta )
{
	if ( WindowButton::onCursorMove( window, position, delta ) )
	{
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );
		NounShip * pShip = pDoc->ship();
		if (! pShip )
			return false;
		Noun * pCargo = m_Cargo;
		if (! pCargo )
			return false;

		pDoc->setLeftHint( CharString().format( "TARGET %s", pCargo->name() ) );

		Noun * pTarget = pDoc->target();
		if ( WidgetCast<CargoEnhancement>( pCargo ) 
			&& VerbApplyEnhancement::canApply( (CargoEnhancement *)pCargo, pShip ) )
			pDoc->setRightHint( CharString().format( "USE %s", pCargo->name() ) );
		else if ( pTarget != NULL && pShip->canUnload( pTarget, pCargo ) )
			pDoc->setRightHint( CharString().format( "UNLOAD %s ONTO %s", pCargo->name(), pTarget->name() ) );
		else
			pDoc->setRightHint( "" );

		return true;
	}

	return false;
}

//----------------------------------------------------------------------------

bool ButtonCargo::trapKey() const
{
	return false;
}

void ButtonCargo::onButtonUp()
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	if (! m_HotKeyDown )
	{
		if ( pDoc->target() != m_Cargo )
			pDoc->setTarget( m_Cargo );
		else
			pDoc->setTarget( NULL );
	}
	else
	{
		// when using the hotkey, then actually unload the cargo item..
		onMenuUp();
	}

	WindowButton::onButtonUp();
}

void ButtonCargo::onMenuUp()
{
	GameDocument * pDoc = (GameDocument *)document();
	if (! pDoc )
		return;
	NounShip * pShip = pDoc->ship();
	if (! pShip )
		return;
	Noun * pCargo = m_Cargo;
	if (! pCargo )
		return;

	Noun * pTarget = pDoc->target();
	if ( WidgetCast<CargoEnhancement>( pCargo ) != NULL
		&& VerbApplyEnhancement::canApply( (CargoEnhancement *)pCargo, pShip ) )
	{
		Verb::Ref( new VerbApplyEnhancement( pShip, (CargoEnhancement *)pCargo ) );
		setEnable( false );
	}
	else if ( pTarget != NULL 
		&& pShip->canUnload( pTarget, pCargo ) )
	{
		pShip->verbUnload( pTarget, pCargo );
		setEnable( false );
	}
	else
		pDoc->setTarget( pCargo );

	WindowButton::onMenuUp();
}

//----------------------------------------------------------------------------

void ButtonCargo::setCargo( Noun * pCargo )
{
	m_Cargo = pCargo;

	// set the tool tip
	if ( WidgetCast<NounUnit>( pCargo ) )
	{
		setTip( CharString().format("%s %s", pCargo->name(), ((NounUnit *)pCargo)->status() ) );
		setIcon( ((NounUnit *)pCargo)->icon() );
	}
	else if(WidgetCast<CargoEnhancement>( pCargo ) )
	{
		CargoEnhancement * pCargoEnhancement = (CargoEnhancement *)pCargo;
		
		setTip( pCargoEnhancement->status() );
		if ( pCargoEnhancement->enhancement() != NULL )
		{
			setIcon( pCargoEnhancement->enhancement()->icon() );
			setColor( pCargoEnhancement->enhancement()->color() );
		}	
	}
	else 
	{
		setTip( pCargo->name() );
		setIcon( pCargo->icon() );
	}

	setEnable( true );
}

//----------------------------------------------------------------------------
// EOF
