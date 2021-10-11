#pragma once


// CWindowsDlg 对话框

class CWindowsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWindowsDlg)

public:
	CWindowsDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWindowsDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_Tree;						//树控件

	
	static BOOL CALLBACK EnumALLWindowsProc(HWND hwnd, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnAddItem(WPARAM wParam, LPARAM lParam);							//添加树表项	
	afx_msg LRESULT OnNowRenewal(WPARAM wParam, LPARAM lParam);							//立即更新
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestryWindow();
};
