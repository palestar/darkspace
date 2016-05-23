/*
	NounDrone.h

	This object moves from it's home to it's destination for a specified duration and then optionally returns home
	(c)2000 Palestar Inc, Richard Lyle
*/

#ifndef NOUNDRONE_H
#define NOUNDRONE_H

#include "Trail.h"
#include "NounGame.h"
#include "GameDll.h"

//----------------------------------------------------------------------------

class DLL NounDrone : public NounGame
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	// possible drone states
	enum State
	{
		GO_TARGET,
		AT_TARGET,
		GO_HOME,
		AT_HOME,
	};

	// The "Action" class allows the drone to callback to the parent when it arrives at it's target position
	// Usage: inherit from this class, then set that object as the "Home"
	class Action
	{
	public:
		virtual void	droneUpdate( NounDrone * pDrone ) = 0;
		virtual void	droneAction( NounDrone * pDrone ) = 0;
		virtual void	droneHome( NounDrone * pDrone ) = 0;
	};

	// Construction
	NounDrone();

	// Node interface
	void				render( RenderContext &context, 
							const Matrix33 & frame, 
							const Vector3 & position );
	// Noun interface
	NounType			nounType() const;	
	
	void				initialize();
	void				release();
	void				simulate( dword nTick );
	void				onWorldTransform( const Vector3 & transform );

	// Accessors
	State				state() const;
	float				speed() const;
	Noun *				target() const;
	const Vector3 &		targetOffset() const;
	int					duration() const;
	Noun *				home() const;
	const Vector3 &		homeOffset() const;

	// Mutators
	void				setState( State state );
	void				setSpeed( float speed );
	void				setTarget( Noun * pTarget, const Vector3 & vTarget );		// vTarget is in object space of the target
	void				setDuration( int ticks );
	void				setHome( Noun * pHome, const Vector3 & vHome );				// vHome is in object space
	void				setVelocity( float velocity );

	void				loadDrone();

private:
	// Data
	State				m_State;
	float				m_Speed;
	Noun::wRef			m_Target;
	Vector3				m_TargetOffset;
	int					m_Duration;
	Noun::wRef			m_Home;
	Vector3				m_HomeOffset;
	float				m_Velocity;

	// non-serialized
	Scene::Link			m_Drone;
	Trail				m_Trail;

	// Mutators
	float				move( const Vector3 & destination );
};

IMPLEMENT_RAW_STREAMING( NounDrone::State );

//----------------------------------------------------------------------------

inline NounDrone::State NounDrone::state() const
{
	return m_State;
}

inline float NounDrone::speed() const
{
	return m_Speed;
}

inline Noun * NounDrone::target() const
{
	return m_Target;
}

inline const Vector3 & NounDrone::targetOffset() const
{
	return m_TargetOffset;
}

inline int NounDrone::duration() const
{
	return m_Duration;
}

inline Noun * NounDrone::home() const
{
	return m_Home;
}

inline const Vector3 & NounDrone::homeOffset() const
{
	return m_HomeOffset;
}

//----------------------------------------------------------------------------

inline void NounDrone::setState( State state )
{
	m_State = state;
}

inline void NounDrone::setSpeed( float speed )
{
	m_Speed = speed;
}

inline void NounDrone::setTarget( Noun * pTarget, const Vector3 & vTarget )
{
	m_Target = pTarget;
	m_TargetOffset = vTarget;
}

inline void NounDrone::setDuration( int ticks )
{
	m_Duration = ticks;
}

inline void NounDrone::setHome( Noun * pHome, const Vector3 & vHome )
{
	m_Home = pHome;
	m_HomeOffset = vHome;

	loadDrone();
}

inline void NounDrone::setVelocity( float velocity )
{
	m_Velocity = velocity;
}

//----------------------------------------------------------------------------




#endif

//----------------------------------------------------------------------------
//EOF
