#include "sensor.h"
#include <QMessageBox>

const int DEVICE_ID = 0;

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

BOOL Sensor::CheckReturnCode(int nRc)
{
	if (nRc == LJV7IF_RC_OK) return true;

	QString strMessage;
	strMessage = QString("Error : 0x%1").arg(nRc, 8, 16, QLatin1Char('0'));
	QMessageBox::warning(this,"Attention", strMessage);

	return false;
}

void Sensor::OnInitial()
{
	LJV7IF_Initialize();
}

//void Sensor::OnFinal()
//{
//	LJV7IF_Finalize();
//}
void Sensor::OnClose()
{

}

void Sensor::OnOpen()
{
	n_isopen = true;
	if (n_isopen)
	{
		ConnectDevice *condev = new ConnectDevice();
		condev->show();

		int nRc = LJV7IF_RC_OK;

		// Initialize the Dll
		nRc = LJV7IF_Initialize();
		if (!CheckReturnCode(nRc)) return;

		// Open the communication path
		if (m_xvConnection == 0)			// USB
		{
			nRc = LJV7IF_UsbOpen(DEVICE_ID);
		}
		else if (m_xvConnection == 1)	// Ethernet
		{
			// Generate the settings for Ethernet communication.
			LJV7IF_ETHERNET_CONFIG config;
			m_xcIPAddressComb.GetAddress(config.abyIpAddress[0], config.abyIpAddress[1], config.abyIpAddress[2], config.abyIpAddress[3]);
			config.wPortNo = m_xvCommandPort;
			config.reserve[0] = 0;
			config.reserve[1] = 0;

			nRc = LJV7IF_EthernetOpen(DEVICE_ID, &config);
		}

		if (!CheckReturnCode(nRc)) return;

		//LJV7IF_ETHERNET_CONFIG ethernetConfig = condev->GetEthernetConfig();
		//int nRc = LJV7IF_EthernetOpen(m_xvCurrentDeviceID, &ethernetConfig);

		//DeviceData::DEVICESTATUS status = (nRc == LJV7IF_RC_OK) ? DeviceData::DEVICESTATUS_Ethernet : DeviceData::DEVICESTATUS_NoConnection;
		//m_aDeviceData[m_xvCurrentDeviceID].m_deviceStatus = status;
		//m_aDeviceData[m_xvCurrentDeviceID].m_ethernetConfig = ethernetConfig;

	}

	}



			
