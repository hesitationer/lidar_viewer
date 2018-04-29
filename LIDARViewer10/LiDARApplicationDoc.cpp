// LiDARApplicationDoc.cpp : implementation of the CLiDARApplicationDoc class
//

#include "stdafx.h"
#include "LiDARApplication.h"

#include "LiDARApplicationDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLiDARApplicationDoc

IMPLEMENT_DYNCREATE(CLiDARApplicationDoc, CDocument)

BEGIN_MESSAGE_MAP(CLiDARApplicationDoc, CDocument)
END_MESSAGE_MAP()


// CLiDARApplicationDoc construction/destruction

CLiDARApplicationDoc::CLiDARApplicationDoc()
{
	// Initialize member variables
	m_pGridSpace	= NULL;
	m_pLiDARImage	= NULL;
	m_dGridSize		= 1.0;
}

CLiDARApplicationDoc::~CLiDARApplicationDoc()
{
	int i;

	// Delete the LiDAR points
	for (i = 0; i < m_deqPoints.size(); i++)
	{
		delete m_deqPoints[i];
	}

	// Delete the CGridSpace object.
	if (m_pGridSpace != NULL)
		delete m_pGridSpace;
}

BOOL CLiDARApplicationDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/* void BOOL CLiDARApplicationDoc::OnOpenDocument(LPCTSTR lpszPathName)
 * 
 * Description: To open the LiDAR ASCII file. This function works after the 
 *				CLiDARApplicationApp::OnFileOpen function that will forward
 *				the lpszPathName to this function.
 * @param lpszPathName	The path to the image file
 */
BOOL CLiDARApplicationDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	theApp.m_strFileName = CString(lpszPathName);

	/******************************************************
	 * Read the ASCII LiDAR File to get the MBR for
	 * intializing the CLiDARImage (CImage) class.
	 ******************************************************/
	if (!ReadASCIILiDARFile(lpszPathName))
		return FALSE;

	// Instantiate the CGridSpace object and associate the LiDAR points.
	m_pGridSpace = new CGridSpace;
	if (m_pGridSpace == NULL)
		return FALSE;			

	if (!m_pGridSpace->ManageGridSpace(m_xyzMBR, &m_deqPoints))
	{
		MessageBox(NULL, "The application experiences internal error and will be immediately terminated.",
					"Fatal Error", MB_OK);
		
		exit(1);
	}

	return TRUE;
}


// CLiDARApplicationDoc serialization

void CLiDARApplicationDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CLiDARApplicationDoc diagnostics

#ifdef _DEBUG
void CLiDARApplicationDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLiDARApplicationDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLiDARApplicationDoc commands

/* bool CLiDARApplicationDoc::ReadASCIILiDARFile(LPCTSTR lpszPathName)
 * 
 * Description: To read the LiDAR ASCII file.
 *
 * @param lpszPathName	The path to the image file
 *
 * Return	true - successfully read.
 */
