// ImageSegmentationDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LaserDeicing.h"
#include "ImageSegmentationDlg.h"
#include "afxdialogex.h"


// ImageSegmentationDlg �Ի���

IMPLEMENT_DYNAMIC(ImageSegmentationDlg, CDialogEx)

ImageSegmentationDlg::ImageSegmentationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ImageSegmentationDlg::IDD, pParent)
	, m_threshold(0)
	, m_BlockSize(0)
	, m_nCountourNum(0)
	, m_dCountourArea(0)
{

}

ImageSegmentationDlg::~ImageSegmentationDlg()
{
}

void ImageSegmentationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_THRESHOLD_EDIT, m_threshold);
	DDX_Text(pDX, IDC_BLOCKSIZE_EDIT, m_BlockSize);
	DDX_Control(pDX, IDC_THRESHOLD_TYPE_COMBO, m_ComboThresholdType);
	DDX_Control(pDX, IDC_THRESHOLD_ADAPTIVE_METHOD_COMBO, m_ComboThresholdAdaptiveMethod);
	DDX_Text(pDX, IDC_CONTOURS_NUMBER_EDIT, m_nCountourNum);
	DDX_Control(pDX, IDC_CONTOURS_MODE_COMBO, m_ComboModeType);
	DDX_Control(pDX, IDC_CONTOURS_METHOD_COMBO, m_ComboMetodType);
	DDX_Text(pDX, IDC_CONTOUR_AREA_THRESHOLD_EDIT, m_dCountourArea);
}


