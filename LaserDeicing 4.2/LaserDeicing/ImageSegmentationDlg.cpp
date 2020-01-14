// ImageSegmentationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LaserDeicing.h"
#include "ImageSegmentationDlg.h"
#include "afxdialogex.h"


// ImageSegmentationDlg 对话框

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


// ImageSegmentationDlg 消息处理程序
//手动阈值分割
void ImageSegmentationDlg::OnBnClickedThresholdButton()
{
	showSRCImage();
	Mat temp_image;
	src_image = src.clone();
	UpdateData(TRUE);
	if(m_threshold>255 || m_threshold<=0)
	{
		MessageBox(_T("请确认正确的图像分割阈值！"), MB_OK);
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
		threshold(gray, temp_image, m_threshold, 255, THRESH_BINARY);//手动阈值法
		break;
	case 1:
		threshold(gray, temp_image, m_threshold, 255, THRESH_BINARY_INV);//手动阈值法
		break;
	case 2:
		threshold(gray, temp_image, m_threshold, 255, THRESH_TRUNC);//手动阈值法
		break;
	case 3:
		threshold(gray, temp_image, m_threshold, 255, THRESH_TOZERO);//手动阈值法
		break;
	case 4:
		threshold(gray, temp_image, m_threshold, 255, THRESH_TOZERO_INV);//手动阈值法
		break;
	case 6:
		threshold(gray, temp_image, m_threshold, 255, THRESH_OTSU);//手动阈值法
		break;
	default:
		MessageBox(_T("请选择合适的阈值类型"));
		break;
	}
	Show(temp_image, IDC_DST_IMSHOW);
	Image_Threshold = temp_image.clone();
	imwrite("./图像/图像分割/手动阈值分割.jpg", Image_Threshold);
	dst = Image_Threshold;
}

//大律法阈值分割
void ImageSegmentationDlg::OnBnClickedOtusButton()
{
	showSRCImage();
	Mat temp_image;
	src_image = src.clone();
	if(src_image.channels() >=3)
	{
		cvtColor(src_image, gray, COLOR_BGR2GRAY);
		Show(gray, IDC_DST_IMSHOW);
		//threshold(gray, temp_image, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY_INV);//大津法
		threshold(gray, temp_image, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY);//大津法
		Show(temp_image, IDC_DST_IMSHOW);
	}
	else
	{
		Show(src_image, IDC_DST_IMSHOW);
		//threshold(src_image, temp_image, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY_INV);//大津法
		threshold(src_image, temp_image, 0, 255, CV_THRESH_OTSU|CV_THRESH_BINARY);//大津法
		Show(temp_image, IDC_DST_IMSHOW);
	}
	Image_OstuThreshold = temp_image.clone();
	imwrite("./图像/图像分割/大律法阈值分割.jpg", Image_OstuThreshold);
	dst = Image_OstuThreshold;
}

//自适应阈值分割
void ImageSegmentationDlg::OnBnClickedAdaptiveButton()
{
	showSRCImage();
	UpdateData(TRUE);
	Mat temp_image;
	src_image = src.clone();
	if(m_BlockSize%2 ==0)
	{
		MessageBox("请确认输入的核参数为奇数！)", MB_OK);
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
	imwrite("./图像/图像分割/自适应阈值分割.jpg", Image_adaptiveThreshold);
	dst = Image_adaptiveThreshold;
}

//打开图片
void ImageSegmentationDlg::OnBnClickedOpenFile()
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
		Show(src, IDC_SRC_IMSHOW);
		filepath.ReleaseBuffer();
		return;
	}
	else
	{
		return;
	}
}

//关闭图片
void ImageSegmentationDlg::OnBnClickedCloseFile()
{
	Mat temp(3, 3, CV_8UC3, Scalar(194, 194, 194));//设置一副背景图
	Show(temp, IDC_SRC_IMSHOW);
	Show(temp, IDC_DST_IMSHOW);
	src.release();//输入图像
	dst.release();//输出图像
	src_image.release();//每次处理时候的临时变量
	gray.release();//灰度图像
	Image_Threshold.release();//手动阈值图像
	Image_OstuThreshold.release(); //大律法阈值图像
	Image_adaptiveThreshold.release(); //自适应阈值图像
}

