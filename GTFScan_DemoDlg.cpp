// GTFScan_DemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include <Windows.h>
#include <io.h>
#include <direct.h>
#include <process.h>

#include <afxtempl.h>
#include "GTFScan_Demo.h"
#include "GTFScan_DemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		SCANDATA_SIZE		100
#define		SCAN_TIMEOUT		10

#define YYYYMMDD                 1
#define YYYY_MM_DD_hh_mm_ss      2
#define YYYYMMDDhhmmss			 3
#define YYYY_MM_DD_hh_mm_ss_SSS	 4


extern "C" __declspec(dllimport) int __stdcall OpenPort();
extern "C" __declspec(dllimport) int __stdcall ClosePort();
extern "C" __declspec(dllimport) int __stdcall Scan();
extern "C" __declspec(dllimport) int __stdcall ScanCancel();
extern "C" __declspec(dllimport) int __stdcall ReceiveData(int TimeOut);
extern "C" __declspec(dllimport) int __stdcall GetPassportInfo(char *refPassInfo);
extern "C" __declspec(dllimport) int __stdcall OpenPortByNum(int PorNum);
extern "C" __declspec(dllimport) int __stdcall Clear();

extern "C" __declspec(dllimport) int __stdcall GetMRZ1(char *refMRZ1);
extern "C" __declspec(dllimport) int __stdcall GetMRZ2(char *refMRZ2);


int PrintLog(const char *fmt, ...);
void GetCurDtTm(char *targetbuf, int type);
BOOL isNumeric(CString str);
/////////////////////////////////////////////////////////////////////////////
// CGTFScan_DemoDlg dialog

CGTFScan_DemoDlg::CGTFScan_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGTFScan_DemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGTFScan_DemoDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGTFScan_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGTFScan_DemoDlg)
	DDX_Control(pDX, IDC_EDIT1, m_EditReCnt);

	DDX_Control(pDX, IDC_LIST1, m_Box);
	DDX_Control(pDX, IDC_COMBO2, m_cmbPortNo);
	DDX_Control(pDX, IDC_COMBO1, m_cmbScan);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGTFScan_DemoDlg, CDialog)
	//{{AFX_MSG_MAP(CGTFScan_DemoDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnDataScan)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGTFScan_DemoDlg message handlers

BOOL CGTFScan_DemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	Init_Control();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGTFScan_DemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGTFScan_DemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGTFScan_DemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	Clear();						//����� disconnect �ؾ���. (DAWIN)
	
}



BOOL CGTFScan_DemoDlg::Init_Control()
{
	// �޺��ڽ� �ʱ�ȭ

	m_cmbScan.AddString(_T("GTF ��ĳ��"));
	m_cmbScan.AddString(_T("wisescan 420 ��ĳ��"));
	m_cmbScan.AddString(_T("DAWIN ��ĳ��"));
	m_cmbScan.AddString(_T("OKPOS ��ĳ��"));

	m_cmbPortNo.AddString(_T("COM1"));
	m_cmbPortNo.AddString(_T("COM2"));
	m_cmbPortNo.AddString(_T("COM3"));
	m_cmbPortNo.AddString(_T("COM4"));
	m_cmbPortNo.AddString(_T("COM5"));
	m_cmbPortNo.AddString(_T("COM6"));
	m_cmbPortNo.AddString(_T("COM7"));
	m_cmbPortNo.AddString(_T("COM8"));
	m_cmbPortNo.AddString(_T("COM9"));
	m_cmbPortNo.AddString(_T("COM10"));
	m_cmbPortNo.AddString(_T("COM11"));
	m_cmbPortNo.AddString(_T("COM12"));
	m_cmbPortNo.AddString(_T("COM13"));
	m_cmbPortNo.AddString(_T("COM14"));
	m_cmbPortNo.AddString(_T("COM15"));
	m_cmbPortNo.AddString(_T("COM16"));
	m_cmbPortNo.AddString(_T("COM17"));
	m_cmbPortNo.AddString(_T("COM18"));
	m_cmbPortNo.AddString(_T("COM19"));
	m_cmbPortNo.AddString(_T("COM20"));
	m_cmbPortNo.AddString(_T("COM21"));
	m_cmbPortNo.AddString(_T("COM22"));
	m_cmbPortNo.AddString(_T("COM23"));
	m_cmbPortNo.AddString(_T("COM24"));
	m_cmbPortNo.AddString(_T("COM25"));
	m_cmbPortNo.AddString(_T("COM26"));
	m_cmbPortNo.AddString(_T("COM27"));
	m_cmbPortNo.AddString(_T("COM28"));
	m_cmbPortNo.AddString(_T("COM29"));
	m_cmbPortNo.AddString(_T("COM30"));
	m_cmbPortNo.AddString(_T("AUTO"));

	
	/*
	CString strCnt = "";
	for(int i=1 ; i <= 50 ; i ++)
	{
		strCnt.Format("%d", i);
		m_cmtRetryCnt.AddString(_T(strCnt));
	}*/

	//m_cmtRetryCnt.SetCurSel(0);
	m_EditReCnt.SetWindowText("1");
	// ȯ�� �ʱⰪ ��� (��ĳ�� ����, Port �ʱ� ��ġ)
	Ini_Set(0);

	return TRUE;
}


