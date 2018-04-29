/***********************************************************************
 * File Name:	LiDARPoint.cpp
 * Description: This file contains the implementation for the CLiDARPoint
 *				class which will represent the LiDAR point.
 ***********************************************************************/

#include "StdAfx.h"
#include ".\lidarpoint.h"

/*
 * Default constructor - must *not* be used.
 */
CLiDARPoint::CLiDARPoint(void)
{
	m_dX			= 0.0;
	m_dY			= 0.0;
	m_dZ			= 0.0;
	m_iIntensity	= 0;
}

/* 
 * Constructor which will receive the coordinates and assign to its member.
 */
CLiDARPoint::CLiDARPoint(double dx, double dy, double dz, int intensity)
{
	m_dX			= dx;
	m_dY			= dy;
	m_dZ			= dz;
	m_iIntensity	= intensity;
}

CLiDARPoint::~CLiDARPoint(void)
{
}
