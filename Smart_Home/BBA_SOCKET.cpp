#include "stdafx.h"
#include "BBA_SOCKET.h"

BBA_UDP::BBA_UDP()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//printf("WSAStartup failed\n");
		//return -1;
	}
	GetLocalName_IP();
}

BBA_UDP::BBA_UDP(char ipaddress[8] = "127.0.0.1", short PORT = 1025)
{
	//��ʼ�����绷��  
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//printf("WSAStartup failed\n");
		//return -1;
	}
	GetLocalName_IP();
	//����һ��UDP��socket  
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		//printf("create socket failed\n");
		//return -1;
	}
	SetRemote(ipaddress, PORT);

}

BBA_UDP::BBA_UDP(char *local_address, short local_port, char *remote_address, short remote_port)
{
	//��ʼ�����绷��  
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//printf("WSAStartup failed\n");
		//return -1;
	}
	GetLocalName_IP();
	//����һ��UDP��socket  
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		//printf("create socket failed\n");
		//return -1;
	}
	SetRemote(remote_address, remote_port);
	BindLocal(local_address, local_port);
}

BBA_UDP::~BBA_UDP()
{
	WSACleanup();
}

int BBA_UDP::BBA_UDP_Init(char *local_address, short local_port, char *remote_address, short remote_port)
{
	//����һ��UDP��socket  
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket == INVALID_SOCKET)
	{
		//printf("create socket failed\n");
		//return -1;
	}
	SetRemote(remote_address, remote_port);
	BindLocal(local_address, local_port);
	return 1;
}


int BBA_UDP::SetRemote(char* ipaddress, short PORT)
{
	memset(&addrTarge, 0x00, sizeof(addrTarge));
	addrTarge.sin_family = AF_INET;
	addrTarge.sin_port = htons(PORT);
	addrTarge.sin_addr.S_un.S_addr = inet_addr(ipaddress);
	return 1;
}
int BBA_UDP::BindLocal(char* ipaddress, short PORT)
{
	memset(&addrLocal, 0x00, sizeof(addrTarge));
	addrLocal.sin_family = AF_INET;
	addrLocal.sin_port = htons(PORT);
	addrLocal.sin_addr.S_un.S_addr = inet_addr(ipaddress);
	bind(m_socket,(sockaddr*)&addrLocal, sizeof(sockaddr));
	return 1;
}

int BBA_UDP::Send(char *bytes, unsigned int len)
{
	sendto(m_socket, bytes, len, 0, (SOCKADDR *)&addrTarge, sizeof(SOCKADDR));
	int ret2 = WSAGetLastError();
	return ret2;
}

int BBA_UDP::Recvfrom(char *buf)
{
	memset(&addrRemote, 0x00, sizeof(addrRemote));
	return recvfrom(m_socket, buf, 512, 0, (SOCKADDR *)&addrRemote, &nServerAddrLen);
	
}

long BBA_UDP::OnServerMsg(WPARAM wParam, LPARAM lParam)
{
	return 1;
}

int BBA_UDP::GetLocalName_IP()
{
	gethostname(localName, sizeof(localName));// ��ñ���������.
	hostent* hn;
	hn = gethostbyname(localName);//���ݱ����������õ�����ip
	CString temp = inet_ntoa(*(in_addr *)hn->h_addr_list[0]);//��ip�����ַ�����ʽ
	strcpy(localIP, temp);

	return 1;
}