// ImagePreprocessingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LaserDeicing.h"
#include "ImagePreprocessingDlg.h"
#include "afxdialogex.h"


// ImagePreprocessingDlg 对话框

IMPLEMENT_DYNAMIC(ImagePreprocessingDlg, CDialogEx)

ImagePreprocessingDlg::ImagePreprocessingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ImagePreprocessingDlg::IDD, pParent)
	, g_nRoiX(0)
	, g_nRoiY(0)
	, g_nRoiHeight(0)
	, g_nRoiWidth(0)
	, m_nBlurSize(0)
	, m_nGaussianSize(0)
	, m_nMedianBlurSize(0)
	, m_nBilateralSize(0)
	, flag(false)
{

}

ImagePreprocessingDlg::~ImagePreprocessingDlg()
{
}

void ImagePreprocessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ROI_X_EDIT, g_nRoiX);
	DDX_Text(pDX, IDC_ROIA_Y_EDIT, g_nRoiY);
	DDX_Text(pDX, IDC_ROI_HEIGHT_EDIT, g_nRoiWidth);
	DDX_Text(pDX, IDC_ROI_HEIGHT_EDIT, g_nRoiHeight);
	DDX_Text(pDX, IDC_ROI_WIDTH_EDIT, g_nRoiWidth);
	DDX_Text(pDX, IDC_BLUR_EDIT, m_nBlurSize);
	DDX_Text(pDX, IDC_GAUSSIAN_EDIT, m_nGaussianSize);
	DDX_Text(pDX, IDC_MEDIAN_EDIT, m_nMedianBlurSize);
	DDX_Text(pDX, IDC_BILATERAL_EDIT, m_nBilateralSize);
}


