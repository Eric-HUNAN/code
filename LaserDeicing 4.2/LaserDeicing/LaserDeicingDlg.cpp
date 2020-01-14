#include "stdafx.h"
#include "LaserDeicing.h"
#include "LaserDeicingDlg.h"
#include "DlgPTZCruise.h"
#include <math.h>

void  on_mouse(int event, int x, int y, int flags, void* prarm);//定义on_mouse回调函数
static float getDistance(Point2f pointA, Point2f pointB);//定义求距离的函数

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	//对话框数据
	enum { IDD = IDD_ABOUTBOX };
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CLaserDeicingDlg 对话框

CLaserDeicingDlg::CLaserDeicingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLaserDeicingDlg::IDD, pParent)
	/*弹出窗口初始化*/
	,flag(TRUE)
	,p1(0.0,0.0)
	,p2(0.0,0.0)
	, m_strPoint1(_T(""))
	, m_strPoint2(_T(""))
	, m_strDistance(_T(""))
	, m_strPoint(_T(""))
{
	m_nDevPort = 8000;
	m_csUser = _T("admin");
	m_csPWD = _T("wangZENGZHUAN199");
	m_bIsLogin = FALSE;
//	m_lLoginID = -1;;
	m_bIsPlaying = FALSE;
	m_bIsRecording = FALSE;
	m_iCurChanIndex = -1;
	m_lPlayHandle = -1;
	m_bIsOnCruise = FALSE;
    m_bTrackRun = FALSE;
	m_bAutoOn = FALSE;
	m_bLightOn = FALSE;
    m_bWiperOn= FALSE;
	m_bFanOn= FALSE;
	m_bHeaterOn= FALSE;
	m_bAuxOn1= FALSE;
	m_bAuxOn2= FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CLaserDeicingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_JPG_QUALITY, m_coJpgQuality);
	DDX_Control(pDX, IDC_COMBO_JPG_SIZE, m_coJpgSize);
	DDX_Control(pDX, IDC_COMBO_PIC_TYPE, m_coPicType);
	//DDX_Control(pDX, IDC_COMBO_SEQ, m_comboSeq);//巡航轨迹下拉选项
	DDX_Control(pDX, IDC_COMBO_PRESET, m_comboPreset);
	DDX_Control(pDX, IDC_COMBO_PTZ_SPEED, m_comboPTZSpeed);
	DDX_Control(pDX, IDC_TREE_CHAN, m_ctrlTreeChan);
	DDX_Control(pDX, IDC_IPADDRESS_DEV, m_ctrlDevIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nDevPort);
	DDX_Text(pDX, IDC_EDIT_USER, m_csUser);
	DDX_Text(pDX, IDC_EDIT_PWD, m_csPWD);

	//弹出窗口选点
	DDX_Text(pDX, IDC_EDIT_P1, m_strPoint1);
	DDX_Text(pDX, IDC_EDIT_P2, m_strPoint2);
	DDX_Text(pDX, IDC_EDIT_DISTANCE, m_strDistance);
	DDX_Text(pDX, IDC_EDIT_POINT, m_strPoint);

	//信息状态提示栏
	DDX_Control(pDX, IDC_INFO, m_info);
}

BEGIN_MESSAGE_MAP(CLaserDeicingDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_CHAN, OnDblclkTreeChan)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CHAN, OnSelchangedTreeChan)
	ON_BN_CLICKED(IDC_BUTTON_RECORD, OnButtonRecord)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, OnButtonCapture)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_CBN_SELCHANGE(IDC_COMBO_PRESET, OnSelchangeComboPreset)
	ON_BN_CLICKED(IDC_BUTTON_PRESET_GOTO, OnButtonPresetGoto)
	ON_BN_CLICKED(IDC_BUTTON_PRESET_SET, OnButtonPresetSet)
	ON_BN_CLICKED(IDC_BUTTON_PRESET_DEL, OnButtonPresetDel)
	//ON_BN_CLICKED(IDC_BUTTON_SEQ_GOTO, OnButtonSeqGoto)//调用巡航
	//ON_BN_CLICKED(IDC_BUTTON_SEQ_SET, OnButtonSeqSet)//设置巡航轨迹，弹出DlgPTZCruise窗口
	//ON_BN_CLICKED(IDC_BUTTON_TRACK_RUN, OnButtonTrackRun)//运行轨迹记录
	//ON_BN_CLICKED(IDC_BUTTON_TRACK_START, OnButtonTrackStart)//开始轨迹记录
	//ON_BN_CLICKED(IDC_BUTTON_TRACK_STOP, OnButtonTrackStop)//停止轨迹记录
	ON_BN_CLICKED(IDC_BTN_PTZ_AUTO, OnBtnPtzAuto)
	//ON_BN_CLICKED(IDC_BTN_AUX_PWRON, OnBtnAuxPwron)//辅助设备1
	//ON_BN_CLICKED(IDC_BTN_FAN_PWRON, OnBtnFanPwron)//风扇
	//ON_BN_CLICKED(IDC_BTN_HEATER_PWRON, OnBtnHeaterPwron)//风扇
	//ON_BN_CLICKED(IDC_BTN_LIGHT_PWRON, OnBtnLightPwron)//灯光控制
	//ON_BN_CLICKED(IDC_BTN_WIPER_PWRON, OnBtnWiperPwron)//雨刷
	//ON_BN_CLICKED(IDC_BTN_AUX_PWRON2, OnBtnAuxPwron2)//辅助设备2
	ON_CBN_SELCHANGE(IDC_COMBO_PIC_TYPE, OnSelchangeComboPicType)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CLaserDeicingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_IMAGE_PREPROCESSING_BUTTON, &CLaserDeicingDlg::OnBnClickedImagePreprocessingButton)
	ON_BN_CLICKED(IDC_IMAGE_SEGMENTATION_BUTTON, &CLaserDeicingDlg::OnBnClickedImageSegmentationButton)
	ON_BN_CLICKED(IDC_IMAGE_EDGEEXTRACTION_BUTTON, &CLaserDeicingDlg::OnBnClickedImageEdgeextractionButton)
	ON_BN_CLICKED(IDC_OPENFILE_BUTTON1, &CLaserDeicingDlg::OnBnClickedOpenfileButton1)
	ON_BN_CLICKED(IDC_UPDATE_BUTTON, &CLaserDeicingDlg::OnBnClickedUpdateButton)
	ON_BN_CLICKED(IDC_CLINK_ON_BUTTON, &CLaserDeicingDlg::OnBnClickedClinkOnButton)
	ON_BN_CLICKED(IDC_BUTTON_INFO_CLC, &CLaserDeicingDlg::OnBnClickedButtonInfoClc)
	ON_BN_CLICKED(IDC_SetPtzPos_BUTTON, &CLaserDeicingDlg::OnBnClickedSetptzposButton)
