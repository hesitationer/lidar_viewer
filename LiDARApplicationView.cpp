// LiDARApplicationView.cpp : implementation of the CLiDARApplicationView class
//

#include "stdafx.h"
#include "LiDARApplication.h"

#include "LiDARApplicationDoc.h"
#include "LiDARApplicationView.h"
#include ".\lidarapplicationview.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLiDARApplicationView

IMPLEMENT_DYNCREATE(CLiDARApplicationView, CScrollView)

BEGIN_MESSAGE_MAP(CLiDARApplicationView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_DISPLAY_MAXIMUM, OnDisplayMaxHeightImage)
	ON_COMMAND(ID_DISPLAY_MINIMUM, OnDisplayMinHeightImage)
	ON_COMMAND(ID_DISPLAY_AVERAGE, OnDisplayAvgHeightImage)
	ON_COMMAND(ID_DISPLAY_DENSITY, OnDisplayDensityImage)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomout)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CLiDARApplicationView construction/destruction

CLiDARApplicationView::CLiDARApplicationView()
{
	// Initialize the zoom factor
	m_iZoomFactor = 0;

	// Initialize that mouse is not hold down.
	m_bMouseDown = false;

	m_xyLCoord.iX	= 0;	
	m_xyLCoord.iY	= 0;	
    m_xyOrigin.iX	= 0;
    m_xyOrigin.iY	= 0;
}

CLiDARApplicationView::~CLiDARApplicationView()
{
}

BOOL CLiDARApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CLiDARApplicationView drawing

void CLiDARApplicationView::OnDraw(CDC* pDC)
{
	CLiDARApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Get the pointer to the CLiDARImage object
	CLiDARImage * pImage = pDoc->GetImage();
	if (pImage == NULL)
		return;
	
	CSize	sizeImage;
	CRect	DrawingRect, ClientRect;
	
	// Get the client area
	GetClientRect(&ClientRect);
	
	// Get the image size
	sizeImage.cx = pImage->GetWidth();
	sizeImage.cy = pImage->GetHeight();

	pImage->StretchBlt(pDC->m_hDC, 0, 0, sizeImage.cx, sizeImage.cy, SRCCOPY);

	SetScrollSizes(MM_TEXT, sizeImage);

	UpdateStatusBar();
}

/* MFC created
 * This function is called by framework after the view is first attached to the document.
 */
void CLiDARApplicationView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CLiDARApplicationView printing

BOOL CLiDARApplicationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLiDARApplicationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLiDARApplicationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/*
 * To display the image's properties in the status bar
 */
void CLiDARApplicationView::OnActiveView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	if (bActivate)
	{
		UpdateStatusBar();
	}

	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

/*
 * To update image's information on the status bar
 */
void CLiDARApplicationView::UpdateStatusBar()
{
	CLiDARApplicationDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CLiDARImage * pImage = pDoc->GetImage();
	if (pImage == NULL)
		return;

	CString strImageDetail;
	CStatusBar& statusBar = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetStatusBar();
	strImageDetail.Format(_T("Y%d x X%d"), pImage->GetHeight(), pImage->GetWidth());
	statusBar.SetPaneText(1, strImageDetail);
	strImageDetail.Format(_T("Zoom %d%%"), GetZoomPercentage());
	statusBar.SetPaneText(2, strImageDetail);
}

/*
 * To calculate zoom percentage
 */
int CLiDARApplicationView::GetZoomPercentage()
{
	int iZoomPercentage;

	// Normal size
	if (m_iZoomFactor == 0)
	{
		iZoomPercentage = 100;
	}
	// Zoom-in
	else if (m_iZoomFactor > 0)
	{
		iZoomPercentage = 100 * m_iZoomFactor * ZOOM_FACTOR;	
	}
	// Zoom-out
	else
	{
		iZoomPercentage = 100 * (1.0 / (ZOOM_FACTOR * abs(m_iZoomFactor)));	
	}

	return iZoomPercentage;
}

// CLiDARApplicationView diagnostics

#ifdef _DEBUG
void CLiDARApplicationView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CLiDARApplicationView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CLiDARApplicationDoc* CLiDARApplicationView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLiDARApplicationDoc)));
	return (CLiDARApplicationDoc*)m_pDocument;
}
#endif //_DEBUG

/*
 * To prevent the screen from redrawing.
 */
BOOL CLiDARApplicationView::OnEraseBkgnd(CDC* pDC)
{
	// 1. Get the pointer to the document object
	CLiDARApplicationDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// 2. Get the image object from the document object.
	//	  If there is no image object, call the framework's 
	//	  CScrollView::OnEraseBkgnd() function to erasing
	//	  the object backround.
	CLiDARImage * pLiDARImage = pDoc->GetImage();
	if(pLiDARImage == NULL) 
		return CScrollView::OnEraseBkgnd(pDC);

	// 3. In case there exist an image object.
	//	  3.1 Create a brush in diagonal direction in red
    CBrush brush(HS_DIAGCROSS,RGB(255, 0, 128));
	//    3.2 Get the image region
	CRect rectImage, rectClient;
	rectImage.left		= 0;
	rectImage.top		= 0;
	rectImage.right		=(int)(pLiDARImage->GetWidth());
	rectImage.bottom	=(int)(pLiDARImage->GetHeight());
	//    3.3 Get the client region
    GetClientRect(rectClient);

	// 4. Set the backround color
	pDC->SetBkColor(RGB(236,255,255));
    ::FillRect(pDC->m_hDC, CRect(0, rectImage.bottom, rectClient.right, rectClient.bottom),
			   (HBRUSH)brush);
	::FillRect(pDC->m_hDC, CRect(rectImage.right, 0, rectClient.right, rectImage.bottom),
			   (HBRUSH)brush);
   
	return TRUE;
}