BEGIN_MESSAGE_MAP(ImagePreprocessingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ROI_BUTTON, &ImagePreprocessingDlg::OnBnClickedRoiButton)
	ON_BN_CLICKED(IDC_GRAY_BUTTON, &ImagePreprocessingDlg::OnBnClickedGrayButton)
	ON_BN_CLICKED(IDC_IMAGESTRETCHING_BUTTON, &ImagePreprocessingDlg::OnBnClickedImagestretchingButton)
	ON_BN_CLICKED(IDC_COMPENSATION_BUTTON, &ImagePreprocessingDlg::OnBnClickedCompensationButton)
	ON_BN_CLICKED(IDC_CALC_HIST_BUTTON, &ImagePreprocessingDlg::OnBnClickedCalcHistButton)
	ON_BN_CLICKED(IDC_EQUALIZE_BUTTON, &ImagePreprocessingDlg::OnBnClickedEqualizeButton)
	ON_BN_CLICKED(IDC_BLUR_BUTTON, &ImagePreprocessingDlg::OnBnClickedBlurButton)
	ON_BN_CLICKED(IDC_GAUSSIAN_BUTTON, &ImagePreprocessingDlg::OnBnClickedGaussianButton)
	ON_BN_CLICKED(IDC_MEDIANBLUR_BUTTON, &ImagePreprocessingDlg::OnBnClickedMedianblurButton)
	ON_BN_CLICKED(IDC_BILATERALFILTER_BUTTON, &ImagePreprocessingDlg::OnBnClickedBilateralfilterButton)
	ON_BN_CLICKED(IDC_IMAGEPREPROCESSONG_OPEN_FILE, &ImagePreprocessingDlg::OnBnClickedImagepreprocessongOpenFile)
	ON_BN_CLICKED(IDC_IMAGEPREPROCESSING_CLOSE_FILE, &ImagePreprocessingDlg::OnBnClickedImagepreprocessingCloseFile)
	ON_BN_CLICKED(IDOK, &ImagePreprocessingDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// ImagePreprocessingDlg 消息处理程序
void ImagePreprocessingDlg::OnBnClickedRoiButton()
{
	if(src.empty())
	{
		MessageBox(_T("请先输入要处理的图像！"), MB_OK);
		return ;
	}
	else
	{
		UpdateData(true);
		//ROI参数均为0，则感兴趣区域取为原图size
		if(g_nRoiX ==0 && g_nRoiY ==0 && g_nRoiWidth ==0 && g_nRoiHeight == 0)
		{
			srcROI = src.clone(); //感兴趣图像复制原图像
			Show(srcROI,IDC_DST_IMSHOW); //显示到输入图像窗口
			Mat img = src.clone(); //复制原图像
			rectangle(img, 
				      Point(g_nRoiX, g_nRoiY), 
					  Point(g_nRoiX+g_nRoiWidth, g_nRoiY+g_nRoiHeight), 
					  Scalar(0, 0, 255), 
					  1);//画矩形框
			Show(img, IDC_SRC_IMSHOW); //显示到目标图像窗口
			imwrite("./图像/图像预处理/imageROI.jpg", srcROI); //存ROI图像，此处存的和原图像一样
		}
		else
		{
			if(g_nRoiX>=0 
			   && g_nRoiY>=0 
			   && g_nRoiX<=src.cols 
			   && g_nRoiY<=src.rows 
			   && g_nRoiHeight>0 
			   && g_nRoiWidth>0 
			   && g_nRoiWidth+g_nRoiX<=src.cols 
			   && g_nRoiHeight+g_nRoiY<=src.rows )
			{
				//也可以image(Range(g_nRoiY, g_nRoiY + g_nRoiHeight + 1), Range(g_nRoiX, g_nRoiX + g_nRoiWidth + 1)); 前一个Range是行，后一个是列，注意的是Range的范围是左包含右不包含
				src(Rect(g_nRoiX, g_nRoiY, g_nRoiWidth, g_nRoiHeight)).copyTo(srcROI); //截取感兴趣（ROI）区域
				Show(srcROI, IDC_DST_IMSHOW); //显示到目标图像窗口
				Mat img = src.clone(); //复制原图像
				rectangle(img,
					      Point(g_nRoiX, g_nRoiY), 
						  Point(g_nRoiX+g_nRoiWidth, 
						  g_nRoiY+g_nRoiHeight), 
						  Scalar(0, 0, 255),
						  1);//在img上画矩形框
				Show(img, IDC_SRC_IMSHOW);//将带有矩形框的图像显示到源图像窗口
				imwrite("./图像/图像预处理/imageROI.jpg", srcROI); //存选取后的ROI图像
			}
			else
			{
				MessageBox(_T("请设置合适的ROI参数！"), MB_OK);
				return ;
			}
		}
		dst = srcROI.clone();
		imageROI = srcROI;//将得到的roi传到主窗口
		CString s;
		//Format方法给字符串的转换带来了很大的方便 int、float、double -> CString
		s.Format(_T("感兴趣区域的左上角坐标为(%d, %d)，区域的宽度值为%d，高度值为%d"), 
			     g_nRoiX, g_nRoiY, srcROI.size().width, srcROI.size().height);
		MessageBox(s, MB_OK);
		UpdateData(false);
	}
}

//灰度图像转换
void ImagePreprocessingDlg::OnBnClickedGrayButton()
{
	if (!gray.empty())
	{
		AfxMessageBox(_T("已经是灰度图像"));
		return;
	}
	if(src.empty())
	{
		MessageBox(_T("请先输入要处理的图像！"), MB_OK);
		return ;
	}
	else if (dst.empty())
	{
		cvtColor(src, gray, CV_BGR2GRAY);//转换灰度图像
		Show(gray, IDC_DST_IMSHOW);	//将灰度图像显示到灰度图像窗口
		imwrite("./图像/图像预处理理/灰度图像.jpg", gray); //存灰度图像
		dst = gray.clone(); //复制灰度图像-----输出图像
	}
	else
	{
		cvtColor(dst, gray, CV_BGR2GRAY);//转换灰度图像
		Show(gray, IDC_DST_IMSHOW);	//将灰度图像显示到灰度图像窗口
		imwrite("./图像/图像预处理/灰度图像.jpg", gray); //存灰度图像
		dst = gray.clone(); //复制灰度图像-----输出图像
	}
}

//灰度拉伸
void ImagePreprocessingDlg::OnBnClickedImagestretchingButton()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("测试"));
}

