/*
	ViewOptions.h
	(c)2000 Palestar, Richard Lyle
*/

#ifndef VIEWOPTIONS_H
#define VIEWOPTIONS_H

#include "System/Messages.h"
#include "Gui3d/WindowView.h"
#include "SliderHUD.h"

//----------------------------------------------------------------------------

class ViewOptions : public WindowView::View
{
public:
	DECLARE_WIDGET_CLASS();

	// Types
	typedef Reference< ViewOptions >	Ref;

	// Construction
						ViewOptions();
	// View interface
	void				onActivate();
	void				onDeactivate();
	void				onUpdate( float t );
	bool				onMessage( const Message & msg );
	void				onDocumentUpdate();

	// {{BEGIN_DATA
	WindowButton *				m_pButtonDisableShaders;
	WindowButton *				m_pButtonShadows;
	WindowButton *				m_pButtonSoftCursor;
	SliderHUD *			m_pBloomSlider;
	WindowButton *		m_pButtonHDR;
	WindowButton *		m_pButtonVideo;
	WindowButton *		m_pButtonAudio;
	WindowList *		m_pVideoModeList;
	SliderHUD *			m_pTrailsSlider;
	WindowText *		m_pTextTrails;
	SliderHUD *			m_pParticlesSlider;
	WindowText *		m_pTextParticles;
	SliderHUD *			m_pGraphicsSlider;
	WindowText *		m_pTextGraphics;
	WindowText *		m_pTextTextures;
	SliderHUD *			m_pTexturesSlider;
	WindowButton *		m_pButtonWindowed;
	WindowText *		m_pVideoOptions;
	SliderHUD *			m_pSFXVolumeSlider;
	SliderHUD *			m_pMusicVolumeSlider;
	WindowText *		m_pTextRemap;
	NodeWindow *		m_pDialogRemap;
	WindowList *		m_pListKeys;
	NodeWindow *		m_pKeyboardOptions;
	WindowText *		m_pAudioOptions;
	WindowText *		m_pColorOptions;
	WindowButton *		m_pButtonKeyboard;
	WindowText *		m_pTextSoundVolume;
	WindowText *		m_pTextMusicVolume;
	// END_DATA}}

	// {{BEGIN_MSG
	bool				onButtonDisableShaders(const Message & msg);
	bool				onButtonShadows(const Message & msg);
	bool				onButtonSoftCursor(const Message & msg);
	bool				onBloomSlider(const Message & msg);
	bool				onButtonHDR(const Message & msg);
	bool				onButtonVideo(const Message & msg);
	bool				onButtonAudio(const Message & msg);
	bool				onVideoModeList(const Message & msg);
	bool				onTrailsSlider(const Message & msg);
	bool				onParticlesSlider(const Message & msg);
	bool				onGraphicsSlider(const Message & msg);
	bool				onTexturesSlider(const Message & msg);
	bool				onButtonWindowed(const Message & msg);
	bool				onSFXVolumeSlider(const Message & msg);
	bool				onMusicVolumeSlider(const Message & msg);
	bool				onButtonCancelRemap(const Message & msg);
	bool				onButtonRemapKey(const Message & msg);
	bool				onButtonResetKeys(const Message & msg);
	bool				onButtonKeyboard(const Message & msg);
	bool				onButtonBack(const Message & msg);
	// END_MSG}}

	dword				m_KeyRemap;
	void				updateKeyList();
};

//----------------------------------------------------------------------------



#endif

//----------------------------------------------------------------------------
// EOF
