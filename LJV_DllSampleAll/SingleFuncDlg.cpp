//Copyright(c) 2013, KEYENCE Corporation. All rights reserved.

// SingleFuncDlg.cpp :Implementation file
//
#include "stdafx.h"
#include <afx.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <Windows.h>
#include "LJV_DllSampleAll.h"
#include "SingleFuncDlg.h"
#include "DataExport.h"
#include "OpenEthernetDlg.h"
#include "GetErrorDlg.h"
#include "ClearErrorDlg.h"
#include "OutSelectDlg.h"
#include "SettingDlg.h"
#include "DepthProgramSelectDlg.h"
#include "TimeDlg.h"
#include "GetProfileDlg.h"
#include "GetBatchProfileDlg.h"
#include "GetBatchProfileAdvanceDlg.h"
#include "GetStorageStatusDlg.h"
#include "GetStorageDataDlg.h"
#include "GetStorageBatchProfileDlg.h"
#include "HighSpeedUsbInitializeDlg.h"
#include "ThreadSafeBuffer.h"
#include "HighSpeedEthernetInitializeDlg.h"
#include "PreStartHighSpeedDlg.h"

// CSingleFuncDlg Dialog

IMPLEMENT_DYNAMIC(CSingleFuncDlg, CDialog)

LJV7IF_PROFILE_INFO CSingleFuncDlg::m_aProfileInfo[LJV7IF_DEVICE_COUNT];

CSingleFuncDlg::CSingleFuncDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSingleFuncDlg::IDD, pParent)
	, m_xvCurrentDeviceID(0)
	, m_xvHeadNum(1)
	, m_xvXRange(0)
	, m_xvBinning(0)
	, m_xvIsEnvelope(TRUE)
	, m_xvXCompression(0)
	, m_xvSaveFilePath(_T(""))
	, m_xvIsCountOnly(TRUE)
	, m_xvIsStartTimer(FALSE)
{
	// Save file path
	CString strFilePath = NULL;
	TCHAR tcPath[MAX_PATH];
	if (::GetModuleFileName(NULL, tcPath, MAX_PATH))
	{
		TCHAR tcDrive[_MAX_DRIVE];		// Drive name
		TCHAR tcDir[_MAX_DIR];			// Directory name
		TCHAR tcFileName[_MAX_FNAME];	// File name
		TCHAR tcExtension[_MAX_PATH];	// Extension name

		_tsplitpath_s( tcPath, tcDrive, _MAX_DRIVE, tcDir, _MAX_DIR, tcFileName, _MAX_FNAME, tcExtension, _MAX_EXT );
		strFilePath += tcDrive;
		strFilePath += tcDir;
		strFilePath += "outputfile.txt";
	}
	//m_xvSaveFilePath = strFilePath;
}

CSingleFuncDlg::~CSingleFuncDlg()
{
}

void CSingleFuncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_COMMANDLOG, m_strCommandLog);
	DDX_Control(pDX, IDC_COMMANDLOG, m_editCommandLog);
	DDX_Control(pDX, IDC_STATIC_DEVICESTATUS0, m_stxtConnectionStatus[0]);
	DDX_Control(pDX, IDC_STATIC_DEVICESTATUS1, m_stxtConnectionStatus[1]);
	DDX_Control(pDX, IDC_STATIC_DEVICESTATUS2, m_stxtConnectionStatus[2]);
	DDX_Control(pDX, IDC_STATIC_DEVICESTATUS3, m_stxtConnectionStatus[3]);
	DDX_Control(pDX, IDC_STATIC_DEVICESTATUS4, m_stxtConnectionStatus[4]);
	DDX_Control(pDX, IDC_STATIC_DEVICESTATUS5, m_stxtConnectionStatus[5]);

	DDX_Radio(pDX, IDC_RD_STATE0, m_xvCurrentDeviceID);
	DDX_Radio(pDX, IDC_RD_ONEHEAD, m_xvHeadNum);
	DDX_Control(pDX, IDC_CMB_RANGE, m_xcXRange);
	DDX_CBIndex(pDX, IDC_CMB_RANGE, m_xvXRange);
	DDX_CBIndex(pDX, IDC_CMB_BINNING, m_xvBinning);
	DDX_Control(pDX, IDC_CMB_BINNING, m_xcBinning);
	DDX_Check(pDX, IDC_CHK_ENVELOPE, m_xvIsEnvelope);

	DDX_Control(pDX, IDC_SPIN_STARTTIMER, m_xcSpinStartTimer);
	DDX_Control(pDX, IDC_SPIN_PROFINDEX, m_xcSpinProfIndex);
	DDX_Control(pDX, IDC_CMB_COMP, m_xcXCompression);
	DDX_CBIndex(pDX, IDC_CMB_COMP, m_xvXCompression);
	DDX_Text(pDX, IDC_EDIT_SAVEFILEPATH, m_xvSaveFilePath);
	DDX_Check(pDX, IDC_CHK_COUNTONLY, m_xvIsCountOnly);
	DDX_Check(pDX, IDC_CHK_TIMER, m_xvIsStartTimer);
}


BEGIN_MESSAGE_MAP(CSingleFuncDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_INITIALIZE, &CSingleFuncDlg::OnBnClickedBtnInitialize)
	ON_BN_CLICKED(IDC_CHK_TIMER, &CSingleFuncDlg::OnBnClickedChkTimer)
	ON_BN_CLICKED(IDC_BTN_SAVE_DESTINATION, &CSingleFuncDlg::OnBnClickedBtnSaveDestination)
	ON_BN_CLICKED(IDC_BTN_GETVERSION, &CSingleFuncDlg::OnBnClickedBtnGetversion)
	ON_BN_CLICKED(IDC_BTN_CLEARLOG, &CSingleFuncDlg::OnBnClickedBtnClearlog)
	ON_BN_CLICKED(IDC_SAVE_PROFILE, &CSingleFuncDlg::OnBnClickedSaveProfile)
	ON_BN_CLICKED(IDC_SAVEMEASUREMENTVALUE, &CSingleFuncDlg::OnBnClickedSavemeasurementvalue)
	ON_BN_CLICKED(IDC_BTN_FINALIZE, &CSingleFuncDlg::OnBnClickedBtnFinalize)
	ON_BN_CLICKED(IDC_BTN_USBOPEN, &CSingleFuncDlg::OnBnClickedBtnUsbopen)
	ON_BN_CLICKED(IDC_BTN_ETHERNETOPEN, &CSingleFuncDlg::OnBnClickedBtnEthernetopen)
	ON_BN_CLICKED(IDC_BTN_COMMCLOSE, &CSingleFuncDlg::OnBnClickedBtnCommclose)
	ON_BN_CLICKED(IDC_BTN_REBOOTCONTROLLER, &CSingleFuncDlg::OnBnClickedBtnRebootcontroller)
	ON_BN_CLICKED(IDC_BTN_RETURNTOFACTORYSETTIG, &CSingleFuncDlg::OnBnClickedBtnReturntofactorysettig)
	ON_BN_CLICKED(IDC_BTN_GETERROR, &CSingleFuncDlg::OnBnClickedBtnGeterror)
	ON_BN_CLICKED(IDC_BTN_CLEARERROR, &CSingleFuncDlg::OnBnClickedBtnClearerror)
	ON_BN_CLICKED(IDC_BTN_TRIGGER, &CSingleFuncDlg::OnBnClickedBtnTrigger)
	ON_BN_CLICKED(IDC_BTN_STARTMEASURE, &CSingleFuncDlg::OnBnClickedBtnStartmeasure)
	ON_BN_CLICKED(IDC_BTN_STOPMEASURE, &CSingleFuncDlg::OnBnClickedBtnStopmeasure)
	ON_BN_CLICKED(IDC_BTN_AUTOZERO, &CSingleFuncDlg::OnBnClickedBtnAutozero)
	ON_BN_CLICKED(IDC_BTN_TIMING, &CSingleFuncDlg::OnBnClickedBtnTiming)
	ON_BN_CLICKED(IDC_BTN_RESET, &CSingleFuncDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_CLEARMEMORY, &CSingleFuncDlg::OnBnClickedBtnClearmemory)
	ON_BN_CLICKED(IDC_BTN_SETSETTIG, &CSingleFuncDlg::OnBnClickedBtnSetsettig)
	ON_BN_CLICKED(IDC_BTN_GETSETTING, &CSingleFuncDlg::OnBnClickedBtnGetsetting)
	ON_BN_CLICKED(IDC_BTN_INITIALIZESETTING, &CSingleFuncDlg::OnBnClickedBtnInitializesetting)
	ON_BN_CLICKED(IDC_BTN_REFLECTSETTING, &CSingleFuncDlg::OnBnClickedBtnReflectsetting)
	ON_BN_CLICKED(IDC_BTN_REWRITETEMPORARYSETTING, &CSingleFuncDlg::OnBnClickedBtnRewritetemporarysetting)
	ON_BN_CLICKED(IDC_BTN_CHECKMEMORYACCESS, &CSingleFuncDlg::OnBnClickedBtnCheckmemoryaccess)
	ON_BN_CLICKED(IDC_BTN_SETTIME, &CSingleFuncDlg::OnBnClickedBtnSettime)
	ON_BN_CLICKED(IDC_BTN_GETTIME, &CSingleFuncDlg::OnBnClickedBtnGettime)
	ON_BN_CLICKED(IDC_BTN_CHANGEACTIVEPROGRAM, &CSingleFuncDlg::OnBnClickedBtnChangeactiveprogram)
	ON_BN_CLICKED(IDC_BTN_GETACTIVEPROGRAM, &CSingleFuncDlg::OnBnClickedBtnGetactiveprogram)
	ON_BN_CLICKED(IDC_BTN_GETMEASUREMENTVALUE, &CSingleFuncDlg::OnBnClickedBtnGetmeasurementvalue)
	ON_BN_CLICKED(IDC_BTN_GETPROFILE, &CSingleFuncDlg::OnBnClickedBtnGetprofile)
	ON_BN_CLICKED(IDC_BTN_GETBATCHPROFILEE, &CSingleFuncDlg::OnBnClickedBtnGetbatchprofilee)
	ON_BN_CLICKED(IDC_BTN_GETPROFILEADVANCE, &CSingleFuncDlg::OnBnClickedBtnGetprofileadvance)
	ON_BN_CLICKED(IDC_BTN_GETBATCHPROFILEADVANCE, &CSingleFuncDlg::OnBnClickedBtnGetbatchprofileadvance)
	ON_BN_CLICKED(IDC_BTN_STARTSTORAGE, &CSingleFuncDlg::OnBnClickedBtnStartstorage)
	ON_BN_CLICKED(IDC_BTN_STOPSTORAGE, &CSingleFuncDlg::OnBnClickedBtnStopstorage)
	ON_BN_CLICKED(IDC_BTN_GETSTORAGESTATUS, &CSingleFuncDlg::OnBnClickedBtnGetstoragestatus)
	ON_BN_CLICKED(IDC_BTN_GETSTORAGEDATA, &CSingleFuncDlg::OnBnClickedBtnGetstoragedata)
	ON_BN_CLICKED(IDC_BTN_GETSTORAGEPROFILE, &CSingleFuncDlg::OnBnClickedBtnGetstorageprofile)
	ON_BN_CLICKED(IDC_BTN_GETSTORAGEBATCHPROFILE, &CSingleFuncDlg::OnBnClickedBtnGetstoragebatchprofile)
	ON_BN_CLICKED(IDC_BTN_HIGHSPEEDDATAUSBCOMMUNICATIONINITALIZE, &CSingleFuncDlg::OnBnClickedBtnHighspeeddatausbcommunicationinitalize)
	ON_BN_CLICKED(IDC_BTN_HIGHSPEEDDATAETHERNETCOMMUNICATIONINITALIZE, &CSingleFuncDlg::OnBnClickedBtnHighspeeddataethernetcommunicationinitalize)
	ON_BN_CLICKED(IDC_BTN_PRESTARTHIGHSPEEDDATACOMMUNICATION, &CSingleFuncDlg::OnBnClickedBtnPrestarthighspeeddatacommunication)
	ON_BN_CLICKED(IDC_BTN_STARTHIGHSPEEDDATACOMMUNICATION, &CSingleFuncDlg::OnBnClickedBtnStarthighspeeddatacommunication)
	ON_BN_CLICKED(IDC_BTN_STOPHIGHSPEEDDATACOMMUNICATION, &CSingleFuncDlg::OnBnClickedBtnStophighspeeddatacommunication)
	ON_BN_CLICKED(IDC_BTN_HIGHSPEEDDATACOMMUNICATIONFINALIZE, &CSingleFuncDlg::OnBnClickedBtnHighspeeddatacommunicationfinalize)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSingleFuncDlg Message handler