END_MESSAGE_MAP()

// CLaserDlg 消息处理程序

BOOL CLaserDeicingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	// TODO: 在此添加额外的初始化代码
	//默认IP
	m_ctrlDevIp.SetAddress(192,168,1,64);

	//云台速度
	m_comboPTZSpeed.SetCurSel(5);

    //云台按钮
	//SubclassDlgItem子类化一个控件
	//把对话框中已有的控件与某个窗口对象动态连接起来，改窗口对象接管控件的消息处理，从而使控件具有新的特性
	m_btnPtzUp.SubclassDlgItem(IDC_BTN_PTZ_UP,this);
	m_btnPtzDown.SubclassDlgItem(IDC_BTN_PTZ_DOWN,this);
	m_btnPtzLeft.SubclassDlgItem(IDC_BTN_PTZ_LEFT,this);
	m_btnPtzRight.SubclassDlgItem(IDC_BTN_PTZ_RIGHT,this);	
	m_btnZoomOut.SubclassDlgItem(IDC_BTN_ZOOM_OUT,this);
	m_btnZoomIn.SubclassDlgItem(IDC_BTN_ZOOM_IN,this);
	m_btnFocusNear.SubclassDlgItem(IDC_BTN_FOCUS_NEAR,this);
	m_btnFocusFar.SubclassDlgItem(IDC_BTN_FOCUS_FAR,this);
	m_btnIrisOpen.SubclassDlgItem(IDC_BTN_IRIS_OPEN,this);
	m_btnIrisClose.SubclassDlgItem(IDC_BTN_IRIS_CLOSE,this);
    m_btnPtzUpleft.SubclassDlgItem(IDC_BTN_PTZ_UPLEFT,this);
    m_btnPtzUpright.SubclassDlgItem(IDC_BTN_PTZ_UPRIGHT,this);
	m_btnPtzDownleft.SubclassDlgItem(IDC_BTN_PTZ_DOWNLEFT,this);
    m_btnPtzDownright.SubclassDlgItem(IDC_BTN_PTZ_DOWNRIGHT,this);

	//抓图combo
	m_coPicType.SetCurSel(1);
	m_coJpgSize.SetCurSel(0);
	m_coJpgQuality.SetCurSel(0);
	//GetDlgItem(IDC_STATIC_JPGPARA)->EnableWindow(TRUE);
	//m_coJpgSize.EnableWindow(TRUE);
	//m_coJpgQuality.EnableWindow(TRUE);

	//运行状态信息提示栏初始化
	m_info.AddString("运行状态信息：");
	m_info.SetTopIndex(1);

	return TRUE;   // 除非将焦点设置到控件，否则返回 TRUE
}

void CLaserDeicingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLaserDeicingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLaserDeicingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//下面是相机二次开发程序
//设置云台位置信息
void CLaserDeicingDlg::OnBnClickedSetptzposButton()
{
	// TODO: 在此添加控件通知处理程序代码
	SetPtzPosDlg dlg;
	dlg.DoModal();
}

//注册/注销 按钮
void CLaserDeicingDlg::OnButtonLogin() 
{
	
	if(!m_bIsLogin)    //注册
	{
		if(!DoLogin())
			return;
		DoGetDeviceResoureCfg();//获取设备资源信息	
		CreateDeviceTree();//创建通道树
		GetDecoderCfg();//获取云台解码器信息
		InitDecoderReferCtrl();//初始化解码器相关控件      
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText("注销");
		m_bIsLogin = TRUE;
	}
	else      //注销
	{
		if(m_bIsPlaying|| m_bIsRecording)
		{
			MessageBox("Stop Play or record first!");
			return;
		}
        NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID); //用户注销
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText("登录");
		m_ctrlTreeChan.DeleteAllItems();//删除设备树的所有信息
		m_struDeviceInfo.lLoginID = -1;
		m_bIsLogin = FALSE;
	}
}

//DoLogin，向设备注册
BOOL CLaserDeicingDlg::DoLogin()
{
	UpdateData(TRUE);
	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);//获取IP地址
	
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;//设备参数的结构体
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));//结构体内存空间初始化
	
	//向设备注册，ip地址，设备的端口号，用户名，密码，设备信息
	//return -1表示失败，0表示没有错误，1表示用户名或者密码错误，3表示SDK未初始化
	LONG lLoginID = NET_DVR_Login_V30(DeviceIp.GetBuffer(DeviceIp.GetLength()),m_nDevPort, \
		m_csUser.GetBuffer(m_csUser.GetLength()),m_csPWD.GetBuffer(m_csPWD.GetLength()),&DeviceInfoTmp);
	if(lLoginID == -1)
	{
		m_info.AddString("相机[info:注册失败!]\n");
		return FALSE;
	}
	if(lLoginID == 1)
	{
		m_info.AddString("相机[info:用户名或密码错误!]\n");
		return FALSE;
	}
	if(lLoginID == 7)
	{
		m_info.AddString("相机[info:连接设备失败，检查电脑IP地址!]\n");
		return FALSE;
	}
    m_struDeviceInfo.lLoginID = lLoginID;//注册返回的值
	m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;//设备的通道数
    m_struDeviceInfo.iIPChanNum = DeviceInfoTmp.byIPChanNum;//最大数字通道个数
    m_struDeviceInfo.iStartChan  = DeviceInfoTmp.byStartChan;//设备开始通道号
    m_struDeviceInfo.iIPStartChan  = DeviceInfoTmp.byStartDChan;//数字通道起始通道号
	return TRUE;
}

