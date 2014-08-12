#include "stdafx.h"
#include "Tiling.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <direct.h>

using namespace std;

int Tiling::PerformT(const char *pFilename)
{
	
	ifstream infile;
	infile.open(pFilename);

					CString strShortName;
					char strCName[1024] = {0};

	if (!infile.is_open()){ 
		cerr << "open error" << endl;
	}
	else{
		double fx = 0.;
		double fy = 0.;
		double fz = 0.;
		double finte = 0.;

		double dminX = 99999999.;
		double dminY = 99999999.;
		double dmaxX = 0.;
		double dmaxY = 0.;

		while(!infile.eof()){
			infile >> fx;
			infile >> fy;
			infile >> fz;
			infile >> finte;

			dmaxX = (fx > dmaxX)? fx : dmaxX;
			dminX = (fx < dminX)? fx : dminX;
			dmaxY = (fy > dmaxY)? fy : dmaxY; 
			dminY = (fy < dminY)? fy : dminY;
		}
		infile.close();
		infile.clear();

		double bin_Xw = (dmaxX - dminX)/m_nNumX;
		double bin_Yw = (dmaxY - dminY)/m_nNumY;

		vector< vector<double> > vecBinRan(m_nNumX * m_nNumY, vector<double>(4));

		int m, n;

		char tilingfname[1024];

		for( m=0; m < m_nNumY; ++m){
			for( n=0; n < m_nNumX; ++n){
				vecBinRan[m*m_nNumX+n][0] = dminX + bin_Xw * n;
				vecBinRan[m*m_nNumX+n][1] = dminX + bin_Xw * (n+1); 
				vecBinRan[m*m_nNumX+n][2] = dminY + bin_Yw * m;
				vecBinRan[m*m_nNumX+n][3] = dminY + bin_Yw * (m+1);

				vecBinRan[m*m_nNumX+n][1] = (vecBinRan[m*m_nNumX+n][1] == dmaxX)?dmaxX+0.00001 : vecBinRan[m*m_nNumX+n][1];
				vecBinRan[m*m_nNumX+n][3] = (vecBinRan[m*m_nNumX+n][3] == dmaxY)?dmaxY+0.00001 : vecBinRan[m*m_nNumX+n][3];

				CString strFileName = CString(pFilename);
					int lenght = strFileName.GetLength();
					int strSlash = strFileName.ReverseFind('\\');
					int indexShortName = lenght - strSlash + 1;

					//strShortName = strFileName.Right(indexShortName);
					strShortName = strFileName.Mid(strSlash+1, lenght);
		
					// To get the file name in c-style string
//					char strCName[1024] = {0};
					strcpy(strCName, (LPCTSTR)strShortName);

//				sprintf(tilingfname, ".\\%s_tiled_%d_%d", pFilename, m, n);
				sprintf(tilingfname, ".\\%s_tiled_%d_%d", strCName, m, n);
				_mkdir(tilingfname);

			}
		}

		FILE * fOutput = NULL;
		
		infile.open(pFilename);

		if (!infile.is_open()){ 
			cerr << "open error" << endl;
			return -1;
		}

		bool * ch = new bool[m_nNumX*m_nNumY];
		for(m = 0; m < m_nNumX*m_nNumY; ++m) ch[m] = false;
		
		
//		infile >> finte;
		while(!infile.eof())
		{
			infile >> fx;
			infile >> fy;
			infile >> fz;
			infile >> finte;

			for( m=0; m < m_nNumY; ++m)
			{
				for( n=0; n < m_nNumX; ++n)
				{
					if(fx >= vecBinRan[m*m_nNumX+n][0]
						&& fx < vecBinRan[m*m_nNumX+n][1]
						&& fy >= vecBinRan[m*m_nNumX+n][2]
						&& fy < vecBinRan[m*m_nNumX+n][3])
					{
						sprintf(tilingfname, ".\\%s_tiled_%d_%d\\%s_tiled_%d_%d", strCName, m, n, strCName, m, n);
						if(ch[m*m_nNumX+n] == false)
						{
							fOutput = fopen(tilingfname, "a");
							if (fOutput == NULL)
								exit(0);

							ch[m*m_nNumX+n] = true;
						}
						else{
							fOutput = fopen(tilingfname, "a");
							if (fOutput == NULL)
								exit(0);
						}
						fprintf(fOutput, "%lf\t%lf\t%lf\t%d\n", fx, fy, fz, (int)finte);
						fclose(fOutput);					
						break;
					}
				}
			}
		}
		if (fOutput)
			fclose(fOutput);
		infile.close();
		delete [] ch;
	}
	return 0;

}