BOOL CSingleFuncDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set Spin control parameter
	m_xcSpinProfIndex.SetRange32(0, 65535);
	m_xcSpinStartTimer.SetPos32(0);
	m_xcSpinStartTimer.SetRange32(0, 1000);
	m_xcSpinStartTimer.SetPos32(500);

	for(int i = 0; i < LJV7IF_DEVICE_COUNT; i++)
	{
		m_anProfReceiveCnt[i] = 0;
	}

	m_sendCommand = SENDCOMMAND_NONE;

	return TRUE;
}


/*
 "Clear the log" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnClearlog()
{
	UpdateData(TRUE);

	m_strCommandLog = "";
	UpdateData(FALSE);
}

/*
 "…" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnSaveDestination()
{
	UpdateData(TRUE);

	CFileDialog selDlg(FALSE, _T("txt"), m_xvSaveFilePath);

	if (selDlg.DoModal() == IDOK)
	{
		m_xvSaveFilePath = selDlg.GetPathName();
		UpdateData(FALSE);
	}
}

/*
 "Save the profile" button clicked
*/
void CSingleFuncDlg::OnBnClickedSaveProfile()
{
	UpdateData(TRUE);

	unsigned int profileNumber = m_xcSpinProfIndex.GetPos32();		// The limit is 2^16-1
	
	if(profileNumber >= m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.size())
	{
		ShowMessage(_T("There is no profile specified.\r\n"));
		return;
	}

	if(CDataExport::ExportProfile(&(m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.at(0)), m_xvSaveFilePath, profileNumber))
	{
		ShowMessage(_T("Saved."));
	}
}

/*
 "Save the measurement value" button clicked
*/
void CSingleFuncDlg::OnBnClickedSavemeasurementvalue()
{
	UpdateData(TRUE);

	CDataExport::ExportMeasureDatas(m_vecMeasureData, m_xvSaveFilePath);
	
	CString strSaveFilePath = m_xvSaveFilePath + "2.txt";
	CDataExport::ExportMeasureDatas(m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData, strSaveFilePath);
}

