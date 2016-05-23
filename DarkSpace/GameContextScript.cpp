/**
	@file GameContextScript.cpp

	(c)2006 Palestar Inc
	@author Richard Lyle @date 10/18/2006 10:02:33 PM
*/

#include "GameContext.h"
#include "NounStar.h"
#include "NounAsteroid.h"
#include "NounNebula.h"
#include "NounUnit.h"
#include "NounBeacon.h"
#include "NounProjectile.h"
#include "NounJumpGate.h"
#include "GadgetArmor.h"
#include "GadgetShield.h"
#include "GadgetBeamWeapon.h"
#include "VerbEnterOrbit.h"
#include "VerbBreakOrbit.h"
#include "VerbPlanetEvent.h"
#include "VerbRevolt.h"
#include "VerbOrderUnit.h"

#include "Math/Helpers.h"

#include "World/LuaHeaders.h"

#ifdef USE_PLAIN_LUA
	#ifdef _DEBUG
		#pragma comment( lib, "../../Medusa/ThirdParty/Lua51/bin/Lua51D.lib")
	#else
		#pragma comment( lib, "../../Medusa/ThirdParty/Lua51/bin/Lua51.lib")
	#endif
#else
	#include "../../Medusa/ThirdParty/LuaJIT/jit/opt.h"
	#include "../../Medusa/ThirdParty/LuaJIT/jit/opt_inline.h"

	#ifdef _DEBUG
		#pragma comment( lib, "../../Medusa/ThirdParty/LuaJIT/bin/Lua51D.lib")
	#else
		#pragma comment( lib, "../../Medusa/ThirdParty/LuaJIT/bin/Lua51.lib")
	#endif
#endif

//---------------------------------------------------------------------------------------------------