// CLiDARApplicationView message handlers

void CLiDARApplicationView::OnDisplayMaxHeightImage()
{
	CLiDARApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CLiDARImage * pImage = pDoc->GetMaxHeightImage();

	if (pImage == NULL)
		return;

	pDoc->UpdateAllViews(NULL);
}

void CLiDARApplicationView::OnDisplayMinHeightImage()
{
	CLiDARApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CLiDARImage * pImage = pDoc->GetMinHeightImage();

	if (pImage == NULL)
		return;

	pDoc->UpdateAllViews(NULL);
}

void CLiDARApplicationView::OnDisplayAvgHeightImage()
{
	CLiDARApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CLiDARImage * pImage = pDoc->GetAvgHeightImage();

	if (pImage == NULL)
		return;

	pDoc->UpdateAllViews(NULL);
}

void CLiDARApplicationView::OnDisplayDensityImage()
{
	CLiDARApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CLiDARImage * pImage = pDoc->GetDensityImage();

	if (pImage == NULL)
		return;

	pDoc->UpdateAllViews(NULL);
}

/* void CLiDARApplicationView::OnViewZoomin()
 * 
 * Description: To zoom in the image by increasing the resoltuion. The drawing will be made
 *				in the CLiDARApplicationView::OnDraw() function.
 *				*** The image will be bigger.
 * @param		None
 */
void CLiDARApplicationView::OnViewZoomin()
{
	m_iZoomFactor = m_iZoomFactor + 1;
	OnViewZoom();
}

/* void CLiDARApplicationView::OnViewZoomout()
 * 
 * Description: To zoom in the image by reducing the resoltuion. The drawing will be made
 *				in the CLiDARApplicationView::OnDraw() function.
 *				*** The image will be smaller.
 * @param		None
 */
void CLiDARApplicationView::OnViewZoomout()
{
	m_iZoomFactor = m_iZoomFactor - 1;
	OnViewZoom();
}

/*
 * To construct zoomed image according to the zoom factor
 */
void CLiDARApplicationView::OnViewZoom()
{
	CLiDARApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// Produce the zoom image
	pDoc->Zoom(m_iZoomFactor);

	pDoc->UpdateAllViews(NULL);

	// Update the status bar
	UpdateStatusBar();
}

/*
 * To disable or enable the menu for some zoom factors
 */
void CLiDARApplicationView::OnUpdateViewZoomin(CCmdUI *pCmdUI)
{
	CLiDARApplicationDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CLiDARImage * pImage = pDoc->GetImage();
	if (pImage == NULL)
		return;

	int nPixels = pImage->GetHeight() * pImage->GetWidth();

	// The zoom-in toolbar will be disable if four times of the current image size 
	// is greater than 10,000 x 10,000 pixels.

	if (nPixels * ZOOM_FACTOR > 10000 * 10000)
		pCmdUI->Enable(FALSE);
}

/*
 * To disable or enable the menu for some zoom factors
 */
void CLiDARApplicationView::OnUpdateViewZoomout(CCmdUI *pCmdUI)
{
	CLiDARApplicationDoc * pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CLiDARImage * pImage = pDoc->GetImage();
	if (pImage == NULL)
		return;

	int nPixels = pImage->GetHeight() * pImage->GetWidth();

	// The zoom-out toolbar will be disable if four divided of the current image size 
	// is less than 1 x 1 pixels.

	if (nPixels / ZOOM_FACTOR < 1 * 1)
		pCmdUI->Enable(FALSE);
}

/* 
 *	To get the coordinate of the pixel
 */
void CLiDARApplicationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Set flag that the mouse is down.
	m_bMouseDown = true;

	// Store the coordinate of the center pixel.
	m_xyLCoord.iX	= point.x;	
	m_xyLCoord.iY	= point.y;	

	SetCursor(LoadCursor(NULL, IDC_SIZEALL));
}

/* 
 *	Reset the panning operation
 */
void CLiDARApplicationView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// Release the mouse
	m_bMouseDown = false;

	SetCursor(LoadCursor(NULL, IDC_ARROW));	
}

/* 
 *	Handle mouse move
 */
void CLiDARApplicationView::OnMouseMove(UINT nFlags, CPoint point)
{
	// Start the panning operation
	if (m_bMouseDown == true)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));

		// Update the origin of the image
        m_xyOrigin.iX	= m_xyOrigin.iX + point.x - m_xyLCoord.iX;
		m_xyOrigin.iY	= m_xyOrigin.iY + point.y - m_xyLCoord.iY;

		// Handle the case that the coordinate beyond the valid value.
		if (m_xyOrigin.iX < 0)	m_xyOrigin.iX = 0;
		if (m_xyOrigin.iY < 0)	m_xyOrigin.iY = 0;

		// Get the pointer to the CLiDARImage object
		CLiDARApplicationDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		CLiDARImage * pImage = pDoc->GetImage();
		if (pImage == NULL)
			return;

		if (m_xyOrigin.iX > pImage->GetWidth())		m_xyOrigin.iX = pImage->GetWidth();
		if (m_xyOrigin.iY > pImage->GetHeight())	m_xyOrigin.iY = pImage->GetHeight();
	
		// Set the scroll bar position
		POINT ptOrigin;
		ptOrigin.x = m_xyOrigin.iX;
		ptOrigin.y = m_xyOrigin.iY;

		// Adjust the scroll bar to the new position
		ScrollToPosition(ptOrigin);
		SetScrollPos(SB_HORZ, ptOrigin.x, TRUE);
		SetScrollPos(SB_VERT, ptOrigin.y, TRUE);
	}
}

