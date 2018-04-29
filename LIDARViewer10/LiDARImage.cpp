#include "StdAfx.h"
#include ".\lidarimage.h"

CLiDARImage::CLiDARImage(void)
{
}

CLiDARImage::~CLiDARImage(void)
{
}

/* void CLiDARImage::ExchangeImage(CLiDARImage & Img)
 * 
 * Description: To detache and destroy the current bitmap from this object.
 *				Then, attache the passed-in image to this object. After that,
 *				detache the bitmap from the passed-in object (since it now attach
 *				to a new object)
 * @param Img	The new image to be attached
 */
void CLiDARImage::ExchangeImage(CLiDARImage & Img)
{
	HBITMAP hBitmap = (HBITMAP) Img;
	
	// DETACHES THE BITMAP FROM THE CImage OBJECT AND DESTROY THE BITMAP
	Destroy();
	
	// ATTACHES AN HBITMAP TO A CImage OBJECT. 
	Attach(hBitmap);

	// DETACHES THE BITMAP FROM THE PASSED-IN OBJECT
	Img.Detach();
}

/* BOOL CLiDARImage::AssignSpectralToAPixel()
 * 
 * Description: To assign spectral RGB to a pixel.
 *
 * NOTE:	To speed the processing, the function assumes (x,y) does not exceed
 *			the actual image size.
 *
 */
BOOL CLiDARImage::AssignSpectralToAPixel(int x, int y, int B, int G, int R)
{
	BYTE * pPixel;		

	// Get the pointer to the pixel at (x,y) coordinate
	pPixel = (BYTE *) GetPixelAddress(x,y);
			
	pPixel[2]	= R;	// Red
	pPixel[1]	= G;	// Green
	pPixel[0]	= B;	// Blue

	return TRUE;
}

