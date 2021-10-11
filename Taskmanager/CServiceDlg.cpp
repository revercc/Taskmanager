// CServiceDlg.cpp: 实现文件
//

#include "pch.h"
#include "Taskmanager.h"
#include "CServiceDlg.h"
#include "afxdialogex.h"


// CServiceDlg 对话框

IMPLEMENT_DYNAMIC(CServiceDlg, CDialogEx)

CServiceDlg::CServiceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG7, pParent)
{

}

CServiceDlg::~CServiceDlg()
{
}

void CServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CServiceDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_NOWRENEWAL, OnNowRenewal)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CServiceDlg::OnRclickList1)
	ON_COMMAND(ID_32792, &CServiceDlg::OnStartService)
	ON_COMMAND(ID_32793, &CServiceDlg::OnStopService)
	ON_COMMAND(ID_32794, &CServiceDlg::OnReService)
	ON_COMMAND(ID_32795, &CServiceDlg::OnOpenService)
	ON_COMMAND(ID_32796, &CServiceDlg::OnToParticular)
END_MESSAGE_MAP()


// CServiceDlg 消息处理程序


BOOL CServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect	rect;							//初始化列表控件
	GetClientRect(&rect);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_List.InsertColumn(0, TEXT("名称"), LVCFMT_LEFT, rect.right / 3);
	m_List.InsertColumn(1, TEXT("PID"), LVCFMT_LEFT, rect.right / 6);
	m_List.InsertColumn(2, TEXT("描述"), LVCFMT_LEFT, rect.right / 3);
	m_List.InsertColumn(3, TEXT("状态"), LVCFMT_LEFT, rect.right / 6);

	_AddItem();								//添加列表项


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CServiceDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_List.m_hWnd)
	{
		CRect	rect;
		GetClientRect(&rect);

		m_List.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
		m_List.SetColumnWidth(0, rect.right / 3);
		m_List.SetColumnWidth(1, rect.right / 6);
		m_List.SetColumnWidth(2, rect.right / 3);
		m_List.SetColumnWidth(3, rect.right / 6);
	}
}


void CServiceDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CRect   rect;	   //改变对话框的背景为白色
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
}

//添加列表项
void CServiceDlg::_AddItem()
{
	LVITEM stLvitem;
	stLvitem.mask = LVIF_TEXT;


	DWORD dwDataSize;
	DWORD dwServiceNum;
	DWORD dwResumeHandle = 0;
	LPVOID lpServiceAddress;
	SC_HANDLE hService = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);

	::EnumServicesStatusEx(hService, SC_ENUM_PROCESS_INFO, SERVICE_DRIVER | SERVICE_WIN32, SERVICE_STATE_ALL, NULL, 0, &dwDataSize, &dwServiceNum, &dwResumeHandle, NULL);									//获取所需内存大小
	lpServiceAddress = new BYTE[dwDataSize]();
	::EnumServicesStatusEx(hService, SC_ENUM_PROCESS_INFO, SERVICE_DRIVER | SERVICE_WIN32, SERVICE_STATE_ALL, (LPBYTE)lpServiceAddress, dwDataSize, &dwDataSize, &dwServiceNum, &dwResumeHandle, NULL);		//枚举服务信息

	TCHAR szBuffer[MAX_PATH] = { 0 };
	LPVOID lpOldServiceAddress = lpServiceAddress;
	for (int i = 0; i < dwServiceNum; i++)
	{
		stLvitem.iItem = i;
		stLvitem.iSubItem = 0;
		stLvitem.pszText = ((ENUM_SERVICE_STATUS_PROCESS*)lpServiceAddress)->lpServiceName;
		m_List.InsertItem(&stLvitem);



		if (((ENUM_SERVICE_STATUS_PROCESS*)lpServiceAddress)->ServiceStatusProcess.dwProcessId == 0)
			lstrcpy(szBuffer, TEXT(""));
		else
			wsprintf(szBuffer, TEXT("%d"), ((ENUM_SERVICE_STATUS_PROCESS*)lpServiceAddress)->ServiceStatusProcess.dwProcessId);
		stLvitem.iSubItem = 1;
		stLvitem.pszText = szBuffer;
		m_List.SetItem(&stLvitem);


		stLvitem.iSubItem = 2;
		stLvitem.pszText = ((ENUM_SERVICE_STATUS_PROCESS*)lpServiceAddress)->lpDisplayName;
		m_List.SetItem(&stLvitem);

		
		switch (((ENUM_SERVICE_STATUS_PROCESS*)lpServiceAddress)->ServiceStatusProcess.dwCurrentState)
		{
		case SERVICE_CONTINUE_PENDING:
		case SERVICE_RUNNING:
		case SERVICE_START_PENDING:
			stLvitem.pszText = TEXT("正在运行");
			break;
		case SERVICE_PAUSE_PENDING:
		case SERVICE_PAUSED:
		case SERVICE_STOP_PENDING:
		case SERVICE_STOPPED:
			stLvitem.pszText = TEXT("已停止");
			break;
		
		}
		stLvitem.iSubItem = 3;
		m_List.SetItem(&stLvitem);
		
	
		lpServiceAddress = (LPBYTE)lpServiceAddress + sizeof(ENUM_SERVICE_STATUS_PROCESS);			//指向下一个服务信息
	}

	delete[] lpOldServiceAddress;
	::CloseServiceHandle(hService);
}


