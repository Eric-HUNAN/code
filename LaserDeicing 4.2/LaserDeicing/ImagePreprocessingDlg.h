#pragma once
#include "string"
#include "CvvImage.h" //OpenCVͷ�ļ�
#include "opencv2/opencv.hpp" //OpenCVͷ�ļ�
#include "LaserDeicingDlg.h"//������Ի���ͷ�ļ�
#include "afxwin.h"

using namespace std;
using namespace cv;

// ImagePreprocessingDlg �Ի���

class ImagePreprocessingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ImagePreprocessingDlg)

public:
	ImagePreprocessingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ImagePreprocessingDlg();

// �Ի�������
	enum { IDD = IDD_IMAGE_PREPROCESSING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//������ͼ��Ԥ�����ڿؼ���ť��ͼ����Ϣ������
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
	afx_msg void OnBnClickedImagepreprocessongOpenFile();//���ļ�
	afx_msg void OnBnClickedImagepreprocessingCloseFile();//�ر��ļ�
	void Show(Mat &src, UINT ID);
	// ROI�������Ͻ�����X
	int g_nRoiX;
	// ROI�������Ͻ�����Y
	int g_nRoiY;
	// ROI����߶ȴ�С
	int g_nRoiHeight;
	// ROI�����ȴ�С
	int g_nRoiWidth;
	static const int s_nBlockSize = 32;
	// ��ֵ�˲��˴�С
	int m_nBlurSize;
	// ��˹�˲��˴�С
	int m_nGaussianSize;
	// ��ֵ�˲��˴�С
	int m_nMedianBlurSize;
	// ˫���˲��˴�С
	int m_nBilateralSize;
	afx_msg void OnBnClickedOk();
	Mat src;//����ͼ��
	Mat dst;//���ͼ��
	Mat src_image;//ÿ�δ���ʱ�����ʱ����
	Mat srcROI;//��ȡROI���ͼ��
	Mat gray;//�Ҷ�ͼ��
	Mat imageROI;//ѡȡ��ROIͼ��
	Mat COMPENSATION_image;//���ղ���ͼ��
	Mat equalizeImage;//ֱ��ͼ���⻯ͼ��
	Mat blurImage;//��ֵ�˲�
	Mat GaussianBlurImage; //��˹�˲�
	Mat MedianBlurImage; //��ֵ�˲�
	Mat BilateralFilterImage;//˫���˲�

	// ���ͨ�������ڵ���
	bool flag;
	//��ʼ������
	virtual BOOL OnInitDialog();
};
