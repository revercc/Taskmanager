// CModuleDlg.cpp: 实现文件
//

#include "pch.h"
#include "Taskmanager.h"
#include "CModuleDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>

// CModuleDlg 对话框

IMPLEMENT_DYNAMIC(CModuleDlg, CDialogEx)

CModuleDlg::CModuleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CModuleDlg::~CModuleDlg()
{
}

void CModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CModuleDlg, CDialogEx)
END_MESSAGE_MAP()


// CModuleDlg 消息处理程序


BOOL CModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect	rect;							//初始化列表控件
	GetClientRect(&rect);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);
	m_List.InsertColumn(0, TEXT("名称"), LVCFMT_LEFT, rect.right / 4);
	m_List.InsertColumn(3, TEXT("路径"), LVCFMT_LEFT, (rect.right / 4) * 3);
	
	if (FALSE == _AddItem())				//添加列表项，如果失败就关闭对话框
		EndDialog(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CModuleDlg::_AddItem()
{
	LVITEM stLvitem;
	stLvitem.mask = LVIF_TEXT;

	MODULEENTRY32 stModuel32 = { 0 };
	stModuel32.dwSize = sizeof(MODULEENTRY32);
	HANDLE hModuleSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwPid);
	if (hModuleSnap == INVALID_HANDLE_VALUE)		//权限不足无法查看
	{
		MessageBox(TEXT("无法查看"));
		return FALSE;
	}

	Module32First(hModuleSnap, &stModuel32);
	int i = 0;
	do {
		stLvitem.iItem = i;
		stLvitem.iSubItem = 0;
		stLvitem.pszText = stModuel32.szModule;
		m_List.InsertItem(&stLvitem);
		
		stLvitem.iSubItem = 1;
		stLvitem.pszText = stModuel32.szExePath;
		m_List.SetItem(&stLvitem);

	} while (Module32Next(hModuleSnap, &stModuel32));
	::CloseHandle(hModuleSnap);
	return TRUE;
}