//云台控制类
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
函数名:    	OnLButtonDown
函数描述:	鼠标左键按下，开始云台动作
输入参数:   
输出参数:   			
返回值:		
*************************************************/
void CPTZButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	LONG lPlayHandle = g_pMainDlg->GetPlayHandle();
	int iPTZSpeed = g_pMainDlg->GetPTZSpeed();//获取云台的速度
	switch(m_iSubBtnIndex)
	{
	case 0:        //云台上
		PTZControlAll(lPlayHandle,TILT_UP,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始云台上动作]");
		break;
	case 1:			//云台下
		PTZControlAll(lPlayHandle,TILT_DOWN,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始云台下动作]");
		break;
	case 2:			//云台左
		PTZControlAll(lPlayHandle,PAN_LEFT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始云台左动作]");
		break;
	case 3:			//云台右
		PTZControlAll(lPlayHandle,PAN_RIGHT,0,iPTZSpeed);
		g_pMainDlg->m_info.AddString("相机[info:开始云台右动作]");
		break;
	case 4:         //调焦左
		PTZControlAll(lPlayHandle,ZOOM_IN,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始调焦左动作]");
		break;
	case 5:			//调焦右
		PTZControlAll(lPlayHandle,ZOOM_OUT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始调焦右动作]");
		break;
	case 6:			//聚焦左
		PTZControlAll(lPlayHandle,FOCUS_NEAR,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始聚焦左动作]");
		break;
	case 7:			//聚焦右
		PTZControlAll(lPlayHandle,FOCUS_FAR,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始聚焦右动作]");
		break;
	case 8:			//光圈左
		PTZControlAll(lPlayHandle,IRIS_OPEN,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始光圈左动作]");
		break;
	case 9:			//光圈右
		PTZControlAll(lPlayHandle,IRIS_CLOSE,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始云光圈右动作]");
		break;
	case 10:        //左上
		PTZControlAll(lPlayHandle,UP_LEFT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始云台左上动作]");
		break;
	case 11:        //右上
		PTZControlAll(lPlayHandle,UP_RIGHT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始云台右上动作]");
		break;
	case 12:        //左下
		PTZControlAll(lPlayHandle,DOWN_LEFT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始云台左下动作]");
		break;
	case 13:        //右下
		PTZControlAll(lPlayHandle,DOWN_RIGHT,0,iPTZSpeed);
		//g_pMainDlg->m_info.AddString("相机[info:开始云台右下动作]");
		break;

	default:
		break;
	}
	
	CButton::OnLButtonDown(nFlags, point);
}

//十六进制转换成十进制
WORD HexToDec(WORD wHex)
{
	return (wHex / 4096) * 1000 + ((wHex % 4096) / 256) * 100 + ((wHex % 256) / 16) * 10 + (wHex % 16);
}


/*************************************************
函数名:    	OnLButtonUp
函数描述:	鼠标左键弹起，停止云台动作
输入参数:   
输出参数:   			
返回值:		
*************************************************/
void CPTZButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//获取云台位置信息的结构体
	NET_DVR_PTZPOS m_ptzPos = { 0 };
	DWORD dwReturned;

//	int iChanIndex = pMainDlg->GetCurChanIndex();
	LONG lPlayHandle = g_pMainDlg->GetPlayHandle();
	int iPTZSpeed = g_pMainDlg->GetPTZSpeed();
	switch(m_iSubBtnIndex)
	{
	case 0:        //云台上
		{
			PTZControlAll(lPlayHandle,TILT_UP,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:云台上动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 1:			//云台下
		{
			PTZControlAll(lPlayHandle,TILT_DOWN,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:云台下动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 2:			//云台左
		{
			PTZControlAll(lPlayHandle,PAN_LEFT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:云台左动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 3:			//云台右
		{
			PTZControlAll(lPlayHandle,PAN_RIGHT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:云台右动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 4:         //调焦左
		{
			PTZControlAll(lPlayHandle,ZOOM_IN,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:调焦左动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 5:			//调焦右
		{
			PTZControlAll(lPlayHandle,ZOOM_OUT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:调焦右动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 6:			//聚焦左
		{
			PTZControlAll(lPlayHandle,FOCUS_NEAR,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:聚焦左动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 7:			//聚焦右
		{
			PTZControlAll(lPlayHandle,FOCUS_FAR,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:聚焦右动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 8:			//光圈左
		{
			PTZControlAll(lPlayHandle,IRIS_OPEN,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:光圈左动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 9:			//光圈右
		{
			PTZControlAll(lPlayHandle,IRIS_CLOSE,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:光圈右动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 10:        //左上
		{
			PTZControlAll(lPlayHandle,UP_LEFT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:云台左上动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 11:        //右上
		{
			PTZControlAll(lPlayHandle,UP_RIGHT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:云台右上动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 12:        //左下
		{
			PTZControlAll(lPlayHandle,DOWN_LEFT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:云台左下动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
	case 13:        //右下
		{
			PTZControlAll(lPlayHandle,DOWN_RIGHT,1,iPTZSpeed);
			g_pMainDlg->m_info.AddString("相机[info:云台右下动作]");
			BOOL bRet = NET_DVR_GetDVRConfig(g_pMainDlg->m_struDeviceInfo.lLoginID, NET_DVR_GET_PTZPOS, 0, &m_ptzPos, sizeof(NET_DVR_PTZPOS), &dwReturned);
			int m_iPara1 = HexToDec(m_ptzPos.wPanPos);//十六进制转换成十进制
			int m_iPara2 = HexToDec(m_ptzPos.wTiltPos);
			int m_iPara3 = HexToDec(m_ptzPos.wZoomPos);
			CString info = "";
			info.Format("相机[info:水平参数:%0.1f°;垂直参数:%0.1f°;变倍参数:%0.1f倍]",(float)m_iPara1*0.1,(float)m_iPara2*0.1,(float)m_iPara3*0.1);
			g_pMainDlg->m_info.AddString(info);
			break;
		}
//	case 14:        //自动
//		PTZControlAll(lPlayHandle,PAN_AUTO,1,iPTZSpeed);
//		break;
	default:
		break;
	}
	CButton::OnLButtonUp(nFlags, point);
}

/*************************************************
函数名:    	PTZControlAll
函数描述:	云台控制函数
输入参数:   
输出参数:   			
返回值:		
*************************************************/
void CPTZButton::PTZControlAll(LONG lRealHandle, DWORD dwPTZCommand, DWORD dwStop, int Speed)
{
	if(lRealHandle>=0)
	{
		BOOL ret;
		if(Speed>=1)
		{
			//带速度操作云台（需先启动图像预览）
			ret = NET_DVR_PTZControlWithSpeed(lRealHandle,dwPTZCommand,dwStop,Speed);
			if(!ret)
			{
				MessageBox("云台控制失败!");
				return;
			}
		}
		else
		{
			ret = NET_DVR_PTZControl(lRealHandle,dwPTZCommand,dwStop);
			if(!ret)
			{
				MessageBox("云台控制失败!");
				return;
			}
		}
	}
}
