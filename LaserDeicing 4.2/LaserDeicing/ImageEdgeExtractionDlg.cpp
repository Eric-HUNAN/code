// ImageEdgeExtractionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LaserDeicing.h"
#include "ImageEdgeExtractionDlg.h"
#include "afxdialogex.h"


// ImageEdgeExtractionDlg �Ի���

IMPLEMENT_DYNAMIC(ImageEdgeExtractionDlg, CDialogEx)

ImageEdgeExtractionDlg::ImageEdgeExtractionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ImageEdgeExtractionDlg::IDD, pParent)
	, m_nStructElement(0)
	, m_nErodeSize(0)
	, m_nDilateSize(0)
	, m_nHighThreshold(0)
	, m_nLowThreshold(0)
{

}

ImageEdgeExtractionDlg::~ImageEdgeExtractionDlg()
{
}

void ImageEdgeExtractionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STRUCT_ELEMENT_EDIT, m_nStructElement);
	DDX_Text(pDX, IDC_ERODE_EDIT, m_nErodeSize);
	DDX_Text(pDX, IDC_DILATE_EDIT, m_nDilateSize);
	DDX_Text(pDX, IDC_HIGHTHRESHOLD_EDIT, m_nHighThreshold);
	DDX_Text(pDX, IDC_LOWTHRESHOLD_EDIT, m_nLowThreshold);
	DDX_Control(pDX, IDC_MORPHOLOGY_COMBO, m_ComboMorphology);
}


BEGIN_MESSAGE_MAP(ImageEdgeExtractionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_EDGEEXRACTION_OPEN_FILE, &ImageEdgeExtractionDlg::OnBnClickedEdgeexractionOpenFile)
	ON_BN_CLICKED(IDC_EDGEEXRACTION_CLOSE_FILE, &ImageEdgeExtractionDlg::OnBnClickedEdgeexractionCloseFile)
	ON_BN_CLICKED(IDC_ERODE_BUTTON, &ImageEdgeExtractionDlg::OnBnClickedErodeButton)
	ON_BN_CLICKED(IDC_DILATE_BUTTON, &ImageEdgeExtractionDlg::OnBnClickedDilateButton)
	ON_BN_CLICKED(IDC_SOBEL_BUTTON, &ImageEdgeExtractionDlg::OnBnClickedSobelButton)
	ON_BN_CLICKED(IDC_ROBERTS_BUTTON, &ImageEdgeExtractionDlg::OnBnClickedRobertsButton)
	ON_BN_CLICKED(IDC_PREWITT_BUTTON, &ImageEdgeExtractionDlg::OnBnClickedPrewittButton)
	ON_BN_CLICKED(IDC_CANNY_BUTTON, &ImageEdgeExtractionDlg::OnBnClickedCannyButton)
	ON_BN_CLICKED(IDC_LAPLACE_BUTTON, &ImageEdgeExtractionDlg::OnBnClickedLaplaceButton)
	ON_CBN_SELCHANGE(IDC_MORPHOLOGY_COMBO, &ImageEdgeExtractionDlg::OnCbnSelchangeMorphologyCombo)
	ON_BN_CLICKED(IDOK, &ImageEdgeExtractionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ImageEdgeExtractionDlg ��Ϣ�������
//���ļ�
void ImageEdgeExtractionDlg::OnBnClickedEdgeexractionOpenFile()
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
		Show(src, IDC_SRC_IMSHOW1);
		dst = src;
		filepath.ReleaseBuffer();
		return;
	}
	else
	{
		return;
	}
}

//��ʾͼ�񵽴���
void ImageEdgeExtractionDlg::Show( Mat &src, UINT ID )
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


//�ر���ʾ
void ImageEdgeExtractionDlg::OnBnClickedEdgeexractionCloseFile()
{
	Mat temp(3, 3, CV_8UC3, Scalar(194, 194, 194));//����һ������ͼ
	Show(temp, IDC_SRC_IMSHOW1);
	Show(temp, IDC_DST_IMSHOW1);
	src.release();//����ͼ��
	dst.release();//���ͼ��
	gray.release();//�Ҷ�ͼ��
	edges.release();//��Եͼ��
	CannyImage.release();//Canny��Ե���
	SobelImage.release(); //Sobel��Ե���
	LaplaceImage.release(); //Laplace��Ե���
	ErodeImage.release(); //��ʴͼ��
	DilateImage.release();//����ͼ�� 
}