//光照补偿
void ImagePreprocessingDlg::OnBnClickedCompensationButton()
{
	if (src.empty())
	{
		MessageBox(_T("请先输入要处理的图像！"), MB_OK);
		return ;
	}
	else if (dst.empty())
	{
		dst = src.clone();
	}
	Mat temp_image;
	src_image = dst.clone();
	if (src_image.channels() == 3) 
	{
		cvtColor(src_image, temp_image, COLOR_RGB2GRAY);  //如果是彩色图，则转换为灰度图
	}
	else
	{
		temp_image = src_image.clone();
	}
	double average = mean(temp_image)[0];        //求出image图像的灰度平均值 
	int rows_new = ceil(double(temp_image.rows) / double(s_nBlockSize));   //计算每个子块的行数和列数
	int cols_new = ceil(double(temp_image.cols) / double(s_nBlockSize));   
	Mat blockImage;    
	blockImage = Mat::zeros(rows_new, cols_new, CV_32FC1);  //blockImage矩阵将要存放每个子块的灰度平均值
	for (int i = 0; i < rows_new; i++)     //行列访问每个子块
	{  
		for (int j = 0; j < cols_new; j++)  
		{  
			int rowmin = i*s_nBlockSize;  		
			int rowmax = (i+1)*s_nBlockSize;  			
			if (rowmax > temp_image.rows) rowmax = temp_image.rows;  
			int colmin = j*s_nBlockSize;  
			int colmax = (j + 1)*s_nBlockSize;  
			if (colmax > temp_image.cols) colmax = temp_image.cols;    //得到每个子块的开始行列数和结束行列数
			Mat imageSmall = temp_image(Range(rowmin, rowmax), Range(colmin, colmax));   //子块图像imageROI
			double temaver = mean(imageSmall)[0];       //计算子块图像的平均值
			blockImage.at<float>(i, j) = temaver;     //将值放入矩阵blockImage中，
		}  
	}  
	blockImage = blockImage - average;    //子块图像灰度均值减去image图像的灰度均值，得到均值差值矩阵
	Mat blockImage2;  
	resize(blockImage, blockImage2, temp_image.size(), (0, 0), (0, 0), INTER_CUBIC);  //将差值矩阵通过双三次差值方法扩大为imge图像规格
	Mat image2;  
	temp_image.convertTo(image2, CV_32FC1); //将image转换成CV_32FC1的规格大小
	Mat dst = image2 - blockImage2;    //image图像减去差值矩阵，得到补偿矩阵图像
	dst.convertTo(temp_image, CV_8UC1); 	
	Show(temp_image, IDC_DST_IMSHOW);
	imwrite("./图像/图像预处理/光照补偿图像.jpg", temp_image);
	dst = temp_image.clone();
	COMPENSATION_image = temp_image;//将得到的光照补偿图像传到主窗口
}

//直方图计算
void ImagePreprocessingDlg::OnBnClickedCalcHistButton()
{
	src_image = dst.clone();
	Mat temp_image;
	if(src_image.channels()==3)
	{
		cvtColor(src_image, temp_image, CV_RGB2GRAY);
		if(temp_image.empty())
		{
			MessageBox(_T("灰度图为空！"));
			return ;
		}
	}
	else
	{
		temp_image = src_image.clone();
	}
	int histSize = 256;        //直方图的横轴的大小，
	float range[] = {0, 256}; // 数组range[2]；range[0]= 0, range[1]= 256;
	const float* histRange = {range};
	int channels[] = {0};
	Mat hist;	
	calcHist(&temp_image, 1, channels, Mat(), hist, 1, &histSize, &histRange, true, false);  //计算直方图
	//imshow("histImage", hist);
	double max_val;      
	minMaxLoc(hist, 0, &max_val, 0, 0);
	int scale = 2;
	int hist_height = 256; //直方图的纵轴的大小
	Mat hist_img = Mat::zeros(hist_height, histSize*scale, CV_8UC3);
	for(int i=0; i<histSize; i++)
	{
		float bin_val = hist.at<float>(i);
		int intensity = cvRound(bin_val*hist_height/max_val);  //要绘制的高度
		rectangle(hist_img, 
			      Point(i*scale, hist_height-1), 
				  Point((i+1)*scale-1, hist_height-intensity), 
				  CV_RGB(255, 0, 0));
	}   
	//Point(i*scale, hist_height-1)为左下角的坐标，Point((i+1)*scale-1, hist_height-intensity)为右上角坐标，
	//imshow("直方图", hist_img);
	Show(hist_img, IDC_DST_IMSHOW);
/*
	if(m_isCam == false)  //未处于视频采集状态
	{
		DrawPicToHDC(hist_img, IDC_VIDEO);

	}
	else{
		imshow("直方图", hist_img);
	}
*/
}

