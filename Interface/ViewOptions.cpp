/*
	ViewOptions.cpp
	(c)2000 Palestar, Richard Lyle
*/

#include "Debug/Assert.h"
#include "System/Keyboard.h"
#include "Standard/Settings.h"
#include "Interface/ViewOptions.h"
#include "Render3D/NodeBoolean.h"
#include "Render3D/NodeParticleSystem.h"
#include "Render3D/NodeComplexMesh2.h"
#include "Gui3d/InterfaceContext.h"
#include "System/Platform.h"
#include "DarkSpace/Trail.h"
#include "DarkSpace/NounProjectile.h"

#include "GameDocument.h"

//----------------------------------------------------------------------------

#if defined(_DEBUG)
const char * CLIENT_SETTINGS = "ClientD";
#else
const char * CLIENT_SETTINGS = "Client";
#endif

struct Mapping
{
	const char *	pName;
	dword			key;
};

static Mapping KEYBOARD_MAP[] = 
{
	{ "Help",							HK_F1 },
	{ "Navigation",						HK_F2 },
	{ "Planets",						HK_F3 },
	{ "Communications Log",				HK_F4 },
	{ "Profile",						HK_F5 },
	{ "Enhancements",					HK_F7 },
	
	{ "Turn Left",						'A' },
	{ "Stop Turn",						'S' },
	{ "Turn Right",						'D' },
	{ "Increase Velocity",				'W' },
	{ "Decrease Velocity",				'Q' },
	{ "Zoom In",						'Z' },
	{ "Zoom Out",						'X' },
	{ "Chat",							HK_RETURN },
	{ "Decrease contact range",			219 },
	{ "Increase contact range",			221 },
	{ "Toggle Shield/Turn Display",		191 },
	{ "Toggle Ship Status Display",		HK_NUMPAD7 },
	{ "Toggle Left Console",			HK_NUMPAD1 },
	{ "Toggle Right Console",			HK_NUMPAD3 },
	{ "Toggle Camera Mode",				HK_TAB },

	{ "Follow",							'F' },
	{ "Shadow",							'G' },
	{ "Orbit",							'O' },
	{ "Load",							'L' },
	{ "Unload",							'U' },

	{ "Target Nearest Enemy",			'E' },
	{ "Target Nearest Friendly",		'R' },
	{ "Closest Supply/Depot",			HMK_CONTROL_MASK | 'R' },
	{ "Closest Friendly Jumpgate",		HMK_CONTROL_MASK | 'J' },
	{ "Target Last Distress",			'H' },
	{ "Send Distress",					HMK_CONTROL_MASK | 'H' },
	{ "Send Supply Distress",			HMK_SHIFT_MASK | 'H' },
	{ "Self Destruct",					HMK_CONTROL_MASK | 'D' },
	{ "Toggle Point Defense",			HMK_CONTROL_MASK | 'B' },

	{ "Tractor Beam",					'T' },
	{ "Mining Beam",					'G' },
	{ "Bombs/Mines",					'B' },
	{ "Explode Bombs/Mines",			HMK_SHIFT_MASK | 'B' },
	{ "Jump Drive",						'J' },
	{ "Emergency Jump",					HMK_SHIFT_MASK | 'J' },
	{ "ECM",							'M' },
	{ "ECCM",							'P' },
	{ "Scanner",						'K' },
	{ "Cloak/Pulse Wave/Flux Wave",		'V' },
	{ "Reload",							'Y' },
	{ "Reload Self",					HMK_SHIFT_MASK | 'Y' },
	{ "Fighters",						'N' },
	{ "Recall Fighters",				HMK_SHIFT_MASK | 'N' },

	{ "Increase Full Shields",			HK_NUMPAD5 },
	{ "Increase Front Shields",			HK_NUMPAD8 },
	{ "Increase Right Shields",			HK_NUMPAD6 },
	{ "Increase Aft Shields",			HK_NUMPAD2 },
	{ "Increase Left Shields",			HK_NUMPAD4 },
	{ "Toggle Shields",					HMK_CONTROL_MASK | 'S' },
	{ "Interdictor",					'I' },
	

	{ NULL,								0 }		// leave as last item
};

