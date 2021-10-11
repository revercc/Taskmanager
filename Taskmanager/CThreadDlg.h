#pragma once


// CThreadDlg 对话框

class CThreadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CThreadDlg)

public:
	CThreadDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CThreadDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwPid;						//进程PID
	DWORD m_iTem;						//被右键单击的列表索引
	CListCtrl m_List;					//列表控件
	virtual BOOL OnInitDialog();
	BOOL _AddItem();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExitThread();
	afx_msg void OnStopThread();
	afx_msg void OnBeginThread();
};
