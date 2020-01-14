//��̨������
#include "stdafx.h"
#include "LaserDeicing.h"
#include "PTZButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int g_iPtzBtnIndex  = 0;
/////////////////////////////////////////////////////////////////////////////
// CPTZButton

CPTZButton::CPTZButton()
{
	m_iSubBtnIndex = g_iPtzBtnIndex++;
	m_lPlayHandle  = -1;
}

CPTZButton::~CPTZButton()
{

}


BEGIN_MESSAGE_MAP(CPTZButton, CButton)
	//{{AFX_MSG_MAP(CPTZButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTZButton message handlers

/*************************************************
������:    	OnLButtonDown
��������:	���������£���ʼ��̨����
�������:   
�������:   			
����ֵ:		
*************************************************/
void CPTZButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	LONG lPlayHandle = g_pMainDlg->GetPlayHandle();
	int iPTZSpeed = g_pMainDlg->GetPTZSpeed();//��ȡ��̨���ٶ�
	switch(m_iSubBtnIndex)
	{
	case 0:        //��̨��
		PTZControlAll(lPlayHandle,TILT_UP,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ��̨�϶���]");
		break;
	case 1:			//��̨��
		PTZControlAll(lPlayHandle,TILT_DOWN,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ��̨�¶���]");
		break;
	case 2:			//��̨��
		PTZControlAll(lPlayHandle,PAN_LEFT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ��̨����]");
		break;
	case 3:			//��̨��
		PTZControlAll(lPlayHandle,PAN_RIGHT,0,iPTZSpeed);
		g_pMainDlg->m_info.AddString("���[info:��ʼ��̨�Ҷ���]");
		break;
	case 4:         //������
		PTZControlAll(lPlayHandle,ZOOM_IN,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ��������]");
		break;
	case 5:			//������
		PTZControlAll(lPlayHandle,ZOOM_OUT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ�����Ҷ���]");
		break;
	case 6:			//�۽���
		PTZControlAll(lPlayHandle,FOCUS_NEAR,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ�۽�����]");
		break;
	case 7:			//�۽���
		PTZControlAll(lPlayHandle,FOCUS_FAR,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ�۽��Ҷ���]");
		break;
	case 8:			//��Ȧ��
		PTZControlAll(lPlayHandle,IRIS_OPEN,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ��Ȧ����]");
		break;
	case 9:			//��Ȧ��
		PTZControlAll(lPlayHandle,IRIS_CLOSE,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ�ƹ�Ȧ�Ҷ���]");
		break;
	case 10:        //����
		PTZControlAll(lPlayHandle,UP_LEFT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ��̨���϶���]");
		break;
	case 11:        //����
		PTZControlAll(lPlayHandle,UP_RIGHT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ��̨���϶���]");
		break;
	case 12:        //����
		PTZControlAll(lPlayHandle,DOWN_LEFT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ��̨���¶���]");
		break;
	case 13:        //����
		PTZControlAll(lPlayHandle,DOWN_RIGHT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("���[info:��ʼ��̨���¶���]");
		break;

	default:
		break;
	}
	
	CButton::OnLButtonDown(nFlags, point);
}

//ʮ������ת����ʮ����
WORD HexToDec(WORD wHex)
{
	return (wHex / 4096) * 1000 + ((wHex % 4096) / 256) * 100 + ((wHex % 256) / 16) * 10 + (wHex % 16);
}


/*************************************************
������:    	OnLButtonUp
��������:	����������ֹͣ��̨����
�������:   
�������:   			
����ֵ:		
*************************************************/
void CPTZButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//��ȡ��̨λ����Ϣ�Ľṹ��
	NET_DVR_PTZPOS m_ptzPos = { 0 };
	DWORD dwReturned;

//	int iChanIndex = pMainDlg->GetCurChanIndex();
	LONG lPlayHandle = g_pMainDlg->GetPlayHandle();
	int iPTZSpeed = g_pMainDlg->GetPTZSpeed();
	switch(m_iSubBtnIndex)
	{
	case 0:        //��̨��
		{
			PTZControlAll(lPlayHandle,TILT_UP,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��̨�϶���]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 1:			//��̨��
		{
			PTZControlAll(lPlayHandle,TILT_DOWN,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��̨�¶���]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 2:			//��̨��
		{
			PTZControlAll(lPlayHandle,PAN_LEFT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��̨����]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 3:			//��̨��
		{
			PTZControlAll(lPlayHandle,PAN_RIGHT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��̨�Ҷ���]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 4:         //������
		{
			PTZControlAll(lPlayHandle,ZOOM_IN,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��������]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 5:			//������
		{
			PTZControlAll(lPlayHandle,ZOOM_OUT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:�����Ҷ���]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 6:			//�۽���
		{
			PTZControlAll(lPlayHandle,FOCUS_NEAR,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:�۽�����]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 7:			//�۽���
		{
			PTZControlAll(lPlayHandle,FOCUS_FAR,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:�۽��Ҷ���]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 8:			//��Ȧ��
		{
			PTZControlAll(lPlayHandle,IRIS_OPEN,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��Ȧ����]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 9:			//��Ȧ��
		{
			PTZControlAll(lPlayHandle,IRIS_CLOSE,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��Ȧ�Ҷ���]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 10:        //����
		{
			PTZControlAll(lPlayHandle,UP_LEFT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��̨���϶���]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 11:        //����
		{
			PTZControlAll(lPlayHandle,UP_RIGHT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��̨���϶���]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 12:        //����
		{
			PTZControlAll(lPlayHandle,DOWN_LEFT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��̨���¶���]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 13:        //����
		{
			PTZControlAll(lPlayHandle,DOWN_RIGHT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("���[info:��̨���¶���]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//ʮ������ת����ʮ����
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("���[info:ˮƽ����:%0.1f��;��ֱ����:%0.1f��;�䱶����:%0.1f��]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
//	case 14:        //�Զ�
//		PTZControlAll(lPlayHandle,PAN_AUTO,1,iPTZSpeed);
//		break;
	default:
		break;
	}
	CButton::OnLButtonUp(nFlags, point);
}

/*************************************************
������:    	PTZControlAll
��������:	��̨���ƺ���
�������:   
�������:   			
����ֵ:		
*************************************************/
void CPTZButton::PTZControlAll(LONG lRealHandle, DWORD dwPTZCommand, DWORD dwStop, int Speed)
{
	if(lRealHandle>=0)
	{
		BOOL ret;
		if(Speed>=1)
		{
			//���ٶȲ�����̨����������ͼ��Ԥ����
			ret = NET_DVR_PTZControlWithSpeed(lRealHandle,dwPTZCommand,dwStop,Speed);
			if(!ret)
			{
				MessageBox("��̨����ʧ��!");
				return;
			}
		}
		else
		{
			ret = NET_DVR_PTZControl(lRealHandle,dwPTZCommand,dwStop);
			if(!ret)
			{
				MessageBox("��̨����ʧ��!");
				return;
			}
		}
	}
}
