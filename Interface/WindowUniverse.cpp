/*
	WindowUniverse.cpp
	(c)2000 Palestar Inc, Richard Lyle
*/

#define INTERFACE_DLL
#include "Debug/Assert.h"
#include "Interface/WindowUniverse.h"
#include "Interface/GameDocument.h"
#include "Display/PrimitiveLineStrip.h"
#include "Display/PrimitiveLineList.h"
#include "Display/PrimitiveMaterial.h"
#include "Display/PrimitiveWindow.h"

//----------------------------------------------------------------------------

const float UPDATE_UNIVERSE_RATE = 7.0f;

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( WindowUniverse, NodeWindow );
REGISTER_FACTORY_KEY( WindowUniverse, 4308899575499507265 );

WindowUniverse::WindowUniverse()
{
	m_Time = UPDATE_UNIVERSE_RATE;
	m_Destination = NULL_WIDGET;
	m_Highlighted = NULL_WIDGET;
}

//----------------------------------------------------------------------------

static CharString GetKey( const char * pKey, const char * pString )
{
	CharString source( pString );

	CharString token;
	while( source.tokenize( token, ";" ) )
	{
		CharString key;
		if ( token.tokenize( key, "=" ) )
		{
			if ( stricmp( key, pKey ) == 0 )
				return token;
		}
	}
	return "";
}

template<class T>
inline T sqr( T number )
{
	return number * number;
}

//----------------------------------------------------------------------------

void WindowUniverse::onUpdate( float t )
{
	m_Time += t;
	if ( m_Time > UPDATE_UNIVERSE_RATE )
	{
		GameDocument * pDoc = (GameDocument *)document();
		ASSERT( pDoc );

		m_Context.release();
		m_Name.release();
		m_Labels.release();
		m_Colors.release();
		m_Position.release();
		m_Links.release();

		// gather information on the systems
		if ( pDoc->linkCount() > 0 )
		{
			for(int i=0;i<pDoc->linkCount();i++)
			{
				GameServer::ServerLink link;
				link = pDoc->link( i );

				m_Context.push( link.context );
				
				CharString name( GetKey( "ContextName", link.data ) );
				m_Name.push( name );

				CharString label( "%s : %u / %u", name,
					link.clients, link.maxClients );
				m_Labels.push( label );

				m_Position.push( Vector3( atof( GetKey( "X", link.data ) ), 
					atof( GetKey( "Y", link.data ) ),
					atof( GetKey( "Z", link.data ) ) ) );

				Color color( YELLOW );
				for(int j=0;j<FACTION_COUNT;j++)
				{
					if ( j == FACTION_NEUTRAL )
						continue;

					int planets = atoi( GetKey( CharString("FP%d",j), link.data ) );
					if ( planets > 0 && j != pDoc->faction() )
					{
						// system is enemy or some planets are owned by another faction, so show it in red
						color = RED;
						break;
					}
					else if ( planets > 0 && j == pDoc->faction() )
						color = GREEN;
				}

				m_Colors.push( color );
			}

			// resolve the links
			for(i=0;i<pDoc->linkCount();i++)
			{
				GameServer::ServerLink link;
				link = pDoc->link( i );

				int linkCount = atoi( GetKey( "LC", link.data ) );
				for(int j=0;j<linkCount;j++)
				{
					CharString context( GetKey( CharString("L%d", j), link.data ) );
					if ( context.length() > 0 )
					{
						int server = m_Context.search( WidgetKey( context ) );
						if ( server >= 0 && server != i )
						{
							//TRACE( CharString("%s -> %s", link.name, pDoc->link( server ).name) );
							m_Links[ i ].push( server );
						}
					}
				}
			}
		}
		else
		{
			m_Context.push( pDoc->context()->key() );

			CharString name( pDoc->context()->starName() );
			m_Name.push( name );
			m_Labels.push( CharString("%s : No Links", name) );
			m_Position.push( Vector3( 0.0f ) );
			m_Colors.push( WHITE );
		}

		m_Extents = 1.0f;
		for(int i=0;i<m_Position.size();i++)
		{
			float range = m_Position[i].magnitude();
			if ( range > m_Extents )
				m_Extents = range;
		}

		m_Time = 0.0f;
	}
}

