/*
	ButtonGadget.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "System/Keyboard.h"
#include "System/Platform.h"
#include "Display/PrimitiveMaterial.h"
#include "Display/PrimitiveLineStrip.h"
#include "Interface/ButtonGadget.h"
#include "DarkSpace/GadgetBeamWeapon.h"
#include "DarkSpace/VerbSetGadgetGroup.h"
#include "Interface/GameDocument.h"

//----------------------------------------------------------------------------

ButtonGadget::ButtonGadget() : 
	m_bCursorOver( false )
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL;
	m_Alpha = 0.75f;
	m_IconColor = GREEN;

	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setEnable( false );
}

//----------------------------------------------------------------------------

bool ButtonGadget::onMessage( const Message & msg )
{
	// we have to capture keystrokes instead of WindowButton::onMessage().. the reason is multiple buttons can have the same 
	// hotkey and they all need to be activated by a single key. The default handler, will stop and return true on the first button

	if ( enabled() )
	{
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );

		switch( msg.message )
		{
		case HM_KEYDOWN:
			{
				// key the key depressed, remove the SHIFT flag from the key code
				dword key = msg.wparam & ~HMK_SHIFT_MASK;
				if ( key == m_Gadget->hotkey() || key == m_Gadget->group() )
				{
					m_HotKeyDown = true;
					onMenuDown();

					return false;
				}
			}
			break;
		case HM_KEYUP:
			{
				dword key = msg.wparam & ~HMK_SHIFT_MASK;		// get the key, remove the shift flag
				if ( key >= '0' && key <= '9' )
				{
					// handle group assignment
					NounGadget * pTarget = WidgetCast<NounGadget>( pDoc->target() );
					if ( pTarget != NULL && pTarget->parent() == pDoc->ship() )
					{
						m_HotKeyDown = false;

						// the current target is a gadget on the players ship, do not use the device, assign a group instead
						// if our gadget is the current target
						if ( pTarget == m_Gadget )
						{
							// check if we are unassigning the group
							if ( key == '0' )
								key = 0;

							Verb::Ref( new VerbSetGadgetGroup( m_Gadget, key ) );
							return true;
						}

						// not assigning a group to our gadget, next button
						return false;
					}
				}
				if ( key == m_Gadget->hotkey() || key == m_Gadget->group() )
				{
					m_HotKeyDown = false;
					onMenuUp();

					return false;					// allow more than one button to be assigned to this key
				}
			}
			break;
		}
	}

	return WindowButton::onMessage( msg );
}


//----------------------------------------------------------------------------

void ButtonGadget::onUpdate( float t )
{
	WindowButton::onUpdate( t );

	if ( enabled() )
	{
		GameDocument * pDoc = (GameDocument *)document();
		if (! pDoc )
			return;
		NounGadget * pGadget = m_Gadget;
		if (! pGadget )
			return;

		// fade  or show the window background based on if the gadget can be used on the current target
		bool isShiftDown = Keyboard::shiftDown();
		if ( pGadget->useActive() )
		{
			setTargetAlpha( 0.75f );
			m_IconColor = WHITE;
		}
		else if ( pGadget->usable( pDoc->target(), isShiftDown  ) )
		{
			setTargetAlpha( 0.75f );
			m_IconColor = GREEN;
		}
		else
		{
			setTargetAlpha( 0.35f );
			m_IconColor = GREY;
		}

		if ( m_bCursorOver )
		{
			// update the tool tip
			CharString sTip;
			sTip += pGadget->nounContext()->name();
			sTip += CharString("\n") + pGadget->useTip( pDoc->target(), isShiftDown );

			setTip( sTip );
		}
	}
}

const Color INACTIVE(0, 255, 0 );
const Color ACTIVE( 0, 255, 0 );
const RectFloat WINDOW_UV(0,0,1,1);

void ButtonGadget::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	// get a pointer to our gadget
	NounGadget * pGadget = m_Gadget;
	if ( pGadget != NULL )
	{
		DisplayDevice * pDisplay = context.display();
		ASSERT( pDisplay );
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );
		WindowStyle * pStyle = windowStyle();
		ASSERT( pStyle );
		Font * pFont = pStyle->font();
		ASSERT( pFont );

		// display bar if gadget has delay before usabled
		int delay = pGadget->usableWhen();
		if ( delay > 0 )
		{
			if ( (pDoc->tick() % 10) < 6 )		// make the bar blink
			{
				RectInt bar( window.left, window.top, 
					window.right - ((window.width() * delay) / 100), window.top + 16 );

				PrimitiveMaterial::push( pDisplay, PrimitiveMaterial::ADDITIVE );
				PrimitiveWindow::push( pDisplay, bar, WINDOW_UV, Color(0,0,255,255) );
			}			
		}

		// draw the gadget icon
		Material * pIcon = pGadget->icon();
		if ( pIcon != NULL )
		{
			RectInt iconBox( PointInt( window.left, window.top ), SizeInt( 32, 16 ) );

			Material::push( context, pIcon );
			PrimitiveWindow::push( pDisplay, iconBox, WINDOW_UV, m_IconColor );
		}

		// display any gadget status text
		WideString sStatus( pGadget->status() );
		if ( sStatus.length() > 0 )
		{
			SizeInt stringSize( pFont->size( sStatus ) );
			Font::push( pDisplay, pFont, PointInt( window.m_Right - stringSize.width, window.top ), sStatus, YELLOW );
		}

		// display hotkey in lower-left corner of button
		CharString sHotKey;
		if ( pGadget->hotkey() != 0 && pGadget->hotkey() != HK_SPACE )
			sHotKey += keyText( Keyboard::unmap( pGadget->hotkey() ) );

		if ( m_Gadget->group() != 0 )
			sHotKey += CharString().format(" %c", m_Gadget->group() );

		if ( WidgetCast<GadgetBeamWeapon>( pGadget ) && ((GadgetBeamWeapon *)pGadget)->pointDefense() )
			sHotKey += " PD";

		if ( sHotKey.length() > 0 )
		{
			WideString sWide = sHotKey;
			SizeInt stringSize( pFont->size( sWide ) );

			Font::push( pDisplay, pFont, PointInt( window.m_Right - stringSize.width, window.m_Bottom - stringSize.height ), 
				sWide, YELLOW );
		}

		// display the damage bar
		if ( pGadget->damage() > 0 )
		{
			if ( (pDoc->tick() % 10) < 6 )		// make the bar blink
			{
				float damage = pGadget->damageRatioInv();
				RectInt bar( window.m_Left, window.m_Bottom + 1, 
					window.m_Right - (window.width() * (1.0f - damage)), window.m_Bottom + 3 );

				Color barColor( 255 * (1.0f - damage), 255 * damage,0,255 );
				PrimitiveMaterial::push( pDisplay, PrimitiveMaterial::NONE );
				PrimitiveWindow::push( pDisplay, bar, WINDOW_UV, barColor );
			}
		}

		// blink a white border if this is the current target
		if ( pDoc->target() == m_Gadget && (pDoc->tick() % 10) < 6 )
			renderGlow( context );
	}
}

bool ButtonGadget::onCursorMove( const RectInt & window, const PointInt & position, const PointInt & delta )
{
	if ( WindowButton::onCursorMove( window, position, delta ) )
	{
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );
		
		pDoc->setLeftHint( CharString().format( "TARGET %s", m_Gadget->name() ) );
		pDoc->setRightHint( CharString().format( "USE %s", m_Gadget->name() ) );
		m_bCursorOver = true;
		return true;
	}

	m_bCursorOver = false;
	return false;
}

//----------------------------------------------------------------------------

bool ButtonGadget::trapKey() const
{
	return false;
}

void ButtonGadget::onButtonUp()
{
	WindowButton::onButtonUp();

	if ( enabled() )
	{
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );

		if ( pDoc->target() != m_Gadget )
			pDoc->setTarget( m_Gadget );
		else
			pDoc->setTarget( NULL );
	}
}

void ButtonGadget::onMenuUp()
{
	WindowButton::onMenuUp();

	if ( enabled() )
	{
		ASSERT( m_Gadget.valid() );

		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );

		bool isShift = Keyboard::shiftDown();

		// use the gadget, sometimes the ship may have been detached and the gadget may have a null parent, so check
		// to make sure it is still attached to the players ship
		NounShip * pParent = WidgetCast<NounShip>( m_Gadget->parent() );
		if ( pParent != NULL && m_Gadget->usable( pDoc->target(), isShift ) )
		{
			ASSERT( pParent == pDoc->ship() );		// this should be the same object!
			pParent->useGadget( m_Gadget, pDoc->target(), isShift );
		}
	}
}

//----------------------------------------------------------------------------

void ButtonGadget::setGadget( NounGadget * pGadget )
{
	ASSERT( windowStyle() );

	if ( m_Gadget != pGadget )
	{
		m_Gadget = pGadget;

		if ( m_Gadget.valid() )
		{
			setName( pGadget->name() );
			setEnable( true );				// enable this button
		}
		else
		{
			setName( "" );
			setEnable( false );
		}
	}
}

//----------------------------------------------------------------------------
// EOF
