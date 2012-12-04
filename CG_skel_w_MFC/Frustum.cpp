// Frustum.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "Frustum.h"




Frustum::Frustum(CWnd* pParent /*=NULL*/)
: CDialogEx(Frustum::IDD, pParent)
{
	
}

Frustum::~Frustum()
{
}


BEGIN_MESSAGE_MAP(Frustum, CDialogEx)
	ON_BN_CLICKED(IDOK, &Frustum::OnBnClickedOk)
END_MESSAGE_MAP()



// Frustum message handlers


// Frustum.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "Frustum.h"


// Frustum

IMPLEMENT_DYNAMIC(Frustum, CDialogEx)




void Frustum::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, ID_left, left);
	DDX_Control(pDX, ID_right, right);
	DDX_Control(pDX, ID_up, up);
	DDX_Control(pDX, ID_down, down);
	DDX_Control(pDX, ID_znear, znear);
	DDX_Control(pDX, ID_zfar, zfar);
}


float Frustum::getleft()
{
	char buffer[5];
	left.GetWindowTextA(buffer, 5);
	return atof(buffer);
}
float Frustum::getRight()
{
	char buffer[5];
	right.GetWindowTextA(buffer, 5);
	return atof(buffer);
}
float Frustum::getUP()
{
	char buffer[5];
	up.GetWindowTextA(buffer, 5);
	return atof(buffer);
}
float Frustum::getDown()
{
	char buffer[5];
	down.GetWindowTextA(buffer, 5);
	return atof(buffer);
}
float Frustum::getZNear()
{
	char buffer[5];
	znear.GetWindowTextA(buffer, 5);
	return atof(buffer);
}
float Frustum::getZFar()
{
	char buffer[5];
	zfar.GetWindowTextA(buffer, 5);
	return atof(buffer);
}

// Frustum message handlers




void Frustum::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	if(mode)
		m_scene->setOrtho(getleft(),getRight(),getUP(),getDown(),getZNear(),getZFar());
	else
		m_scene->setFrustum(getleft(),getRight(),getUP(),getDown(),getZNear(),getZFar());
}
void Frustum::setName(string name)
{
	SetWindowText(name.c_str());
	left.SetWindowTextA(_T("0"));
	right.SetWindowTextA(_T("0"));
	up.SetWindowTextA(_T("0"));
	down.SetWindowTextA(_T("0"));
	znear.SetWindowTextA(_T("0"));
	zfar.SetWindowTextA(_T("0"));
}