//直方图均衡化
void ImagePreprocessingDlg::OnBnClickedEqualizeButton()
{
	src_image = dst.clone();
	Mat temp_image = src_image.clone();
	if(temp_image.empty())
	{
		MessageBox(_T("请先设置图像ROI区域！"), MB_OK);
		return ;
	}
	if(temp_image.channels() >=3)      //分RGB通道均衡化后图像颜色有失真情况，而亮度通道YCrCb均衡化结果不会，
	{ 
		vector<Mat> channels;
		split(temp_image, channels);          //分离颜色通道；	
		Mat B, G, R;
		equalizeHist(channels[0], B);
		equalizeHist(channels[1], G);
		equalizeHist(channels[2], R);
		vector<Mat> combines;
		combines.push_back(B);
		combines.push_back(G);
		combines.push_back(R);
		merge(combines, temp_image);
		cvtColor(temp_image, gray, CV_BGR2GRAY);
		Show(gray, IDC_DST_IMSHOW);
	}
	if(temp_image.channels() == 1)
	{
		equalizeHist(temp_image, temp_image);		
	}
	Show(temp_image, IDC_DST_IMSHOW);
	imwrite("./图像/图像预处理/直方图均衡化.jpg", temp_image);
	dst = temp_image.clone();
	equalizeImage = temp_image.clone();//将直方图均衡化图像传到主窗口
}

//均值滤波
void ImagePreprocessingDlg::OnBnClickedBlurButton()
{
	UpdateData(TRUE);
	if (m_nBlurSize == 0)
	{
		AfxMessageBox(_T("请在“设置”面板设置“均值滤波核大小”！"));
		return;
	}
	src_image = dst.clone();
	Mat temp_image;
	if(src_image.channels() == 3)
	{
		cvtColor(src_image, temp_image, CV_BGR2GRAY);
	}
	else
	{
		temp_image = src_image.clone();
	}
	blur(temp_image, temp_image, Size(m_nBlurSize, m_nBlurSize));  //调整Size()中的值来改变均值滤波器的滤波内核的尺寸大小，
	Show(temp_image, IDC_DST_IMSHOW);
	imwrite("./图像/图像预处理/均值滤波.jpg", temp_image);
	UpdateData(FALSE);
	dst = temp_image.clone();
	blurImage = temp_image;
}

//高斯滤波
void ImagePreprocessingDlg::OnBnClickedGaussianButton()
{
	UpdateData(TRUE);
	if (m_nGaussianSize == 0)
	{
		AfxMessageBox(_T("请在“设置”面板设置“高斯滤波核大小”！"));
		return;
	}
	if(m_nGaussianSize%2 == 0)
	{
		MessageBox(_T("请确认输入的核参数为奇数！"), MB_OK);
		return ;
	}
	Mat temp_image;
	src_image = dst.clone();
	if(src_image.channels() == 3)
	{
		cvtColor(src_image, temp_image, CV_BGR2GRAY);
	}
	else
	{
		temp_image = src_image.clone();
	}
	cv::GaussianBlur(temp_image, temp_image, Size(m_nGaussianSize, m_nGaussianSize), 0 , 0);   //Size()内核中的两个值可以不同，但必须为奇数
	Show(temp_image, IDC_DST_IMSHOW);
	imwrite("./图像/图像预处理/高斯滤波.jpg", temp_image);
	UpdateData(FALSE);
	dst = temp_image.clone();
	GaussianBlurImage = temp_image;
}

