
// Smart_HomeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Smart_Home.h"
#include "Smart_HomeDlg.h"
#include "afxdialogex.h"
#include "smartHomeTypedef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmartHomeDlg 对话框



CSmartHomeDlg::CSmartHomeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SMART_HOME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSmartHomeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_CListBox_Ctrl);
}

BEGIN_MESSAGE_MAP(CSmartHomeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CSmartHomeDlg::OnBnClickedButtonScan)
	ON_MESSAGE(WM_BBA_UDP_RECV, OnServerMsg)
	ON_BN_CLICKED(IDC_BUTTON_SET_WIFI, &CSmartHomeDlg::OnBnClickedButtonSetWifi)
	ON_BN_CLICKED(IDC_BUTTON_CLEAN_LOG, &CSmartHomeDlg::OnBnClickedButtonCleanLog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_TEMP, &CSmartHomeDlg::OnBnClickedButtonTemp)
END_MESSAGE_MAP()




BOOL CSmartHomeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	bbaLog.BBA_LOG_Init(&m_CListBox_Ctrl);
	
	bba_udp.BBA_UDP_Init(bba_udp.localIP, 1026, bba_udp.localIP, 1026);

	((CSliderCtrl*)GetDlgItem(IDC_SLIDER_LIGHT))->SetRange(0, 255, TRUE);
	//SOCKET 异步消息
	WSAAsyncSelect(bba_udp.m_socket, m_hWnd, WM_BBA_UDP_RECV, FD_READ);
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSmartHomeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSmartHomeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSmartHomeDlg::OnBnClickedButtonScan()
{
	// TODO: 在此添加控件通知处理程序代码
	bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, "开始扫描局域网");
	deviceCounter = 0;
	char ipAddress[20];
	char logChar[100];
	int number=254;		//局域网主机数量
	int ret=0;	//获取socket发送结果码

	char device[10];
	MessagePacketUnion messPack;
	uint8 ssidLen, passLen, deviceNum;
	//CString cstringDevice;

	//GetDlgItem(IDC_EDIT_DEVICE_NUM)->GetWindowText(cstringDevice);
	//strcpy(device, cstringDevice);

	messPack.messagePacket.sender = 1;
	messPack.messagePacket.receiver = 0;
	messPack.messagePacket.function_word = FIND_DEVICE;

	for (int i = 1; i < number; i++)
	{
		bba_udp.addrTarge.sin_addr.S_un.S_un_b=bba_udp.addrLocal.sin_addr.S_un.S_un_b;
		bba_udp.addrTarge.sin_addr.S_un.S_un_b.s_b4 = i;	//只取IP前3位
	
		ret = bba_udp.Send((char *)messPack.p_buff, 5);
		//sprintf(logChar, "发送结果%d\r\n",ret);
		//bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logChar);
		
	}
	bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, "局域网请求发送完毕\r\n");
}

LRESULT CSmartHomeDlg::OnServerMsg(WPARAM wParam, LPARAM lParam)
{
	//return 0;
	SOCKET m_socket = (SOCKET)wParam;
	char p_buffer[200],logStr[200];
	memset(p_buffer, 0x00, 200);
	int len;
	CString temp;
	

	switch (WSAGETSELECTEVENT(lParam))
	{

	case FD_READ:

		len = bba_udp.Recvfrom(p_buffer);
		enum MessageCtrFunction messageCtrFunction;
		static union MessagePacketUnion messagePacketUnion;
		memcpy(messagePacketUnion.p_buff, p_buffer, MESSAGE_PACKET_SIZE);

		messageCtrFunction = (MessageCtrFunction)messagePacketUnion.messagePacket.function_word;
		sprintf(logStr, "功能字：%d\r\n", messageCtrFunction);
		bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);

		sprintf(logStr, "发送者：%d\r\n", messagePacketUnion.messagePacket.sender);
		bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
		
		sprintf(logStr,"接收者：%d\r\n", messagePacketUnion.messagePacket.receiver);
		bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);

		switch (messageCtrFunction)
		{
		case HEARTBEAT:
			//messagePacketUnion.messagePacket.function_word = 0;	//心跳包
			sprintf(logStr, "接收到设备：%d的心跳包\r\n", messagePacketUnion.messagePacket.sender);
			bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
			break;
		case FIND_DEVICE:
			//心跳包给查找者
			
			break;
		case LIGHT:
			uint8 luminance;
			luminance= messagePacketUnion.messagePacket.data[0];	//亮度
			sprintf(logStr, "设备：%d确认亮度：%d\r\n", messagePacketUnion.messagePacket.sender,luminance);
			bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
			break;
		case FAN:
			break;
		case HUMITURE:
			uint8 temperature, humidity;
			temperature=messagePacketUnion.messagePacket.data[0];
			humidity=messagePacketUnion.messagePacket.data[1];

			temp.AppendFormat("温度：%d\t,湿度：%d\t", temperature, humidity);
			GetDlgItem(IDC_STATIC_TEMP_HUMIDITY)->SetWindowText(temp);

		sprintf(logStr,"设备：%d的温度:%d\t 湿度:%d\r\n", messagePacketUnion.messagePacket.sender, temperature, humidity);
		bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
		break;
		case DEVICE_ERROS:
			deviceCounter++;
			SOCKADDR_IN m_address;
			memset(&m_address, 0, sizeof(m_address));
			int nAddrLen;
			nAddrLen= sizeof(m_address);

			devicesStruct[messagePacketUnion.messagePacket.sender].address.sin_addr = bba_udp.addrRemote.sin_addr;
			devicesStruct[messagePacketUnion.messagePacket.sender].isExist = 1;

			sprintf(logStr, "设备：%d 回应\r\n", messagePacketUnion.messagePacket.sender);
			bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);

			sprintf(logStr, "设备：%d 对应IP：%s\r\n", messagePacketUnion.messagePacket.sender,inet_ntoa(bba_udp.addrRemote.sin_addr));
			bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);

			temp.AppendFormat("找到 %d 个设备\r\n", deviceCounter);
			GetDlgItem(IDC_STATIC_DEVICE_NUM)->SetWindowText(temp);
			break;
		default:
			sprintf(logStr, "没有对应功能字:%d\r\n", messagePacketUnion.messagePacket.function_word);
			bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
			break;
		}
		
	case FD_CLOSE:
		break;
	default:
		break;
	}
	return 1;
}


