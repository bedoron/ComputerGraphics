// PresModel.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "PresModel.h"
#include "afxdialogex.h"


// PresModel dialog

IMPLEMENT_DYNAMIC(PresModel, CDialogEx)

PresModel::PresModel(CWnd* pParent /*=NULL*/)
	: CDialogEx(PresModel::IDD, pParent)
{
}

PresModel::~PresModel()
{
}

void PresModel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_fovy, fovy);
	DDX_Control(pDX, IDC_aspect, aspect);
	DDX_Control(pDX, IDC_znear, znear);
	DDX_Control(pDX, IDC_zfar, zfar);
}
float PresModel::getFovy()
{
	char buffer[5];
	fovy.GetWindowTextA(buffer, 5);
	return atof(buffer);
}
float PresModel::getAspect()
{
	char buffer[5];
	aspect.GetWindowTextA(buffer, 5);
	return atof(buffer);
}
float PresModel::getZnear()
{
	char buffer[5];
	znear.GetWindowTextA(buffer, 5);
	return atof(buffer);
}
float PresModel::getzfar()
{
	char buffer[5];
	zfar.GetWindowTextA(buffer, 5);
	return atof(buffer);
}
BEGIN_MESSAGE_MAP(PresModel, CDialogEx)
	ON_BN_CLICKED(IDOK, &PresModel::OnBnClickedOk)
END_MESSAGE_MAP()


// PresModel message handlers


void PresModel::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	m_scene->setPrespective(getFovy(),getAspect(),getZnear(),getzfar());
}
