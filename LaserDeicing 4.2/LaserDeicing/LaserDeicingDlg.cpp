#include "stdafx.h"
#include "LaserDeicing.h"
#include "LaserDeicingDlg.h"
#include "DlgPTZCruise.h"
#include <math.h>

void  on_mouse(int event, int x, int y, int flags, void* prarm);//����on_mouse�ص�����
static float getDistance(Point2f pointA, Point2f pointB);//���������ĺ���

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	//�Ի�������
	enum { IDD = IDD_ABOUTBOX };
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// ʵ��
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

// CLaserDeicingDlg �Ի���

CLaserDeicingDlg::CLaserDeicingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLaserDeicingDlg::IDD, pParent)
	/*�������ڳ�ʼ��*/
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
	//DDX_Control(pDX, IDC_COMBO_SEQ, m_comboSeq);//Ѳ���켣����ѡ��
	DDX_Control(pDX, IDC_COMBO_PRESET, m_comboPreset);
	DDX_Control(pDX, IDC_COMBO_PTZ_SPEED, m_comboPTZSpeed);
	DDX_Control(pDX, IDC_TREE_CHAN, m_ctrlTreeChan);
	DDX_Control(pDX, IDC_IPADDRESS_DEV, m_ctrlDevIp);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nDevPort);
	DDX_Text(pDX, IDC_EDIT_USER, m_csUser);
	DDX_Text(pDX, IDC_EDIT_PWD, m_csPWD);

	//��������ѡ��
	DDX_Text(pDX, IDC_EDIT_P1, m_strPoint1);
	DDX_Text(pDX, IDC_EDIT_P2, m_strPoint2);
	DDX_Text(pDX, IDC_EDIT_DISTANCE, m_strDistance);
	DDX_Text(pDX, IDC_EDIT_POINT, m_strPoint);

	//��Ϣ״̬��ʾ��
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
	//ON_BN_CLICKED(IDC_BUTTON_SEQ_GOTO, OnButtonSeqGoto)//����Ѳ��
	//ON_BN_CLICKED(IDC_BUTTON_SEQ_SET, OnButtonSeqSet)//����Ѳ���켣������DlgPTZCruise����
	//ON_BN_CLICKED(IDC_BUTTON_TRACK_RUN, OnButtonTrackRun)//���й켣��¼
	//ON_BN_CLICKED(IDC_BUTTON_TRACK_START, OnButtonTrackStart)//��ʼ�켣��¼
	//ON_BN_CLICKED(IDC_BUTTON_TRACK_STOP, OnButtonTrackStop)//ֹͣ�켣��¼
	ON_BN_CLICKED(IDC_BTN_PTZ_AUTO, OnBtnPtzAuto)
	//ON_BN_CLICKED(IDC_BTN_AUX_PWRON, OnBtnAuxPwron)//�����豸1
	//ON_BN_CLICKED(IDC_BTN_FAN_PWRON, OnBtnFanPwron)//����
	//ON_BN_CLICKED(IDC_BTN_HEATER_PWRON, OnBtnHeaterPwron)//����
	//ON_BN_CLICKED(IDC_BTN_LIGHT_PWRON, OnBtnLightPwron)//�ƹ����
	//ON_BN_CLICKED(IDC_BTN_WIPER_PWRON, OnBtnWiperPwron)//��ˢ
	//ON_BN_CLICKED(IDC_BTN_AUX_PWRON2, OnBtnAuxPwron2)//�����豸2
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

// CLaserDlg ��Ϣ�������

BOOL CLaserDeicingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�

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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//Ĭ��IP
	m_ctrlDevIp.SetAddress(192,168,1,64);

	//��̨�ٶ�
	m_comboPTZSpeed.SetCurSel(5);

    //��̨��ť
	//SubclassDlgItem���໯һ���ؼ�
	//�ѶԻ��������еĿؼ���ĳ�����ڶ���̬�����������Ĵ��ڶ���ӹܿؼ�����Ϣ�����Ӷ�ʹ�ؼ������µ�����
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

	//ץͼcombo
	m_coPicType.SetCurSel(1);
	m_coJpgSize.SetCurSel(0);
	m_coJpgQuality.SetCurSel(0);
	//GetDlgItem(IDC_STATIC_JPGPARA)->EnableWindow(TRUE);
	//m_coJpgSize.EnableWindow(TRUE);
	//m_coJpgQuality.EnableWindow(TRUE);

	//����״̬��Ϣ��ʾ����ʼ��
	m_info.AddString("����״̬��Ϣ��");
	m_info.SetTopIndex(1);

	return TRUE;   // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLaserDeicingDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLaserDeicingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//������������ο�������
//������̨λ����Ϣ
void CLaserDeicingDlg::OnBnClickedSetptzposButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetPtzPosDlg dlg;
	dlg.DoModal();
}

