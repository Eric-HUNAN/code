#pragma once
#include "string"
#include "CvvImage.h" //OpenCVͷ�ļ�
#include "opencv2/opencv.hpp" //OpenCVͷ�ļ�
#include "afxwin.h"
#include "LaserDeicingDlg.h"//������Ի���ͷ�ļ�


// ImageEdgeExtractionDlg �Ի���

class ImageEdgeExtractionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ImageEdgeExtractionDlg)

public:
	ImageEdgeExtractionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ImageEdgeExtractionDlg();

// �Ի�������
	enum { IDD = IDD_IMAGE_EDGEEXTRACTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	void Show(Mat &src, UINT ID);//��ʾͼ�񵽴���
	Mat src;//����ͼ��
	Mat dst;//���ͼ��
	Mat src_image;//ÿ�δ���ʱ�����ʱ����
	Mat gray;//�Ҷ�ͼ��
	Mat edges;//��Եͼ��
	Mat CannyImage;//Canny��Ե���
	Mat SobelImage; //Sobel��Ե���
	Mat LaplaceImage; //Laplace��Ե���
	Mat ErodeImage; //��ʴͼ��
	Mat DilateImage;//����ͼ�� 
	int m_nStructElement;
	// ��ʴ�˴�С
	int m_nErodeSize;
	// ���ͺ˴�С
	int m_nDilateSize;
	// Canny��Ե������ֵ
	int m_nHighThreshold;
	// Canny��Ե������ֵ
	int m_nLowThreshold;
	// ѡ����ʲô������̬ѧ����
	CComboBox m_ComboMorphology;

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeMorphologyCombo();
	afx_msg void OnBnClickedOk();
	void showSRCImage();
};
