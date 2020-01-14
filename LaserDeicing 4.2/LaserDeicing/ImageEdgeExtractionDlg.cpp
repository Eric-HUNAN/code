// ImageEdgeExtractionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LaserDeicing.h"
#include "ImageEdgeExtractionDlg.h"
#include "afxdialogex.h"


// ImageEdgeExtractionDlg 对话框

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


// ImageEdgeExtractionDlg 消息处理程序
//打开文件
void ImageEdgeExtractionDlg::OnBnClickedEdgeexractionOpenFile()
{
	CFileDialog OpenImage(
		TRUE, 
		_T("*.jpg"), 
		NULL, 
		0, 
		_T("图片文件(*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp|所有文件(*.*)|*.*||"), 
		NULL);
	OpenImage.m_ofn.lpstrTitle = _T("激光除冰智能监测v1.0");	
	if (IDOK == OpenImage.DoModal())
	{
		CString filepath;
		CString filename;
		filepath = OpenImage.GetPathName();
		filename = filepath.GetBuffer(0);
		string name = filename.GetBuffer();//将CString类型转换成string
		src = imread(name, 1);
		if(src.empty())	 
		{
			AfxMessageBox(_T("no image loaded！"));
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

//显示图像到窗口
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


//关闭显示
void ImageEdgeExtractionDlg::OnBnClickedEdgeexractionCloseFile()
{
	Mat temp(3, 3, CV_8UC3, Scalar(194, 194, 194));//设置一副背景图
	Show(temp, IDC_SRC_IMSHOW1);
	Show(temp, IDC_DST_IMSHOW1);
	src.release();//输入图像
	dst.release();//输出图像
	gray.release();//灰度图像
	edges.release();//边缘图像
	CannyImage.release();//Canny边缘检测
	SobelImage.release(); //Sobel边缘检测
	LaplaceImage.release(); //Laplace边缘检测
	ErodeImage.release(); //腐蚀图像
	DilateImage.release();//膨胀图像 
}

//腐蚀
void ImageEdgeExtractionDlg::OnBnClickedErodeButton()
{
	showSRCImage();
	UpdateData(TRUE);
	if(m_nErodeSize%2 == 0)
	{
		MessageBox(_T("请输入一个奇数核值！"), MB_OK);
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
		MessageBox(_T("请输入源图像"));
		return;
	}
	Mat erode_kernel = getStructuringElement(MORPH_RECT, Size(m_nErodeSize, m_nErodeSize)); 
	erode(src_image, temp_image, erode_kernel, Point(-1,-1), 1);
	imwrite("./图像/边缘提取/erodeImage.jpg", temp_image);
	Show(temp_image, IDC_DST_IMSHOW1);
	ErodeImage = temp_image.clone();
	dst = temp_image.clone();	
}

//膨胀
void ImageEdgeExtractionDlg::OnBnClickedDilateButton()
{
	showSRCImage();
	UpdateData(TRUE);
	if(m_nDilateSize%2 == 0)
	{
		MessageBox(_T("请输入一个奇数核值！"), MB_OK);
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
		MessageBox(_T("请输入源图像"));
		return;
	}
	Mat dilate_kernel = getStructuringElement(MORPH_RECT, Size(m_nDilateSize, m_nDilateSize)); 
	dilate(src_image, temp_image, dilate_kernel, Point(-1,-1), 1);
	imwrite("./图像/边缘提取/dilate.jpg", temp_image);
	Show(temp_image, IDC_DST_IMSHOW1);
	DilateImage = temp_image.clone();
	dst = temp_image.clone();
}

//sobel算子
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
		MessageBox(_T("请输入源图像"));
		return;
	}
	if(src_image.channels() ==3)
	{
		cvtColor(src_image, temp_image, CV_BGR2GRAY);  
		Show(gray,  IDC_DST_IMSHOW1);
	}//得到image的灰度图gray
	else
	{
		temp_image = src_image.clone();
	}

	Sobel(temp_image, grad_x, CV_16S, 1, 0, 3, 1, 1);     
	convertScaleAbs(grad_x, abs_grad_x);//因Sobel求导后会产生负数或大于255的值，所以将CV_16S转化为8位uint,
	imwrite("./图像/边缘提取/Sobel_X.jpg", abs_grad_x);

	Sobel(temp_image, grad_y, CV_16S, 0, 1, 3, 1, 1);
	convertScaleAbs(grad_y, abs_grad_y);
	imwrite("./图像/边缘提取/Sobel_Y.jpg", abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, edges);//将x方向与y方向的Sobel值相加，近似得到整体方向的导数值，

	Show(edges, IDC_DST_IMSHOW1);
	imwrite("./图像/边缘提取/Sobel.jpg", edges);
	SobelImage = edges.clone();
	dst = edges.clone();
}

//Roberts算子
void ImageEdgeExtractionDlg::OnBnClickedRobertsButton()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("测试"));
}