//ע��/ע�� ��ť
void CLaserDeicingDlg::OnButtonLogin() 
{
	
	if(!m_bIsLogin)    //ע��
	{
		if(!DoLogin())
			return;
		DoGetDeviceResoureCfg();//��ȡ�豸��Դ��Ϣ	
		CreateDeviceTree();//����ͨ����
		GetDecoderCfg();//��ȡ��̨��������Ϣ
		InitDecoderReferCtrl();//��ʼ����������ؿؼ�      
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText("ע��");
		m_bIsLogin = TRUE;
	}
	else      //ע��
	{
		if(m_bIsPlaying|| m_bIsRecording)
		{
			MessageBox("Stop Play or record first!");
			return;
		}
        NET_DVR_Logout_V30(m_struDeviceInfo.lLoginID); //�û�ע��
		GetDlgItem(IDC_BUTTON_LOGIN)->SetWindowText("��¼");
		m_ctrlTreeChan.DeleteAllItems();//ɾ���豸����������Ϣ
		m_struDeviceInfo.lLoginID = -1;
		m_bIsLogin = FALSE;
	}
}

//DoLogin�����豸ע��
BOOL CLaserDeicingDlg::DoLogin()
{
	UpdateData(TRUE);
	CString DeviceIp;
	BYTE nField0,nField1,nField2,nField3; 
	m_ctrlDevIp.GetAddress(nField0,nField1,nField2,nField3);
	DeviceIp.Format("%d.%d.%d.%d",nField0,nField1,nField2,nField3);//��ȡIP��ַ
	
	NET_DVR_DEVICEINFO_V30 DeviceInfoTmp;//�豸�����Ľṹ��
	memset(&DeviceInfoTmp,0,sizeof(NET_DVR_DEVICEINFO_V30));//�ṹ���ڴ�ռ��ʼ��
	
	//���豸ע�ᣬip��ַ���豸�Ķ˿ںţ��û��������룬�豸��Ϣ
	//return -1��ʾʧ�ܣ�0��ʾû�д���1��ʾ�û��������������3��ʾSDKδ��ʼ��
	LONG lLoginID = NET_DVR_Login_V30(DeviceIp.GetBuffer(DeviceIp.GetLength()),m_nDevPort, \
		m_csUser.GetBuffer(m_csUser.GetLength()),m_csPWD.GetBuffer(m_csPWD.GetLength()),&DeviceInfoTmp);
	if(lLoginID == -1)
	{
		m_info.AddString("���[info:ע��ʧ��!]\n");
		return FALSE;
	}
	if(lLoginID == 1)
	{
		m_info.AddString("���[info:�û������������!]\n");
		return FALSE;
	}
	if(lLoginID == 7)
	{
		m_info.AddString("���[info:�����豸ʧ�ܣ�������IP��ַ!]\n");
		return FALSE;
	}
    m_struDeviceInfo.lLoginID = lLoginID;//ע�᷵�ص�ֵ
	m_struDeviceInfo.iDeviceChanNum = DeviceInfoTmp.byChanNum;//�豸��ͨ����
    m_struDeviceInfo.iIPChanNum = DeviceInfoTmp.byIPChanNum;//�������ͨ������
    m_struDeviceInfo.iStartChan  = DeviceInfoTmp.byStartChan;//�豸��ʼͨ����
    m_struDeviceInfo.iIPStartChan  = DeviceInfoTmp.byStartDChan;//����ͨ����ʼͨ����
	return TRUE;
}

