#include "ConnectDevice.h"


ConnectDevice::ConnectDevice(QWidget *parent)
	: QWidget(parent)
	, m_xvPortNum(24691)
{
		ui.setupUi(this);
}


ConnectDevice::~ConnectDevice()
{
}

LJV7IF_ETHERNET_CONFIG ConnectDevice::GetEthernetConfig()
{
	LJV7IF_ETHERNET_CONFIG ethernetConfig;
	DWORD ipAddress = m_dwIPAddress;

	ethernetConfig.abyIpAddress[0] = (BYTE)((ipAddress & 0xFF000000) >> 24);
	ethernetConfig.abyIpAddress[1] = (BYTE)((ipAddress & 0x00FF0000) >> 16);
	ethernetConfig.abyIpAddress[2] = (BYTE)((ipAddress & 0x0000FF00) >> 8);
	ethernetConfig.abyIpAddress[3] = (BYTE)(ipAddress & 0x000000FF);
	ethernetConfig.wPortNo = (WORD)m_xvPortNum;
	ethernetConfig.reserve[0] = (BYTE)0;
	ethernetConfig.reserve[1] = (BYTE)0;

	return ethernetConfig;
}

void ConnectDevice::OnBnClickedOk1()
{
	
}

void ConnectDevice::OnBnClickedOk2()
{

}