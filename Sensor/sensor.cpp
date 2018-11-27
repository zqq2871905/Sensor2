#include "sensor.h"
#include "qmessagebox.h"
#include "ConnectDevice.h"
#include "DeviceData.h"
#include "Define.h"
#include "ThreadSafeBuffer.h"

LJV7IF_PROFILE_INFO Sensor::m_profileInfo;

Sensor::Sensor(QWidget *parent)
	: QWidget(parent)
{
	n_isopen = false;
	
	//n_isOpenDevice = false;
	ui.setupUi(this);

}

Sensor::~Sensor()
{

}

void Sensor::OnClose()
{
	ConnectDevice condev0;
	int nRc = LJV7IF_RC_OK;
	// Close the communication
	nRc = LJV7IF_CommClose(condev0.DEVICE_ID[0]);
	nRc = LJV7IF_CommClose(condev0.DEVICE_ID[1]);
	if (!condev0.CheckReturnCode(nRc)) return;
	// Finalize the DLL
	nRc = LJV7IF_Finalize();
	if (!condev0.CheckReturnCode(nRc)) return;
	QMessageBox::warning(this, "Attention", "Connect Stopped!");

}

void Sensor::OnOpen()
{
	n_isopen = true;
	if (n_isopen)
	{
		ConnectDevice *condev = new ConnectDevice();
		condev->show();

		// Open the communication path
//		if (m_xvConnection == 0)			// USB
//		{
//			nRc = LJV7IF_UsbOpen(DEVICE_ID);
//		}
//		else if (m_xvConnection == 1)	// Ethernet
//		{
//			// Generate the settings for Ethernet communication.
//
////			LJV7IF_ETHERNET_CONFIG config;//控制器配置		
//////			config.abyIpAddress[4] = LocalHostIP[4];//将本机IP地址给控制器IP
//////			config.wPortNo = m_xvCommandPort;
////			config.reserve[0] = 0;
////			config.reserve[1] = 0;
////
////			nRc = LJV7IF_EthernetOpen(DEVICE_ID, &config);
//		}
//
//		if (!CheckReturnCode(nRc)) return;

	}
}

/*
Obtain Target parameter information
@param LJV7IF_TARGET_SETTING
@return Data size
*/
DWORD Sensor::ObtainTargetParamInfo(LJV7IF_TARGET_SETTING& target)
{
	//Set target as "all parameter"
	target.byCategory = 0xFF;
	target.byItem = 0x00;
	target.byTarget1 = 0;
	target.byTarget2 = 0;
	target.byTarget3 = 0;
	target.byTarget4 = 0;

	if (m_xvSelectedProgram == 0)				//Environment Settings
	{
		target.byType = LJV7IFEX_CONFIG_TYPE_SYS;
		return LJV7IFEX_CONFIG_SIZE_SYSTEM;
	}
	else if (m_xvSelectedProgram == 1)			//Common Settings
	{
		target.byType = LJV7IFEX_CONFIG_TYPE_MEASURE_COMMON;
		return LJV7IFEX_CONFIG_SIZE_COMMON;
	}
	else if (m_xvSelectedProgram < 18)			//Program No.0~16
	{
		target.byType = LJV7IFEX_CONFIG_TYPE_PROGRAM_00 + (m_xvSelectedProgram - 2);
		return LJV7IFEX_CONFIG_SIZE_PROGRAM;
	}

	return 0;
}

/*
callack function(receive profile data)
@param Pointer for profile data
@param One profile data size
@param Profile count
@param notify
@param UserID
*/
void Sensor::ReceiveHighSpeedData(BYTE* pBuffer, DWORD dwSize, DWORD dwCount, DWORD dwNotify, DWORD dwUser)
{
	vector<PROFILE_DATA> vecProfileData;

	int nProfDataCnt = (dwSize - sizeof(LJV7IF_PROFILE_HEADER) - sizeof(LJV7IF_PROFILE_FOOTER)) / sizeof(DWORD);

	for (DWORD i = 0; i < dwCount; i++)
	{
		BYTE *pbyBlock = pBuffer + dwSize * i;

		LJV7IF_PROFILE_HEADER* pHeader = (LJV7IF_PROFILE_HEADER*)pbyBlock;
		int* pnProfileData = (int*)(pbyBlock + sizeof(LJV7IF_PROFILE_HEADER));
		LJV7IF_PROFILE_FOOTER* pFooter = (LJV7IF_PROFILE_FOOTER*)(pbyBlock + dwSize - sizeof(LJV7IF_PROFILE_FOOTER));

		vecProfileData.push_back(PROFILE_DATA(m_profileInfo, pHeader, pnProfileData, pFooter));
	}
	ThreadSafeBuffer* threadSafeBuf = ThreadSafeBuffer::getInstance();

	threadSafeBuf->Add(dwUser, vecProfileData, dwNotify);
}