//DoGetDeviceResoureCfg  获取设备的通道资源
void CLaserDeicingDlg::DoGetDeviceResoureCfg()
{
	NET_DVR_IPPARACFG_V40 IpAccessCfg;//IP设备资源及IP通道资源的结构体
	memset(&IpAccessCfg,0,sizeof(IpAccessCfg));	//分配内存空间
	DWORD  dwReturned;//unsigned long

	//是否支持IP接入
	//NET_DVR_GetDVRConfig是否支持IP接入
	m_struDeviceInfo.bIPRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID,NET_DVR_GET_IPPARACFG_V40,0,&IpAccessCfg,sizeof(NET_DVR_IPPARACFG_V40),&dwReturned);
	int i;
    if(!m_struDeviceInfo.bIPRet)   //不支持ip接入,9000以下设备不支持禁用模拟通道
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)
		{
			if (i < m_struDeviceInfo.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName,"camera%d",i+m_struDeviceInfo.iStartChan);
				m_struDeviceInfo.struChanInfo[i].iChanIndex=i+m_struDeviceInfo.iStartChan;  //通道号
				m_struDeviceInfo.struChanInfo[i].bEnable = TRUE;
				
			}
			else
			{
				m_struDeviceInfo.struChanInfo[i].iChanIndex = -1;
				m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName, "");	
			}
		}
	}
	else        //支持IP接入，9000设备
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)  //模拟通道
		{
			if (i < m_struDeviceInfo.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName,"camera%d",i+m_struDeviceInfo.iStartChan);
				m_struDeviceInfo.struChanInfo[i].iChanIndex=i+m_struDeviceInfo.iStartChan;
				if(IpAccessCfg.byAnalogChanEnable[i])
				{
					m_struDeviceInfo.struChanInfo[i].bEnable = TRUE;
				}
				else
				{
					m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				}
			}
			else//clear the state of other channel
			{
				m_struDeviceInfo.struChanInfo[i].iChanIndex = -1;
				m_struDeviceInfo.struChanInfo[i].bEnable = FALSE;
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName, "");	
			}		
		}

		//数字通道
		for(i=0; i<MAX_IP_CHANNEL; i++)
		{
			if(IpAccessCfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)  //ip通道在线
			{
				m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].bEnable = TRUE;
                m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].iChanIndex = i+IpAccessCfg.dwStartDChan;
				sprintf(m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].chChanName,"IP Camera %d",i+1);

			}
			else
			{
               m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].bEnable = FALSE;
			    m_struDeviceInfo.struChanInfo[i+MAX_ANALOG_CHANNUM].iChanIndex = -1;
			}
		}
	}
}

//创建通道树
void CLaserDeicingDlg::CreateDeviceTree()
{
	m_hDevItem = m_ctrlTreeChan.InsertItem("Dev");
	m_ctrlTreeChan.SetItemData(m_hDevItem,DEVICETYPE*1000);//赋值
    for(int i=0; i<MAX_CHANNUM_V30; i++)
	{
		if(m_struDeviceInfo.struChanInfo[i].bEnable)  //通道有效，插入通道树
		{
             HTREEITEM ChanItem = m_ctrlTreeChan.InsertItem(m_struDeviceInfo.struChanInfo[i].chChanName,m_hDevItem);
			 m_ctrlTreeChan.SetItemData(ChanItem,CHANNELTYPE*1000+i);   //Data对应通道在数组中的索引
		}
	}
	m_ctrlTreeChan.Expand(m_hDevItem,TVE_EXPAND);
}


//双击通道数，播放选中通道
void CLaserDeicingDlg::OnDblclkTreeChan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hSelected = m_ctrlTreeChan.GetSelectedItem();
	//未选中
	if(NULL == hSelected)
		return;
	DWORD itemData = m_ctrlTreeChan.GetItemData(hSelected);
	HTREEITEM hParent = NULL;
	int itype = itemData/1000;    
	int iIndex = itemData%1000;

	switch(itype)
	{
		case DEVICETYPE:
			m_iCurChanIndex = -1;
			break;
		case CHANNELTYPE:
			m_iCurChanIndex = iIndex;
			TRACE("select chan: %d\n",iIndex);//debug调试时候的输出信息
			DbPlayChannel(iIndex);
			OnSelchangeComboPreset();
			break;
		default:
			break;
	}
	*pResult = 0;
}

//双击播放
void CLaserDeicingDlg::DbPlayChannel(int ChanIndex/*通道号*/)
{
 
	if(!m_bIsPlaying)  //Play
	{
		StartPlay(ChanIndex);
	}
	else                //Stop,play
	{
        StopPlay();
		StartPlay(ChanIndex);

	}
}

//开始一路播放
void CLaserDeicingDlg::StartPlay(int iChanIndex)
{
	NET_DVR_CLIENTINFO ClientInfo;//预览参数结构体
	ClientInfo.hPlayWnd     = GetDlgItem(IDC_STATIC_PLAY)->m_hWnd;//播放窗口的句柄，为NULL表示不显示图像
	ClientInfo.lChannel     = m_iCurChanIndex+1;//通道号
	ClientInfo.lLinkMode    = 0;//连接模式  TCP模式
    ClientInfo.sMultiCastIP = NULL;//多播组地址
	TRACE("Channel number:%d\n",ClientInfo.lChannel);//输出通道信息
	m_lPlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID,&ClientInfo,NULL,NULL,TRUE);//实时预览
	if(-1 == m_lPlayHandle)  //失败
	{
		DWORD err=NET_DVR_GetLastError();//返回错误码
		CString m_csErr;
        m_csErr.Format("播放出错，错误代码%d",err);
		MessageBox(m_csErr);
	}

	m_bIsPlaying = TRUE;
	GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText("停止播放");
}

