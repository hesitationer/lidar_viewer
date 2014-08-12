#pragma once
#include "atlimage.h"
#include <math.h>
#include <deque>
#include "stdafx.h"

using namespace std;

const int GRAY_LEVELS = 256;

class CLiDARImage : public CImage
{
public:
	CLiDARImage(void);
	~CLiDARImage(void);

	// Destroy and detach the current bitmap then attach the passed-in bitmap with
	void ExchangeImage(CLiDARImage & Img);

	// Get the height of the image
	int GetImageHeight();

	// Get the width of the image
	int GetImageWidth();

	// To assign spectral RGB to a pixel
	BOOL AssignSpectralToAPixel(int x, int y, int B, int G, int R);

public:
	CView * m_pParent;							// The parent view.
	char m_cstrAppPath[_MAX_PATH];				// The current executable path.

private:
	int		m_nHeight;							// The height of the image.
	int		m_nWidth;							// The width of the image.
};