/*
 "Initialize" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnInitialize()
{
	UpdateData(TRUE);

	int nRc = LJV7IF_Initialize();
	DisplayCommandLog(nRc, IDS_SCMD_INITIALIZE);

	for (int i = 0; i < LJV7IF_DEVICE_COUNT; i++)
	{
		m_aDeviceData[i].m_deviceStatus = CDeviceData::DEVICESTATUS_NoConnection;
		SetDlgItemText(m_stxtConnectionStatus[i].GetDlgCtrlID(), m_aDeviceData[i].GetStatusString() );
	}
}

/*
 "GetVersion" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetversion()
{
	UpdateData(TRUE);

	int nVersion = LJV7IF_GetVersion();
	DisplayCommandLog(LJV7IF_RC_OK, IDS_SCMD_GETVERSION);
	AddResult(_T("version:%X \r\n"), nVersion);
}

/*
 "Finalize" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnFinalize()
{
	UpdateData(TRUE);

	int nRc = LJV7IF_Finalize();
	DisplayCommandLog(nRc, IDS_SCMD_FINALIZE);
	for (int i = 0; i < LJV7IF_DEVICE_COUNT; i++)
	{
		m_aDeviceData[i].m_deviceStatus = CDeviceData::DEVICESTATUS_NoConnection;
		SetDlgItemText(	m_stxtConnectionStatus[i].GetDlgCtrlID(), m_aDeviceData[i].GetStatusString() );
	}
}

/*
 "Usbopen" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnUsbopen()
{
	// @Point
	// # Enter the "m_xvCurrentDeviceID" set here for the communication settings into the arguments of each DLL function.
	// # If you want to get data from multiple controllers, prepare and set multiple "m_xvCurrentDeviceID" values,
	//   enter these values into the arguments of the DLL functions, and then use the functions.

	UpdateData(TRUE);
	int nRc = LJV7IF_UsbOpen(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_USBOPEN);

	CDeviceData::DEVICESTATUS status = (nRc == LJV7IF_RC_OK) ? CDeviceData::DEVICESTATUS_Usb : CDeviceData::DEVICESTATUS_NoConnection;
	m_aDeviceData[m_xvCurrentDeviceID].m_deviceStatus = status;
	SetDlgItemText(m_stxtConnectionStatus[m_xvCurrentDeviceID].GetDlgCtrlID(), m_aDeviceData[m_xvCurrentDeviceID].GetStatusString());
}

/*
 "Ethernetopen" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnEthernetopen()
{
	// @Point
	// # Enter the "m_xvCurrentDeviceID" set here for the communication settings into the arguments of each DLL function.
	// # If you want to get data from multiple controllers, prepare and set multiple "m_xvCurrentDeviceID" values,
	//   enter these values into the arguments of the DLL functions, and then use the functions.	

	UpdateData(TRUE);

	COpenEthernetDlg communicaionSettinDlg;
	if(communicaionSettinDlg.DoModal() == IDOK)
	{
		LJV7IF_ETHERNET_CONFIG ethernetConfig = communicaionSettinDlg.GetEthernetConfig();
		int nRc = LJV7IF_EthernetOpen(m_xvCurrentDeviceID, &ethernetConfig);

		CDeviceData::DEVICESTATUS status = (nRc == LJV7IF_RC_OK) ? CDeviceData::DEVICESTATUS_Ethernet : CDeviceData::DEVICESTATUS_NoConnection;
		m_aDeviceData[m_xvCurrentDeviceID].m_deviceStatus = status;
		m_aDeviceData[m_xvCurrentDeviceID].m_ethernetConfig = ethernetConfig;
		SetDlgItemText(	m_stxtConnectionStatus[m_xvCurrentDeviceID].GetDlgCtrlID(), m_aDeviceData[m_xvCurrentDeviceID].GetStatusString() );

		DisplayCommandLog(nRc, IDS_SCMD_EHTERNETOPEN);
	}
}

/*
 "CommClose" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnCommclose()
{
	UpdateData(TRUE);

	int nRc = LJV7IF_CommClose(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_COMMCLOSE);

	m_aDeviceData[m_xvCurrentDeviceID].m_deviceStatus = CDeviceData::DEVICESTATUS_NoConnection;
	SetDlgItemText( m_stxtConnectionStatus[m_xvCurrentDeviceID].GetDlgCtrlID(), m_aDeviceData[m_xvCurrentDeviceID].GetStatusString() );
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
}


/*
 "RebootController" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnRebootcontroller()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_REBOOTCONTROLLER;
	int nRc = LJV7IF_RebootController(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_REBOOTCONTROLLER);
}

/*
 "ReturnToFactorySetting" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnReturntofactorysettig()
{
	UpdateData(TRUE);

	int nRc = LJV7IF_RetrunToFactorySetting(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_RETURNTOFACTORYSETTING);
}

/*
 "GetError" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGeterror()
{
	UpdateData(TRUE);

	CGetErrorDlg getErrorDlg;
	if(getErrorDlg.DoModal() == IDOK)
	{
		BYTE byErrorCnt = 0;
		BYTE byReceiveMax = getErrorDlg.m_xvErrorCnt;
		vector<WORD> errCode(max<int>(1, byReceiveMax));
		
		int nRc = LJV7IF_GetError(m_xvCurrentDeviceID, byReceiveMax, &byErrorCnt, (WORD*)&errCode.at(0));
		DisplayCommandLog(nRc, IDS_SCMD_GETERROR);

		// Display Error Code
		if(nRc == LJV7IF_RC_OK)
		{
			AddResult(_T("Number of Errors:%X\r\n"), byErrorCnt);
			
			CString strLog;
			for(int i = 0; i < byErrorCnt; i++)
			{
				strLog.Format(_T("ErrCnt[%X]:(0x%04X)\r\n"), i, errCode[i]);
				m_strCommandLog += (LPCTSTR) strLog;
			}
			UpdateData(FALSE);
			m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
		}
	}
}

/*
 "ClearError" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnClearerror()
{
	UpdateData(TRUE);

	CClearErrorDlg clearErrorDlg;
	if(clearErrorDlg.DoModal() == IDOK)
	{
		int nRc = LJV7IF_ClearError(m_xvCurrentDeviceID, clearErrorDlg.GetErrorCode());
		DisplayCommandLog(nRc, IDS_SCMD_CLEARERROR);
	}
}

/*
 "Trigger" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnTrigger()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_TRIGGER;
	int nRc = LJV7IF_Trigger(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_TRIGGER);
}

/*
 "StartMeasure" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnStartmeasure()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_STARTMEASURE;
	int nRc = LJV7IF_StartMeasure(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_STARTMEASURE);
}

/*
 "StopMeasure" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnStopmeasure()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_STOPMEASURE;
	int nRc = LJV7IF_StopMeasure(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_STOPMEASURE);
}

/*
 "AutoZero" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnAutozero()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_AUTOZERO;
	COutSelectDlg autoZeroDlg(TRUE);
	int nRc = 0;
	if(autoZeroDlg.DoModal() == IDOK)
	{
		nRc = LJV7IF_AutoZero(m_xvCurrentDeviceID, autoZeroDlg.m_xvOnOffReq, autoZeroDlg.GetSelection());
		DisplayCommandLog(nRc, IDS_SCMD_AUTOZERO);
	}
}

/*
 "Timing" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnTiming()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_TIMING;
	COutSelectDlg timingDlg(TRUE);
	if(timingDlg.DoModal() == IDOK)
	{
		int nRc = LJV7IF_Timing(m_xvCurrentDeviceID, timingDlg.m_xvOnOffReq, timingDlg.GetSelection());
		DisplayCommandLog(nRc, IDS_SCMD_TIMING);
	}
}

/*
 "Reset" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnReset()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_RESET;
	COutSelectDlg resetDlg(FALSE);
	if(resetDlg.DoModal() == IDOK)
	{
		int nRc = LJV7IF_Reset(m_xvCurrentDeviceID, resetDlg.GetSelection());
		DisplayCommandLog(nRc, IDS_SCMD_RESET);
	}
}

/*
 "ClearMemory" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnClearmemory()
{
	UpdateData(TRUE);

	int nRc = LJV7IF_ClearMemory(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_CLEARMEMORY);
}

/*
 "SetSetting" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnSetsettig()
{
	UpdateData(TRUE);

	CSettingDlg setSettingDlg(TRUE);
	if(setSettingDlg.DoModal() == IDOK)
	{
		BYTE byDepth = setSettingDlg.GetDepth();
		LJV7IF_TARGET_SETTING targetSetting = setSettingDlg.GetTargetSetting();
		vector<BYTE> vecSendData = setSettingDlg.GetSendData();
		DWORD dwDataSize = setSettingDlg.m_xvDataAmount;
		DWORD dwError = 0;

		int nRc = LJV7IF_SetSetting(m_xvCurrentDeviceID, byDepth, targetSetting, &vecSendData.at(0), dwDataSize, &dwError);
		
		// @Point
		// # There are three setting areas: a) the write settings area, b) the running area, and c) the save area.
		//   * Specify a) for the setting level when you want to change multiple settings. However, to reflect settings in the LJ-V operations, you have to call LJV7IF_ReflectSetting.
		//	 * Specify b) for the setting level when you want to change one setting but you don't mind if this setting is returned to its value prior to the change when the power is turned off.
		//	 * Specify c) for the setting level when you want to change one setting and you want this new value to be retained even when the power is turned off.

		// @Point
		//  As a usage example, we will show how to use SettingForm to configure settings such that sending a setting, with SettingForm using its initial values,
		//  will change the sampling period in the running area to "100 Hz."
		//  Also see the GetSetting function.

		DisplayCommandLog(nRc, IDS_SCMD_SETSETTING);
		if(nRc != LJV7IF_RC_OK) 
		{
			AddResult( _T("Error Code:0x%08X\r\n"), dwError);
		}
	}
}

/*
 "GetSetting" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetsetting()
{
	UpdateData(TRUE);

	CSettingDlg getSettingDlg(FALSE);
	vector<BYTE> vecReceiveData;
	if(getSettingDlg.DoModal() == IDOK)
	{
		BYTE byDepth = getSettingDlg.GetDepth();
		LJV7IF_TARGET_SETTING targetSetting = getSettingDlg.GetTargetSetting();
		DWORD dwDataSize = getSettingDlg.m_xvDataAmount;
		DWORD dwError = 0;

		for(DWORD i = 0; i < dwDataSize; i++)
		{
			vecReceiveData.push_back(0);
		}
		int nRc = LJV7IF_GetSetting(m_xvCurrentDeviceID, byDepth, targetSetting, &vecReceiveData.at(0), dwDataSize);
		// @Point
		//  We have prepared an object for reading the sampling period into the setting's initial value.
		//  Also see the SetSetting function.

		DisplayCommandLog(nRc, IDS_SCMD_GETSETTING);
		
		CString strLog ;
		m_strCommandLog += _T("\t   0   1   2   3   4   5   6   7  \r\n");
		for(UINT i = 0; i < vecReceiveData.size(); i++)
		{
			if((i % 8) == 0) 
			{
				strLog.Format(_T("[0x%04X] "), i);
				m_strCommandLog += (LPCTSTR) strLog;
			}
			strLog.Format(_T(" %02X"), vecReceiveData.at(i));
			m_strCommandLog += (LPCTSTR) strLog;

			if((i % 8) == 7 || i == dwDataSize -1) 
			{
				strLog.Format(_T("\r\n"));
				m_strCommandLog += (LPCTSTR) strLog;
			}
		}
		m_strCommandLog += (LPCTSTR) _T("\r\n");

		UpdateData(FALSE);
		m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
	}
}

/*
 "InitializeSetting" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnInitializesetting()
{
	UpdateData(TRUE);

	CDepthProgramSelectDlg depthProgramSelectDlg(TRUE, TRUE);
	if(depthProgramSelectDlg.DoModal() == IDOK)
	{
		int nRc = LJV7IF_InitializeSetting(m_xvCurrentDeviceID, depthProgramSelectDlg.GetDepth(), depthProgramSelectDlg.GetTarget());
		DisplayCommandLog(nRc, IDS_SCMD_INITIALIZESETTING);
	}
}

/*
 "ReflectSetting" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnReflectsetting()
{
	UpdateData(TRUE);

	CDepthProgramSelectDlg depthSelectDlg(FALSE, TRUE);
	if(depthSelectDlg.DoModal() == IDOK)
	{
		DWORD dwError = 0;
		int nRc = LJV7IF_ReflectSetting(m_xvCurrentDeviceID, depthSelectDlg.GetDepth(), &dwError);
		DisplayCommandLog(nRc, IDS_SCMD_REFLECTSETTING);
		if(nRc != LJV7IF_RC_OK) 
		{
			AddResult(_T("Error Code:0x%08X\r\n"), dwError);
		}
	}
}

/*
 "RewriteTemporarySettig" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnRewritetemporarysetting()
{
	UpdateData(TRUE);

	CDepthProgramSelectDlg depthSelectDlg(FALSE, TRUE);
	if(depthSelectDlg.DoModal() == IDOK)
	{
		int nRc = LJV7IF_RewriteTemporarySetting(m_xvCurrentDeviceID, depthSelectDlg.GetDepth());
		DisplayCommandLog(nRc, IDS_SCMD_REWRITETEMPORARYSETTING);
	}
}

/*
 "CheckMemoryAccess" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnCheckmemoryaccess()
{
	UpdateData(TRUE);

	BYTE byBusy = 0;
	int nRc = LJV7IF_CheckMemoryAccess(m_xvCurrentDeviceID, &byBusy);
	DisplayCommandLog(nRc, IDS_SCMD_CHECKMEMORYACCESS);

	if(nRc == LJV7IF_RC_OK)
	{
		CString strLog;
		if(byBusy != 0)
		{
			m_strCommandLog += _T("Accessing the save area \r\n");
		}
		else
		{
			m_strCommandLog += _T("No access \r\n");
		}
		UpdateData(FALSE);
		m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
	}
}

/*
 "SetTime" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnSettime()
{
	UpdateData(TRUE);

	CTimeDlg timeDlg;
	if(timeDlg.DoModal() == IDOK)
	{
		LJV7IF_TIME time = timeDlg.GetTimeParameter();
		int nRc = LJV7IF_SetTime(m_xvCurrentDeviceID, &time);
		DisplayCommandLog(nRc, IDS_SCMD_SETTIME);
	}
}

/*
 "GetTime" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGettime()
{
	UpdateData(TRUE);

	LJV7IF_TIME time;
	int nRc = LJV7IF_GetTime(m_xvCurrentDeviceID, &time);
	DisplayCommandLog(nRc, IDS_SCMD_GETTIME);
	if(nRc == LJV7IF_RC_OK)
	{
		CString strLog;
		strLog.Format( _T("20%d/%2d/%2d   %d:%d:%d \r\n"), time.byYear, time.byMonth, time.byDay, time.byHour, time.byMinute, time.bySecond );
		m_strCommandLog += (LPCTSTR) strLog;
		UpdateData(FALSE);
		m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
	}
}

/*
 "ChangeActiveProgram" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnChangeactiveprogram()
{
	UpdateData(TRUE);
	
	m_sendCommand = SENDCOMMAND_CHANGEACTIVEPROGRAM;
	CDepthProgramSelectDlg programSelectDlg(TRUE, FALSE);
	if(programSelectDlg.DoModal() == IDOK)
	{
		int nRc = LJV7IF_ChangeActiveProgram(m_xvCurrentDeviceID, programSelectDlg.GetTarget());
		DisplayCommandLog(nRc, IDS_SCMD_CHANGEACTIVEPROGRAM);
	}
}

/*
 "GetActiveProgram" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetactiveprogram()
{
	UpdateData(TRUE);

	BYTE progNo = 0;
	int nRc = LJV7IF_GetActiveProgram(m_xvCurrentDeviceID, &progNo);
	DisplayCommandLog(nRc, IDS_SCMD_GETACTIVEPROGRAM);
	if (nRc == LJV7IF_RC_OK)
	{
		AddResult(_T("Program No :%d\r\n"), progNo);
	}
}

/*
 "GetMeasurementValue" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetmeasurementvalue()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_GETMEASUREMENTVALUE;
	LJV7IF_MEASURE_DATA aMeasureData[LJV7IF_OUT_COUNT];

	int nRc = LJV7IF_GetMeasurementValue(m_xvCurrentDeviceID, aMeasureData);
	DisplayCommandLog(nRc, IDS_SCMD_GETMEASUREMENTVALUE);
	if (nRc == LJV7IF_RC_OK)
	{
		// Stores the result
		m_vecMeasureData.clear();
		m_vecMeasureData.push_back(aMeasureData);

		// Display the result
		CString strLog;
		for(int i = 0; i < LJV7IF_OUT_COUNT; i++)
		{
			strLog.Format( _T("OUT%2d : %s %g\r\n"), i+1, GetMeasureDataInfo(aMeasureData[i]), aMeasureData[i].fValue);
			m_strCommandLog += strLog;	
		}
		UpdateData(FALSE);
		m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
	}
}

/*
 "GetProfile" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetprofile()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_GETPROFILE;
	CGetProfileDlg getProfileDlg;
	if(getProfileDlg.DoModal() == IDOK)
	{
		LJV7IF_GET_PROFILE_REQ req = getProfileDlg.GetProfileReq();
		LJV7IF_GET_PROFILE_RSP rsp;
		LJV7IF_PROFILE_INFO profileInfo;
		DWORD dwOneDataSize = GetOneProfileDataSize();
		DWORD dwAllDataSize = dwOneDataSize * req.byGetProfCnt;
		vector<int> vecProfileData(dwAllDataSize/sizeof(DWORD));

		// Send Command
		int nRc = LJV7IF_GetProfile(m_xvCurrentDeviceID, &req, &rsp, &profileInfo, (DWORD*)&vecProfileData.at(0), dwAllDataSize);
		
		// Result output
		DisplayCommandLog(nRc, IDS_SCMD_GETPROFILE);

		if(nRc == LJV7IF_RC_OK)
		{
			// Clear the data
			m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.clear();
			m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.clear();

			// Show response
			ShowResponse(rsp);
			ShowProfileInfo(profileInfo);
			

			// Analize the data
			int nDataUnitSize = (sizeof(LJV7IF_PROFILE_HEADER) + sizeof(int) * profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1) + sizeof(LJV7IF_PROFILE_FOOTER)) / sizeof(int);
			for(int i = 0; i < rsp.byGetProfCnt; i++)
			{
				int *pnBlock = &vecProfileData.at(nDataUnitSize * i);
				
				LJV7IF_PROFILE_HEADER* pHeader = (LJV7IF_PROFILE_HEADER*)pnBlock;
				int* pnProfileData			   = pnBlock + (sizeof(LJV7IF_PROFILE_HEADER) / sizeof(DWORD));
				LJV7IF_PROFILE_FOOTER* pFooter = (LJV7IF_PROFILE_FOOTER*)(pnProfileData + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1));

				// Store the profile data
				m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.push_back(PROFILE_DATA(profileInfo, pHeader, pnProfileData, pFooter));
			}

			// Profile output(one profile only)
			if(CDataExport::ExportProfile(&(m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.at(0)), m_xvSaveFilePath, 0))
			{
				ShowMessage(_T("###Save!!"));
			}
		}
	}
}

/*
 "GetBatchProfile" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetbatchprofilee()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_GETBATCHPROFILE;
	CGetBatchProfileDlg getBatchProfile;
	if(getBatchProfile.DoModal() == IDOK)
	{	
		LJV7IF_GET_BATCH_PROFILE_REQ req = getBatchProfile.GetBatchProfileReq();
		LJV7IF_GET_BATCH_PROFILE_RSP rsp;
		LJV7IF_PROFILE_INFO profileInfo;
		DWORD dwOneDataSize = GetOneProfileDataSize();
		DWORD dwAllDataSize = dwOneDataSize * req.byGetProfCnt;
		vector<int> vecProfileData(dwAllDataSize/sizeof(int));
		
		// Send Command
		int nRc = LJV7IF_GetBatchProfile(m_xvCurrentDeviceID, &req, &rsp, &profileInfo, (DWORD*)&vecProfileData.at(0), dwAllDataSize);
		// @Point
		// # When reading all the profiles from a single batch, the specified number of profiles may not be read.	
		// # To read the remaining profiles after the first set of profiles have been read,
		//    set the specification method (byPosMode)to 0x02, specify the batch number (dwGetBatchNo),
		//    and then set the number to start reading profiles from (dwGetProfNo) and the number of profiles to read (byGetProfCnt) to values 
		//    that specify a range of profiles that have not been read to read the profiles in order.
		// # For the basic code, see "CCombinationFuncDlg::OnBnClickedBtnGetbatchprofiledata"
		
		// Result output
		DisplayCommandLog(nRc, IDS_SCMD_GETBATCHPROFILE);

		if(nRc == LJV7IF_RC_OK)
		{
			// Clear the data
			m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.clear();
			m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.clear();

			// Show Response
			ShowResponse(rsp);
			ShowProfileInfo(profileInfo);

			// Analize the data
			int nDataUnitSize = (sizeof(LJV7IF_PROFILE_HEADER) + sizeof(int) * profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1) + sizeof(LJV7IF_PROFILE_FOOTER)) / sizeof(int);
			for(int i = 0; i < rsp.byGetProfCnt; i++)
			{
				int *pnBlock = &vecProfileData.at(nDataUnitSize * i);
				
				LJV7IF_PROFILE_HEADER* pHeader = (LJV7IF_PROFILE_HEADER*)pnBlock;
				int* pnProfileData             = pnBlock + (sizeof(LJV7IF_PROFILE_HEADER) / sizeof(DWORD));
				LJV7IF_PROFILE_FOOTER* pFooter = (LJV7IF_PROFILE_FOOTER*)(pnProfileData + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1));

				// Store the profile data
				m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.push_back(PROFILE_DATA(profileInfo, pHeader, pnProfileData, pFooter));
			}
		}
	}
}

/*
 "GetProfileAdvance" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetprofileadvance()
{
	UpdateData(TRUE);
	m_sendCommand = SENDCOMMAND_GETPROFILEADVANCE;
	
	// Setting of function
	LJV7IF_PROFILE_INFO profileInfo;
	DWORD dwProfileDataSize = GetOneProfileDataSize();
	vector<int> vecProfileData( dwProfileDataSize / (sizeof(unsigned int)));
	LJV7IF_MEASURE_DATA aMeasureData[LJV7IF_OUT_COUNT];

	// Send Command
	int nRc = LJV7IF_GetProfileAdvance(m_xvCurrentDeviceID, &profileInfo, (DWORD*)&vecProfileData.at(0), dwProfileDataSize, aMeasureData);
	
	// Result output
	DisplayCommandLog(nRc, IDS_SCMD_GETPROFILEADVANCE);

	if(nRc == LJV7IF_RC_OK)
	{
		// Clear the data
		m_vecMeasureData.clear();
		m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.clear();
		m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.clear();

		ShowProfileInfo(profileInfo);

		// Analize the data
		int *pnBlock = &vecProfileData.at(0);
		LJV7IF_PROFILE_HEADER* pHeader = (LJV7IF_PROFILE_HEADER*)pnBlock;
		int* pnProfileData             = pnBlock + (sizeof(LJV7IF_PROFILE_HEADER) / sizeof(DWORD));
		LJV7IF_PROFILE_FOOTER* pFooter = (LJV7IF_PROFILE_FOOTER*)(pnProfileData + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1));

		// Store the profile data
		m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.push_back(PROFILE_DATA(profileInfo, pHeader, pnProfileData, pFooter));
		m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.at(0);
		
		// Store the measure data
		m_vecMeasureData.push_back(MEASURE_DATA(aMeasureData));

		// Measure data output
		CString strLog;
		for(int i = 0; i < LJV7IF_OUT_COUNT; i++)
		{
			strLog.Format( _T("OUT%2d : %s %g\r\n"), i+1, GetMeasureDataInfo(aMeasureData[i]), aMeasureData[i].fValue);
			m_strCommandLog += strLog;	
		}
		UpdateData(FALSE);
		m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
	}
}

/*
 "GetBatchProfileAdvance" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetbatchprofileadvance()
{
	UpdateData(TRUE);
	
	m_sendCommand = SENDCOMMAND_GETBATCHPROFILEADVANCE;
	CGetBatchProfileAdvanceDlg getBatchProfileAdvance;
	if(getBatchProfileAdvance.DoModal() == IDOK)
	{
		// Clear the data
		m_vecMeasureData.clear();
		m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.clear();
		m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.clear();

		// Setting of fuction
		LJV7IF_GET_BATCH_PROFILE_ADVANCE_REQ req = getBatchProfileAdvance.GetBatchProfileAdvanceReq();
		LJV7IF_GET_BATCH_PROFILE_ADVANCE_RSP rsp;
		LJV7IF_PROFILE_INFO profileInfo;
		DWORD dwOneDataSize = GetOneProfileDataSize() + sizeof(LJV7IF_MEASURE_DATA) * LJV7IF_OUT_COUNT;
		DWORD dwAllDataSize = dwOneDataSize * req.byGetProfCnt;
		vector<int> vecProfileData(dwAllDataSize/sizeof(int));
		LJV7IF_MEASURE_DATA aMeasureData[LJV7IF_OUT_COUNT];
		LJV7IF_MEASURE_DATA aBatchMeasureData[LJV7IF_OUT_COUNT];
		
		// Send Command
		int nRc = LJV7IF_GetBatchProfileAdvance(m_xvCurrentDeviceID, &req, &rsp, &profileInfo, (DWORD*)&vecProfileData.at(0),
			dwAllDataSize, aBatchMeasureData, aMeasureData);
		// @Point
		// # When reading all the profiles frvom a single batch, the specified number of profiles may not be read.	
		// # To read the remaining profiles after the first set of profiles have been read,
		//   set the specification method (byPosMode)to 0x02, specify the batch number (dwGetBatchNo),
		//   and then set the number to start reading profiles from (dwGetProfNo) and the number of profiles to read (byGetProfCnt) to values 
		//   that specify a range of profiles that have not been read to read the profiles in order.
		// # For the basic code, see "CCombinationFuncDlg::OnBnClickedBtnGetbatchprofiledata"
		
		// The resulting output
		DisplayCommandLog(nRc, IDS_SCMD_GETBATCHPROFILEADVANCE);

		if(nRc == LJV7IF_RC_OK)
		{
			ShowResponse(rsp);
			ShowProfileInfo(profileInfo);

			m_vecMeasureData.push_back(aMeasureData);

			// Analize the data
			int nDataUnitSize = (sizeof(LJV7IF_PROFILE_HEADER) + sizeof(LJV7IF_PROFILE_FOOTER) + sizeof(LJV7IF_MEASURE_DATA ) * LJV7IF_OUT_COUNT) / sizeof(int) + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1);

			for(int i = 0; i < rsp.byGetProfCnt; i++)
			{
				int *pnBlock = &vecProfileData.at(nDataUnitSize * i);
				
				LJV7IF_PROFILE_HEADER* pHeader = (LJV7IF_PROFILE_HEADER*)pnBlock;
				int* pnProfileData             = pnBlock + (sizeof(LJV7IF_PROFILE_HEADER) / sizeof(DWORD));
				LJV7IF_PROFILE_FOOTER* pFooter = (LJV7IF_PROFILE_FOOTER*)(pnProfileData + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1));
				MEASURE_DATA* measureData      = (MEASURE_DATA*)(pnProfileData + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1) + sizeof(LJV7IF_PROFILE_FOOTER) / sizeof(DWORD));

				// Store the profile data and measure data
				m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.push_back(PROFILE_DATA(profileInfo, pHeader, pnProfileData, pFooter));
				m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.push_back(*measureData);
			}

			// Measure data output
			CString strLog;
			for(int i = 0; i < LJV7IF_OUT_COUNT; i++)
			{
				strLog.Format( _T("OUT%2d : %s %g\r\n"), i+1, GetMeasureDataInfo(aMeasureData[i]), aMeasureData[i].fValue);
				m_strCommandLog += strLog;	
			}
			UpdateData(FALSE);
			m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());

			// Profile output(one profile only)
			if(CDataExport::ExportProfile(&(m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.at(0)), m_xvSaveFilePath, 0))
			{
				ShowMessage(_T("###Save!!"));
			}
		}
	}
}

/*
 "StartStorage" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnStartstorage()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_STARTSTORAGE;
	int nRc = LJV7IF_StartStorage(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_STARTSTORAGE);
}

/*
 "StopStorage" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnStopstorage()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_STOPSTORAGE;
	int nRc = LJV7IF_StopStorage(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_STOPSTORAGE);
}

/*
 "GetstorageStatus" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetstoragestatus()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_GETSTORAGESTATUS;
	CGetStorageStatusDlg getStorageStatusDlg;
	if(getStorageStatusDlg.DoModal() == IDOK)
	{
		LJV7IF_GET_STRAGE_STATUS_REQ req = getStorageStatusDlg.getStorageStatusReq();
		LJV7IF_GET_STRAGE_STATUS_RSP rsp;
		LJV7IF_STORAGE_INFO info;
		// @Point
		// # dwReadArea is the target surface to read.
		//   The target surface to read indicates where in the internal memory usage area to read.
		// # The method to use in specifying dwReadArea varies depending on how internal memory is allocated.
		//   * Double buffer
		//      0 indicates the active surface, 1 indicates surface A, and 2 indicates surface B.
		//   * Entire area (overwrite)
		//      Fixed to 1
		//   * Entire area (do not overwrite)
		//      After a setting modification, data is saved in surfaces 1, 2, 3, and so on in order, and 0 is set as the active surface.
		// # For details, see "9.2.9.2 Internal memory."

		int nRc = LJV7IF_GetStorageStatus(m_xvCurrentDeviceID, &req, &rsp, &info);
		// @Point
		// # Terminology
		//  * Base time … time expressed with 32 bits (<- the time when the setting was changed)
		//  * Accumulated date and time	 … counter value that indicates the elapsed time, in units of 10 ms, from the base time
		// # The accumulated date and time are stored in the accumulated data.
		// # The accumulated time of read data is calculated as shown below.
		//   Accumulated time = "base time (stBaseTime of LJV7IF_GET_STORAGE_RSP)" + "accumulated date and time × 10 ms"
		
		DisplayCommandLog(nRc, IDS_SCMD_GETSTORAGESTATUS);
		if(nRc == LJV7IF_RC_OK)
		{
			ShowStorageStatusRsp(rsp);
			ShowStrageInfo(info);
		}
	}
}

/*
 "Getstoragedata" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetstoragedata()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_GETSTORAGEDATA;
	CGetStorageDataDlg getStorageDataDlg;
	if(getStorageDataDlg.DoModal() == IDOK)
	{
		LJV7IF_GET_STORAGE_REQ req = getStorageDataDlg.getStorageStatusReq();
		// @Point
		// # dwReadArea is the target surface to read.
		//    The target surface to read indicates where in the internal memory usage area to read.
		// # The method to use in specifying dwReadArea varies depending on how internal memory is allocated.
		//   * Double buffer
		//      0 indicates the active surface, 1 indicates surface A, and 2 indicates surface B.
		//   * Entire area (overwrite)
		//      Fixed to 1
		//   * Entire area (do not overwrite)
		//      After a setting modification, data is saved in surfaces 1, 2, 3, and so on in order, and 0 is set as the active surface.
		// # For details, see "9.2.9.2 Internal memory."

		LJV7IF_GET_STORAGE_RSP rsp;
		LJV7IF_STORAGE_INFO info;
		int nOneDataSize = sizeof(DWORD) + (DWORD)(sizeof(LJV7IF_MEASURE_DATA) * LJV7IF_OUT_COUNT);
		int nAllDataSize = min<int>(LJV7IFEX_MAXIMUM_READ_DATA_SIZE, nOneDataSize * getStorageDataDlg.getStorageStatusReq().dwDataCnt);
		vector<int> vecReceiveData(nAllDataSize / sizeof(int));

		// Send command
		int nRc = LJV7IF_GetStorageData(m_xvCurrentDeviceID, &req, &info, &rsp, (DWORD*)&vecReceiveData.at(0), nAllDataSize);
		DisplayCommandLog(nRc, IDS_SCMD_GETSTORAGEDATA);

		if(nRc == LJV7IF_RC_OK)
		{
			m_vecMeasureData.clear();

			int nDataUnitSize = (sizeof(DWORD) + sizeof(LJV7IF_MEASURE_DATA) * LJV7IF_OUT_COUNT) / sizeof(int);
			for (int i = 0; i < (int)rsp.dwDataCnt; i++)
			{
				int *pnBlock = &vecReceiveData.at(nDataUnitSize * i);
				DWORD* pdwTimeOffset = (DWORD*)pnBlock;
				MEASURE_DATA* measureData = (MEASURE_DATA*)(pnBlock + 1);
				m_vecMeasureData.push_back(*measureData);
			}
			
			// Response Data display
			ShowStrageInfo(info);
			ShowStorageDataRsp(rsp);
		}
	}
}

/*
 "Getstorageprofile" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetstorageprofile()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_GETSTORAGEPROFILE;
	CGetStorageDataDlg getStorageDataDlg;
	if(getStorageDataDlg.DoModal() == IDOK)
	{
		LJV7IF_GET_STORAGE_REQ req = getStorageDataDlg.getStorageStatusReq();
		// @Point
		// # dwReadArea is the target surface to read.
		//    The target surface to read indicates where in the internal memory usage area to read.
		// # The method to use in specifying dwReadArea varies depending on how internal memory is allocated.
		//   * Double buffer
		//      0 indicates the active surface, 1 indicates surface A, and 2 indicates surface B.
		//   * Entire area (overwrite)
		//      Fixed to 1
		//   * Entire area (do not overwrite)
		//      After a setting modification, data is saved in surfaces 1, 2, 3, and so on in order, and 0 is set as the active surface.
		// # For details, see "9.2.9.2 Internal memory."

		LJV7IF_GET_STORAGE_RSP rsp;
		LJV7IF_STORAGE_INFO storageInfo;
		LJV7IF_PROFILE_INFO profileInfo;

		int nOneDataSize
			= sizeof(unsigned int) + (unsigned int)sizeof(LJV7IF_MEASURE_DATA) * (unsigned int)LJV7IF_OUT_COUNT * 2
			+ GetOneProfileDataSize();
		int nAllDataSize = min<int>(LJV7IFEX_MAXIMUM_READ_DATA_SIZE, nOneDataSize * getStorageDataDlg.getStorageStatusReq().dwDataCnt);
		vector<int> vecReceiveData(nAllDataSize / sizeof(int));

		// Send command
		int nRc = LJV7IF_GetStorageProfile(m_xvCurrentDeviceID, &req, &storageInfo, &rsp, &profileInfo,(DWORD*)&vecReceiveData.at(0), nAllDataSize);
		DisplayCommandLog(nRc, IDS_SCMD_GETSTORAGEPROFILE);

		// @Point
		// # Terminology	
		//  * Base time … time expressed with 32 bits (<- the time when the setting was changed)
		//  * Accumulated date and time	 … counter value that indicates the elapsed time, in units of 10 ms, from the base time
		// # The accumulated date and time are stored in the accumulated data.
		// # The accumulated time of read data is calculated as shown below.
		//   Accumulated time = "base time (stBaseTime of LJV7IF_GET_STORAGE_RSP)" + "accumulated date and time × 10 ms"

		// @Point
		// # When reading multiple profiles, the specified number of profiles may not be read.
		// # To read the remaining profiles after the first set of profiles have been read,
		//   set the number to start reading profiles from (dwStartNo) and the number of profiles to read (byDataCnt) 
		//   to values that specify a range of profiles that have not been read to read the profiles in order.
		// # For the basic code, see "CCombinationFuncDlg::OnBnClickedBtnGetbatchprofiledata"

		if(nRc == LJV7IF_RC_OK)
		{
			// Clear the data
			m_vecMeasureData.clear();
			m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.clear();
			m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.clear();

			// Temporarily retain the get data.
			int nLatestMeasureDataSize  = (sizeof(DWORD) + sizeof(LJV7IF_MEASURE_DATA) * LJV7IF_OUT_COUNT) / sizeof(DWORD);
			int nProfileDataSize        = CalculateDataSize(profileInfo);
			int nProfileMeasureDataSize = (sizeof(LJV7IF_MEASURE_DATA) * LJV7IF_OUT_COUNT) / sizeof(int);
			int nDataUnitSize = nLatestMeasureDataSize + nProfileDataSize + nProfileMeasureDataSize;

			// Analize the data
			for (int i = 0; i < (int)rsp.dwDataCnt; i++)
			{
				int *pnBlock = &vecReceiveData.at(nDataUnitSize * i);
				
				DWORD* pdwTimeOffset              = (DWORD*)pnBlock;

				MEASURE_DATA* pLatestMeasureData  = (MEASURE_DATA*)(pnBlock + 1);
				LJV7IF_PROFILE_HEADER* pHeader    = (LJV7IF_PROFILE_HEADER*)(pnBlock + nLatestMeasureDataSize);
				int* pnProfileData                = pnBlock + nLatestMeasureDataSize + sizeof(LJV7IF_PROFILE_HEADER) / sizeof(DWORD);
				LJV7IF_PROFILE_FOOTER* pFooter    = (LJV7IF_PROFILE_FOOTER*)(pnProfileData + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1));
				MEASURE_DATA* pProfileMeasureData = (MEASURE_DATA*)(pnProfileData + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1) + sizeof(LJV7IF_PROFILE_FOOTER)/sizeof(DWORD));

				// Store the data
				m_vecMeasureData.push_back(*pLatestMeasureData);
				m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.push_back(PROFILE_DATA(profileInfo, pHeader, pnProfileData, pFooter));
				m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.push_back(*pProfileMeasureData);
			}
			
			// Response Data display
			ShowStrageInfo(storageInfo);
			ShowProfileInfo(profileInfo);
			ShowStorageDataRsp(rsp);
		}
	}
}

/*
 "Getstoragebatchprofile" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnGetstoragebatchprofile()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_GETSTORAGEBATCHPROFILE;
	CGetStorageBatchProfileDlg getStorageBatchProfileDlg;
	if(getStorageBatchProfileDlg.DoModal() == IDOK)
	{
		LJV7IF_GET_BATCH_PROFILE_STORAGE_REQ req = getStorageBatchProfileDlg.getBatchProfileStorageReq();
		LJV7IF_GET_BATCH_PROFILE_STORAGE_RSP rsp;
		LJV7IF_STORAGE_INFO storageInfo;
		LJV7IF_PROFILE_INFO profileInfo;

		int nOneDataSize = GetOneProfileDataSize() + (unsigned int)sizeof(LJV7IF_MEASURE_DATA) * (unsigned int)LJV7IF_OUT_COUNT ;
		int nAllDataSize = nOneDataSize * getStorageBatchProfileDlg.getBatchProfileStorageReq().byGetProfCnt;
		LJV7IF_MEASURE_DATA measureData[LJV7IF_OUT_COUNT];
		vector<int> vecReceiveData(nAllDataSize/sizeof(int));
		DWORD offsetTime = 0;

		// Send command
		int nRc = LJV7IF_GetStorageBatchProfile(m_xvCurrentDeviceID, &req, &storageInfo, &rsp, &profileInfo,(DWORD*)&vecReceiveData.at(0), nAllDataSize, &offsetTime, measureData);

		// @Point
		// # Terminology	
		//  * Base time … time expressed with 32 bits (<- the time when the setting was changed)
		//  * Accumulated date and time	 … counter value that indicates the elapsed time, in units of 10 ms, from the base time
		// # The accumulated date and time are stored in the accumulated data.
		// # The accumulated time of read data is calculated as shown below.
		//   Accumulated time = "base time (stBaseTime of LJV7IF_GET_STORAGE_RSP)" + "accumulated date and time × 10 ms"

		// @Point
		// # When reading all the profiles from a single batch, the specified number of profiles may not be read.	
		// # To read the remaining profiles after the first set of profiles have been read, 
		//   specify the batch number (dwGetBatchNo), and then set the number to start reading profiles 
		//   from (dwGetTopProfNo) and the number of profiles to read (byGetProfCnt) to values 
		//   that specify a range of profiles that have not been read to read the profiles in order.
		// # For the basic code, see "CCombinationFuncDlg::OnBnClickedBtnGetbatchprofiledata"

		DisplayCommandLog(nRc, IDS_SCMD_GETSTORAGEBATCHPROFILE);
		
		if(nRc == LJV7IF_RC_OK)
		{
			// Clear the data
			m_vecMeasureData.clear();
			m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.clear();
			m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.clear();

			m_vecMeasureData.push_back(measureData);

			int nDataUnitSize = (sizeof(LJV7IF_PROFILE_HEADER) + sizeof(LJV7IF_PROFILE_FOOTER) + sizeof(LJV7IF_MEASURE_DATA ) * LJV7IF_OUT_COUNT) / sizeof(int) + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1);
			for (int i = 0; i < (int)rsp.byGetProfCnt; i++)
			{
				int *pnBlock = &vecReceiveData.at(nDataUnitSize * i);
				
				LJV7IF_PROFILE_HEADER* pHeader    = (LJV7IF_PROFILE_HEADER*)pnBlock;
				int* pnProfileData                = pnBlock + sizeof(LJV7IF_PROFILE_HEADER) / sizeof(DWORD);
				LJV7IF_PROFILE_FOOTER* pFooter    = (LJV7IF_PROFILE_FOOTER*)(pnProfileData + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1));
				MEASURE_DATA* pProfileMeasureData = (MEASURE_DATA*)(pnProfileData + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1) + sizeof(LJV7IF_PROFILE_FOOTER)/sizeof(DWORD));
				
				// Store the data
				m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.push_back(PROFILE_DATA(profileInfo, pHeader, pnProfileData, pFooter));
				m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.push_back(*pProfileMeasureData);
			}
			ShowStrageInfo(storageInfo);
			ShowProfileInfo(profileInfo);
			ShowBatchStorageDataRsp(rsp);
		}
	}
}

/*
 "HighspeeddataUSBCommunicationInitalize" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnHighspeeddatausbcommunicationinitalize()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_HIGHSPEEDDATAUSBCOMMUNICATIONINITALIZE;
	CHighSpeedUsbInitializeDlg highSpeedUsbInitializeDlg;
	if(highSpeedUsbInitializeDlg.DoModal() == IDOK)
	{
		// Clear the data
		m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.clear();
		m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.clear();

		int nRc = LJV7IF_HighSpeedDataUsbCommunicationInitalize(m_xvCurrentDeviceID,
				( m_xvIsCountOnly ?  CountProfileReceive : ReceiveHighSpeedData ),
				highSpeedUsbInitializeDlg.m_xvProfileCnt, (UINT)m_xvCurrentDeviceID);
		// @Point
		// # When the frequency of calls is low, the callback function may not be called once per specified number of profiles.
		// # The callback function is called when both of the following conditions are met.
		//   * There is one packet of received data.
		//   * The specified number of profiles have been received by the time the call frequency has been met.
	
		DisplayCommandLog(nRc, IDS_SCMD_HIGHSPEEDDATAUSBCOMMUNICATIONINITALIZE);

		if(nRc == LJV7IF_RC_OK)
		{
			m_aDeviceData[m_xvCurrentDeviceID].m_deviceStatus = CDeviceData::DEVICESTATUS_UsbFast;
			SetDlgItemText(m_stxtConnectionStatus[m_xvCurrentDeviceID].GetDlgCtrlID(), m_aDeviceData[m_xvCurrentDeviceID].GetStatusString() );
		}
	}
}

/*
 "HighspeeddataEthernetCommunicationInitalize" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnHighspeeddataethernetcommunicationinitalize()
{
	UpdateData(TRUE);

	m_sendCommand = SENDCOMMAND_HIGHSPEEDDATAETHERNETCOMMUNICATIONINITALIZE;
	CHighSpeedEthernetInitializeDlg highSpeedEthernetInitializeDlg;
	highSpeedEthernetInitializeDlg.ethernetConfig = m_aDeviceData[m_xvCurrentDeviceID].m_ethernetConfig;
	if(highSpeedEthernetInitializeDlg.DoModal() == IDOK)
	{
		// Clear the data
		m_aDeviceData[m_xvCurrentDeviceID].m_vecMeasureData.clear();
		m_aDeviceData[m_xvCurrentDeviceID].m_vecProfileData.clear();

		LJV7IF_ETHERNET_CONFIG config = highSpeedEthernetInitializeDlg.GetEthernetConfig();

		int nRc = LJV7IF_HighSpeedDataEthernetCommunicationInitalize(m_xvCurrentDeviceID, &config,
				(WORD)highSpeedEthernetInitializeDlg.m_xvHighSpeedPort, 
				(m_xvIsCountOnly ? CountProfileReceive : ReceiveHighSpeedData),
				highSpeedEthernetInitializeDlg.m_xvProfileCnt, m_xvCurrentDeviceID);
		// @Point
		// # When the frequency of calls is low, the callback function may not be called once per specified number of profiles.
		// # The callback function is called when both of the following conditions are met.
		//   * There is one packet of received data.
		//   * The specified number of profiles have been received by the time the call frequency has been met.
	
		DisplayCommandLog(nRc, IDS_SCMD_HIGHSPEEDDATAETHERNETCOMMUNICATIONINITALIZE);
		if(nRc == LJV7IF_RC_OK)
		{
			m_aDeviceData[m_xvCurrentDeviceID].m_deviceStatus = CDeviceData::DEVICESTATUS_EthernetFast;
			m_aDeviceData[m_xvCurrentDeviceID].m_ethernetConfig = config;
			SetDlgItemText(m_stxtConnectionStatus[m_xvCurrentDeviceID].GetDlgCtrlID(), m_aDeviceData[m_xvCurrentDeviceID].GetStatusString() );
		}
	}
}

/*
 "PreStartHighSpeedDataCommunication" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnPrestarthighspeeddatacommunication()
{
	UpdateData(TRUE);
	m_sendCommand = SENDCOMMAND_PRESTARTHIGHSPEEDDATACOMMUNICATION;

	CPreStartHighSpeedDlg preStartHighSpeedDlg;
	if(preStartHighSpeedDlg.DoModal() == IDOK)
	{
		LJV7IF_HIGH_SPEED_PRE_START_REQ req = preStartHighSpeedDlg.getHighSpeedPreStartReq();
		// @Point
		// # SendPos is used to specify which profile to start sending data from during high-speed communication.
		// # When "Overwrite" is specified for the operation when memory full and 
		//   "0: From previous send complete position" is specified for the send start position,
		//    if the LJ-V continues to accumulate profiles, the LJ-V memory will become full,
		//    and the profile at the previous send complete position will be overwritten with a new profile.
		//    In this situation, because the profile at the previous send complete position is not saved, an error will occur.
		LJV7IF_PROFILE_INFO info;

		int nRc = LJV7IF_PreStartHighSpeedDataCommunication(m_xvCurrentDeviceID, &req, &info);
		DisplayCommandLog(nRc, IDS_SCMD_PRESTARTHIGHSPEEDDATACOMMUNICATION);

		if(nRc == LJV7IF_RC_OK)
		{
			m_aProfileInfo[m_xvCurrentDeviceID] = info;
			ShowProfileInfo(info);
		}
	}
}

/*
 "StartHighSpeedDataCommunication" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnStarthighspeeddatacommunication()
{
	UpdateData(TRUE);

	CThreadSafeBuffer* threadSafeBuf = CThreadSafeBuffer::getInstance();
	threadSafeBuf->ClearBuffer(m_xvCurrentDeviceID);

	m_sendCommand = SENDCOMMAND_STARTHIGHSPEEDDATACOMMUNICATION;
	int nRc = LJV7IF_StartHighSpeedDataCommunication(m_xvCurrentDeviceID);
	// @Point
	//  If the LJ-V does not measure the profile for 30 seconds from the start of transmission,
	//  "0x00000008" is stored in dwNotify of the callback function.
	DisplayCommandLog(nRc, IDS_SCMD_STARTHIGHSPEEDDATACOMMUNICATION);
}

/*
 "StopHighSpeedDataCommunication" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnStophighspeeddatacommunication()
{
	UpdateData(TRUE);

	int nRc = LJV7IF_StopHighSpeedDataCommunication(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_STOPHIGHSPEEDDATACOMMUNICATION);
}

/*
 "HighSpeedDataCommunicationFinalize" button clicked
*/
void CSingleFuncDlg::OnBnClickedBtnHighspeeddatacommunicationfinalize()
{
	UpdateData(TRUE);

	int nRc = LJV7IF_HighSpeedDataCommunicationFinalize(m_xvCurrentDeviceID);
	DisplayCommandLog(nRc, IDS_SCMD_HIGHSPEEDDATACOMMUNICATIONFINALIZE);
	LJV7IF_ETHERNET_CONFIG config;

	switch(m_aDeviceData[m_xvCurrentDeviceID].m_deviceStatus)
	{
		case CDeviceData::DEVICESTATUS_UsbFast:
			m_aDeviceData[m_xvCurrentDeviceID].m_deviceStatus = CDeviceData::DEVICESTATUS_Usb;
			break;

		case CDeviceData::DEVICESTATUS_EthernetFast:
			config = m_aDeviceData[m_xvCurrentDeviceID].m_ethernetConfig;
			m_aDeviceData[m_xvCurrentDeviceID].m_deviceStatus = CDeviceData::DEVICESTATUS_Ethernet;
			m_aDeviceData[m_xvCurrentDeviceID].m_ethernetConfig = config;
			break;

		default:
		break;
	}

	SetDlgItemText(m_stxtConnectionStatus[m_xvCurrentDeviceID].GetDlgCtrlID(), m_aDeviceData[m_xvCurrentDeviceID].GetStatusString());
}