//停止播放
void CLaserDeicingDlg::StopPlay()
{
	if(m_lPlayHandle != -1)  //实时预览的返回值
	{
		if(m_bIsRecording)  //正在录像，先停止
		{
			StopRecord();
		}
		NET_DVR_StopRealPlay(m_lPlayHandle); //停止预览，true表示成功，false表示失败
		m_lPlayHandle=-1;
		m_bIsPlaying = FALSE;
		GetDlgItem(IDC_STATIC_PLAY)->Invalidate();
	}
}

//获取通道中的信号
void CLaserDeicingDlg::OnSelchangedTreeChan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hSelected = m_ctrlTreeChan.GetSelectedItem();
	//未选中
	if(NULL == hSelected)
		return;
	DWORD itemData = m_ctrlTreeChan.GetItemData(hSelected);
	HTREEITEM hParent = NULL;
	int itype = itemData/1000;    //
	int iIndex = itemData%1000;
	
	switch(itype)
	{
	case DEVICETYPE:
        m_iCurChanIndex = -1;
		break;
	case CHANNELTYPE:
		m_iCurChanIndex = iIndex;
		OnSelchangeComboPreset();
		TRACE("select chan index: %d\n",iIndex);
		break;
	default:
		break;
		
	}
	*pResult = 0;
}

//获取当前选中的通道号
int CLaserDeicingDlg::GetCurChanIndex()
{
    return m_iCurChanIndex;
}

//获取播放句柄
LONG CLaserDeicingDlg::GetPlayHandle()
{
    return m_lPlayHandle;
}

//获取云台速度
int CLaserDeicingDlg::GetPTZSpeed()
{
   return (m_comboPTZSpeed.GetCurSel());
}

//开始/停止录像
void CLaserDeicingDlg::OnButtonRecord() 
{
	if(m_lPlayHandle == -1) //实时预览没有开启
	{
        MessageBox("请先选择一个通道播放");
		return;
	}
	if(!m_bIsRecording) //录像标识
	{
		StartRecord();
	}
	else
	{
        StopRecord();
	}
	
}

//开始录像
void CLaserDeicingDlg::StartRecord()
{
	char RecName[256] = {0};
	
	CTime CurTime = CTime::GetCurrentTime();
	sprintf(RecName,"%04d%02d%02d%02d%02d%02d_ch%02d.mp4",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
		CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond(),m_struDeviceInfo.struChanInfo[GetCurChanIndex()].iChanIndex);

	 if(!NET_DVR_SaveRealData(m_lPlayHandle,RecName))//NET_DVR_SaveRealData捕获数据并存储到指定的文件中，true表示成功，false表示失败
	 {
		 MessageBox("启动录像失败");
		 return;
	 }
     m_bIsRecording = TRUE;//录像标识
	 GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("停止录像");
}

//停止录像
void CLaserDeicingDlg::StopRecord()
{
    if(!NET_DVR_StopSaveRealData(m_lPlayHandle)) //停止数据的捕获
	{
		MessageBox("停止录像失败");
		return;
	}
	m_bIsRecording = FALSE;//录像标识
	 GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("录像");
}

//抓图
void CLaserDeicingDlg::OnButtonCapture() 
{
	if(m_lPlayHandle == -1)
	{
        MessageBox("请先选择一个通道播放");
		return;
	}
	UpdateData(TRUE);

	char PicName[256] = {0};//图片文件名
	
	int iPicType = m_coPicType.GetCurSel();//保存图片的格式
	if(0 == iPicType)  //bmp
	{
		CTime CurTime = CTime::GetCurrentTime();//获取当前时间
		sprintf(PicName,"./图像/源图像/%04d%02d%02d%02d%02d%02d_ch%02d.bmp",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
			CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond(),m_struDeviceInfo.struChanInfo[GetCurChanIndex()].iChanIndex);//抓图图像的文件名
    
		if(NET_DVR_CapturePicture(m_lPlayHandle,PicName))//预览时，单帧数据捕获并保存成图片
		{
			CString info;
			info.Format("相机[info:抓图成功!图片保存路径:%s]",PicName);
			m_info.AddString(info);
		}
	}
	else if(1 == iPicType)  //jgp
	{
		CTime CurTime = CTime::GetCurrentTime();;
		sprintf(PicName,"./图像/源图像/%04d%02d%02d%02d%02d%02d_ch%02d.jpg",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
			CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond(),m_struDeviceInfo.struChanInfo[GetCurChanIndex()].iChanIndex);
    
		//组建jpg结构
		NET_DVR_JPEGPARA JpgPara = {0};
        JpgPara.wPicSize = (WORD)m_coJpgSize.GetCurSel();
		JpgPara.wPicQuality = (WORD)m_coJpgQuality.GetCurSel();

		LONG iCurChan = m_struDeviceInfo.struChanInfo[GetCurChanIndex()].iChanIndex;

		if(NET_DVR_CaptureJPEGPicture(m_struDeviceInfo.lLoginID, iCurChan, &JpgPara, PicName))
		{
			CString info;
			info.Format("相机[info:抓图成功!图片保存路径:%s]",PicName);
			m_info.AddString(info);
		}
	}
	return;	
}

//播放选中通道
void CLaserDeicingDlg::OnButtonPlay() 
{
	if(m_iCurChanIndex == -1)
	{
		MessageBox("选择一个通道");
		return;
	}
	if(!m_bIsPlaying)
	{
         StartPlay(m_iCurChanIndex);
		 m_bIsPlaying = TRUE;
		 GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText("停止播放");

	}
	else
	{
		StopPlay();
		m_bIsPlaying = FALSE;
		GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText("播放");
	}
	
}

//获取云台解码信息
void CLaserDeicingDlg::GetDecoderCfg()
{
     NET_DVR_DECODERCFG_V30 DecoderCfg;//通道解码器（云台）参数结构体
	 DWORD  dwReturned;
	 BOOL bRet;//存储获取设备的配置信息
		
	 //获取通道解码器信息
	 for(int i=0; i<MAX_CHANNUM_V30; i++)
	 {
		 if(m_struDeviceInfo.struChanInfo[i].bEnable)
		 {
			 memset(&DecoderCfg,0,sizeof(NET_DVR_DECODERCFG_V30));//分配内存空间
			 //获取设备的配置信息
			 bRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID,NET_DVR_GET_DECODERCFG_V30 , \
				 m_struDeviceInfo.struChanInfo[i].iChanIndex,&DecoderCfg,sizeof(NET_DVR_DECODERCFG_V30),&dwReturned);
			 if(!bRet)
			 {
				 TRACE("获取设备信息失败,通道:%d\n",m_struDeviceInfo.struChanInfo[i].iChanIndex);
				 continue;
			 }

			 memcpy(&m_struDeviceInfo.struChanInfo[i].struDecodercfg,&DecoderCfg,sizeof(NET_DVR_DECODERCFG_V30));
		 }
	 }

}

