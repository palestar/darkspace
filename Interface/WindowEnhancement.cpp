/*
	WindowEnhancement.h

	(c)2008 Palestar, Jeff Everett
*/

#include "Debug/Assert.h"
#include "Interface/ButtonEnhancement.h"
#include "Interface/WindowEnhancement.h"
#include "Interface/GameDocument.h"
#include "Gui3d/WindowText.h"
#include "DarkSpace/NounShip.h"

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowEnhancement, NodeWindow )

WindowEnhancement::WindowEnhancement()
{}

void WindowEnhancement::onUpdate( float t )
{
	GameDocument * pDoc = WidgetCast<GameDocument>( document() );

	NounShip *pShip = pDoc->ship();
	if(pShip)
	{
		// go through the enhancements attached to this ship and add the info to the 
		// enhancement window, so that when it is showed, players can view the details
		int nEnhCount = 0;

		// reset the totals
		CharString sText;
		for(int i=0;i<pShip->childCount();i++)
		{
			BaseNode * pChild = pShip->child(i);

			NounEnhancement *pEnh = WidgetCast<NounEnhancement>( pChild );
			if ( pEnh && nEnhCount < NounEnhancement::MAX_ENHANCEMENTS_PER_SHIP)
			{
				ButtonEnhancement * pEnhButton = m_pEnhancementButtons[ nEnhCount++ ];
				if (! pEnhButton )
					break;

				if ( m_pSelectedButton != NULL && m_pSelectedButton->enhancement() == pEnh )
					sText = pEnh->status();

				pEnhButton->showWindow(true);
				pEnhButton->setTip( pEnh->status() ); //CharString().format("<color;%6.6x>%s</color>",pEnh->color().RGB(),pEnh->getName()));
				pEnhButton->setEnhancement(pEnh);
				pEnhButton->setEnhancementWindow(this);
				pEnhButton->setIcon(pEnh->icon());
				pEnhButton->setColor( m_pSelectedButton == pEnhButton ? WHITE : GREY );
			}
		}

		// if no enhancement is selected, then build a summary of all ship modifiers..
		if ( m_pSelectedButton == NULL )
		{
			for( int i=0;i<MT_COUNT;++i)
			{
				int nValue = pShip->getModifier( (ModifierType)i );
				int nClamp = pShip->modifierTypeClamp( (ModifierType)i );
				if ( nClamp != 0 && (pShip->flags() & NounShip::FLAG_ADMIN) == 0 )
					nValue = Clamp<int>( nValue, -nClamp, nClamp );

				if ( nValue != 0 )
				{
					if ( nValue > 0 )
						sText += CharString().format( "%s +%d%%\n", NounShip::modifierTypeText( (ModifierType)i ), nValue );
					else
						sText += CharString().format( "%s -%d%%\n", NounShip::modifierTypeText( (ModifierType)i ), nValue );
				}
			}
		}

		// make sure the rest of the buttons are hidden
		for(int i=nEnhCount;i<NounEnhancement::MAX_ENHANCEMENTS_PER_SHIP;i++)
			m_pEnhancementButtons[i]->showWindow(false);

		// display the text description of the enhancements or selected enhancement
		m_pWindowText->setText( sText );

		// show the remove ehancement button only if we have one selected..
		if ( m_pSelectedButton != NULL )
		{
			NounEnhancement * pEnh = m_pSelectedButton->enhancement();

			m_pRemoveEnhancementButton->showWindow( true );
			m_pScrapEnhancementButton->showWindow( true );

			int nRemoveCost = pEnh->calculateRemoveCost();
			if ( nRemoveCost > 0 )
				m_pRemoveEnhancementButton->setTip( CharString().format( "Remove '%s' for %s credits.", pEnh->getName(), FormatNumber<char,int>( nRemoveCost ) ) );
			else
				m_pRemoveEnhancementButton->setTip( CharString().format( "Remove '%s'.", pEnh->getName()) );
		}
		else
		{
			m_pRemoveEnhancementButton->showWindow( false );
			m_pScrapEnhancementButton->showWindow( false );
		}
	}

	NodeWindow::onUpdate( t );
}

void WindowEnhancement::onActivate()
{
	NodeWindow::onActivate();

	for(int i=0;i<12;i++)
	{
		CharString buttonName;
		buttonName = CharString().format("ButtonEnh_%02d",i+1);
		m_pEnhancementButtons[i] = WidgetCast<ButtonEnhancement>( findNode( buttonName ) );
		// 
		m_pEnhancementButtons[i]->showWindow(false);
	}

	m_pWindowText = WidgetCast<WindowText>(findNode("InfoBox"));
	m_pRemoveEnhancementButton = WidgetCast<WindowButton>(findNode("ButtonRemoveEnhancement"));
	m_pScrapEnhancementButton = WidgetCast<WindowButton>(findNode("ButtonScrapEnhancement"));
	m_pSelectedButton = NULL;
}


//----------------------------------------------------------------------------
// EOF
