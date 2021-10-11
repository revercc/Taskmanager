
// TaskmanagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Taskmanager.h"
#include "TaskmanagerDlg.h"
#include "CNewexeDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{

}



void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTaskmanagerDlg 对话框

CTaskmanagerDlg::CTaskmanagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASKMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bTop = 0;
	m_bLittle = 1;
	m_bLittleHide = 0;
	m_dwFirstShow = 0;


	TCHAR szFilePath[MAX_PATH + 1];							//获取配置文件路径
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	lstrcat(szFilePath, TEXT("deploy"));

															//当文件不存在时创建文件
	HANDLE hFile = ::CreateFile(szFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(hFile);


	CFile file;												//利用串行化得到配置信息
	file.Open(szFilePath, CFile::modeReadWrite);
	if (file.GetLength() == 0)
	{
		CArchive arStore(&file, CArchive::store);
		arStore << m_dwFirstShow;
	}
	else
	{
		CArchive arLoad(&file, CArchive::load);
		arLoad >> m_dwFirstShow;
		m_dwFirstShow = m_dwFirstShow % 3;
	}
	file.Close();

}


CTaskmanagerDlg::~CTaskmanagerDlg()
{
	TCHAR szFilePath[MAX_PATH + 1];							//获取配置文件路径
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	lstrcat(szFilePath, TEXT("deploy"));

	//当文件不存在时创建文件
	HANDLE hFile = ::CreateFile(szFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(hFile);

	CFile file;												//利用串行化得到配置信息
	file.Open(szFilePath, CFile::modeReadWrite);


	CArchive arStore(&file, CArchive::store);				//当CArchive析构函数调用后，文件才能关闭
	arStore << m_dwFirstShow;
	
}

void CTaskmanagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MyTab);
}

BEGIN_MESSAGE_MAP(CTaskmanagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTaskmanagerDlg::OnSelchangeTab1)
	ON_COMMAND(ID_32772, &CTaskmanagerDlg::OnExit)
	ON_COMMAND(ID_32771, &CTaskmanagerDlg::OnNewExe)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_32773, &CTaskmanagerDlg::OnTop)
	ON_COMMAND(ID_32774, &CTaskmanagerDlg::OnLittle)
	ON_COMMAND(ID_32775, &CTaskmanagerDlg::OnLittleHide)
	ON_COMMAND(ID_32785, &CTaskmanagerDlg::OnNowRenewal)
	ON_MESSAGE(WM_SETTOPITEM, OnSetTopItem)
	ON_COMMAND(ID_32797, &CTaskmanagerDlg::OnSetProcess)
	ON_COMMAND(ID_32798, &CTaskmanagerDlg::OnSetWindows)
	ON_COMMAND(ID_32799, &CTaskmanagerDlg::OnSetService)
	ON_COMMAND(ID_32800, &CTaskmanagerDlg::OnSetUser)
	
END_MESSAGE_MAP()


// CTaskmanagerDlg 消息处理程序

BOOL CTaskmanagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();										//意思是告诉编译器不从虚表中调用，而是直接调用CDialogEx的OnInitDialog函数

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	


	m_MyTab.InsertItem(0, TEXT("进程"));
	m_MyTab.InsertItem(1, TEXT("窗口"));
	m_MyTab.InsertItem(2, TEXT("服务"));
	m_MyTab.InsertItem(3, TEXT("用户"));
	

	CRect	rect;
	m_MyTab.GetClientRect(&rect);
	rect.DeflateRect(0, 23, 0, 0);

	m_Process.Create(IDD_DIALOG1, &m_MyTab);	//创建子页面
	m_Windows.Create(IDD_DIALOG2, &m_MyTab);
	m_Service.Create(IDD_DIALOG7, &m_MyTab);
	m_User.Create(IDD_DIALOG8, &m_MyTab);

												//使子页面适应TAB控件
	m_MyTab.SetCurFocus(m_dwFirstShow);			//设置top默认的选项
	m_Process.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, (m_dwFirstShow == 0) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
	m_Windows.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, (m_dwFirstShow == 1) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
	m_Service.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, (m_dwFirstShow == 2) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);
	m_User.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, (m_dwFirstShow == 3) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW);



	m_Service.m_hParent = m_hWnd;				//父窗口句柄

	CRect	WndRect;
	GetClientRect(&WndRect);
	m_iAttachSize = WndRect.bottom - rect.bottom;//附加信息窗口大小

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTaskmanagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)							//如果点击的是关于菜单项，就弹出关于菜单对话框
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else														//否则就调用默认消息处理函数
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}






void CTaskmanagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	switch (nType)
	{
	case SIZE_MAXIMIZED:
	case SIZE_RESTORED:
		if (m_Process.m_hWnd != NULL)												//使tab控件和子页面的大小随着主窗口的大小变化而变化
		{
			CRect rect;
			m_MyTab.GetClientRect(&rect);
			m_MyTab.SetWindowPos(NULL, rect.left, rect.top, cx, cy - m_iAttachSize, SWP_NOACTIVATE);

			m_Process.GetClientRect(&rect);
			rect.DeflateRect(0, 23, 0, 0);
			m_Process.SetWindowPos(NULL, rect.left, rect.top, cx, cy - m_iAttachSize, SWP_NOACTIVATE);

			m_Windows.GetClientRect(&rect);
			rect.DeflateRect(0, 23, 0, 0);
			m_Windows.SetWindowPos(NULL, rect.left, rect.top, cx, cy - m_iAttachSize, SWP_NOACTIVATE);

			m_Service.GetClientRect(&rect);
			rect.DeflateRect(0, 23, 0, 0);
			m_Service.SetWindowPos(NULL, rect.left, rect.top, cx, cy - m_iAttachSize, SWP_NOACTIVATE);

			m_User.GetClientRect(&rect);
			rect.DeflateRect(0, 23, 0, 0);
			m_User.SetWindowPos(NULL, rect.left, rect.top, cx, cy - m_iAttachSize, SWP_NOACTIVATE);

		}
		break;
	case SIZE_MINIMIZED:
		if (m_bLittleHide)		//如果设置了最小化时隐藏
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_HIDEWINDOW);
		break;
	default:
		break;
	}
	

	
}