const RectInt	STAR_BOX(-8,-8,8,8);
const RectInt	POSITION_BOX(-9,-9,9,9);
const RectInt	SELECT_BOX(-7,-7,7,7);
const RectFloat	STAR_UV(0,0,1,1);
const Color		JUMP_COLOR( SKYBLUE );

void WindowUniverse::onRender( RenderContext & context, const RectInt & window )
{
	GameDocument * pDoc = (GameDocument *)document();
	if ( pDoc == NULL )
		return;		// no valid document

	Font * pFont = windowStyle()->font();
	ASSERT( pFont );

	bool blink = (pDoc->tick() % 20) > 5;

	// get the star icon
	Material * pStar = WidgetCast<Material>( resource( "STAR" ) );

	// get the window center
	PointInt center( window.center() );
	// calculate the scale
	float pixels = Min( window.height(), window.width() );
	float scale = pixels / (m_Extents * 2.0f);

	// project into screen space
	m_Position2.allocate( m_Position.size() );
	for(int i=0;i<m_Position.size();i++)
	{
		m_Position2[ i ] = PointInt( center.x + (m_Position[i].x * scale),
			center.y - (m_Position[i].z * scale) );
	}

	// push alpha material, no lighting
	PrimitiveMaterial::push( context.display(), PrimitiveMaterial::ALPHA, false );

	int closestRange = 5000;
	int closest = -1;
	int current = -1;
	int selected = -1;

	// render all the links first
	for(i=0;i<m_Position2.size();i++)
	{
		PointInt src( m_Position2[i] );

		int range = sqr(src.x - m_CursorPosition.x) + sqr(src.y - m_CursorPosition.y);
		if ( range < closestRange )
		{
			closest = i;
			closestRange = range;
		}
		
		if ( m_Context[i] == pDoc->context()->key() )
			current = i;
		if ( m_Context[i] == m_Destination )
			selected = i;

		Array< int > & links = m_Links[ i ];
		for(int j=0;j<links.size();j++)
		{
			PointInt dst( m_Position2[ links[j] ] );

			LineTL line( VertexTL( Vector3( src.x,src.y,0 ), 0, JUMP_COLOR, 0, 0 ),
				VertexTL( Vector3( dst.x,dst.y,0 ), 0, JUMP_COLOR, 0, 0 ) );
			PrimitiveLineListDTL::push( context.display(), 1, &line );
		}
	}

	if ( closest >= 0 )
		m_Highlighted = m_Context[ closest ];
	else
		m_Highlighted = NULL_WIDGET;

	if ( closest >= 0 && current >= 0 )
	{
		int next = renderPath( context, current, closest, blink ? GREEN : DARK_GREEN );
		if ( next >= 0 )
		{
			// find jumpgate leading to next system, set it as target
			BaseNode * pRoot = pDoc->context()->universe()->node();
			for(int i=0;i<pRoot->childCount();i++)
			{
				NounJumpGate * pGate = WidgetCast<NounJumpGate>( pRoot->child(i) );
				if ( pGate != NULL && pGate->destinationContext() == m_Context[ next ] )
				{
					m_NextJump = pGate;
					break;
				}
			}
		}
	}

	if ( selected >= 0 && current >= 0 )
		renderPath( context, current, selected, blink ? WHITE : GREY );

	// render the stars
	for(i=0;i<m_Position2.size();i++)
	{
		PointInt point( m_Position2[i] );

		// draw the star
		Material::push( context, pStar );
		PrimitiveWindow::push( context.display(), STAR_BOX + point, STAR_UV, m_Colors[ i ] );

		// drawn outline around current system
		if ( current == i )
		{
			PrimitiveMaterial::push( context.display(), PrimitiveMaterial::ALPHA, false );
			PrimitiveLineStripDTL::push( context.display(), STAR_BOX + point, blink ? WHITE : GREY );
		}

		CharString label( m_Labels[i] );
		SizeInt labelSize( pFont->size( label ) );
		PointInt labelPos( point + PointInt( -labelSize.width / 2, 10 ) );

		Font::push( context.display(), pFont, labelPos, label, DARK_YELLOW );
	}

	// highlight destination
	if ( closest >= 0 )
	{
		PointInt point( m_Position2[ closest] );
		PrimitiveMaterial::push( context.display(), PrimitiveMaterial::ALPHA, false );
		PrimitiveLineStripDTL::createPushDiamond( context.display(), point, 8, blink ? GREEN : DARK_GREEN );
	}
	// highlight current selection
	if ( selected >= 0 )
	{
		PointInt point( m_Position2[ selected ] );
		PrimitiveMaterial::push( context.display(), PrimitiveMaterial::ALPHA, false );
		PrimitiveLineStripDTL::createPushDiamond( context.display(), point, 8, blink ? WHITE : GREY );
	}
}