BEGIN_MESSAGE_MAP(ImageSegmentationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_THRESHOLD_BUTTON, &ImageSegmentationDlg::OnBnClickedThresholdButton)
	ON_BN_CLICKED(IDC_OTUS_BUTTON, &ImageSegmentationDlg::OnBnClickedOtusButton)
	ON_BN_CLICKED(IDC_ADAPTIVE_BUTTON, &ImageSegmentationDlg::OnBnClickedAdaptiveButton)
	ON_BN_CLICKED(IDC_EDGEEXRACTION_OPEN_FILE, &ImageSegmentationDlg::OnBnClickedOpenFile)
	ON_BN_CLICKED(IDC_EDGEEXRACTION_CLOSE_FILE, &ImageSegmentationDlg::OnBnClickedCloseFile)
	ON_BN_CLICKED(IDC_CONTOURS_BUTTON, &ImageSegmentationDlg::OnBnClickedContoursButton)
	ON_BN_CLICKED(IDC_DST_CONTOURS_BUTTON, &ImageSegmentationDlg::OnBnClickedDstContoursButton)
	ON_BN_CLICKED(IDOK, &ImageSegmentationDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ImageSegmentationDlg ��Ϣ�������
//�ֶ���ֵ�ָ�
void ImageSegmentationDlg::OnBnClickedThresholdButton()
{
	showSRCImage();
	Mat temp_image;
	src_image = src.clone();
	UpdateData(TRUE);
	if(m_threshold>255 || m_threshold<=0)
	{
		MessageBox(_T("��ȷ����ȷ��ͼ��ָ���ֵ��"), MB_OK);
		return ;
	}
	if(src_image.channels() >=3)
	{
		cvtColor(src_image, gray, COLOR_BGR2GRAY);
	} else {
		gray = src;
	}
	int nSel = m_ComboThresholdType.GetCurSel();
	switch(nSel)
	{
	case 0: 
		threshold(gray, temp_image, m_threshold, 255, THRESH_BINARY);//�ֶ���ֵ��
		break;
	case 1:
		threshold(gray, temp_image, m_threshold, 255, THRESH_BINARY_INV);//�ֶ���ֵ��
		break;
	case 2:
		threshold(gray, temp_image, m_threshold, 255, THRESH_TRUNC);//�ֶ���ֵ��
		break;
	case 3:
		threshold(gray, temp_image, m_threshold, 255, THRESH_TOZERO);//�ֶ���ֵ��
		break;
	case 4:
		threshold(gray, temp_image, m_threshold, 255, THRESH_TOZERO_INV);//�ֶ���ֵ��
		break;
	case 6:
		threshold(gray, temp_image, m_threshold, 255, THRESH_OTSU);//�ֶ���ֵ��
		break;
	default:
		MessageBox(_T("��ѡ����ʵ���ֵ����"));
		break;
	}
	Show(temp_image, IDC_DST_IMSHOW);
	Image_Threshold = temp_image.clone();
	imwrite("./ͼ��/ͼ��ָ�/�ֶ���ֵ�ָ�.jpg", Image_Threshold);
	dst = Image_Threshold;
}

//���ɷ���ֵ�ָ�
void ImageSegmentationDlg::OnBnClickedOtusButton()
{
	showSRCImage();
	Mat temp_image;
	src_image = src.clone();
	if(src_image.channels() >=3)
	{
		cvtColor(src_image, gray, COLOR_BGR2GRAY);
		Show(gray, IDC_DST_IMSHOW);
		//threshold(gray, temp_image, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY_INV);//���
		threshold(gray, temp_image, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY);//���
		Show(temp_image, IDC_DST_IMSHOW);
	}
	else
	{
		Show(src_image, IDC_DST_IMSHOW);
		//threshold(src_image, temp_image, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY_INV);//���
		threshold(src_image, temp_image, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY);//���
		Show(temp_image, IDC_DST_IMSHOW);
	}
	Image_OstuThreshold = temp_image.clone();
	imwrite("./ͼ��/ͼ��ָ�/���ɷ���ֵ�ָ�.jpg", Image_OstuThreshold);
	dst = Image_OstuThreshold;
}

//����Ӧ��ֵ�ָ�
void ImageSegmentationDlg::OnBnClickedAdaptiveButton()
{
	showSRCImage();
	UpdateData(TRUE);
	Mat temp_image;
	src_image = src.clone();
	if(m_BlockSize%2 ==0)
	{
		MessageBox("��ȷ������ĺ˲���Ϊ������)", MB_OK);
		return ;
	}
	if(src_image.channels() >=3)
	{
		cvtColor(src_image, gray, COLOR_BGR2GRAY);
	}
	else
	{
		gray = src_image;
	}
	int nSel = m_ComboThresholdAdaptiveMethod.GetCurSel();
	switch(nSel)
	{
	case 0: 
		{
			int nSel = m_ComboThresholdType.GetCurSel();
			switch(nSel)
			{
			case 0: 
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, m_BlockSize, 0);
				break;
			case 1:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, m_BlockSize, 0);
				break;
			case 2:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_TRUNC, m_BlockSize, 0);
				break;
			case 3:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_TOZERO, m_BlockSize, 0);
				break;
			case 4:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_TOZERO_INV, m_BlockSize, 0);
				break;
			case 5:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_MASK, m_BlockSize, 0);
				break;
			case 6:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_OTSU, m_BlockSize, 0);
				break;
			}
		}
		break;
	case 1:
		{
			int nSel = m_ComboThresholdType.GetCurSel();
			switch(nSel)
			{
			case 0: 
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, m_BlockSize, 0);
				break;
			case 1:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, m_BlockSize, 0);
				break;
			case 2:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_TRUNC, m_BlockSize, 0);
				break;
			case 3:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_TOZERO, m_BlockSize, 0);
				break;
			case 4:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_TOZERO_INV, m_BlockSize, 0);
				break;
			case 5:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_MASK, m_BlockSize, 0);
				break;
			case 6:
				adaptiveThreshold(gray, temp_image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_OTSU, m_BlockSize, 0);
				break;
			}
		}
		break;
	}
	Show(temp_image, IDC_DST_IMSHOW);
	Image_adaptiveThreshold = temp_image.clone();
	imwrite("./ͼ��/ͼ��ָ�/����Ӧ��ֵ�ָ�.jpg", Image_adaptiveThreshold);
	dst = Image_adaptiveThreshold;
}

