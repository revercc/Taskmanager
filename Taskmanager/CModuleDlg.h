#pragma once


// CModuleDlg 对话框

class CModuleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModuleDlg)

public:
	CModuleDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CModuleDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DWORD dwPid;						//对应进程的PID
	CListCtrl m_List;					//列表控件

	virtual BOOL OnInitDialog();
	BOOL _AddItem();
};
