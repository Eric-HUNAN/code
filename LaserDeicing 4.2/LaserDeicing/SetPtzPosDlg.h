#pragma once
#include "afxwin.h"


// SetPtzPosDlg �Ի���

class SetPtzPosDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetPtzPosDlg)

public:
	SetPtzPosDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetPtzPosDlg();

// �Ի�������
	enum { IDD = IDD_DLG_SetPtzPos };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetptzpos();
	// �������ͣ���������ʱ��Ч��1-��λPTZ������2-��λP������3-��λT������4-��λZ������5-��λPT���� 
	CComboBox m_combo1;
	// ˮƽ����
	float m_fPara1;
	// ��ֱ����
	float m_fPara2;
	// �䱶����
	float m_fPara3;

	int m_iPara1;
	int m_iPara2;
	int m_iPara3;
	//��̨λ����Ϣ�ṹ��
	NET_DVR_PTZPOS m_ptzPos;
	virtual BOOL OnInitDialog();
};
