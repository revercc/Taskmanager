// CNewexeDlg.cpp: 实现文件
//

#include "pch.h"
#include "Taskmanager.h"
#include "CNewexeDlg.h"
#include "afxdialogex.h"


// CNewexeDlg 对话框

IMPLEMENT_DYNAMIC(CNewexeDlg, CDialogEx)

CNewexeDlg::CNewexeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

CNewexeDlg::~CNewexeDlg()
{
}

void CNewexeDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_EDIT1, m_string);
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewexeDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewexeDlg 消息处理程序