// this is a list of keys that cannot be assigned to a function
static dword KEYBOARD_EXCLUDE[] = 
{
	HK_ESCAPE,		// escape always should be escape
	HK_SPACE,		// ignore space

	HMK_SHIFT_MASK | HK_SHIFT,
	HMK_CONTROL_MASK | HK_CONTROL,
	HMK_ALT_MASK | HK_MENU,

	'1',			// ignore weapon grouping key assigments
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',

	0
};

static float TextToScale( const CharString & sText )
{
	if ( sText == "HIGH" )
		return 1.0f;
	else if ( sText == "LOW" )
		return 0.0f;

	return 0.5f;	// MEDIUM
}

static const char * ScaleToText( float fScale )
{
	if ( fScale > 0.75f )
		return "HIGH";
	else if ( fScale < 0.25 )
		return "LOW";

	return "MEDIUM";
}

static float DetailToScale( NodeBoolean::DetailLevel eLevel )
{
	if ( eLevel == NodeBoolean::LOW )
		return 0.0f;
	else if ( eLevel == NodeBoolean::HIGH )
		return 1.0f;

	return 0.5f;	// NodeBoolean::MEDIUM
}

static NodeBoolean::DetailLevel ScaleToDetal( float fScale )
{
	if ( fScale > 0.75f )
		return NodeBoolean::HIGH;
	else if ( fScale < 0.25 )
		return NodeBoolean::LOW;

	return NodeBoolean::MEDIUM;
}

//----------------------------------------------------------------------------

IMPLEMENT_FACTORY( ViewOptions, WindowView::View );
REGISTER_FACTORY_KEY( ViewOptions, 4346437673160472801 );

ViewOptions::ViewOptions() : m_KeyRemap( 0 )
{}

//----------------------------------------------------------------------------

