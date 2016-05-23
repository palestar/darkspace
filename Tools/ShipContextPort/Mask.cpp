// Mask.cpp : implementation file
//

#include "stdafx.h"
#include "Port.h"
#include "Mask.h"

/////////////////////////////////////////////////////////////////////////////
// CMask dialog


CMask::CMask(CWnd* pParent /*=NULL*/)
	: CDialog(CMask::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMask)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMask)
	DDX_Control(pDX, IDC_LIST2, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMask, CDialog)
	//{{AFX_MSG_MAP(CMask)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMask message handlers

BOOL CMask::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for(int i=0;i<m_Items.size();i++)
	{
		int listItem = m_List.InsertItem( 0, (CString)m_Items[i] );
		m_List.SetItemData( listItem, i );

		if ( (1 << i) & m_Mask )
			m_List.SetItemState( listItem, LVIS_SELECTED, LVIS_SELECTED );
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMask::OnOK() 
{
	m_Mask = 0;
	for(int i=0;i<m_List.GetItemCount();i++)
		if ( m_List.GetItemState( i, LVIS_SELECTED ) )
			m_Mask |= (1 << m_List.GetItemData( i ));
	
	CDialog::OnOK();
}
