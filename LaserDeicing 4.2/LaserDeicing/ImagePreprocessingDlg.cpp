// ImagePreprocessingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LaserDeicing.h"
#include "ImagePreprocessingDlg.h"
#include "afxdialogex.h"


// ImagePreprocessingDlg �Ի���

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


// ImagePreprocessingDlg ��Ϣ�������
void ImagePreprocessingDlg::OnBnClickedRoiButton()
{
	if(src.empty())
	{
		MessageBox(_T("��������Ҫ�����ͼ��"), MB_OK);
		return ;
	}
	else
	{
		UpdateData(true);
		//ROI������Ϊ0�������Ȥ����ȡΪԭͼsize
		if(g_nRoiX ==0 && g_nRoiY ==0 && g_nRoiWidth ==0 && g_nRoiHeight == 0)
		{
			srcROI = src.clone(); //����Ȥͼ����ԭͼ��
			Show(srcROI,IDC_DST_IMSHOW); //��ʾ������ͼ�񴰿�
			Mat img = src.clone(); //����ԭͼ��
			rectangle(img, 
				      Point(g_nRoiX, g_nRoiY), 
					  Point(g_nRoiX+g_nRoiWidth, g_nRoiY+g_nRoiHeight), 
					  Scalar(0, 0, 255), 
					  1);//�����ο�
			Show(img, IDC_SRC_IMSHOW); //��ʾ��Ŀ��ͼ�񴰿�
			imwrite("./ͼ��/ͼ��Ԥ����/imageROI.jpg", srcROI); //��ROIͼ�񣬴˴���ĺ�ԭͼ��һ��
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
				//Ҳ����image(Range(g_nRoiY, g_nRoiY + g_nRoiHeight + 1), Range(g_nRoiX, g_nRoiX + g_nRoiWidth + 1)); ǰһ��Range���У���һ�����У�ע�����Range�ķ�Χ��������Ҳ�����
				src(Rect(g_nRoiX, g_nRoiY, g_nRoiWidth, g_nRoiHeight)).copyTo(srcROI); //��ȡ����Ȥ��ROI������
				Show(srcROI, IDC_DST_IMSHOW); //��ʾ��Ŀ��ͼ�񴰿�
				Mat img = src.clone(); //����ԭͼ��
				rectangle(img,
					      Point(g_nRoiX, g_nRoiY), 
						  Point(g_nRoiX+g_nRoiWidth, 
						  g_nRoiY+g_nRoiHeight), 
						  Scalar(0, 0, 255),
						  1);//��img�ϻ����ο�
				Show(img, IDC_SRC_IMSHOW);//�����о��ο��ͼ����ʾ��Դͼ�񴰿�
				imwrite("./ͼ��/ͼ��Ԥ����/imageROI.jpg", srcROI); //��ѡȡ���ROIͼ��
			}
			else
			{
				MessageBox(_T("�����ú��ʵ�ROI������"), MB_OK);
				return ;
			}
		}
		dst = srcROI.clone();
		imageROI = srcROI;//���õ���roi����������
		CString s;
		//Format�������ַ�����ת�������˺ܴ�ķ��� int��float��double -> CString
		s.Format(_T("����Ȥ��������Ͻ�����Ϊ(%d, %d)������Ŀ��ֵΪ%d���߶�ֵΪ%d"), 
			     g_nRoiX, g_nRoiY, srcROI.size().width, srcROI.size().height);
		MessageBox(s, MB_OK);
		UpdateData(false);
	}
}