void CSmartHomeDlg::OnBnClickedButtonSetWifi()
{
	// TODO: 在此添加控件通知处理程序代码

	bba_udp.BBA_UDP_Init("192.168.4.2", 1026, "192.168.4.1", 1025);

	char wifiSSid[50];
	char wifiPass[50];
	char device[10];
	MessagePacketUnion messPack;
	uint8 ssidLen, passLen,deviceNum;
	CString cstringWifiSSid;
	CString cstringWifiPass;
	CString cstringDevice;

	GetDlgItem(IDC_EDIT_DEVICE_NUM)->GetWindowText(cstringDevice);
	strcpy(device, cstringDevice);

	messPack.messagePacket.sender = 1;
	messPack.messagePacket.receiver = atoi(device);
	messPack.messagePacket.function_word = WIFI_CONFIG;
	
	GetDlgItem(IDC_EDIT_WIFI_SSID)->GetWindowText(cstringWifiSSid);
	GetDlgItem(IDC_EDIT_WIFI_PASSWORD)->GetWindowText(cstringWifiPass);

	strcpy(wifiSSid, cstringWifiSSid);
	strcpy(wifiPass, cstringWifiPass);
	
	ssidLen = strlen(wifiSSid);
	passLen = strlen(wifiPass);

	messPack.messagePacket.data[0] = ssidLen;
	memcpy(messPack.messagePacket.data +1, wifiSSid, ssidLen);
	messPack.messagePacket.data[ssidLen+1] = passLen;
	memcpy(messPack.messagePacket.data +ssidLen+2, wifiPass, passLen);

	bba_udp.Send((char *)messPack.p_buff, 5+ssidLen+passLen+2);
	//bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, buf);
}


void CSmartHomeDlg::OnBnClickedButtonCleanLog()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CListBox_Ctrl.ResetContent();
}


void CSmartHomeDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

	static short staticValue=0;
	uint8 luminance;
	luminance = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_LIGHT))->GetPos();
	if (staticValue == luminance) return;
	staticValue = luminance;
	

	char device[10];
	char logStr[100];
	CString cstringDevice;

	GetDlgItem(IDC_EDIT_DEVICE_NUM)->GetWindowText(cstringDevice);
	strcpy(device, cstringDevice);

	MessagePacketUnion messPack;
	messPack.messagePacket.sender = 1;
	messPack.messagePacket.receiver = atoi(device);
	messPack.messagePacket.function_word = LIGHT;

	if (devicesStruct[messPack.messagePacket.receiver].isExist == 1)	//设备存在
	{
		bba_udp.SetRemote(devicesStruct[messPack.messagePacket.receiver].address, 1026);
		messPack.messagePacket.data[0] = luminance;

		bba_udp.Send((char *)messPack.p_buff, 6);
		sprintf(logStr, "发送亮度：%d\r\n", luminance);
		
	}
	else
	{
		sprintf(logStr, "设备：%d不存在\r\n", messPack.messagePacket.receiver);
	}
	
	bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);
	
}


void CSmartHomeDlg::OnBnClickedButtonTemp()
{
	// TODO: 在此添加控件通知处理程序代码

	CString cstringDevice;
	char device[10];
	char logStr[100];

	GetDlgItem(IDC_EDIT_DEVICE_NUM)->GetWindowText(cstringDevice);
	strcpy(device, cstringDevice);

	MessagePacketUnion messPack;
	messPack.messagePacket.sender = 1;
	messPack.messagePacket.receiver = atoi(device);

	if (devicesStruct[messPack.messagePacket.receiver].isExist == 1)	//设备存在
	{
		bba_udp.SetRemote(devicesStruct[messPack.messagePacket.receiver].address, 1026);
		messPack.messagePacket.function_word = HUMITURE;
		bba_udp.Send((char *)messPack.p_buff, 5);
		sprintf(logStr, "请求设备：%d IP:%s 温度\r\n", messPack.messagePacket.receiver, inet_ntoa(bba_udp.addrTarge.sin_addr));
	}
	else
	{
		sprintf(logStr, "设备：%d不存在\r\n", messPack.messagePacket.receiver);
	}

	
	
	bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, logStr);

}
