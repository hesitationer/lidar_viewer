#include "StdAfx.h"
#include ".\gridspace.h"
#include "Math.h"

CGridSpace::CGridSpace(void)
{
	m_p2DGridSpace = NULL;
}

CGridSpace::~CGridSpace(void)
{
	int i;
	CGridCell * pGridCell;

	// Delete all grid cells
	for (i = 0; i < m_nYCells * m_nXCells; i++)
	{
		pGridCell = m_p2DGridSpace[i];
		delete pGridCell;
	}

	if (m_p2DGridSpace != NULL)
		free(m_p2DGridSpace);

	for (i = 0; i < NUM_IMG; i++)
	{
		delete m_pLiDARImages[i];
	}
}

/* 
 * Calculate the number of grid cell required for x and y axis by using MBR and grid size.
 */
bool CGridSpace::Initialization(XYZ_MBR mbrImage, DEQUE_LIDAR_POINTS * deqLiDARPoints, double dGridSize)
{
	double dRange;

	// Store the Grid size.
	m_dGridSize = dGridSize;

	// Store the pointer to the collection of LiDAR points
	m_pdeqLiDARPoints = deqLiDARPoints;

	// Store the MBR
	m_xyzMBR = mbrImage;

	// Calculate the number of grid cells in *y-axis*
	dRange = abs(mbrImage.dMaxY - mbrImage.dMinY);
	m_nYCells = ceil(dRange/dGridSize);

	// Calculate the number of grid cells in *x-axis*
	dRange = abs(mbrImage.dMaxX - mbrImage.dMinX);
	m_nXCells = dRange/dGridSize;

	// Initialize the GridSpace object.
	if (!InitializeGridSpace())
		return false;

	return true;
}

/*
 * This function is to initialize the Grid Space object.
 *
 * Return	true - successfully allocate memory
 *			false - fail to allocate memory
 */
bool CGridSpace::InitializeGridSpace()
{
	int i;
	CGridCell * pGridCell;	

	// Allocate the grid space area
	m_p2DGridSpace = reinterpret_cast<CGridCell **> (malloc(m_nYCells * m_nXCells * sizeof(CGridCell *)));
	
	if (m_p2DGridSpace == NULL)
	{
		MessageBox(NULL, "Failed to allocate memory for the grid space", "Error", IDOK);
		return false;				
	}

	// Set the member variable to zero
	memset(m_p2DGridSpace, 0, m_nYCells * m_nXCells * sizeof(CGridCell *));

	// Construct the grid cells and associate them with the grid space.
	for (i = 0; i < m_nYCells * m_nXCells; i++)
	{
		// Construct the instance of the CGridCell class.
		pGridCell			= new CGridCell;

		if (pGridCell == NULL)
		{
			MessageBox(NULL, "Failed to allocate memory for the grid cell", "Error", IDOK);
			return false;						
		}
		
		// Associate the CGridCell object with the grid space.
        m_p2DGridSpace[i]	= pGridCell;
	}

	return true;
}

/*
 * Manipulate the grid space by
 * 1.	Initialize the grid space
 * 2.	Initialize grid cells and associate them with the grid space
 *
 */
bool CGridSpace::ManageGridSpace(XYZ_MBR mbrImage, DEQUE_LIDAR_POINTS * deqLiDARPoints)
{
	return ManageGridSpace(mbrImage, deqLiDARPoints, 1.0);
}

bool CGridSpace::ManageGridSpace(XYZ_MBR mbrImage, DEQUE_LIDAR_POINTS * deqLiDARPoints, double dGridSize)
{
	if (!Initialization(mbrImage, deqLiDARPoints, dGridSize))
		return false;

	if (!AssignLiDARPointsToGridCell(mbrImage))
		return false;

	if (!ConstructCLiDARImages())
		return false;

	if (!LoadPropertiesToImages())
		return false;

	return true;
}

/* 
 * Assign each LiDAR point cloud to each grid cell according to their coordinates
 */
