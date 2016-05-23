/*
	NounGadget.cpp

	This object is the base class for any object that can be mounted onto another noun
	(c)1999 Palestar Development, Richard Lyle
*/


#include "Debug/Assert.h"
#include "Standard/Limits.h"
#include "NounShip.h"
#include "NounStructure.h"
#include "NounProjectile.h"
#include "SceneryEffect.h"
#include "NounGadget.h"
#include "NounFighter.h"
#include "resource.h"
#include "GameContext.h"
#include "FactionInfo.h"

//----------------------------------------------------------------------------

bool NounGadget::sm_bRenderGadgets = false;				// render the gadgets

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( NounGadget, NounGame );
REGISTER_FACTORY_KEY( NounGadget, 4040930398244108078LL );

BEGIN_PROPERTY_LIST( NounGadget, NounGame )
	CLEAR_PROPERTY_FLAGS( m_Position, PF_UPDATE );
	CLEAR_PROPERTY_FLAGS( m_vOrientation, PF_UPDATE );
	ADD_REPLICATED_PROPERTY( m_nDamage, TICKS_PER_SECOND * 5 );
	ADD_TRANSMIT_UPDATE_PROPERTY( m_nGroup );
END_PROPERTY_LIST();

NounGadget::NounGadget() : m_nLevel( 0 ), m_nDamage( 0 ), m_nGroup( 0 ), m_nCooldownTick( 0 )
{
	m_Flags |= FLAG_MOUNT_FORE;			// default to fore mounted
}

//----------------------------------------------------------------------------

void NounGadget::render( RenderContext &context, 
							const Matrix33 & frame, 
							const Vector3 & position )
{
	if ( sm_bRenderGadgets )
		Noun::render( context, frame, position );
	else
		NodeTransform::render( context, frame, position );
}

//----------------------------------------------------------------------------

NounType NounGadget::nounType() const
{
	return TYPE_LOCAL_OBJECT;
}

bool NounGadget::canBeDetected() const
{
	return true;
}

float NounGadget::baseSignature() const
{
	return addSignature();	// return the amount of signature we would add to the ship as our signature
}

void NounGadget::setup()
{
	Noun::setup();
	// reset this gadget
	m_nDamage = 0;
	// set the default group
	m_nGroup = typeGroup( type() );
}

bool NounGadget::postInitialize()
{
	if (! Noun::postInitialize() )
		return false;

	if ( m_NounContext.valid() )
	{
		// pre-load assets..
		m_NounContext->resource( "USE", false );
		m_NounContext->resource( "PROJECTILE", false );
		m_NounContext->resource( "TRACER", false );
		m_NounContext->resource( "HIT", false );
	}

	return true;
}

void NounGadget::inflictDamage( dword nWhen, Noun * pFrom, int damage, dword type, const Vector3 & direction )
{
	if ( canDamage( type ) )
	{
		LOG_DEBUG_LOW( "NounGadget", CharString().format("Damaged inflicted, When: %u, From: %s, Damage: %d, Type: 0x%X", 
			nWhen, pFrom != NULL ? pFrom->name() : "NULL", damage, type ) );

		// get destroyed state before we apply the damage
		//bool checkDestroyed = !destroyed();
		m_nDamage = Clamp( m_nDamage + damage, 0, maxDamage());

		//if ( checkDestroyed && destroyed() )
		//	message( CharString().format( "<color;ffffff>SERVER: %s Destroyed.", name() ) );
	}
}

//----------------------------------------------------------------------------

dword NounGadget::hotkey() const
{
	return 0;
}

NounGadget::Type NounGadget::type() const
{
	return UNKNOWN;
}

NounGadget::EnergyClass NounGadget::energyClass() const
{
	return ENERGY_CLASS_SPECIAL;		// return the lowest priority class by default
}

CharString NounGadget::status() const
{
	return CharString("");
}

CharString NounGadget::description() const
{
	CharString description;
	description += CharString().format( "<color;FFFFFF>Health: %d%%", int( damageRatioInv() * 100 ) );
	if ( WidgetCast<NounShip>( parent() ) )
		description += CharString().format( "\nMount: %s", NounGadget::mountText( flags() ).cstr() );
	description += CharString().format("\n<color;808080>%s</color>", nounContext()->description() );

	return description;
}

CharString NounGadget::useTip( Noun * pTarget, bool shift ) const
{
	return ""; 
}

bool NounGadget::usable( Noun * pTarget, bool shift ) const
{
	if ( tick() < m_nCooldownTick )
		return false;
	return true;
}

int NounGadget::usableWhen() const
{
	if ( cooldown() > 0 && tick() < m_nCooldownTick )
		return 100 - (((m_nCooldownTick - tick()) * 100) / cooldown());
	return 100;
}

int	NounGadget::useModeCount() const
{
	return 1;
}

int NounGadget::useMode() const
{
	return 0;
}

bool NounGadget::useActive() const
{
	return false;
}

bool NounGadget::useTarget() const
{
	return false;
}

bool NounGadget::canDamage( dword type ) const
{
	return (type & DAMAGE_EMP) != 0;
}