//�Ҷ�ͼ��ת��
void ImagePreprocessingDlg::OnBnClickedGrayButton()
{
	if (!gray.empty())
	{
		AfxMessageBox(_T("�Ѿ��ǻҶ�ͼ��"));
		return;
	}
	if(src.empty())
	{
		MessageBox(_T("��������Ҫ�����ͼ��"), MB_OK);
		return ;
	}
	else if (dst.empty())
	{
		cvtColor(src, gray, CV_BGR2GRAY);//ת���Ҷ�ͼ��
		Show(gray, IDC_DST_IMSHOW);	//���Ҷ�ͼ����ʾ���Ҷ�ͼ�񴰿�
		imwrite("./ͼ��/ͼ��Ԥ������/�Ҷ�ͼ��.jpg", gray); //��Ҷ�ͼ��
		dst = gray.clone(); //���ƻҶ�ͼ��-----���ͼ��
	}
	else
	{
		cvtColor(dst, gray, CV_BGR2GRAY);//ת���Ҷ�ͼ��
		Show(gray, IDC_DST_IMSHOW);	//���Ҷ�ͼ����ʾ���Ҷ�ͼ�񴰿�
		imwrite("./ͼ��/ͼ��Ԥ����/�Ҷ�ͼ��.jpg", gray); //��Ҷ�ͼ��
		dst = gray.clone(); //���ƻҶ�ͼ��-----���ͼ��
	}
}

//�Ҷ�����
void ImagePreprocessingDlg::OnBnClickedImagestretchingButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(_T("����"));
}

//���ղ���
void ImagePreprocessingDlg::OnBnClickedCompensationButton()
{
	if (src.empty())
	{
		MessageBox(_T("��������Ҫ�����ͼ��"), MB_OK);
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
		cvtColor(src_image, temp_image, COLOR_RGB2GRAY);  //����ǲ�ɫͼ����ת��Ϊ�Ҷ�ͼ
	}
	else
	{
		temp_image = src_image.clone();
	}
	double average = mean(temp_image)[0];        //���imageͼ��ĻҶ�ƽ��ֵ 
	int rows_new = ceil(double(temp_image.rows) / double(s_nBlockSize));   //����ÿ���ӿ������������
	int cols_new = ceil(double(temp_image.cols) / double(s_nBlockSize));   
	Mat blockImage;    
	blockImage = Mat::zeros(rows_new, cols_new, CV_32FC1);  //blockImage����Ҫ���ÿ���ӿ�ĻҶ�ƽ��ֵ
	for (int i = 0; i < rows_new; i++)     //���з���ÿ���ӿ�
	{  
		for (int j = 0; j < cols_new; j++)  
		{  
			int rowmin = i*s_nBlockSize;  		
			int rowmax = (i+1)*s_nBlockSize;  			
			if (rowmax > temp_image.rows) rowmax = temp_image.rows;  
			int colmin = j*s_nBlockSize;  
			int colmax = (j + 1)*s_nBlockSize;  
			if (colmax > temp_image.cols) colmax = temp_image.cols;    //�õ�ÿ���ӿ�Ŀ�ʼ�������ͽ���������
			Mat imageSmall = temp_image(Range(rowmin, rowmax), Range(colmin, colmax));   //�ӿ�ͼ��imageROI
			double temaver = mean(imageSmall)[0];       //�����ӿ�ͼ���ƽ��ֵ
			blockImage.at<float>(i, j) = temaver;     //��ֵ�������blockImage�У�
		}  
	}  
	blockImage = blockImage - average;    //�ӿ�ͼ��ҶȾ�ֵ��ȥimageͼ��ĻҶȾ�ֵ���õ���ֵ��ֵ����
	Mat blockImage2;  
	resize(blockImage, blockImage2, temp_image.size(), (0, 0), (0, 0), INTER_CUBIC);  //����ֵ����ͨ��˫���β�ֵ��������Ϊimgeͼ����
	Mat image2;  
	temp_image.convertTo(image2, CV_32FC1); //��imageת����CV_32FC1�Ĺ���С
	Mat dst = image2 - blockImage2;    //imageͼ���ȥ��ֵ���󣬵õ���������ͼ��
	dst.convertTo(temp_image, CV_8UC1); 	
	Show(temp_image, IDC_DST_IMSHOW);
	imwrite("./ͼ��/ͼ��Ԥ����/���ղ���ͼ��.jpg", temp_image);
	dst = temp_image.clone();
	COMPENSATION_image = temp_image;//���õ��Ĺ��ղ���ͼ�񴫵�������
}

