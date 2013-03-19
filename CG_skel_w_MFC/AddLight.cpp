// AddLight.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "AddLight.h"
#include "afxdialogex.h"


// AddLight dialog

IMPLEMENT_DYNAMIC(AddLight, CDialogEx)

AddLight::AddLight(CWnd* pParent /*=NULL*/)
	: CDialogEx(AddLight::IDD, pParent)
{

}

AddLight::~AddLight()
{
}

void AddLight::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_RD_AM, l_ambient);
	DDX_Control(pDX, IDC_RD_PRL, l_Parelel);
	DDX_Control(pDX, IDC_RD_pnt, l_point);
	DDX_Control(pDX, IDC_DirectionX, DirectionX);
	DDX_Control(pDX, IDC_DirectionY, DirectionY);
	DDX_Control(pDX, IDC_DirectionZ, DirectionZ);
	//DDX_Control(pDX, IDC_LocationX, locationX);
	//DDX_Control(pDX, IDC_LocationY, locationY);
	//DDX_Control(pDX, IDC_LocationZ, locationZ);
	DDX_Control(pDX, IDC_IntencityR, IntencityX);
	DDX_Control(pDX, IDC_IntencityG, IntencityY);
	DDX_Control(pDX, IDC_IntencityB, IntencityZ);
	//DDX_Control(pDX, IDC_angle, Angle);

	DDX_Control(pDX, DIR_LOC, dir_loc);
}


BEGIN_MESSAGE_MAP(AddLight, CDialogEx)
	//ON_BN_CLICKED(IDC_RD_AM, &AddLight::OnBnClickedRdAm)
	ON_BN_CLICKED(IDC_RD_PRL, &AddLight::OnBnClickedRdPrl)
	ON_BN_CLICKED(IDC_RD_pnt, &AddLight::OnBnClickedRdpnt)
	ON_BN_CLICKED(IDOK, &AddLight::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &AddLight::OnBnClickedButton1)
END_MESSAGE_MAP()


// AddLight message handlers



void AddLight::OnBnClickedRdAm()
{
	// Removed
}


void AddLight::OnBnClickedRdPrl() // Parallel light
{
	dir_loc.SetWindowTextA(_T("Direction"));
	isPoint = false;
}


void AddLight::OnBnClickedRdpnt() // Point light
{
	dir_loc.SetWindowTextA(_T("Location"));
	isPoint = true;
}

int AddLight::getLightType()
{
	return 0;
}

void AddLight::OnBnClickedOk()
{
	CDialogEx::OnOK();
	float r,g,b;

	char bufferX[5];
	char bufferY[5];
	char bufferZ[5];

	IntencityX.GetWindowTextA(bufferX,5);
	IntencityY.GetWindowTextA(bufferY,5);
	IntencityZ.GetWindowTextA(bufferZ,5);

	r = atof(bufferX); 
	g = atof(bufferY);
	b = atof(bufferZ);

	vec4 color(r, g, b, 0);
	

	DirectionX.GetWindowTextA(bufferX,5);
	DirectionY.GetWindowTextA(bufferY,5);
	DirectionZ.GetWindowTextA(bufferZ,5);	

	vec4 point(atof(bufferX),atof(bufferY),atof(bufferZ), isPoint);

	Light *newLight = new Light(point, color);
	_scene->addLight(newLight);
}


void AddLight::OnBnClickedButton1()
{
	DirectionX.SetWindowTextA(_T("1"));
	DirectionY.SetWindowTextA(_T("1"));
	DirectionZ.SetWindowTextA(_T("1"));
	IntencityX.SetWindowTextA(_T("1"));
	IntencityY.SetWindowTextA(_T("1"));
	IntencityZ.SetWindowTextA(_T("1"));
}
