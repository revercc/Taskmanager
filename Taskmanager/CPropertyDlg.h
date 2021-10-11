#pragma once
#include "CConventionDlg.h"


// CPropertyDlg

class CPropertyDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropertyDlg)

public:
	CPropertyDlg(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	CPropertyDlg(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	virtual ~CPropertyDlg();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CConventionDlg m_ConventionPage;				//常规属性表页
};


