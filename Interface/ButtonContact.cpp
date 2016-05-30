/*
	ButtonContact.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "System/Keyboard.h"
#include "Display/PrimitiveLineStrip.h"
#include "Render3D/NodeLight.h"
#include "Render3D/NodeSound.h"
#include "DarkSpace/NounStar.h"
#include "DarkSpace/NounNebula.h"
#include "DarkSpace/NounAsteroid.h"
#include "World/NounTarget.h"
#include "DarkSpace/NounJumpGate.h"
#include "DarkSpace/NounProjectile.h"

#include "GameDocument.h"
#include "ButtonContact.h"
#include "ViewGame.h"
#include "WindowNavigation.h"

//----------------------------------------------------------------------------

const int BUTTON_WIDTH = 52;
const int BUTTON_HEIGHT = 24;

//----------------------------------------------------------------------------

ButtonContact::ButtonContact() : 
	m_IsObjective( false ),
	m_bGroupLeader( false ),
	m_bGroupPending( false )

{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | SMOOTH_LL | EFFECT_FADEIN;
	m_Alpha = 0.75f;

	setWindow( RectInt( PointInt(0,0), SizeInt( BUTTON_WIDTH, BUTTON_HEIGHT ) ) );
}

ButtonContact::ButtonContact( NodeWindow * pParent, Noun * pNoun ) : 
	m_IsObjective( false ),
	m_bGroupLeader( false ),
	m_bGroupPending( false )
{
	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | SMOOTH_LL | EFFECT_FADEIN;
	m_Alpha = 0.75f;

	setWindow( RectInt( PointInt(0,0), SizeInt( BUTTON_WIDTH, BUTTON_HEIGHT ) ) );

	// attach this button to the parent
	pParent->attachNode( this );
	// set this button
	setButton( pNoun );
}

//----------------------------------------------------------------------------

bool ButtonContact::onMessage( const Message & msg )
{
	return WindowButton::onMessage( msg );
}

//----------------------------------------------------------------------------

const Color INACTIVE(0, 255, 0 );
const Color ACTIVE( 0, 255, 0 );
const RectFloat WINDOW_UV(0,0,1,1);

void ButtonContact::onUpdate( float t )
{
	WindowButton::onUpdate( t );
}

void ButtonContact::onRender( RenderContext & context, const RectInt & window )
{
	WindowButton::onRender( context, window );

	DisplayDevice * pDisplay = context.display();
	ASSERT( pDisplay );
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );
	NounShip * pShip = pDoc->ship();
	if (! pShip )
		return;
	WindowStyle * pStyle = windowStyle();
	ASSERT( pStyle );
	Font * pFont = pStyle->font();
	ASSERT( pFont );

	// get a pointer to our gadget
	Noun * pContact = m_Noun;
	if (! pContact )
		return;

	RectInt iconBox( PointInt( window.left, window.top ), SizeInt( 16, 16 ) );

	Color iconColor( YELLOW );
	if ( pShip->isFriend( pContact ) )
		iconColor = GREEN;
	else if ( pShip->isEnemy( pContact ) )
		iconColor = RED;

	// draw the gadget icon
	Material::push( context, m_Icon );
	PrimitiveWindow::push( pDisplay, iconBox, WINDOW_UV, iconColor );

	if ( WidgetCast<NounShip>( pContact ) 
		&& ((NounShip *)pContact)->canOrder( pShip ) )
	{
		Material::push( context, WidgetCast<Material>( resource("team") ) );
		iconBox += PointInt( 16, 0 );
		PrimitiveWindow::push( pDisplay, iconBox, WINDOW_UV, WHITE );
	}
	else if ( m_IsObjective )
	{
		Material::push( context, WidgetCast<Material>( resource("objective") ) );
		iconBox += PointInt( 16, 0 );
		PrimitiveWindow::push( pDisplay, iconBox, WINDOW_UV, WHITE );
	}

	// display status text
	CharString sStatus = pContact->displayName( false );
	if ( m_HotKey != 0 )
		sStatus = CharString().format("%c:%s", m_HotKey, sStatus );
	if ( sStatus.length() > 0 )
	{
		SizeInt stringSize( pFont->size( sStatus ) );
		// make sure the text fits on the label
		while( stringSize.width > (BUTTON_WIDTH - 5 ) )
		{
			// remove the right most character and check the width again
			sStatus.left( sStatus.length() - 1 );
			stringSize = pFont->size( sStatus );
		}

		PointInt stringPos( window.m_Right - stringSize.width, window.m_Bottom - stringSize.height );
		Font::push( pDisplay, pFont, stringPos, sStatus, m_bGroupLeader ? YELLOW : (m_bGroupPending ? GREY : WHITE) );
	}

	// display the damage bar
	if ( WidgetCast<NounShip>( pContact ) )
	{
		if ( ((NounShip *)pContact)->damage() > 0 )
		{
			float damage = ((NounShip *)pContact)->damageRatioInv();
			RectInt bar( window.m_Left, window.m_Bottom + 1, 
				window.m_Right - (window.width() * (1.0f - damage)), window.m_Bottom + 3 );

			Color barColor( 255 * (1.0f - damage), 255 * damage,0,255 );
			PrimitiveMaterial::push( pDisplay, PrimitiveMaterial::NONE );
			PrimitiveWindow::push( pDisplay, bar, WINDOW_UV, barColor );
		}
	}

	// render additional border if this contact is our current target
	if ( pDoc->rootTarget() == m_Noun && (pDoc->tick() % 10) < 6 )
		renderGlow( context );
}

bool ButtonContact::onCursorMove( const RectInt & window, const PointInt & position, const PointInt & delta )
{
	if ( WindowButton::onCursorMove( window, position, delta ) )
	{
		if ( m_Noun.valid() )
		{
			((GameDocument *)document())->setCursorTarget( m_Noun );
			if ( ViewGame::sm_pViewGame->updateAction( true ) )
				setTip( "" );					// clear the button tip
			else
				setTip( m_Noun->displayName( false ) );		// restore the tool tip then..

			WindowNavigation::sm_bStickyCursorTarget = true;
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------------

void ButtonContact::onButtonDown()
{
	WindowButton::onButtonDown();

	// set the current target when button depressed
	((GameDocument *)document())->setTarget( m_Noun );
	((GameDocument *)document())->setCursorTarget( m_Noun );
	ViewGame::sm_pViewGame->updateAction( true );
}

void ButtonContact::onButtonUp()
{
	WindowButton::onButtonUp();
}

void ButtonContact::onMenuUp()
{
	ViewGame::sm_pViewGame->doAction( true );
}

//----------------------------------------------------------------------------

void ButtonContact::setButton( Noun * pNoun )
{
	ASSERT( pNoun != NULL );

	m_Flags |= NOCLIP;
	m_Style = HAS_BACK | EFFECT_HIGHLIGHT | SMOOTH_LL;
	//m_Alpha = 0.75f;

	// set the window name to the same as our contact
	setName( pNoun->name() );

	// set the noun
	m_Noun = pNoun;
	if ( WidgetCast<NounShip>( pNoun ) )
		setIcon( WidgetCast<Material>( resource( NounShip::typeText( ((NounShip *)pNoun)->type() ) ) ) );
	else if ( WidgetCast<NounPlanet>( pNoun ) )
		setIcon( WidgetCast<Material>( resource( "PLANET" ) ) );
	else if ( WidgetCast<NounStar>( pNoun ) )
		setIcon( WidgetCast<Material>( resource( "STAR" ) ) );
	else if ( WidgetCast<NounNebula>( pNoun ) )
		setIcon( WidgetCast<Material>( resource( "NEBULA" ) ) );
	else if ( WidgetCast<NounAsteroid>( pNoun ) )
		setIcon( WidgetCast<Material>( resource( "ASTEROID" ) ) );
	else if ( WidgetCast<NounTarget>( pNoun ) )
		setIcon( WidgetCast<Material>( resource( "TARGET" ) ) );
	else if ( WidgetCast<NounJumpGate>( pNoun ) )
		setIcon( WidgetCast<Material>( resource( "JUMPGATE" ) ) );
	else if ( WidgetCast<NounProjectile>( pNoun ) )
		setIcon( WidgetCast<Material>( resource( "PROJECTILE" ) ) );
	else
		setIcon( WidgetCast<Material>( resource( "UNKNOWN" ) ) );

	// determine if this contact is an objective!
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );
	GameContext * pContext = pDoc->context();
	ASSERT( pContext );
	GameClient * pClient = pDoc->client();
	ASSERT( pClient );

	int myFaction = pDoc->factionId();

	m_IsObjective = false;
	for(int i=0;i<pContext->conditionCount();i++)
	{
		const GameContext::Condition & condition = pContext->conditionN( i );
		for(int j=0;j<condition.objectives.size();j++)
			if ( condition.objectives[j].noun == m_Noun->key() && condition.objectives[j].factionId == myFaction )
			{
				m_IsObjective = true;
				m_Objective = (ObjectiveType)condition.objectives[j].type;
				break;
			}

		if ( m_IsObjective )
			break;
	}

	if ( pNoun->userId() != 0 )
	{
		Group * pGroup = pClient->group();
		if ( pGroup != NULL )
		{
			m_bGroupLeader = pGroup->isLeader( pNoun->userId() );
			m_bGroupPending = pGroup->isPending( pNoun->userId() );
		}
	}

	// set the tool tip
	CharString tip( pNoun->name() );
	if ( m_IsObjective )
		tip += CharString().format(" / Objective: %s", GameContext::objectiveText( m_Objective ) );

	setTip( tip );
}

void ButtonContact::setIcon( Material * pIcon )
{
	m_Icon = pIcon;
}

//----------------------------------------------------------------------------
//EOF