void ViewOptions::onActivate()
{
	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	ASSERT( pDoc );

	//{{BEGIN_DATA_INIT
	DATA_MAP( m_pButtonDisableShaders, WindowButton, "ButtonDisableShaders" );
	DATA_MAP( m_pButtonShadows, WindowButton, "ButtonShadows" );
	DATA_MAP( m_pButtonSoftCursor, WindowButton, "ButtonSoftCursor" );
	DATA_MAP( m_pBloomSlider, SliderHUD, "BloomSlider" );
	DATA_MAP( m_pButtonHDR, WindowButton, "ButtonHDR" );
	DATA_MAP( m_pButtonVideo, WindowButton, "ButtonVideo" );
	DATA_MAP( m_pButtonAudio, WindowButton, "ButtonAudio" );
	DATA_MAP( m_pVideoModeList, WindowList, "VideoModeList" );
	DATA_MAP( m_pTrailsSlider, SliderHUD, "TrailsSlider" );
	DATA_MAP( m_pTextTrails, WindowText, "TextTrails" );
	DATA_MAP( m_pParticlesSlider, SliderHUD, "ParticlesSlider" );
	DATA_MAP( m_pTextParticles, WindowText, "TextParticles" );
	DATA_MAP( m_pGraphicsSlider, SliderHUD, "GraphicsSlider" );
	DATA_MAP( m_pTextGraphics, WindowText, "TextGraphics" );
	DATA_MAP( m_pTextTextures, WindowText, "TextTextures" );
	DATA_MAP( m_pTexturesSlider, SliderHUD, "TexturesSlider" );
	DATA_MAP( m_pButtonWindowed, WindowButton, "ButtonWindowed" );
	DATA_MAP( m_pVideoOptions, WindowText, "VideoOptions" );
	DATA_MAP( m_pSFXVolumeSlider, SliderHUD, "SFXVolumeSlider" );
	DATA_MAP( m_pMusicVolumeSlider, SliderHUD, "MusicVolumeSlider" );
	m_pTextRemap = WidgetCast<WindowText>( window()->findNode( "TextRemap" ) );
	m_pDialogRemap = WidgetCast<NodeWindow>( window()->findNode( "DialogRemap" ) );
	m_pListKeys = WidgetCast<WindowList>( window()->findNode( "ListKeys" ) );
	m_pKeyboardOptions = WidgetCast<NodeWindow>( window()->findNode( "KeyboardOptions" ) );
	m_pAudioOptions = WidgetCast<WindowText>( window()->findNode( "AudioOptions" ) );
	m_pButtonKeyboard = WidgetCast<WindowButton>( window()->findNode( "ButtonKeyboard" ) );
	m_pTextSoundVolume = WidgetCast<WindowText>( window()->findNode( "TextSoundVolume" ) );
	m_pTextMusicVolume = WidgetCast<WindowText>( window()->findNode( "TextMusicVolume" ) );
	//END_DATA_INIT}}

	Settings settings( CLIENT_SETTINGS );

	m_pDialogRemap->hideWindow();
	m_pKeyboardOptions->hideWindow();
	m_pAudioOptions->hideWindow();
	m_pVideoOptions->showWindow();

	// initialize the keyboard settings
	m_pListKeys->setSortEnable( false );
	m_pListKeys->addColumn( 300, WHITE );
	m_pListKeys->addColumn( 100, WHITE );
	m_KeyRemap = 0;

	// initialize the video settings
	CharString sDisplayMode( settings.get( "displayMode", "0:1024x768x32" ) );
	m_pVideoModeList->setSortEnable( false );
	m_pVideoModeList->addColumn( 300, WHITE );
	m_pVideoModeList->removeAllItems();

	DisplayDevice * pDisplay = context()->display();
	ASSERT( pDisplay );

	for(int i=0;i<pDisplay->modeCount();++i)
	{
		const DisplayDevice::Mode * pMode = pDisplay->mode( i );
		if ( pMode->screenSize.width < 800 || pMode->screenSize.height < 600 )
			continue;	// skip anything below 800x600

		int nItem = m_pVideoModeList->addItem( pMode->modeDescription );
		if ( pMode->modeDescription == sDisplayMode )
			m_pVideoModeList->setSelected( nItem );
	}

	m_pButtonWindowed->setButtonDown( settings.get( "windowed", (dword)0 ) != 0 );
	m_pButtonSoftCursor->setButtonDown( settings.get( "softwareCursor", (dword)0 ) != 0 );
	m_pButtonHDR->setButtonDown( settings.get( "HDR", 1 ) != 0 );
	m_pButtonShadows->setButtonDown( settings.get( "enableShadows", 1 ) != 0 );
	m_pButtonDisableShaders->setButtonDown( settings.get( "useFixedFunction", DisplayDevice::sm_bUseFixedFunction) != 0 );

	// set the texture detail
	m_pTexturesSlider->setPosition( TextToScale( settings.get( "textureDetail", "HIGH" ) ) );
	m_pGraphicsSlider->setPosition( DetailToScale( (NodeBoolean::DetailLevel)settings.get( "graphicsDetail", 1 ) ) );
	m_pParticlesSlider->setPosition( Clamp<float>( settings.get( "particleEffects", 100 ) / 100.0f, 0.0f, 1.0f ) );
	m_pTrailsSlider->setPosition( Clamp<float>( settings.get( "shipTrails", 100 ) / 100.0f, 0.0f, 1.0f ) );
	m_pBloomSlider->setPosition( Clamp<float>( settings.get( "bloomScale", 100 ) / 100.0f, 0.0f, 1.0f ) );

	// initialize the audio settings
	m_pMusicVolumeSlider->setPosition( Clamp<float>( settings.get( "musicVolume", 100 ) / 100.0f, 0.0f, 1.0f ) );
	m_pSFXVolumeSlider->setPosition( Clamp<float>( settings.get( "sfxVolume", 100 ) / 100.0f, 0.0f, 1.0f ) );
}

void ViewOptions::onDeactivate()
{
	Settings settings( CLIENT_SETTINGS );

	// make sure keymap is enabled before leaving view
	Keyboard::enableKeyMap();
	// save any changes to the keyboard mapping
	Keyboard::saveKeyMap( settings.get( "keyMap", ".\\keyboard.map" ) );
}

void ViewOptions::onUpdate( float t )
{
	m_pButtonKeyboard->setAlpha( Clamp( m_pButtonKeyboard->alpha() + (m_pKeyboardOptions->visible() ? t : -t), 0.25f, 1.0f ) );
	m_pButtonAudio->setAlpha( Clamp( m_pButtonAudio->alpha() + (m_pAudioOptions->visible() ? t : -t), 0.25f, 1.0f ) );
	m_pButtonVideo->setAlpha( Clamp( m_pButtonVideo->alpha() + (m_pVideoOptions->visible() ? t : -t), 0.25f, 1.0f ) );
}