int GameContext::isStar( lua_State * pScript )
{
	if ( lua_gettop( pScript ) == 1 )
	{
		NounStar * pNoun = WidgetCast<NounStar>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		lua_pushboolean( pScript, pNoun != NULL );
		return 1;
	}

	Alert( pScript, "isStar() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::isAsteroid( lua_State * pScript )
{
	if ( lua_gettop( pScript ) == 1 )
	{
		NounAsteroid * pNoun = WidgetCast<NounAsteroid>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		lua_pushboolean( pScript, pNoun != NULL );
		return 1;
	}

	Alert( pScript, "isAsteroid() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::isNebula( lua_State * pScript )
{
	if ( lua_gettop( pScript ) == 1 )
	{
		NounNebula * pNoun = WidgetCast<NounNebula>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		lua_pushboolean( pScript, pNoun != NULL );
		return 1;
	}

	Alert( pScript, "isNebula() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::isBody( lua_State * pScript )
{
	if ( lua_gettop( pScript ) == 1 )
	{
		NounBody * pNoun = WidgetCast<NounBody>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		lua_pushboolean( pScript, pNoun != NULL );
		return 1;
	}

	Alert( pScript, "isBody() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::isUnit( lua_State * pScript )
{
	if ( lua_gettop( pScript ) == 1 )
	{
		NounUnit * pNoun = WidgetCast<NounUnit>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		lua_pushboolean( pScript, pNoun != NULL );
		return 1;
	}

	Alert( pScript, "isUnit() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::isPlanet( lua_State * pScript )
{
	if ( lua_gettop( pScript ) == 1 )
	{
		NounPlanet * pNoun = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		lua_pushboolean( pScript, pNoun != NULL );
		return 1;
	}

	Alert( pScript, "isPlanet() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::isOrbiting( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounBody * pNoun1 = WidgetCast<NounBody>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		Noun * pNoun2 = GetNoun( pScript, lua_tostring( pScript, 2 ) );
		if ( pNoun1 != NULL && pNoun2 != NULL )
		{
			lua_pushboolean( pScript, pNoun1->orbiting() == pNoun2 );
			return 1;
		}
		else
			sErrorDetail = "NounBody not found.";
	}
	else
		sErrorDetail = "This function needs 2 parameters.";

	Alert( pScript, "isOrbiting() error! ", sErrorDetail );
	return 0;
}

int GameContext::enterOrbit( lua_State * pScript )
{
	CharString sErrorDetail;
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 3 && pContext->isServer() )
	{
		NounBody * pBody = WidgetCast<NounBody>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		NounBody * pOrbit = WidgetCast<NounBody>( GetNoun( pScript, lua_tostring( pScript, 2 ) ) );
		float velocity = lua_tonumber( pScript, 3 );

		if ( pBody != NULL && pOrbit != NULL )
		{
			Verb::Ref( new VerbEnterOrbit( pBody, pOrbit, velocity ) );
			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "NounBody not found.";
	}
	else
		sErrorDetail = "This function needs 3 parameters and to be executed serverside.";

	Alert( pScript, "enterOrbit() error! ", sErrorDetail );
	return 0;
}

int GameContext::breakOrbit( lua_State * pScript )
{
	CharString sErrorDetail;
	WorldContext * pContext = GetContext( pScript );
	if ( lua_gettop( pScript ) == 1 && pContext->isServer() )
	{
		NounBody * pBody = WidgetCast<NounBody>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pBody != NULL )
		{
			Verb::Ref( new VerbBreakOrbit( pBody ) );
			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "NounBody not found.";
	}
	else
		sErrorDetail = "This function needs 1 parameter and to be executed serverside.";

	Alert( pScript, "breakOrbit() error! ", sErrorDetail );
	return 0;
}

int GameContext::isBeaconed( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounBody * pBody = WidgetCast<NounBody>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pBody != NULL )
		{
			for(int i=0;i<pBody->childCount();i++)
			{
				NounBeacon * pBeacon = WidgetCast<NounBeacon>( pBody->child(i) );
				if ( pBeacon != NULL )
				{
					lua_pushboolean( pScript, true );
					return 1;
				}
			}
			
			lua_pushboolean( pScript, false );
			return 1;
		}
		else
			sErrorDetail = "NounBody not found.";
	}
	else
		sErrorDetail = "This function needs 1 parameter.";

	Alert( pScript, "isBeaconed() error! ", sErrorDetail );
	return 0;
}

int GameContext::canDetect( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		Noun * pDetector = GetNoun( pScript, lua_tostring( pScript, 1 ) );
		if( pDetector )
		{
			Noun * pTarget = GetNoun( pScript, lua_tostring( pScript, 2 ) );
			if( pTarget )
			{
				NounShip * pSelf = WidgetCast<NounShip>( pDetector );
				if ( pSelf != NULL )
				{
					lua_pushboolean( pScript, pSelf->canDetect( pTarget ) );
					return 1;
				}

				NounPlanet * pPlanet = WidgetCast<NounPlanet>( pDetector );
				if ( pPlanet != NULL )
				{
					lua_pushboolean( pScript, pPlanet->canDetect( pTarget ) );
					return 1;
				}
				
				sErrorDetail = "Detector must be a ship or a planet.";
			}
			else
				sErrorDetail = "Target not found.";
		}
		else
			sErrorDetail = "Detector (Ship or Planet) not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "canDetect() error! ", sErrorDetail );
	return 0;
}

int GameContext::getAllNouns( lua_State * pScript )	// getAllNouns( nounId, range ) returns a table 1..n
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		Noun * pNoun = GetNoun( pScript, lua_tostring( pScript, 1) );
		if ( pNoun != NULL )
		{
			WorldContext * pContext = GetContext( pScript );
			int range = lua_tonumber( pScript, 2 );
			
			int itemCount = 0;
			lua_newtable( pScript );

			Array< WorldContext::NounCollision > nouns;
			if ( pContext->proximityCheck( pNoun->worldPosition(), range, nouns ) )
			{	
				for(int i=0;i<nouns.size();i++)
				{
					Noun * pNoun = nouns[i].pNoun;

					lua_pushnumber( pScript, ++itemCount );				// index ( 1 .. n )
					lua_pushstring( pScript, CharString(pNoun->key().string()) );	// value ( nounId )
					lua_rawset( pScript, -3 );
				}
			}
			
			// set total number of elements contained
			lua_pushliteral( pScript, "n" );
			lua_pushnumber( pScript, itemCount );
			lua_rawset( pScript, -3 );

			return 1;	// return the table

		}
		else
			sErrorDetail = "Noun not found.";
	}
	else
		sErrorDetail = "This function needs 2 parameters.";

	Alert( pScript, "getAllNouns() error! ", sErrorDetail );
	return 0;
}

int GameContext::getAllNounsRelative( lua_State * pScript )	// getAllNounsRelative( shipId, range ) returns a table 1..n
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pSelf != NULL )
		{
			WorldContext * pContext = GetContext( pScript );
			int range = lua_tonumber( pScript, 2 );
			
			int itemCount = 0;
			lua_newtable( pScript );

			Array< WorldContext::NounCollision > nouns;
			if ( pContext->proximityCheck( pSelf->worldPosition(), range, nouns ) )
			{	
				float fHeading = -pSelf->heading();	// negate heading, as the objects around the ship are to be
				float fCos = cosf( fHeading );		// rotated to relative heading 0
				float fSin = sinf( fHeading );		

				for(int i=0;i<nouns.size();i++)
				{
					Noun * pNoun = nouns[i].pNoun;

					lua_pushnumber( pScript, ++itemCount );				// index ( 1 .. n )

					// table2 (value), containing key, x and z position
					lua_newtable( pScript );

					lua_pushliteral( pScript, "key" );
					lua_pushstring( pScript, CharString(pNoun->key().string()) );
					lua_rawset( pScript, -3 );
					
					// get the nouns relative coordinates and rotate it to fit the ships heading
					Vector3 vRelPos( pNoun->worldPosition() - pSelf->worldPosition() );
					
					float fRelPosZ = vRelPos.z * fCos - vRelPos.x * fSin;
					float fRelPosX = vRelPos.x * fCos + vRelPos.z * fSin;

					lua_pushliteral( pScript, "z" );
					lua_pushnumber( pScript, fRelPosZ );
					lua_rawset( pScript, -3 );

					lua_pushliteral( pScript, "x" );
					lua_pushnumber( pScript, fRelPosX );
					lua_rawset( pScript, -3 );

					// How fast is the object coming towards the center (0/0/0) ?
					// a projectiles worldvelocity is always zero, thus velocity() has to be used
					Vector3 vRelVel;
					NounProjectile * pNounProjectile = WidgetCast<NounProjectile>( pNoun );
					if( pNounProjectile )
						vRelVel = pNounProjectile->velocity();
					else
						vRelVel = pNoun->worldVelocity();
					
					float fRelVelocity = vRelPos.magnitude() - ( vRelPos + vRelVel - pSelf->worldVelocity() ).magnitude();

					lua_pushliteral( pScript, "vel" );
					lua_pushnumber( pScript, fRelVelocity );	// positive velocity -> moving towards the center
					lua_rawset( pScript, -3 );

					// end of table 2

					lua_rawset( pScript, -3 );
				}
			}
			
			// set total number of elements contained
			lua_pushliteral( pScript, "n" );
			lua_pushnumber( pScript, itemCount );
			lua_rawset( pScript, -3 );

			return 1;	// return the table

		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "This function needs 2 parameters.";

	Alert( pScript, "getAllNounsRelative() error! ", sErrorDetail );
	return 0;
}

//----------------------------------------------------------------------------
// PLANET FUNCTIONS

int GameContext::planetCount( lua_State * pScript )		// planetCount()
{
	WorldContext * pContext = GetContext( pScript );

	int planets = 0;
	for(int j=0;j<pContext->zoneCount();j++)
	{
		BaseNode * pRoot = pContext->zone( j );
		for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
			if ( WidgetCast<NounPlanet>( pRoot->child(i) ) )
				planets++;
	}

	lua_pushnumber( pScript, planets );
	return 1;
}

int GameContext::getPlanet( lua_State * pScript )			// getPlanet( n )
{
	CharString sErrorDetail;
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 1 )
	{
		int planet = lua_tonumber( pScript, 1 );
		int index = 0;

		for(int j=0;j<pContext->zoneCount();j++)
		{
			BaseNode * pRoot = pContext->zone( j );
			for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
				if ( WidgetCast<NounPlanet>( pRoot->child(i) ) )
				{
					if ( planet == index )
					{
						lua_pushstring( pScript, CharString(pRoot->child(i)->key().string()) );
						return 1;
					}

					index++;
				}
		}
		sErrorDetail = "Index out of range.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getPlanet() error! ", sErrorDetail );
	return 0;
}

int GameContext::getPlanetMorale( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			lua_pushnumber( pScript, pPlanet->moral() * 100 );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getPlanetMorale() error! ", sErrorDetail );
	return 0;
}

int GameContext::getPlanetPopulation( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			lua_pushnumber( pScript, pPlanet->population() );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getPlanetPopulation() error! ", sErrorDetail );
	return 0;
}

int GameContext::setPlanetPopulation( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pPlanet != NULL )
		{
			float number = lua_tonumber( pScript, 2 );
			pPlanet->setPopulation( number );

			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "NounPlanet not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "setPlanetPopulation() error! ", sErrorDetail );
	return 0;
}

int GameContext::getPlanetResources( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			lua_pushnumber( pScript, pPlanet->resourceCount() );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getPlanetResources() error! ", sErrorDetail );
	return 0;
}

int GameContext::setPlanetResources( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pPlanet != NULL )
		{
			float number = lua_tonumber( pScript, 2 );
			pPlanet->setResources( number );

			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "NounPlanet not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "setPlanetResources() error! ", sErrorDetail );
	return 0;
}

int GameContext::getPlanetTech( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			lua_pushnumber( pScript, pPlanet->technology() );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getPlanetTech() error! ", sErrorDetail );
	return 0;
}

int GameContext::setPlanetTech( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pPlanet != NULL )
		{
			float number = lua_tonumber( pScript, 2 );
			pPlanet->setTechnology( number );

			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "NounPlanet not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "setPlanetTech() error! ", sErrorDetail );
	return 0;
}

int GameContext::getPlanetValue( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			lua_pushnumber( pScript, pPlanet->value() );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getPlanetValue() error! ", sErrorDetail );
	return 0;
}

int GameContext::getPlanetFlags( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			lua_pushnumber( pScript, pPlanet->flags() );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getPlanetFlags() error! ", sErrorDetail );
	return 0;
}

int GameContext::planetEvent( lua_State * pScript )
{
	CharString sErrorDetail;
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 3 && pContext->isServer() )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			int event = lua_tonumber( pScript, 2 );
			int active = lua_tonumber( pScript, 3 );

			Verb::Ref( new VerbPlanetEvent( pPlanet, (VerbPlanetEvent::Event)event, active != 0) );
			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 3 parameters and to be executed serverside.";

	Alert( pScript, "planetEvent() error! ", sErrorDetail );
	return 0;
}

int GameContext::revoltPlanet( lua_State * pScript )
{
	CharString sErrorDetail;
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 2 && pContext->isServer() )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			int team = lua_tonumber( pScript, 2 );
			if ( team >= 0 && team < pContext->teamCount() )
			{
				Verb::Ref( new VerbRevolt( pPlanet, pContext->teamId( team ) ) );
				lua_pushboolean( pScript, true );
				return 1;
			}
			else
				sErrorDetail = "Team index out of range.";
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters and to be executed serverside.";

	Alert( pScript, "revoltPlanet() error! ", sErrorDetail );
	return 0;
}

int GameContext::hexCount( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			lua_pushnumber( pScript, pPlanet->hexCount() );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "hexCount() error! ", sErrorDetail );
	return 0;
}

int GameContext::getHex( lua_State * pScript )
{
	/*
	if ( lua_gettop( pScript ) == 2 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			int hex = lua_tonumber( pScript, 2 );
			if ( hex >= 0 && hex < pPlanet->hexCount() )
			{
				NounPlanet::Hex h = pPlanet->hex( hex );
				lua_pushstring( pScript, h.noun.valid() ? h.noun->name() : "" );
				return 1;
			}
		}
	}

	Alert( pScript, "getHex() error!"  );
	*/

	Alert( pScript, "getHex() error! Function is currently disabled." );
	return 0;
}

int GameContext::getHexPosition( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			int hex = lua_tonumber( pScript, 2 );
			if ( hex >= 0 && hex < pPlanet->hexCount() )
			{
				NounPlanet::Hex h = pPlanet->hex( hex );
				lua_pushnumber( pScript, h.position.x );
				lua_pushnumber( pScript, h.position.y );
				lua_pushnumber( pScript, h.position.z );
				return 3;
			}
			else
				sErrorDetail = "Hex index out of range.";
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "getHexPosition() error! ", sErrorDetail );
	return 0;
}

