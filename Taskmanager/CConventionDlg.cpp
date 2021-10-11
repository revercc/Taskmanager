// CConventionDlg.cpp: 实现文件
//

#include "pch.h"
#include "Taskmanager.h"
#include "CConventionDlg.h"


// CConventionDlg

IMPLEMENT_DYNAMIC(CConventionDlg, CMFCPropertyPage)

CConventionDlg::CConventionDlg():CMFCPropertyPage(IDD_DIALOG5)
{
	
}

CConventionDlg::~CConventionDlg()
{
}


BEGIN_MESSAGE_MAP(CConventionDlg, CMFCPropertyPage)
END_MESSAGE_MAP()



// CConventionDlg 消息处理程序




void CConventionDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 在此添加专用代码和/或调用基类
	DDX_Control(pDX, IDC_EDIT1, m_CreateTime);
	DDX_Control(pDX, IDC_EDIT2, m_ChangeTime);
	DDX_Control(pDX, IDC_EDIT3, m_VisitTime);
	DDX_Control(pDX, IDC_EDIT4, m_Path);
	DDX_Control(pDX, IDC_EDIT5, m_Size);

	CMFCPropertyPage::DoDataExchange(pDX);
}


BOOL CConventionDlg::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	WIN32_FIND_DATA stFileData;
	::FindFirstFile(m_szFilePath, &stFileData);


	//将各个常规属性页的内容更新
	SYSTEMTIME stSysTime;
	::FileTimeToSystemTime(&stFileData.ftCreationTime, &stSysTime);
	
	TCHAR szTime[MAX_PATH] = { 0 };
	wsprintf(szTime, TEXT("%d年%d月%d日 星期%d, %d:%d:%d"), stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay, stSysTime.wDayOfWeek, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);
	m_CreateTime.SetWindowText(szTime);


	::FileTimeToSystemTime(&stFileData.ftLastWriteTime, &stSysTime);
	wsprintf(szTime, TEXT("%d年%d月%d日 星期%d, %d:%d:%d"), stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay, stSysTime.wDayOfWeek, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);
	m_ChangeTime.SetWindowText(szTime);


	::FileTimeToSystemTime(&stFileData.ftLastAccessTime, &stSysTime);
	wsprintf(szTime, TEXT("%d年%d月%d日 星期%d, %d:%d:%d"), stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay, stSysTime.wDayOfWeek, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);
	m_VisitTime.SetWindowText(szTime);


	wsprintf(szTime, TEXT("%d字节"), (stFileData.nFileSizeHigh * (MAXDWORD + 1)) + stFileData.nFileSizeLow);
	m_Size.SetWindowText(szTime);

	
	m_Path.SetWindowText(m_szFilePath);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