/*
 Show the profile informaion
 @param LJV7IF_PROFILE_INFO
*/
void CSingleFuncDlg::ShowProfileInfo(LJV7IF_PROFILE_INFO profileInfo)
{
	CAtlString strLog;

	strLog.Format(_T(" [Profile Info]============================\r\n"));
	m_strCommandLog += (LPCTSTR) strLog;
	
	strLog.Format(_T("  Profile Data Num			: %d\r\n"), profileInfo.byProfileCnt);
	m_strCommandLog += (LPCTSTR) strLog;

	CString envelope = profileInfo.byEnvelope == 1 ? _T("ON") : _T("OFF");
	strLog.Format(_T("  Envelope			: %s\r\n"), envelope);
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  Profile Data Points			: %d\r\n"), profileInfo.wProfDataCnt);
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  X coordinate of the first point	: %d\r\n"), profileInfo.lXStart);
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  X-direction interval		: %d\r\n"), profileInfo.lXPitch);
	m_strCommandLog += (LPCTSTR) strLog;

	UpdateData(FALSE);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());	
}

/*
 Show the profile response
 @param LJV7IF_GET_PROFILE_RSP
*/
void CSingleFuncDlg::ShowResponse(LJV7IF_GET_PROFILE_RSP rsp)
{
	CAtlString strLog;

	strLog.Format(_T(" [Response]===============================\r\n"));
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  CurrentProfNo	: %d\r\n"), rsp.dwCurrentProfNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  OldestProfNo	: %d\r\n"), rsp.dwOldestProfNo);
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  GetTopProfNo	: %d\r\n"), rsp.dwGetTopProfNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  GetProfCnt	: %d\r\n"), rsp.byGetProfCnt);
	m_strCommandLog += (LPCTSTR) strLog;

	UpdateData(FALSE);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
}