//DoGetDeviceResoureCfg  ��ȡ�豸��ͨ����Դ
void CLaserDeicingDlg::DoGetDeviceResoureCfg()
{
	NET_DVR_IPPARACFG_V40 IpAccessCfg;//IP�豸��Դ��IPͨ����Դ�Ľṹ��
	memset(&IpAccessCfg,0,sizeof(IpAccessCfg));	//�����ڴ�ռ�
	DWORD  dwReturned;//unsigned long

	//�Ƿ�֧��IP����
	//NET_DVR_GetDVRConfig�Ƿ�֧��IP����
	m_struDeviceInfo.bIPRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID,NET_DVR_GET_IPPARACFG_V40,0,&IpAccessCfg,sizeof(NET_DVR_IPPARACFG_V40),&dwReturned);
	int i;
    if(!m_struDeviceInfo.bIPRet)   //��֧��ip����,9000�����豸��֧�ֽ���ģ��ͨ��
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)
		{
			if (i < m_struDeviceInfo.iDeviceChanNum)
			{
				sprintf(m_struDeviceInfo.struChanInfo[i].chChanName,"camera%d",i+m_struDeviceInfo.iStartChan);
				m_struDeviceInfo.struChanInfo[i].iChanIndex=i+m_struDeviceInfo.iStartChan;  //ͨ����
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
	else        //֧��IP���룬9000�豸
	{
		for(i=0; i<MAX_ANALOG_CHANNUM; i++)  //ģ��ͨ��
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

		//����ͨ��
		for(i=0; i<MAX_IP_CHANNEL; i++)
		{
			if(IpAccessCfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)  //ipͨ������
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

//����ͨ����
void CLaserDeicingDlg::CreateDeviceTree()
{
	m_hDevItem = m_ctrlTreeChan.InsertItem("Dev");
	m_ctrlTreeChan.SetItemData(m_hDevItem,DEVICETYPE*1000);//��ֵ
    for(int i=0; i<MAX_CHANNUM_V30; i++)
	{
		if(m_struDeviceInfo.struChanInfo[i].bEnable)  //ͨ����Ч������ͨ����
		{
             HTREEITEM ChanItem = m_ctrlTreeChan.InsertItem(m_struDeviceInfo.struChanInfo[i].chChanName,m_hDevItem);
			 m_ctrlTreeChan.SetItemData(ChanItem,CHANNELTYPE*1000+i);   //Data��Ӧͨ���������е�����
		}
	}
	m_ctrlTreeChan.Expand(m_hDevItem,TVE_EXPAND);
}


//˫��ͨ����������ѡ��ͨ��
void CLaserDeicingDlg::OnDblclkTreeChan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM hSelected = m_ctrlTreeChan.GetSelectedItem();
	//δѡ��
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
			TRACE("select chan: %d\n",iIndex);//debug����ʱ��������Ϣ
			DbPlayChannel(iIndex);
			OnSelchangeComboPreset();
			break;
		default:
			break;
	}
	*pResult = 0;
}

//˫������
void CLaserDeicingDlg::DbPlayChannel(int ChanIndex/*ͨ����*/)
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

//��ʼһ·����
void CLaserDeicingDlg::StartPlay(int iChanIndex)
{
	NET_DVR_CLIENTINFO ClientInfo;//Ԥ�������ṹ��
	ClientInfo.hPlayWnd     = GetDlgItem(IDC_STATIC_PLAY)->m_hWnd;//���Ŵ��ڵľ����ΪNULL��ʾ����ʾͼ��
	ClientInfo.lChannel     = m_iCurChanIndex+1;//ͨ����
	ClientInfo.lLinkMode    = 0;//����ģʽ  TCPģʽ
    ClientInfo.sMultiCastIP = NULL;//�ಥ���ַ
	TRACE("Channel number:%d\n",ClientInfo.lChannel);//���ͨ����Ϣ
	m_lPlayHandle = NET_DVR_RealPlay_V30(m_struDeviceInfo.lLoginID,&ClientInfo,NULL,NULL,TRUE);//ʵʱԤ��
	if(-1 == m_lPlayHandle)  //ʧ��
	{
		DWORD err=NET_DVR_GetLastError();//���ش�����
		CString m_csErr;
        m_csErr.Format("���ų����������%d",err);
		MessageBox(m_csErr);
	}

	m_bIsPlaying = TRUE;
	GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText("ֹͣ����");
}

//ֹͣ����
void CLaserDeicingDlg::StopPlay()
{
	if(m_lPlayHandle != -1)  //ʵʱԤ���ķ���ֵ
	{
		if(m_bIsRecording)  //����¼����ֹͣ
		{
			StopRecord();
		}
		NET_DVR_StopRealPlay(m_lPlayHandle); //ֹͣԤ����true��ʾ�ɹ���false��ʾʧ��
		m_lPlayHandle=-1;
		m_bIsPlaying = FALSE;
		GetDlgItem(IDC_STATIC_PLAY)->Invalidate();
	}
}

//��ȡͨ���е��ź�
void CLaserDeicingDlg::OnSelchangedTreeChan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hSelected = m_ctrlTreeChan.GetSelectedItem();
	//δѡ��
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

//��ȡ��ǰѡ�е�ͨ����
int CLaserDeicingDlg::GetCurChanIndex()
{
    return m_iCurChanIndex;
}

//��ȡ���ž��
LONG CLaserDeicingDlg::GetPlayHandle()
{
    return m_lPlayHandle;
}

//��ȡ��̨�ٶ�
int CLaserDeicingDlg::GetPTZSpeed()
{
   return (m_comboPTZSpeed.GetCurSel());
}

//��ʼ/ֹͣ¼��
void CLaserDeicingDlg::OnButtonRecord() 
{
	if(m_lPlayHandle == -1) //ʵʱԤ��û�п���
	{
        MessageBox("����ѡ��һ��ͨ������");
		return;
	}
	if(!m_bIsRecording) //¼���ʶ
	{
		StartRecord();
	}
	else
	{
        StopRecord();
	}
	
}

//��ʼ¼��
void CLaserDeicingDlg::StartRecord()
{
	char RecName[256] = {0};
	
	CTime CurTime = CTime::GetCurrentTime();
	sprintf(RecName,"%04d%02d%02d%02d%02d%02d_ch%02d.mp4",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
		CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond(),m_struDeviceInfo.struChanInfo[GetCurChanIndex()].iChanIndex);

	 if(!NET_DVR_SaveRealData(m_lPlayHandle,RecName))//NET_DVR_SaveRealData�������ݲ��洢��ָ�����ļ��У�true��ʾ�ɹ���false��ʾʧ��
	 {
		 MessageBox("����¼��ʧ��");
		 return;
	 }
     m_bIsRecording = TRUE;//¼���ʶ
	 GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("ֹͣ¼��");
}

//ֹͣ¼��
void CLaserDeicingDlg::StopRecord()
{
    if(!NET_DVR_StopSaveRealData(m_lPlayHandle)) //ֹͣ���ݵĲ���
	{
		MessageBox("ֹͣ¼��ʧ��");
		return;
	}
	m_bIsRecording = FALSE;//¼���ʶ
	 GetDlgItem(IDC_BUTTON_RECORD)->SetWindowText("¼��");
}

//ץͼ
void CLaserDeicingDlg::OnButtonCapture() 
{
	if(m_lPlayHandle == -1)
	{
        MessageBox("����ѡ��һ��ͨ������");
		return;
	}
	UpdateData(TRUE);

	char PicName[256] = {0};//ͼƬ�ļ���
	
	int iPicType = m_coPicType.GetCurSel();//����ͼƬ�ĸ�ʽ
	if(0 == iPicType)  //bmp
	{
		CTime CurTime = CTime::GetCurrentTime();//��ȡ��ǰʱ��
		sprintf(PicName,"./ͼ��/Դͼ��/%04d%02d%02d%02d%02d%02d_ch%02d.bmp",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
			CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond(),m_struDeviceInfo.struChanInfo[GetCurChanIndex()].iChanIndex);//ץͼͼ����ļ���
    
		if(NET_DVR_CapturePicture(m_lPlayHandle,PicName))//Ԥ��ʱ����֡���ݲ��񲢱����ͼƬ
		{
			CString info;
			info.Format("���[info:ץͼ�ɹ�!ͼƬ����·��:%s]",PicName);
			m_info.AddString(info);
		}
	}
	else if(1 == iPicType)  //jgp
	{
		CTime CurTime = CTime::GetCurrentTime();;
		sprintf(PicName,"./ͼ��/Դͼ��/%04d%02d%02d%02d%02d%02d_ch%02d.jpg",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
			CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond(),m_struDeviceInfo.struChanInfo[GetCurChanIndex()].iChanIndex);
    
		//�齨jpg�ṹ
		NET_DVR_JPEGPARA JpgPara = {0};
        JpgPara.wPicSize = (WORD)m_coJpgSize.GetCurSel();
		JpgPara.wPicQuality = (WORD)m_coJpgQuality.GetCurSel();

		LONG iCurChan = m_struDeviceInfo.struChanInfo[GetCurChanIndex()].iChanIndex;

		if(NET_DVR_CaptureJPEGPicture(m_struDeviceInfo.lLoginID, iCurChan, &JpgPara, PicName))
		{
			CString info;
			info.Format("���[info:ץͼ�ɹ�!ͼƬ����·��:%s]",PicName);
			m_info.AddString(info);
		}
	}
	return;	
}

//����ѡ��ͨ��
void CLaserDeicingDlg::OnButtonPlay() 
{
	if(m_iCurChanIndex == -1)
	{
		MessageBox("ѡ��һ��ͨ��");
		return;
	}
	if(!m_bIsPlaying)
	{
         StartPlay(m_iCurChanIndex);
		 m_bIsPlaying = TRUE;
		 GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText("ֹͣ����");

	}
	else
	{
		StopPlay();
		m_bIsPlaying = FALSE;
		GetDlgItem(IDC_BUTTON_PLAY)->SetWindowText("����");
	}
	
}

//��ȡ��̨������Ϣ
void CLaserDeicingDlg::GetDecoderCfg()
{
     NET_DVR_DECODERCFG_V30 DecoderCfg;//ͨ������������̨�������ṹ��
	 DWORD  dwReturned;
	 BOOL bRet;//�洢��ȡ�豸��������Ϣ
		
	 //��ȡͨ����������Ϣ
	 for(int i=0; i<MAX_CHANNUM_V30; i++)
	 {
		 if(m_struDeviceInfo.struChanInfo[i].bEnable)
		 {
			 memset(&DecoderCfg,0,sizeof(NET_DVR_DECODERCFG_V30));//�����ڴ�ռ�
			 //��ȡ�豸��������Ϣ
			 bRet = NET_DVR_GetDVRConfig(m_struDeviceInfo.lLoginID,NET_DVR_GET_DECODERCFG_V30 , \
				 m_struDeviceInfo.struChanInfo[i].iChanIndex,&DecoderCfg,sizeof(NET_DVR_DECODERCFG_V30),&dwReturned);
			 if(!bRet)
			 {
				 TRACE("��ȡ�豸��Ϣʧ��,ͨ��:%d\n",m_struDeviceInfo.struChanInfo[i].iChanIndex);
				 continue;
			 }

			 memcpy(&m_struDeviceInfo.struChanInfo[i].struDecodercfg,&DecoderCfg,sizeof(NET_DVR_DECODERCFG_V30));
		 }
	 }

}

//��ʼ����̨������ؿؼ�
void CLaserDeicingDlg::InitDecoderReferCtrl()
{
    int i;
	CString tmp;
	//����Ԥ�õ�
	for(i=0; i<MAX_PRESET_V30; i++)
	{  
		tmp.Format("%d",i+1);     //i+1
		m_comboPreset.AddString(tmp);
	}
	m_comboPreset.SetCurSel(0);

	GetDlgItem(IDC_BUTTON_PRESET_GOTO)->EnableWindow(FALSE);//����Ԥ�õ�
    //GetDlgItem(IDC_BUTTON_PRESET_SET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_PRESET_DEL)->EnableWindow(FALSE);//ɾ��Ԥ�õ�

	//Ѳ���켣
	/*
    for(i=0; i<MAX_CRUISE_SEQ; i++)
	{
         tmp.Format("%d",i+1);     //i+1
		 m_comboSeq.AddString(tmp);
	}
	m_comboSeq.SetCurSel(0);*/
}

//Ԥ�õ�combobox���ı䰴ť״̬
void CLaserDeicingDlg::OnSelchangeComboPreset() 
{
	int iIndex = m_comboPreset.GetCurSel();//��ȡԤ�õ�����ѡ��    
	
	if(m_struDeviceInfo.struChanInfo[m_iCurChanIndex].struDecodercfg.bySetPreset[iIndex])//�ж��Ƿ����������Ԥ�õ���Ϣ
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

//����Ԥ�õ�
void CLaserDeicingDlg::OnButtonPresetGoto() 
{
	int iPreset = m_comboPreset.GetCurSel()+1;    //+1
	if(m_lPlayHandle >= 0)
	{
		if(!NET_DVR_PTZPreset(m_lPlayHandle,GOTO_PRESET,iPreset))//��̨Ԥ�õ��������������Ԥ����
		{
			MessageBox("����Ԥ�õ�ʧ��");
			return;
		}
	}
	else
	{
		//��̨Ԥ�õ����
		if(!NET_DVR_PTZPreset_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
			                  GOTO_PRESET,iPreset))
		{
			MessageBox("����Ԥ�õ�ʧ��");
			return;
		}

	}
}

//����Ԥ�õ�
void CLaserDeicingDlg::OnButtonPresetSet() 
{
	int iPreset = m_comboPreset.GetCurSel()+1;    //+1
	if(m_lPlayHandle >= 0)
	{
		if(!NET_DVR_PTZPreset(m_lPlayHandle,SET_PRESET,iPreset))
		{
			MessageBox("����Ԥ�õ�ʧ��");
			return;
		}
	}
	else
	{
		if(!NET_DVR_PTZPreset_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
			SET_PRESET,iPreset))
		{
			MessageBox("����Ԥ�õ�ʧ��");
			return;
		}
		
	}

	//��ӵ�Ԥ�õ���Ϣ
	m_struDeviceInfo.struChanInfo[m_iCurChanIndex].struDecodercfg.bySetPreset[iPreset-1] = TRUE;
	//���°�ť״̬
    OnSelchangeComboPreset();
	
}

//ɾ��Ԥ�õ�
void CLaserDeicingDlg::OnButtonPresetDel() 
{
	int iPreset = m_comboPreset.GetCurSel()+1;    //+1
	if(m_lPlayHandle >= 0)
	{
		if(!NET_DVR_PTZPreset(m_lPlayHandle,CLE_PRESET,iPreset))
		{
			MessageBox("ɾ��Ԥ�õ�ʧ��");
			return;
		}
	}
	else
	{
		if(!NET_DVR_PTZPreset_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
			CLE_PRESET,iPreset))
		{
			MessageBox("ɾ��Ԥ�õ�ʧ��");
			return;
		}
		
	}
	
	//��ӵ�Ԥ�õ���Ϣ
	m_struDeviceInfo.struChanInfo[m_iCurChanIndex].struDecodercfg.bySetPreset[iPreset-1] = FALSE;
	//���°�ť״̬
    OnSelchangeComboPreset();
	
}