bool WindowUniverse::onCursorMove( const RectInt & window, 
						const PointInt & position,
						const PointInt & delta )
{
	if ( window.inRect( position ) )
		m_CursorPosition = windowToScreen( position );
	return false;
}

bool WindowUniverse::onRightDown( const RectInt & window,
						const PointInt & position )
{
	if ( window.inRect( position ) )
	{
		((NodeWindow *)parent())->hideWindow();
		return true;
	}
	return false;
}

bool WindowUniverse::onLeftDown( const RectInt & window,
						const PointInt & position )
{
	GameDocument * pDoc = (GameDocument *)document();
	ASSERT( pDoc );

	if ( window.inRect( position ) && m_Highlighted != NULL_WIDGET )
	{
		int selected = m_Context.search( m_Highlighted );
		if ( selected >= 0 )
		{
			m_Destination = m_Highlighted;
			pDoc->setTarget( m_NextJump );
			pDoc->pushChat( CharString("<color;ffffff>Jump gate to %s system targeted...</color>", m_Name[ selected ]) );
		}
		return true;
	}

	return false;
}

//----------------------------------------------------------------------------

int WindowUniverse::renderPath( RenderContext & context, int src, int dst, Color color )
{
	int rv = -1;
	if ( src >= 0 && dst >= 0 )
	{
		// A* - find path from current position to destination, draw lines to mark path
		Array< int > path( m_Context.size() );
		for(int i=0;i<path.size();i++)
			path[ i ] = -1;

		path[ dst ] = 0;

		Queue< int > queue;
		queue.push( dst );

		while( queue.valid() )
		{
			int system = *queue;
			queue.pop();
			
			if ( system == src )
				break;

			int step = path[ system ];

			// enumerate the links from this system
			Array< int > & links = m_Links[ system ];
			for(int j=0;j<links.size();j++)
			{
				int linked = links[ j ];
				if ( path[ linked ] < 0 )
				{
					path[ linked ] = step + 1;
					queue.push( linked );
				}
			}
		}

		// no path found from destination to source
		if ( path[ src ] < 0 )
			return -1;

		int nextStep = 1000000;

		// draw path from src position to destination
		int system = src;
		while( system != dst )
		{
			int next = -1;

			Array< int > & links = m_Links[ system ];
			for(int j=0;j<links.size();j++)
			{
				int linked = links[ j ];
				if ( path[ linked ] >= 0 && path[ linked ] < nextStep )
				{
					next = linked;
					nextStep = path[ linked ];
				}
			}

			// check for no path, if so stop
			if ( next < 0 )
				break;

			// if this is the next system from the src system, return this next system
			if ( system == src )
				rv = next;

			// draw line 
			PointInt src( m_Position2[ system ] );
			PointInt dst( m_Position2[ next ] );

			LineTL line( VertexTL( Vector3( src.x,src.y,0 ), 0, color, 0, 0 ),
				VertexTL( Vector3( dst.x,dst.y,0 ), 0, color, 0, 0 ) );
			PrimitiveLineListDTL::push( context.display(), 1, &line );

			// next system
			system = next;
		}
	}
	return rv;
}

//----------------------------------------------------------------------------
//EOF
