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
	DDX_Control(pDX, IDC_RD_AM, l_ambient);
	DDX_Control(pDX, IDC_RD_PRL, l_Parelel);
	DDX_Control(pDX, IDC_RD_pnt, l_point);
	DDX_Control(pDX, IDC_DirectionX, DirectionX);
	DDX_Control(pDX, IDC_DirectionY, DirectionY);
	DDX_Control(pDX, IDC_DirectionZ, DirectionZ);
	DDX_Control(pDX, IDC_LocationX, locationX);
	DDX_Control(pDX, IDC_LocationY, locationY);
	DDX_Control(pDX, IDC_LocationZ, locationZ);
	DDX_Control(pDX, IDC_IntencityB, IntencityX);
	DDX_Control(pDX, IDC_IntencityG, IntencityY);
	DDX_Control(pDX, IDC_IntencityR, IntencityZ);
	DDX_Control(pDX, IDC_angle, Angle);

}


BEGIN_MESSAGE_MAP(AddLight, CDialogEx)
	ON_BN_CLICKED(IDC_RD_AM, &AddLight::OnBnClickedRdAm)
	ON_BN_CLICKED(IDC_RD_PRL, &AddLight::OnBnClickedRdPrl)
	ON_BN_CLICKED(IDC_RD_pnt, &AddLight::OnBnClickedRdpnt)
	ON_BN_CLICKED(IDOK, &AddLight::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &AddLight::OnBnClickedButton1)
END_MESSAGE_MAP()


// AddLight message handlers



void AddLight::OnBnClickedRdAm()
{
	DirectionX.EnableWindow(false);
	DirectionY.EnableWindow(false);
	DirectionZ.EnableWindow(false);
	locationX.EnableWindow(false);
	locationY.EnableWindow(false);
	locationZ.EnableWindow(false);
	Angle.EnableWindow(false);
	type= L_AMBIANT;
}


void AddLight::OnBnClickedRdPrl()
{
	locationX.EnableWindow(false);
	locationY.EnableWindow(false);
	locationZ.EnableWindow(false);
	DirectionX.EnableWindow(true);
	DirectionY.EnableWindow(true);
	DirectionZ.EnableWindow(true);
	Angle.EnableWindow(false);
	type= L_PARALEL;
}


void AddLight::OnBnClickedRdpnt()
{
	locationX.EnableWindow(true);
	locationY.EnableWindow(true);
	locationZ.EnableWindow(true);
	DirectionX.EnableWindow(true);
	DirectionY.EnableWindow(true);
	DirectionZ.EnableWindow(true);
	Angle.EnableWindow(true);
	type= L_POINT;
}
int AddLight::getLightType()
{
	switch(l_ambient.GetCheckedRadioButton(IDC_RD_AM,IDC_RD_PRL))
	{
	case IDC_RD_AM:
	{
		return L_AMBIANT;
		break;
	}
	case IDC_RD_pnt:
	{
		return L_POINT;
		break;
	}
	case IDC_RD_PRL:
	{
		return L_PARALEL;
		break;
	}
	}
}

void AddLight::OnBnClickedOk()
{
	CDialogEx::OnOK();
	Light* newLight = new Light();
	newLight->setLightType(type);
	char bufferX[5];
	char bufferY[5];
	char bufferZ[5];
	IntencityX.GetWindowTextA(bufferX,5);
	IntencityY.GetWindowTextA(bufferY,5);
	IntencityZ.GetWindowTextA(bufferZ,5);
	newLight->setIntencity(vec3(atof(bufferX),atof(bufferY),atof(bufferZ)));
	switch (type)
	{
	case L_AMBIANT:
	{
		_scene->addLight(newLight);
		break;
	}
	case L_POINT:
	{
		DirectionX.GetWindowTextA(bufferX,5);
		DirectionY.GetWindowTextA(bufferY,5);
		DirectionZ.GetWindowTextA(bufferZ,5);
		newLight->setDirection(vec3(atof(bufferX),atof(bufferY),atof(bufferZ)));
		locationX.GetWindowTextA(bufferX,5);
		locationY.GetWindowTextA(bufferY,5);
		locationZ.GetWindowTextA(bufferZ,5);
		newLight->setLocation(vec3(atof(bufferX),atof(bufferY),atof(bufferZ)));
		Angle.GetWindowTextA(bufferX,5);
		newLight->setAngle(atof(bufferX));
		_scene->addLight(newLight);
		break;
	}
	case L_PARALEL:
	{
		DirectionX.GetWindowTextA(bufferX,5);
		DirectionY.GetWindowTextA(bufferY,5);
		DirectionZ.GetWindowTextA(bufferZ,5);
		newLight->setDirection(vec3(atof(bufferX),atof(bufferY),atof(bufferZ)));
		locationX.GetWindowTextA(bufferX,5);
		locationY.GetWindowTextA(bufferY,5);
		locationZ.GetWindowTextA(bufferZ,5);
		newLight->setLocation(vec3(atof(bufferX),atof(bufferY),atof(bufferZ)));
		_scene->addLight(newLight);
		break;
	}
	}
}


void AddLight::OnBnClickedButton1()
{
	DirectionX.SetWindowTextA(_T("1"));
	DirectionY.SetWindowTextA(_T("1"));
	DirectionZ.SetWindowTextA(_T("1"));
	IntencityX.SetWindowTextA(_T("255"));
	IntencityY.SetWindowTextA(_T("255"));
	IntencityZ.SetWindowTextA(_T("255"));
}
