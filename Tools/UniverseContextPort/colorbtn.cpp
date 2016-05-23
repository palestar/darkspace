// ColorBtn.cpp : implementation file

#include "stdafx.h"
#include "ColorBtn.h"

//#include "windef.h"		// COLOREF

// The color table, initialized to windows' 20 static colors

#undef RGB
COLORREF RGB( unsigned char r, unsigned char g, unsigned char b )
{
	return( r | (g << 8) | (b << 16) );
}

COLORREF CColorBtnDlg::colors[20] =
{
    RGB(0,0,0),
    RGB(128,0,0),
    RGB(0,128,0),
    RGB(128,128,0),
    RGB(0,0,128),
    RGB(128,0,128),
    RGB(0,128,128),
    RGB(192,192,192),
    RGB(192,220,192),
    RGB(166,202,240),
    RGB(255,251,240),
    RGB(160,160,164),
    RGB(128,128,128),
    RGB(255,0,0),
    RGB(0,255,0),
    RGB(255,255,0),
    RGB(0,0,255),
    RGB(255,0,255),
    RGB(0,255,255),
    RGB(255,255,255)
};

// MRU table. See notes for Reset()

BYTE CColorBtnDlg::used[20] =
{    
    1,3,5,7,9,11,13,15,17,19,20,18,16,14,12,10,8,6,4,2    
};

/////////////////////////////////////////////////////////////////////////////
// CColorBtn

CColorBtn::CColorBtn()
{
    currentcolor = RGB(255,255,255);

    dlg.parent = this;   // This will allow the dialog to position itself

    // Create the pens and brushes that we'll need to draw the button

    nullpen.CreateStockObject(NULL_PEN);
    blackpen.CreateStockObject(BLACK_PEN);
    whitepen.CreateStockObject(WHITE_PEN);

    nullbrush.CreateStockObject(NULL_BRUSH);
    backbrush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
    dkgray.CreatePen(PS_SOLID,1,RGB(128,128,128));         
}


CColorBtn::~CColorBtn()
{
}


BEGIN_MESSAGE_MAP(CColorBtn, CButton)
	//{{AFX_MSG_MAP(CColorBtn)	
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBtn message handlers


