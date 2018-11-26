#pragma once
#ifndef CONNECTDEVICE_H
#define CONNECTDEVICE_H

#include <QtWidgets/QWidget>
#include "ui_ConnectDevice.h"
#include "sensor.h"
#include "LJV7_IF.h"

class ConnectDevice :
	public QWidget
{
	Q_OBJECT

public:
	ConnectDevice(QWidget *parent = 0);
	~ConnectDevice();
	Ui::ConnectDevice ui;
	LJV7IF_ETHERNET_CONFIG GetEthernetConfig();

private:
	//Ui::ConnectDevice ui;
	//CIPAddressCtrl m_xcIPAddress;
	DWORD m_dwIPAddress;
	int m_xvPortNum;

private slots:
	void OnBnClickedOk1();
	void OnBnClickedOk2();
};


#endif // SENSOR_H

