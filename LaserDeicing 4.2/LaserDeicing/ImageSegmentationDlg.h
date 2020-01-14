#pragma once
#include "string"
#include "CvvImage.h" //OpenCV头文件
#include "opencv2/opencv.hpp" //OpenCV头文件
#include "afxwin.h"
#include "LaserDeicing.h"//添加主对话框头文件

using namespace std;
using namespace cv;

// ImageSegmentationDlg 对话框

class ImageSegmentationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ImageSegmentationDlg)

public:
	ImageSegmentationDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ImageSegmentationDlg();

// 对话框数据
	enum { IDD = IDD_IMAGE_SEGMENTATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	Mat src;//输入图像
	Mat dst;//输出图像
	Mat show_img;//用于向主窗口传递识别的目标标记图像
	Mat src_image;//每次处理时候的临时变量
	Mat gray;//灰度图像
	Mat Image_Threshold;//手动阈值图像
	Mat Image_OstuThreshold; //大律法阈值图像
	Mat Image_adaptiveThreshold; //自适应阈值图像
	afx_msg void OnBnClickedThresholdButton();//手动阈值分割
	afx_msg void OnBnClickedOtusButton();//大律法阈值分割
	afx_msg void OnBnClickedAdaptiveButton();//自适应阈值分割
	afx_msg void OnBnClickedOpenFile();//打开文件
	void Show(Mat &src, UINT ID);//显示图像
	afx_msg void OnBnClickedCloseFile();//关闭文件
	// 阈值设置
	int m_threshold;
	// BlockSize
	int m_BlockSize;
	// 选择阈值类型
	CComboBox m_ComboThresholdType;
	virtual BOOL OnInitDialog();
	// 自适应方法
	CComboBox m_ComboThresholdAdaptiveMethod;
	afx_msg void OnBnClickedContoursButton();//提取所有轮廓
	afx_msg void OnBnClickedDstContoursButton();//按面积提取目标轮廓
	//定义轮廓的层次结构
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	// 检索到的轮廓数
	int m_nCountourNum;
	afx_msg void OnBnClickedOk();
	void showSRCImage();
	// //轮廓检索的模式
	CComboBox m_ComboModeType;
	// 定义轮廓的近似方法
	CComboBox m_ComboMetodType;
	// 轮廓面积阈值
	double m_dCountourArea;
};
