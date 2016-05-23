
/*************************************************************************

ColorBtn.h header file

CColorBtn: A control panel like color picker
           by The Gremlin

Compatibility: Visual C++ 4.0 and up.

**************************************************************************/

#ifndef _COLORBTN_H_
#define _COLORBTN_H_

// In order to ease use, these values have been hard coded in colorbtn.rc
// This avoids the need for another header file.


#define IDD_COLORBTN                    (5100)
#define IDC_COLOR1                      (5101)
#define IDC_COLOR20                     (5120)
#define IDC_OTHER                       (5121)


// Utility class for the dialog

class CColorBtnDlg : public CDialog
{
// Construction
public:

	
	static COLORREF colors[20];
    static BYTE used[20];

    CButton *parent;
    int colorindex;

	CColorBtnDlg(CWnd* pParent = NULL);   // standard constructor

     
// Dialog Data
	//{{AFX_DATA(CColorBtnDlg)
	enum { IDD = IDD_COLORBTN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBtnDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorBtnDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnOther();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	void OnColor(UINT id);    
    void EndDialog( int nResult );

	DECLARE_MESSAGE_MAP()

};



class CColorBtn : public CButton
{
// Construction
public:

    CColorBtn();

    // The selected color,set this variable to initialize the 
    // button's color and read to see the results

    COLORREF currentcolor;

    // Use Serialize to store the color table in a file. Call at the document's Serialize()
    
	//static void Serialize( CArchive& ar );

    // If using Serialize, call Reset to reinitialize the color table at OnNewDocument()

	static void Reset();

    // Use Load and Store to save/restore the color table to/from the registry

	static BOOL Load();
    static BOOL Store();
	  


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBtn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorBtn)
	afx_msg void OnClicked();
	//}}AFX_MSG

    void DrawItem(LPDRAWITEMSTRUCT);

	DECLARE_MESSAGE_MAP()

private:

    // A number of pens and brushes needed to paint the button

    CPen *oldpen;
    CBrush *oldbrush;
    CPen blackpen;
    CPen dkgray;
    CPen whitepen;
    CBrush backbrush;
    CBrush nullbrush;
    CPen nullpen;
    


   CColorBtnDlg dlg;
};




#endif