int NounGadget::maxDamage() const
{
	return 2500;
}

int NounGadget::durability() const
{
	return 5;
}

//----------------------------------------------------------------------------

int NounGadget::buildTechnology() const
{
	return 10;
}

int NounGadget::buildCost() const
{
	return 10;
}	

dword NounGadget::buildFlags() const
{
	return 0;
}

int NounGadget::buildTime() const
{
	return 30;
}

int NounGadget::cooldown() const
{
	return 0;
}

//----------------------------------------------------------------------------

float NounGadget::addMass() const
{
	return 0.0f;
}

int NounGadget::addValue() const
{
	return 1;
}

float NounGadget::addMaxVelocity() const
{
	return 0.0f;
}

float NounGadget::addThrust() const
{
	return 0.0f;
}

int NounGadget::addMaxDamage() const
{
	return 0;
}

int NounGadget::addMaxEnergy() const
{
	return 0;
}

float NounGadget::addSignature() const
{
	return 0.0f;
}

float NounGadget::addSensor() const
{
	return 0.0f;
}

float NounGadget::addVisibility() const
{
	return 0.0f;
}

int NounGadget::addCargo() const
{
	return 0;
}

bool NounGadget::needReload( Noun * pWhom ) const
{
	return false;
}

bool NounGadget::needRepair( Noun * pWhom ) const
{
	return m_nDamage > 0;
}

//----------------------------------------------------------------------------

bool NounGadget::repair( Noun * pWhom, int nDamage )
{
	m_nDamage = Clamp( m_nDamage - nDamage, 0, maxDamage() );
	return m_nDamage > 0;
}

bool NounGadget::reload( Noun * pWhom )
{
	return false;
}

void NounGadget::useMode( int mode )
{}

void NounGadget::use( dword when, Noun * pTarget, bool shift )
{
	m_nCooldownTick = when + cooldown();
}

int NounGadget::useEnergy( dword nTick, int energy )
{
	// use no energy by default
	return energy;
}

bool NounGadget::updateLogic()
{
	return true;
}

//----------------------------------------------------------------------------

void NounGadget::message( const char * pMessage, bool a_bAllowRemote/* = false */)
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );
	if ( pShip != NULL )
		pShip->message( pMessage, a_bAllowRemote );
}

Material * NounGadget::icon()
{
	if (! m_pIcon.valid() )
	{
		Resource * pResource = nounContext()->resource( "ICON" );
		if ( pResource )
		{
			m_pIcon = WidgetCast<Material>( pResource );
			if (! m_pIcon.valid() )
			{
				Image * pImage = WidgetCast<Image>( pResource );
				if ( pImage )
					m_pIcon = new Material( pImage, PrimitiveMaterial::ADDITIVE );
			}
		}

		if (! m_pIcon.valid() )
			m_pIcon = new Material( PrimitiveMaterial::ADDITIVE );		// create some material, if we found nothing attached to this gadget
	}

	return m_pIcon;
}

bool NounGadget::getUpgrades( int a_nFactionID, Array< NounGadget * > & upgrades ) const
{
	NounShip * pShip = WidgetCast<NounShip>( parent() );

	Scene::Ref pScene = WidgetCast<Scene>( nounContext()->resource( "UPGRADES" ) );
	if (! pScene.valid() )
		return false;

	BaseNode * pRoot = pScene->node();
	for(int i=0;i<pRoot->childCount();i++)
	{
		NounGadget * pGadget = WidgetCast<NounGadget>( pRoot->child(i) );
		if ( pGadget == NULL )
			continue;		// not a NounGadget object
		if ( pGadget->nounContext() == nounContext() )
			continue;		// gadgets are the same, not an upgrade then...
			
		FactionInfo * pFactionInfo = pGadget->nounContext()->findData<FactionInfo>( "FACTIONINFO" );
		if ( pFactionInfo != NULL && pFactionInfo->m_nFactionInfo != a_nFactionID )
			continue;

		upgrades.push( pGadget );
	}

	return upgrades.size() > 0;
}

bool NounGadget::checkFacing( const Vector3 & position ) const
{
	if ( WidgetCast<NounShip>( parent() ) )
	{
		if ( (flags() & FLAG_MOUNT_FULL) == FLAG_MOUNT_FULL )
			return true;

		NounShip * pShip = (NounShip *)parent();
		ASSERT( pShip );

		Vector3 direction( pShip->worldFrame() * (position - pShip->worldPosition()) );
		direction.y = 0.0f;
		direction.normalize();

		float angle = atan2( direction.x, direction.z );
		
		if ( (flags() & FLAG_MOUNT_FORE) && angle > -PI4 && angle < PI4 )
			return true;
		if ( (flags() & FLAG_MOUNT_RIGHT) && angle > PI4 && angle < (PI - PI4) )
			return true;
		if ( (flags() & FLAG_MOUNT_LEFT) && angle < -PI4 && angle > -(PI - PI4) )
			return true;
		if ( (flags() & FLAG_MOUNT_AFT) && (angle < -(PI - PI4) || angle > (PI - PI4)) )
			return true;

		return false;
	}
	else if ( WidgetCast<NounStructure>( parent() ) )
	{
		NounStructure * pStructure = (NounStructure *)parent();
		NounPlanet * pPlanet = WidgetCast<NounPlanet>( pStructure->parent() );

		if ( pPlanet != NULL )
		{
			Vector3 delta( pPlanet->worldFrame() * (position - pPlanet->worldPosition()) );
			if ( (delta | pStructure->position()) > 0.0f )
				return true;
		}

		// target is on the wrong side of the planet
		return false;
	}
	else if ( WidgetCast<NounFighter>( parent() ) )
		return true;

	return false;
}