//初始化云台控制相关控件
void CLaserDeicingDlg::InitDecoderReferCtrl()
{
    int i;
	CString tmp;
	//设置预置点
	for(i=0; i<MAX_PRESET_V30; i++)
	{  
		tmp.Format("%d",i+1);     //i+1
		m_comboPreset.AddString(tmp);
	}
	m_comboPreset.SetCurSel(0);

	GetDlgItem(IDC_BUTTON_PRESET_GOTO)->EnableWindow(FALSE);//调用预置点
    //GetDlgItem(IDC_BUTTON_PRESET_SET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PRESET_DEL)->EnableWindow(FALSE);//删除预置点

	//巡航轨迹
	/*
    for(i=0; i<MAX_CRUISE_SEQ; i++)
	{
         tmp.Format("%d",i+1);     //i+1
		 m_comboSeq.AddString(tmp);
	}
	m_comboSeq.SetCurSel(0);*/
}

//预置点combobox，改变按钮状态
void CLaserDeicingDlg::OnSelchangeComboPreset() 
{
	int iIndex = m_comboPreset.GetCurSel();//获取预置点下拉选项    
	
	if(m_struDeviceInfo.struChanInfo[m_iCurChanIndex].struDecodercfg.bySetPreset[iIndex])//判断是否设置了这个预置点信息
	{
        GetDlgItem(IDC_BUTTON_PRESET_GOTO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PRESET_DEL)->EnableWindow(TRUE);
	}
	else
	{
        GetDlgItem(IDC_BUTTON_PRESET_GOTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PRESET_DEL)->EnableWindow(FALSE);
	}
}

//调用预置点
void CLaserDeicingDlg::OnButtonPresetGoto() 
{
	int iPreset = m_comboPreset.GetCurSel()+1;    //+1
	if(m_lPlayHandle >= 0)
	{
		if(!NET_DVR_PTZPreset(m_lPlayHandle,GOTO_PRESET,iPreset))//云台预置点操作（需先启动预览）
		{
			MessageBox("调用预置点失败");
			return;
		}
	}
	else
	{
		//云台预置点操作
		if(!NET_DVR_PTZPreset_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
			                  GOTO_PRESET,iPreset))
		{
			MessageBox("调用预置点失败");
			return;
		}

	}
}

//设置预置点
void CLaserDeicingDlg::OnButtonPresetSet() 
{
	int iPreset = m_comboPreset.GetCurSel()+1;    //+1
	if(m_lPlayHandle >= 0)
	{
		if(!NET_DVR_PTZPreset(m_lPlayHandle,SET_PRESET,iPreset))
		{
			MessageBox("设置预置点失败");
			return;
		}
	}
	else
	{
		if(!NET_DVR_PTZPreset_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
			SET_PRESET,iPreset))
		{
			MessageBox("设置预置点失败");
			return;
		}
		
	}

	//添加到预置点信息
	m_struDeviceInfo.struChanInfo[m_iCurChanIndex].struDecodercfg.bySetPreset[iPreset-1] = TRUE;
	//更新按钮状态
    OnSelchangeComboPreset();
	
}

//删除预置点
void CLaserDeicingDlg::OnButtonPresetDel() 
{
	int iPreset = m_comboPreset.GetCurSel()+1;    //+1
	if(m_lPlayHandle >= 0)
	{
		if(!NET_DVR_PTZPreset(m_lPlayHandle,CLE_PRESET,iPreset))
		{
			MessageBox("删除预置点失败");
			return;
		}
	}
	else
	{
		if(!NET_DVR_PTZPreset_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
			CLE_PRESET,iPreset))
		{
			MessageBox("删除预置点失败");
			return;
		}
		
	}
	
	//添加到预置点信息
	m_struDeviceInfo.struChanInfo[m_iCurChanIndex].struDecodercfg.bySetPreset[iPreset-1] = FALSE;
	//更新按钮状态
    OnSelchangeComboPreset();
	
}

//调用/停止 巡航
/*
void CLaserDeicingDlg::OnButtonSeqGoto() 
{
	int iSeq = m_comboSeq.GetCurSel()+1;    //+1
	if(!m_bIsOnCruise)
	{
		if(m_lPlayHandle >= 0)
		{
	    	if(!NET_DVR_PTZCruise(m_lPlayHandle,RUN_SEQ,iSeq,0,0))
			{
		    	MessageBox("调用巡航失败");
		    	return;
			}
		}
	    else
		{
	    	if(!NET_DVR_PTZCruise_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
		                             	RUN_SEQ,iSeq,0,0))
			{
		    	MessageBox("调用巡航失败");
		    	return;
			}
		
		}
        m_bIsOnCruise = TRUE;
		GetDlgItem(IDC_BUTTON_SEQ_GOTO)->SetWindowText("停止");
	}
	else
	{
        if(m_lPlayHandle >= 0)
		{
			if(!NET_DVR_PTZCruise(m_lPlayHandle,STOP_SEQ,iSeq,0,0))
			{
				MessageBox("停止巡航失败");
				return;
			}
		}
		else
		{
			if(!NET_DVR_PTZCruise_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
				STOP_SEQ,iSeq,0,0))
			{
				MessageBox("停止巡航失败");
				return;
			}
			
		}
		m_bIsOnCruise = FALSE;
		GetDlgItem(IDC_BUTTON_SEQ_GOTO)->SetWindowText("调用");
	}
	
}*/

//设置巡航路径
/*
void CLaserDeicingDlg::OnButtonSeqSet() 
{
	CDlgPTZCruise Dlg;
	Dlg.DoModal();
	
}*/

