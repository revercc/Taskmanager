﻿#pragma once


// CNewexeDlg 对话框

class CNewexeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewexeDlg)

public:
	CNewexeDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CNewexeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	CString m_string;
};