int GameContext::structureCount( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			lua_pushnumber( pScript, pPlanet->structureCount() );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "structureCount() error! ", sErrorDetail );
	return 0;
}

int GameContext::getStructure( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			int n = lua_tonumber( pScript, 2 );
			int count = 0;
			
			for(int i=0;i<pPlanet->childCount();i++)
			{
				NounStructure * pStructure = WidgetCast<NounStructure>( pPlanet->child(i) );
				if ( pStructure != NULL )
				{
					if ( count == n )
					{
						lua_pushstring( pScript, CharString(pStructure->key().string()) );
						return 1;
					}
					count++;
				}
			}
			
			sErrorDetail = "Structure index out of range.";
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "getStructure() error! ", sErrorDetail );
	return 0;
}

int GameContext::getStructureHex( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounStructure * pStructure = WidgetCast<NounStructure>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pStructure != NULL )
		{
			lua_pushnumber( pScript, pStructure->hex() );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getStructureHex() error! ", sErrorDetail );
	return 0;
}

int GameContext::getStructureHull( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounStructure * pStruc = WidgetCast<NounStructure>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pStruc != NULL )
		{
			float percentage = 100 - ( (float)pStruc->damage() / (float)pStruc->maxDamage() ) * 100;
			lua_pushnumber( pScript, percentage );
			return 1;
		}
		else
			sErrorDetail = "NounStructure not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getStructureHull() error! ", sErrorDetail );
	return 0;
}

int GameContext::setStructureHull( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounStructure * pStruc = WidgetCast<NounStructure>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pStruc != NULL )
		{
			int percentage = lua_tonumber( pScript, 2 );
			pStruc->setDamage( ( ( 100 - percentage ) * pStruc->maxDamage()) / 100 );

			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "NounStructure not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "setStructureHull() error! ", sErrorDetail );
	return 0;
}

int GameContext::isStructureActive( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounStructure * pStruc = WidgetCast<NounStructure>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pStruc != NULL )
		{
			lua_pushboolean( pScript, pStruc->active() );
			return 1;
		}
		else
			sErrorDetail = "NounStructure not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "isStructureActive() error! ", sErrorDetail );
	return 0;
}

int GameContext::isStructurePaused( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounStructure * pStruc = WidgetCast<NounStructure>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pStruc != NULL )
		{
			lua_pushboolean( pScript, pStruc->isPaused() );
			return 1;
		}
		else
			sErrorDetail = "NounStructure not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "isStructurePaused() error! ", sErrorDetail );
	return 0;
}

int GameContext::pauseStructure( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounStructure * pStruc = WidgetCast<NounStructure>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pStruc != NULL )
		{
			int enable = lua_toboolean( pScript, 2 );
			pStruc->setActive( !enable );
			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "NounStructure not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "isStructurePaused() error! ", sErrorDetail );
	return 0;
}

int GameContext::isStructureFinished( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounStructure * pStruc = WidgetCast<NounStructure>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pStruc != NULL )
		{
			lua_pushboolean( pScript, !pStruc->isBuilding() );
			return 1;
		}
		else
			sErrorDetail = "NounStructure not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "isStructureFinished() error! ", sErrorDetail );
	return 0;
}

int GameContext::unitCount( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			lua_pushnumber( pScript, pPlanet->unitCount() );
			return 1;
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "unitCount() error! ", sErrorDetail );
	return 0;
}

int GameContext::getUnit( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pPlanet != NULL )
		{
			int n = lua_tonumber( pScript, 2 );
			int count = 0;
			
			for(int i=0;i<pPlanet->childCount();i++)
			{
				NounUnit * pUnit = WidgetCast<NounUnit>( pPlanet->child(i) );
				if ( pUnit != NULL )
				{
					if ( count == n )
					{
						lua_pushstring( pScript, CharString(pUnit->key().string()) );
						return 1;
					}
					count++;
				}
			}
			sErrorDetail = "Structure index out of range.";
		}
		else
			sErrorDetail = "Planet not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "getUnit() error! ", sErrorDetail );
	return 0;
}

int GameContext::getUnitHex( lua_State * pScript )
{
	/*
	if ( lua_gettop( pScript ) == 1 )
	{
		NounUnit * pUnit = WidgetCast<NounUnit>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pUnit != NULL )
		{
			lua_pushnumber( pScript, pUnit->hex() );
			return 1;
		}
	}

	Alert( pScript, "getUnitHex() error!"  );
	*/
	Alert( pScript, "getUnitHex() error! Function is currently disabled."  );
	return 0;
}

