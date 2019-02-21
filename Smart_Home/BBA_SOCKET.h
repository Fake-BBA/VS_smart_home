#ifndef _BBA_SOCKET_H__
#define _BBA_SOCKET_H__

#include "stdafx.h" 

#define WM_BBA_UDP_RECV	1080
class BBA_UDP
{
public:
	BBA_UDP();
	BBA_UDP(char *ipaddress , short PORT );
	int BBA_UDP_Init(HWND m_hWnd,char *local_address, short local_port, char *remote_address, short remote_port);
	BBA_UDP(char *local_address, short local_port, char *remote_address, short remote_port);
	int SetRemote(char* addrTarge,short PORT);	//����Ŀ��
	int BindLocal(char* addrLocal,short PORT);
	int Send(char *bytes, unsigned int len);
	int Recvfrom(char *buf);
	long OnServerMsg(WPARAM wParam, LPARAM lParam);
	~BBA_UDP();
public:
	SOCKET m_socket;
	sockaddr_in addrLocal;			//�󶨵ı���IP
	sockaddr_in addrTarge;			//������ϢĿ��
	sockaddr_in addrRemote;			//������Ϣ��һ��
	int nServerAddrLen = sizeof(sockaddr_in);
	char recvBuf[512];

};
#endif