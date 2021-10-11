// CUserDlg.cpp: 实现文件
//

#include "pch.h"
#include "Taskmanager.h"
#include "CUserDlg.h"
#include "afxdialogex.h"


// CUserDlg 对话框

IMPLEMENT_DYNAMIC(CUserDlg, CDialogEx)

CUserDlg::CUserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG8, pParent)
{

}

CUserDlg::~CUserDlg()
{
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CUserDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_NOWRENEWAL, OnNowRenewal)
END_MESSAGE_MAP()


// CUserDlg 消息处理程序


BOOL CUserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	CRect	rect;							//初始化列表控件
	GetClientRect(&rect);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_List.InsertColumn(0, TEXT("名称"), LVCFMT_LEFT, rect.right / 4);
	m_List.InsertColumn(1, TEXT("密码"), LVCFMT_LEFT, rect.right / 4);
	m_List.InsertColumn(2, TEXT("权限"), LVCFMT_LEFT, rect.right / 6);
	m_List.InsertColumn(3, TEXT("描述信息"), LVCFMT_LEFT, rect.right / 3);

	_AddItem();								//添加列表项

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CUserDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	if (m_List.m_hWnd)
	{
		CRect	rect;
		GetClientRect(&rect);

		m_List.SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
		m_List.SetColumnWidth(0, rect.right / 4);
		m_List.SetColumnWidth(1, rect.right / 4);
		m_List.SetColumnWidth(2, rect.right / 6);
		m_List.SetColumnWidth(3, rect.right / 3);
	}
}


void CUserDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()

	CRect   rect;	   //改变对话框的背景为白色
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
}


void CUserDlg::_AddItem()
{
	LVITEM stLvitem;
	stLvitem.mask = LVIF_TEXT;


	USER_INFO_1* szData = NULL;	
	USER_INFO_1* szOldDataAddress = NULL;
	DWORD dwNum;
	DWORD dwTalentries;
	DWORD dwResumeHandle = 0;
	::NetUserEnum(NULL, 1, 0, (LPBYTE*)&szData, MAX_PREFERRED_LENGTH, &dwNum, &dwTalentries, &dwResumeHandle);

	szOldDataAddress = szData;
	TCHAR szBuffer[MAX_PATH];
	for (int i = 0; i < dwNum; i++)
	{
		SHUnicodeToAnsi((szData)->usri1_name, (PSTR)szBuffer, MAX_PATH);
		stLvitem.iItem = i;
		stLvitem.iSubItem = 0;
		stLvitem.pszText = (LPSTR)szBuffer;
		m_List.InsertItem(&stLvitem);

		SHUnicodeToAnsi(szData->usri1_password, (PSTR)szBuffer, MAX_PATH);
		stLvitem.iSubItem = 1;
		stLvitem.pszText = (LPSTR)szBuffer;
		m_List.SetItem(&stLvitem);


		switch (szData->usri1_priv)
		{
		case USER_PRIV_GUEST:
			stLvitem.pszText = TEXT("来宾");
			break;
		case USER_PRIV_USER:
			stLvitem.pszText = TEXT("用户");
			break;
		case USER_PRIV_ADMIN:
			stLvitem.pszText = TEXT("管理员");
			break;
		}
		stLvitem.iSubItem = 2;
		m_List.SetItem(&stLvitem);


		SHUnicodeToAnsi(szData->usri1_comment, (PSTR)szBuffer, MAX_PATH);
		stLvitem.iSubItem = 3;
		stLvitem.pszText = (LPSTR)szBuffer;
		m_List.SetItem(&stLvitem);
		
		szData = (USER_INFO_1*)((LPBYTE)szData + sizeof(USER_INFO_1));
	}

	NetApiBufferFree(szOldDataAddress);
}

LRESULT CUserDlg::OnNowRenewal(WPARAM wParam, LPARAM lParam)
{
	m_List.DeleteAllItems();
	_AddItem();
	return TRUE;
}