int GameContext::getRandomPlanet( lua_State * pScript )
{
	WorldContext * pContext = GetContext( pScript );

	Array< NounPlanet * > planets;

	for(int j=0;j<pContext->zoneCount();j++)
	{
		BaseNode * pRoot = pContext->zone( j );
		for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
			if ( WidgetCast<NounPlanet>( pRoot->child(i) ) )
				planets.push( (NounPlanet *)pRoot->child(i) );
	}

	if ( planets.size() > 0 )
	{
		lua_pushstring( pScript, CharString(planets[ rand() % planets.size() ]->key().string()) );
		return 1;
	}
	return 0;
}

int GameContext::getFriendlyPlanet( lua_State * pScript )
{
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 1 )
	{
		int faction = lua_tonumber( pScript, 1 );

		Array< NounPlanet * > planets;

		for(int j=0;j<pContext->zoneCount();j++)
		{
			BaseNode * pRoot = pContext->zone( j );
			for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
			{
				NounPlanet * pPlanet = WidgetCast<NounPlanet>( pRoot->child(i) );
				if ( pPlanet != NULL && pPlanet->factionId() == faction )
					planets.push( pPlanet );
			}
		}

		if ( planets.size() > 0 )
		{
			lua_pushstring( pScript, CharString(planets[ rand() % planets.size() ]->key().string()) );
			return 1;
		}
		return 0;	// no alert() message, this is intentional
	}

	Alert( pScript, "getFriendlyPlanet() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::getEnemyPlanet( lua_State * pScript )
{
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 1 )
	{
		int faction = lua_tonumber( pScript, 1 );

		Array< NounPlanet * > planets;

		for(int j=0;j<pContext->zoneCount();j++)
		{
			BaseNode * pRoot = pContext->zone( j );
			for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
			{
				NounPlanet * pPlanet = WidgetCast<NounPlanet>( pRoot->child(i) );
				if ( pPlanet != NULL && pPlanet->factionId() != faction )
					planets.push( pPlanet );
			}
		}

		if ( planets.size() > 0 )
		{
			lua_pushstring( pScript, CharString(planets[ rand() % planets.size() ]->key().string()) );
			return 1;
		}
		return 0;	// no alert() message, this is intentional
	}

	Alert( pScript, "getEnemyPlanet() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::setAllegiance( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pPlanet != NULL )
		{
			float number = lua_tonumber( pScript, 2 );
			pPlanet->setAllegiance( number );

			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "NounPlanet not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "setAllegiance() error! ", sErrorDetail );
	return 0;
}

int GameContext::setAllegianceXP( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pPlanet != NULL )
		{
			float number = lua_tonumber( pScript, 2 );
			pPlanet->setAllegianceXP( number );

			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "NounPlanet not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "setAllegianceXP() error! ", sErrorDetail );
	return 0;
}

//----------------------------------------------------------------------------
// SHIP FUNCTIONS

int GameContext::isShip( lua_State * pScript )
{
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pNoun = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		lua_pushboolean( pScript, pNoun != NULL );
		return 1;
	}

	Alert( pScript, "isShip() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::isShipDestroyed( lua_State * pScript )
{
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if( pSelf != NULL )
		{
			lua_pushboolean( pScript, pSelf->isDestroyed() );
			return 1;
		}

		Alert( pScript, "isShipDestroyed() error! Parameter needs to be a ship." );
		return 0;
	}

	Alert( pScript, "isShipDestroyed() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::shipCount( lua_State * pScript )		// shipCount()
{
	WorldContext * pContext = GetContext( pScript );

	int count = 0;
	for(int j=0;j<pContext->zoneCount();j++)
	{
		NodeZone * pZone = pContext->zone( j );
		for(int i=0;pZone != NULL && i<pZone->childCount();i++)
			if ( WidgetCast<NounShip>( pZone->child(i) ) )
				count++;
	}

	lua_pushnumber( pScript, count );
	return 1;
}

int GameContext::getShip( lua_State * pScript )			// getShip( n )
{
	CharString sErrorDetail;
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 1 )
	{
		int ship = lua_tonumber( pScript, 1 );
		int index = 0;

		for(int j=0;j<pContext->zoneCount();j++)
		{
			NodeZone * pRoot = pContext->zone( j );
			for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
				if ( WidgetCast<NounShip>( pRoot->child(i) ) )
				{
					if ( ship == index )
					{
						lua_pushstring( pScript, CharString(pRoot->child(i)->key().string()) );
						return 1;
					}

					index++;
				}
		}
		sErrorDetail = "Index out of range.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getShip() error! ", sErrorDetail );

	return 0;
}

int GameContext::getHull( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			float percentage = pSelf->damageRatioInv() * 100;
			lua_pushnumber( pScript, percentage );
			return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getHull() error! ", sErrorDetail );
	return 0;
}

int GameContext::setHull( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			int percentage = lua_tonumber( pScript, 2 );
			pSelf->setDamage( ( ( 100 - percentage ) * pSelf->maxDamage()) / 100 );

			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "setHull() error! ", sErrorDetail );
	return 0;
}

int GameContext::getSignature( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			lua_pushnumber( pScript, pSelf->signature() );
			return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getSignature() error! ", sErrorDetail );
	return 0;
}

int GameContext::useGadget( lua_State * pScript )			// useGadget( ship, key, target, shift )
{
	int n = lua_gettop( pScript );
	if ( n != 4 )
	{
		Alert( pScript, "useGadget() error! Function needs 4 arguments."  );
		return 0;
	}

	NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
	if ( pSelf == NULL )
	{
		Alert( pScript, "useGadget() error! Failed to find ship."  );
		return 0;
	}

	const char *	pKey = lua_tostring( pScript, 2 );
	Noun *			pTarget = GetNoun( pScript, lua_tostring( pScript, 3 ) );
	int				shift = lua_tonumber( pScript, 4 );

	int count = 0;
	for(int i=0;i<pSelf->childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( pSelf->child(i) );
		if ( pGadget == NULL )
			continue;

		if ( pGadget->hotkey() == (dword)pKey[ 0 ] )
		{
			pSelf->useGadget( pGadget, pTarget, shift != 0 );
			count++;
		}
	}

	// use the gadget
	lua_pushnumber( pScript, count );
	return 1;
}

int GameContext::playersShip( lua_State * pScript )			// playersShip()
{
	WorldContext * pContext = GetContext( pScript );
	
	BaseNode * pRoot = pContext->activeZone();
	for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
	{
		NounShip * pSelf = WidgetCast<NounShip>( pRoot->child(i) );
		if ( pSelf != NULL && pSelf->isLocal() )
		{
			lua_pushstring( pScript, CharString(pSelf->key().string()) );
			return 1;
		}
	}

	Alert( pScript, "playersShip() error! Ship not found."  );
	return 0;
}

int GameContext::getShipType( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			lua_pushnumber( pScript, pSelf->type() );
			return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getShipType() error! ", sErrorDetail );
	return 0;
}

int GameContext::spawnShip( lua_State * pScript )		// spawnShip( resource, where, name, team )
{
	WorldContext * pContext = GetContext( pScript );

	int n = lua_gettop( pScript );
	if ( n != 4 )
	{
		Alert( pScript, "spawnShip() error!. Invalid number of arguments."  );
		return 0;
	}
	if (! pContext->isServer() )
	{
		Alert( pScript, "spawnShip() error! Server side function only!" );
		return 0;
	}

	// load the ship context
	NounTemplate::Link pNounTemplate;
	if (! loadResource( CharString( lua_tostring( pScript, 1 )), pNounTemplate ) )
	{
		Alert( pScript, "spawnShip() error! Failed to load resource."  );
		return 0;
	}

	// find the location
	Noun * pWhere = GetNoun( pScript, lua_tostring( pScript, 2 ) );
	if ( pWhere == NULL )
	{
		Alert( pScript, "spawnShip() error! Failed to find where location."  );
		return 0;
	}

	// create the actual ship object
	Noun::Ref pUncasted = pNounTemplate->spawn();
	NounShip * pSpawn = WidgetCast<NounShip>( pUncasted );
	if ( pSpawn == NULL )
	{
		Alert( pScript, "spawnShip() error!. Failed to spawn ship"  );
		return 0;
	}

	// generate the spawn location outside the radius of the target object
	Vector3 offset( RandomFloat(-1,1), 0.0f, RandomFloat(-1,1) );
	offset.normalize();

	float radius = pWhere->radius() + pSpawn->radius();
	radius *= 1.2f;		// expand the radius by 20%

	Vector3 position( pWhere->worldPosition() + (offset * radius) );

	// set the ship home
	pSpawn->setHeading( atan2( offset.x, offset.z ) );
	pSpawn->setPosition( position );
	pSpawn->setName( CharString(lua_tostring( pScript, 3 )) );
	pSpawn->setTeamId( lua_tonumber( pScript, 4 ) );
	pSpawn->setTick( pContext->tick() );
	pSpawn->setHome( pWhere );
	pSpawn->setOrder( NounShip::DEFEND, pWhere, pSpawn );
	pSpawn->setup();

	// attach the new object to the world
	pContext->attachNoun( pSpawn );

	lua_pushstring( pScript, CharString(pSpawn->key().string()) );
	return 1;
}

int GameContext::orderShip( lua_State * pScript )		// orderShip( name, target, order )
{
	WorldContext * pContext = GetContext( pScript );

	if ( pContext->isServer() && lua_gettop( pScript ) == 3 )
	{
		// find the ship
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf == NULL )
		{
			Alert( pScript, "orderShip() error! Failed to find ship"  );
			return 0;
		}

		// find the target
		Noun * pTarget = GetNoun( pScript, lua_tostring( pScript, 2 ) );

		int order = lua_tonumber( pScript, 3 );
		if ( order < NounShip::NOORDER || order > NounShip::ORDER_COUNT )
		{
			Alert( pScript, "orderShip() error! Invalid order" );
			return 0;
		}

		pSelf->verbOrder( (NounShip::Order)order, pTarget, pSelf );
		//pSelf->setOrder( (NounShip::Order)order, pTarget, pSelf );
		lua_pushboolean( pScript, true );
		return 1;
	}

	Alert( pScript, "orderShip() error! Function needs 3 parameters and to be executed serverside." );
	return 0;
}

int GameContext::getShipOrder( lua_State * pScript )		// getShipOrder( name )
{
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 1 )
	{
		// find the ship
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf == NULL )
		{
			Alert( pScript, "orderShip() error! Failed to find ship"  );
			return 0;
		}

		lua_pushnumber( pScript, pSelf->order() );
		return 1;
	}

	Alert( pScript, "getShipOrder() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::isJumping( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			GadgetJumpDrive * pDrive = pSelf->jumpDrive();
			lua_pushboolean( pScript, pDrive != NULL && pDrive->engaged() );
			return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "isJumping() error! ", sErrorDetail );
	return 0;
}

