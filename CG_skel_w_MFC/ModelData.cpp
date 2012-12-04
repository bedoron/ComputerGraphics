// ModelData.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "ModelData.h"
#include "afxdialogex.h"
#include "vec.h"
#include "MeshModel.h"


// CModelData dialog

IMPLEMENT_DYNAMIC(CModelData, CDialogEx)

CModelData::CModelData(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModelData::IDD, pParent)
	, model_rotate(0)
{

}

CModelData::~CModelData()
{
}

void CModelData::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, XCORD, xcord);
	DDX_Control(pDX, YCORD, ycord);
	DDX_Control(pDX, ZCORD, zcord);
	DDX_Control(pDX, XROT, xrot);
	DDX_Control(pDX, YROT, yrot);
	DDX_Control(pDX, ZROT, zrot);
	DDX_Control(pDX, MODEL_NAME_MAIN, model_name);
	DDX_Control(pDX, ROTATION_SELECT, rotation_select);
	DDX_Control(pDX, XSCALE, xscale);
	DDX_Control(pDX, YSCALE, yscale);
	DDX_Control(pDX, ZSCALE, zscale);
	DDX_Control(pDX, DRAW_BOX, draw_box);
	DDX_Control(pDX, DRAW_NORMAL, draw_normals);
	DDX_Control(pDX, DRAW_VNORMAL, draw_vnormals);
}


BEGIN_MESSAGE_MAP(CModelData, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CModelData::OnBnClickedOk)
	ON_CBN_SELCHANGE(ROTATION_SELECT, &CModelData::OnCbnSelchangeSelect)
END_MESSAGE_MAP()


// CModelData message handlers


void CModelData::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	// TODO: Add your message handler code here
}

vec3 CModelData::readAndClearScale() {
	float scalex, scaley, scalez;
	char buffer[5];
	xscale.GetWindowTextA(buffer, 5);
	scalex = atof(buffer);
	yscale.GetWindowTextA(buffer, 5);
	scaley = atof(buffer);
	zscale.GetWindowTextA(buffer, 5);
	scalez = atof(buffer);
	xscale.SetWindowTextA(_T("1"));
	yscale.SetWindowTextA(_T("1"));
	zscale.SetWindowTextA(_T("1"));
	return vec3(scalex, scaley, scalez);
}


void CModelData::refreshModelData() {
	UpdateData(false);
	SetWindowText(_T("Model properties"));
	model_name.SetWindowTextA(_T(m_model->getName().c_str()));

	// Select rotation type
	if(rotation_select.GetCurSel()<0)
		rotation_select.SetCurSel(0);
	OnCbnSelchangeSelect();

	// Set Default values
	readAndClearRotation();
	readAndClearScale();
	draw_box.SetCheck((static_cast<MeshModel*>(m_model)->getDrawBox())?BST_CHECKED:BST_UNCHECKED);
	draw_normals.SetCheck((static_cast<MeshModel*>(m_model)->getNormal())?BST_CHECKED:BST_UNCHECKED);
	draw_vnormals.SetCheck((static_cast<MeshModel*>(m_model)->getVertexNormal())?BST_CHECKED:BST_UNCHECKED);
	// Get center Coords
	updateCenter();
	UpdateData(true);
}

void CModelData::OnShowWindow(BOOL bShow, UINT nStatus) {
	CDialogEx::OnShowWindow(bShow, nStatus);
	// Get model data from here
	refreshModelData();
//	ShowWindow(SW_SHOW);
}

mat4 CModelData::readAndClearRotation() {
	char buffer[5];
	mat4 rotx, roty, rotz;

	xrot.GetWindowTextA(buffer, 5);
	rotx = RotateX(atof(buffer));
	yrot.GetWindowTextA(buffer, 5);
	roty = RotateY(atof(buffer));
	zrot.GetWindowTextA(buffer, 5);
	rotz = RotateZ(atof(buffer));


	xrot.SetWindowTextA(_T("0"));
	yrot.SetWindowTextA(_T("0"));
	zrot.SetWindowTextA(_T("0"));

	return rotx*roty*rotz;
}

void CModelData::rotateModel(mat4 rotation) {
	if(m_scene->isModelFrame()) {// Apply rotation to model frame origin
		m_model->setObjectTransform(m_model->getObjectTransform()*rotation);
	} else { // Apply rotation around world frame origin
		m_model->setObjectTransform(rotation*m_model->getObjectTransform());
	}
}

void CModelData::OnBnClickedOk()
{
	UpdateData(false);

	char buffer[5];
	int newX, newY, newZ;
	float newXRot, newYRot, newZRot;
	float newScale;
	// Deal with Checkboxes
	static_cast<MeshModel*>(m_model)->setDrawBox(draw_box.GetCheck()==BST_CHECKED);
	static_cast<MeshModel*>(m_model)->setVertexNormal(draw_vnormals.GetCheck()==BST_CHECKED);
	static_cast<MeshModel*>(m_model)->setNormal(draw_normals.GetCheck()==BST_CHECKED);
	// Deal with rotation
	rotateModel(readAndClearRotation());
	// Load New Center coords
	xcord.GetWindowTextA(buffer, 5);
	newX = atoi(buffer);
	ycord.GetWindowTextA(buffer, 5);
	newY = atoi(buffer);
	zcord.GetWindowTextA(buffer, 5);
	newZ = atoi(buffer);
	// Load new Scale factor
	vec3 scaler = readAndClearScale();
	// Calculate difference of center coords
	vec3 objCenter = m_model->getModelCenter();
	mat4 objTransform = m_model->getObjectTransform();
	objCenter.x = -(objCenter.x-newX);
	objCenter.y = -(objCenter.y-newY);
	objCenter.z = -(objCenter.z-newZ);
	
	// Set center coords and scale
	static_cast<MeshModel*>(m_model)->scale(scaler);
	m_scene->draw(Translate(objCenter));
	//CDialogEx::OnOK();
}

void CModelData::updateCenter() {
	UpdateData(false);
	vec3 center = m_model->getModelCenter();
	string x(SSTR(center.x));
	string y(SSTR(center.y));
	string z(SSTR(center.z));
	xcord.SetWindowTextA(_T(x.c_str()));
	ycord.SetWindowTextA(_T(y.c_str()));
	zcord.SetWindowTextA(_T(z.c_str()));
}


void CModelData::OnCbnSelchangeSelect()
{	
	m_scene->activateModel();
	if(0== rotation_select.GetCurSel())
		m_scene->modelFrame();	
	else 
		m_scene->worldFrame();
//		m_scene->activateCamera();
	UpdateData(true);
}
