#pragma once


// CConventionDlg

class CConventionDlg : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CConventionDlg)

public:
	CConventionDlg();
	virtual ~CConventionDlg();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CString m_szFilePath;

	CEdit m_CreateTime;
	CEdit m_ChangeTime;
	CEdit m_VisitTime;
	CEdit m_Size;
	CEdit m_Path;
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
};