int NounGadget::useDelay() const
{
	Scene * pUseEffect = useEffect();
	if ( pUseEffect != NULL )
	{
		int seg = pUseEffect->findSegment( "UseDelay" );
		if ( seg >= 0 )
		{
			const Scene::Segment & segment = pUseEffect->segment( seg );
			return (segment.endTime - segment.beginTime) * TICKS_PER_SECOND;
		}
	}

	return 0;
}

SceneryEffect * NounGadget::createUseEffect( bool attachToGadget /*= true*/ ) 
{
	Scene * pUseEffect = useEffect();
	if ( pUseEffect != NULL && context()->isClient() )
	{
		SceneryEffect * pUse = new SceneryEffect;
		pUse->setNounContext( new NounContext(pUseEffect) );

		if (! pUse->setSegment( "Effect" ) )
			pUse->setLife( 1.0f );

		// attach use effect to this gadget
		if (! attachToGadget )
		{
			pUse->setPosition( worldPosition() );
			pUse->setFrame( worldFrame() );

			context()->attachNoun( pUse );
		}
		else
			context()->attachNoun( pUse, this );


		return( pUse );
	}

	return( NULL );
}

NounBody * NounGadget::parentBody() const
{
	BaseNode * pParent = parent();
	while( pParent != NULL )
	{
		if ( WidgetCast<NounFighter>( pParent ) )
		{
			pParent = ((NounFighter *)pParent)->weapon();
			if ( pParent == NULL )
				return NULL;
		}
		if ( WidgetCast<NounBody>( pParent ) )
			return (NounBody *)pParent;

		pParent = pParent->parent();
	}

	return NULL;
}

bool NounGadget::isCloaked() const
{
	if ( WidgetCast<NounShip>( parent() ) )
		return ((NounShip *)parent())->testFlags( NounShip::FLAG_CLOAKED );
	return false;
}

bool NounGadget::isJumping() const
{
	if ( WidgetCast<NounShip>( parent() ) )
		return ((NounShip *)parent())->testFlags( NounShip::FLAG_JUMPING );
	return false;
}

bool NounGadget::isSafeZone() const
{
	if ( WidgetCast<NounShip>( parent() ) )
		return ((NounShip *)parent())->testFlags( NounShip::FLAG_IN_SAFE_ZONE );
	return false;
}

float NounGadget::calculateModifier( ModifierType a_eType,
									  bool a_bInvert /*= false*/ ) const
{
	NounShip * pParent = WidgetCast<NounShip>( parent() );
	if ( pParent != NULL )
		return pParent->calculateModifier( a_eType, a_bInvert );

	return 1.0f;
}

void NounGadget::onModifiersChanged()
{}

//----------------------------------------------------------------------------

dword NounGadget::typeGroup( Type nType )
{
	switch( nType )
	{
	case WEAPON:			return '1';
	case WEAPON_CLOSE:		return '2';
	case WEAPON_MEDIUM:		return '3';
	case WEAPON_RANGED:		return '3';
	case WEAPON_FIGHTER:	return '4';
	case WEAPON_MINE:		return '5';
	case WEAPON_BOMB:		return '6';
	}

	return 0;
}

inline void AppendMountString( CharString & sMounts, const char * pAppend )
{
	if ( sMounts.length() > 0 )
		sMounts += ",";
	sMounts += pAppend;
}

CharString NounGadget::mountText( dword nFlags )
{
	CharString sMount;
	
	sMount = "";
	if ( nFlags & FLAG_MOUNT_FORE )
		AppendMountString( sMount, "FORE" );
	if ( nFlags & FLAG_MOUNT_RIGHT )
		AppendMountString( sMount, "RIGHT" );
	if ( nFlags & FLAG_MOUNT_LEFT )
		AppendMountString( sMount, "LEFT" );
	if ( nFlags & FLAG_MOUNT_AFT )
		AppendMountString( sMount, "AFT" );
	
	return sMount;
}

dword NounGadget::facingMountFlags( Facing face )
{
	static dword FACING_FLAGS[] =
	{
		FLAG_MOUNT_FORE,	// FRONT
		FLAG_MOUNT_RIGHT,	// RIGHT
		FLAG_MOUNT_LEFT,	// LEFT
		FLAG_MOUNT_AFT,		// BACK
	};

	return FACING_FLAGS[ face ];
}


//----------------------------------------------------------------------------
// EOF