/*
 Show the batch profile response
 @param LJV7IF_GET_BATCH_PROFILE_RSP
*/
void CSingleFuncDlg::ShowResponse(LJV7IF_GET_BATCH_PROFILE_RSP rsp)
{
	CAtlString strLog;

	strLog.Format(_T(" [Response]===============================\r\n"));
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  CurrentBatchNo		: %d\r\n"), rsp.dwCurrentBatchNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  CurrentBatchProfCnt	: %d\r\n"), rsp.dwCurrentBatchProfCnt);
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  OldestBatchNo		: %d\r\n"), rsp.dwOldestBatchNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  OldestBatchProfCnt	: %d\r\n"), rsp.dwOldestBatchProfCnt);
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  GetBatchNo		: %d\r\n"), rsp.dwGetBatchNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  GetBatchProfCnt		: %d\r\n"), rsp.dwGetBatchProfCnt);
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  GetBatchTopProfNo	: %d\r\n"), rsp.dwGetBatchTopProfNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  GetProfCnt		: %d\r\n"), rsp.byGetProfCnt);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  CurrentBatchCommited	: %d\r\n"), rsp.byCurrentBatchCommited);
	m_strCommandLog += (LPCTSTR) strLog;

	UpdateData(FALSE);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
}

/*
 Show the batch profile advance response
 @param LJV7IF_GET_BATCH_PROFILE_ADVANCE_RSP
*/
void CSingleFuncDlg::ShowResponse(LJV7IF_GET_BATCH_PROFILE_ADVANCE_RSP rsp)
{
	CAtlString strLog;

	strLog.Format(_T(" [Response]===============================\r\n"));
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  GetBatchNo		: %d\r\n"), rsp.dwGetBatchNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  GetBatchProfCnt		: %d\r\n"), rsp.dwGetBatchProfCnt);
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  GetBatchTopProfNo	: %d\r\n"), rsp.dwGetBatchTopProfNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  GetProfCnt		: %d\r\n"), rsp.byGetProfCnt);
	m_strCommandLog += (LPCTSTR) strLog;


	UpdateData(FALSE);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
}