// ��ĵ ó���� ������ ���
void CGTFScan_DemoDlg::OnDataScan() 
{
	((CListBox*)GetDlgItem(IDC_LIST1))->ResetContent();
	((CListBox*)GetDlgItem(IDC_LIST1))->InsertString(0, (LPTSTR)"");
	// ��ĳ���� ��ĳ�� ���� ���� (Ini ����)
	Ini_Set(1);

	// Scan ó���� �ϰ� �����͸� ����Ѵ�
	long nRet = 0L;

	// ��ĳ�� �����Ϳ�
	char PassportInfo[SCANDATA_SIZE+1];
	memset(PassportInfo, 0x00, sizeof(PassportInfo));

	char mrz1[SCANDATA_SIZE+1];
	char mrz2[SCANDATA_SIZE+1];
	

	//2017.03.17 ��õ� ��ĵ Ƚ���� ���� ���ǽ�ĵ �õ�
	//int nRetryCnt = m_cmtRetryCnt.GetCurSel() + 1;
	CString strInput; 
	m_EditReCnt.GetWindowText(strInput);
	if(!isNumeric(strInput))
	{
		AfxMessageBox("��ĵ Ƚ���� ��Ȯ�� �Է��� �ּ���!");
		return;
	}
	int nRetryCnt = _ttoi(strInput);
	strInput.Empty();
	CString strTempLog ;
	strTempLog.Empty();

	CMap<CString, LPCSTR, int, int> mapMrz1;
	CMap<CString, LPCSTR, int, int> mapMrz2;
	//std::map<CString, CString> mapMrz1;
	//std::map<CString, CString> mapMrz2;
	PrintLog("###########################################################################\n");
	for(int i=0; i < nRetryCnt ; i ++)
	{
		memset(mrz1, 0x00, sizeof(mrz1));
		memset(mrz2, 0x00, sizeof(mrz2));
		// Scanner Open
		int nPortNo = m_cmbPortNo.GetCurSel() + 1;
			
		if(nPortNo > 30)
			nRet = OpenPort(); //OPEN �ڵ����� ����
		else
			nRet = OpenPortByNum(nPortNo);

		strTempLog.Format("Retry Count:%d\n", (i+1));
		PrintLog(strTempLog);

	//	nRet = OpenPort();
		if(nRet)
		{
			// Scan �϶�� command�� �����Ѵ�
			nRet = Scan();
			if(nRet)
			{
				// Scan �� ������� ����Ѵ�. (��ĵ ������)
				nRet = ReceiveData(SCAN_TIMEOUT);			// Time-Out �ð� ����(��)
				if(nRet > 0)								// ����ó��
				{
					// GTF ������� �������� ������ ���ġ
					GetPassportInfo(PassportInfo);

					((CListBox*)GetDlgItem(IDC_LIST1))->ResetContent();
					((CListBox*)GetDlgItem(IDC_LIST1))->InsertString(0, (LPTSTR)PassportInfo);

					GetMRZ1(mrz1);
					GetMRZ2(mrz2);
					nRet = 0;
					
					//strTempLog.Format("PassData:%s\n", &PassportInfo);

					strTempLog.Format("mrz1:%s\n", &mrz1);
					PrintLog(strTempLog);
					strTempLog.Format("mrz2:%s\n", &mrz2);
					PrintLog(strTempLog);
					CString strMrz1 = (CString)mrz1;
					CString strMrz2 = (CString)mrz2;
					int nMrz1Cnt = 0;
					int nMrz2Cnt = 0;

					if ( mapMrz1.Lookup(strMrz1, nMrz1Cnt) )
						mapMrz1.SetAt(strMrz1, nMrz1Cnt+1);
					else
						mapMrz1.SetAt(strMrz1, 1);

					if ( mapMrz2.Lookup(strMrz2, nMrz2Cnt) )
						mapMrz2.SetAt(strMrz2, nMrz2Cnt+1);
					else
						mapMrz2.SetAt(strMrz2, 1);

					strMrz1.Empty();
					strMrz2.Empty();
					strTempLog.Empty();
				}
				else if(nRet == 0)
				{
					nRet = 1;						// TIEMEOUT
					((CListBox*)GetDlgItem(IDC_LIST1))->ResetContent();
				}else if(nRet < 0)
				{
	// nRet �� �״�� ������ ��
	//				nRet = -1;						// ������ �־��ּ���
					((CListBox*)GetDlgItem(IDC_LIST1))->ResetContent();
				}
			}
			ClosePort();
		}else{
			nRet = 2;								// ���̺� ���ῡ��
		}
		Sleep(1000);
	}

	PrintLog("###########################################################################\n");
	CString strKey;
	int nValue;
    POSITION pos = NULL;

	if(mapMrz1.GetCount() > 0)
	{
	
		strTempLog.Format("MRZ1 Pattern Count:%d\n",mapMrz1.GetCount());
		PrintLog(strTempLog);

		pos = mapMrz1.GetStartPosition();
 
		while ( pos != NULL )
		{
			mapMrz1.GetNextAssoc(pos, strKey, nValue);
			strTempLog.Format("data:%s , count:%d\n",strKey ,nValue);
			PrintLog(strTempLog);

		}
	}
	
	if(mapMrz2.GetCount() > 0)
	{
		strTempLog.Format("MRZ2 Pattern Count:%d\n",mapMrz2.GetCount());
		PrintLog(strTempLog);

		pos = mapMrz2.GetStartPosition();
 
		while ( pos != NULL )
		{
			mapMrz2.GetNextAssoc(pos, strKey, nValue);
			strTempLog.Format("data:%s , count:%d\n",strKey ,nValue);
			PrintLog(strTempLog);

		}
	}
	PrintLog("###########################################################################\n");

	strKey.Empty();
	strTempLog.Empty();
	mapMrz1.RemoveAll();
	mapMrz2.RemoveAll();

	Disp_ErrorMessage(nRet);
}

