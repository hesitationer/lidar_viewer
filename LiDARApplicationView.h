// LiDARApplicationView.h : interface of the CLiDARApplicationView class
//


#pragma once


class CLiDARApplicationView : public CScrollView
{
protected: // create from serialization only
	CLiDARApplicationView();
	DECLARE_DYNCREATE(CLiDARApplicationView)

// Attributes
public:
	CLiDARApplicationDoc* GetDocument() const;
	// To update image's information on the status bar
	void UpdateStatusBar();
	// To calculate zoom percentage
	int GetZoomPercentage();

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActiveView(BOOL bActivate, CView* pActivateView, CView*);

// Implementation
public:
	virtual ~CLiDARApplicationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// To construct zoomed image according to the zoom factor
 	void OnViewZoom();

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDisplayMaxHeightImage();
	afx_msg void OnDisplayMinHeightImage();
	afx_msg void OnDisplayAvgHeightImage();
	afx_msg void OnDisplayDensityImage();
 	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnUpdateViewZoomin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewZoomout(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point );

private:
	int			m_iZoomFactor;		// Zoom Factor
	bool		m_bMouseDown;		// Flag whether mouse is hold down.
	XY_POSITION m_xyLCoord;			// The coordinate that the left mouse is down.
	XY_POSITION m_xyOrigin;			// The coordinate of the origin of the image.
};

#ifndef _DEBUG  // debug version in LiDARApplicationView.cpp
inline CLiDARApplicationDoc* CLiDARApplicationView::GetDocument() const
   { return reinterpret_cast<CLiDARApplicationDoc*>(m_pDocument); }
#endif