bool CGridSpace::AssignLiDARPointsToGridCell(XYZ_MBR mbrImage)
{
	int				iPoint;			// Index of the LiDAR point.
	double			dX, dY;			// Coordinates of the LiDAR point.
	XY_POSITION		posGridCell;	// Position of GridCell.
	int				cellID;			// The cell id.

	// Calculate the offset at the top left most of the grid space.
	m_iXOffset = (int) mbrImage.dMinX;
	m_iYOffset = (int) mbrImage.dMinY;

	// Assign each LiDAR point cloud to each grid cell.
	for (iPoint = 0; iPoint < m_pdeqLiDARPoints->size(); iPoint++)
	{
		// Get the coordinate of the LiDAR point
		dX = (*m_pdeqLiDARPoints)[iPoint]->m_dX;
		dY = (*m_pdeqLiDARPoints)[iPoint]->m_dY;

		// Get the grid cell coordinate
		posGridCell = GetGridCellPosition(dX, dY);

		// Assign each LiDAR point to the grid space cell
		cellID = posGridCell.iY * m_nXCells + posGridCell.iX;

		m_p2DGridSpace[cellID]->AddToGrid((*m_pdeqLiDARPoints)[iPoint]);
	}

	return true;
}

/* 
 * Map the real coordinate to the grid position.
 */
XY_POSITION CGridSpace::GetGridCellPosition(double dXCoordinate, double dYCoordinate)
{
	XY_POSITION		posGridCell;
	
	// Find the grid position from the real coordinate.
	posGridCell.iX = (dXCoordinate - m_iXOffset)/m_dGridSize;
	posGridCell.iY = (dYCoordinate - m_iYOffset)/m_dGridSize;

	// Adjust the outlier
	if (posGridCell.iX < 0)				posGridCell.iX = 0;
	if (posGridCell.iX >= m_nXCells)	posGridCell.iX = m_nXCells - 1;
	if (posGridCell.iY < 0)				posGridCell.iY = 0;
	if (posGridCell.iY >= m_nYCells)	posGridCell.iY = m_nYCells - 1;

	return posGridCell;
}


/*
 * Construct 4 LiDAR Images
 *	1. m_pLiDARImages[MAX_HEIGHT_IMG]	Max height image
 *	2. m_pLiDARImages[MIN_HEIGHT_IMG]	Min height image
 *	3. m_pLiDARImages[AVG_HEIGHT_IMG]	Average height image
 *	4. m_pLiDARImages[DENSITY_IMG]		Density image
 */
bool CGridSpace::ConstructCLiDARImages()
{
	// Define 24 bits per pixels.
	const int BPP = 24;

	int i;
	BOOL bResult = true;

	// Create instances of the CLiDARImage.
	for (i = 0; i < NUM_IMG; i++)
	{
		CLiDARImage * objImage = new CLiDARImage;

		if (objImage == NULL)
		{
			MessageBox(NULL, "Failed to create a CLiDARImage instance", "Error", IDOK);
			return false;			
		}

		// Initilize the CImage object by assigning its width, height and #bits.
		bResult = objImage->Create(m_nXCells, 0-m_nYCells, BPP);

		if (!bResult)
		{
			MessageBox(NULL, "Failed to initialize the CCDImage structure", "Error", IDOK);
			return false;					
		}

		m_pLiDARImages[i] = objImage;
	}

	return true;
}

/*
 * Iterate through each grid cell and assign their 4 values to each image.
 *	There are 4 properties for 4 images:
 *	1. Max height	m_pLiDARImages[MAX_HEIGHT_IMG]
 *	2. Min height	m_pLiDARImages[MIN_HEIGHT_IMG]
 *	3. Avg height	m_pLiDARImages[AVG_HEIGHT_IMG]
 *	4. Density		m_pLiDARImages[DENSITY_IMG]
 */