//����/ֹͣ Ѳ��
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
		    	MessageBox("����Ѳ��ʧ��");
		    	return;
			}
		}
	    else
		{
	    	if(!NET_DVR_PTZCruise_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
		                             	RUN_SEQ,iSeq,0,0))
			{
		    	MessageBox("����Ѳ��ʧ��");
		    	return;
			}
		
		}
        m_bIsOnCruise = TRUE;
		GetDlgItem(IDC_BUTTON_SEQ_GOTO)->SetWindowText("ֹͣ");
	}
	else
	{
        if(m_lPlayHandle >= 0)
		{
			if(!NET_DVR_PTZCruise(m_lPlayHandle,STOP_SEQ,iSeq,0,0))
			{
				MessageBox("ֹͣѲ��ʧ��");
				return;
			}
		}
		else
		{
			if(!NET_DVR_PTZCruise_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex, \
				STOP_SEQ,iSeq,0,0))
			{
				MessageBox("ֹͣѲ��ʧ��");
				return;
			}
			
		}
		m_bIsOnCruise = FALSE;
		GetDlgItem(IDC_BUTTON_SEQ_GOTO)->SetWindowText("����");
	}
	
}*/

//����Ѳ��·��
/*
void CLaserDeicingDlg::OnButtonSeqSet() 
{
	CDlgPTZCruise Dlg;
	Dlg.DoModal();
	
}*/