//ֱ��ͼ����
void ImagePreprocessingDlg::OnBnClickedCalcHistButton()
{
	src_image = dst.clone();
	Mat temp_image;
	if(src_image.channels()==3)
	{
		cvtColor(src_image, temp_image, CV_RGB2GRAY);
		if(temp_image.empty())
		{
			MessageBox(_T("�Ҷ�ͼΪ�գ�"));
			return ;
		}
	}
	else
	{
		temp_image = src_image.clone();
	}
	int histSize = 256;        //ֱ��ͼ�ĺ���Ĵ�С��
	float range[] = {0, 256}; // ����range[2]��range[0]= 0, range[1]= 256;
	const float* histRange = {range};
	int channels[] = {0};
	Mat hist;	
	calcHist(&temp_image, 1, channels, Mat(), hist, 1, &histSize, &histRange, true, false);  //����ֱ��ͼ
	//imshow("histImage", hist);
	double max_val;      
	minMaxLoc(hist, 0, &max_val, 0, 0);
	int scale = 2;
	int hist_height = 256; //ֱ��ͼ������Ĵ�С
	Mat hist_img = Mat::zeros(hist_height, histSize*scale, CV_8UC3);
	for(int i=0; i<histSize; i++)
	{
		float bin_val = hist.at<float>(i);
		int intensity = cvRound(bin_val*hist_height/max_val);  //Ҫ���Ƶĸ߶�
		rectangle(hist_img, 
			      Point(i*scale, hist_height-1), 
				  Point((i+1)*scale-1, hist_height-intensity), 
				  CV_RGB(255, 0, 0));
	}   
	//Point(i*scale, hist_height-1)Ϊ���½ǵ����꣬Point((i+1)*scale-1, hist_height-intensity)Ϊ���Ͻ����꣬
	//imshow("ֱ��ͼ", hist_img);
	Show(hist_img, IDC_DST_IMSHOW);
/*
	if(m_isCam == false)  //δ������Ƶ�ɼ�״̬
	{
		DrawPicToHDC(hist_img, IDC_VIDEO);

	}
	else{
		imshow("ֱ��ͼ", hist_img);
	}
*/
}