//中值滤波
void ImagePreprocessingDlg::OnBnClickedMedianblurButton()
{
	UpdateData(TRUE);
	if (m_nMedianBlurSize == 0)
	{
		AfxMessageBox(_T("请在“设置”面板设置“中值滤波核大小”！"));
		return;
	}
	if(m_nMedianBlurSize%2 ==0)
	{
		MessageBox(_T("请确认输入的核参数为奇数！"), MB_OK);
		return ;
	}
	src_image = dst.clone();
	Mat temp_image;
	if(src_image.channels() == 3)
	{
		cvtColor(src_image, temp_image, CV_BGR2GRAY);
	}
	else
	{
		temp_image = src_image.clone();
	}
	medianBlur(temp_image, temp_image, m_nMedianBlurSize);   //中值滤波内核孔径的线性尺寸，必须为奇数
	Show(temp_image, IDC_DST_IMSHOW);
	imwrite("./图像/图像预处理/中值滤波.jpg", temp_image);
	UpdateData(FALSE);
	dst = temp_image.clone();
	MedianBlurImage = temp_image;
}

//双边滤波
void ImagePreprocessingDlg::OnBnClickedBilateralfilterButton()
{
	UpdateData(TRUE);
	if (m_nBilateralSize == 0)
	{
		AfxMessageBox(_T("请在“设置”面板设置“双边滤波核大小”！"));
		return;
	}
	if(m_nBilateralSize%2 ==0)
	{
		MessageBox(_T("请确认输入的核参数为奇数！"), MB_OK);
		return ;
	}
	Mat temp_image;
	src_image = dst.clone();
	if(src_image.channels() == 3)
	{
		cvtColor(src_image, temp_image, CV_BGR2GRAY);
	}
	else
	{
		temp_image = src_image.clone();
	}
	Mat temp_dst_imge;
	bilateralFilter(temp_image, temp_dst_imge, m_nBilateralSize, m_nBilateralSize*2, m_nBilateralSize/2);       //25表示每个像素邻域的直径，25*2表示颜色空间滤波器，25/2表示坐标空间滤波器
	Show(temp_dst_imge, IDC_DST_IMSHOW);
	imwrite("./图像/图像预处理/双边滤波.jpg", temp_image);
	UpdateData(FALSE);
	dst = temp_dst_imge.clone();
	BilateralFilterImage = temp_image;
}

//打开文件
void ImagePreprocessingDlg::OnBnClickedImagepreprocessongOpenFile()
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

//显示图像
void ImagePreprocessingDlg::Show( Mat &src, UINT ID )
{
	if (flag == true)
	{
		return;
	}
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

//关闭文件
void ImagePreprocessingDlg::OnBnClickedImagepreprocessingCloseFile()
{
	Mat temp(3, 3, CV_8UC3, Scalar(194, 194, 194));//设置一副背景图
	Show(temp, IDC_SRC_IMSHOW);
	Show(temp, IDC_DST_IMSHOW);
	src.release();//输入图像
	dst.release();//输出图像
	src_image.release();//每次处理时候的临时变量
	srcROI.release();//截取ROI后的图像
	gray.release();//灰度图像
	imageROI.release();//选取的ROI图像
	COMPENSATION_image.release();//光照补偿图像
	equalizeImage.release();//直方图均衡化图像
	blurImage.release();//中值滤波
	GaussianBlurImage.release(); //高斯滤波
	MedianBlurImage.release(); //中值滤波
	BilateralFilterImage.release();//双边滤波
}


void ImagePreprocessingDlg::OnBnClickedOk()
{
	CLaserDeicingDlg *dlg = (CLaserDeicingDlg*)AfxGetMainWnd();
	dlg->gray = gray;
	dlg->imageROI = imageROI;
	dlg->COMPENSATION_image = COMPENSATION_image;
	dlg->equalizeImage = equalizeImage;
	dlg->blurImage = blurImage;
	dlg->GaussianBlurImage = GaussianBlurImage;
	dlg->MedianBlurImage = MedianBlurImage;
	dlg->BilateralFilterImage = BilateralFilterImage;
	dlg->dst = dst;
	EndDialog(0);
}

BOOL ImagePreprocessingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!src.empty())
	{
		Show(src, IDC_SRC_IMSHOW);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