//��ʴ
void ImageEdgeExtractionDlg::OnBnClickedErodeButton()
{
	showSRCImage();
	UpdateData(TRUE);
	if(m_nErodeSize%2 == 0)
	{
		MessageBox(_T("������һ��������ֵ��"), MB_OK);
		return;
	}
	Mat temp_image;
	if (!dst.empty())
	{
		src_image = dst.clone();
	} 
	else if(!src.empty())
	{
		src_image = src;
	} else {
		MessageBox(_T("������Դͼ��"));
		return;
	}
	Mat erode_kernel = getStructuringElement(MORPH_RECT, Size(m_nErodeSize, m_nErodeSize)); 
	erode(src_image, temp_image, erode_kernel, Point(-1,-1), 1);
	imwrite("./ͼ��/��Ե��ȡ/erodeImage.jpg", temp_image);
	Show(temp_image, IDC_DST_IMSHOW1);
	ErodeImage = temp_image.clone();
	dst = temp_image.clone();	
}

//����
void ImageEdgeExtractionDlg::OnBnClickedDilateButton()
{
	showSRCImage();
	UpdateData(TRUE);
	if(m_nDilateSize%2 == 0)
	{
		MessageBox(_T("������һ��������ֵ��"), MB_OK);
		return;
	}
	Mat temp_image;
	if (!dst.empty())
	{
		src_image = dst.clone();
	} 
	else if(!src.empty())
	{
		src_image = src;
	} else {
		MessageBox(_T("������Դͼ��"));
		return;
	}
	Mat dilate_kernel = getStructuringElement(MORPH_RECT, Size(m_nDilateSize, m_nDilateSize)); 
	dilate(src_image, temp_image, dilate_kernel, Point(-1,-1), 1);
	imwrite("./ͼ��/��Ե��ȡ/dilate.jpg", temp_image);
	Show(temp_image, IDC_DST_IMSHOW1);
	DilateImage = temp_image.clone();
	dst = temp_image.clone();
}

//sobel����
void ImageEdgeExtractionDlg::OnBnClickedSobelButton()
{
	showSRCImage();
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat temp_image;
	if (!dst.empty())
	{
		src_image = dst.clone();
	} 
	else if(!src.empty())
	{
		src_image = src;
	} else {
		MessageBox(_T("������Դͼ��"));
		return;
	}
	if(src_image.channels() ==3)
	{
		cvtColor(src_image, temp_image, CV_BGR2GRAY);  
		Show(gray,  IDC_DST_IMSHOW1);
	}//�õ�image�ĻҶ�ͼgray
	else
	{
		temp_image = src_image.clone();
	}

	Sobel(temp_image, grad_x, CV_16S, 1, 0, 3, 1, 1);     
	convertScaleAbs(grad_x, abs_grad_x);//��Sobel�󵼺��������������255��ֵ�����Խ�CV_16Sת��Ϊ8λuint,
	imwrite("./ͼ��/��Ե��ȡ/Sobel_X.jpg", abs_grad_x);

	Sobel(temp_image, grad_y, CV_16S, 0, 1, 3, 1, 1);
	convertScaleAbs(grad_y, abs_grad_y);
	imwrite("./ͼ��/��Ե��ȡ/Sobel_Y.jpg", abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, edges);//��x������y�����Sobelֵ��ӣ����Ƶõ����巽��ĵ���ֵ��

	Show(edges, IDC_DST_IMSHOW1);
	imwrite("./ͼ��/��Ե��ȡ/Sobel.jpg", edges);
	SobelImage = edges.clone();
	dst = edges.clone();
}

//Roberts����
void ImageEdgeExtractionDlg::OnBnClickedRobertsButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(_T("����"));
}

//Prewitt����
void ImageEdgeExtractionDlg::OnBnClickedPrewittButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(_T("����"));
}

