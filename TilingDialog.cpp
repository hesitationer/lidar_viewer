// TilingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "LiDARApplication.h"
#include "TilingDialog.h"
#include ".\tilingdialog.h"
#include "Tiling.h"


// CTilingDialog dialog

IMPLEMENT_DYNAMIC(CTilingDialog, CDialog)
CTilingDialog::CTilingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTilingDialog::IDD, pParent)
{
	m_iNumRows = 0;
	m_iNumCols = 0;
}

CTilingDialog::~CTilingDialog()
{
}

void CTilingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTilingDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CTilingDialog message handlers

void CTilingDialog::OnBnClickedOk()
{
	// Get the number of rows
	m_iNumRows		= GetDlgItemInt(IDC_NUM_ROWS);

	// Get the number of columns
	m_iNumCols		= GetDlgItemInt(IDC_NUM_COLS);

	// Get the file name
	CString strFileName = theApp.m_strFileName;

/*
	CString strShortName;

	int indexShortName = strFileName.GetLength() - strFileName.ReverseFind('\\') + 1;
	strShortName = strFileName.Right(indexShortName);
*/

	// To get the file name in c-style string
	char strCName[1024] = {0};
	strcpy(strCName, (LPCTSTR)strFileName);

	// Call Tiling
	Tiling tile(m_iNumCols, m_iNumRows);
	tile.PerformT(strCName);

	OnOK();
}

void CTilingDialog::OnBnClickedCancel()
{
	OnCancel();
}
