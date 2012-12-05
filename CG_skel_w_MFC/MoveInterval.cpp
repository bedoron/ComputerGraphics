// MoveInterval.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "MoveInterval.h"
#include "afxdialogex.h"


// MoveInterval dialog

IMPLEMENT_DYNAMIC(MoveInterval, CDialogEx)

MoveInterval::MoveInterval(CWnd* pParent /*=NULL*/)
	: CDialogEx(MoveInterval::IDD, pParent)
{

}

MoveInterval::~MoveInterval()
{
}

void MoveInterval::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_INT, _interval);
}

GLfloat MoveInterval::getInterval()
{
	char buffer[5];
	_interval.GetWindowTextA(buffer, 5);
	return atof(buffer);
}
BEGIN_MESSAGE_MAP(MoveInterval, CDialogEx)
	ON_BN_CLICKED(IDOK, &MoveInterval::OnBnClickedOk)
END_MESSAGE_MAP()


// MoveInterval message handlers


void MoveInterval::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	m_scene->setMoveInterval(getInterval());
}