bool ViewOptions::onMessage( const Message & msg )
{
	if ( m_KeyRemap != 0 && msg.message == HM_KEYDOWN )
	{
		dword key = msg.wparam & 0xff;
		if ( key ==	HK_SHIFT || key == HK_CONTROL || key == HK_MENU )
			return true;

		// make sure this isn't an ignore key
		int i = 0;
		while( KEYBOARD_EXCLUDE[ i ] != 0 )
		{
			if ( KEYBOARD_EXCLUDE[ i ] == msg.wparam )
				return true;
			i++;
		}

		Keyboard::enableKeyMap();
		Keyboard::remap( msg.wparam, m_KeyRemap );

		m_KeyRemap = 0;
		updateKeyList();

		m_pDialogRemap->hideWindow();

		return true;
	}

	//{{BEGIN_MSG_MAP
	MESSAGE_MAP( WB_BUTTONDOWN, 1294950518, onButtonDisableShaders);
	MESSAGE_MAP( WB_BUTTONUP, 1294950518, onButtonDisableShaders);
	MESSAGE_MAP( WB_BUTTONDOWN, 4248795755, onButtonShadows);
	MESSAGE_MAP( WB_BUTTONUP, 4248795755, onButtonShadows);
	MESSAGE_MAP( WB_BUTTONUP, 3104681820, onButtonSoftCursor);
	MESSAGE_MAP( WB_BUTTONDOWN, 3104681820, onButtonSoftCursor);
	MESSAGE_MAP( WS_END_SLIDE, 2089464636, onBloomSlider);
	MESSAGE_MAP( WB_BUTTONUP, 2597129717, onButtonHDR);
	MESSAGE_MAP( WB_BUTTONDOWN, 2597129717, onButtonHDR);
	MESSAGE_MAP( WB_BUTTONUP, 149801867, onButtonVideo);
	MESSAGE_MAP( WB_BUTTONUP, 657324939, onButtonAudio);
	MESSAGE_MAP( WL_SELECT, 1039690049, onVideoModeList);
	MESSAGE_MAP( WS_END_SLIDE, 884895945, onTrailsSlider);
	MESSAGE_MAP( WS_END_SLIDE, 959727917, onParticlesSlider);
	MESSAGE_MAP( WS_END_SLIDE, 1704619692, onGraphicsSlider);
	MESSAGE_MAP( WS_END_SLIDE, 4159507523, onTexturesSlider);
	MESSAGE_MAP( WB_BUTTONUP, 1743722996, onButtonWindowed);
	MESSAGE_MAP( WB_BUTTONDOWN, 1743722996, onButtonWindowed);
	MESSAGE_MAP( WS_END_SLIDE, 436959305, onSFXVolumeSlider);
	MESSAGE_MAP( WS_END_SLIDE, 335444927, onMusicVolumeSlider);
	MESSAGE_MAP( WB_BUTTONUP, 1760410273, onButtonCancelRemap);
	MESSAGE_MAP( WB_BUTTONUP, 2886803252, onButtonRemapKey);
	MESSAGE_MAP( WB_BUTTONUP, 2317609988, onButtonResetKeys);
	MESSAGE_MAP( WB_BUTTONUP, 2883869073, onButtonKeyboard);
	MESSAGE_MAP( WB_BUTTONUP, 1493398483, onButtonBack);
	//END_MSG_MAP}}

	return false;
}

void ViewOptions::onDocumentUpdate()
{
	// document data has changed, update this view if needed
}

//----------------------------------------------------------------------------

void ViewOptions::updateKeyList()
{
	int selected = m_pListKeys->selected();
	int highlighted = m_pListKeys->highlighted();

	m_pListKeys->removeAllItems();

	int i = 0;
	while( KEYBOARD_MAP[ i ].key != 0 )
	{
		dword key = Keyboard::unmap( KEYBOARD_MAP[ i ].key );
		if ( key != 0 )
		{
			int item = m_pListKeys->addItem( KEYBOARD_MAP[ i ].pName );
			m_pListKeys->setItem( item, 1, Keyboard::keyText( key ) );
		}
		else // no valid keyboard assignment
			m_pListKeys->addItem( CharString().format("<color;0000ff>%s</color>", KEYBOARD_MAP[ i ].pName ) );

		i++;
	}

	// restore selected/highlighted items
	m_pListKeys->setSelected( selected );
	m_pListKeys->setHighlighted( highlighted );
}	