//��ʼ���й켣
/*
void CLaserDeicingDlg::OnButtonTrackRun() 
{
	if(!m_bTrackRun)
	{
		if(m_lPlayHandle >= 0)
		{
			if(!NET_DVR_PTZTrack(m_lPlayHandle,RUN_CRUISE))
			{
				MessageBox("���й켣ʧ��");
			}
		}
		else
		{
			if(!NET_DVR_PTZTrack(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex))
			{
				MessageBox("���й켣ʧ��");
			}
		}
		m_bTrackRun = TRUE;
		GetDlgItem(IDC_BUTTON_TRACK_RUN)->SetWindowText("ֹͣ");    
	}
	else
	{
        //��㷢��һ����̨��������ֹͣ����
        if(m_lPlayHandle >= 0)
		{
			if(!NET_DVR_PTZControl(m_lPlayHandle,TILT_UP,1))
			{
				MessageBox("ֹͣ�켣ʧ��");
			}
		}
		else
		{
			if(!NET_DVR_PTZControl_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex,TILT_UP,1))
			{
				MessageBox("ֹͣ�켣ʧ��");
			}
		}
		m_bTrackRun = FALSE;
		GetDlgItem(IDC_BUTTON_TRACK_RUN)->SetWindowText("����");   
	}
}*/

