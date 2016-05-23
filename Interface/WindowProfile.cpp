/*
	WindowProfile.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#include "Debug/Assert.h"
#include "Interface/GameDocument.h"
#include "Interface/WindowProfile.h"
#include "Standard/StringBuffer.h"

#include <map>
#include <string>

//----------------------------------------------------------------------------

const float PROFILE_UPDATE_RATE = 5.0f;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowProfile, WindowText );
REGISTER_FACTORY_KEY( WindowProfile, 4308562802021456953 );

WindowProfile::WindowProfile()
{}

//----------------------------------------------------------------------------

void WindowProfile::onUpdate( float t )
{
	WindowText::onUpdate( t );

	if ( visible() && activeTime() > PROFILE_UPDATE_RATE )
	{
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );

		GameProfile * pProfile = pDoc->profile();

		CharString profile;
		float fPrestige = pProfile->calculatePrestige();


		profile += CharString().format( "<color;ffffff>%s\n", pProfile->name() );
		profile += CharString().format("\nRank:<X;150>%s", GameProfile::rankText( pProfile->calculateRank() ) );
		
		if( pProfile->calculateRank() != CHIEF_MARSHAL )
			profile += CharString().format("\n<X;150>%.1f%% to %s\n", pProfile->calculateRankUpPercent() * 100,GameProfile::rankText( pProfile->calculateRank()+1 ) );
		
		profile += CharString().format("\nPrestige:<X;150>%.1f\n",fPrestige  );

		

		for(int i=FIRST_STATISTIC;i<=LAST_STATISTIC;i++)
			profile += CharString().format("\n%s:<X;150>%.1f", 
				GameProfile::statisticName( (Statistics)i ), (*pProfile)[ (Statistics)i ] );

		// use a map so we can sort the badges by name...
		std::map< std::string, std::string > medalMap;

		dword badges = pProfile->calculateBadges();
		for(int b=FIRST_BADGE;b<=LAST_BADGE;b++)
		{
			if ( ((1 << b) & badges) != 0 )
				medalMap[ GameProfile::badgeText( b ) ] = CharString().format("\n<Image;badge%2.2u>%s", b, GameProfile::badgeText( b ) );
		}
		dword medals = pProfile->calculateMedals( badges );
		for(int m=FIRST_MEDAL;m<=LAST_MEDAL;m++)
		{
			if ( ((1 << m) & medals) != 0 )
				medalMap[ GameProfile::medalText( m ) ] = CharString().format("\n<Image;medal%2.2u>%s", m, GameProfile::medalText( m ) );
		}

		for( std::map< std::string, std::string >::iterator iMedal = medalMap.begin(); iMedal != medalMap.end(); ++iMedal )
			profile += iMedal->second.c_str();

		setText( profile );
		setActiveTime( 0 );

		// request profile update for next time..
		pDoc->client()->sendProfile();
	}
}

bool WindowProfile::onRightDown( const RectInt & window,
						const PointInt & position )
{
	if ( window.inRect( position ) )
	{
		((NodeWindow *)parent())->hideWindow();
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
//EOF
