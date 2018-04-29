#pragma once

class CGridSpace
{
public:
	CGridSpace(void);

	~CGridSpace(void);

	// Calculate the number of grid cell required for x and y axis.
	bool Initialization(XYZ_MBR mbrImage, DEQUE_LIDAR_POINTS * deqLiDARPoints, double dGridSize);

	// Associate LiDAR points with the grid space.
	bool ManageGridSpace(XYZ_MBR mbrImage, DEQUE_LIDAR_POINTS * deqLiDARPoints);
	bool ManageGridSpace(XYZ_MBR mbrImage, DEQUE_LIDAR_POINTS * deqLiDARPoints, double dGridSize);

	// Assign each LiDAR point cloud to each grid cell according to their coordinates.
	bool AssignLiDARPointsToGridCell(XYZ_MBR mbrImage);

	// Get the pointer to the CLiDARImage object.
	CLiDARImage * GetMaxHeightImage() {return m_pLiDARImages[MAX_HEIGHT_IMG];}
	CLiDARImage * GetMinHeightImage() {return m_pLiDARImages[MIN_HEIGHT_IMG];}
	CLiDARImage * GetAvgHeightImage() {return m_pLiDARImages[AVG_HEIGHT_IMG];}
	CLiDARImage * GetDensityImage() {return m_pLiDARImages[DENSITY_IMG];}


private:

	// Initialize the CGridSpace object
	bool InitializeGridSpace();

	// Map the real coordinate to the grid position
	XY_POSITION GetGridCellPosition(double dXCoordinate, double dYCoordinate);

	// Construct 4 LiDAR Images
	bool ConstructCLiDARImages();

	// Assign the height properties to image
	bool LoadPropertiesToImages();

	// Calculate scaling factors
	void CalculateScalingFactor(double * dScalingFactors, PAIR_DOUBLE * pairRanges);

protected:
	double	m_dGridSize;		// The size of grid in *metre*, m_iGridSize x m_iGridSize
	int		m_nYCells;			// The number of grid cells in the y-axis.
	int		m_nXCells;			// The number of gird cells in the x-axis.
	XYZ_MBR	m_xyzMBR;			// The minimum bounding box.

	int		m_iXOffset;			// The X offset coordinate at the left most of the Grid space.
	int		m_iYOffset;			// The Y offset coordinate at the left most of the Grid space.

	CGridCell ** m_p2DGridSpace;	// The 2D space for grid cells

	DEQUE_LIDAR_POINTS * m_pdeqLiDARPoints;	// The pointer to the lidar point collection.

	// Array of 4 LiDAR images
	CLiDARImage * m_pLiDARImages[NUM_IMG];
};