//��ʼ��¼�켣
/*
void CLaserDeicingDlg::OnButtonTrackStart() 
{
	
	if(m_lPlayHandle >= 0)
	{
		if(!NET_DVR_PTZTrack(m_lPlayHandle,STA_MEM_CRUISE))
		{
			MessageBox("��ʼ��¼�켣ʧ��");
			return;
		}

	}
	else
	{
       if(!NET_DVR_PTZTrack_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex,STA_MEM_CRUISE))
	   {
		   MessageBox("��ʼ��¼�켣ʧ��");
			return;
	   }
	}
}*/

//ֹͣ��¼�켣
/*
void CLaserDeicingDlg::OnButtonTrackStop() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!NET_DVR_PTZTrack(m_lPlayHandle,STO_MEM_CRUISE))
		{
			MessageBox("ֹͣʧ��");
			return;
		}
		
	}
	else
	{
		if(!NET_DVR_PTZTrack_Other(m_struDeviceInfo.lLoginID,m_struDeviceInfo.struChanInfo[m_iCurChanIndex].iChanIndex,STO_MEM_CRUISE))
		{
			MessageBox("ֹͣʧ��");
			return;
		}
	}
}*/

//��̨��ʼ/ֹͣ�����Զ�ɨ��
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
        GetDlgItem(IDC_BTN_PTZ_AUTO)->SetWindowText("ֹͣ");
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
		 GetDlgItem(IDC_BTN_PTZ_AUTO)->SetWindowText("�Զ�");
		 m_bAutoOn = FALSE;
	}
	}
	
}

//��ʼ/ֹͣ �����豸1
/*
void CLaserDeicingDlg::OnBtnAuxPwron() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!m_bAuxOn1)
		{
			NET_DVR_PTZControl(m_lPlayHandle,AUX_PWRON1,0);
			GetDlgItem(IDC_BTN_AUX_PWRON)->SetWindowText("ֹͣ1");
			m_bAuxOn1 = TRUE;
		}
		else
		{
			NET_DVR_PTZControl(m_lPlayHandle,AUX_PWRON1,1);
			GetDlgItem(IDC_BTN_AUX_PWRON)->SetWindowText("����1");
			m_bAuxOn1 = FALSE;
		}
	}
}*/

