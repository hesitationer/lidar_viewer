/***********************************************************************
 * File Name:	GridCell.h
 * Description: This file contains the declaration for the CGridCell
 *				class. This class represent a cell grid in which it
 *				may contain several LiDAR points.
 ***********************************************************************/

#pragma once

#include "Definition.h"

class CGridCell
{
public:
	CGridCell(void);
	~CGridCell(void);

	// The function is to add the passed-in LiDAR point to the collection and
	// recalculate the min/max/avg height.
	bool AddToGrid(CLiDARPoint * objLiDAR);

	// Recalculate the avg height in the grid cell after a new point added.
	double GetNewAverageHeight(double dHeight);

public:
	DEQUE_LIDAR_POINTS	m_deqLiDARPoints;	// The collection of LiDAR points in the cell.
	double	m_dMinHeight;	// The minimum value of height.
	double	m_dMaxHeight;	// The maximum value of height.
	double	m_dAvgHeight;	// The average height.
	int		m_nPoints;		// The total number of height in the grid cell.
};
