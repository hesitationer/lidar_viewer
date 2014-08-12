/***********************************************************************
 * File Name:	GridCell.cpp
 * Description: This file contains the implementation for the CGridCell
 *				class. This class represent a cell grid in which it
 *				may contain several LiDAR points.
 ***********************************************************************/


#include "StdAfx.h"
#include ".\gridcell.h"

/*
 * Constructor
 */
CGridCell::CGridCell(void)
{
	// Initialize the member variables.
	m_dMinHeight	= 1e20;
	m_dMaxHeight	= -1e20;
	m_dAvgHeight	= 0.0;
	m_nPoints		= 0;
}

/*
 * Destructor
 */
CGridCell::~CGridCell(void)
{
	// RECHECK - If calling this, the CLiDARPoints will be removed.


	/*
	// While there exist some LiDAR points in the collection.
	while (m_deqLiDARPoints.size() != 0)
	{
		DEQUE_LIDAR_POINTS::iterator iter = m_deqLiDARPoints.begin();
		
		// Remove from the collection and delete object.
		static_cast<void>(m_deqLiDARPoints.erase(iter));
	}
	*/
}

/*
 * The function is to add the passed-in LiDAR point to the collection and
 * recalculate the min/max/avg height.
 * 
 * Return: true - successfully added.
 */
bool CGridCell::AddToGrid(CLiDARPoint * objLiDAR)
{
	if (objLiDAR == NULL)
		return false;

	m_deqLiDARPoints.push_back(objLiDAR);

	// Recalculate the minimum height.
	if (objLiDAR->m_dZ < m_dMinHeight)
		m_dMinHeight = objLiDAR->m_dZ;
	
	// Recalculate the maximum height.
	if (objLiDAR->m_dZ > m_dMaxHeight)
		m_dMaxHeight = objLiDAR->m_dZ;

	// Recalculate the mean value.
	m_dAvgHeight = GetNewAverageHeight(objLiDAR->m_dZ);

	// Increment the number of points.
	m_nPoints = m_nPoints + 1;

	return true;
}

/*
 * The function is to recalculate the average height in the grid cell
 * after a new point is added.
 */
double CGridCell::GetNewAverageHeight(double dHeight)
{
	m_dAvgHeight = (m_dAvgHeight * m_nPoints + dHeight)/(m_nPoints + 1);

	return m_dAvgHeight;
}