int GameContext::isAIControlled( lua_State * pScript )
{
	CharString sErrorDetail;
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 1 && pContext->isServer() )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			lua_pushboolean( pScript, pSelf->userId() == 0 );
			return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter and to be called serverside.";

	Alert( pScript, "isAIControlled() error! ", sErrorDetail );
	return 0;
}

int GameContext::getEnergy( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			float percentage = pSelf->energyRatio() * 100;
			lua_pushnumber( pScript, percentage );
			return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getEnergy() error! ", sErrorDetail );
	return 0;
}

int GameContext::getFacingForNoun( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			Noun * pNoun = WidgetCast<Noun>( GetNoun( pScript, lua_tostring( pScript, 2 ) ) );
			if ( pNoun != NULL )
			{
				Vector3 delta( pSelf->worldPosition() - pNoun->worldPosition() );
				delta = pSelf->worldFrame() * delta;	// transform to relative object rotation
				
				lua_pushnumber( pScript, pSelf->getFacing( atan2( delta.x, delta.z ), true ) );
				return 1;
			}
			else
				sErrorDetail = "Noun not found.";
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameter.";

	Alert( pScript, "getFacingForNoun() error! ", sErrorDetail );
	return 0;
}

int GameContext::enableShipAI( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			int enable = lua_toboolean( pScript, 2 );
			if( enable )
				pSelf->clearFlags( NounShip::FLAG_COMMAND_DISABLED );
			else
				pSelf->addFlags( NounShip::FLAG_COMMAND_DISABLED );

			return 0;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "enableAI() error! ", sErrorDetail );
	return 0;
}

int GameContext::controlShipAbsolute( lua_State * pScript ) // controlShipAbsolute( shipId, heading, velocity )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 3 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			if ( lua_isnumber( pScript, 2 ) && lua_isnumber( pScript, 3 ) )
			{
				float fHeading = lua_tonumber( pScript, 2 );
				float fVelocity = lua_tonumber( pScript, 3 );
				pSelf->controlHeadingVelocity( fHeading, fVelocity );
				
				return 0;
			}
			else
				sErrorDetail = "Heading and Velocity must be numeric.";
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 3 parameters.";

	Alert( pScript, "controlShipAbsolute() error! ", sErrorDetail );
	return 0;
}

int GameContext::controlShipRelative( lua_State * pScript ) // controlShipRelative( shipId, yaw, velocity )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 3 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			if ( lua_isnumber( pScript, 2 ) && lua_isnumber( pScript, 3 ) )
			{
				float fYaw = lua_tonumber( pScript, 2 );
				float fVelocity = lua_tonumber( pScript, 3 );
				pSelf->controlYawVelocity( fYaw, fVelocity );
				
				return 0;
			}
			else
				sErrorDetail = "Yaw and Velocity must be numeric.";
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 3 parameters.";

	Alert( pScript, "controlShipRelative() error! ", sErrorDetail );
	return 0;
}

int GameContext::getShipVelocity( lua_State * pScript ) // getShipVelocity( shipId )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
				lua_pushnumber( pScript, pSelf->velocity() );
				return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getShipVelocity() error! ", sErrorDetail );
	return 0;
}

int GameContext::getShipHeading( lua_State * pScript ) // getShipHeading( shipId )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
				lua_pushnumber( pScript, pSelf->heading() );
				return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getShipHeading() error! ", sErrorDetail );
	return 0;
}