//开始运行轨迹
/*
void CLaserDeicingDlg::OnButtonTrackRun() 
{
	if(!m_bTrackRun)
	{
		if(m_lPlayHandle >= 0)
		{
			if(!NET_DVR_PTZTrack(m_lPlayHandle,RUN_CRUISE))
			{
				MessageBox("运行轨迹失败");
			}
		}
		else
		{
			if(!NET_DVR_PTZTrack(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex))
			{
				MessageBox("运行轨迹失败");
			}
		}
		m_bTrackRun = TRUE;
		GetDlgItem(IDC_BUTTON_TRACK_RUN)->SetWindowText("停止");    
	}
	else
	{
        //随便发送一个云台控制命令停止运行
        if(m_lPlayHandle >= 0)
		{
			if(!NET_DVR_PTZControl(m_lPlayHandle,TILT_UP,1))
			{
				MessageBox("停止轨迹失败");
			}
		}
		else
		{
			if(!NET_DVR_PTZControl_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex,TILT_UP,1))
			{
				MessageBox("停止轨迹失败");
			}
		}
		m_bTrackRun = FALSE;
		GetDlgItem(IDC_BUTTON_TRACK_RUN)->SetWindowText("运行");   
	}
}*/

//开始记录轨迹
/*
void CLaserDeicingDlg::OnButtonTrackStart() 
{
	
	if(m_lPlayHandle >= 0)
	{
		if(!NET_DVR_PTZTrack(m_lPlayHandle,STA_MEM_CRUISE))
		{
			MessageBox("开始记录轨迹失败");
			return;
		}

	}
	else
	{
       if(!NET_DVR_PTZTrack_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex,STA_MEM_CRUISE))
	   {
		   MessageBox("开始记录轨迹失败");
			return;
	   }
	}
}*/

//停止记录轨迹
/*
void CLaserDeicingDlg::OnButtonTrackStop() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!NET_DVR_PTZTrack(m_lPlayHandle,STO_MEM_CRUISE))
		{
			MessageBox("停止失败");
			return;
		}
		
	}
	else
	{
		if(!NET_DVR_PTZTrack_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex,STO_MEM_CRUISE))
		{
			MessageBox("停止失败");
			return;
		}
	}
}*/

//云台开始/停止左右自动扫描
void CLaserDeicingDlg::OnBtnPtzAuto() 
{
	int iSpeed = GetPTZSpeed();
	if(m_lPlayHandle >= 0)
	{
	if(!m_bAutoOn)
	{
       if(iSpeed >= 1)
	   {
		   NET_DVR_PTZControlWithSpeed(m_lPlayHandle,PAN_AUTO,0,iSpeed);
	   }
	   else
	   {
		    NET_DVR_PTZControl(m_lPlayHandle,PAN_AUTO,0);
	   }
        GetDlgItem(IDC_BTN_PTZ_AUTO)->SetWindowText("停止");
		m_bAutoOn = TRUE;
	}
	else
	{
		 if(iSpeed >= 1)
		 {
			 NET_DVR_PTZControlWithSpeed(m_lPlayHandle,PAN_AUTO,1,iSpeed);
		 }
		 else
		 {
             NET_DVR_PTZControl(m_lPlayHandle,PAN_AUTO,1);
		 }
		 GetDlgItem(IDC_BTN_PTZ_AUTO)->SetWindowText("自动");
		 m_bAutoOn = FALSE;
	}
	}
	
}

//开始/停止 辅助设备1
/*
void CLaserDeicingDlg::OnBtnAuxPwron() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!m_bAuxOn1)
		{
			NET_DVR_PTZControl(m_lPlayHandle,AUX_PWRON1,0);
			GetDlgItem(IDC_BTN_AUX_PWRON)->SetWindowText("停止1");
			m_bAuxOn1 = TRUE;
		}
		else
		{
			NET_DVR_PTZControl(m_lPlayHandle,AUX_PWRON1,1);
			GetDlgItem(IDC_BTN_AUX_PWRON)->SetWindowText("辅助1");
			m_bAuxOn1 = FALSE;
		}
	}
}*/

//开始/停止 辅助设备2
/*
void CLaserDeicingDlg::OnBtnAuxPwron2() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!m_bAuxOn2)
		{
			NET_DVR_PTZControl(m_lPlayHandle,AUX_PWRON2,0);
			GetDlgItem(IDC_BTN_AUX_PWRON2)->SetWindowText("停止2");
			m_bAuxOn2 = TRUE;
		}
		else
		{
			NET_DVR_PTZControl(m_lPlayHandle,AUX_PWRON2,1);
			GetDlgItem(IDC_BTN_AUX_PWRON2)->SetWindowText("辅助2");
			m_bAuxOn2 = FALSE;
		}
	}
}*/


//开始/停止 风扇
/*
void CLaserDeicingDlg::OnBtnFanPwron() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!m_bFanOn)
		{
			NET_DVR_PTZControl(m_lPlayHandle,FAN_PWRON,0);
			GetDlgItem(IDC_BTN_FAN_PWRON)->SetWindowText("停风");
			m_bFanOn = TRUE;
		}
		else
		{
			NET_DVR_PTZControl(m_lPlayHandle,FAN_PWRON,1);
			GetDlgItem(IDC_BTN_FAN_PWRON)->SetWindowText("风扇");
			m_bFanOn = FALSE;
		}
	}
}*/

//开始/停止加热
/*
void CLaserDeicingDlg::OnBtnHeaterPwron() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!m_bHeaterOn)
		{
			NET_DVR_PTZControl(m_lPlayHandle,HEATER_PWRON,0);
			GetDlgItem(IDC_BTN_HEATER_PWRON)->SetWindowText("停止");
			m_bHeaterOn = TRUE;
		}
		else
		{
			NET_DVR_PTZControl(m_lPlayHandle,HEATER_PWRON,1);
			GetDlgItem(IDC_BTN_HEATER_PWRON)->SetWindowText("加热");
			m_bHeaterOn = FALSE;
		}
	}
}*/

