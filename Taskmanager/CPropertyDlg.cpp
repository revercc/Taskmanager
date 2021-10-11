// CPropertyDlg.cpp: 实现文件
//

#include "pch.h"
#include "Taskmanager.h"
#include "CPropertyDlg.h"


// CPropertyDlg

IMPLEMENT_DYNAMIC(CPropertyDlg, CPropertySheet)

CPropertyDlg::CPropertyDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CPropertyDlg::CPropertyDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_ConventionPage);						//添加常规属性表页
}

CPropertyDlg::~CPropertyDlg()
{
}


BEGIN_MESSAGE_MAP(CPropertyDlg, CPropertySheet)
END_MESSAGE_MAP()


// CPropertyDlg 消息处理程序