/*
 Show the storage status informaion
 @param LJV7IF_STORAGE_INFO
*/
void CSingleFuncDlg::ShowStrageInfo(LJV7IF_STORAGE_INFO storageInfo)
{
	CAtlString strLog;
	strLog.Format(_T(" [Storage Info]============================\r\n"));
	m_strCommandLog += (LPCTSTR) strLog;

	CString status = NULL;

	switch (storageInfo.byStatus)
	{
		case STORAGE_INFO_STATUS_EMPTY:
			status = _T("EMPTY");
			break;

		case STORAGE_INFO_STATUS_STORING:
			status = _T("STORING");
			break;

		case STORAGE_INFO_STATUS_FINISHED:
			status = _T("FINISHED");
			break;

		default:
			status = _T("UNEXPECTED");
			break;
	}
	strLog.Format(_T("  Status		: %s\r\n"), status);
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  ProgramNo	: %d\r\n"), storageInfo.byProgramNo);
	m_strCommandLog += (LPCTSTR) strLog;

	CString target = NULL;
	switch (storageInfo.byTarget)
	{
	case STORAGE_INFO_TARGET_DATA:
		target = _T("DATA");
		break;

	case STORAGE_INFO_TARGET_PROFILE:
		target = _T("PROFILE");
		break;

	case STORAGE_INFO_TARGET_BATCH:
		target = _T("BATCH PROFILE");
		break;

	default:
		target = _T("UNEXPECTED");
		break;
	}
	
	strLog.Format(_T("  Target		: %s\r\n"), target);
	m_strCommandLog += (LPCTSTR) strLog;

	strLog.Format(_T("  StorageCnt	: %d\r\n"), storageInfo.dwStorageCnt);
	m_strCommandLog += (LPCTSTR) strLog;

	UpdateData(FALSE);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());	
}

