#pragma once
#ifndef SENSOR_H
#define SENSOR_H

#include <Windows.h>
#include "LJV7_IF.h"
#include "LJV7_ErrorCode.h"
#include <QtWidgets/QWidget>
#include "ui_sensor.h"

class Sensor : public QWidget
{
	Q_OBJECT

public:
	Sensor(QWidget *parent = 0);
	~Sensor();

	bool n_isOpenDevice;
	static void ReceiveHighSpeedData(BYTE* pBuffer, DWORD dwSize, DWORD dwCount, DWORD dwNotify, DWORD dwUser);

private:
	Ui::SensorClass ui;
	bool n_isopen;
	int m_xvCurrentDeviceID;
	//int m_xvConnection;
	//DeviceData m_aDeviceData[LJV7IF_DEVICE_COUNT];
	//QString localIP = "10 111 228 20";
	DWORD ObtainTargetParamInfo(LJV7IF_TARGET_SETTING& target);
	int m_xvSelectedProgram;
	static LJV7IF_PROFILE_INFO m_profileInfo;
	QString m_xvResult;
	QString m_xvResult2;

private slots:
	void OnOpen();
	void OnClose();
	void OnGetcurrentvalue();
	void OnGetprofiledata();
};

#endif // SENSOR_H