//显示图像
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

	// TODO:  在此添加额外的初始化
	//设置阈值类型的值
	m_ComboThresholdType.InsertString(0,_T("THRESH_BINARY"));
	m_ComboThresholdType.InsertString(1,_T("THRESH_BINARY_INV"));
	m_ComboThresholdType.InsertString(2,_T("THRESH_TRUNC"));
	m_ComboThresholdType.InsertString(3,_T("THRESH_TOZERO"));
	m_ComboThresholdType.InsertString(4,_T("THRESH_TOZERO_INV"));
	m_ComboThresholdType.InsertString(5,_T("THRESH_MASK"));
	m_ComboThresholdType.InsertString(6,_T("THRESH_OTSU"));
	m_ComboThresholdType.SetCurSel(0);//阈值类型初始值
	//设置自适应方法
	m_ComboThresholdAdaptiveMethod.InsertString(0,_T("ADAPTIVE_THRESH_MEAN_C"));
	m_ComboThresholdAdaptiveMethod.InsertString(1,_T("ADAPTIVE_THRESH_GUASSIAN_C"));
	m_ComboThresholdAdaptiveMethod.SetCurSel(0);//阈值类型初始值
	//设置轮廓检索模式
	m_ComboModeType.InsertString(0, _T("CV_RETR_EXTERNAL"));//只检测外部轮廓
	m_ComboModeType.InsertString(1, _T("CV_RETR_LIST"));
	m_ComboModeType.InsertString(2, _T("CV_RETR_CCOMP"));
	m_ComboModeType.InsertString(3, _T("CV_RETR_TRE"));
	m_ComboModeType.SetCurSel(0);
	//定义轮廓的近似方法
	m_ComboMetodType.InsertString(0, _T("CV_CHAIN_APPROX_NONE"));
	m_ComboMetodType.InsertString(1, _T("CV_CHAIN_APPROX_SIMPLE"));
	m_ComboMetodType.InsertString(2, _T("CV_CHAIN_APPROX_TC89_L1"));
	m_ComboMetodType.InsertString(3, _T("CV_CHAIN_APPROX_TC89_KCOS"));
	m_ComboMetodType.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//提取外部轮廓
void ImageSegmentationDlg::OnBnClickedContoursButton()
{
	src_image = src.clone();
	show_img = imread("./图像/源图像/src.jpg");
	//vector<vector<Point>> contours;//存储每个轮廓，有多少组点集，就有多少元素
	//Vec4i每个元素包含4个int型变量
	//vector<Vec4i>保存了每个元素包含4个int变量的整型数组的容器，hierarchy[i][0] - hierarchy[i][3]
	//分别表示的i的后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有值为-1
	//vector<Vec4i> hierarchy;//元素的个数 = 轮廓数
	findContours(src_image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//CV_RETR_EXTERNAL只检测外部轮廓，可根据自身需求进行调整
	Mat contoursImage(src_image.rows, src_image.cols, CV_8U, Scalar(0));//可以设置提取轮廓的背景图
	int index = 0;
	for (; index >= 0; index = hierarchy[index][0]) {
		cv::drawContours(contoursImage, contours, index, Scalar(255), 1, 8, hierarchy);//描绘字符的外轮廓 index画第几个轮廓
		Rect rect = boundingRect(contours[index]);//检测外轮廓
		rectangle(show_img, rect, Scalar(0,0,255), 3);//对外轮廓加矩形框
		m_nCountourNum++;//输出检测到的轮廓数
	}
	imwrite("./图像/图像分割/连通域轮廓.jpg", contoursImage);
	dst = contoursImage;
	Show(contoursImage, IDC_DST_IMSHOW);
	UpdateData(FALSE);
}

bool ContoursSortFun(vector<cv::Point> contour1, vector<cv::Point> contour2) {
	return  (contourArea(contour1), contourArea(contour2)); // 面积
}

//按面积阈值提取轮廓
void ImageSegmentationDlg::OnBnClickedDstContoursButton()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	src_image = src.clone();
	show_img = imread("./图像/源图像/src.jpg");
	findContours(src_image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//CV_RETR_EXTERNAL只检测外部轮廓，可根据自身需求进行调整
	Mat contoursImage(src_image.rows, src_image.cols, CV_8U, Scalar(0));//可以设置提取轮廓的背景图
	//筛选剔除掉面积小于100的轮廓
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
		Rect rect = boundingRect(contours[i]);//检测外轮廓
		rectangle(show_img, rect, Scalar(0,0,255), 3);//对外轮廓加矩形框
	}
	m_nCountourNum = contours.size();
	imwrite("./图像/图像分割/连通域轮廓.jpg", contoursImage);
	dst = contoursImage;
	Show(contoursImage, IDC_DST_IMSHOW);
	UpdateData(FALSE);
}


void ImageSegmentationDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CLaserDeicingDlg *dlg = (CLaserDeicingDlg *)AfxGetMainWnd();
	dlg->Image_Threshold = Image_Threshold;//手动阈值图像
	dlg->Image_OstuThreshold = Image_OstuThreshold; //大律法阈值图像
	dlg->Image_adaptiveThreshold = Image_adaptiveThreshold; //自适应阈值图像
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
