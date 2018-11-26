#ifndef SENSOR_H
#define SENSOR_H

#include <Windows.h>
#include "LJV7_IF.h"
#include "LJV7_ErrorCode.h"
#include <QtWidgets/QWidget>
#include "ui_sensor.h"
#include "ConnectDevice.h"
#include "DeviceData.h"


class Sensor : public QWidget
{
	Q_OBJECT

public:
	Sensor(QWidget *parent = 0);
	~Sensor();
	void OnInitial();
	//void OnFinal();
	bool n_isOpenDevice;

private:
	Ui::SensorClass ui;
	bool n_isopen;
	int m_xvCurrentDeviceID;
	int m_xvConnection;
	DeviceData m_aDeviceData[LJV7IF_DEVICE_COUNT];
	BOOL CheckReturnCode(int nRc);


private slots:
	void OnOpen();
	void OnClose();


};

#endif // SENSOR_H
