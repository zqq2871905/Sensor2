//Copyright(c) 2013, KEYENCE Corporation. All rights reserved.
#pragma once
#include "qdialog.h"
#include "LJV7_IF.h"

class OpenEthernetDlg : public QDialog
{
	//DECLARE_DYNAMIC(OpenEthernetDlg)
public:
	OpenEthernetDlg(QDialog* pParent = NULL);   // Standard constructer
	virtual ~OpenEthernetDlg();

	// Dialog data
	enum { IDD = IDD_OPENETHERNETDLG };

	LJV7IF_ETHERNET_CONFIG GetEthernetConfig();
	BOOL OnInitDialog();
	//afx_msg void OnBnClickedOk();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support

	DECLARE_MESSAGE_MAP()

private:
	CIPAddressCtrl m_xcIPAddress;
	DWORD m_dwIPAddress;
	int m_xvPortNum;
};
