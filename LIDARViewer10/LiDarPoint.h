/***********************************************************************
 * File Name:	LiDARPoint.h
 * Description: This file contains the declaration for the CLiDARPoint
 *				class which will represent the LiDAR point.
 ***********************************************************************/

#pragma once

class CLiDARPoint
{
public:
	// Default constructor - must *not* be used.
	CLiDARPoint(void);

	// Constructor
	CLiDARPoint(double dx, double dy, double dz, int intensity);
	
	~CLiDARPoint(void);
public:
	double	m_dX;			// x-coordinate
	double	m_dY;			// y-coordinate
	double	m_dZ;			// z-coordinate
	int		m_iIntensity;	// intensity
};
