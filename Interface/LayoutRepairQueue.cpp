/*
	LayoutRepairQueue.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "DarkSpace/VerbRepairGadget.h"
#include "Interface/GameDocument.h"
#include "Interface/LayoutRepairQueue.h"

//----------------------------------------------------------------------------

Constant UPDATE_REPAIR_QUEUE_LAYOUT( "UPDATE_REPAIR_QUEUE_LAYOUT", 1.0f );

const RectFloat		DAMAGE_BAR_UV( 0,0,0,0);
const RectFloat		WINDOW_UV(0,0,1,1);

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutRepairQueue, WindowLayout );
REGISTER_FACTORY_KEY( LayoutRepairQueue, 4552747052922259409 );

LayoutRepairQueue::LayoutRepairQueue()
{}

//----------------------------------------------------------------------------

void LayoutRepairQueue::onActivate()
{
	WindowLayout::onActivate();
	detachAllNodes();
}

void LayoutRepairQueue::onDeactivate()
{
	WindowLayout::onDeactivate();
	detachAllNodes();
}

void LayoutRepairQueue::onUpdate( float t )
{
	WindowLayout::onUpdate( t );

	if ( visible() )
	{
		if ( activeTime() > UPDATE_REPAIR_QUEUE_LAYOUT )
		{
			GameDocument * pDoc = WidgetCast<GameDocument>( document() );
			ASSERT( pDoc );

			int buttonCount = 0;

			NounShip * pShip = pDoc->ship();
			if ( pShip != NULL )
			{
				for(int i=0;i<pShip->repairCount();i++)
					GetButton<ButtonGadget>( this, buttonCount++ )->setGadget( pShip->repair( i ) );
			}

			// remove any extra buttons
			cullChildren( buttonCount );
			// reset active time
			setActiveTime( 0 );
		}
	}
}

//----------------------------------------------------------------------------

LayoutRepairQueue::ButtonGadget::ButtonGadget() 
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | EFFECT_FADEIN | SMOOTH_LL | LOCK_ICON_SIZE;
	m_Alpha = 0.75f;

	setWindow( RectInt( PointInt(0,0), SizeInt( 32, 24 ) ) );
	setEnable( false );
}

void LayoutRepairQueue::ButtonGadget::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	NounGadget * pGadget = m_rGadget;
	if ( pGadget != NULL && enabled() )
	{
		DisplayDevice * pDisplay = context.display();
		ASSERT( pDisplay );
		Font * pFont = windowStyle()->font();
		ASSERT( pFont );

		// display damage percentage
		WideString status;
		status.format( STR("%d%%"), int( m_rGadget->damageRatioInv() * 100) );

		SizeInt stringSize( pFont->size( status ) );
		PointInt stringPos( window.m_Right - stringSize.width, window.top );
		Font::push( pDisplay, pFont, stringPos, status, YELLOW );

		// draw the gadget icon
		Material * pIcon = pGadget->icon();
		if ( pIcon != NULL )
		{
			RectInt iconBox( PointInt( window.left, window.top ), SizeInt( 32, 16 ) );

			Material::push( context, pIcon );
			PrimitiveWindow::push( pDisplay, iconBox, WINDOW_UV, RED );
		}

		// display the damage bar
		if ( m_rGadget->damage() > 0 )
		{
			if ( fmod( activeTime(), 1.0f ) < 0.5f )		// make the bar blink
			{
				float damage = m_rGadget->damageRatioInv();
				RectInt bar( window.m_Left, window.m_Bottom + 1, 
					window.m_Right - (window.width() * (1.0f - damage)), window.m_Bottom + 3 );

				Color barColor( 255 * (1.0f - damage), 255 * damage,0,255 );
				PrimitiveMaterial::push( pDisplay, PrimitiveMaterial::NONE );
				PrimitiveWindow::push( pDisplay, bar, DAMAGE_BAR_UV, barColor );
			}
		}

		// display white blinking box around gadget currently at the top of the queue
		NounShip * pShip = WidgetCast<NounShip>( m_rGadget->parentBody() );
		if ( pShip != NULL && pShip->repairCount() > 0 && pShip->repair( 0 ) == m_rGadget && fmod( activeTime(), 1.0f ) < 0.5f )
			renderGlow( context );
	}
}

void LayoutRepairQueue::ButtonGadget::onButtonUp()
{
	if ( enabled() )
	{
		// move this gadget to the top of the repair queue
		Verb::Ref( new VerbRepairGadget( m_rGadget, NounShip::NOW ) );	
	}

	WindowButton::onButtonUp();
}

void LayoutRepairQueue::ButtonGadget::setGadget( NounGadget * pGadget )
{
	m_rGadget = pGadget;

	setName( pGadget->name() );
	//setIcon( pGadget->icon() );
	setTip( CharString().format("%s\n%d%%", pGadget->nounContext()->name(), int( pGadget->damageRatioInv() * 100) ) );
	setEnable( true );
}

//----------------------------------------------------------------------------
//EOF