bool CGridSpace::LoadPropertiesToImages()
{
	const int COLOUR_RANGE = 256;		// The range of colour is [0, 255].
	const int BPP = 24;					// Define 24 bits per pixels.

	int		i, x, y;

	BOOL	bResult = true;
	int		iValue;
	int		cellID;

	double dScalingFactors[NUM_IMG];	// The array stores 4 scaling factors.
	PAIR_DOUBLE pairRanges[NUM_IMG];	// The array stores 4 pairs of min & max values.

	// Get the scaling factors
	CalculateScalingFactor(dScalingFactors, pairRanges);

	// Iterate through each grid cell
	for (y = 0; y < m_nYCells; y++)
	{
		for (x = 0; x < m_nXCells; x++)
		{
			// Calculate the Grid Cell ID from the x,y coordinate
			cellID = y * m_nXCells + x;

			// Maximum height
			iValue = (m_p2DGridSpace[cellID]->m_dMaxHeight - m_xyzMBR.dMinZ)/dScalingFactors[MAX_HEIGHT_IMG];
			iValue = (iValue > 255) ? 255:iValue;
			iValue = (iValue < 0) ? 0: iValue;
			m_pLiDARImages[MAX_HEIGHT_IMG]->AssignSpectralToAPixel(x, y, /*B*/ colorRGB[iValue].b, /*G*/ colorRGB[iValue].g, /*R*/ colorRGB[iValue].r);

			// Minimum height
			iValue = (m_p2DGridSpace[cellID]->m_dMinHeight - m_xyzMBR.dMinZ)/dScalingFactors[MAX_HEIGHT_IMG];
			iValue = (iValue > 255) ? 255:iValue;
			iValue = (iValue < 0) ? 0: iValue;
			m_pLiDARImages[MIN_HEIGHT_IMG]->AssignSpectralToAPixel(x, y, /*B*/ colorRGB[iValue].b, /*G*/ colorRGB[iValue].g, /*R*/ colorRGB[iValue].r);

			// Average height
			iValue = (m_p2DGridSpace[cellID]->m_dAvgHeight - m_xyzMBR.dMinZ)/dScalingFactors[AVG_HEIGHT_IMG];
			iValue = (iValue > 255) ? 255:iValue;
			iValue = (iValue < 0) ? 0: iValue;
			m_pLiDARImages[AVG_HEIGHT_IMG]->AssignSpectralToAPixel(x, y, /*B*/ colorRGB[iValue].b, /*G*/ colorRGB[iValue].g, /*R*/ colorRGB[iValue].r);

			// Density
			iValue = (m_p2DGridSpace[cellID]->m_dAvgHeight - m_xyzMBR.dMinZ)/dScalingFactors[DENSITY_IMG];
			iValue = (iValue > 255) ? 255:iValue;
			iValue = (iValue < 0) ? 0: iValue;
			m_pLiDARImages[DENSITY_IMG]->AssignSpectralToAPixel(x, y, /*B*/ colorRGB[iValue].b, /*G*/ colorRGB[iValue].g, /*R*/ colorRGB[iValue].r);
		}
	}

	return true;
}

/*
 * Iterate through each grid cell to calculate scaling factors for 4 properties.
 *	1. Max height	m_pLiDARImages[MAX_HEIGHT_IMG]
 *	2. Min height	m_pLiDARImages[MIN_HEIGHT_IMG]
 *	3. Avg height	m_pLiDARImages[AVG_HEIGHT_IMG]
 *	4. Density		m_pLiDARImages[DENSITY_IMG]
 */
