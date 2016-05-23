/*
	GadgetSpreadWeapon.cpp

	(c)2013 Palestar
*/


#include "Debug/Assert.h"
#include "System/Messages.h"
#include "Constants.h"
#include "NounProjectile.h"
#include "NounSmartProjectile.h"
#include "NounStructure.h"
#include "NounJumpGate.h"
#include "GadgetSpreadWeapon.h"
#include "GadgetWeapon.h"
#include "GameContext.h"
#include "StructureDefense.h"

IMPLEMENT_ABSTRACT_FACTORY( GadgetSpreadWeapon, GadgetWeapon ); 

BEGIN_ABSTRACT_PROPERTY_LIST( GadgetSpreadWeapon, GadgetWeapon );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Energy );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_Ammo );
END_PROPERTY_LIST();

GadgetSpreadWeapon::GadgetSpreadWeapon() : 
	GadgetWeapon()
{}

CharString GadgetSpreadWeapon::useTip( Noun * pTarget, bool shift ) const
{
	CharString tip = GadgetWeapon::useTip(pTarget, shift);

	tip += CharString().format("\n");
	tip += CharString().format("Spread amount:<X;100>%.1f Degrees\n", spreadDegree());
	tip += CharString().format("Extra shots:<X;100>%.0i shots\n", numSpread()*2 );

	/*
	if(pTarget != NULL)
	{
		Vector3 position;
		intercept( pTarget, position );		

		Vector3 velocity( position - worldPosition() );
		float degspread = spreadDegree() * (PI/180);

		Vector3 sLeft = Vector3(velocity.x, velocity.y, velocity.z);
		Vector3 sRight = Vector3(velocity.x, velocity.y, velocity.z);

		sLeft.rotateXZ(degspread);
		sRight.rotateXZ(-degspread);

		tip += CharString().format("DEBUG:\n");
		tip += CharString().format("velocity = %.3f, %.3f, %.3f\n", velocity.x, velocity.y, velocity.z);
		tip += CharString().format("sLeft = %.3f, %.3f, %.3f\n", sLeft.x, sLeft.y, sLeft.z);
		tip += CharString().format("sRight = %.3f, %.3f, %.3f\n", sRight.x, sRight.y, sRight.z);

	}
	*/

	return tip;
}


NounProjectile * GadgetSpreadWeapon::createProjectile( dword when, Noun * pTarget )
{
	// create the use effect
	if ( context()->isClient() )
		createUseEffect();

	//This weapon is always a turreted weapon

	// get the intercept point for this weapon
	Vector3 position;
	intercept( pTarget, position );		

	Vector3 velocity( position - worldPosition() );
	
	//We have the vector that is dead straight towards a ship. We want to create additional numSpread*2 projectiles
	//that are modified by spreadDegree degrees off the 'center' vector
	for(int i = 1; i <= numSpread(); i++)
	{
		float degspread = (float)i * spreadDegree() * (PI/180);

		Vector3 sLeft = Vector3(velocity.x, velocity.y, velocity.z);
		Vector3 sRight = Vector3(velocity.x, velocity.y, velocity.z);

		sLeft.rotateXZ(degspread);
		sRight.rotateXZ(-degspread);
		sLeft.normalize();
		sRight.normalize();

		sLeft *= projectileVelocity();
		sRight *= projectileVelocity();

		GadgetWeapon::createProjectile(when, sLeft, pTarget);
		GadgetWeapon::createProjectile(when, sRight, pTarget);
	}

	//fire the original dead straight projectile
	velocity.normalize();
	velocity *= projectileVelocity();
		
	return GadgetWeapon::createProjectile( when, velocity, pTarget );
}