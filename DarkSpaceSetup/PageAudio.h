#if !defined(AFX_PAGEAUDIO_H__9A798A27_10A4_47EB_B213_D78CB952FB5C__INCLUDED_)
#define AFX_PAGEAUDIO_H__9A798A27_10A4_47EB_B213_D78CB952FB5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageAudio.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageAudio dialog

class CPageAudio : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageAudio)

// Construction
public:
	CPageAudio();
	~CPageAudio();

// Dialog Data
	//{{AFX_DATA(CPageAudio)
	enum { IDD = IDD_PAGE_AUDIO };
	CComboBox	m_AudioVoices;
	CComboBox	m_AudioChan;
	CComboBox	m_AudioKhz;
	int		m_SoundVolume;
	int		m_MusicVolume;
	CString	m_SoundVolumePercent;
	CString	m_MusicVolumePercent;
	CString	m_MusicDirectory;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageAudio)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageAudio)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowseMusic();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEAUDIO_H__9A798A27_10A4_47EB_B213_D78CB952FB5C__INCLUDED_)
