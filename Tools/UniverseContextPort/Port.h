/*
	Port.h

	Port object
	(c)1998 Palestar, Richard Lyle
*/

#ifndef UNIVERSECONTEXTPORT_H
#define UNIVERSECONTEXTPORT_H

#include "Standard/Dll.h"
#include "DarkSpace/GameContext.h"
#include "Tools/ResourcerDoc/Port.h"

//-------------------------------------------------------------------------------

#undef DLL
#ifdef UNIVERSECONTEXTPORT_DLL
#define DLL		DLL_EXPORT
#else
#define DLL		DLL_IMPORT
#endif

//-------------------------------------------------------------------------------

class DLL UniverseContextPort : public Port
{
public:
	DECLARE_WIDGET_CLASS();
	DECLARE_PROPERTY_LIST();

	typedef Reference<UniverseContextPort>	Ref;
	typedef GameContext::ObjectiveType	ObjectiveType;

	struct Objective
	{
		Objective() : faction( 0 )
		{}
		Objective( const Objective & copy ) : faction( 0 )
		{
			operator=( copy );
		}

		CharString				description;
		ObjectiveType			type;
		int						faction;
		CharString				noun;

		Objective &				operator=( const Objective & copy );
	};
	struct Condition
	{
		Condition()
		{}
		Condition( const Condition & copy ) 
		{
			operator=( copy );
		}

		CharString				message;		// message sent to players when this condition meet
		CharString				next;			// next scenerio 
		Array< Objective >		objectives;		// objectives to activate this condition

		Condition &				operator=( const Condition & copy );
	};
	struct Ship
	{
		Ship() : resources( 0 ), reserved1( 0 ), reserved2( 0 )
		{}
		Ship( const Ship & copy ) : resources( 0 ), reserved1( 0 ), reserved2( 0 )
		{
			operator=( copy );
		}

		dword					resources;
		dword					reserved1;
		dword					reserved2;
		CharString				context;		// ship context

		Ship &					operator=( const Ship & copy );
	};
	struct Team
	{
		Team() : faction( 0 ), color( WHITE ), flags( 0 )
		{}
		Team( const Team & copy ) : faction( 0 ), color( WHITE ), flags( 0 )
		{
			operator=( copy );
		}

		int						faction;
		CharString				logo;
		CharString				name;
		CharString				description;
		Color					color;
		CharString				ships;
		dword					flags;

		Team &					operator=( const Team & copy );
	};
	struct Zone
	{
		Zone() : vPosition( 0.0f, 0.0f, 0.0f )
		{}
		Zone( const Zone & copy )
		{
			operator=( copy );
		}

		CharString				sScene;
		Vector3					vPosition;

		Zone &					operator=( const Zone & copy );
	};

	// Construction
	UniverseContextPort();

	// Widget Interface
	bool						read( const InStream & input );
	// Port Interface
	bool						autoLoad();									// enable autoload for this resource
	void						dependencies( DependentArray & dep );		// get the depdendencies
	Resource::Ref				createResource();							// create the resource 
	CFrameWnd *					createView();								// create a view of the port

	bool						importFile( const char * fileName );		// import from file
	void						shellOpen() const;							// open the source object through a shell program

	// Static
	static int					factionCount();
	static const char *			factionName( int faction );
	static Tech					factionTech( int faction );
	static Race					factionRace( int faction );

	// Data
	CharString					m_Name;
	CharString					m_Description;
	int							m_Type;
	CharString					m_Script;
	CharString					m_Scenery;
	Vector3						m_Position;
						
	Array< Zone >				m_Zones;
	Array< Team >				m_Teams;
	Array< Condition >			m_Conditions;
	dword						m_TimeLimit;

	// deprecated
	CharString					m_UniversePort;
};

//----------------------------------------------------------------------------

inline UniverseContextPort::Objective & UniverseContextPort::Objective::operator=( const Objective & copy )
{
	description = copy.description;
	type = copy.type;
	faction = copy.faction;
	noun = copy.noun;
	return *this;
}

const int OBJECTIVE_VERSION_1 = 22201;
const int OBJECTIVE_VERSION_2 = 2;

inline const InStream & operator>>( const InStream & input, UniverseContextPort::Objective & objective )
{
	int version;
	input >> version;

	dword alliance;
	switch( version )
	{
	case OBJECTIVE_VERSION_2:
		input >> objective.description;
		input >> objective.type;
		input >> objective.faction;
		input >> objective.noun;
		break;
	case OBJECTIVE_VERSION_1:
		input >> objective.description;
		input >> objective.type;
		input >> objective.faction;
		input >> objective.noun;
		objective.faction += 1;
		break;
	default:
		input.file()->setPosition( input.file()->position() - sizeof(int) );
		input >> objective.description;
		input >> objective.type;
		input >> alliance;
		input >> objective.noun;
		objective.faction = 0;
		break;
	}

	return input;
}

