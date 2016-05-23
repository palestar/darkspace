/**
	@file GameProfile.h
	@brief TODO

	(c)2006 Palestar Inc
	@author Richard Lyle @date 10/18/2006 8:34:57 PM
*/

#ifndef GAMEPROFILE_H
#define GAMEPROFILE_H

#include "World/Profile.h"
#include "Constants.h"
#include "GameDll.h"

//---------------------------------------------------------------------------------------------------

class DLL GameProfile : public Profile
{
public:
	DECLARE_WIDGET_CLASS();

	// Construction
	GameProfile();

	// Profile interface
	virtual void		load( const MetaClient::Profile & profile );		// clears the time modified
	virtual void		save( MetaClient::Profile & profile);				// clears the time modified
	virtual int			spawnScore();

	// Accessors
	float				calculatePrestige() const;
	float				calculateRankUpPercent() const;
	Rank				calculateRank() const;
	Rank				getRank( float fPrestige ) const;
	dword				calculateBadges() const;
	dword				calculateMedals( dword badges ) const;

	float				operator[]( Statistics nStat ) const;

	// Static
	static const char * rankText( int rank );
	static const char * rankShortText( int rank );
	static const char * badgeText( int badge );
	static const char * medalText( int medal );
	static const char * statisticName( Statistics nStat );
	static float		calculateModifier( Statistics nStat );
};

//---------------------------------------------------------------------------------------------------

inline float GameProfile::operator[]( Statistics nStat ) const
{
	return Profile::operator[]( statisticName( nStat ) );
}

//---------------------------------------------------------------------------------------------------

#endif

//---------------------------------------------------------------------------------------------------
//EOF
