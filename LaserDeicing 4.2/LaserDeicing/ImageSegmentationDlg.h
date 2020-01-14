#pragma once
#include "string"
#include "CvvImage.h" //OpenCVͷ�ļ�
#include "opencv2/opencv.hpp" //OpenCVͷ�ļ�
#include "afxwin.h"
#include "LaserDeicing.h"//������Ի���ͷ�ļ�

using namespace std;
using namespace cv;

// ImageSegmentationDlg �Ի���

class ImageSegmentationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ImageSegmentationDlg)

public:
	ImageSegmentationDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ImageSegmentationDlg();

// �Ի�������
	enum { IDD = IDD_IMAGE_SEGMENTATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	Mat src;//����ͼ��
	Mat dst;//���ͼ��
	Mat show_img;//�����������ڴ���ʶ���Ŀ����ͼ��
	Mat src_image;//ÿ�δ���ʱ�����ʱ����
	Mat gray;//�Ҷ�ͼ��
	Mat Image_Threshold;//�ֶ���ֵͼ��
	Mat Image_OstuThreshold; //���ɷ���ֵͼ��
	Mat Image_adaptiveThreshold; //����Ӧ��ֵͼ��
	afx_msg void OnBnClickedThresholdButton();//�ֶ���ֵ�ָ�
	afx_msg void OnBnClickedOtusButton();//���ɷ���ֵ�ָ�
	afx_msg void OnBnClickedAdaptiveButton();//����Ӧ��ֵ�ָ�
	afx_msg void OnBnClickedOpenFile();//���ļ�
	void Show(Mat &src, UINT ID);//��ʾͼ��
	afx_msg void OnBnClickedCloseFile();//�ر��ļ�
	// ��ֵ����
	int m_threshold;
	// BlockSize
	int m_BlockSize;
	// ѡ����ֵ����
	CComboBox m_ComboThresholdType;
	virtual BOOL OnInitDialog();
	// ����Ӧ����
	CComboBox m_ComboThresholdAdaptiveMethod;
	afx_msg void OnBnClickedContoursButton();//��ȡ��������
	afx_msg void OnBnClickedDstContoursButton();//�������ȡĿ������
	//���������Ĳ�νṹ
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	// ��������������
	int m_nCountourNum;
	afx_msg void OnBnClickedOk();
	void showSRCImage();
	// //����������ģʽ
	CComboBox m_ComboModeType;
	// ���������Ľ��Ʒ���
	CComboBox m_ComboMetodType;
	// ���������ֵ
	double m_dCountourArea;
};