//打开/关闭灯光
/*
void CLaserDeicingDlg::OnBtnLightPwron() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!m_bLightOn)
		{
			NET_DVR_PTZControl(m_lPlayHandle,LIGHT_PWRON,0);
			GetDlgItem(IDC_BTN_LIGHT_PWRON)->SetWindowText("关灯");
			m_bLightOn = TRUE;
		}
		else
		{
			NET_DVR_PTZControl(m_lPlayHandle,LIGHT_PWRON,1);
			GetDlgItem(IDC_BTN_LIGHT_PWRON)->SetWindowText("灯光");
			m_bLightOn = FALSE;
		}
	}
}*/

//开始/停止雨刷
/*
void CLaserDeicingDlg::OnBtnWiperPwron() 
{
	if(m_lPlayHandle >= 0)
	{	
		if(!m_bWiperOn)
		{
		    NET_DVR_PTZControl(m_lPlayHandle,WIPER_PWRON,0);
		    GetDlgItem(IDC_BTN_WIPER_PWRON)->SetWindowText("雨停");
		    m_bWiperOn = TRUE;
		}
	    else
		{
            NET_DVR_PTZControl(m_lPlayHandle,WIPER_PWRON,1);
	    	GetDlgItem(IDC_BTN_WIPER_PWRON)->SetWindowText("雨刷");
	    	m_bWiperOn = FALSE;
		}

	}

}*/


void CLaserDeicingDlg::OnSelchangeComboPicType() 
{
	int iSel = m_coPicType.GetCurSel();
	if(0 == iSel)  //bmp
	{   
		GetDlgItem(IDC_STATIC_JPGPARA)->EnableWindow(FALSE);
        m_coJpgSize.EnableWindow(FALSE);
		m_coJpgQuality.EnableWindow(FALSE);
	}
	else if(1 == iSel)          //jpg
	{
		GetDlgItem(IDC_STATIC_JPGPARA)->EnableWindow(TRUE);
        m_coJpgSize.EnableWindow(TRUE);
		m_coJpgQuality.EnableWindow(TRUE);
	}
	
}

void CLaserDeicingDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bIsLogin)
	{
        StopPlay();		
        NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID); 
	}
	CDialog::OnClose();
}

//测试
void CLaserDeicingDlg::OnBnClickedButton1()
{
	//ImageSegmentationDlg dlg;
	//dlg.DoModal();
	m_info.AddString("1111");
}

//图像预处理
void CLaserDeicingDlg::OnBnClickedImagePreprocessingButton()
{
	Mat temp_image;
	ImagePreprocessingDlg dlg;//图像预处理窗口
	dlg.src = src.clone();
	dlg.DoModal();
	temp_image = dst.clone();
	//putText(temp_image, "Image Preprocessing", Point(50,50),FONT_HERSHEY_COMPLEX,1,Scalar(255),2);//显示文字
	DrawPicToHDC(temp_image, IDC_IMSHOW_3);
}

//将消息显示到窗口上
void CLaserDeicingDlg::DrawPicToHDC( cv::Mat& mat,UINT ID )
{
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	IplImage img = mat;
	CvvImage cimg;//Windows环境中加载显示OpenCV中的图像
	cimg.CopyOf(&img);//从img复制图像到当前的对象中
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect); //绘制图像的ROI区域到DC的pDstRect。 设备描述符、设备描述区域
	ReleaseDC(pDC);
}

//图像分割
void CLaserDeicingDlg::OnBnClickedImageSegmentationButton()
{
	Mat temp_image;
	ImageSegmentationDlg dlg;
	dlg.src = dst;
	dlg.DoModal();
	temp_image = dst.clone();
	//putText(temp_image, "Image Segmentation", Point(50,50),FONT_HERSHEY_COMPLEX,1,Scalar(255),2);
	DrawPicToHDC(temp_image, IDC_IMSHOW_3);
	if (!show_img.empty())
	{
		DrawPicToHDC(show_img, IDC_IMSHOW_4);
	}
}

//边缘提取
void CLaserDeicingDlg::OnBnClickedImageEdgeextractionButton()
{
	Mat temp_image;
	ImageEdgeExtractionDlg dlg;
	dlg.src = dst;
	dlg.DoModal();
	temp_image = dst.clone();
	//putText(temp_image, "Image EdegeExtraction", Point(50,50),FONT_HERSHEY_COMPLEX,1,Scalar(255),2);
	DrawPicToHDC(temp_image, IDC_IMSHOW_3);
}

//打开图像
void CLaserDeicingDlg::OnBnClickedOpenfileButton1()
{
	CFileDialog OpenImage(
		TRUE, 
		_T("*.jpg"), 
		NULL, 
		0, 
		_T("图片文件(*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp|所有文件(*.*)|*.*||"), 
		NULL);
	OpenImage.m_ofn.lpstrTitle = _T("LaserDeicing v2.0");	
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
		DrawPicToHDC(src, IDC_IMSHOW_2);
		imwrite("./图像/源图像/src.jpg", src.clone());//将输入图像拷贝保存到源图像中
		dst = src;
		filepath.ReleaseBuffer();
		return;
	}
	else
	{
		return;
	}
}

//刷新
void CLaserDeicingDlg::OnBnClickedUpdateButton()
{
	if (src.empty())
	{
		return;
	}
	Mat temp(3, 3, CV_8UC3, Scalar(194, 194, 194));//设置一副背景图
	DrawPicToHDC(temp, IDC_IMSHOW_2);
	DrawPicToHDC(temp, IDC_IMSHOW_3);
	DrawPicToHDC(temp, IDC_IMSHOW_4);
	src.release();//打开图像的src图像
	dst.release();//打开图像后处理的dst图像
	show_img.release();//用于向主窗口显示标记了目标的图像
	gray.release();
	imageROI.release();//选取的ROI图像
	COMPENSATION_image.release();//光照补偿图像
	equalizeImage.release();//直方图均衡化图像
	Image_Threshold.release();//手动阈值图像
	Image_OstuThreshold.release(); //大律法阈值图像
	Image_adaptiveThreshold.release(); //自适应阈值图像
	blurImage.release();//中值滤波
	GaussianBlurImage.release(); //高斯滤波
	MedianBlurImage.release(); //中值滤波
	BilateralFilterImage.release();//双边滤波
	CannyImage.release();//Canny边缘检测
	SobelImage.release(); //Sobel边缘检测
	LaplaceImage.release(); //Laplace边缘检测
	ErodeImage.release(); //腐蚀图像
	DilateImage.release();//膨胀图像
}