//----------------------------------------------------------------------------

bool ViewOptions::onButtonBack(const Message & msg)
{
	Settings settings( CLIENT_SETTINGS );

	document()->setScene( "Main" );
	return true;
}

bool ViewOptions::onButtonKeyboard(const Message & msg)
{
	m_KeyRemap = 0;
	m_pDialogRemap->hideWindow();
	m_pKeyboardOptions->showWindow();
	m_pAudioOptions->hideWindow();
	m_pVideoOptions->hideWindow();

	updateKeyList();
	return true;
}

bool ViewOptions::onButtonAudio(const Message & msg)
{
	m_KeyRemap = 0;
	m_pDialogRemap->hideWindow();
	m_pKeyboardOptions->hideWindow();
	m_pAudioOptions->showWindow();
	m_pVideoOptions->hideWindow();
	return true;
}

bool ViewOptions::onButtonVideo(const Message & msg)
{
	m_KeyRemap = 0;
	m_pDialogRemap->hideWindow();
	m_pKeyboardOptions->hideWindow();
	m_pAudioOptions->hideWindow();
	m_pVideoOptions->showWindow();
	return true;
}

bool ViewOptions::onButtonResetKeys(const Message & msg)
{
	Keyboard::resetKeyMap();
	updateKeyList();
	return true;
}

bool ViewOptions::onButtonRemapKey(const Message & msg)
{
	int selected = m_pListKeys->selected();
	if ( selected >= 0 )
	{
		m_KeyRemap = KEYBOARD_MAP[ selected ].key;
		m_pTextRemap->setText( CharString().format("Press KEY assignment for %s", KEYBOARD_MAP[ selected ].pName) );
		m_pDialogRemap->showWindow();

		// disable the keymap
		Keyboard::disableKeyMap();
	}
	return true;
}

bool ViewOptions::onButtonCancelRemap(const Message & msg)
{
	// enable keyboard remapping
	Keyboard::enableKeyMap();
	// reset remap
	m_KeyRemap = 0;
	m_pDialogRemap->hideWindow();

	return true;
}

bool ViewOptions::onMusicVolumeSlider(const Message & msg)
{
	int nVolume = Clamp<int>( m_pMusicVolumeSlider->position() * 100, 0, 100 );

	GameDocument * pDoc = WidgetCast<GameDocument>( document() );
	if ( pDoc != NULL && pDoc->jukeBox() != NULL )
		pDoc->jukeBox()->setVolume( nVolume );

	Settings settings( CLIENT_SETTINGS );
	settings.put( "musicVolume", nVolume );
	return true;
}

bool ViewOptions::onSFXVolumeSlider(const Message & msg)
{
	int nVolume = Clamp<int>( m_pSFXVolumeSlider->position() * 100, 0, 100 );
	context()->audio()->setVolume(  nVolume / 100.0f );

	Settings settings( CLIENT_SETTINGS );
	settings.put( "sfxVolume", nVolume );
	return true;
}

bool ViewOptions::onButtonWindowed(const Message & msg)
{
	bool bWindowed = m_pButtonWindowed->buttonDown();

	DisplayDevice * pDisplay = context()->display();
	if ( pDisplay != NULL )
		pDisplay->setMode( pDisplay->activeMode(), bWindowed );

	Settings settings( CLIENT_SETTINGS );
	settings.put( "windowed", bWindowed );
	return true;
}

bool ViewOptions::onTexturesSlider(const Message & msg)
{
	float fTextureDetail = m_pTexturesSlider->position();
	CharString sTextureDetail = ScaleToText( fTextureDetail );
	m_pTexturesSlider->setPosition( TextToScale( sTextureDetail ) );

	if ( sTextureDetail == "LOW" )
		Material::sm_MaxTextureSize = SizeInt( 1024, 1024 );
	else if ( sTextureDetail == "MEDIUM" )
		Material::sm_MaxTextureSize = SizeInt( 2048, 2048 );
	else
		Material::sm_MaxTextureSize = SizeInt( 4096, 4096 );

	Settings settings( CLIENT_SETTINGS );
	settings.put( "textureDetail", sTextureDetail );
	return true;
}

