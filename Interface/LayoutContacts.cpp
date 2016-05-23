/*
	LayoutContacts.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Standard/Settings.h"
#include "DarkSpace/NounProjectile.h"
#include "DarkSpace/NounAsteroid.h"
#include "DarkSpace/NounCargo.h"
#include "DarkSpace/NounShip.h"
#include "DarkSpace/NounPod.h"
#include "World/NounTarget.h"
#include "Interface/GameDocument.h"
#include "Interface/ButtonContact.h"
#include "Interface/LayoutContacts.h"

//----------------------------------------------------------------------------

static Constant MAX_NAVIGATION_CONTACT_RANGE( "MAX_NAVIGATION_CONTACT_RANGE", 250000.0f );
static Constant NAV_UPDATE_RATE( "NAV_UPDATE_RATE", 1.0f );

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutContacts, WindowLayout );
REGISTER_FACTORY_KEY( LayoutContacts, 4242551209511067324 );

LayoutContacts::LayoutContacts() : m_Update( 0 ), m_bShowAI( true )
{}

//----------------------------------------------------------------------------

void LayoutContacts::onActivate()
{
	detachAllNodes();
	m_Update = 0.0f;

	Settings settings( "Options" );
	m_bShowAI = settings.get( "showAI", 1 ) != 0;
}

void LayoutContacts::onDeactivate()
{
	detachAllNodes();
}

//----------------------------------------------------------------------------

void LayoutContacts::onUpdate( float t )
{
	WindowLayout::onUpdate( t );

	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	ASSERT( pDoc );
	NounShip * pShip = pDoc->ship();
	if (! pShip )
		return;
	m_pContext = pDoc->context();
	if (! m_pContext )
		return;

	m_Update -= t;
	if ( m_Update <= 0.0f )		
	{
		m_Update = NAV_UPDATE_RATE;

		// hash all contacts by their key
		Hash< WidgetKey, Noun * > contactHash;
		// get the players team
		int nFactionId = pDoc->factionId();

		// build a hash of all the buttons..
		Hash< WidgetKey, ButtonContact * > buttonHash;
		for(int i=0;i<childCount();i++)
		{
			// get the button
			ButtonContact * pButton = WidgetCast<ButtonContact>( child(i) );
			if (! pButton || !pButton->noun() )
				continue;
			buttonHash[ pButton->noun()->key() ] = pButton;
		}

		for(int j=-1;j<m_pContext->worldCount();++j)
		{
			WorldContext * pWorld = j >= 0 ? m_pContext->world(j) : m_pContext;

			// find all contacts within a certain radius of our current target.
			for(int z=0;z<pWorld->zoneCount();z++)
			{
				NodeZone * pZone = pWorld->zone( z );
				for(int i=0;i<pZone->childCount();i++)
				{
					NounGame * pNoun = WidgetCast<NounGame>( pZone->child(i) );
					if ( pNoun != NULL && pNoun->isDetected( nFactionId ) && isNavigatonContact( pNoun ) )
					{
						contactHash[ pNoun->key() ] = pNoun;
						if ( !buttonHash.find( pNoun->key() ).valid() )
							new ButtonContact( this, pNoun );
					}
				}
			}
		}

		// remove buttons for removed contacts
		for(int i=0;i<childCount();i++)
		{
			// get the button
			ButtonContact * pButton = WidgetCast<ButtonContact>( child(i) );
			if (! pButton )
				continue;
			if ( !pButton->noun() || !contactHash.find( pButton->noun()->key() ).valid() )
				pButton->destroy();
		}
		
		sortButtons();
	}
}

//----------------------------------------------------------------------------

void LayoutContacts::doLayout()
{}

//----------------------------------------------------------------------------

const int MAX_BUTTONS = 30;

void CullButtons( Array< ButtonContact *> & buttons, const Vector3 & myPosition )
{
	// set all buttons visible
	for(int i=0;i<buttons.size();i++)
		buttons[i]->showWindow();


	int count = buttons.size();
	while( count > MAX_BUTTONS )
	{
		// enumerate through all the buttons and hide the buttons for objects too far away
		float	hideDistance = 0.0f;
		int		hideButton = -1;
		
		for(int i=0;i<buttons.size();i++)
		{
			if (! buttons[i]->visible() )
				continue;		// button is already hidden!
			if ( buttons[i]->isObjective() )
				continue;		// always show objectives
			if ( !buttons[i]->noun() )
				continue;

			float distance = (buttons[i]->noun()->worldPosition() - myPosition).magnitude();
			if ( distance > hideDistance )
			{
				hideButton = i;
				hideDistance = distance;
			}
		}

		if ( hideButton < 0 )
			break;

		buttons[ hideButton ]->hideWindow();
		count--;
	}
}

void LayoutContacts::setShowAI( bool bShowAI )
{
	if ( m_bShowAI != bShowAI )
	{
		m_bShowAI = bShowAI;

		Settings settings( "Options" );
		settings.put( "showAI", m_bShowAI );

		// force a update next onUpdate() call..
		m_Update = 0.0f;		
	}
}

void LayoutContacts::sortButtons()
{
	if ( childCount() == 0 )
		return;

	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	if ( pDoc == NULL )
		return;
	
	NounShip * pMyShip = pDoc->ship();
	ASSERT( pMyShip );

	Array< ButtonContact * > general;
	Array< ButtonContact * > enemy;
	Array< ButtonContact * > friendly;

	for(int i=0;i<childCount();i++)
	{
		ButtonContact * pButton = WidgetCast<ButtonContact>( child(i) );
		if (! pButton )
			continue;
		Noun * pContact = pButton->noun();
		if (! pContact )
			continue;

		if ( WidgetCast<NounShip>( pContact ) )
		{
			NounShip * pShip = (NounShip *)pContact;
			if ( pMyShip->isFriend( pContact ) )
			{
				// insert into friendly array, sorted by ship type
				bool inserted = false;
				for(int i=0;i<friendly.size() && !inserted;i++)
					if ( (((NounShip *)friendly[i]->noun())->type() > pShip->type()) ||
						(((NounShip *)friendly[i]->noun())->type() == pShip->type() && 
						_stricmp( pContact->name(), friendly[i]->noun()->name() ) > 0 ) )
					{
						friendly.insert( i, pButton );
						inserted = true;
					}

				if (!inserted)
					friendly.push( pButton );		// add to the end
			}
			else
			{
				// insert into enemy array, sorted by ship type
				bool inserted = false;
				for(int i=0;i<enemy.size() && !inserted;i++)
					if ( ((((NounShip *)enemy[i]->noun())->type() > pShip->type()) ||
						(((NounShip *)enemy[i]->noun())->type() == pShip->type() && 
						_stricmp( pContact->name(), enemy[i]->noun()->name() ) > 0 ) ) && 
						!((NounShip *)enemy[i]->noun())->testFlags( NounShip::FLAG_CLOAKED ))
					{
						enemy.insert( i, pButton );
						inserted = true;
					}

				if (!inserted)
					enemy.push( pButton );		// add to the end
			}
		}
		else
		{
			bool inserted = false;
			for(int i=0;i<general.size() && !inserted;i++)
				if ( pButton->isObjective() < general[i]->isObjective() || 
					(pButton->isObjective() == general[i]->isObjective() && 
					_stricmp( pContact->name(), general[i]->noun()->name() ) > 0 ) )
				{
					general.insert( i, pButton );
					inserted = true;
				}

			if (!inserted )
				general.push( pButton );
		}
	}

	Vector3 myPosition( pMyShip->worldPosition() );
	if ( pDoc->target() != NULL )
		myPosition = pDoc->target()->worldPosition();

	CullButtons( general, myPosition );
	CullButtons( friendly, myPosition );
	CullButtons( enemy, myPosition );

	RectInt localWindow( window() - window().upperLeft() );
	SizeInt localSize( localWindow.size() );

	int x = 0;
	int y = localSize.height;		// assume top justified
	int w = localSize.width / 3;
	int ww = w;
	int yy = 0;

	for(int i=0;i<general.size();i++)
	{
		ButtonContact * pButton = general[i];
		if (! pButton->visible() )
			continue;

		SizeInt buttonSize( pButton->window().size() );
		yy = Max( buttonSize.height, yy );

		if ( (x + buttonSize.width ) > ww )
		{
			// move one line up
			x = ww - w;
			y -= (yy + 4);
			yy = 0;
		}

		RectInt newWindow( PointInt( x, y - buttonSize.height ), buttonSize );
		pButton->setWindow( newWindow );
		x += buttonSize.width + 4;
	}

	ww += w;
	x = ww - w;
	y = localSize.height;

	for(int i=0;i<enemy.size();i++)
	{
		ButtonContact * pButton = enemy[i];
		if (! pButton->visible() )
			continue;
		
		SizeInt buttonSize( pButton->window().size() );
		yy = Max( buttonSize.height, yy );

		if ( (x + buttonSize.width ) > ww )
		{
			// move one line up
			x = ww - w;
			y -= (yy + 4);
			yy = 0;
		}

		RectInt newWindow( PointInt( x, y - buttonSize.height ), buttonSize );
		pButton->setWindow( newWindow );
		x += buttonSize.width + 4;
	}

	ww += w;
	x = ww - w;
	y = localSize.height;

	for(int i=0;i<friendly.size();i++)
	{
		ButtonContact * pButton = friendly[i];
		if (! pButton->visible() )
			continue;
		
		SizeInt buttonSize( pButton->window().size() );
		yy = Max( buttonSize.height, yy );

		if ( (x + buttonSize.width ) > ww )
		{
			// move one line up
			x = ww - w;
			y -= (yy + 4);
			yy = 0;
		}

		RectInt newWindow( PointInt( x, y - buttonSize.height ), buttonSize );
		pButton->setWindow( newWindow );
		x += buttonSize.width + 4;
	}
}

//---------------------------------------------------------------------------------------------------

bool LayoutContacts::isNavigatonContact( Noun * pNoun )
{
	if ( WidgetCast<NounProjectile>( pNoun ) )
		return false;
	if ( WidgetCast<NounAsteroid>( pNoun ) )
		return false;
	if ( WidgetCast<NounPod>( pNoun ) )
		return false;
	if ( WidgetCast<NounTarget>( pNoun ) )
		return false;
	if ( WidgetCast<NounShip>( pNoun ) && (!m_bShowAI && ((NounShip *)pNoun)->userId() == 0) )
		return false;		// do not display AI ships
	if ( WidgetCast<NounCargo>( pNoun ) )
		return false;

	return true;
}

//----------------------------------------------------------------------------
//EOF
