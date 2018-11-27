
#include "ConnectDevice.h"

#include "qmessagebox.h"

ConnectDevice::ConnectDevice(QWidget *parent)
	: QWidget(parent)
	, m_xvPortNum(24691)
{
		ui.setupUi(this);
}


ConnectDevice::~ConnectDevice()
{
}

BOOL ConnectDevice::CheckReturnCode(int nRc)
{
	if (nRc == LJV7IF_RC_OK) return true;

	QString strMessage;
	strMessage = QString("Error : 0x%1").arg(nRc, 8, 16, QLatin1Char('0'));
	QMessageBox::warning(this, "Attention", strMessage);

	return false;
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
	//Sensor IPConnect;
	//将输入的IP地址给控制器
	//ControllerIP[4] = BYTE((ui.lineEdit->text() + ui.lineEdit_2->text() + ui.lineEdit_3->text() + ui.lineEdit_4->text()).toLatin1().data());
	
	DeviceCount = 1;//连接1台设备

	LJV7IF_ETHERNET_CONFIG config0;
	config0.abyIpAddress[0] = unsigned char(10);
	config0.abyIpAddress[1] = unsigned char(111);
	config0.abyIpAddress[2] = unsigned char(228);
	config0.abyIpAddress[3] = unsigned char(194);//DEVICE_ID[0]
	config0.wPortNo = 24691;
	config0.reserve[0] = 0;
	config0.reserve[1] = 0;
	LJV7IF_ETHERNET_CONFIG config1;
	config1.abyIpAddress[0] = unsigned char(10);
	config1.abyIpAddress[1] = unsigned char(111);
	config1.abyIpAddress[2] = unsigned char(228);
	config1.abyIpAddress[3] = unsigned char(197);//DEVICE_ID[1]
	config1.wPortNo = 24691;
	config1.reserve[0] = 0;
	config1.reserve[1] = 0;

	bool ok;
	ControllerPortNum = (ui.lineEdit_21->text()).toInt(&ok, 10);
	controllerIP = ui.lineEdit->text() + ui.lineEdit_2->text() + ui.lineEdit_3->text();
	if ((controllerIP == "10111228") && ((ui.lineEdit_4->text() == "194") || (ui.lineEdit_4->text() == "197")) && (ControllerPortNum == 24691))
	{
		int nRc = LJV7IF_RC_OK;
		nRc = LJV7IF_Initialize();
		if (!CheckReturnCode(nRc)) return;

		if (ui.lineEdit_4->text() == "194")
		{
			nRc = LJV7IF_EthernetOpen(DEVICE_ID[0], &config0);
			DeviceNum = 0;//标志，194连接
		}
		else if(ui.lineEdit_4->text() == "197")
		{
			nRc = LJV7IF_EthernetOpen(DEVICE_ID[1], &config1);
			DeviceNum = 1;//标志，197连接
		}
		QMessageBox::warning(this, "Attention", "Connect Successed!");
	}
	else
		QMessageBox::warning(this, "Attention", "Connect Failed!");
	close();
}

void ConnectDevice::OnBnClickedOk2()
{
	//将输入的两个IP地址给两个控制器
	//OneControllerIP[4] = BYTE((ui.lineEdit_9->text() + ui.lineEdit_10->text() + ui.lineEdit_11->text() + ui.lineEdit_12->text()).toLatin1().data());
	//TwoControllerIP[4] = BYTE((ui.lineEdit_13->text() + ui.lineEdit_14->text() + ui.lineEdit_15->text() + ui.lineEdit_16->text()).toLatin1().data());
	
	DeviceCount = 2;//连接2台设备
	
	LJV7IF_ETHERNET_CONFIG config0;
	config0.abyIpAddress[0] = unsigned char(10);
	config0.abyIpAddress[1] = unsigned char(111);
	config0.abyIpAddress[2] = unsigned char(228);
	config0.abyIpAddress[3] = unsigned char(194);//DEVICE_ID[0]
	config0.wPortNo = 24691;
	config0.reserve[0] = 0;
	config0.reserve[1] = 0;
	LJV7IF_ETHERNET_CONFIG config1;
	config1.abyIpAddress[0] = unsigned char(10);
	config1.abyIpAddress[1] = unsigned char(111);
	config1.abyIpAddress[2] = unsigned char(228);
	config1.abyIpAddress[3] = unsigned char(197);//DEVICE_ID[1]
	config1.wPortNo = 24691;
	config1.reserve[0] = 0;
	config1.reserve[1] = 0;
	
	bool ok;
	OneControllerPortNum = (ui.lineEdit_17->text()).toInt(&ok, 10);
	TwoControllerPortNum = (ui.lineEdit_18->text()).toInt(&ok, 10);
	OneControllerFastPortNum = (ui.lineEdit_19->text()).toInt(&ok, 10);
	TwoControllerFastPortNum = (ui.lineEdit_20->text()).toInt(&ok, 10);
	OnecontrollerIP = ui.lineEdit_9->text() + ui.lineEdit_10->text() + ui.lineEdit_11->text();
	TwocontrollerIP = ui.lineEdit_13->text() + ui.lineEdit_14->text() + ui.lineEdit_15->text();
	if ((OnecontrollerIP == TwocontrollerIP) && (TwocontrollerIP == "10111228") && (((ui.lineEdit_12->text() == "194") && (ui.lineEdit_16->text() == "197")) || ((ui.lineEdit_12->text() == "197") && (ui.lineEdit_16->text() == "194"))) && (OneControllerPortNum == TwoControllerPortNum) && (OneControllerPortNum == 24691) && (OneControllerFastPortNum == 24692) && (TwoControllerFastPortNum == 24692))
	{
		LJV7IF_Initialize();
		LJV7IF_EthernetOpen(DEVICE_ID[0], &config0);
		LJV7IF_EthernetOpen(DEVICE_ID[1], &config1);
		QMessageBox::warning(this, "Attention", "Two Devices Connect Successed!");
	}else
		QMessageBox::warning(this, "Attention", "Two Devices Connect Failed!");
	close();
}