// LaserDeicingDlg.h : header file
//

#if !defined(AFX_LaserDeicingDLG_H__4C5D1B67_AC1D_499C_A88A_C0B663F739D0__INCLUDED_)
#define AFX_LaserDeicingDLG_H__4C5D1B67_AC1D_499C_A88A_C0B663F739D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeneralDef.h"
#include "PTZButton.h"//��̨������
#include "SetPtzPosDlg.h"//������̨��Ϣ��
#include "ImagePreprocessingDlg.h" //ͼ��Ԥ��������
#include "ImageEdgeExtractionDlg.h" //ͼ���Ե��ȡ������
#include "ImageSegmentationDlg.h" //ͼ��ָ����
#include "CvvImage.h" //OpenCVͷ�ļ�
#include "opencv2/opencv.hpp" //OpenCVͷ�ļ�
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CLaserDeicingDlg dialog

class CLaserDeicingDlg : public CDialog
{
// Construction
public:
	CLaserDeicingDlg(CWnd* pParent = NULL);	// standard constructor

public:
	BOOL m_bTrackRun;
	BOOL m_bIsOnCruise;
	void InitDecoderReferCtrl();
	void GetDecoderCfg();
	void StopRecord();
	void StartRecord();
	int GetPTZSpeed();
	LONG GetPlayHandle();
	int GetCurChanIndex();
	void DbPlayChannel(int ChanIndex);
	void CreateDeviceTree();
	void StartPlay(int iChanIndex);
	void StopPlay();
	BOOL DoLogin();
	void DoGetDeviceResoureCfg();
	BOOL m_bIsLogin;//���ע��/ע�����
//	LONG m_lLoginID;
	BOOL m_bIsPlaying;//ͨ�����ű�ʶ
	BOOL m_bIsRecording;//¼���ʶ
	HTREEITEM m_hDevItem;//��CTreeCtrl�ؼ������������ڸ����ؼ���ӡ���ѯ��ɾ����Ŀ
	LOCAL_DEVICE_INFO m_struDeviceInfo;//��¼�ӿڵķ���ֵ
	int m_iCurChanIndex; //��ǰͨ��������������
	LONG m_lPlayHandle;//NET_DVR_RealPlay_V30����ֵ��ʵʱԤ������ֵ
	



	//���а�ť��˳��һ��Ҫ��CPTZButton�еĴ���һ��
	CPTZButton m_btnPtzUp;
	CPTZButton m_btnPtzDown;
	CPTZButton m_btnPtzLeft;
	CPTZButton m_btnPtzRight;
	CPTZButton m_btnZoomIn;//��������,�Ŵ�
	CPTZButton m_btnZoomOut;//������Զ,��С
	CPTZButton m_btnFocusNear;//�۽�����,�Ŵ�
	CPTZButton m_btnFocusFar;//�۽���Զ,��С
	CPTZButton m_btnIrisOpen;//��Ȧ�Ŵ�
	CPTZButton m_btnIrisClose;//��Ȧ��С
	CPTZButton m_btnPtzUpleft; //����
	CPTZButton m_btnPtzUpright; //����
	CPTZButton m_btnPtzDownleft; //����
	CPTZButton m_btnPtzDownright; //����
// 	CPTZButton m_btnPtzAuto;      //�Զ�
// 	CPTZButton m_btnLightPwron;   //�ƹ�
// 	CPTZButton m_btnWiperPwron;   //��ˢ
// 	CPTZButton m_btnFanPwron;     //����
// 	CPTZButton m_btnHeaterPwron;  //����
// 	CPTZButton m_btnAuxPwron;     //����

	BOOL m_bAutoOn;       //�Զ���
    BOOL m_bLightOn;      //�ƹ⿪
    BOOL m_bWiperOn;      //��ˢ��
	BOOL m_bFanOn;        //���ȿ�
	BOOL m_bHeaterOn;     //���ȿ�
	BOOL m_bAuxOn1;        //�����豸1��
	BOOL m_bAuxOn2;        //�����豸2��
// Dialog Data
	//{{AFX_DATA(CLaserDeicingDlg)
	enum { IDD = IDD_LaserDeicing_DIALOG };
	CComboBox	m_coJpgQuality;
	CComboBox	m_coJpgSize;
	CComboBox	m_coPicType;//����ͼƬ�ĸ�ʽѡ�� bmp��jpg
	//CComboBox	m_comboSeq;//Ѳ���켣����ѡ��
	CComboBox	m_comboPreset;//Ԥ�õ�
	CComboBox	m_comboPTZSpeed;//��̨���ٶ�
	CTreeCtrl	m_ctrlTreeChan;//ͨ����
	CIPAddressCtrl	m_ctrlDevIp;//IP��ַ
	UINT	m_nDevPort;//�˿ں�
	CString	m_csUser;//�û���
	CString	m_csPWD;//����
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaserDeicingDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLaserDeicingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonLogin();
	afx_msg void OnDblclkTreeChan(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeChan(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonRecord();
	afx_msg void OnButtonCapture();
	afx_msg void OnButtonPlay();
	afx_msg void OnSelchangeComboPreset();
	afx_msg void OnButtonPresetGoto();
	afx_msg void OnButtonPresetSet();
	afx_msg void OnButtonPresetDel();
	//afx_msg void OnButtonSeqGoto();//����Ѳ��
	//afx_msg void OnButtonSeqSet();//Ѳ���켣����
	//afx_msg void OnButtonTrackRun();//���й켣��¼
	//afx_msg void OnButtonTrackStart();//��ʼ�켣��¼
	//afx_msg void OnButtonTrackStop();//ֹͣ�켣��¼
	afx_msg void OnBtnPtzAuto();
	//afx_msg void OnBtnAuxPwron();//�����豸1
	//afx_msg void OnBtnFanPwron();����
	//afx_msg void OnBtnHeaterPwron();//����
	//afx_msg void OnBtnLightPwron();//�ƹ����
	//afx_msg void OnBtnWiperPwron();//��ˢ
	//afx_msg void OnBtnAuxPwron2();//�����豸2
	afx_msg void OnSelchangeComboPicType();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();//����
	afx_msg void OnBnClickedImagePreprocessingButton();//ͼ��Ԥ����
	void DrawPicToHDC(cv::Mat& mat,UINT ID);//��ͼ����Ϣ��ʾ��picture�ؼ�����
	afx_msg void OnBnClickedImageSegmentationButton();//ͼ��ָ�
	afx_msg void OnBnClickedImageEdgeextractionButton();//��Ե��ȡ
	afx_msg void OnBnClickedOpenfileButton1();//��ͼ��
	afx_msg void OnBnClickedUpdateButton();//ˢ��
	afx_msg void OnBnClickedClinkOnButton();//�ֶ�ѡ��
public:
	//������ѡȡ���Ӵ��ڴ��������ڵ����ݼ�ͼ��������еĽ��ͼ��
	Mat src;//��ͼ���srcͼ��
	Mat dst;//��ͼ������dstͼ��
	Mat show_img;//��������������ʾ�����Ŀ���ͼ��
	Mat gray;
	Mat imageROI;//ѡȡ��ROIͼ��
	Mat COMPENSATION_image;//���ղ���ͼ��
	Mat equalizeImage;//ֱ��ͼ���⻯ͼ��
	Mat Image_Threshold;//�ֶ���ֵͼ��
	Mat Image_OstuThreshold; //���ɷ���ֵͼ��
	Mat Image_adaptiveThreshold; //����Ӧ��ֵͼ��
	Mat blurImage;//��ֵ�˲�
	Mat GaussianBlurImage; //��˹�˲�
	Mat MedianBlurImage; //��ֵ�˲�
	Mat BilateralFilterImage;//˫���˲�
	Mat CannyImage;//Canny��Ե���
	Mat SobelImage; //Sobel��Ե���
	Mat LaplaceImage; //Laplace��Ե���
	Mat ErodeImage; //��ʴͼ��
	Mat DilateImage;//����ͼ��

	//ͨ������ѡ�����ز�������
	boolean flag;//�ֶ�ѡ��ʱ��ѡȡ�����ı��
	Point2f p1;//�ֶ�ѡȡ��һ����
	Point2f p2;//�ֶ�ѡȡ�ڶ�����
	// ѡȡ�ĵ�һ���㣬��ʾ������
	CString m_strPoint1;
	// ѡȡ�ĵڶ����㣬��ʾ������
	CString m_strPoint2;
	// ���룬��ʾ������
	CString m_strDistance;
	//���λ�ã���ʾ������
	CString m_strPoint;
	
	// ״̬��Ϣ��
	CListBox m_info;
	afx_msg void OnBnClickedButtonInfoClc();
	afx_msg void OnBnClickedSetptzposButton();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LaserDeicingDLG_H__4C5D1B67_AC1D_499C_A88A_C0B663F739D0__INCLUDED_)
