#pragma once
#include "string"
#include "CvvImage.h" //OpenCV头文件
#include "opencv2/opencv.hpp" //OpenCV头文件
#include "afxwin.h"
#include "LaserDeicingDlg.h"//添加主对话框头文件


// ImageEdgeExtractionDlg 对话框

class ImageEdgeExtractionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ImageEdgeExtractionDlg)

public:
	ImageEdgeExtractionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ImageEdgeExtractionDlg();

// 对话框数据
	enum { IDD = IDD_IMAGE_EDGEEXTRACTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEdgeexractionOpenFile();
	afx_msg void OnBnClickedEdgeexractionCloseFile();
	afx_msg void OnBnClickedErodeButton();
	afx_msg void OnBnClickedDilateButton();
	afx_msg void OnBnClickedSobelButton();
	afx_msg void OnBnClickedRobertsButton();
	afx_msg void OnBnClickedPrewittButton();
	afx_msg void OnBnClickedCannyButton();
	afx_msg void OnBnClickedLaplaceButton();
	void Show(Mat &src, UINT ID);//显示图像到窗口
	Mat src;//输入图像
	Mat dst;//输出图像
	Mat src_image;//每次处理时候的临时变量
	Mat gray;//灰度图像
	Mat edges;//边缘图像
	Mat CannyImage;//Canny边缘检测
	Mat SobelImage; //Sobel边缘检测
	Mat LaplaceImage; //Laplace边缘检测
	Mat ErodeImage; //腐蚀图像
	Mat DilateImage;//膨胀图像 
	int m_nStructElement;
	// 腐蚀核大小
	int m_nErodeSize;
	// 膨胀核大小
	int m_nDilateSize;
	// Canny边缘检测高阈值
	int m_nHighThreshold;
	// Canny边缘检测低阈值
	int m_nLowThreshold;
	// 选择用什么样的形态学运算
	CComboBox m_ComboMorphology;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeMorphologyCombo();
	afx_msg void OnBnClickedOk();
	void showSRCImage();
};
