// CWindowsDlg.cpp: 实现文件
//

#include "pch.h"
#include "Taskmanager.h"
#include "CWindowsDlg.h"
#include "afxdialogex.h"


// CWindowsDlg 对话框

IMPLEMENT_DYNAMIC(CWindowsDlg, CDialogEx)

CWindowsDlg::CWindowsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CWindowsDlg::~CWindowsDlg()
{
}

void CWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CWindowsDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_MESSAGE(WM_ADDWINDOWITEM, OnAddItem)
	ON_MESSAGE(WM_NOWRENEWAL, OnNowRenewal)

	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CWindowsDlg::OnRclickTree1)
	ON_COMMAND(ID_32791, &CWindowsDlg::OnDestryWindow)
END_MESSAGE_MAP()


// CWindowsDlg 消息处理程序


void CWindowsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()

	CRect   rect;	   //改变对话框的背景为白色
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
}


void CWindowsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_Tree.m_hWnd)
	{
		CRect	rect;
		GetClientRect(&rect);
		m_Tree.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
	}

}


//枚举窗口回调函数
BOOL CALLBACK CWindowsDlg::EnumALLWindowsProc(HWND hwnd, LPARAM lParam)
{

	
	
	if (::IsWindowEnabled(hwnd) && ::IsWindowVisible(hwnd))
	{
		::SendMessage((HWND)lParam, WM_ADDWINDOWITEM, 0, (LPARAM)hwnd);	//发送消息，显示窗口信息
	}
	return TRUE;
}


BOOL CWindowsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	EnumWindows(EnumALLWindowsProc, (LPARAM)m_hWnd);					//枚举窗口

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//添加窗口信息项
LRESULT CWindowsDlg::OnAddItem(WPARAM wParam, LPARAM lParam)
{

	
	TCHAR szWindowText[MAX_PATH] = {0};
	
	::GetWindowText((HWND)lParam, szWindowText, MAX_PATH);
	HTREEITEM hTreeitem;
	if (lstrlen(szWindowText))
	{
		hTreeitem = m_Tree.InsertItem(szWindowText, TVI_ROOT, TVI_SORT);
		wsprintf(szWindowText, TEXT("hwnd：0x%x"), lParam);
		m_Tree.InsertItem(szWindowText, hTreeitem);
	}
	return TRUE;
}




void CWindowsDlg::OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CMenu subMenu;
	subMenu.LoadMenu(IDR_MENU4);


	CPoint   point;
	GetCursorPos(&point);
	subMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
}


void CWindowsDlg::OnDestryWindow()
{
	// TODO: 在此添加命令处理程序代码
	HWND hwnd = 0;
	CString szHwnd = m_Tree.GetItemText(m_Tree.GetChildItem(m_Tree.GetSelectedItem()));
	m_Tree.DeleteItem(m_Tree.GetSelectedItem());
	
	CString string = (szHwnd.GetBuffer() + 6);
	StrToIntEx(string,STIF_SUPPORT_HEX, (int *)&hwnd);
	::PostMessage(hwnd, WM_QUIT, NULL, NULL);								//向对应窗口发送WM_QUIT消息
}	


LRESULT CWindowsDlg::OnNowRenewal(WPARAM wParam, LPARAM lParam)
{
	m_Tree.DeleteAllItems();
	EnumWindows(EnumALLWindowsProc, (LPARAM)m_hWnd);
	return TRUE;
}