//��ʼ/ֹͣ �����豸2
/*
void CLaserDeicingDlg::OnBtnAuxPwron2() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!m_bAuxOn2)
		{
			NET_DVR_PTZControl(m_lPlayHandle,AUX_PWRON2,0);
			GetDlgItem(IDC_BTN_AUX_PWRON2)->SetWindowText("ֹͣ2");
			m_bAuxOn2 = TRUE;
		}
		else
		{
			NET_DVR_PTZControl(m_lPlayHandle,AUX_PWRON2,1);
			GetDlgItem(IDC_BTN_AUX_PWRON2)->SetWindowText("����2");
			m_bAuxOn2 = FALSE;
		}
	}
}*/


//��ʼ/ֹͣ ����
/*
void CLaserDeicingDlg::OnBtnFanPwron() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!m_bFanOn)
		{
			NET_DVR_PTZControl(m_lPlayHandle,FAN_PWRON,0);
			GetDlgItem(IDC_BTN_FAN_PWRON)->SetWindowText("ͣ��");
			m_bFanOn = TRUE;
		}
		else
		{
			NET_DVR_PTZControl(m_lPlayHandle,FAN_PWRON,1);
			GetDlgItem(IDC_BTN_FAN_PWRON)->SetWindowText("����");
			m_bFanOn = FALSE;
		}
	}
}*/

//��ʼ/ֹͣ����
/*
void CLaserDeicingDlg::OnBtnHeaterPwron() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!m_bHeaterOn)
		{
			NET_DVR_PTZControl(m_lPlayHandle,HEATER_PWRON,0);
			GetDlgItem(IDC_BTN_HEATER_PWRON)->SetWindowText("ֹͣ");
			m_bHeaterOn = TRUE;
		}
		else
		{
			NET_DVR_PTZControl(m_lPlayHandle,HEATER_PWRON,1);
			GetDlgItem(IDC_BTN_HEATER_PWRON)->SetWindowText("����");
			m_bHeaterOn = FALSE;
		}
	}
}*/

//��/�رյƹ�
/*
void CLaserDeicingDlg::OnBtnLightPwron() 
{
	if(m_lPlayHandle >= 0)
	{
		if(!m_bLightOn)
		{
			NET_DVR_PTZControl(m_lPlayHandle,LIGHT_PWRON,0);
			GetDlgItem(IDC_BTN_LIGHT_PWRON)->SetWindowText("�ص�");
			m_bLightOn = TRUE;
		}
		else
		{
			NET_DVR_PTZControl(m_lPlayHandle,LIGHT_PWRON,1);
			GetDlgItem(IDC_BTN_LIGHT_PWRON)->SetWindowText("�ƹ�");
			m_bLightOn = FALSE;
		}
	}
}*/

