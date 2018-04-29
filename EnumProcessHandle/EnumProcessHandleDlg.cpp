
// EnumProcessHandleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EnumProcessHandle.h"
#include "EnumProcessHandleDlg.h"
#include "afxdialogex.h"
#include <MyDebugTools.hpp>
#include "Psapi.h"
#pragma comment (lib,"Psapi.lib")
#include "TlHelp32.h"
#include "EnumHandle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEnumProcessHandleDlg �Ի���



CEnumProcessHandleDlg::CEnumProcessHandleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ENUMPROCESSHANDLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEnumProcessHandleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST2, m_list1);
}

BEGIN_MESSAGE_MAP(CEnumProcessHandleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CEnumProcessHandleDlg::OnNMClickList1)
END_MESSAGE_MAP()


// CEnumProcessHandleDlg ��Ϣ�������

BOOL CEnumProcessHandleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	EnumHandle::Init();

	m_list.InsertColumn(0, L"PID", 0, 60);
	m_list.InsertColumn(1, L"Process name", 0, 160);
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	SetProcessInfo();

	m_list1.InsertColumn(0, L"Handle", 0, 100);
	m_list1.InsertColumn(1, L"Type", 0, 160);
	m_list1.InsertColumn(2, L"Process", 0, 160);
	m_list1.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEnumProcessHandleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEnumProcessHandleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEnumProcessHandleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEnumProcessHandleDlg::SetProcessInfo()
{
	HANDLE hProcessShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessShot == INVALID_HANDLE_VALUE)
		AfxMessageBox(L"ö�ٽ���ʧ�ܣ�");
	PROCESSENTRY32 procinfo;
	procinfo.dwSize = sizeof(PROCESSENTRY32);
	DWORD pid = 0;
	if (Process32First(hProcessShot, &procinfo)) {
		do {
			CString name(procinfo.szExeFile);
			CString pid;pid.Format(L"%d", procinfo.th32ProcessID);
			auto dwLid = m_list.InsertItem(0, pid, 0);
			m_list.SetItemText(dwLid, 1, name);
			m_list.SetItemData(dwLid, (DWORD_PTR)procinfo.th32ProcessID);
		} while (Process32Next(hProcessShot, &procinfo));
	}
}


void CEnumProcessHandleDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	int index = m_list.GetSelectionMark();
	if (index == -1) {
		*pResult = 0;
		return;
	}

	DWORD pid = m_list.GetItemData(index);
	EnumHandle::Show(pid,&m_list1);
}
