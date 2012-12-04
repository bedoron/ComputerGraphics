// AddCamera.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "AddCamera.h"
#include "afxdialogex.h"


// AddCamera dialog

IMPLEMENT_DYNAMIC(AddCamera, CDialogEx)

AddCamera::AddCamera(CWnd* pParent /*=NULL*/)
	: CDialogEx(AddCamera::IDD, pParent)
{

}

AddCamera::~AddCamera()
{
}

void AddCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_eye_x, eyeX);
	DDX_Control(pDX, IDC_eye_y, eyeY);
	DDX_Control(pDX, IDC_eye_z, eyeZ);
	DDX_Control(pDX, IDC_at_x, atX);
	DDX_Control(pDX, IDC_at_y, atY);
	DDX_Control(pDX, IDC_at_z, atZ);
	DDX_Control(pDX, IDC_up_x, upX);
	DDX_Control(pDX, IDC_up_y, upY);
	DDX_Control(pDX, IDC_up_z, upZ);
}

vec3 AddCamera::getEye()
{
	float x,y,z;
	char bufferX[5];
	char bufferY[5];
	char bufferZ[5];
	eyeX.GetWindowTextA(bufferX, 5);
	x= atof(bufferX);
	eyeY.GetWindowTextA(bufferY, 5);
	y= atof(bufferY);
	eyeZ.GetWindowTextA(bufferZ, 5);
	z= atof(bufferZ);
	return vec3(x,y,z);
}
vec3 AddCamera::getAt()
{
	float x,y,z;
	char bufferX[5];
	char bufferY[5];
	char bufferZ[5];
	atX.GetWindowTextA(bufferX, 5);
	x= atof(bufferX);
	atY.GetWindowTextA(bufferY, 5);
	y= atof(bufferY);
	atZ.GetWindowTextA(bufferZ, 5);
	z= atof(bufferZ);
	return vec3(x,y,z);
}
vec3 AddCamera::getUp()
{
	float x,y,z;
	char bufferX[5];
	char bufferY[5];
	char bufferZ[5];
	upX.GetWindowTextA(bufferX, 5);
	x= atof(bufferX);
	upY.GetWindowTextA(bufferY, 5);
	y= atof(bufferY);
	upZ.GetWindowTextA(bufferZ, 5);
	z= atof(bufferZ);
	return vec3(x,y,z);
}
BEGIN_MESSAGE_MAP(AddCamera, CDialogEx)
	ON_BN_CLICKED(IDOK, &AddCamera::OnBnClickedOk)
END_MESSAGE_MAP()


// AddCamera message handlers
void AddCamera::resetData()
{
	eyeX.SetWindowTextA(_T("0"));
	eyeY.SetWindowTextA(_T("0"));
	eyeZ.SetWindowTextA(_T("0"));
	atX.SetWindowTextA(_T("0"));
	atY.SetWindowTextA(_T("0"));
	atZ.SetWindowTextA(_T("0"));
	upX.SetWindowTextA(_T("0"));
	upY.SetWindowTextA(_T("0"));
	upZ.SetWindowTextA(_T("0"));

}

void AddCamera::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	initMenu(false);
	if(_mode)
		m_scene->addCamera(vec4(getEye()),vec4(getAt()),vec4(getUp()));
	else
	{
		m_scene->getActiveCamera()->LookAt(getEye(),getAt(),getUp());	
		m_scene->draw();
	}
	initMenu();
}
void AddCamera::refreshCameraData()
{
	Camera* m_activeCamera = m_scene->getActiveCamera();
	std::ostringstream eyexStr;
	eyexStr << m_activeCamera->getEye().x;
	eyeX.SetWindowTextA(_T(eyexStr.str().c_str()));
	std::ostringstream eyeyStr;
	eyeyStr << m_activeCamera->getEye().y;
	eyeY.SetWindowTextA(_T(eyeyStr.str().c_str()));
	std::ostringstream eyezStr;
	eyezStr << m_activeCamera->getEye().z;
	eyeZ.SetWindowTextA(_T(eyezStr.str().c_str()));
	std::ostringstream atxStr;
	atxStr << m_activeCamera->getAt().x;
	atX.SetWindowTextA(_T(atxStr.str().c_str()));
	std::ostringstream atyStr;
	atyStr << m_activeCamera->getAt().y;
	atY.SetWindowTextA(_T(atyStr.str().c_str()));
	std::ostringstream atzStr;
	atzStr << m_activeCamera->getAt().z;
	atZ.SetWindowTextA(_T(atzStr.str().c_str()));
	std::ostringstream upxStr;
	upxStr << m_activeCamera->getUp().x;
	upX.SetWindowTextA(_T(upxStr.str().c_str()));
	std::ostringstream upyStr;
	upyStr << m_activeCamera->getUp().y;
	upY.SetWindowTextA(_T(upyStr.str().c_str()));
	std::ostringstream upzStr;
	upzStr << m_activeCamera->getUp().z;
	upZ.SetWindowTextA(_T(upzStr.str().c_str()));

}
