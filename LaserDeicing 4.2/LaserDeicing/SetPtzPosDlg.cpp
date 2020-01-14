// SetPtzPosDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LaserDeicing.h"
#include "SetPtzPosDlg.h"
#include "afxdialogex.h"


// SetPtzPosDlg �Ի���

IMPLEMENT_DYNAMIC(SetPtzPosDlg, CDialogEx)

SetPtzPosDlg::SetPtzPosDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetPtzPosDlg::IDD, pParent)
	, m_fPara1(0)
	, m_fPara2(0)
	, m_fPara3(0)
{

}

SetPtzPosDlg::~SetPtzPosDlg()
{
}

void SetPtzPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PARA1, m_combo1);
	DDX_Text(pDX, IDC_EDIT_PARA1, m_fPara1);
	DDX_Text(pDX, IDC_EDIT_PARA2, m_fPara2);
	DDX_Text(pDX, IDC_EDIT_PARA3, m_fPara3);
}


BEGIN_MESSAGE_MAP(SetPtzPosDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SetPtzPos, &SetPtzPosDlg::OnBnClickedSetptzpos)
END_MESSAGE_MAP()


// SetPtzPosDlg ��Ϣ�������
//ʮ������ת����ʮ����
WORD HexToDec1(WORD wHex)
{
	return (wHex / 4096) * 1000 + ((wHex % 4096) / 256) * 100 + ((wHex % 256) / 16) * 10 + (wHex % 16);
}

//ʮ����ת����ʮ������
WORD DecToHex(WORD wDec)
{
	return (wDec / 1000) * 4096 + ((wDec % 1000) / 100) * 256 + ((wDec % 100) / 10) * 16 + (wDec % 10);
}
//������̨λ����Ϣ
void SetPtzPosDlg::OnBnClickedSetptzpos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//������̨λ��
	UpdateData(TRUE);
	BOOL bRet1 = FALSE;
	m_ptzPos.wAction = m_combo1.GetCurSel();
	m_iPara1 = (int)(m_fPara1 * 10);
	m_iPara2 = (int)(m_fPara2 * 10);
	m_iPara3 = (int)(m_fPara3 * 10);
	
	m_ptzPos.wPanPos = DecToHex(m_iPara1);
	m_ptzPos.wTiltPos = DecToHex(m_iPara2);
	m_ptzPos.wZoomPos = DecToHex(m_iPara3);
	bRet1 = NET_DVR_SetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_SET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS));
	if (!bRet1)
	{
		MessageBox("����ʧ��");
		return;
	}
	Sleep(1000);
	//��ȡ��ǰ��λ����Ϣ
	NET_DVR_PTZPOS m_ptzPosNow1 = { 0 };
	DWORD dwReturnedNow1;
	BOOL bRet2 = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPosNow1, sizeof(NET_DVR_PTZPOS), &dwReturnedNow1);
	int m_iPara1Now1 = HexToDec1(m_ptzPosNow1.wPanPos);//ʮ������ת����ʮ����
	int m_iPara2Now1 = HexToDec1(m_ptzPosNow1.wTiltPos);
	int m_iPara3Now1 = HexToDec1(m_ptzPosNow1.wZoomPos);
	CString info2 = "";
	info2.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1Now1*0.1,(float)m_iPara2Now1*0.1,(float)m_iPara3Now1*0.1);
	g_pMainDlg->m_info.AddString(info2);
}


BOOL SetPtzPosDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_combo1.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