bool CLiDARApplicationDoc::ReadASCIILiDARFile(LPCTSTR lpszPathName)
{
	const int LINE_SIZE = 81;		// The size of the buffer.
	char lineBuf[LINE_SIZE] = {0};	// Buffer for one line read from file.
	int i;
	
	// Open the LiDAR ASCII file
	FILE * fLiDAR = fopen(lpszPathName, "r+");

	if (fLiDAR == NULL)
	{
		MessageBox(NULL, "Failed to read the LiDAR file or the the file does not exist.\nThe application is going to terminate.", 
					"Error", MB_OK);
		return false;
	}

	// Check the format of the file.
	//	1.	Read the 1st line from the file.
	fgets(lineBuf, LINE_SIZE, fLiDAR);

	//	2.	Count the number of tokens in the string to consider which 
	//		format the file is.
	//		For example:
	//		1. d d d i
	//		2. d d d i	d d d i
	int nTokens = countTokens(lineBuf);

	//	3.	Construct the reading format.
	const char LIDAR_FORMAT_STRING[] = "%lf %lf %lf %d";
	char lidarFormat[LINE_SIZE] = {0};
	int nColumn = nTokens/4;

	//		Support at most 2 columns
	if (nColumn > 2)
	{
		MessageBox(NULL, "Incompatible LiDAR format file.\nThe application is going to terminate.", 
					"Error", MB_OK);
		return false;
	}

	strcat(lidarFormat, LIDAR_FORMAT_STRING);

	for (i = 1; i < nColumn; i++)
	{
		strcat(lidarFormat, " "); 
		strcat(lidarFormat, LIDAR_FORMAT_STRING);
	}

	LIDAR_POINT lidar[2];			// Temporary store LiDAR point

	// To record the MBR
	double X_MAX	= -1e20;
	double X_MIN	= 1e20;
	double Y_MAX	= -1e20;
	double Y_MIN	= 1e20;
	double Z_MAX	= -1e20;
	double Z_MIN	= 1e20;

	// Rewind to the starting point of the file.
	rewind(fLiDAR);

	// Read the ASCII LiDAR file line by line.
	while (fgets(lineBuf, LINE_SIZE, fLiDAR) != NULL)
	{
		// The LiDAR format is "x y z i"
		if (nColumn == 1)
		{
			if (sscanf(lineBuf, lidarFormat, &lidar[0].dx, &lidar[0].dy, &lidar[0].dz, &lidar[0].icolour) != 4)
				continue;
		}
		// The LiDAR format is "x y z i x y z i"
		else if (nColumn == 2)
		{
			if (sscanf(lineBuf, lidarFormat, &lidar[0].dx, &lidar[0].dy, &lidar[0].dz, &lidar[0].icolour,
											 &lidar[1].dx, &lidar[1].dy, &lidar[1].dz, &lidar[1].icolour) != 8)
				continue;
		}

		// Find the minimum bounding box.
		for (i = 0; i < nColumn; i++)
		{
			// Initialize the LiDAR Object
			CLiDARPoint * objLiDAR = new CLiDARPoint(lidar[i].dx, lidar[i].dy, lidar[i].dz, lidar[i].icolour);
			
			if (objLiDAR == NULL)
			{
				MessageBox(NULL, "Insufficient memory.\nThe application is going to terminate.", 
							"Error", MB_OK);
				return false;			
			}

			// Store the lidar point into the LiDAR point collection
			m_deqPoints.push_back(objLiDAR);

			// Find the right most coordinate.
			if (lidar[i].dx > X_MAX)		
				X_MAX = lidar[i].dx;

			// Find the left most coordinate.
			if (lidar[i].dx < X_MIN)	
				X_MIN = lidar[i].dx;

			// Find the bottom most coordinate.
			if (lidar[i].dy > Y_MAX)
				Y_MAX = lidar[i].dy;

			// Find the top most coordinate.
			if (lidar[i].dy < Y_MIN)
				Y_MIN = lidar[i].dy;

			// Find the highest z coordinate.
			if (lidar[i].dz > Z_MAX)
				Z_MAX = lidar[i].dz;

			// Find the lowest z coordinate.
			if (lidar[i].dz < Z_MIN)
				Z_MIN = lidar[i].dz;
		}
	
	} // end while loop reading file line by line

	fclose(fLiDAR);	

	m_xyzMBR.dMinX	= floor(X_MIN);
	m_xyzMBR.dMinY	= floor(Y_MIN);
	m_xyzMBR.dMinZ	= floor(Z_MIN);
	m_xyzMBR.dMaxX	= ceil(X_MAX);
	m_xyzMBR.dMaxY	= ceil(Y_MAX);
	m_xyzMBR.dMaxZ	= ceil(Z_MAX);

	return true;
}

/* CLiDARImage * CLiDARApplicationDoc::GetMaxHeightImage(void)
 * 
 * Description: To return the pointer to the CLiDARImage for *Maximum Height*
 * @param		None
 */
