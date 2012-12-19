// AdFog.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "AddFog.h"
#include "afxdialogex.h"


// AddFog dialog

IMPLEMENT_DYNAMIC(AddFog, CDialogEx)

AddFog::AddFog(CWnd* pParent /*=NULL*/)
	: CDialogEx(AddFog::IDD, pParent)
{

}

AddFog::~AddFog()
{
}

void AddFog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Fog_Blue, _fogColorBlue);
	DDX_Control(pDX, IDC_Fog_Green, _fogColorGreen);
	DDX_Control(pDX, IDC_Fog_Red, _fogColorRed);
	DDX_Control(pDX, IDC_Fog_density, _fogDensity);
}


BEGIN_MESSAGE_MAP(AddFog, CDialogEx)
	ON_BN_CLICKED(IDOK, &AddFog::OnBnClickedOk)
END_MESSAGE_MAP()


// AddFog message handlers


void AddFog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	char buffer[5];
	_fogColorBlue.GetWindowTextA(buffer, 5);
	GLfloat blue = atof(buffer);
	_fogColorGreen.GetWindowTextA(buffer, 5);
	GLfloat green = atof(buffer);
	_fogColorRed.GetWindowTextA(buffer, 5);
	GLfloat red = atof(buffer);
	_fogDensity.GetWindowTextA(buffer, 5);
	GLfloat density = atof(buffer);
	scene->setFog(vec3(blue,green,red),density);
}
