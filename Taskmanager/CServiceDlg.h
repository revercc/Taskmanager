#pragma once


// CServiceDlg 对话框

class CServiceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServiceDlg)

public:
	CServiceDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CServiceDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iTem;							//右击列表项的索引
	HWND m_hParent;						//父窗口句柄

	CListCtrl m_List;					//列表控件
	void _AddItem();					//添加列表项
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg LRESULT OnNowRenewal(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStartService();
	afx_msg void OnStopService();
	afx_msg void OnReService();
	afx_msg void OnOpenService();
	afx_msg void OnToParticular();
};