/*
 Show the storage status response
 @param LJV7IF_GET_STRAGE_STATUS_RSP
*/
void CSingleFuncDlg::ShowStorageStatusRsp(LJV7IF_GET_STRAGE_STATUS_RSP rsp)
{
	CAtlString strLog;
	strLog.Format(_T(" [Response]===============================\r\n"));
	m_strCommandLog += (LPCTSTR) strLog;
	
	strLog.Format(_T("  SurfaceCnt	: %d\r\n"), rsp.dwSurfaceCnt);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  ActiveSurface	: %d\r\n"), rsp.dwActiveSurface);
	m_strCommandLog += (LPCTSTR) strLog;


	UpdateData(FALSE);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
}

/*
 Show the storage data response
 @param LJV7IF_GET_STORAGE_RSP
*/
void CSingleFuncDlg::ShowStorageDataRsp(LJV7IF_GET_STORAGE_RSP rsp)
{
	CAtlString strLog;
	strLog.Format(_T(" [Response]===============================\r\n"));
	m_strCommandLog += (LPCTSTR) strLog;
	
	strLog.Format(_T("  StartNo		: %d\r\n"), rsp.dwStartNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  DataCnt		: %d\r\n"), rsp.dwDataCnt);
	m_strCommandLog += (LPCTSTR) strLog;
	
	strLog.Format( _T("  BaseTime	: 20%d/%2d/%2d   %2d:%2d:%2d \r\n"), rsp.stBaseTime.byYear, 
		rsp.stBaseTime.byMonth, rsp.stBaseTime.byMinute, rsp.stBaseTime.byHour, rsp.stBaseTime.byMinute, rsp.stBaseTime.bySecond );
	m_strCommandLog += (LPCTSTR) strLog;

	UpdateData(FALSE);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
}

/*
 Show the batch storage data response
 @param LJV7IF_GET_BATCH_PROFILE_STORAGE_RSP
*/
void CSingleFuncDlg::ShowBatchStorageDataRsp(LJV7IF_GET_BATCH_PROFILE_STORAGE_RSP rsp)
{
	CAtlString strLog;
	strLog.Format(_T(" [Response]===============================\r\n"));
	m_strCommandLog += (LPCTSTR) strLog;
	
	strLog.Format(_T("  BatchNo		: %d\r\n"), rsp.dwGetBatchNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  BatchProfCnt	: %d\r\n"), rsp.dwGetBatchProfCnt);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  BatchTopProfNo	: %d\r\n"), rsp.dwGetBatchTopProfNo);
	m_strCommandLog += (LPCTSTR) strLog;
	strLog.Format(_T("  ProfCnt		: %d\r\n"), rsp.byGetProfCnt);
	m_strCommandLog += (LPCTSTR) strLog;
	
	strLog.Format( _T("  BaseTime	: 20%d/%2d/%2d   %d:%d:%d \r\n"), rsp.stBaseTime.byYear, 
		rsp.stBaseTime.byMonth, rsp.stBaseTime.byMinute, rsp.stBaseTime.byHour, rsp.stBaseTime.byMinute, rsp.stBaseTime.bySecond );
	m_strCommandLog += (LPCTSTR) strLog;

	UpdateData(FALSE);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
}


/*
 Display the send command and the result
 @param Return code
 @param Command ID
*/
void CSingleFuncDlg::DisplayCommandLog(int nRc, int nCommandID)
{
	CString strCommand;
	strCommand.LoadStringW(nCommandID);
	AddLogResult(nRc, strCommand);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
}

/*
 Add log and reslut
 @param Return code
 @param Command code
*/
void CSingleFuncDlg::AddLogResult(int nRc, CString strCommandCode)
{
	CString strResult;

	if(nRc == LJV7IF_RC_OK)
	{	
		strResult.LoadStringW(IDS_RESULT_OK);
		AddLog(strCommandCode, strResult, nRc);
	}
	else
	{
		strResult.LoadStringW(IDS_RESULT_NG);
		AddLog(strCommandCode, strResult, nRc);
		AddErrorLog(nRc);
	}
}

/*
 Add error log
 @param Return code
*/
void CSingleFuncDlg::AddErrorLog(int nRc)
{
	if (nRc < 0x8000)
	{
		CommonErrorLog(nRc);		// Common return code
	}
	else
	{
		IndividualErrorLog(nRc);	// Individual return code
	}
}

/*
 Commoon error log
 @param Return code
*/
void CSingleFuncDlg::CommonErrorLog(int nRc)
{
	switch (nRc)
	{
		case (int)LJV7IF_RC_OK:
			AddAdditionalErrorLog(IDS_RC_FORMAT, IDS_RC_OK);
			break;
		case (int)LJV7IF_RC_ERR_OPEN:
			AddAdditionalErrorLog(IDS_RC_FORMAT, IDS_RC_ERR_OPEN_DEVICE);
			break;
		case (int)LJV7IF_RC_ERR_NOT_OPEN:
			AddAdditionalErrorLog(IDS_RC_FORMAT, IDS_RC_ERR_NO_DEVICE);
			break;
		case (int)LJV7IF_RC_ERR_SEND:
			AddAdditionalErrorLog(IDS_RC_FORMAT, IDS_RC_ERR_SEND);
			break;
		case (int)LJV7IF_RC_ERR_RECEIVE:
			AddAdditionalErrorLog(IDS_RC_FORMAT, IDS_RC_ERR_RECEIVE);
			break;
		case (int)LJV7IF_RC_ERR_TIMEOUT:
			AddAdditionalErrorLog(IDS_RC_FORMAT, IDS_RC_ERR_TIMEOUT);
			break;
		case (int)LJV7IF_RC_ERR_PARAMETER:
			AddAdditionalErrorLog(IDS_RC_FORMAT, IDS_RC_ERR_PARAMETER);
			break;
		case (int)LJV7IF_RC_ERR_NOMEMORY:
			AddAdditionalErrorLog(IDS_RC_FORMAT, IDS_RC_ERR_NOMEMORY);
			break;
		default:
			AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
			break;
	}
}