//Tab控件被单击
void CTaskmanagerDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CRect	rect;
	m_MyTab.GetClientRect(&rect);
	rect.DeflateRect(0, 23, 0, 0);

	switch (m_MyTab.GetCurSel())
	{
	case 0:
		m_Process.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
		m_Windows.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		m_Service.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		m_User.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		break;
	case 1:
		m_Process.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		m_Windows.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
		m_Service.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		m_User.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		break;
	case 2:
		m_Process.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		m_Windows.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		m_Service.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
		m_User.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		break;
	case 3:
		m_Process.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		m_Windows.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		m_Service.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_HIDEWINDOW);
		m_User.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
		break;
	default:
		break;
	}

}


void CTaskmanagerDlg::OnExit()
{
	// TODO: 在此添加命令处理程序代码
	EndDialog(0);
}


void CTaskmanagerDlg::OnNewExe()
{
	// TODO: 在此添加命令处理程序代码
	CNewexeDlg newExe;
	switch (newExe.DoModal())
	{
	case IDOK:
		if (WinExec(newExe.m_string, 0) <= 31)
			MessageBox(TEXT("运行失败"));
		break;
	case IDCANCEL:
		break;
	}
	
}


//更新菜单项
void CTaskmanagerDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	// TODO: 在此处添加消息处理程序代码

	if (!bSysMenu && (nIndex == 1))			//更新菜单项
	{
		pPopupMenu->CheckMenuItem(ID_32773, MF_BYCOMMAND | (m_bTop == 1 ? MF_CHECKED : MF_UNCHECKED));
		pPopupMenu->CheckMenuItem(ID_32774, MF_BYCOMMAND | (m_bLittle == 1 ? MF_CHECKED : MF_UNCHECKED));
		pPopupMenu->CheckMenuItem(ID_32775, MF_BYCOMMAND | (m_bLittleHide == 1 ? MF_CHECKED : MF_UNCHECKED));

		switch (m_dwFirstShow)
		{
		case 0:
			pPopupMenu->CheckMenuRadioItem(ID_32797, ID_32800, ID_32797, MF_BYCOMMAND);
			break;
		case 1:
			pPopupMenu->CheckMenuRadioItem(ID_32797, ID_32800, ID_32798, MF_BYCOMMAND);
			break;
		case 2:
			pPopupMenu->CheckMenuRadioItem(ID_32797, ID_32800, ID_32799, MF_BYCOMMAND);
			break;
		case 3:
			pPopupMenu->CheckMenuRadioItem(ID_32797, ID_32800, ID_32800, MF_BYCOMMAND);
			break;
		}

	}
	
}







void CTaskmanagerDlg::OnTop()
{
	// TODO: 在此添加命令处理程序代码
	m_bTop = !m_bTop;

	CWnd wnd;
	if (m_bTop)
		wnd.m_hWnd = HWND_TOPMOST;
	else
		wnd.m_hWnd = HWND_BOTTOM;
	SetWindowPos(&wnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


void CTaskmanagerDlg::OnLittle()
{
	// TODO: 在此添加命令处理程序代码
	m_bLittle = !m_bLittle;
}


void CTaskmanagerDlg::OnLittleHide()
{
	// TODO: 在此添加命令处理程序代码
	m_bLittleHide = !m_bLittleHide;
}


void CTaskmanagerDlg::OnNowRenewal()
{
	// TODO: 在此添加命令处理程序代码
	m_Process.SendMessage(WM_NOWRENEWAL);					//向进程窗口发送消息，立即更新
	m_Windows.SendMessage(WM_NOWRENEWAL);					//向窗口信息窗口发送消息，立即更新
	m_Service.SendMessage(WM_NOWRENEWAL);					//向服务窗口发送消息，立即更新
	m_User.SendMessage(WM_NOWRENEWAL);						//向用户窗口发送消息，立即更新
}


//设置到指定的服务的详细信息
LRESULT CTaskmanagerDlg::OnSetTopItem(WPARAM wParam, LPARAM lParam)
{
	m_MyTab.SetCurFocus(0);

	TCHAR szPid[MAX_PATH] = { 0 };
	lstrcpy(szPid, m_Service.m_List.GetItemText((int)lParam, 1));

	CSize size;
	CRect rect;
	for (int i = 0; i < m_Process.m_List.GetItemCount(); i++)
	{
		if (0 == lstrcmp(m_Process.m_List.GetItemText(i, 2), szPid))
		{
			m_Process.m_List.GetItemRect(0, &rect, LVIR_BOUNDS);

			size.cx = 0;
			size.cy = i * (rect.bottom - rect.top);
			m_Process.m_List.Scroll(size);
		}
	}
	
	return TRUE;
}

void CTaskmanagerDlg::OnSetProcess()
{
	// TODO: 在此添加命令处理程序代码
	m_dwFirstShow = 0;
}


void CTaskmanagerDlg::OnSetWindows()
{
	// TODO: 在此添加命令处理程序代码
	m_dwFirstShow = 1;
}


void CTaskmanagerDlg::OnSetService()
{
	// TODO: 在此添加命令处理程序代码
	m_dwFirstShow = 2;
}


void CTaskmanagerDlg::OnSetUser()
{
	// TODO: 在此添加命令处理程序代码
	m_dwFirstShow = 3;
}