//更新
LRESULT CServiceDlg::OnNowRenewal(WPARAM wParam, LPARAM lParam)
{
	m_List.DeleteAllItems();
	_AddItem();
	return TRUE;
}

void CServiceDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_iTem = pNMItemActivate->iItem;

	CMenu SubMenu;
	SubMenu.LoadMenu(IDR_MENU5);

	CPoint point;
	point.x = pNMItemActivate->ptAction.x;
	point.y = pNMItemActivate->ptAction.y;
	ClientToScreen(&point);							//客户区坐标转化为屏幕坐标


	//更新各个菜单项
	CString szBuffer = m_List.GetItemText(m_iTem, 1);
	if (szBuffer.GetLength() == 0)
		SubMenu.GetSubMenu(0)->EnableMenuItem(ID_32796, MF_BYCOMMAND | MF_GRAYED);

	szBuffer = m_List.GetItemText(m_iTem, 3);
	if (!lstrcmp(szBuffer.GetBuffer(), TEXT("已停止")))
	{
		SubMenu.GetSubMenu(0)->EnableMenuItem(ID_32793, MF_BYCOMMAND | MF_GRAYED);
		SubMenu.GetSubMenu(0)->EnableMenuItem(ID_32794, MF_BYCOMMAND | MF_GRAYED);
	}
	else if (!lstrcmp(szBuffer.GetBuffer(), TEXT("正在运行")))
	{
		SubMenu.GetSubMenu(0)->EnableMenuItem(ID_32792, MF_BYCOMMAND | MF_GRAYED);
	}


	//弹出菜单
	SubMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, point.x, point.y, this);

}


void CServiceDlg::OnStartService()
{
	// TODO: 在此添加命令处理程序代码
	SC_HANDLE hScmanger = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	SC_HANDLE hService = OpenService(hScmanger, m_List.GetItemText(m_iTem, 0), SERVICE_START);
	
	if (!StartService(hService, 0, NULL))
		MessageBox(TEXT("启动失败"));
	else
		m_List.SetItemText(m_iTem, 3, TEXT("正在运行"));

	CloseServiceHandle(hService);
	CloseServiceHandle(hScmanger);
}


void CServiceDlg::OnStopService()
{
	SC_HANDLE hScmanger = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	SC_HANDLE hService = OpenService(hScmanger, m_List.GetItemText(m_iTem, 0), SERVICE_STOP);

	SERVICE_STATUS stService;
	if (!ControlService(hService, SERVICE_CONTROL_STOP, &stService))
		MessageBox(TEXT("停止失败"));
	else
		m_List.SetItemText(m_iTem, 3, TEXT("已停止"));

	CloseServiceHandle(hService);
	CloseServiceHandle(hScmanger);
	// TODO: 在此添加命令处理程序代码
}


void CServiceDlg::OnReService()
{
	// TODO: 在此添加命令处理程序代码
	SC_HANDLE hScmanger = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	SC_HANDLE hService = OpenService(hScmanger, m_List.GetItemText(m_iTem, 0), SERVICE_STOP | SERVICE_START);

	SERVICE_STATUS stService;
	ControlService(hService, SERVICE_CONTROL_STOP, &stService);				//先暂停，在启动
	StartService(hService, 0, NULL);
	SendMessage(WM_NOWRENEWAL);	

	CloseServiceHandle(hService);
	CloseServiceHandle(hScmanger);

}


void CServiceDlg::OnOpenService()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR szBuffer[MAX_PATH] = { 0 };
	lstrcpy(szBuffer, TEXT("C:\\Windows\\System32\\mmc.exe services.msc"));
	if(WinExec(szBuffer, 0) <= 31)
		MessageBox("打开服务失败");
}


void CServiceDlg::OnToParticular()
{
	// TODO: 在此添加命令处理程序代码
	
	::SendMessage(m_hParent, WM_SETTOPITEM, 0, (LPARAM)m_iTem);
	
}