/*
 Indivisual error Log
 @param Return code
*/
void CSingleFuncDlg::IndividualErrorLog(int nRc)
{
	switch (m_sendCommand)
	{
		case SENDCOMMAND_REBOOTCONTROLLER:
			{
				switch (nRc)
				{
					case 0x80A0:
						AddLog(IDS_RC_FORMAT, _T("Accessing the save area"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_TRIGGER:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The trigger mode is not [external trigger]"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_STARTMEASURE:
		case SENDCOMMAND_STOPMEASURE:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("Batch measurements are off"));
						break;
					case 0x80A0:
						AddLog(IDS_RC_FORMAT, _T("Batch measurement start processing could not be performed because the REMOTE terminal is off or the LASER_OFF terminal is on"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_AUTOZERO:
		case SENDCOMMAND_TIMING:
		case SENDCOMMAND_RESET:
		case SENDCOMMAND_GETMEASUREMENTVALUE:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The operation mode is [high-speed (profile only)]"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_CHANGEACTIVEPROGRAM:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The change program setting is [terminal]"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_GETPROFILE:
		case SENDCOMMAND_GETPROFILEADVANCE:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The operation mode is [advanced (with OUT measurement)]"));
						break;
					case 0x8081:
						AddLog(IDS_RC_FORMAT, _T("Batch measurements on and profile compression (time axis) off"));
						break;
					case 0x80A0:
						AddLog(IDS_RC_FORMAT, _T("No profile data"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_GETBATCHPROFILE:
		case SENDCOMMAND_GETBATCHPROFILEADVANCE:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The operation mode is [advanced (with OUT measurement)]"));
						break;
					case 0x8081:
						AddLog(IDS_RC_FORMAT, _T("Not [batch measurements on and profile compression (time axis) off]"));
						break;
					case 0x80A0:
						AddLog(IDS_RC_FORMAT, _T("No batch data (batch measurements not run even once)"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_STARTSTORAGE:
		case SENDCOMMAND_STOPSTORAGE:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The operation mode is [high-speed (profile only)]"));
						break;
					case 0x8081:
						AddLog(IDS_RC_FORMAT, _T("Storage target setting is [OFF] (no storage)"));
						break;
					case 0x8082:
						AddLog(IDS_RC_FORMAT, _T("The storage condition setting is not [terminal/command]"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_GETSTORAGESTATUS:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The operation mode is [high-speed (profile only)]"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_GETSTORAGEDATA:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The operation mode is [high-speed (profile only)]"));
						break;
					case 0x8081:
						AddLog(IDS_RC_FORMAT, _T("The storage target setting is not [OUT value]"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_GETSTORAGEPROFILE:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The operation mode is [high-speed (profile only)]"));
						break;
					case 0x8081:
						AddLog(IDS_RC_FORMAT, _T("The storage target setting is not profile, or batch measurements on and profile compression (time axis) off"));
						break;
					case 0x8082:
						AddLog(IDS_RC_FORMAT, _T("Batch measurements on and profile compression (time axis) off"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_GETSTORAGEBATCHPROFILE:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The operation mode is [high-speed (profile only)]"));
						break;
					case 0x8081:
						AddLog(IDS_RC_FORMAT, _T("The storage target setting is not profile, or batch measurements on and profile compression (time axis) off"));
						break;
					case 0x8082:
						AddLog(IDS_RC_FORMAT, _T("Not [batch measurements on and profile compression (time axis) off]"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_HIGHSPEEDDATAUSBCOMMUNICATIONINITALIZE:
		case SENDCOMMAND_HIGHSPEEDDATAETHERNETCOMMUNICATIONINITALIZE:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The operation mode is [advanced (with OUT measurement)]"));
						break;
					case 0x80A1:
						AddLog(IDS_RC_FORMAT, _T("Already performing high-speed communication error (for high-speed communication)"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		case SENDCOMMAND_PRESTARTHIGHSPEEDDATACOMMUNICATION:
		case SENDCOMMAND_STARTHIGHSPEEDDATACOMMUNICATION:
			{
				switch (nRc)
				{
					case 0x8080:
						AddLog(IDS_RC_FORMAT, _T("The operation mode is [advanced (with OUT measurement)]"));
						break;
					case 0x8081:
						AddLog(IDS_RC_FORMAT, _T("The data specified as the send start position does not exist"));
						break;
					case 0x80A0:
						AddLog(IDS_RC_FORMAT, _T("A high-speed data communication connection was not established"));
						break;
					case 0x80A1:
						AddLog(IDS_RC_FORMAT, _T("Already performing high-speed communication error (for high-speed communication)"));
						break;
					case 0x80A4:
						AddLog(IDS_RC_FORMAT, _T("The send target data was cleared"));
						break;
					default:
						AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
						break;
				}
			}
			break;

		default:
			AddLog(IDS_NOT_DEFINE_FORMAT, nRc);
			break;
	}
}


/*
 Add log
 @param Command code
 @param Result
 @param Return code
*/
void CSingleFuncDlg::AddLog(CString commandCode, CString result, int nRc)
{	
	CAtlString strLog;

	strLog.Format(_T("[%s]:%s (0x%04X)\r\n"), commandCode, result, nRc);
	m_strCommandLog += (LPCTSTR) strLog;

	UpdateData(FALSE);
}

/*
 Add Log
 @param Display format
 @param Error message
*/
void CSingleFuncDlg::AddLog(int nDisplayFormat, CString strErrorMessage)
{	
	CString strDisplayFormat;
	strDisplayFormat.LoadStringW(nDisplayFormat);

	CAtlString strLog;
	strLog.Format(strDisplayFormat, strErrorMessage);
	m_strCommandLog += (LPCTSTR) strLog;

	UpdateData(FALSE);
}

/*
 Add Log
 @param Display format
 @param Return Code
*/
void CSingleFuncDlg::AddLog(int nDisplayFormat, int nRc)
{
	CString strDisplayFormat;
	strDisplayFormat.LoadStringW(nDisplayFormat);

	CAtlString strLog;
	strLog.Format(strDisplayFormat, nRc);
	m_strCommandLog += (LPCTSTR) strLog;

	UpdateData(FALSE);
}

/*
 Add Adddirional Error Log
 @param Display format
 @param Error log
*/
void CSingleFuncDlg::AddAdditionalErrorLog(int nDisplayFormat, int nErrorLog)
{	
	CString strDisplayFormat;
	strDisplayFormat.LoadStringW(nDisplayFormat);

	CString strErrorLog;
	strErrorLog.LoadStringW(nErrorLog);

	CAtlString strLog;
	strLog.Format(strDisplayFormat, strErrorLog);
	m_strCommandLog += (LPCTSTR) strLog;

	UpdateData(FALSE);
}

/*
 Add Result
 @param Display format
 @param Result for display
*/
void CSingleFuncDlg::AddResult(LPCTSTR format, int nNum)
{	
	CString strLog;
	strLog.Format(format, nNum);
	m_strCommandLog += (LPCTSTR) strLog;
	
	UpdateData(FALSE);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
}

/*
 Get MeasureData information
 @param LJV7IF_MEASURE_DATA
 @return Measure data information
*/
CString CSingleFuncDlg::GetMeasureDataInfo(LJV7IF_MEASURE_DATA measureData)
{
	CString strMeasureDataInfo;

	switch (measureData.byDataInfo)
	{
		case LJV7IF_MEASURE_DATA_INFO_VALID:
			strMeasureDataInfo = _T("Valid	");
			break;
		case LJV7IF_MEASURE_DATA_INFO_ALARM:
			strMeasureDataInfo = _T("Alarm value  ");
			break;
		case LJV7IF_MEASURE_DATA_INFO_WAIT:
			strMeasureDataInfo = _T("Judgment wait value  ");
			break;
		default:
			strMeasureDataInfo = _T("Unexpected value	");
			break;
	}

	switch (measureData.byJudge)
	{
		case 0:
			strMeasureDataInfo += _T("______  ");
			break;
		case LJV7IF_JUDGE_RESULT_HI:
			strMeasureDataInfo += _T("HI____  ");
			break;
		case LJV7IF_JUDGE_RESULT_GO:
			strMeasureDataInfo += _T("__GO__  ");
			break;
		case LJV7IF_JUDGE_RESULT_LO:
			strMeasureDataInfo += _T("____LO  ");
			break;
		case (LJV7IF_JUDGE_RESULT_HI | LJV7IF_JUDGE_RESULT_LO):
			strMeasureDataInfo += _T("HI__LO  ");
			break;
		case (int)(LJV7IF_JUDGE_RESULT_HI
				| LJV7IF_JUDGE_RESULT_GO
				| LJV7IF_JUDGE_RESULT_LO):
			strMeasureDataInfo += _T("ALL BIT  ");
			break;
		default:
			strMeasureDataInfo += _T("UNEXPECTED ");
			break;
	}

	return strMeasureDataInfo;
}

/*
 Timer Control
*/
void CSingleFuncDlg::OnBnClickedChkTimer()
{
	UpdateData(TRUE);
	UINT nTimerELASP = m_xcSpinStartTimer.GetPos32();

	// Start Timer or Stop Timer
	if(m_xvIsStartTimer)
	{
		SetTimer(TIMER_ID, nTimerELASP, NULL);
	}
	else
	{
		KillTimer(TIMER_ID);
	}

	// It prevents a callback function from being available
	CButton *button=(CButton*)GetDlgItem(IDC_CHK_COUNTONLY);
	if(m_xvIsStartTimer)
	{
		button->EnableWindow(FALSE);
	}
	else
	{
		button->EnableWindow(TRUE);
	}
}

/*
 Show the message
 @param Message
*/
void CSingleFuncDlg::ShowMessage(CString message)
{
	CAtlString strLog;
	strLog.Format( message +_T("\r\n"));
	m_strCommandLog += (LPCTSTR) strLog;
	UpdateData(FALSE);
	m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
}

/*
 Get one profile size for current specified state
 @return one profile data size(byte)
*/
int CSingleFuncDlg::GetOneProfileDataSize()
{
	// Get the Profile Count
	int nProfileCnt = 0;
	CString strXRange;
	m_xcXRange.GetLBText(m_xvXRange, strXRange);
	if(strXRange == _T("FULL"))
	{
		nProfileCnt = 800;
	}
	else if(strXRange == _T("MIDDLE"))
	{
		nProfileCnt = 600;
	}
	else if(strXRange == _T("SMALL"))
	{
		nProfileCnt = 200;
	}

	// Get the compressibility
	int nCompression = 0;
	CString strCompresion;
	m_xcXCompression.GetLBText(m_xvXCompression, strCompresion);
	if(strCompresion == _T("OFF"))
	{
		nCompression = 1;
	}
	else if(strCompresion == _T("2"))
	{
		nCompression = 2;
	}
	else if(strCompresion == _T("4"))
	{
		nCompression = 4;
	}

	UINT retBuffSize = 0;											// Buffer size (in units of bytes)
	int basicSize = nProfileCnt / (m_xvBinning + 1);				// Basic size
	basicSize /= nCompression;

	retBuffSize += (UINT)basicSize * (m_xvHeadNum == 0 ? 1U : 2U);	// Number of headers
	retBuffSize *= (m_xvIsEnvelope ? 2U : 1U);						// Envelope setting
	retBuffSize *= sizeof(UINT);									//in units of bytes

	retBuffSize += sizeof(LJV7IF_PROFILE_HEADER);					// Sizes of the header and footer structures
	retBuffSize += sizeof(LJV7IF_PROFILE_FOOTER);

	return retBuffSize;
}

/*
 Calcultate the data size
 @param LJV7IF_PROFILE_INFO
 @return Data size(byte)
*/
int CSingleFuncDlg::CalculateDataSize(const LJV7IF_PROFILE_INFO &profileInfo)
{
	return profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1) +
		(sizeof(LJV7IF_PROFILE_HEADER) + sizeof(LJV7IF_PROFILE_FOOTER)) / sizeof(int);
}

/*
 callack function(receive profile data)
 @param Pointer for profile data
 @param One profile data size
 @param Profile count
 @param notify
 @param UserID
*/
void CSingleFuncDlg::ReceiveHighSpeedData(BYTE* pBuffer, DWORD dwSize, DWORD dwCount, DWORD dwNotify, DWORD dwUser)
{
	// @Point
	// Take care to only implement storing profile data in a thread save buffer in the callback function.
	// As the thread used to call the callback function is the same as the thread used to receive data,
	// the processing time of the callback function affects the speed at which data is received,
	// and may stop communication from being performed properly in some environments.

	vector<PROFILE_DATA> vecProfileData;

	int nProfDataCnt = (dwSize - sizeof(LJV7IF_PROFILE_HEADER) - sizeof(LJV7IF_PROFILE_FOOTER)) / sizeof(DWORD);
	
	for(DWORD i = 0; i < dwCount; i++)
	{
		BYTE *pbyBlock = pBuffer + dwSize * i;
	
		LJV7IF_PROFILE_HEADER* pHeader    = (LJV7IF_PROFILE_HEADER*)pbyBlock;
		int* pnProfileData                = (int*)(pbyBlock + sizeof(LJV7IF_PROFILE_HEADER));
		LJV7IF_PROFILE_FOOTER* pFooter    = (LJV7IF_PROFILE_FOOTER*)(pbyBlock + dwSize - sizeof(LJV7IF_PROFILE_FOOTER));
	
		vecProfileData.push_back(PROFILE_DATA(m_aProfileInfo[dwUser], pHeader, pnProfileData, pFooter));
	}
	CThreadSafeBuffer* threadSafeBuf = CThreadSafeBuffer::getInstance();
	threadSafeBuf->Add(dwUser, vecProfileData, dwNotify);	
}

/*
 callack function(count only)
 @param Pointer for profile data
 @param One profile data size
 @param Profile count
 @param notify
 @param UserID
*/
void CSingleFuncDlg::CountProfileReceive(BYTE* pBuffer, DWORD dwSize, DWORD dwCount, DWORD dwNotify, DWORD dwUser)
{
	// @Point
	// Take care to only implement storing profile data in a thread save buffer in the callback function.
	// As the thread used to call the callback function is the same as the thread used to receive data,
	// the processing time of the callback function affects the speed at which data is received,
	// and may stop communication from being performed properly in some environments.

	CThreadSafeBuffer* threadSafeBuf = CThreadSafeBuffer::getInstance();
	threadSafeBuf -> AddCount(dwUser, dwCount, dwNotify);
}

/*
 Timer function
 @param Event ID
*/
void CSingleFuncDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == TIMER_ID)
	{
		int nBatchNo    = 0;
		DWORD dwNotify  = 0;
		
		for(int i = 0; i < LJV7IF_DEVICE_COUNT; i++)
		{
			if(m_xvIsCountOnly)
			{
				CThreadSafeBuffer* threadSafeBuf = CThreadSafeBuffer::getInstance();
				m_anProfReceiveCnt[i] = threadSafeBuf->GetCount(i, &dwNotify, &nBatchNo);
			}
			else
			{
				CThreadSafeBuffer* threadSafeBuf = CThreadSafeBuffer::getInstance();
				vector<PROFILE_DATA> vecProfileData;
				threadSafeBuf->Get(i, &dwNotify, &nBatchNo, vecProfileData);

				if(vecProfileData.size() == 0) continue;

				for(unsigned int j = 0; j < vecProfileData.size(); j++)
				{		
					if(m_aDeviceData[i].m_vecProfileData.size() < WRITE_DATA_SIZE)
					{
						m_aDeviceData[i].m_vecProfileData.push_back(vecProfileData.at(j));
					}
					m_anProfReceiveCnt[i]++;
				}
			}

			if(dwNotify != 0)
			{
				CString strLog;
				strLog.Format( _T("notify[%d] = 0x%08X \t batch[%d] \r\n"), i, dwNotify, nBatchNo);
				m_strCommandLog += (LPCTSTR) strLog;
				UpdateData(FALSE);
				m_editCommandLog.LineScroll(m_editCommandLog.GetLineCount());
			}
		}
		UpDateReceiveCnt();
	}

	CDialog::OnTimer(nIDEvent);
}

/*
 Update the number of the profiles
*/
void CSingleFuncDlg::UpDateReceiveCnt()
{
	CString strLog[LJV7IF_DEVICE_COUNT];

	for(int i = 0; i < LJV7IF_DEVICE_COUNT; i++)
	{
		strLog[i].Format(_T("%d"), m_anProfReceiveCnt[i]);
	}

	this->SetDlgItemTextW(IDC_STATIC_RECEIVE0, (LPCTSTR)strLog[0]);
	this->SetDlgItemTextW(IDC_STATIC_RECEIVE1, (LPCTSTR)strLog[1]);
	this->SetDlgItemTextW(IDC_STATIC_RECEIVE2, (LPCTSTR)strLog[2]);
	this->SetDlgItemTextW(IDC_STATIC_RECEIVE3, (LPCTSTR)strLog[3]);
	this->SetDlgItemTextW(IDC_STATIC_RECEIVE4, (LPCTSTR)strLog[4]);
	this->SetDlgItemTextW(IDC_STATIC_RECEIVE5, (LPCTSTR)strLog[5]);
}
