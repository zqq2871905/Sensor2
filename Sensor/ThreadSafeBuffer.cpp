//Copyright(c) 2013, KEYENCE Corporation. All rights reserved.
#include "ThreadSafeBuffer.h"

// Singleton Pattern

ThreadSafeBuffer* ThreadSafeBuffer::m_threadSafeBuffer = 0;

ThreadSafeBuffer::ThreadSafeBuffer(void)
{
	for (int i = 0; i < LJV7IF_DEVICE_COUNT; i++) { m_adwCount[i] = 0; }
	for (int i = 0; i < LJV7IF_DEVICE_COUNT; i++) { m_adwNotify[i] = 0; }
	for (int i = 0; i < LJV7IF_DEVICE_COUNT; i++) { m_anBatchNo[i] = 0; }
}

ThreadSafeBuffer::~ThreadSafeBuffer(void)
{
}

ThreadSafeBuffer* ThreadSafeBuffer::getInstance(void)
{
	if (m_threadSafeBuffer == 0)
	{
		m_threadSafeBuffer = new ThreadSafeBuffer();
	}
	return m_threadSafeBuffer;
}

/*
@note Add the profile data
@param Current device ID
@param Profile data
@param notify
*/
void ThreadSafeBuffer::Add(DWORD dwIndex, vector<PROFILE_DATA> &vecProfileData, DWORD dwNotify)
{
	for (unsigned int i = 0; i < vecProfileData.size(); i++)
	{
		m_vecProfileData[dwIndex].push_back(vecProfileData.at(i));
	}

	m_adwCount[dwIndex] += (unsigned int)vecProfileData.size();
	m_adwNotify[dwIndex] |= dwNotify;
	if ((dwNotify & (0x1 << 16)) != 0) m_anBatchNo[dwIndex]++;
}

/*
@note Add the profile count
@param Current device ID
@param Profile count
@param notify
*/
void ThreadSafeBuffer::AddCount(DWORD dwIndex, DWORD dwCount, DWORD dwNotify)
{
	m_adwCount[dwIndex] += dwCount;
	m_adwNotify[dwIndex] |= dwNotify;
	if ((dwNotify & (0x1 << 16)) != 0) m_anBatchNo[dwIndex]++;
}

/*
@note Get the profile
@param Current device ID
@param Notify
@param Batch Number
@param Profile data
*/
void ThreadSafeBuffer::Get(DWORD dwIndex, DWORD* pdwNotify, int* pnBatchNo, vector<PROFILE_DATA> &vecProfileData)
{
	vecProfileData = m_vecProfileData[dwIndex];		// Store the data
	m_vecProfileData[dwIndex].clear();				// Clear the data

	*pdwNotify = m_adwNotify[dwIndex];
	m_adwNotify[dwIndex] = 0;
	*pnBatchNo = m_anBatchNo[dwIndex];
}

/*
@note Get the profile count
@param Current device ID
@param Notify
@param Batch Number
*/
DWORD ThreadSafeBuffer::GetCount(DWORD dwIndex, DWORD* pdwNotify, int* pnBatchNo)
{
	*pdwNotify = m_adwNotify[dwIndex];
	m_adwNotify[dwIndex] = 0;
	*pnBatchNo = m_anBatchNo[dwIndex];

	return m_adwCount[dwIndex];
}

/*
@note Clear the buffer data
@param Current Device ID
*/
void ThreadSafeBuffer::ClearBuffer(int nCurrentDeviceID)
{
	m_vecProfileData[nCurrentDeviceID].clear();
	m_adwCount[nCurrentDeviceID] = 0;
	m_anBatchNo[nCurrentDeviceID] = 0;
	m_adwNotify[nCurrentDeviceID] = 0;
}