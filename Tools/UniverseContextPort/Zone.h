#pragma once

#include "Resource.h"
#include "Port.h"

// CZone dialog

class CZone : public CDialog
{
	DECLARE_DYNAMIC(CZone)

public:
	CZone(CWnd* pParent = NULL);   // standard constructor
	virtual ~CZone();

// Dialog Data
	enum { IDD = IDD_ZONE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_fX;
	float m_fY;
	float m_fZ;
	CString m_sScenePort;
	afx_msg void OnOpenScene();
	afx_msg void OnBrowseScene();
};
