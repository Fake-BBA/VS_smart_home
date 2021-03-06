
// Smart_HomeDlg.h: 头文件
//
#include "BBA_SOCKET.h"
#include "bba_log.h"
#include "smartHomeTypedef.h"
#pragma once

// CSmartHomeDlg 对话框
class CSmartHomeDlg : public CDialogEx
{
// 构造
public:
	CSmartHomeDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMART_HOME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonScan();
	afx_msg LRESULT OnServerMsg(WPARAM wParam, LPARAM lParam);

	BBA_Log bbaLog;
	BBA_UDP bba_udp;
	DeviceInfo devicesStruct[256];
	int deviceCounter;
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonSetWifi();
	afx_msg void OnTRBNThumbPosChangingSliderLight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderLight(NMHDR *pNMHDR, LRESULT *pResult);

	CListBox m_CListBox_Ctrl;
	afx_msg void OnBnClickedButtonCleanLog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonTemp();
};