bool ViewOptions::onGraphicsSlider(const Message & msg)
{
	float fGraphics = m_pGraphicsSlider->position();
	NodeBoolean::DetailLevel eLevel = ScaleToDetal( fGraphics );
	m_pGraphicsSlider->setPosition( DetailToScale( eLevel ) );

	NodeBoolean::s_DetailLevel = eLevel;

	if ( NodeBoolean::s_DetailLevel == NodeBoolean::LOW )
		RenderContext::sm_fDefaultDetail = 0.0f;
	else if ( NodeBoolean::s_DetailLevel == NodeBoolean::MEDIUM )
		RenderContext::sm_fDefaultDetail = 0.5f;
	else
		RenderContext::sm_fDefaultDetail = 1.0f;	// disabled all detail levels on models, show full details always

	Settings settings( CLIENT_SETTINGS );
	settings.put( "graphicsDetail", eLevel );
	return true;
}

bool ViewOptions::onParticlesSlider(const Message & msg)
{
	float fParticles = m_pParticlesSlider->position();
	NodeParticleSystem::sm_fDetail = fParticles;

	Settings settings( CLIENT_SETTINGS );
	settings.put( "particleEffects", fParticles * 100 );
	return true;
}

bool ViewOptions::onTrailsSlider(const Message & msg)
{
	float fTrails = m_pTrailsSlider->position();
	Trail::sm_fTrailScalar = NounProjectile::sm_fTrailScalar = fTrails;

	Settings settings( CLIENT_SETTINGS );
	settings.put( "shipTrails", fTrails * 100 );
	settings.put( "missileTrails", fTrails * 100 );
	return true;
}

bool ViewOptions::onVideoModeList(const Message & msg)
{
	DisplayDevice * pDisplay = context()->display();
	if (! pDisplay )
		return false;
	int nSelected = m_pVideoModeList->selected();
	if ( nSelected < 0 || nSelected >= pDisplay->modeCount() )
		return false;

	CharString sSelected = m_pVideoModeList->item( nSelected, 0 );
	for(int i=0;i<pDisplay->modeCount();++i)
	{
		const DisplayDevice::Mode * pMode = pDisplay->mode( i );
		if ( pMode->modeDescription == sSelected )
		{
			pDisplay->setMode( pMode, pDisplay->windowed() );
			break;
		}
	}

	Settings settings( CLIENT_SETTINGS );
	settings.put( "displayMode", sSelected );
	return true;
}

bool ViewOptions::onButtonHDR(const Message & msg)
{
	bool bHDR = m_pButtonHDR->buttonDown();

	GameContext::sm_bEnableHDR = bHDR;

	Settings settings( CLIENT_SETTINGS );
	settings.put( "HDR", bHDR );

	return true;
}

bool ViewOptions::onBloomSlider(const Message & msg)
{
	float fBloomScale = m_pBloomSlider->position();

	Settings settings( CLIENT_SETTINGS );
	settings.put( "bloomScale", fBloomScale * 100 );
	GameContext::sm_bUpdateHDR = true;

	return true;
}

bool ViewOptions::onButtonSoftCursor(const Message & msg)
{
	bool bSoftwareCursor = m_pButtonSoftCursor->buttonDown();

	Platform * pPlatform = Platform::sm_pPlatform;
	if ( pPlatform != NULL )
		pPlatform->setSoftwareCursor( bSoftwareCursor );

	Settings settings( CLIENT_SETTINGS );
	settings.put( "softwareCursor", bSoftwareCursor );
	return true;

}

bool ViewOptions::onButtonShadows(const Message & msg)
{
	bool bEnableShadows = m_pButtonShadows->buttonDown();

	WorldContext::sm_bEnableShadows = bEnableShadows;

	Settings settings( CLIENT_SETTINGS );
	settings.put( "enableShadows", bEnableShadows );

	return true;
}

bool ViewOptions::onButtonDisableShaders(const Message & msg)
{
	bool bDisableShaders = m_pButtonDisableShaders->buttonDown();

	DisplayDevice::sm_bUseFixedFunction = bDisableShaders;

	Settings settings( CLIENT_SETTINGS );
	settings.put( "useFixedFunction", bDisableShaders );

	return true;
}

