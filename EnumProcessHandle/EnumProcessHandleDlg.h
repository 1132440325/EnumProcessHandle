
// EnumProcessHandleDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CEnumProcessHandleDlg �Ի���
class CEnumProcessHandleDlg : public CDialogEx
{
// ����
public:
	CEnumProcessHandleDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENUMPROCESSHANDLE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	void SetProcessInfo();
	CListCtrl m_list1;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