//Canny����
void ImageEdgeExtractionDlg::OnBnClickedCannyButton()
{
	showSRCImage();
	UpdateData(TRUE);
	if (!dst.empty())
	{
		src_image = dst.clone();
	} 
	else if(!src.empty())
	{
		src_image = src;
	} else {
		MessageBox(_T("������Դͼ��"));
		return;
	}
	if(src_image.channels() ==3)
	{
		cvtColor(src_image, gray, CV_BGR2GRAY);  
	}
	else
	{
		gray = src_image.clone();
	}
	GaussianBlur(gray, gray, Size(5,5),0,0);//��grayͼ���и�˹�˲���ȥ��	
	Show(gray, IDC_SRC_IMSHOW1);

	if(m_nLowThreshold <= 0) //������ֵ�����ֵ֮��������2:1��3:1֮��
	{
		MessageBox(_T("������һ������0��Canny���ӵ���ֵ������"), MB_OK);
		return ;
	}
	else if(m_nHighThreshold <= 0)
	{
		MessageBox(_T("������һ������0��Canny���Ӹ���ֵ������"), MB_OK);
		return ;
	}
	else if(m_nHighThreshold > 3*m_nLowThreshold || m_nHighThreshold < 2*m_nLowThreshold)
	{
		MessageBox(_T("����ֵ�����ֵ֮�������2:1��3:1֮��"), MB_OK);
		return ;	
	}

	Canny(gray, edges, m_nLowThreshold , m_nHighThreshold, 5);  //���Ҷ�ͼ����Canny���Ӳ������õ���Եͼ,apertureSize=5;Ĭ��Ϊ3
	imwrite("./ͼ��/��Ե��ȡ/CannyImage.jpg", edges);
	Show(edges, IDC_DST_IMSHOW1);
	CannyImage = edges;
	dst = edges.clone();
}

//Laplacian����
void ImageEdgeExtractionDlg::OnBnClickedLaplaceButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(_T("����"));
}

//���ڳ�ʼ���麯��
BOOL ImageEdgeExtractionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ComboMorphology.AddString(_T("������"));
	m_ComboMorphology.AddString(_T("������"));
	m_ComboMorphology.AddString(_T("��̬ѧ�ݶ�"));
	m_ComboMorphology.AddString(_T("��ñ"));
	m_ComboMorphology.AddString(_T("��ñ"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//��̬ѧ����
void ImageEdgeExtractionDlg::OnCbnSelchangeMorphologyCombo()
{
	showSRCImage();
	UpdateData(TRUE);
	if(m_nStructElement%2 == 0)
	{
		MessageBox(_T("������һ��������ֵ��"), MB_OK);
		return;
	}
	Mat temp_image;
	if (!dst.empty())
	{
		src_image = dst.clone();
	} 
	else if(!src.empty())
	{
		src_image = src;
	} else {
		MessageBox(_T("������Դͼ��"));
		return;
	}
	//�����
	Mat element_Kernel = getStructuringElement(MORPH_RECT, Size(m_nStructElement, m_nStructElement));
	int nSel = m_ComboMorphology.GetCurSel();
	switch(nSel)
	{
	case 0:
		//������̬ѧ����  
		morphologyEx(src_image, temp_image, MORPH_OPEN, element_Kernel, Point(-1, -1), 2); 
		imwrite("./ͼ��/��Ե��ȡ/������.png", temp_image);
		break;
	case 1:
		morphologyEx(src_image, temp_image, MORPH_CLOSE, element_Kernel, Point(-1, -1), 2);
		imwrite("./ͼ��/��Ե��ȡ/������.png", temp_image);
		break;
	case 2:
		morphologyEx(src_image, temp_image, MORPH_GRADIENT, element_Kernel, Point(-1, -1), 2);
		imwrite("./ͼ��/��Ե��ȡ/��̬ѧ�ݶ�.png", temp_image);
		break;
	case 3:
		morphologyEx(src_image, temp_image, MORPH_TOPHAT, element_Kernel, Point(-1, -1), 2);
		imwrite("./ͼ��/��Ե��ȡ/��ñ.png", temp_image);
		break;
	case 4:
		morphologyEx(src_image, temp_image, MORPH_BLACKHAT, element_Kernel, Point(-1, -1), 2);
		imwrite("./ͼ��/��Ե��ȡ/��ñ.png", temp_image);
		break;
	default:
		break;
	}
	Show(temp_image, IDC_DST_IMSHOW1);
	dst = temp_image.clone();
	UpdateData(FALSE);
}

//�������ڴ�����
void ImageEdgeExtractionDlg::OnBnClickedOk()
{
	CLaserDeicingDlg *dlg = (CLaserDeicingDlg*)AfxGetMainWnd();
	dlg->CannyImage = CannyImage;//Canny��Ե���
	dlg->SobelImage = SobelImage; //Sobel��Ե���
	dlg->LaplaceImage = LaplaceImage; //Laplace��Ե���
	dlg->ErodeImage = ErodeImage; //��ʴͼ��
	dlg->DilateImage = DilateImage;//����ͼ�� 
	dlg->dst = dst;
	EndDialog(0);
}

void ImageEdgeExtractionDlg::showSRCImage() {
	if (!src.empty())
	{
		Show(src, IDC_SRC_IMSHOW1);
	}
}