//��ͼƬ
void ImageSegmentationDlg::OnBnClickedOpenFile()
{
	CFileDialog OpenImage(
		TRUE, 
		_T("*.jpg"), 
		NULL, 
		0, 
		_T("ͼƬ�ļ�(*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp|�����ļ�(*.*)|*.*||"), 
		NULL);
	OpenImage.m_ofn.lpstrTitle = _T("����������ܼ��v1.0");	
	if (IDOK == OpenImage.DoModal())
	{
		CString filepath;
		CString filename;
		filepath = OpenImage.GetPathName();
		filename = filepath.GetBuffer(0);
		string name = filename.GetBuffer();//��CString����ת����string
		src = imread(name, 1);
		if(src.empty())	 
		{
			AfxMessageBox(_T("no image loaded��"));
			return;
		}
		Show(src, IDC_SRC_IMSHOW);
		filepath.ReleaseBuffer();
		return;
	}
	else
	{
		return;
	}
}

//�ر�ͼƬ
void ImageSegmentationDlg::OnBnClickedCloseFile()
{
	Mat temp(3, 3, CV_8UC3, Scalar(194, 194, 194));//����һ������ͼ
	Show(temp, IDC_SRC_IMSHOW);
	Show(temp, IDC_DST_IMSHOW);
	src.release();//����ͼ��
	dst.release();//���ͼ��
	src_image.release();//ÿ�δ���ʱ�����ʱ����
	gray.release();//�Ҷ�ͼ��
	Image_Threshold.release();//�ֶ���ֵͼ��
	Image_OstuThreshold.release(); //���ɷ���ֵͼ��
	Image_adaptiveThreshold.release(); //����Ӧ��ֵͼ��
}

//��ʾͼ��
void ImageSegmentationDlg::Show( Mat &src, UINT ID )
{
	CDC* pDC;
	HDC hDC;
	pDC = GetDlgItem(ID)->GetDC();
	hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	IplImage imag = src;                  // IplImage img = (IplImage)src;
	CvvImage cimg;
	cimg.CopyOf(&imag, 1); 
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
	cimg.Destroy();
}


BOOL ImageSegmentationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//������ֵ���͵�ֵ
	m_ComboThresholdType.InsertString(0,_T("THRESH_BINARY"));
	m_ComboThresholdType.InsertString(1,_T("THRESH_BINARY_INV"));
	m_ComboThresholdType.InsertString(2,_T("THRESH_TRUNC"));
	m_ComboThresholdType.InsertString(3,_T("THRESH_TOZERO"));
	m_ComboThresholdType.InsertString(4,_T("THRESH_TOZERO_INV"));
	m_ComboThresholdType.InsertString(5,_T("THRESH_MASK"));
	m_ComboThresholdType.InsertString(6,_T("THRESH_OTSU"));
	m_ComboThresholdType.SetCurSel(0);//��ֵ���ͳ�ʼֵ
	//��������Ӧ����
	m_ComboThresholdAdaptiveMethod.InsertString(0,_T("ADAPTIVE_THRESH_MEAN_C"));
	m_ComboThresholdAdaptiveMethod.InsertString(1,_T("ADAPTIVE_THRESH_GUASSIAN_C"));
	m_ComboThresholdAdaptiveMethod.SetCurSel(0);//��ֵ���ͳ�ʼֵ
	//������������ģʽ
	m_ComboModeType.InsertString(0, _T("CV_RETR_EXTERNAL"));//ֻ����ⲿ����
	m_ComboModeType.InsertString(1, _T("CV_RETR_LIST"));
	m_ComboModeType.InsertString(2, _T("CV_RETR_CCOMP"));
	m_ComboModeType.InsertString(3, _T("CV_RETR_TRE"));
	m_ComboModeType.SetCurSel(0);
	//���������Ľ��Ʒ���
	m_ComboMetodType.InsertString(0, _T("CV_CHAIN_APPROX_NONE"));
	m_ComboMetodType.InsertString(1, _T("CV_CHAIN_APPROX_SIMPLE"));
	m_ComboMetodType.InsertString(2, _T("CV_CHAIN_APPROX_TC89_L1"));
	m_ComboMetodType.InsertString(3, _T("CV_CHAIN_APPROX_TC89_KCOS"));
	m_ComboMetodType.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//��ȡ�ⲿ����
void ImageSegmentationDlg::OnBnClickedContoursButton()
{
	src_image = src.clone();
	show_img = imread("./ͼ��/Դͼ��/src.jpg");
	//vector<vector<Point>> contours;//�洢ÿ���������ж�����㼯�����ж���Ԫ��
	//Vec4iÿ��Ԫ�ذ���4��int�ͱ���
	//vector<Vec4i>������ÿ��Ԫ�ذ���4��int���������������������hierarchy[i][0] - hierarchy[i][3]
	//�ֱ��ʾ��i�ĺ�һ��������ǰһ������������������Ƕ������������ţ����û��ֵΪ-1
	//vector<Vec4i> hierarchy;//Ԫ�صĸ��� = ������
	findContours(src_image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//CV_RETR_EXTERNALֻ����ⲿ�������ɸ�������������е���
	Mat contoursImage(src_image.rows, src_image.cols, CV_8U, Scalar(0));//����������ȡ�����ı���ͼ
	int index = 0;
	for (; index >= 0; index = hierarchy[index][0]) {
		cv::drawContours(contoursImage, contours, index, Scalar(255), 1, 8, hierarchy);//����ַ��������� index���ڼ�������
		Rect rect = boundingRect(contours[index]);//���������
		rectangle(show_img, rect, Scalar(0,0,255), 3);//���������Ӿ��ο�
		m_nCountourNum++;//�����⵽��������
	}
	imwrite("./ͼ��/ͼ��ָ�/��ͨ������.jpg", contoursImage);
	dst = contoursImage;
	Show(contoursImage, IDC_DST_IMSHOW);
	UpdateData(FALSE);
}

bool ContoursSortFun(vector<cv::Point> contour1, vector<cv::Point> contour2) {
	return  (contourArea(contour1), contourArea(contour2)); // ���
}

//�������ֵ��ȡ����
void ImageSegmentationDlg::OnBnClickedDstContoursButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData(TRUE);
	src_image = src.clone();
	show_img = imread("./ͼ��/Դͼ��/src.jpg");
	findContours(src_image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//CV_RETR_EXTERNALֻ����ⲿ�������ɸ�������������е���
	Mat contoursImage(src_image.rows, src_image.cols, CV_8U, Scalar(0));//����������ȡ�����ı���ͼ
	//ɸѡ�޳������С��100������
	vector <vector<Point>>::iterator iter = contours.begin();
	for (; iter != contours.end();)
	{
		double g_dConArea = contourArea(*iter);
		if (g_dConArea < m_dCountourArea)
		{
			iter = contours.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	for (int i = 0; i < (int)contours.size(); i++)
	{
		drawContours(contoursImage, contours, i, Scalar(255), 1, 8);
		Rect rect = boundingRect(contours[i]);//���������
		rectangle(show_img, rect, Scalar(0,0,255), 3);//���������Ӿ��ο�
	}
	m_nCountourNum = contours.size();
	imwrite("./ͼ��/ͼ��ָ�/��ͨ������.jpg", contoursImage);
	dst = contoursImage;
	Show(contoursImage, IDC_DST_IMSHOW);
	UpdateData(FALSE);
}


void ImageSegmentationDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLaserDeicingDlg *dlg = (CLaserDeicingDlg *)AfxGetMainWnd();
	dlg->Image_Threshold = Image_Threshold;//�ֶ���ֵͼ��
	dlg->Image_OstuThreshold = Image_OstuThreshold; //���ɷ���ֵͼ��
	dlg->Image_adaptiveThreshold = Image_adaptiveThreshold; //����Ӧ��ֵͼ��
	dlg->dst = dst;
	dlg->show_img = show_img;
	EndDialog(0);
}

void ImageSegmentationDlg::showSRCImage() {
	if (!src.empty())
	{
		Show(src, IDC_SRC_IMSHOW);
	}
}
