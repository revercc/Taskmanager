#pragma once


// CProcessDlg 对话框

class CProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcessDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	void _AddItem();									//添加列表项
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;									//列表控件对象
	int m_iTem;											//右键单击选中的项的索引

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnExitProcess();
	afx_msg void OnLookModule();
	afx_msg void OnLookThread();
	afx_msg void OnOpenFile();
	afx_msg void OnProperty();
	afx_msg LRESULT OnRenewal(WPARAM wParam, LPARAM lParam);
	
};