//手动选点
void CLaserDeicingDlg::OnBnClickedClinkOnButton()
{
	if (src.empty())
	{
		MessageBox(_T("没有可用源图像"));
		return;
	}
	namedWindow("手动选点", 0);
	resizeWindow("手动选点", src.cols/2, src.rows/2);
	imshow("手动选点", src);
	cvSetMouseCallback("手动选点", on_mouse, (void*)&src);
	imshow("手动选点", src);
	UpdateData(FALSE);
}

//通过opencv选点的on_mouse回调函数
void  on_mouse(int event, int x, int y, int flags, void* prarm){
	const int g_dReferWidth=60;//mm 像素当量
	Mat& pic1 = *(Mat*)prarm;
	Mat pic2 = pic1.clone();
	int cha = pic1.channels();//获取图像的通道数
	char label[50];
	char label2[20];
	CLaserDeicingDlg *dlg=(CLaserDeicingDlg*)AfxGetMainWnd();
	CString str = "";
	str.Format(_T("%d,%d"),x, y);
	dlg->m_strPoint = str;
	dlg->UpdateData(FALSE);
	if (event == CV_EVENT_LBUTTONDOWN){
		Point p(x, y);
		int B = pic1.at<Vec3b>(p)[0];//B
		int G = pic1.at<Vec3b>(p)[1];//G
		int R = pic1.at<Vec3b>(p)[2];//R
		//uchar* ptr = (uchar*)pic1.data + y * pic1.cols; //获取指定点的像素值（要求输入图像是灰度图）
		sprintf(label, "gray value: (%d,%d,%d)", B, G, R);
		sprintf(label2, "pixel: (%d,%d)", x, y);
		CvPoint centerPoint;
		centerPoint.x = x;
		centerPoint.y = y;
		if (dlg->flag)
		{
			if (pic1.cols - x <= 180 || pic1.rows - y <= 20)
			{
				cv::putText(pic1, label, cvPoint(x - 300, y - 30), CV_FONT_HERSHEY_SIMPLEX, 0.7,Scalar(0, 0, 255),2,8);
				cv::putText(pic1, label2, cvPoint(x - 300, y - 10), CV_FONT_HERSHEY_SIMPLEX,0.7,Scalar(0, 0, 255),2,8);
				cv::circle(pic2, centerPoint, 6, Scalar(0, 0, 255));
			}
			else
			{
				cv::putText(pic1, label, cvPoint(x + 10, y), CV_FONT_HERSHEY_SIMPLEX,0.7, Scalar(0, 0, 255),2,8);
				cv::putText(pic1, label2, cvPoint(x + 10, y + 20),CV_FONT_HERSHEY_SIMPLEX,0.7, Scalar(0, 0, 255),2,8);
				cv::circle(pic1, centerPoint, 6, Scalar(0, 0, 255));
			}
			CString str = "";
			str.Format(_T("(%d,%d)(%d,%d,%d)"),x, y, B, G, R);
			dlg->m_strPoint1 = str;
			dlg->UpdateData(FALSE);
			dlg->p1.x = (float)x;
			dlg->p1.y = (float)y;
			cv::imshow("手动选点", pic1);   //绘制以鼠标坐标为圆心的圆
			dlg->flag = FALSE;
		}
		else
		{
			if (pic1.cols - x <= 180 || pic1.rows - y <= 20)
			{
				cv::putText(pic1, label, cvPoint(x - 300, y - 30), CV_FONT_HERSHEY_SIMPLEX, 0.7,Scalar(0, 0, 255),2,8);
				cv::putText(pic1, label2, cvPoint(x - 300, y - 10), CV_FONT_HERSHEY_SIMPLEX,0.7,Scalar(0, 0, 255),2,8);
				cv::circle(pic1, centerPoint, 6, Scalar(0, 0, 255));
			}
			else
			{
				cv::putText(pic1, label, cvPoint(x + 10, y), CV_FONT_HERSHEY_SIMPLEX,0.7, Scalar(0, 0, 255),2,8);
				cv::putText(pic1, label2, cvPoint(x + 10, y + 20),CV_FONT_HERSHEY_SIMPLEX,0.7, Scalar(0, 0, 255),2,8);
				cv::circle(pic1, centerPoint, 6, Scalar(0, 0, 255));
			}
			CString str = "";
			str.Format(_T("(%d,%d)(%d,%d,%d)"),x, y, B, G, R);
			dlg->m_strPoint2 = str;
			dlg->UpdateData(FALSE);
			dlg->p2.x = (float)x;
			dlg->p2.y = (float)y;
			float distance = getDistance(dlg->p1,dlg->p2);
			CString strDistance = "";
			strDistance.Format(_T("%f"), distance);
			dlg->m_strDistance = strDistance;
			dlg->UpdateData(FALSE);
			line(pic1, dlg->p1, dlg->p2, cv::Scalar(0, 0, 255));//画线
			putText(pic1, cv::format(_T("Distance:%fmm"), distance/7.633333), Point(50,50),FONT_HERSHEY_COMPLEX,1,Scalar(0,0,0),2);//图像中标识距离
			cv::imshow("手动选点", pic1);   //绘制以鼠标坐标为圆心的圆
			dlg->flag = TRUE;
		}
	}
}

//求两点之间距离的函数实现
float getDistance(Point2f pointA, Point2f pointB)
{
	float distance;
	distance = powf((pointA.x - pointB.x), 2) + powf((pointA.y - pointB.y), 2);
	distance = sqrtf(distance);
	return distance;
}

//清空信息状态栏
void CLaserDeicingDlg::OnBnClickedButtonInfoClc()
{
	m_info.ResetContent();
	m_info.AddString("运行状态信息：");
}