void CColorBtn::DrawItem(LPDRAWITEMSTRUCT lpd)
{
    // Draw the button

    CBrush colorbrush;

    CDC DC;

    DC.Attach(lpd->hDC);
    
    int top,left,bottom,right;

    // Store this for convinience

    top    = lpd->rcItem.top;
    left   = lpd->rcItem.left;
    bottom = lpd->rcItem.bottom;
    right  = lpd->rcItem.right;
                
    colorbrush.CreateSolidBrush(currentcolor);
    
    oldpen   = DC.SelectObject(&nullpen);
    oldbrush = DC.SelectObject(&backbrush);

    // Clear the background using the 3DFACE color.

    DC.Rectangle(&lpd->rcItem);

    // Draw the border

    if (!(lpd->itemState & ODS_SELECTED))
    {
        // Button is up

        DC.SelectObject(&blackpen);

        DC.MoveTo(left,bottom-1);
        DC.LineTo(right-1,bottom-1);
        DC.LineTo(right-1,top);

        DC.SelectObject(&dkgray);

        DC.MoveTo(left+1,bottom-2);
        DC.LineTo(right-2,bottom-2);
        DC.LineTo(right-2,top+1);

        DC.SelectObject(&whitepen);

        DC.LineTo(left+1,top+1);
        DC.LineTo(left+1,bottom-2);
        
    }
    else
    {
        // Button is down

        DC.SelectObject(&dkgray);            

        DC.MoveTo(left,bottom-1);

        DC.LineTo(left,top);
        DC.LineTo(right-1,top);

        DC.SelectObject(&whitepen);

        DC.MoveTo(right-1,top-1);

        DC.LineTo(right-1,bottom-1);
        DC.LineTo(left+1,bottom-1);

        DC.SelectObject(&blackpen);

        DC.MoveTo(left+1,bottom-2);
        DC.LineTo(left+1,top+1);
        DC.LineTo(right-2,top+1);

        // by moving this, we get the things inside the button
        // to draw themselves one pixel down and one to the right.
        // This completes the "pushed" effect

        left++;
        right++;
        bottom++;
        top++;

    }

    // The division

    DC.SelectObject(&whitepen);
    
    DC.MoveTo(right-10,top+4);
    DC.LineTo(right-10,bottom-4);

    DC.SelectObject(dkgray);

    DC.MoveTo(right-11,top+4);
    DC.LineTo(right-11,bottom-4);

    // The triangle

    if (lpd->itemState & ODS_DISABLED)
        DC.SelectObject(dkgray);
    else
        DC.SelectObject(blackpen);
    
    DC.MoveTo(right-4,(bottom/2)-1);
    DC.LineTo(right-9,(bottom/2)-1);

    DC.MoveTo(right-5,(bottom/2));
    DC.LineTo(right-8,(bottom/2));

    if (lpd->itemState & ODS_DISABLED)    
    {
        DC.SetPixel(right-4,(bottom/2)-1,RGB(255,255,255));
        DC.SetPixel(right-5,(bottom/2),RGB(255,255,255));
        DC.SetPixel(right-6,(bottom/2)+1,RGB(255,255,255));
    }
    else
    {
        DC.SetPixel(right-6,(bottom/2)+1,RGB(0,0,0));
    }

    if (!(lpd->itemState & ODS_DISABLED))
    {
        // The color rectangle, only if enabled

        DC.SelectObject(&colorbrush);

        DC.Rectangle(left+5,top+4,right-15,bottom-4);    
    }

    if (lpd->itemState & ODS_FOCUS)
    {
        // Draw the focus
        //
        // It would have been nice just to
        // draw a rectangle using a pen created
        // with the PS_ALTERNATE style, but
        // this is not supported by WIN95

        int i;

        for (i=left+3;i<right-4;i+=2)
        {
            DC.SetPixel(i,top+3,RGB(0,0,0));
            DC.SetPixel(i,bottom-4,RGB(0,0,0));
        }

        for (i=top+3;i<bottom-4;i+=2)
        {
            DC.SetPixel(left+3,i,RGB(0,0,0));
            DC.SetPixel(right-4,i,RGB(0,0,0));
        }       
    }
        
    DC.SelectObject(oldpen);
    DC.SelectObject(oldbrush);


    DC.Detach();    
}


void CColorBtn::OnClicked() 
{
    // When the button is clicked, show the dialog.

	if (dlg.DoModal() == IDOK)
    {
        currentcolor = CColorBtnDlg::colors[dlg.colorindex];
        InvalidateRect(NULL);
	}	
}

// Store and Load use an undocumented CWinApp function

BOOL CColorBtn::Store()
{
    return (AfxGetApp()->WriteProfileBinary(_T("ColorData"), _T("ColorTable"),(LPBYTE)CColorBtnDlg::colors,sizeof(COLORREF)*20) &&
            AfxGetApp()->WriteProfileBinary(_T("ColorData"), _T("MRU"),(LPBYTE)CColorBtnDlg::used,sizeof(BYTE)*20));

}

BOOL CColorBtn::Load()
{
    BYTE *data = NULL;
    UINT size;

    // This function allocates the memory it needs

    AfxGetApp()->GetProfileBinary(_T("ColorData"), _T("ColorTable"),&data,&size);	

    if (data)
    {
        // Copy the data into our table and get rid of the buffer

        memcpy((void *)CColorBtnDlg::colors,(void *)data,size);
        free((void *)data);

        AfxGetApp()->GetProfileBinary(_T("ColorData"), _T("MRU"),&data,&size);	

        if (data)
        {
            memcpy((void *)CColorBtnDlg::used,(void *)data,size);
            free((void *)data);
            return TRUE;
        }
        
    }

    // If the loading fails, back to the defaults
    
    Reset();

    return FALSE;
}


