//Copyright(c) 2013, KEYENCE Corporation. All rights reserved.
#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "DeviceData.h"

using namespace std;

class DataExport
{
public:
	DataExport(void);
	~DataExport(void);

	static BOOL ExportMeasureDatas(vector<MEASURE_DATA> &vecMeasureData, QString strfileName);
	static BOOL ExportProfileEx(const PROFILE_DATA *profileData, QString strFileName, int nProfileCnt);
	static BOOL ExportProfile(const PROFILE_DATA *profileData, QString strFileName, int nProfileCnt);
};

