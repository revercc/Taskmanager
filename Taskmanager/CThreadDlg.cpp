// CThreadDlg.cpp: 实现文件
//

#include "pch.h"
#include "Taskmanager.h"
#include "CThreadDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// CThreadDlg 对话框

IMPLEMENT_DYNAMIC(CThreadDlg, CDialogEx)

CThreadDlg::CThreadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CThreadDlg::~CThreadDlg()
{
}

void CThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CThreadDlg, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CThreadDlg::OnRclickList1)
	ON_COMMAND(ID_32782, &CThreadDlg::OnExitThread)
	ON_COMMAND(ID_32783, &CThreadDlg::OnStopThread)
	ON_COMMAND(ID_32784, &CThreadDlg::OnBeginThread)
END_MESSAGE_MAP()


// CThreadDlg 消息处理程序


BOOL CThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect	rect;							//初始化列表控件
	GetClientRect(&rect);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_List.InsertColumn(0, TEXT("起始地址"), LVCFMT_LEFT, rect.right / 4);
	m_List.InsertColumn(1, TEXT("线程ID"), LVCFMT_LEFT, rect.right / 4);
	m_List.InsertColumn(2, TEXT("状态"), LVCFMT_LEFT, rect.right / 2);


	if (FALSE == _AddItem())				//添加列表项，如果失败就关闭对话框
		EndDialog(0);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



BOOL CThreadDlg::_AddItem()
{


	LVITEM stLvitem;
	stLvitem.mask = LVIF_TEXT;
	pfnNtQuerySystemInformation NtQuerySystemInformation = (pfnNtQuerySystemInformation)::GetProcAddress(::LoadLibrary(TEXT("ntdll.dll")), TEXT("NtQuerySystemInformation"));

	TCHAR szBuffer[MAX_PATH] = { 0 };
	DWORD dwThreadNum;
	DWORD dwBufferProcessSize;
	LPVOID dwBufferProcess;
	NtQuerySystemInformation(SystemProcessInformation, 0, 0, &dwBufferProcessSize);
	dwBufferProcess = new BYTE[dwBufferProcessSize + 0x10000]();									//为了防止进程/线程信息发生突变，多申请0x10000内存
	LPVOID dwOldBufferProcess = dwBufferProcess;
	NtQuerySystemInformation(SystemProcessInformation, dwBufferProcess, dwBufferProcessSize + 0x10000, &dwBufferProcessSize);


	dwThreadNum = ((SYSTEM_PROCESS_INFORMATION*)dwBufferProcess)->NumberOfThreads;				//线程数目
	while(TRUE)
	{
		LPVOID dwAddress = dwBufferProcess;
		if (((SYSTEM_PROCESS_INFORMATION*)dwBufferProcess)->UniqueProcessId == (HANDLE)m_dwPid)
		{
			dwBufferProcess = (BYTE*)dwBufferProcess + sizeof(SYSTEM_PROCESS_INFORMATION);
			for (DWORD i = 0; i < dwThreadNum; i++)
			{


				wsprintf(szBuffer, TEXT("%x"), ((SYSTEM_THREAD_INFORMATION*)dwBufferProcess)->StartAddress);
				stLvitem.iItem = i;
				stLvitem.iSubItem = 0;
				stLvitem.pszText = szBuffer;
				m_List.InsertItem(&stLvitem);


				wsprintf(szBuffer, TEXT("%d"), ((SYSTEM_THREAD_INFORMATION*)dwBufferProcess)->ClientId.UniqueThread);
				stLvitem.iSubItem = 1;
				stLvitem.pszText = szBuffer;
				m_List.SetItem(&stLvitem);


				//检测进程状态和导致此状态的原因
				if (((SYSTEM_THREAD_INFORMATION*)dwBufferProcess)->ThreadState == 5 && ((SYSTEM_THREAD_INFORMATION*)dwBufferProcess)->WaitReason == 5)
					stLvitem.pszText = TEXT("挂起");
				else
					stLvitem.pszText = TEXT("正在执行");
				stLvitem.iSubItem = 2;
				m_List.SetItem(&stLvitem);

				dwBufferProcess = (BYTE*)dwBufferProcess + sizeof(SYSTEM_THREAD_INFORMATION);
			}
		}
		
		
		dwBufferProcess = ((BYTE*)dwAddress + ((SYSTEM_PROCESS_INFORMATION*)dwAddress)->NextEntryOffset);				//指向下一个进程
		if (((SYSTEM_PROCESS_INFORMATION*)dwAddress)->NextEntryOffset == 0)												//遍历完成结束
			break;
	}
	dwBufferProcess = dwOldBufferProcess;
	delete[] dwBufferProcess;



	return TRUE;
}

void CThreadDlg::OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	m_iTem = pNMItemActivate->iItem;

	CMenu subMenu;
	subMenu.LoadMenu(IDR_MENU3);

	CPoint point;
	point.x = pNMItemActivate->ptAction.x;
	point.y = pNMItemActivate->ptAction.y;
	ClientToScreen(&point);							//客户区坐标转化为屏幕坐标
	subMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, point.x, point.y, this);			//弹出子菜单

}


void CThreadDlg::OnExitThread()
{
	// TODO: 在此添加命令处理程序代码

	CString szTid = m_List.GetItemText(m_iTem, 1);

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, atoi(szTid.GetBuffer()));
	if (!TerminateThread(hThread, 0))
		MessageBox(TEXT("无法结束线程"));
	else
		m_List.DeleteItem(m_iTem);

}


void CThreadDlg::OnStopThread()
{
	// TODO: 在此添加命令处理程序代码
	CString szTid = m_List.GetItemText(m_iTem, 1);

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, atoi(szTid.GetBuffer()));
	if (-1 == SuspendThread(hThread))
		MessageBox(TEXT("无法挂起"));
	else
		m_List.SetItem(m_iTem, 2, LVIF_TEXT, TEXT("已挂起"), 0, 0, 0, 0);
}


void CThreadDlg::OnBeginThread()
{
	// TODO: 在此添加命令处理程序代码
	CString szTid = m_List.GetItemText(m_iTem, 1);

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, 0, atoi(szTid.GetBuffer()));
	if (-1 == ResumeThread(hThread))
		MessageBox(TEXT("无法恢复"));
	else
		m_List.SetItem(m_iTem, 2, LVIF_TEXT, TEXT("正在运行"), 0, 0, 0, 0);
}