void CColorBtn::Reset()
{
    CColorBtnDlg::colors[0]  = RGB(0,0,0);
    CColorBtnDlg::colors[1]  = RGB(128,0,0);
    CColorBtnDlg::colors[2]  = RGB(0,128,0);
    CColorBtnDlg::colors[3]  = RGB(128,128,0);
    CColorBtnDlg::colors[4]  = RGB(0,0,128);
    CColorBtnDlg::colors[5]  = RGB(128,0,128);
    CColorBtnDlg::colors[6]  = RGB(0,128,128);
    CColorBtnDlg::colors[7]  = RGB(192,192,192);
    CColorBtnDlg::colors[8]  = RGB(192,220,192);
    CColorBtnDlg::colors[9]  = RGB(166,202,240);
    CColorBtnDlg::colors[10] = RGB(255,251,240);
    CColorBtnDlg::colors[11] = RGB(160,160,164);
    CColorBtnDlg::colors[12] = RGB(128,128,128);
    CColorBtnDlg::colors[13] = RGB(255,0,0);
    CColorBtnDlg::colors[14] = RGB(0,255,0);
    CColorBtnDlg::colors[15] = RGB(255,255,0);
    CColorBtnDlg::colors[16] = RGB(0,0,255);
    CColorBtnDlg::colors[17] = RGB(255,0,255);
    CColorBtnDlg::colors[18] = RGB(0,255,255);
    CColorBtnDlg::colors[19] = RGB(255,255,255);

    // This "colorful" (no pun intended) order ensures
    // that the colors at the center of the color table
    // will get replaced first. This preserves the white
    // and black colors even if they're not used (They'll
    // get replaced last).
    
    CColorBtnDlg::used[0]= 1;
    CColorBtnDlg::used[1]= 3;
    CColorBtnDlg::used[2]= 5;
    CColorBtnDlg::used[3]= 7;
    CColorBtnDlg::used[4]= 9;
    CColorBtnDlg::used[5]= 11;
    CColorBtnDlg::used[6]= 13;
    CColorBtnDlg::used[7]= 15;
    CColorBtnDlg::used[8]= 17;
    CColorBtnDlg::used[9]= 19;
    CColorBtnDlg::used[10]= 20;
    CColorBtnDlg::used[11]= 18;
    CColorBtnDlg::used[12]= 16;
    CColorBtnDlg::used[13]= 14;
    CColorBtnDlg::used[14]= 12;
    CColorBtnDlg::used[15]= 10;
    CColorBtnDlg::used[16]= 8;
    CColorBtnDlg::used[17]= 6;
    CColorBtnDlg::used[18]= 4;
    CColorBtnDlg::used[19]= 2;
}

//
//void CColorBtn::Serialize( CArchive& ar )
//{
//    if (ar.IsStoring())
//    {
//        ar.Write((void *)CColorBtnDlg::colors,sizeof(COLORREF)*20);
//        ar.Write((void *)CColorBtnDlg::used,sizeof(BYTE)*20);
//    }
//    else
//    {
//        ar.Read((void *)CColorBtnDlg::colors,sizeof(COLORREF)*20);
//        ar.Read((void *)CColorBtnDlg::used,sizeof(BYTE)*20);
//    }
//}
//
//


/////////////////////////////////////////////////////////////////////////////
// CColorBtnDlg dialog


CColorBtnDlg::CColorBtnDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorBtnDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorBtnDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CColorBtnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorBtnDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorBtnDlg, CDialog)
	//{{AFX_MSG_MAP(CColorBtnDlg)
    ON_BN_CLICKED(IDC_OTHER, OnOther)
	ON_WM_LBUTTONDOWN()	
	ON_WM_LBUTTONUP()
    ON_WM_DRAWITEM()	
	//}}AFX_MSG_MAP
    ON_COMMAND_RANGE(IDC_COLOR1,IDC_COLOR20,OnColor)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColorBtnDlg message handlers

BOOL CColorBtnDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    RECT r,r2;
	
	parent->GetWindowRect(&r);
    
    // Move the dialog to be below the button

    SetWindowPos(NULL,r.left,r.bottom,0,0,SWP_NOSIZE|SWP_NOZORDER);

    GetWindowRect(&r2);

    // Check to see if the dialog has a portion outside the
    // screen, if so, adjust.
    
    if (r2.bottom > GetSystemMetrics(SM_CYSCREEN))
    {   
        r2.top = r.top-(r2.bottom-r2.top);        
    }

    if (r2.right > GetSystemMetrics(SM_CXSCREEN))
    {
        r2.left = GetSystemMetrics(SM_CXSCREEN) - (r2.right-r2.left);
    }

    SetWindowPos(NULL,r2.left,r2.top,0,0,SWP_NOSIZE|SWP_NOZORDER);

    // Capture the mouse, this allows the dialog to close when
    // the user clicks outside.

    // Remember that the dialog has no "close" button.

    SetCapture();
	
	return TRUE; 
}



