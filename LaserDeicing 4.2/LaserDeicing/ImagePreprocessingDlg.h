#pragma once
#include "string"
#include "CvvImage.h" //OpenCV头文件
#include "opencv2/opencv.hpp" //OpenCV头文件
#include "LaserDeicingDlg.h"//添加主对话框头文件
#include "afxwin.h"

using namespace std;
using namespace cv;

// ImagePreprocessingDlg 对话框

class ImagePreprocessingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ImagePreprocessingDlg)

public:
	ImagePreprocessingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ImagePreprocessingDlg();

// 对话框数据
	enum { IDD = IDD_IMAGE_PREPROCESSING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//下面是图像预处理窗口控件按钮的图像消息处理方法
	afx_msg void OnBnClickedRoiButton();
	afx_msg void OnBnClickedGrayButton();
	afx_msg void OnBnClickedImagestretchingButton();
	afx_msg void OnBnClickedCompensationButton();
	afx_msg void OnBnClickedCalcHistButton();
	afx_msg void OnBnClickedEqualizeButton();
	afx_msg void OnBnClickedBlurButton();
	afx_msg void OnBnClickedGaussianButton();
	afx_msg void OnBnClickedMedianblurButton();
	afx_msg void OnBnClickedBilateralfilterButton();
	afx_msg void OnBnClickedImagepreprocessongOpenFile();//打开文件
	afx_msg void OnBnClickedImagepreprocessingCloseFile();//关闭文件
	void Show(Mat &src, UINT ID);
	// ROI区域左上角坐标X
	int g_nRoiX;
	// ROI区域左上角坐标Y
	int g_nRoiY;
	// ROI区域高度大小
	int g_nRoiHeight;
	// ROI区域宽度大小
	int g_nRoiWidth;
	static const int s_nBlockSize = 32;
	// 均值滤波核大小
	int m_nBlurSize;
	// 高斯滤波核大小
	int m_nGaussianSize;
	// 中值滤波核大小
	int m_nMedianBlurSize;
	// 双边滤波核大小
	int m_nBilateralSize;
	afx_msg void OnBnClickedOk();
	Mat src;//输入图像
	Mat dst;//输出图像
	Mat src_image;//每次处理时候的临时变量
	Mat srcROI;//截取ROI后的图像
	Mat gray;//灰度图像
	Mat imageROI;//选取的ROI图像
	Mat COMPENSATION_image;//光照补偿图像
	Mat equalizeImage;//直方图均衡化图像
	Mat blurImage;//中值滤波
	Mat GaussianBlurImage; //高斯滤波
	Mat MedianBlurImage; //中值滤波
	Mat BilateralFilterImage;//双边滤波

	// 标记通过主窗口调用
	bool flag;
	//初始化函数
	virtual BOOL OnInitDialog();
};
