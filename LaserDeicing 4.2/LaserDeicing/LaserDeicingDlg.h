// LaserDeicingDlg.h : header file
//

#if !defined(AFX_LaserDeicingDLG_H__4C5D1B67_AC1D_499C_A88A_C0B663F739D0__INCLUDED_)
#define AFX_LaserDeicingDLG_H__4C5D1B67_AC1D_499C_A88A_C0B663F739D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeneralDef.h"
#include "PTZButton.h"//云台控制类
#include "SetPtzPosDlg.h"//设置云台信息类
#include "ImagePreprocessingDlg.h" //图像预处理窗口类
#include "ImageEdgeExtractionDlg.h" //图像边缘提取窗口类
#include "ImageSegmentationDlg.h" //图像分割窗口类
#include "CvvImage.h" //OpenCV头文件
#include "opencv2/opencv.hpp" //OpenCV头文件
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
	BOOL m_bIsLogin;//相机注册/注销标记
//	LONG m_lLoginID;
	BOOL m_bIsPlaying;//通道播放标识
	BOOL m_bIsRecording;//录像标识
	HTREEITEM m_hDevItem;//是CTreeCtrl控件的项句柄，用于给树控件添加、查询、删除项目
	LOCAL_DEVICE_INFO m_struDeviceInfo;//登录接口的返回值
	int m_iCurChanIndex; //当前通道在数组中索引
	LONG m_lPlayHandle;//NET_DVR_RealPlay_V30返回值，实时预览返回值
	



	//下列按钮的顺序一定要和CPTZButton中的处理一致
	CPTZButton m_btnPtzUp;
	CPTZButton m_btnPtzDown;
	CPTZButton m_btnPtzLeft;
	CPTZButton m_btnPtzRight;
	CPTZButton m_btnZoomIn;//调焦拉近,放大
	CPTZButton m_btnZoomOut;//调焦拉远,缩小
	CPTZButton m_btnFocusNear;//聚焦拉近,放大
	CPTZButton m_btnFocusFar;//聚焦拉远,缩小
	CPTZButton m_btnIrisOpen;//光圈放大
	CPTZButton m_btnIrisClose;//光圈缩小
	CPTZButton m_btnPtzUpleft; //左上
	CPTZButton m_btnPtzUpright; //右上
	CPTZButton m_btnPtzDownleft; //左下
	CPTZButton m_btnPtzDownright; //右下
// 	CPTZButton m_btnPtzAuto;      //自动
// 	CPTZButton m_btnLightPwron;   //灯光
// 	CPTZButton m_btnWiperPwron;   //雨刷
// 	CPTZButton m_btnFanPwron;     //风扇
// 	CPTZButton m_btnHeaterPwron;  //加热
// 	CPTZButton m_btnAuxPwron;     //辅助

	BOOL m_bAutoOn;       //自动开
    BOOL m_bLightOn;      //灯光开
    BOOL m_bWiperOn;      //雨刷开
	BOOL m_bFanOn;        //风扇开
	BOOL m_bHeaterOn;     //加热开
	BOOL m_bAuxOn1;        //辅助设备1开
	BOOL m_bAuxOn2;        //辅助设备2开
// Dialog Data
	//{{AFX_DATA(CLaserDeicingDlg)
	enum { IDD = IDD_LaserDeicing_DIALOG };
	CComboBox	m_coJpgQuality;
	CComboBox	m_coJpgSize;
	CComboBox	m_coPicType;//保存图片的格式选择 bmp和jpg
	//CComboBox	m_comboSeq;//巡航轨迹下拉选项
	CComboBox	m_comboPreset;//预置点
	CComboBox	m_comboPTZSpeed;//云台的速度
	CTreeCtrl	m_ctrlTreeChan;//通道树
	CIPAddressCtrl	m_ctrlDevIp;//IP地址
	UINT	m_nDevPort;//端口号
	CString	m_csUser;//用户名
	CString	m_csPWD;//密码
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
	//afx_msg void OnButtonSeqGoto();//调用巡航
	//afx_msg void OnButtonSeqSet();//巡航轨迹设置
	//afx_msg void OnButtonTrackRun();//运行轨迹记录
	//afx_msg void OnButtonTrackStart();//开始轨迹记录
	//afx_msg void OnButtonTrackStop();//停止轨迹记录
	afx_msg void OnBtnPtzAuto();
	//afx_msg void OnBtnAuxPwron();//辅助设备1
	//afx_msg void OnBtnFanPwron();风扇
	//afx_msg void OnBtnHeaterPwron();//加热
	//afx_msg void OnBtnLightPwron();//灯光控制
	//afx_msg void OnBtnWiperPwron();//雨刷
	//afx_msg void OnBtnAuxPwron2();//辅助设备2
	afx_msg void OnSelchangeComboPicType();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();//测试
	afx_msg void OnBnClickedImagePreprocessingButton();//图像预处理
	void DrawPicToHDC(cv::Mat& mat,UINT ID);//将图像信息显示到picture控件上面
	afx_msg void OnBnClickedImageSegmentationButton();//图像分割
	afx_msg void OnBnClickedImageEdgeextractionButton();//边缘提取
	afx_msg void OnBnClickedOpenfileButton1();//打开图像
	afx_msg void OnBnClickedUpdateButton();//刷新
	afx_msg void OnBnClickedClinkOnButton();//手动选点
public:
	//以下是选取的子窗口传入主窗口的数据及图像处理过程中的结果图像
	Mat src;//打开图像的src图像
	Mat dst;//打开图像后处理的dst图像
	Mat show_img;//用于向主窗口显示标记了目标的图像
	Mat gray;
	Mat imageROI;//选取的ROI图像
	Mat COMPENSATION_image;//光照补偿图像
	Mat equalizeImage;//直方图均衡化图像
	Mat Image_Threshold;//手动阈值图像
	Mat Image_OstuThreshold; //大律法阈值图像
	Mat Image_adaptiveThreshold; //自适应阈值图像
	Mat blurImage;//中值滤波
	Mat GaussianBlurImage; //高斯滤波
	Mat MedianBlurImage; //中值滤波
	Mat BilateralFilterImage;//双边滤波
	Mat CannyImage;//Canny边缘检测
	Mat SobelImage; //Sobel边缘检测
	Mat LaplaceImage; //Laplace边缘检测
	Mat ErodeImage; //腐蚀图像
	Mat DilateImage;//膨胀图像

	//通过窗口选点的相关参数设置
	boolean flag;//手动选点时候选取点数的标记
	Point2f p1;//手动选取第一个点
	Point2f p2;//手动选取第二个点
	// 选取的第一个点，显示到窗口
	CString m_strPoint1;
	// 选取的第二个点，显示到窗口
	CString m_strPoint2;
	// 距离，显示到窗口
	CString m_strDistance;
	//光标位置，显示到窗口
	CString m_strPoint;
	
	// 状态信息栏
	CListBox m_info;
	afx_msg void OnBnClickedButtonInfoClc();
	afx_msg void OnBnClickedSetptzposButton();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LaserDeicingDLG_H__4C5D1B67_AC1D_499C_A88A_C0B663F739D0__INCLUDED_)
