
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
	DDX_Control(pDX, IDC_EDIT_LOG, winLog);
}

BEGIN_MESSAGE_MAP(CSmartHomeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CSmartHomeDlg::OnBnClickedButtonScan)
	ON_MESSAGE(WM_BBA_UDP_RECV, OnServerMsg)
	ON_BN_CLICKED(IDC_BUTTON_SET_WIFI, &CSmartHomeDlg::OnBnClickedButtonSetWifi)
END_MESSAGE_MAP()




BOOL CSmartHomeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	bbaLog.BBA_LOG_Init(winLog.m_hWnd);
	bba_udp.BBA_UDP_Init(this->m_hWnd,"192.168.4.2", 1026, "192.168.4.1", 1025);
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
	unsigned char sendBuff[100];
	int number=255;

	for (int i = 0; i < number;i++)
	{
		;// bba_udp.Send(sendBuff, );
	}
}

LRESULT CSmartHomeDlg::OnServerMsg(WPARAM wParam, LPARAM lParam)
{
	SOCKET sock = (SOCKET)wParam;
	char buf[200];
	memset(buf, 0x00, 200);
	int len;

	switch (WSAGETSELECTEVENT(lParam))
	{

	case FD_READ:
		len = bba_udp.Recvfrom(buf);
		
		bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, buf);
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
	messPack.messagePacket.function_word = 254;
	
	GetDlgItem(IDC_EDIT_WIFI_SSID)->GetWindowText(cstringWifiSSid);
	GetDlgItem(IDC_EDIT_WIFI_PASSWORD)->GetWindowText(cstringWifiPass);

	strcpy(wifiSSid, cstringWifiSSid);
	strcpy(wifiPass, cstringWifiPass);
	
	//sprintf(sendBuff,"%s%")
	ssidLen = strlen(wifiSSid);
	passLen = strlen(wifiPass);

	messPack.messagePacket.data[0] = ssidLen;
	memcpy(messPack.messagePacket.data +1, wifiSSid, ssidLen);
	messPack.messagePacket.data[ssidLen+1] = passLen;
	memcpy(messPack.messagePacket.data +ssidLen+2, wifiPass, passLen);

	bba_udp.Send((char *)messPack.p_buff, 5+ssidLen+passLen+2);
	//bbaLog.CoutLog(BBA_LOG_TYPE_PRINT, buf);
}