inline const OutStream & operator<<( const OutStream & output, const UniverseContextPort::Objective & objective )
{
	output << OBJECTIVE_VERSION_2;
	output << objective.description;
	output << objective.type;
	output << objective.faction;
	output << objective.noun;
	return output;
}

//----------------------------------------------------------------------------

inline UniverseContextPort::Condition & UniverseContextPort::Condition::operator=( const Condition & copy )
{
	message = copy.message;
	objectives = copy.objectives;
	next = copy.next;
	return *this;
}

inline const InStream & operator>>( const InStream & input, UniverseContextPort::Condition & condition )
{
	input >> condition.message;
	input >> condition.objectives;
	input >> condition.next;
	return input;
}

inline const OutStream & operator<<( const OutStream & output, const UniverseContextPort::Condition & condition )
{
	output << condition.message;
	output << condition.objectives;
	output << condition.next;
	return output;
}

//----------------------------------------------------------------------------

inline UniverseContextPort::Ship & UniverseContextPort::Ship::operator=( const Ship & copy )
{
	resources = copy.resources;
	reserved1 = copy.reserved1;
	reserved2 = copy.reserved2;
	context = copy.context;
	return *this;
}

inline const InStream & operator>>( const InStream & input, UniverseContextPort::Ship & ship )
{
	input >> ship.resources;
	input >> ship.reserved1;
	input >> ship.reserved2;
	input >> ship.context;
	return input;
}

inline const OutStream & operator<<( const OutStream & output, const UniverseContextPort::Ship & ship )
{
	output << ship.resources;
	output << ship.reserved1;
	output << ship.reserved2;
	output << ship.context;
	return output;
}

//----------------------------------------------------------------------------

inline UniverseContextPort::Team & UniverseContextPort::Team::operator=( const Team & copy )
{
	faction = copy.faction;
	logo = copy.logo;
	name = copy.name;
	description = copy.description;
	color = copy.color;
	ships = copy.ships;
	flags = copy.flags;
	return *this;
}

const int TEAM_VERSION_1 = 22201;
const int TEAM_VERSION_2 = 40501;
const int TEAM_VERSION_3 = 20080413;
const int TEAM_VERSION_4 = 4;

inline const InStream & operator>>( const InStream & input, UniverseContextPort::Team & team )
{
	int version;
	input >> version;

	dword alliance;
	Array< UniverseContextPort::Ship > ships;

	switch( version )
	{
	case TEAM_VERSION_4:
		input >> team.faction;
		input >> team.logo;
		input >> team.name;
		input >> team.description;
		input >> team.color;
		input >> team.ships;
		input >> team.flags;
		break;
	case TEAM_VERSION_3:
		input >> team.faction;
		input >> team.logo;
		input >> team.name;
		input >> team.description;
		input >> team.color;
		input >> team.ships;
		input >> team.flags;
		team.faction += 1;
		break;
	case TEAM_VERSION_2:
		input >> team.faction;
		input >> team.logo;
		input >> team.name;
		input >> team.description;
		input >> team.color;
		input >> team.ships;
		team.faction += 1;
		break;
	case TEAM_VERSION_1:
		input >> team.faction;
		input >> team.logo;
		input >> team.name;
		input >> team.description;
		input >> team.color;
		input >> ships;
		team.faction += 1;
		break;
	default:
		input.file()->setPosition( input.file()->position() - sizeof(int) );
		input >> team.logo;
		input >> team.name;
		input >> team.description;
		input >> team.color;
		input >> alliance;
		input >> ships;
		team.faction = 1;
		break;
	}

	return input;
}

inline const OutStream & operator<<( const OutStream & output, const UniverseContextPort::Team & team )
{
	output << TEAM_VERSION_4;
	output << team.faction;
	output << team.logo;
	output << team.name;
	output << team.description;
	output << team.color;
	output << team.ships;
	output << team.flags;
	return output;
}

//---------------------------------------------------------------------------------------------------

inline UniverseContextPort::Zone & UniverseContextPort::Zone::operator=( const Zone & copy )
{
	sScene = copy.sScene;
	vPosition = copy.vPosition;
	return *this;
}

const int ZONE_VERSION_1 = 1;

inline const InStream & operator>>( const InStream & input, UniverseContextPort::Zone & zone )
{
	int version;
	input >> version;

	switch( version )
	{
	case ZONE_VERSION_1:
		input >> zone.sScene;
		input >> zone.vPosition;
		break;
	}

	return input;
}

inline const OutStream & operator<<( const OutStream & output, const UniverseContextPort::Zone & zone )
{
	output << ZONE_VERSION_1;
	output << zone.sScene;
	output << zone.vPosition;
	return output;
}

#endif

//-------------------------------------------------------------------------------
// EOF