void CGridSpace::CalculateScalingFactor(double * dScalingFactors, PAIR_DOUBLE * pairRanges)
{
	const int COLOUR_RANGE = 256;		// The range of colour is [0, 255].

	int y, x;
	int cellID;

	pairRanges[MAX_HEIGHT_IMG].dVal1 = -1e20; /*Max*/	pairRanges[MAX_HEIGHT_IMG].dVal2 = 1e20; /*Min*/
	pairRanges[MIN_HEIGHT_IMG].dVal1 = -1e20; /*Max*/	pairRanges[MIN_HEIGHT_IMG].dVal2 = 1e20; /*Min*/
	pairRanges[AVG_HEIGHT_IMG].dVal1 = -1e20; /*Max*/	pairRanges[AVG_HEIGHT_IMG].dVal2 = 1e20; /*Min*/
	pairRanges[DENSITY_IMG].dVal1 = -1e20; /*Max*/		pairRanges[DENSITY_IMG].dVal2 = 1e20; /*Min*/

	// Iterate through each grid cell
	for (y = 0; y < m_nYCells; y++)
	{
		for (x = 0; x < m_nXCells; x++)
		{
			// Calculate the Grid Cell ID from the x,y coordinate
			cellID = y * m_nXCells + x;

			// Continue if there is no points in the grid cell.
			if (m_p2DGridSpace[cellID]->m_nPoints == 0)
			{
				// Reset the value to zero
				m_p2DGridSpace[cellID]->m_dMaxHeight = 0.0;		
				m_p2DGridSpace[cellID]->m_dMinHeight = 0.0;		
				m_p2DGridSpace[cellID]->m_dAvgHeight = 0.0;		
				
				continue;
			}

			// Find Min/Max for *Max height*
			if (m_p2DGridSpace[cellID]->m_dMaxHeight > pairRanges[MAX_HEIGHT_IMG].dVal1)		
				pairRanges[MAX_HEIGHT_IMG].dVal1 = m_p2DGridSpace[cellID]->m_dMaxHeight;
			if (m_p2DGridSpace[cellID]->m_dMaxHeight < pairRanges[MAX_HEIGHT_IMG].dVal2)		
				pairRanges[MAX_HEIGHT_IMG].dVal2 = m_p2DGridSpace[cellID]->m_dMaxHeight;

			// Find Min/Max for *Min height*
			if (m_p2DGridSpace[cellID]->m_dMinHeight > pairRanges[MIN_HEIGHT_IMG].dVal1)		
				pairRanges[MIN_HEIGHT_IMG].dVal1 = m_p2DGridSpace[cellID]->m_dMinHeight;
			if (m_p2DGridSpace[cellID]->m_dMinHeight < pairRanges[MIN_HEIGHT_IMG].dVal2)		
				pairRanges[MIN_HEIGHT_IMG].dVal2 = m_p2DGridSpace[cellID]->m_dMinHeight;

			// Find Min/Max for *Average height*
			if (m_p2DGridSpace[cellID]->m_dAvgHeight > pairRanges[AVG_HEIGHT_IMG].dVal1)		
				pairRanges[AVG_HEIGHT_IMG].dVal1 = m_p2DGridSpace[cellID]->m_dAvgHeight;
			if (m_p2DGridSpace[cellID]->m_dAvgHeight < pairRanges[AVG_HEIGHT_IMG].dVal2)		
				pairRanges[AVG_HEIGHT_IMG].dVal2 = m_p2DGridSpace[cellID]->m_dAvgHeight;

			// Find Min/Max for *Density*
			if (m_p2DGridSpace[cellID]->m_nPoints > pairRanges[DENSITY_IMG].dVal1)		
				pairRanges[DENSITY_IMG].dVal1 = m_p2DGridSpace[cellID]->m_nPoints;
			if (m_p2DGridSpace[cellID]->m_nPoints < pairRanges[DENSITY_IMG].dVal2)		
				pairRanges[DENSITY_IMG].dVal2 = m_p2DGridSpace[cellID]->m_nPoints;
		}
	}

	dScalingFactors[MAX_HEIGHT_IMG] = (pairRanges[MAX_HEIGHT_IMG].dVal1 - pairRanges[MAX_HEIGHT_IMG].dVal2)/COLOUR_RANGE;
	dScalingFactors[MIN_HEIGHT_IMG] = (pairRanges[MIN_HEIGHT_IMG].dVal1 - pairRanges[MIN_HEIGHT_IMG].dVal2)/COLOUR_RANGE;
	dScalingFactors[AVG_HEIGHT_IMG] = (pairRanges[AVG_HEIGHT_IMG].dVal1 - pairRanges[AVG_HEIGHT_IMG].dVal2)/COLOUR_RANGE;
	dScalingFactors[DENSITY_IMG]	= (pairRanges[DENSITY_IMG].dVal1 - pairRanges[DENSITY_IMG].dVal2)/COLOUR_RANGE;
}
