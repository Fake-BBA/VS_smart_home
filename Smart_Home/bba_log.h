
#include "stdafx.h"

#define BEI_JIN_TIME


#define BBA_LOG_TYPE_ERROR	0
#define BBA_LOG_TYPE_PRINT	1

#define BBA_LOG_PRINTF printf

#define	BBA_LOG_SINGLE_MAX	100	//单条日志最大内容量
class BBA_Log 
{
public:
	BBA_Log();
	BBA_Log(HWND hwnd);	//构造函数
	int BBA_LOG_Init(HWND hwnd);
	~BBA_Log();	//析构函数
	void CoutLog(int type,char *str);
	//void Printf(int type, char *str);

private:
	HWND m_hwnd;			//日志输出窗口句柄
	CString m_CS_Log;	//日志内容CString型
	char *p_char_Log;		//单条日志内容字符串型
	struct tm m_m_logTm;	//记录当前时间
	long long m_timeNow;	//当前时间以毫秒为单位的整型
};
