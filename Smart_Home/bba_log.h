
#include "stdafx.h"

#define BEI_JIN_TIME


#define BBA_LOG_TYPE_ERROR	0
#define BBA_LOG_TYPE_PRINT	1

#define BBA_LOG_PRINTF printf

#define	BBA_LOG_SINGLE_MAX	100	//������־���������
class BBA_Log 
{
public:
	BBA_Log();
	BBA_Log(HWND hwnd);	//���캯��
	int BBA_LOG_Init(HWND hwnd);
	~BBA_Log();	//��������
	void CoutLog(int type,char *str);
	//void Printf(int type, char *str);

private:
	HWND m_hwnd;			//��־������ھ��
	CString m_CS_Log;	//��־����CString��
	char *p_char_Log;		//������־�����ַ�����
	struct tm m_m_logTm;	//��¼��ǰʱ��
	long long m_timeNow;	//��ǰʱ���Ժ���Ϊ��λ������
};