CLiDARImage * CLiDARApplicationDoc::GetMaxHeightImage(void)
{
	return (m_pLiDARImage = m_pGridSpace->GetMaxHeightImage());
}

/* CLiDARImage * CLiDARApplicationDoc::GetMinHeightImage(void)
 * 
 * Description: To return the pointer to the CLiDARImage for *Minimum Height*
 * @param		None
 */
CLiDARImage * CLiDARApplicationDoc::GetMinHeightImage(void)
{
	return (m_pLiDARImage = m_pGridSpace->GetMinHeightImage());
}

/* CLiDARImage * CLiDARApplicationDoc::GetAvgHeightImage(void)
 * 
 * Description: To return the pointer to the CLiDARImage for *Average Height*
 * @param		None
 */
CLiDARImage * CLiDARApplicationDoc::GetAvgHeightImage(void)
{
	return (m_pLiDARImage = m_pGridSpace->GetAvgHeightImage());
}

/* CLiDARImage * CLiDARApplicationDoc::GetDensityImage(void)
 * 
 * Description: To return the pointer to the CLiDARImage for *Density*
 * @param		None
 */
CLiDARImage * CLiDARApplicationDoc::GetDensityImage(void)
{
	return (m_pLiDARImage = m_pGridSpace->GetDensityImage());
}

/* CLiDARImage * CLiDARApplicationDoc::GetImage(void)
 * 
 * Description: To return the pointer to the latest CLiDARImage
 * @param		None
 */
CLiDARImage * CLiDARApplicationDoc::GetImage(void)
{
	if (m_pLiDARImage == NULL)
		m_pLiDARImage = m_pGridSpace->GetMaxHeightImage();

	return m_pLiDARImage;
}

/* void CLiDARApplicationDoc::GetGridSize(int iZoomFactor)
 * 
 * Description: To calculate the grid size according to the zoom factor
 * @param		iZoomFactor + --> zoom in (small grid size)
 *				iZoomFactor - --> zoom out (large grid size)
 */
void CLiDARApplicationDoc::GetGridSize(int iZoomFactor)
{
	// Calculate grid size for zoom-in
	if (iZoomFactor == 0)
	{
		m_dGridSize = 1.0;
	}
	// Calculate grid size for zoom-in
	else if (iZoomFactor > 0)
	{
		m_dGridSize = 1.0 / (ZOOM_FACTOR * iZoomFactor);	
	}
	// Calculate grid size for zoom-out
	else
	{
		m_dGridSize = (double)abs(iZoomFactor) * ZOOM_FACTOR;	
	}
}

/* BOOL CLiDARApplicationDoc::Zoom(int iZoomFactor)
 * 
 * Description: To produce the zoomed image
 * @param		iZoomFactor which is the times of 4
 *				
 *				... 1/64, 1/16, 1/4, 1, 4, 16, 64, ...
 *		    zoom out (smaller) <---- X ----> zoom in (bigger)
 */
void CLiDARApplicationDoc::Zoom(int iZoomFactor)
{
	// Calculate the size of grid
	GetGridSize(iZoomFactor);

	// If there exist an instance of the CGridSpace object, destroy before creating a new one.
	if (m_pGridSpace != NULL)
		delete m_pGridSpace;

	// Re-instantiate the instance of the CGridSpace object.
	m_pGridSpace = new CGridSpace;
	if (m_pGridSpace == NULL)
	{
		MessageBox(NULL, "Memory allocation failure.\nThe application will be immediately terminated.", "Error", MB_OK);
		exit(1);
	}

	if (!m_pGridSpace->ManageGridSpace(m_xyzMBR, &m_deqPoints, m_dGridSize))
	{
		MessageBox(NULL, "Operation error.\nThe application will be immediately terminated.", "Error", MB_OK);
		exit(1);
	}

	// Reset the pointer to the LiDAR image.
	m_pLiDARImage = NULL;
}