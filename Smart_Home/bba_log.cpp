#include "stdafx.h"
#include "bba_log.h"
#include <stdio.h>
#include <time.h>



BBA_Log::BBA_Log()
{

}

BBA_Log::BBA_Log(HWND hwnd)
{
	m_hwnd = hwnd;
	p_char_Log = new char(BBA_LOG_SINGLE_MAX);
	//AllocConsole();
	//freopen("CONOUT$", "w+t", stdout);
}

int BBA_Log::BBA_LOG_Init(HWND hwnd)
{
	m_hwnd = hwnd;
	p_char_Log = new char(BBA_LOG_SINGLE_MAX);
	//AllocConsole();
	//freopen("CONOUT$", "w+t", stdout);
	return 1;
}

BBA_Log::~BBA_Log()
{

	delete p_char_Log;
}
void BBA_Log::CoutLog(int type, char *str)
{	
	time(&m_timeNow);
	m_m_logTm = *gmtime(&m_timeNow);
	
	
#ifdef BEI_JIN_TIME
	sprintf(p_char_Log, "[%02d:%02d:%02d] :%s\r\n", m_m_logTm.tm_hour + 8, m_m_logTm.tm_min, m_m_logTm.tm_sec, str);
#else
	sprintf(char_Log, "[%02d:%02d:%02d] :%s\r\n", m_logTm.tm_hour + 8, m_logTm.tm_min, m_logTm.tm_sec, str);
#endif


	m_CS_Log.Insert(0, (CString)p_char_Log);
	
	SetWindowText(m_hwnd, m_CS_Log);
}

