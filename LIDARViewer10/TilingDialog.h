#pragma once


// CTilingDialog dialog

class CTilingDialog : public CDialog
{
	DECLARE_DYNAMIC(CTilingDialog)

public:
	CTilingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTilingDialog();

// Dialog Data
	enum { IDD = IDD_TILING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int m_iNumRows;
	int m_iNumCols;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
