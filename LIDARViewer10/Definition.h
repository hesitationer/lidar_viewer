/***********************************************************************
 * File Name:	Definition.h
 * Description: This file contains the struct or type definitions used
				in this program.
 ***********************************************************************/

#pragma once

#include <deque>
#include "LiDARPoint.h"

using namespace std;

/*
 * LiDAR Images
 */
#define	NUM_IMG			4
#define	MAX_HEIGHT_IMG	0
#define	MIN_HEIGHT_IMG	1
#define AVG_HEIGHT_IMG	2
#define	DENSITY_IMG		3

/*
 * Zoom Factor
 */
#define ZOOM_FACTOR		4

/*
 * Deque for pointers to the CLiDARPoint class
 */ 
typedef deque<CLiDARPoint *>	DEQUE_LIDAR_POINTS;

/*
 * struct XY_MBR
 * This struct stores the extream value for bottom, top, left and right.
 */
struct XY_MBR
{
	double	dMinY;
	double	dMinX;
	double	dMaxY;
	double	dMaxX;
};

/*
 * struct XYZ_MBR
 * This struct stores the extream value for x, y and z.
 */
struct XYZ_MBR
{
	double	dMinX;
	double	dMinY;
	double	dMinZ;
	double	dMaxX;
	double	dMaxY;
	double	dMaxZ;
};

/*
 * struct LIDAR_POINT
 */
struct LIDAR_POINT
{
	double	dx;
	double	dy;
	double	dz;
	int		icolour;
};

/*
 * struct XY_POSITION
 */
struct XY_POSITION
{
	int	iX;
	int	iY;
};

/*
 * struct PAIR_DOUBLE
 */
struct PAIR_DOUBLE
{
	double dVal1;
	double dVal2;
};

#define TOTAL_COLOR 256

/*
 * struct R_G_B
 */
struct R_G_B
{
	BYTE	r;
	BYTE	g;
	BYTE	b;
};