//��ʼ/ֹͣ��ˢ
/*
void CLaserDeicingDlg::OnBtnWiperPwron() 
{
	if(m_lPlayHandle >= 0)
	{	
		if(!m_bWiperOn)
		{
		    NET_DVR_PTZControl(m_lPlayHandle,WIPER_PWRON,0);
		    GetDlgItem(IDC_BTN_WIPER_PWRON)->SetWindowText("��ͣ");
		    m_bWiperOn = TRUE;
		}
	    else
		{
            NET_DVR_PTZControl(m_lPlayHandle,WIPER_PWRON,1);
	    	GetDlgItem(IDC_BTN_WIPER_PWRON)->SetWindowText("��ˢ");
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

//����
void CLaserDeicingDlg::OnBnClickedButton1()
{
	//ImageSegmentationDlg dlg;
	//dlg.DoModal();
	m_info.AddString("1111");
}

//ͼ��Ԥ����
void CLaserDeicingDlg::OnBnClickedImagePreprocessingButton()
{
	Mat temp_image;
	ImagePreprocessingDlg dlg;//ͼ��Ԥ������
	dlg.src = src.clone();
	dlg.DoModal();
	temp_image = dst.clone();
	//putText(temp_image, "Image Preprocessing", Point(50,50),FONT_HERSHEY_COMPLEX,1,Scalar(255),2);//��ʾ����
	DrawPicToHDC(temp_image, IDC_IMSHOW_3);
}

//����Ϣ��ʾ��������
void CLaserDeicingDlg::DrawPicToHDC( cv::Mat& mat,UINT ID )
{
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	IplImage img = mat;
	CvvImage cimg;//Windows�����м�����ʾOpenCV�е�ͼ��
	cimg.CopyOf(&img);//��img����ͼ�񵽵�ǰ�Ķ�����
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	cimg.DrawToHDC(hDC,&rect); //����ͼ���ROI����DC��pDstRect�� �豸���������豸��������
	ReleaseDC(pDC);
}

//ͼ��ָ�
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

//��Ե��ȡ
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

//��ͼ��
void CLaserDeicingDlg::OnBnClickedOpenfileButton1()
{
	CFileDialog OpenImage(
		TRUE, 
		_T("*.jpg"), 
		NULL, 
		0, 
		_T("ͼƬ�ļ�(*.jpg;*.png;*.bmp)|*.jpg;*.png;*.bmp|�����ļ�(*.*)|*.*||"), 
		NULL);
	OpenImage.m_ofn.lpstrTitle = _T("LaserDeicing v2.0");	
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
		DrawPicToHDC(src, IDC_IMSHOW_2);
		imwrite("./ͼ��/Դͼ��/src.jpg", src.clone());//������ͼ�񿽱����浽Դͼ����
		dst = src;
		filepath.ReleaseBuffer();
		return;
	}
	else
	{
		return;
	}
}

//ˢ��
void CLaserDeicingDlg::OnBnClickedUpdateButton()
{
	if (src.empty())
	{
		return;
	}
	Mat temp(3, 3, CV_8UC3, Scalar(194, 194, 194));//����һ������ͼ
	DrawPicToHDC(temp, IDC_IMSHOW_2);
	DrawPicToHDC(temp, IDC_IMSHOW_3);
	DrawPicToHDC(temp, IDC_IMSHOW_4);
	src.release();//��ͼ���srcͼ��
	dst.release();//��ͼ������dstͼ��
	show_img.release();//��������������ʾ�����Ŀ���ͼ��
	gray.release();
	imageROI.release();//ѡȡ��ROIͼ��
	COMPENSATION_image.release();//���ղ���ͼ��
	equalizeImage.release();//ֱ��ͼ���⻯ͼ��
	Image_Threshold.release();//�ֶ���ֵͼ��
	Image_OstuThreshold.release(); //���ɷ���ֵͼ��
	Image_adaptiveThreshold.release(); //����Ӧ��ֵͼ��
	blurImage.release();//��ֵ�˲�
	GaussianBlurImage.release(); //��˹�˲�
	MedianBlurImage.release(); //��ֵ�˲�
	BilateralFilterImage.release();//˫���˲�
	CannyImage.release();//Canny��Ե���
	SobelImage.release(); //Sobel��Ե���
	LaplaceImage.release(); //Laplace��Ե���
	ErodeImage.release(); //��ʴͼ��
	DilateImage.release();//����ͼ��
}

//�ֶ�ѡ��
void CLaserDeicingDlg::OnBnClickedClinkOnButton()
{
	if (src.empty())
	{
		MessageBox(_T("û�п���Դͼ��"));
		return;
	}
	namedWindow("�ֶ�ѡ��", 0);
	resizeWindow("�ֶ�ѡ��", src.cols/2, src.rows/2);
	imshow("�ֶ�ѡ��", src);
	cvSetMouseCallback("�ֶ�ѡ��", on_mouse, (void*)&src);
	imshow("�ֶ�ѡ��", src);
	UpdateData(FALSE);
}

//ͨ��opencvѡ���on_mouse�ص�����
void  on_mouse(int event, int x, int y, int flags, void* prarm){
	const int g_dReferWidth=60;//mm ���ص���
	Mat& pic1 = *(Mat*)prarm;
	Mat pic2 = pic1.clone();
	int cha = pic1.channels();//��ȡͼ���ͨ����
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
		//uchar* ptr = (uchar*)pic1.data + y * pic1.cols; //��ȡָ���������ֵ��Ҫ������ͼ���ǻҶ�ͼ��
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
			cv::imshow("�ֶ�ѡ��", pic1);   //�������������ΪԲ�ĵ�Բ
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
			line(pic1, dlg->p1, dlg->p2, cv::Scalar(0, 0, 255));//����
			putText(pic1, cv::format(_T("Distance:%fmm"), distance/7.633333), Point(50,50),FONT_HERSHEY_COMPLEX,1,Scalar(0,0,0),2);//ͼ���б�ʶ����
			cv::imshow("�ֶ�ѡ��", pic1);   //�������������ΪԲ�ĵ�Բ
			dlg->flag = TRUE;
		}
	}
}

//������֮�����ĺ���ʵ��
float getDistance(Point2f pointA, Point2f pointB)
{
	float distance;
	distance = powf((pointA.x - pointB.x), 2) + powf((pointA.y - pointB.y), 2);
	distance = sqrtf(distance);
	return distance;
}

//�����Ϣ״̬��
void CLaserDeicingDlg::OnBnClickedButtonInfoClc()
{
	m_info.ResetContent();
	m_info.AddString("����״̬��Ϣ��");
}