//Prewitt算子
void ImageEdgeExtractionDlg::OnBnClickedPrewittButton()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("测试"));
}

//Canny算子
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
		MessageBox(_T("请输入源图像"));
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
	GaussianBlur(gray, gray, Size(5,5),0,0);//将gray图进行高斯滤波，去噪	
	Show(gray, IDC_SRC_IMSHOW1);

	if(m_nLowThreshold <= 0) //将高阈值与低阈值之比限制在2:1和3:1之间
	{
		MessageBox(_T("请输入一个大于0的Canny算子低阈值！！！"), MB_OK);
		return ;
	}
	else if(m_nHighThreshold <= 0)
	{
		MessageBox(_T("请输入一个大于0的Canny算子高阈值！！！"), MB_OK);
		return ;
	}
	else if(m_nHighThreshold > 3*m_nLowThreshold || m_nHighThreshold < 2*m_nLowThreshold)
	{
		MessageBox(_T("高阈值与低阈值之比最好在2:1和3:1之间"), MB_OK);
		return ;	
	}

	Canny(gray, edges, m_nLowThreshold , m_nHighThreshold, 5);  //将灰度图进行Canny算子操作，得到边缘图,apertureSize=5;默认为3
	imwrite("./图像/边缘提取/CannyImage.jpg", edges);
	Show(edges, IDC_DST_IMSHOW1);
	CannyImage = edges;
	dst = edges.clone();
}

//Laplacian算子
void ImageEdgeExtractionDlg::OnBnClickedLaplaceButton()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("测试"));
}

//窗口初始化虚函数
BOOL ImageEdgeExtractionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ComboMorphology.AddString(_T("开运算"));
	m_ComboMorphology.AddString(_T("闭运算"));
	m_ComboMorphology.AddString(_T("形态学梯度"));
	m_ComboMorphology.AddString(_T("顶帽"));
	m_ComboMorphology.AddString(_T("黑帽"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//形态学运算
void ImageEdgeExtractionDlg::OnCbnSelchangeMorphologyCombo()
{
	showSRCImage();
	UpdateData(TRUE);
	if(m_nStructElement%2 == 0)
	{
		MessageBox(_T("请输入一个奇数核值！"), MB_OK);
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
		MessageBox(_T("请输入源图像"));
		return;
	}
	//定义核
	Mat element_Kernel = getStructuringElement(MORPH_RECT, Size(m_nStructElement, m_nStructElement));
	int nSel = m_ComboMorphology.GetCurSel();
	switch(nSel)
	{
	case 0:
		//进行形态学操作  
		morphologyEx(src_image, temp_image, MORPH_OPEN, element_Kernel, Point(-1, -1), 2); 
		imwrite("./图像/边缘提取/开运算.png", temp_image);
		break;
	case 1:
		morphologyEx(src_image, temp_image, MORPH_CLOSE, element_Kernel, Point(-1, -1), 2);
		imwrite("./图像/边缘提取/闭运算.png", temp_image);
		break;
	case 2:
		morphologyEx(src_image, temp_image, MORPH_GRADIENT, element_Kernel, Point(-1, -1), 2);
		imwrite("./图像/边缘提取/形态学梯度.png", temp_image);
		break;
	case 3:
		morphologyEx(src_image, temp_image, MORPH_TOPHAT, element_Kernel, Point(-1, -1), 2);
		imwrite("./图像/边缘提取/顶帽.png", temp_image);
		break;
	case 4:
		morphologyEx(src_image, temp_image, MORPH_BLACKHAT, element_Kernel, Point(-1, -1), 2);
		imwrite("./图像/边缘提取/黑帽.png", temp_image);
		break;
	default:
		break;
	}
	Show(temp_image, IDC_DST_IMSHOW1);
	dst = temp_image.clone();
	UpdateData(FALSE);
}

//向主窗口传数据
void ImageEdgeExtractionDlg::OnBnClickedOk()
{
	CLaserDeicingDlg *dlg = (CLaserDeicingDlg*)AfxGetMainWnd();
	dlg->CannyImage = CannyImage;//Canny边缘检测
	dlg->SobelImage = SobelImage; //Sobel边缘检测
	dlg->LaplaceImage = LaplaceImage; //Laplace边缘检测
	dlg->ErodeImage = ErodeImage; //腐蚀图像
	dlg->DilateImage = DilateImage;//膨胀图像 
	dlg->dst = dst;
	EndDialog(0);
}

void ImageEdgeExtractionDlg::showSRCImage() {
	if (!src.empty())
	{
		Show(src, IDC_SRC_IMSHOW1);
	}
}