int GameContext::getShipYaw( lua_State * pScript ) // getShipYaw( shipId )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
				lua_pushnumber( pScript, pSelf->yaw() );
				return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getShipYaw() error! ", sErrorDetail );
	return 0;
}



//----------------------------------------------------------------------------
// GADGET FUNCTIONS

int GameContext::isGadget( lua_State * pScript )
{
	if ( lua_gettop( pScript ) == 1 )
	{
		NounGadget * pNoun = WidgetCast<NounGadget>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		lua_pushboolean( pScript, pNoun != NULL );
		return 1;
	}

	Alert( pScript, "isGadget() error! Function needs 1 parameter." );
	return 0;
}

int GameContext::gadgetCount( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			int count = 0;
			for(int i=0;i<pSelf->childCount();i++)
				if ( WidgetCast<NounGadget>( pSelf->child(i) ) != NULL )
					count++;

			lua_pushnumber( pScript, count );
			return 1;
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "gadgetCount() error! ", sErrorDetail );
	return 0;
}

int GameContext::getGadget( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounShip * pSelf = WidgetCast<NounShip>( GetNoun( pScript, lua_tostring( pScript, 1 ) ) );
		if ( pSelf != NULL )
		{
			int n = lua_tonumber( pScript, 2 );

			int c = 0;
			for(int i=0;i<pSelf->childCount();i++)
				if ( WidgetCast<NounGadget>( pSelf->child(i) ) != NULL )
				{
					if ( c == n )
					{
						lua_pushstring( pScript, CharString(pSelf->child(i)->key().string()) );
						return 1;
					}
					c++;
				}
			
			sErrorDetail = "Index out of range.";
		}
		else
			sErrorDetail = "Ship not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "getGadget() error! ", sErrorDetail );
	return 0;
}

int GameContext::getGadgetType( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadget != NULL )
		{
			lua_pushnumber( pScript, pGadget->type() );
			return 1;
		}
		else
			sErrorDetail = "Gadget not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getGadgetType() error! ", sErrorDetail );
	return 0;
}

int GameContext::getGadgetMount( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadget != NULL )
		{
			lua_pushnumber( pScript, pGadget->mountFlags() );
			return 1;
		}
		else
			sErrorDetail = "Gadget not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getGadgetMount() error! ", sErrorDetail );
	return 0;
}

int GameContext::setGadgetMount( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadget != NULL )
		{
			dword nMountFlags = lua_tonumber( pScript, 2 );
			nMountFlags &= NounGadget::FLAG_MOUNT_FULL;
			if ( nMountFlags != 0 )
			{
				pGadget->clearFlags( NounGadget::FLAG_MOUNT_FULL );
				pGadget->addFlags( nMountFlags );
				lua_pushboolean( pScript, true );
				return 1;
			}
			else
				sErrorDetail = "Invalid mount flags.";
		}
		else
			sErrorDetail = "Gadget not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "setGadgetMount() error! ", sErrorDetail );
	return 0;
}

int GameContext::getGadgetAmmo( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		GadgetWeapon * pGadget = WidgetCast<GadgetWeapon>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadget != NULL )
		{
			lua_pushnumber( pScript, pGadget->ammoMax() > 0 ? pGadget->ammo() : 0 );
			return 1;
		}
		else
			sErrorDetail = "GadgetWeapon not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getGadgetAmmo() error! ", sErrorDetail );
	return 0;
}

int GameContext::setGadgetAmmo( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		GadgetWeapon * pGadget = WidgetCast<GadgetWeapon>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadget != NULL )
		{
			pGadget->setAmmo( lua_tonumber( pScript, 2 ) );
			return 0;
		}
		else
			sErrorDetail = "GadgetWeapon not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameter.";

	Alert( pScript, "setGadgetAmmo() error! ", sErrorDetail );
	return 0;
}

int GameContext::hasGadgetAmmo( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		GadgetWeapon * pGadget = WidgetCast<GadgetWeapon>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadget != NULL )
		{
			lua_pushboolean( pScript, pGadget->ammoMax() > 0 );
			return 1;
		}
		else
			sErrorDetail = "GadgetWeapon not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "hasGadgetAmmo() error! ", sErrorDetail );
	return 0;
}

int GameContext::useGadget2( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 3 )
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadget != NULL )
		{
			NounShip * pSelf = WidgetCast<NounShip>( pGadget->parent() );
			if ( pSelf != NULL )
			{
				// get the target argument
				Noun * pTarget = GetNoun( pScript, lua_tostring( pScript, 2 ) );
				// get the shift argument
				int shift = lua_tonumber( pScript, 3 );

				pSelf->useGadget( pGadget, pTarget, shift != 0 );
				lua_pushboolean( pScript, true );
				return 1;
			}
			else
				sErrorDetail = "Ship not found (Gadget not attached to a ship ?).";
		}
		else
			sErrorDetail = "Gadget not found.";
	}
	else
		sErrorDetail = "Function needs 3 parameters.";

	Alert( pScript, "useGadget2() error! ", sErrorDetail );
	return 0;

}

int GameContext::isGadgetActive( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadget != NULL )
		{
			lua_pushboolean( pScript, pGadget->useActive() );
			return 1;
		}
		else
			sErrorDetail = "Gadget not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "isGadgetActive() error! ", sErrorDetail );
	return 0;
}

int GameContext::isGadgetUsable( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 3 )
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadget != NULL )
		{
			int shift = lua_tonumber( pScript, 2 );
			Noun * pTarget = GetNoun( pScript, lua_tostring( pScript, 3) );

			lua_pushboolean( pScript, pGadget->usable( pTarget, shift != 0 ) );
			return 1;
		}
		else
			sErrorDetail = "Gadget not found.";
	}
	else
		sErrorDetail = "Function needs 3 parameters.";

	Alert( pScript, "isGadgetUsable() error! ", sErrorDetail );
	return 0;
}

int GameContext::beamPointDefenceEnabled( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		GadgetBeamWeapon * pBeam = WidgetCast<GadgetBeamWeapon>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pBeam != NULL )
		{
			lua_pushboolean( pScript, pBeam->pointDefense() );
			return 1;
		}
		else
			sErrorDetail = "Beam not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "beamPointDefenceEnabled() error! ", sErrorDetail );
	return 0;
}

int GameContext::setBeamPointDefence( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		GadgetBeamWeapon * pBeam = WidgetCast<GadgetBeamWeapon>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pBeam != NULL )
		{
			pBeam->setPointDefense( lua_tonumber( pScript, 2) == 1 );
			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "Beam not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "beamPointDefenceEnabled() error! ", sErrorDetail );
	return 0;
}

int GameContext::getJumpTarget( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		GadgetJumpDrive * pJumpDrive = WidgetCast<GadgetJumpDrive>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pJumpDrive != NULL )
		{
			Noun * pTarget = pJumpDrive->jumpTarget();
			if( pTarget != NULL )
				lua_pushstring( pScript, CharString(pTarget->key().string()) );
			else
				lua_pushnil( pScript );	// drive has no target

			return 1;
		}
		else
			sErrorDetail = "Parameters needs to be a JumpDrive.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getJumpTarget() error! ", sErrorDetail );
	return 0;
}

