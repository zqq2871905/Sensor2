#pragma once
#ifndef CONNECTDEVICE_H
#define CONNECTDEVICE_H

#include "ui_ConnectDevice.h"
#include "sensor.h"

class ConnectDevice :
	public QWidget
{
	Q_OBJECT

public:
	ConnectDevice(QWidget *parent = 0);
	~ConnectDevice();
	Ui::ConnectDevice ui;
	LJV7IF_ETHERNET_CONFIG GetEthernetConfig();
	int DEVICE_ID[2] = {0,1};
	BOOL CheckReturnCode(int nRc);
	int DeviceCount = 0;
	int DeviceNum = 0;

private:
	//Ui::ConnectDevice ui;
	//CIPAddressCtrl m_xcIPAddress;
	DWORD m_dwIPAddress;
	int m_xvPortNum;
	int ControllerPortNum;
	int OneControllerPortNum;
	int TwoControllerPortNum;
//	int ControllerFastPortNum;
	int OneControllerFastPortNum;
	int TwoControllerFastPortNum;
	//BYTE ControllerIP[4];
	//BYTE OneControllerIP[4];
	//BYTE TwoControllerIP[4];
	QString controllerIP;
	QString OnecontrollerIP;
	QString TwocontrollerIP;


private slots:
	void OnBnClickedOk1();
	void OnBnClickedOk2();
};


#endif // SENSOR_H