/*
"Get current values" button clicked
*/
void Sensor::OnGetcurrentvalue()
{
	LJV7IF_MEASURE_DATA aMeasureData[LJV7IF_OUT_COUNT];
	ConnectDevice devcon;
	//Get measure data

	//只连接1台设备
	if (devcon.DeviceCount == 1)
	{
		if (devcon.DeviceNum == 0)
		{
			LJV7IF_GetMeasurementValue(devcon.DEVICE_ID[0], aMeasureData);
			//Show measure data
			QString strLog;
			for (int i = 0; i < LJV7IF_OUT_COUNT; i++)
			{
				strLog = QString("OUT%2d:\t %04f\r\n").arg(i + 1).arg(aMeasureData[i].fValue);
				m_xvResult += strLog;
				ui.textEdit->setText(m_xvResult);
			}
		}
		else if (devcon.DeviceNum == 1)
		{
			LJV7IF_GetMeasurementValue(devcon.DEVICE_ID[1], aMeasureData);
			//Show measure data
			QString strLog2;
			for (int j = 0; j < LJV7IF_OUT_COUNT; j++)
			{
				strLog2 = QString("OUT%2d:\t %04f\r\n").arg(j + 1).arg(aMeasureData[j].fValue);
				m_xvResult2 += strLog2;
				ui.textEdit_2->setText(m_xvResult2);
			}
		}
	}
	//连接2台设备
	else if (devcon.DeviceCount == 2)
	{
		LJV7IF_GetMeasurementValue(devcon.DEVICE_ID[0], aMeasureData);
		LJV7IF_GetMeasurementValue(devcon.DEVICE_ID[1], aMeasureData);

		QString strLog;
		for (int i = 0; i < LJV7IF_OUT_COUNT; i++)
		{
			strLog = QString("OUT%2d:\t %04f\r\n").arg(i + 1).arg(aMeasureData[i].fValue);
			m_xvResult += strLog;
			ui.textEdit->setText(m_xvResult);
		}

		QString strLog2;
		for (int j = 0; j < LJV7IF_OUT_COUNT; j++)
		{
			strLog2 = QString("OUT%2d:\t %04f\r\n").arg(j + 1).arg(aMeasureData[j].fValue);
			m_xvResult2 += strLog2;
			ui.textEdit_2->setText(m_xvResult2);
		}
	}
}

/*
"Get high-speed mode profiles" button clicked
*/
void Sensor::OnGetprofiledata()
{
	ConnectDevice devcon1;

	LJV7IF_GET_PROFILE_REQ req;
	req.byTargetBank = (BYTE)PROFILEBANK_ACTIVE;
	req.byPosMode = (BYTE)PROFILEPOS_CURRENT;
	req.dwGetProfNo = 0;
	req.byGetProfCnt = 10;
	req.byErase = 0;

	LJV7IF_GET_PROFILE_REQ req1;
	req1.byTargetBank = (BYTE)PROFILEBANK_ACTIVE;
	req1.byPosMode = (BYTE)PROFILEPOS_CURRENT;
	req1.dwGetProfNo = 0;
	req1.byGetProfCnt = 10;
	req1.byErase = 0;

	LJV7IF_GET_PROFILE_RSP rsp;
	LJV7IF_PROFILE_INFO profileInfo;
	DWORD dwOneDataSize = MAX_PROFILE_COUNT + (sizeof(LJV7IF_PROFILE_HEADER) + sizeof(LJV7IF_PROFILE_FOOTER)) / sizeof(DWORD);
	DWORD dwAllDataSize = dwOneDataSize * req.byGetProfCnt;
	std::vector<int> vecProfileData(dwAllDataSize);

	LJV7IF_GET_PROFILE_RSP rsp1;
	LJV7IF_PROFILE_INFO profileInfo1;
	DWORD dwOneDataSize = MAX_PROFILE_COUNT + (sizeof(LJV7IF_PROFILE_HEADER) + sizeof(LJV7IF_PROFILE_FOOTER)) / sizeof(DWORD);
	DWORD dwAllDataSize1 = dwOneDataSize * req1.byGetProfCnt;
	std::vector<int> vecProfileData1(dwAllDataSize1);

	// Send Command
	if (devcon1.DeviceCount == 1)
	{
		if (devcon1.DeviceNum == 0)
		{
			int nRc_GP = LJV7IF_GetProfile(devcon1.DEVICE_ID[0], &req, &rsp, &profileInfo, (DWORD*)&vecProfileData.at(0), dwAllDataSize * sizeof(int));
			if (!devcon1.CheckReturnCode(nRc_GP)) return;
			
			if (nRc_GP == LJV7IF_RC_OK)
			{
				// Analize the data
				int nDataUnitSize = (sizeof(LJV7IF_PROFILE_HEADER) + sizeof(int) * profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1) + sizeof(LJV7IF_PROFILE_FOOTER)) / sizeof(int);
				std::vector<PROFILE_DATA> vecReceiveProfileData;
				for (int i = 0; i < rsp.byGetProfCnt; i++)
				{
					int *pnBlock = &vecProfileData.at(nDataUnitSize * i);

					LJV7IF_PROFILE_HEADER* pHeader = (LJV7IF_PROFILE_HEADER*)pnBlock;
					int* pnProfileData = pnBlock + (sizeof(LJV7IF_PROFILE_HEADER) / sizeof(DWORD));
					LJV7IF_PROFILE_FOOTER* pFooter = (LJV7IF_PROFILE_FOOTER*)(pnProfileData + profileInfo.wProfDataCnt * profileInfo.byProfileCnt * (profileInfo.byEnvelope + 1));

					// Store the profile data
					vecReceiveProfileData.push_back(PROFILE_DATA(profileInfo, pHeader, pnProfileData, pFooter));
				}
				// Profile output
				CDataExport::ExportProfileEx(&(vecReceiveProfileData.at(0)), m_xvDataFilePath, rsp.byGetProfCnt);
			}
		}
		else if (devcon1.DeviceNum == 1)
		{
			int nRc_GP = LJV7IF_GetProfile(devcon1.DEVICE_ID[1], &req1, &rsp1, &profileInfo1, (DWORD*)&vecProfileData1.at(0), dwAllDataSize1 * sizeof(int));
			if (!devcon1.CheckReturnCode(nRc_GP)) return;
		}

	}

	


	
}





			
