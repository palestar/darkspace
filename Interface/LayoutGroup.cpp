/*
	LayoutGroup.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "World/NounUserTarget.h"
#include "Interface/GameDocument.h"
#include "Interface/LayoutGroup.h"
#include "Interface/ButtonContact.h"

//----------------------------------------------------------------------------

template<class T>
T * GetChild( NodeWindow * pWindow, int i )
{
	if ( i < pWindow->childCount() )
		return WidgetCast<T>( pWindow->child(i) );

	return NULL;
}

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( LayoutGroup, WindowLayout );
REGISTER_FACTORY_KEY( LayoutGroup, 4380230593850490619 );

LayoutGroup::LayoutGroup()
{}

//----------------------------------------------------------------------------

void LayoutGroup::onActivate()
{
	m_Update = 0.0f;
	detachAllNodes();
}

void LayoutGroup::onDeactivate()
{
	detachAllNodes();
	m_Update = 0.0f;
	m_DummyObjects.clear();
}

void LayoutGroup::onUpdate( float t )
{
	WindowLayout::onUpdate( t );

	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	ASSERT( pDoc );

	m_Update -= t;
	if ( m_Update <= 0.0f )
	{
		m_Update = 1.0f;

		int nButtonCount = 0;

		Group * pGroup = pDoc->client()->group();
		if ( pGroup != NULL )
		{
			for(int i=0;i<pGroup->memberCount();++i)
			{
				GroupMember * pMember = pGroup->member( i );

				Noun::wRef pMemberObject( pMember->objectId() );
				if (! pDoc->validateNoun( pMemberObject ) )
				{
					// no active object found, create a dummy object for now..
					Noun::Ref pDummyObject = new NounUserTarget( pMember->userId() );
					pDummyObject->setKey( pMember->objectId() );
					pDummyObject->setName( pMember->name() );
					m_DummyObjects.push_back( pDummyObject );

					pMemberObject = pDummyObject;
				}

				ButtonContact * pButton = GetChild<ButtonContact>( this, nButtonCount );
				if ( pButton != NULL )
					pButton->setButton( pMemberObject );
				else
					new ButtonContact( this, pMemberObject );

				nButtonCount++;
			}
		}

		// Include AI allies as well..
		NounShip * pShip = pDoc->ship();
		if ( pShip != NULL && pShip->validateAllyShips() > 0 )
		{
			for(int i=0;i<pShip->allyCount();++i)
			{
				NounShip * pAlly = pShip->ally( i );
				if(! pAlly )
					continue;

				ButtonContact * pButton = GetChild<ButtonContact>( this, nButtonCount );
				if ( pButton != NULL )
					pButton->setButton( pAlly );
				else
					new ButtonContact( this, pAlly );

				nButtonCount++;
			}
		}

		keepButtons( nButtonCount );
	}
}

//----------------------------------------------------------------------------

void LayoutGroup::keepButtons( int keep )
{
	for(int i=keep;i<childCount();i++)
		((WindowButton *)child(i))->destroy();
}

//----------------------------------------------------------------------------
//EOF
