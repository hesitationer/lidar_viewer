#pragma once

class Tiling
{
public:
	Tiling(int X_cnt, int Y_cnt){

		m_nNumX = X_cnt;
		m_nNumY = Y_cnt;

	};

	~Tiling(){}
	
	int PerformT(const char *pFilename);

private:

	int m_nNumX;
	int m_nNumY;

};