void CColorBtnDlg::EndDialog( int nResult )
{
    ReleaseCapture();

    CDialog::EndDialog(nResult);
}


void CColorBtnDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
    RECT r;

    POINT p;
    p.x = point.x;
    p.y = point.y;

    ClientToScreen(&p);

    GetWindowRect(&r);

    // The user clicked...

    if (!PtInRect(&r,p))
    {
        //  ...outside the dialog, close.

        EndDialog(IDCANCEL);
    }
    else
    {
        //  ...inside the dialog. Since this window
        //     has the mouse captured, its children
        //     get no messages. So, check to see
        //     if the click was in one of its children
        //     and tell him.

        //     If the user clicks inside the dialog
        //     but not on any of the controls,
        //     ChildWindowFromPoint returns a
        //     pointer to the dialog. In this
        //     case we do not resend the message
        //     (obviously) because it would cause
        //     a stack overflow.
        
        CWnd *child = ChildWindowFromPoint(point);

        if (child && child != this)
            child->SendMessage(WM_LBUTTONDOWN,0,0l);
    }
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CColorBtnDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpd) 
{
	CDC dc;
    CPen nullpen;
    CBrush brush;
    CPen *oldpen;
    CBrush *oldbrush;

    // Draw the wells using the current color table

    nullpen.CreateStockObject(NULL_PEN);
    brush.CreateSolidBrush(colors[nIDCtl-IDC_COLOR1]);

    dc.Attach(lpd->hDC);

    oldpen = dc.SelectObject(&nullpen);
    oldbrush = dc.SelectObject(&brush);

    lpd->rcItem.right++;
    lpd->rcItem.bottom++;

    dc.Rectangle(&lpd->rcItem);

    dc.SelectObject(oldpen);
    dc.SelectObject(oldbrush);

    dc.Detach();
	
	CDialog::OnDrawItem(nIDCtl, lpd);
}


void CColorBtnDlg::OnColor(UINT id)
{

    // A well has been clicked, set the color index
    // and close.

    colorindex = id-IDC_COLOR1;
    
    int i;

    // This color is now the MRU

    for (i=0;i<20;i++)
    {
        if (used[colorindex] > used[i])
        {
            used[i]++;
        }
    }

    used[colorindex] = 1;

    EndDialog(IDOK);

}

void CColorBtnDlg::OnOther() 
{
int i;
COLORREF newcolor;

    // The "Other" button.

    ReleaseCapture();

	CColorDialog dlg;

    dlg.m_cc.Flags |= CC_FULLOPEN;


    if (dlg.DoModal() == IDOK)
    {
        // The user clicked OK.
        // set the color and close
        
        newcolor = dlg.GetColor();            

        // Check to see if the selected color is
        // already in the table.

        colorindex = -1;

        for (i=0;i<20;i++)
        {
            if (colors[i] == newcolor)
            {
                colorindex = i;
                break;
            }
        }

        // If the color was not found,
        // replace the LRU with this color
 
        if (colorindex == -1)
        {
            for (i=0;i<20;i++)
            {
                if (used[i] == 20)
                {
                    colors[i] = newcolor;
                    colorindex = i;                                 
                    break;
                }
            }
        }

        // This is the new MRU

        for (i=0;i<20;i++)
        {
            if (used[colorindex] > used[i])
            {
                used[i]++;
            }         
        }

        used[colorindex] = 1;

        EndDialog(IDOK);

        return;
    }

    // If the user clicked "Cancel" reclaim the mouse capture.

    SetCapture();        	
}


void CColorBtnDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{

    // See notes for OnLButtonDown.
    
    CWnd *child = ChildWindowFromPoint(point,CWP_ALL);
    
    if (child && child != this)
        child->SendMessage(WM_LBUTTONDOWN,0,0l);	
	
	CDialog::OnLButtonUp(nFlags, point);
}