int GameContext::getShielding( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		GadgetArmor * pGadgetArmor = WidgetCast<GadgetArmor>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadgetArmor != NULL )
		{
			lua_pushnumber( pScript, ( pGadgetArmor->armor() * 100 ) / pGadgetArmor->maxArmor() );
			return 1;
		}

		GadgetShield * pGadgetShield = WidgetCast<GadgetShield>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadgetShield != NULL )
		{
			lua_pushnumber( pScript, ( pGadgetShield->charge() * 100 ) / pGadgetShield->maxCharge() );
			return 1;
		}
			
		sErrorDetail = "GadgetShield/Armor not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getShielding() error! ", sErrorDetail );
	return 0;
}

int GameContext::setShielding( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 2 )
	{
		int strength = lua_tonumber( pScript, 2 );
		GadgetArmor * pGadgetArmor = WidgetCast<GadgetArmor>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadgetArmor != NULL )
		{
			pGadgetArmor->setArmor( ( pGadgetArmor->maxArmor() * strength ) / 100 );
			return 0;
		}

		GadgetShield * pGadgetShield = WidgetCast<GadgetShield>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pGadgetShield != NULL )
		{
			pGadgetShield->setCharge( ( pGadgetShield->maxCharge() * strength ) / 100 );
			return 0;
		}
			
		sErrorDetail = "GadgetShield/Armor not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "setShielding() error! ", sErrorDetail );
	return 0;
}


//----------------------------------------------------------------------------
// UNIT FUNCTIONS

int GameContext::getUnitExperience( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounUnit * pUnit = WidgetCast<NounUnit>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pUnit != NULL )
		{
			lua_pushnumber( pScript, pUnit->experience() * 100 );	// return experience in percent
			return 1;
		}
		else
			sErrorDetail = "Unit not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getUnitExperience() error! ", sErrorDetail );
	return 0;
}

int GameContext::setUnitExperience( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounUnit * pUnit = WidgetCast<NounUnit>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pUnit != NULL )
		{
			pUnit->setExperience( lua_tonumber( pScript, 2) * 100 );
			lua_pushboolean( pScript, true );
			return 1;
		}
		else
			sErrorDetail = "Unit not found.";
	}
	else
		sErrorDetail = "Function needs 2 parameters.";

	Alert( pScript, "setUnitExperience() error! ", sErrorDetail );
	return 0;
}

int GameContext::getUnitOrder( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 1 )
	{
		NounUnit * pUnit = WidgetCast<NounUnit>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pUnit != NULL )
		{
			lua_pushnumber( pScript, pUnit->order() );
			Noun * pTarget = pUnit->target();
			if ( pTarget != NULL )
				lua_pushstring( pScript, CharString( pTarget->key().string() ) );
			else
				lua_pushnil( pScript );

			return 2;
		}
		else
			sErrorDetail = "Unit not found.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getUnitOrder() error! ", sErrorDetail );
	return 0;
}

int GameContext::setUnitOrder( lua_State * pScript )
{
	CharString sErrorDetail;
	if ( lua_gettop( pScript ) == 3 )
	{
		NounUnit * pUnit = WidgetCast<NounUnit>( GetNoun( pScript, lua_tostring( pScript, 1) ) );
		if ( pUnit != NULL )
		{
			Noun * pTarget = WidgetCast<Noun>( GetNoun( pScript, lua_tostring( pScript, 3) ) );
			if ( pTarget != NULL )
			{
				int order = lua_tonumber( pScript, 2 );
				Verb::Ref( new VerbOrderUnit( pUnit, (NounUnit::Order)order, pTarget, NULL ) );

				lua_pushboolean( pScript, true );
				return 1;
			}
			else
				sErrorDetail = "Target not found.";
		}
		else
			sErrorDetail = "Unit not found.";
	}
	else
		sErrorDetail = "Function needs 3 parameters.";

	Alert( pScript, "setUnitOrder() error! ", sErrorDetail );
	return 0;
}

//----------------------------------------------------------------------------
// JUMPGATE FUNCTIONS

int GameContext::jumpgateCount( lua_State * pScript )
{
	WorldContext * pContext = GetContext( pScript );

	int count = 0;

	for(int j=0;j<pContext->zoneCount();j++)
	{
		BaseNode * pRoot = pContext->zone( j );
		for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
			if ( WidgetCast<NounJumpGate>( pRoot->child(i) ) )
				count++;
	}

	lua_pushnumber( pScript, count );
	return 1;
}

int GameContext::getJumpgate( lua_State * pScript )
{
	CharString sErrorDetail;
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 1 )
	{
		int gate = lua_tonumber( pScript, 1 );
		int index = 0;

		for(int j=0;j<pContext->zoneCount();j++)
		{
			BaseNode * pRoot = pContext->zone( j );
			for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
				if ( WidgetCast<NounJumpGate>( pRoot->child(i) ) )
				{
					if ( gate == index )
					{
						lua_pushstring( pScript, CharString(pRoot->child(i)->key().string()) );
						return 1;
					}

					index++;
				}
		}

		sErrorDetail = "Index out of range.";
	}
	else
		sErrorDetail = "Function needs 1 parameter.";

	Alert( pScript, "getJumpgate() error! ", sErrorDetail );

	return 0;
}

int GameContext::getRandomJumpgate( lua_State * pScript )
{
	WorldContext * pContext = GetContext( pScript );

	Array< NounJumpGate * > gates;

	for(int j=0;j<pContext->zoneCount();j++)
	{
		BaseNode * pRoot = pContext->zone( j );
		for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
			if ( WidgetCast<NounJumpGate>( pRoot->child(i) ) )
				gates.push( (NounJumpGate *)pRoot->child(i) );
	}

	if ( gates.size() > 0 )
	{
		NounJumpGate * pGate = gates[ rand() % gates.size() ];
		lua_pushstring( pScript, CharString(pGate->key().string()) );
		return 1;
	}

	return 0; // no alert(), this is intentional
}

int GameContext::getFriendlyJumpgate( lua_State * pScript )
{
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 1 )
	{
		int faction = lua_tonumber( pScript, 1 );

		for(int j=0;j<pContext->zoneCount();j++)
		{
			BaseNode * pRoot = pContext->zone( j );
			for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
				if ( WidgetCast<NounJumpGate>( pRoot->child(i) ) )
				{
					if ( ((NounJumpGate *)pRoot->child(i))->factionId() == faction )
					{
						lua_pushstring( pScript, CharString(pRoot->child(i)->key().string()) );
						return 1;
					}
				}
		}
	}
	// no error message since the script may expect this function to fail
	// and look for an alternative location to spawn ships
	return getRandomJumpgate( pScript );
}