// ���� ���� �޽��� ǥ��
void CGTFScan_DemoDlg::Disp_ErrorMessage(long nErrNo)
{
	CString  strErrMsg;

	switch(nErrNo)
	{
	/////////////////////////////////////////////
	// ���� �޽��� ����
	/////////////////////////////////////////////
	case 1:
		strErrMsg = _T("Time Out");
		break;
	case 2:
		strErrMsg = _T("���̺� ���ῡ��");
		break;
	/////////////////////////////////////////////
	case -1:
		strErrMsg = _T("������ �־��ּ���");
		break;

	/////////////////////////////////////////////
	// wisecube ���� ����
	/////////////////////////////////////////////
	case -10:
		strErrMsg = _T("E 01:�Ϲ����� �����ν� ����");
		break;
	case -56:
		strErrMsg = _T("��� ������ �̻�");
		break;
	case -54:
		strErrMsg = _T("�� ���� ���");
		break;
	case -39:
		strErrMsg = _T("CIS ���� �̼� ����ġ �ν� �ҷ�");
		break;
	case -4:
		strErrMsg = _T("CIS ���� �̵��� ��� �߻�");
		break;

	}

	if(nErrNo)
	{
		::MessageBox(NULL, strErrMsg, _T("�����νĶ��̺귯��"), MB_OK | MB_SYSTEMMODAL | MB_ICONERROR);
	}
}