//ֱ��ͼ���⻯
void ImagePreprocessingDlg::OnBnClickedEqualizeButton()
{
	src_image = dst.clone();
	Mat temp_image = src_image.clone();
	if(temp_image.empty())
	{
		MessageBox(_T("��������ͼ��ROI����"), MB_OK);
		return ;
	}
	if(temp_image.channels() >=3)      //��RGBͨ�����⻯��ͼ����ɫ��ʧ�������������ͨ��YCrCb���⻯������ᣬ
	{ 
		vector<Mat> channels;
		split(temp_image, channels);          //������ɫͨ����	
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
	imwrite("./ͼ��/ͼ��Ԥ����/ֱ��ͼ���⻯.jpg", temp_image);
	dst = temp_image.clone();
	equalizeImage = temp_image.clone();//��ֱ��ͼ���⻯ͼ�񴫵�������
}

//��ֵ�˲�
void ImagePreprocessingDlg::OnBnClickedBlurButton()
{
	UpdateData(TRUE);
	if (m_nBlurSize == 0)
	{
		AfxMessageBox(_T("���ڡ����á�������á���ֵ�˲��˴�С����"));
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
	blur(temp_image, temp_image, Size(m_nBlurSize, m_nBlurSize));  //����Size()�е�ֵ���ı��ֵ�˲������˲��ں˵ĳߴ��С��
	Show(temp_image, IDC_DST_IMSHOW);
	imwrite("./ͼ��/ͼ��Ԥ����/��ֵ�˲�.jpg", temp_image);
	UpdateData(FALSE);
	dst = temp_image.clone();
	blurImage = temp_image;
}

//��˹�˲�
void ImagePreprocessingDlg::OnBnClickedGaussianButton()
{
	UpdateData(TRUE);
	if (m_nGaussianSize == 0)
	{
		AfxMessageBox(_T("���ڡ����á�������á���˹�˲��˴�С����"));
		return;
	}
	if(m_nGaussianSize%2 == 0)
	{
		MessageBox(_T("��ȷ������ĺ˲���Ϊ������"), MB_OK);
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
	cv::GaussianBlur(temp_image, temp_image, Size(m_nGaussianSize, m_nGaussianSize), 0 , 0);   //Size()�ں��е�����ֵ���Բ�ͬ��������Ϊ����
	Show(temp_image, IDC_DST_IMSHOW);
	imwrite("./ͼ��/ͼ��Ԥ����/��˹�˲�.jpg", temp_image);
	UpdateData(FALSE);
	dst = temp_image.clone();
	GaussianBlurImage = temp_image;
}

//��ֵ�˲�
void ImagePreprocessingDlg::OnBnClickedMedianblurButton()
{
	UpdateData(TRUE);
	if (m_nMedianBlurSize == 0)
	{
		AfxMessageBox(_T("���ڡ����á�������á���ֵ�˲��˴�С����"));
		return;
	}
	if(m_nMedianBlurSize%2 ==0)
	{
		MessageBox(_T("��ȷ������ĺ˲���Ϊ������"), MB_OK);
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
	medianBlur(temp_image, temp_image, m_nMedianBlurSize);   //��ֵ�˲��ں˿׾������Գߴ磬����Ϊ����
	Show(temp_image, IDC_DST_IMSHOW);
	imwrite("./ͼ��/ͼ��Ԥ����/��ֵ�˲�.jpg", temp_image);
	UpdateData(FALSE);
	dst = temp_image.clone();
	MedianBlurImage = temp_image;
}

//˫���˲�
void ImagePreprocessingDlg::OnBnClickedBilateralfilterButton()
{
	UpdateData(TRUE);
	if (m_nBilateralSize == 0)
	{
		AfxMessageBox(_T("���ڡ����á�������á�˫���˲��˴�С����"));
		return;
	}
	if(m_nBilateralSize%2 ==0)
	{
		MessageBox(_T("��ȷ������ĺ˲���Ϊ������"), MB_OK);
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
	bilateralFilter(temp_image, temp_dst_imge, m_nBilateralSize, m_nBilateralSize*2, m_nBilateralSize/2);       //25��ʾÿ�����������ֱ����25*2��ʾ��ɫ�ռ��˲�����25/2��ʾ����ռ��˲���
	Show(temp_dst_imge, IDC_DST_IMSHOW);
	imwrite("./ͼ��/ͼ��Ԥ����/˫���˲�.jpg", temp_image);
	UpdateData(FALSE);
	dst = temp_dst_imge.clone();
	BilateralFilterImage = temp_image;
}

//���ļ�
void ImagePreprocessingDlg::OnBnClickedImagepreprocessongOpenFile()
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

//��ʾͼ��
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

//�ر��ļ�
void ImagePreprocessingDlg::OnBnClickedImagepreprocessingCloseFile()
{
	Mat temp(3, 3, CV_8UC3, Scalar(194, 194, 194));//����һ������ͼ
	Show(temp, IDC_SRC_IMSHOW);
	Show(temp, IDC_DST_IMSHOW);
	src.release();//����ͼ��
	dst.release();//���ͼ��
	src_image.release();//ÿ�δ���ʱ�����ʱ����
	srcROI.release();//��ȡROI���ͼ��
	gray.release();//�Ҷ�ͼ��
	imageROI.release();//ѡȡ��ROIͼ��
	COMPENSATION_image.release();//���ղ���ͼ��
	equalizeImage.release();//ֱ��ͼ���⻯ͼ��
	blurImage.release();//��ֵ�˲�
	GaussianBlurImage.release(); //��˹�˲�
	MedianBlurImage.release(); //��ֵ�˲�
	BilateralFilterImage.release();//˫���˲�
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (!src.empty())
	{
		Show(src, IDC_SRC_IMSHOW);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

