// CProcessDlg.cpp: 实现文件
//

#include "pch.h"
#include "Taskmanager.h"
#include "CProcessDlg.h"
#include "afxdialogex.h"
#include "CModuleDlg.h"
#include "CThreadDlg.h"
#include "CPropertyDlg.h"
#include <TlHelp32.h>

// CProcessDlg 对话框

IMPLEMENT_DYNAMIC(CProcessDlg, CDialogEx)

CProcessDlg::CProcessDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CProcessDlg::OnRclickList1)
	ON_COMMAND(ID_32777, &CProcessDlg::OnExitProcess)
	ON_COMMAND(ID_32778, &CProcessDlg::OnLookModule)
	ON_COMMAND(ID_32779, &CProcessDlg::OnLookThread)
	ON_COMMAND(ID_32780, &CProcessDlg::OnOpenFile)
	ON_COMMAND(ID_32781, &CProcessDlg::OnProperty)
	ON_MESSAGE(WM_NOWRENEWAL, OnRenewal)
	
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序


BOOL CProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	

	CRect	rect;							//初始化列表控件
	GetClientRect(&rect);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_List.InsertColumn(0, TEXT("名称"), LVCFMT_LEFT, rect.right / 6);
	m_List.InsertColumn(1, TEXT("状态"), LVCFMT_LEFT, rect.right / 6);
	m_List.InsertColumn(2, TEXT("PID"), LVCFMT_LEFT, rect.right / 6);
	m_List.InsertColumn(3, TEXT("路径"), LVCFMT_LEFT, rect.right / 2 );
	
	_AddItem();						//添加列表子项
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CProcessDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()

	CRect   rect;	   //改变对话框的背景为白色
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
}


void CProcessDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_List.m_hWnd)
	{
		CRect	rect;
		GetClientRect(&rect);

		m_List.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
		m_List.SetColumnWidth(0, rect.right / 6);
		m_List.SetColumnWidth(1, rect.right / 6);
		m_List.SetColumnWidth(2, rect.right / 6);
		m_List.SetColumnWidth(3, rect.right / 2);

	}
}


//添加列表项
void CProcessDlg::_AddItem()
{
	LVITEM stLvitem;
	stLvitem.mask = LVIF_TEXT;

	pfnNtQuerySystemInformation NtQuerySystemInformation = (pfnNtQuerySystemInformation)::GetProcAddress(::LoadLibrary(TEXT("ntdll.dll")), TEXT("NtQuerySystemInformation"));

	HANDLE hProcessSnap;
	PROCESSENTRY32 stPe32 = { 0 };  // 进程快照信息
	stPe32.dwSize = sizeof(PROCESSENTRY32);
	hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	::Process32First(hProcessSnap, &stPe32);

	int i = 0;
	TCHAR szBuffer[MAX_PATH];
	LPVOID dwBufferProcess = 0;
	DWORD dwBufferProcessSize = 0;
	DWORD dwThreadNum;


	NtQuerySystemInformation(SystemProcessInformation, 0, 0, &dwBufferProcessSize);
	dwBufferProcess = new BYTE[dwBufferProcessSize + 0x10000]();									//为了防止进程/线程信息发生突变，多申请0x10000内存
	LPVOID dwOldBufferProcess = dwBufferProcess;
	NtQuerySystemInformation(SystemProcessInformation, dwBufferProcess, dwBufferProcessSize + 0x10000, &dwBufferProcessSize);

	do {
		memset(szBuffer, 0, sizeof(szBuffer));
		

		stLvitem.iItem = i;
		stLvitem.iSubItem = 0;
		stLvitem.pszText = stPe32.szExeFile;
		m_List.InsertItem(&stLvitem);


		stLvitem.iSubItem = 1;
		stLvitem.pszText = TEXT("正在运行");


		
		
		dwThreadNum = ((SYSTEM_PROCESS_INFORMATION*)dwBufferProcess)->NumberOfThreads;				//线程数目
		LPVOID dwAddress = dwBufferProcess;
		dwBufferProcess = (BYTE*)dwBufferProcess + sizeof(SYSTEM_PROCESS_INFORMATION);
		DWORD dwStatus = 0;
		for (int i = 0; i < dwThreadNum; i++)
		{
			//检测进程状态和导致此状态的原因
			if (((SYSTEM_THREAD_INFORMATION*)dwBufferProcess)->ThreadState == 5 && ((SYSTEM_THREAD_INFORMATION*)dwBufferProcess)->WaitReason == 5)
				dwStatus = 1;
			else
				dwStatus = 0;
			dwBufferProcess = (BYTE*)dwBufferProcess + sizeof(SYSTEM_THREAD_INFORMATION);
		}
		dwBufferProcess = (LPVOID)((BYTE*)dwAddress + ((SYSTEM_PROCESS_INFORMATION*)dwAddress)->NextEntryOffset);

		if(dwStatus == 1)
			stLvitem.pszText = TEXT("挂起");
		m_List.SetItem(&stLvitem);

	
		wsprintf(szBuffer, TEXT("%d"), stPe32.th32ProcessID);
		stLvitem.iSubItem = 2;
		stLvitem.pszText = szBuffer;
		m_List.SetItem(&stLvitem);


		DWORD nSize = MAX_PATH;
		TCHAR szBuffer1[MAX_PATH] = { 0 };
		HANDLE hProcess;
		hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, stPe32.th32ProcessID);
		QueryFullProcessImageName(hProcess, 0, szBuffer1, &nSize);					//提升权限，并获得可执行文件路径
		stLvitem.pszText = szBuffer1;
		stLvitem.iSubItem = 3;
		m_List.SetItem(&stLvitem);
		
		i++;
	} while (Process32Next(hProcessSnap, &stPe32));
	
	dwBufferProcess = dwOldBufferProcess;
	delete[] dwBufferProcess;


	::CloseHandle(hProcessSnap);
}

void CProcessDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_iTem = pNMItemActivate->iItem;

	
	CMenu subMenu;
	subMenu.LoadMenu(IDR_MENU2);



	CPoint point;
	point.x = pNMItemActivate->ptAction.x;
	point.y = pNMItemActivate->ptAction.y;
	ClientToScreen(&point);							//客户区坐标转化为屏幕坐标
	
	CString szExePath = m_List.GetItemText(m_iTem, 3);
	if (szExePath.GetLength() == 0)
	{
		subMenu.GetSubMenu(0)->EnableMenuItem(ID_32780, MF_BYCOMMAND | MF_GRAYED);
		subMenu.GetSubMenu(0)->EnableMenuItem(ID_32781, MF_BYCOMMAND | MF_GRAYED);
	}
	subMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, point.x, point.y, this);			//弹出子菜单


}


//结束指定进程
void CProcessDlg::OnExitProcess()
{
	// TODO: 在此添加命令处理程序代码
	
	CString szPid = m_List.GetItemText(m_iTem, 2);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, atoi(szPid.GetBuffer()));
	if (!TerminateProcess(hProcess, 0))
		MessageBox(TEXT("无法结束进程！"));
	else
		m_List.DeleteItem(m_iTem);
}



void CProcessDlg::OnLookModule()
{
	// TODO: 在此添加命令处理程序代码
	CString szPid = m_List.GetItemText(m_iTem, 2);

	CModuleDlg module;
	module.dwPid = atoi(szPid.GetBuffer());
	module.DoModal();
}


void CProcessDlg::OnLookThread()
{
	// TODO: 在此添加命令处理程序代码
	CString szPid = m_List.GetItemText(m_iTem, 2);

	CThreadDlg thread;
	thread.m_dwPid = atoi(szPid.GetBuffer());
	thread.DoModal();
}


void CProcessDlg::OnOpenFile()
{
	// TODO: 在此添加命令处理程序代码
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	TCHAR szExe[MAX_PATH] = TEXT("c:\\Windows\\explorer.exe ");
	TCHAR szCmdLine[MAX_PATH] = TEXT("/e,/select,");
	CString szExe1 = m_List.GetItemText(m_iTem, 3);

	lstrcat(szCmdLine, szExe1.GetBuffer());
	lstrcat(szExe, szCmdLine);

	CreateProcess(NULL, szExe, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}


void CProcessDlg::OnProperty()
{
	// TODO: 在此添加命令处理程序代码

	CString szExePath = m_List.GetItemText(m_iTem, 3);
	CPropertyDlg property(TEXT("属性"));

	property.m_ConventionPage.m_szFilePath = szExePath;
	property.DoModal();
}





//更新
LRESULT CProcessDlg::OnRenewal(WPARAM wParam, LPARAM lParam)
{

	m_List.DeleteAllItems();
	_AddItem();

	return TRUE;
}
