#pragma once
#include "afxwin.h"


// SetPtzPosDlg 对话框

class SetPtzPosDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetPtzPosDlg)

public:
	SetPtzPosDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetPtzPosDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SetPtzPos };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetptzpos();
	// 操作类型，仅在设置时有效。1-定位PTZ参数，2-定位P参数，3-定位T参数，4-定位Z参数，5-定位PT参数 
	CComboBox m_combo1;
	// 水平参数
	float m_fPara1;
	// 垂直方向
	float m_fPara2;
	// 变倍参数
	float m_fPara3;

	int m_iPara1;
	int m_iPara2;
	int m_iPara3;
	//云台位置信息结构体
	NET_DVR_PTZPOS m_ptzPos;
	virtual BOOL OnInitDialog();
};