// disable all safe zones
int GameContext::setSafeZones( lua_State * pScript )
{
	WorldContext * pContext = GetContext( pScript );

	if ( lua_gettop( pScript ) == 1 )
	{
		float fSafeZoneDistance = lua_tonumber( pScript, 1 );

		for(int j=0;j<pContext->zoneCount();j++)
		{
			BaseNode * pRoot = pContext->zone( j );
			for(int i=0;pRoot != NULL && i<pRoot->childCount();i++)
				if ( WidgetCast<NounJumpGate>( pRoot->child(i) ) )
				{
					NounJumpGate * pGate = (NounJumpGate *)pRoot->child(i);
					pGate->setSafeZone( pGate->safeZone(), fSafeZoneDistance );
				}
		}

		return 0;
	}

	Alert( pScript, "setSafeZones() error!, needs 1 argument!" );
	return 0;
}

//----------------------------------------------------------------------------

void GameContext::registerFunctions( lua_State * pScript )
{
	WorldContext::registerFunctions( pScript );

	// General or Noun
	lua_register(pScript, "isStar", isStar );
	lua_register(pScript, "isAsteroid", isAsteroid );
	lua_register(pScript, "isNebula", isNebula );
	lua_register(pScript, "isBody", isBody );
	lua_register(pScript, "isUnit", isUnit );
	lua_register(pScript, "isPlanet", isPlanet );
	lua_register(pScript, "isOrbiting", isOrbiting );
	lua_register(pScript, "enterOrbit", enterOrbit );
	lua_register(pScript, "breakOrbit", breakOrbit );
	lua_register(pScript, "isBeaconed", isBeaconed );
	lua_register(pScript, "isFaction", isFaction );
	lua_register(pScript, "canDetect", canDetect );
	lua_register(pScript, "playersShip", playersShip );
	lua_register(pScript, "getAllNouns", getAllNouns );
	lua_register(pScript, "getAllNounsRelative", getAllNounsRelative );
	lua_register(pScript, "getFleetId", getTeamId );

	// NounPlanet
	lua_register(pScript, "planetCount", planetCount );
	lua_register(pScript, "getPlanet", getPlanet );
	lua_register(pScript, "getPlanetMorale", getPlanetMorale );
	lua_register(pScript, "getPlanetPopulation", getPlanetPopulation );
	lua_register(pScript, "setPlanetPopulation", setPlanetPopulation );
	lua_register(pScript, "getPlanetResources", getPlanetResources );
	lua_register(pScript, "setPlanetResources", setPlanetResources );
	lua_register(pScript, "getPlanetTech", getPlanetTech );
	lua_register(pScript, "setPlanetTech", setPlanetTech );
	lua_register(pScript, "getPlanetValue", getPlanetValue );
	lua_register(pScript, "getPlanetFlags", getPlanetFlags );
	lua_register(pScript, "planetEvent", planetEvent );
	lua_register(pScript, "revoltPlanet", revoltPlanet );
	lua_register(pScript, "hexCount", hexCount );
	lua_register(pScript, "getHex", getHex );
	lua_register(pScript, "getHexPosition", getHexPosition );
	lua_register(pScript, "structureCount", structureCount );
	lua_register(pScript, "getStructure", getStructure );
	lua_register(pScript, "getStructureHex", getStructureHex );
	lua_register(pScript, "setStructureHull", setStructureHull );
	lua_register(pScript, "getStructureHull", getStructureHull );
	lua_register(pScript, "isStructureActive", isStructureActive );
	lua_register(pScript, "isStructurePaused", isStructurePaused );
	lua_register(pScript, "pauseStructure", pauseStructure );
	lua_register(pScript, "isStructureFinished", isStructureFinished );
	lua_register(pScript, "unitCount", unitCount );
	lua_register(pScript, "getUnit", getUnit );
	lua_register(pScript, "getUnitHex", getUnitHex );
	lua_register(pScript, "getRandomPlanet", getRandomPlanet );
	lua_register(pScript, "getFriendlyPlanet", getFriendlyPlanet );
	lua_register(pScript, "getEnemyPlanet", getEnemyPlanet );
	lua_register(pScript, "setAllegiance", setAllegiance );
	lua_register(pScript, "setAllegianceXP", setAllegianceXP );	

	// NounShip
	lua_register(pScript, "isShip", isShip );
	lua_register(pScript, "isShipDestroyed", isShipDestroyed );
	lua_register(pScript, "shipCount", shipCount );
	lua_register(pScript, "getShip", getShip );
	lua_register(pScript, "useGadget", useGadget );
	lua_register(pScript, "getShipType", getShipType );
	lua_register(pScript, "getHull", getHull );
	lua_register(pScript, "setHull", setHull );
	lua_register(pScript, "getSignature", getSignature );
	lua_register(pScript, "spawnShip", spawnShip );
	lua_register(pScript, "orderShip", orderShip );
	lua_register(pScript, "getShipOrder", getShipOrder );
	lua_register(pScript, "isJumping", isJumping );
	lua_register(pScript, "isAIControlled", isAIControlled );
	lua_register(pScript, "getEnergy", getEnergy );
	lua_register(pScript, "getFacingForNoun", getFacingForNoun );
	lua_register(pScript, "enableShipAI", enableShipAI );
	lua_register(pScript, "controlShipAbsolute", controlShipAbsolute );
	lua_register(pScript, "controlShipRelative", controlShipRelative );
	lua_register(pScript, "getShipVelocity", getShipVelocity );
	lua_register(pScript, "getShipHeading", getShipHeading );
	lua_register(pScript, "getShipYaw", getShipYaw );
	
	// NounGadget & related
	lua_register(pScript, "isGadget", isGadget );
	lua_register(pScript, "gadgetCount", gadgetCount );
	lua_register(pScript, "getGadget", getGadget );
	lua_register(pScript, "getGadgetType", getGadgetType );
	lua_register(pScript, "getGadgetMount", getGadgetMount );
	lua_register(pScript, "setGadgetMount", setGadgetMount );
	lua_register(pScript, "getGadgetAmmo", getGadgetAmmo );
	lua_register(pScript, "setGadgetAmmo", setGadgetAmmo );
	lua_register(pScript, "hasGadgetAmmo", hasGadgetAmmo );
	lua_register(pScript, "useGadget2", useGadget2 );
	lua_register(pScript, "isGadgetActive", isGadgetActive );
	lua_register(pScript, "isGadgetUsable", isGadgetUsable );
	lua_register(pScript, "beamPointDefenceEnabled", beamPointDefenceEnabled );
	lua_register(pScript, "setBeamPointDefence", setBeamPointDefence );
	lua_register(pScript, "getJumpTarget", getJumpTarget );
	lua_register(pScript, "getShielding", getShielding );
	lua_register(pScript, "setShielding", getShielding );
	
	// Unit functions
	lua_register(pScript, "getUnitExperience", getUnitExperience );
	lua_register(pScript, "setUnitExperience", setUnitExperience );
	lua_register(pScript, "getUnitOrder", getUnitOrder );
	lua_register(pScript, "setUnitOrder", setUnitOrder );

	// NounJumpGate
	lua_register(pScript, "jumpgateCount", jumpgateCount );
	lua_register(pScript, "getJumpgate", getJumpgate );
	lua_register(pScript, "getFriendlyJumpgate", getFriendlyJumpgate );
	lua_register(pScript, "getRandomJumpgate", getRandomJumpgate );
	lua_register(pScript, "setSafeZones", setSafeZones );
}

//---------------------------------------------------------------------------------------------------
//EOF
