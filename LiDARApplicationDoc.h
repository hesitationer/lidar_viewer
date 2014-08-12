// LiDARApplicationDoc.h : interface of the CLiDARApplicationDoc class
//


#pragma once

class CLiDARApplicationDoc : public CDocument
{
protected: // create from serialization only
	CLiDARApplicationDoc();
	DECLARE_DYNCREATE(CLiDARApplicationDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CLiDARApplicationDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// To open the LiDAR ASCII file.
	virtual	BOOL OnOpenDocument(LPCTSTR lpszPathName);

	// To read the ASCII LiDAR file.
	bool ReadASCIILiDARFile(LPCTSTR lpszPathName);

	// To get the pointer to the maximum height image.
	CLiDARImage * GetMaxHeightImage(void);

	// To get the pointer to the minimum height image.
	CLiDARImage * GetMinHeightImage(void);

	// To get the pointer to the average height image.
	CLiDARImage * GetAvgHeightImage(void);

	// To get the pointer to the density image.
	CLiDARImage * GetDensityImage(void);

	// To return the active instance of the CLiDARImage class.
	CLiDARImage * GetImage();

	// To produce the zoomed image.
	void Zoom(int iZoomFactor);

	// To calculate the grid size according to the zoom factor
	void GetGridSize(int iZoomFactor);

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:

	// LiDAR Point Clouds Collection
	DEQUE_LIDAR_POINTS m_deqPoints;

	// The MBR of the xyz coordinates reading from the ASCII file.
	XYZ_MBR m_xyzMBR;

	// The pointer to the single instance of the CGridSpace class.
	CGridSpace * m_pGridSpace;

	// The active image among 4 properties image.
	CLiDARImage * m_pLiDARImage;

	// The size of grid, m_dGridSize x m_dGridSize.
	double m_dGridSize;
};


