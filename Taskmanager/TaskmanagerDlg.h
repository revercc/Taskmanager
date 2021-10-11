
// TaskmanagerDlg.h: 头文件
//

#pragma once
#include "CProcessDlg.h"
#include "CWindowsDlg.h"
#include "CServiceDlg.h"
#include "CUserDlg.h"

// CTaskmanagerDlg 对话框
class CTaskmanagerDlg : public CDialogEx
{
// 构造
public:
	CTaskmanagerDlg(CWnd* pParent = nullptr);	// 标准构造函数
	virtual ~CTaskmanagerDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TASKMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CTabCtrl m_MyTab;									//Tab控件
	
// 实现
protected:
	BOOL  m_bTop;										//窗口顶层
	BOOL  m_bLittle;									//使用时最小
	BOOL  m_bLittleHide;								//最小化时隐藏
	DWORD m_dwFirstShow;								//默认显示的选项卡
				
	int	  m_iAttachSize;								//附加信息窗口大小
	HICON m_hIcon;


	CProcessDlg		m_Process;							//进程信息子窗口
	CWindowsDlg		m_Windows;							//窗口信息子窗口
	CServiceDlg		m_Service;							//服务信息子窗口
	CUserDlg		m_User;								//用户信息子窗口


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExit();
	afx_msg void OnNewExe();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnTop();
	afx_msg void OnLittle();
	afx_msg void OnLittleHide();
	afx_msg void OnNowRenewal();
	afx_msg LRESULT OnSetTopItem(WPARAM wParam, LPARAM lParam);

	afx_msg void OnSetProcess();
	afx_msg void OnSetWindows();
	afx_msg void OnSetService();
	afx_msg void OnSetUser();
	
	
};