void CGTFScan_DemoDlg::Ini_Set(int nRW)
{
	CString strFileName, strCode;
	strFileName = _T("\\GTF_SET.ini");

	char	szBuf[128];
	int nRet;


	CString strTempDir = _T("C:\\GTF_PASSPORT");		// ������ ���� ���

	if(access(strTempDir, 0) != 0)				// ������ �ִ��� Ȯ��
	{ 
		CreateDirectory(strTempDir, NULL);
	}
	
	TCHAR szCurPath[MAX_PATH];
	memset(szCurPath, 0x00, sizeof(szCurPath));

//	GetCurrentDirectory(MAX_PATH, szCurPath);

	strcpy(szCurPath, (LPCSTR)strTempDir);
	strcat(szCurPath, (LPCSTR)strFileName);		// C:\\GTF_PASSPORT\\GTF_SET.ini   (������)
	
	if(nRW == 0)						// �ʱⰪ �б� ó��
	{
		memset(szBuf, 0, 128);
		nRet = GetPrivateProfileInt ("ENV", "SCANNER", 0, szCurPath);			// ��ĳ�� ����
		m_cmbScan.SetCurSel(nRet);
		nRet = GetPrivateProfileInt ("ENV", "PORT_NO", 4, szCurPath);			// COM Port No
		m_cmbPortNo.SetCurSel(nRet-1);
	}
	else								// ���� ���� ó��
	{
		strCode.Format("%d", m_cmbScan.GetCurSel());					// ��ĳ�� ����
		WritePrivateProfileString("ENV", "SCANNER", strCode, szCurPath);

		strCode.Format("%d", (m_cmbPortNo.GetCurSel()+1));					// COM Port No
		WritePrivateProfileString("ENV", "PORT_NO", strCode, szCurPath);
	}
}


void CGTFScan_DemoDlg::OnEditchangeCombo3() 
{
	// TODO: Add your control notification handler code here
	
}

int PrintLog(const char *fmt, ...)
{
	va_list	ap;
	char curdttm[32];
	FILE *fp;
	char logfilename[100] = {0x00 , };
		
	memset(curdttm, 0x00, sizeof(curdttm)); 
	GetCurDtTm(curdttm, YYYYMMDD);
	_mkdir("..\\gtf_log");

	sprintf(logfilename, "c:\\GTF_PASSPORT\\log\\gtf%s.log", curdttm);
		
	fp = fopen(logfilename, "a+");
	if(fp == NULL) return -1;
		
	memset(curdttm, 0x00, sizeof(curdttm)); GetCurDtTm(curdttm, YYYY_MM_DD_hh_mm_ss_SSS);
	fprintf(fp, "[%s][%d] �� ", curdttm, getpid());
		
	va_start(ap, fmt);
	vfprintf(fp, fmt, ap);
	va_end(ap);
		
	fflush(fp);
	fclose(fp);
	return TRUE;
}
void GetCurDtTm(char *targetbuf, int type)
{
	time_t tmt;
	struct tm *calptr;

	time(&tmt);
	calptr = (struct tm *)localtime(&tmt);

	SYSTEMTIME st;
	GetLocalTime(&st);

	switch(type)
	{	
		case YYYYMMDD:
			strftime(targetbuf, 16, "%Y%m%d", calptr);
			break;
		case YYYY_MM_DD_hh_mm_ss:
			strftime(targetbuf, 32, "%Y/%m/%d %H:%M:%S", calptr);
			break;	
		case YYYYMMDDhhmmss:
			strftime(targetbuf, 32, "%Y%m%d%H%M%S", calptr);
			break;	
		case YYYY_MM_DD_hh_mm_ss_SSS:
			sprintf(targetbuf,"%04d/%02d/%02d %02d:%02d:%02d.%03d",st.wYear, st.wMonth,st.wDay, st.wHour, st.wMinute, st.wSecond , st.wMilliseconds);
			break;	
			
	}
	return;
}

BOOL isNumeric(CString str)
{
    if(str.SpanIncluding("0123456789") == str)
        return TRUE;
    else
        return FALSE;
}
