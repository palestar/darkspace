/*
	NounField.cpp
	(c)2004 Palestar Inc, Richard Lyle
*/


#include "Constants.h"
#include "NounField.h"
#include "GameContext.h"

//----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_FACTORY( NounField, NounGame ); 

BEGIN_ABSTRACT_PROPERTY_LIST( NounField, NounGame );
	ADD_TRANSMIT_PROPERTY( m_fFieldRadius );
END_PROPERTY_LIST();

NounField::NounField() : m_fFieldRadius( 0 )
{}

//----------------------------------------------------------------------------

NounType NounField::nounType() const
{
	return TYPE_LANDMARK;
}

bool NounField::collidable() const
{
	return true;
}

bool NounField::canBeDetected() const
{
	return false;
}

//----------------------------------------------------------------------------

bool NounField::postInitialize()
{
	if (! Noun::postInitialize() )
		return false;

	m_fRadius = m_fFieldRadius;
	m_fRadius2 = m_fRadius * m_fRadius;
	return true;
}

void NounField::initialize()
{
	Noun::initialize();

	for( InFieldMap::iterator iInField = m_InField.begin(); 
		iInField != m_InField.end(); ++iInField )
	{
		Noun * pNoun = iInField->second;
		if ( pNoun != NULL )
			onEnterField( pNoun );
	}
}

void NounField::release()
{
	Noun::release();

	for( InFieldMap::iterator iInField = m_InField.begin(); 
		iInField != m_InField.end(); ++iInField )
	{
		Noun * pNoun = iInField->second;
		if ( pNoun != NULL )
			onLeaveField( pNoun );
	}
	m_InField.clear();
}

void NounField::simulate( dword nTick )
{
	// clear the collision set during the simulation.. collisions are detected after the
	// simulations are ran..
	m_CollisionSet.clear();

	Noun::simulate( nTick );
}

void NounField::collision( Noun * pCollide, const Vector3 & intersect )
{
	// insert the ID of the object into our collision set, so we know we are still colliding with the object..
	m_CollisionSet.insert( pCollide->key() );

	if ( m_InField.find( pCollide->key() ) == m_InField.end() )
	{
		m_InField[ pCollide->key() ] = pCollide;
		onEnterField( pCollide );
	}

	onInsideField( pCollide );
}

void NounField::update()
{
	NounGame::update();

	// detect any objects leaving our field..
	for( InFieldMap::iterator iInField = m_InField.begin(); 
		iInField != m_InField.end(); )
	{
		if ( m_CollisionSet.find( iInField->first ) == m_CollisionSet.end() )
		{
			// no longer colliding, leave the field..
			Noun::Ref pNoun = iInField->second.pointer();
			if ( pNoun.valid() )
				onLeaveField( pNoun );
			m_InField.erase( iInField++ );
		}
		else
		{
			// still colliding..
			++iInField;
		}
	}
}


//----------------------------------------------------------------------------

void NounField::setFieldRadius( float fRadius )
{
	m_fFieldRadius = fRadius;
	m_fRadius = m_fFieldRadius;
	m_fRadius2 = m_fRadius * m_fRadius;
}

//---------------------------------------------------------------------------------------------------

void NounField::removeAllFields( BaseNode * a_pNode )
{
	// remove any extra child fields..
	std::list< NounField * > fields;
	FindNodeClasses<NounField>( a_pNode, fields );

	for( std::list< NounField * >::iterator iField = fields.begin(); 
		iField != fields.end(); ++iField )
	{
		(*iField)->detachSelf();
	}
}

//----------------------------------------------------------------------------
//EOF
