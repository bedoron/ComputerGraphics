// ModelData.cpp : implementation file
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "ModelData.h"
#include "afxdialogex.h"
#include "vec.h"
#include "Model.h"
#include "Texture.h"

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
	//DDX_Control(pDX, DRAW_BOX, draw_box);
	//DDX_Control(pDX, DRAW_NORMAL, draw_normals);
	//DDX_Control(pDX, DRAW_VNORMAL, draw_vnormals);
	DDX_Control(pDX, IDC_color_Blue, _colorX);
	DDX_Control(pDX, IDC_color_green, _colorY);
	DDX_Control(pDX, IDC_color_red, _colorZ);
	DDX_Control(pDX, diffuzeX, _diffX);
	DDX_Control(pDX, diffuzeY, _diffY);
	DDX_Control(pDX, diffuzeZ, _diffZ);
	DDX_Control(pDX, ambiantX, _ambX);
	DDX_Control(pDX, ambiantY, _ambY);
	DDX_Control(pDX, ambiantZ, _ambZ);
	DDX_Control(pDX, SpecularX, _spcX);
	DDX_Control(pDX, SpecularY, _spcY);
	DDX_Control(pDX, SpecularZ, _spcZ);
	DDX_Control(pDX, IDC_shine, _shine);
	DDX_Control(pDX, IDC_colors, _colors);
	DDX_Control(pDX, IDC_clc_nrml, _calcNrml);
	DDX_Control(pDX, ShadersList, shaders);
	DDX_Control(pDX, SamplersList, samplers);
	DDX_Control(pDX, TexturesList, textures);

	DDX_Control(pDX, OVERRIDE_AMBIENT, override_ambient);
	DDX_Control(pDX, OVERRIDE_SPECULAR, override_specular);
	DDX_Control(pDX, OVERRIDE_DIFFUSE, override_diffuse);
}


BEGIN_MESSAGE_MAP(CModelData, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDOK, &CModelData::OnBnClickedOk)
	ON_CBN_SELCHANGE(ROTATION_SELECT, &CModelData::OnCbnSelchangeSelect)
	ON_BN_CLICKED(IDC_RD_flat, &CModelData::OnBnClickedRdflat)
	ON_BN_CLICKED(IDC_RD_frnch, &CModelData::OnBnClickedRdfrnch)
	ON_BN_CLICKED(IDC_RD_png, &CModelData::OnBnClickedRdpng)
	ON_LBN_SELCHANGE(ShadersList, &CModelData::OnShadersChange)
	ON_LBN_SELCHANGE(SamplersList, &CModelData::OnSamplerChange)
	ON_LBN_SELCHANGE(TexturesList, &CModelData::OnTextureChange)
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
void CModelData::setScene(Scene* scene) { 
	m_scene = scene; 
	vector<string> snames = scene->listShaders();
	vector<string>::iterator it = snames.begin();
	UpdateData(false);
	for(; it != snames.end(); ++it) {
		CString strText = _T(it->c_str());
		shaders.AddString(strText);
	}
	UpdateData(true);

};

void CModelData::setModel(Model* model) { 
	m_model = model; 
	Shader* shader = m_model->getShader();
	if(shader == NULL) return;
	currentShader = shader;

	int i=0;
	CString strText;
	int size = m_scene->listShaders().size();
	UpdateData(true);
	for(i=0; i < size; ++i) {
		shaders.GetText(i, strText);
		string theName = strText.GetString();
		if(theName[0] == ' ')
			theName = theName.substr(2);
		if(shader->getName() == theName)
			break;
	}
	UpdateData(false);
	shaders.UpdateData(false);
	shaders.SetCurSel(i);

	override_ambient.SetCheck(BST_UNCHECKED);
	override_diffuse.SetCheck(BST_UNCHECKED);
	override_specular.SetCheck(BST_UNCHECKED);
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
	//draw_box.SetCheck((static_cast<Model*>(m_model)->getDrawBox())?BST_CHECKED:BST_UNCHECKED);
	//draw_normals.SetCheck((static_cast<Model*>(m_model)->getNormal())?BST_CHECKED:BST_UNCHECKED);
	//draw_vnormals.SetCheck((static_cast<Model*>(m_model)->getVertexNormal())?BST_CHECKED:BST_UNCHECKED);
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
	bool bla;
	return readAndClearRotation(bla);
}

mat4 CModelData::readAndClearRotation(bool& rotated) {
	char buffer[5];
	int angleX, angleY, angleZ;
	mat4 rotx, roty, rotz;

	xrot.GetWindowTextA(buffer, 5);
	angleX = atoi(buffer);
	yrot.GetWindowTextA(buffer, 5);
	angleY = atoi(buffer);
	zrot.GetWindowTextA(buffer, 5);
	angleZ = atoi(buffer);

	rotx = RotateX(angleX);
	roty = RotateY(angleY);
	rotz = RotateZ(angleZ);

	rotated = (bool) (angleX + angleY + angleZ);

	xrot.SetWindowTextA(_T("0"));
	yrot.SetWindowTextA(_T("0"));
	zrot.SetWindowTextA(_T("0"));

	return rotx*roty*rotz;
}

void CModelData::rotateModel(mat4 rotation) {

	mat4 oTransform = m_model->getObjectTransform();
	oTransform = oTransform * rotation; // always rotate in respect to self
		
	if(!m_scene->isModelFrame()) { // Apply rotation around world frame origin
		oTransform = transpose(oTransform);
		vec4 eye = oTransform[3];				// Get the translation
		eye = rotation * eye;					// rotate the translation
		oTransform[3] = eye;					// plant it back
		oTransform = transpose(oTransform);		
	}

	m_model->setObjectTransform(oTransform);
}

void CModelData::OnBnClickedOk()
{
	UpdateData(false);

	char buffer[5];
	int newX, newY, newZ;
	float newXRot, newYRot, newZRot;
	float newScale;
	// Deal with Checkboxes
	//static_cast<Model*>(m_model)->setDrawBox(draw_box.GetCheck()==BST_CHECKED);
	//static_cast<Model*>(m_model)->setVertexNormal(draw_vnormals.GetCheck()==BST_CHECKED);
	//static_cast<Model*>(m_model)->setNormal(draw_normals.GetCheck()==BST_CHECKED);

	char colorBufferX[5];
	_colorX.GetWindowTextA(colorBufferX, 5);
	char colorBufferY[5];
	_colorY.GetWindowTextA(colorBufferY, 5);
	char colorBufferZ[5];
	_colorZ.GetWindowTextA(colorBufferZ, 5);
	static_cast<Model*>(m_model)->setColor(vec3(atof(colorBufferX),atof(colorBufferY),atof(colorBufferZ)));
	_diffX.GetWindowTextA(colorBufferX, 5);
	_diffY.GetWindowTextA(colorBufferY, 5);
	_diffZ.GetWindowTextA(colorBufferZ, 5);
	
	if(override_diffuse.GetCheck() == BST_CHECKED) 
		static_cast<Model*>(m_model)->setKDiffuze(vec3(atof(colorBufferX),atof(colorBufferY),atof(colorBufferZ)));

	_ambX.GetWindowTextA(colorBufferX, 5);
	_ambY.GetWindowTextA(colorBufferY, 5);
	_ambZ.GetWindowTextA(colorBufferZ, 5);

	if(override_ambient.GetCheck() == BST_CHECKED) 
		static_cast<Model*>(m_model)->setKAbmbiant(vec3(atof(colorBufferX),atof(colorBufferY),atof(colorBufferZ)));

	_spcX.GetWindowTextA(colorBufferX, 5);
	_spcY.GetWindowTextA(colorBufferY, 5);
	_spcZ.GetWindowTextA(colorBufferZ, 5);
	if(override_specular.GetCheck() == BST_CHECKED) 
		static_cast<Model*>(m_model)->setKspecular(vec3(atof(colorBufferX),atof(colorBufferY),atof(colorBufferZ)));

	_shine.GetWindowTextA(colorBufferX,5);
	static_cast<Model*>(m_model)->setShininess(atof(colorBufferX));
	static_cast<Model*>(m_model)->setVNormal(_calcNrml.GetCheck()==BST_CHECKED);

	_colors.GetWindowTextA(colorBufferX,5);
	static_cast<Model*>(m_model)->setColor(atoi(colorBufferX));
	static_cast<Model*>(m_model)->setRenderType(renderMode);
	// Deal with rotation
	bool dontTranslate;
	rotateModel(readAndClearRotation(dontTranslate));
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
//	mat4 objTransform = m_model->getObjectTransform();
	
	objCenter.x = -(objCenter.x-newX);
	objCenter.y = -(objCenter.y-newY);
	objCenter.z = -(objCenter.z-newZ);
	
	// Set center coords and scale
	static_cast<Model*>(m_model)->scale(scaler);
	
	mat4 moveModel;
	if(!dontTranslate) // When rotating element the data from the input box is invalid
		moveModel = Translate(objCenter); 

	m_scene->draw(moveModel);
	//CDialogEx::OnOK();
}




void CModelData::OnShadersChange() {
	CString strText;
	int index = shaders.GetCurSel();
	shaders.GetText(index, strText);

	Shader* shader = m_scene->getShader(strText.GetString());
	if(shader == NULL) return;
	currentShader = shader;
	const map<string, string> &shaderSamplers = shader->getSamplerNames();
	map<string, string>::const_iterator it = shaderSamplers.begin();

	m_model->setShader(shader);


	samplers.ResetContent();
	UpdateData(false);
	for(; it != shaderSamplers.end(); ++it) {
		CString strText = _T(it->second.c_str());
		samplers.AddString(strText);
	}
	UpdateData(true);



	m_scene->draw();
}

void CModelData::OnSamplerChange() { 
	CString strText;
	int index = samplers.GetCurSel();
	samplers.GetText(index, strText);
	string appName = currentShader->translateSamplerName(strText.GetString());
	activeSamplerName = appName;
	if(appName == "") throw exception();

	// Now check which texture is set for it
	const map<string, Texture*>& model_tex = m_model->getTextures();
	map<string, Texture*>::const_iterator it = model_tex.find(appName);
	if(it == model_tex.end()) return;
	const Texture *tex = it->second;
	const string texName = tex->getName();

	// Iterate textures and select texName
	CString elm;
	int elms = textures.GetCount();
	for(int i=0; i< elms; ++i) {
		textures.GetText(i, elm);
		if(elm.GetString() == texName) {
			UpdateData(false);
			textures.SetCurSel(i);
			break;
		}
	}
}

void CModelData::reloadModelData() {

}

void CModelData::updateTextures(const vector<const string>& texNames) {
	UpdateData(false);
	textures.ResetContent();
	vector<const string>::const_iterator it = texNames.begin();
	for(; it != texNames.end(); ++it) {
		CString strText = _T(it->c_str());
		textures.AddString(strText);
	}
}


void CModelData::OnTextureChange() { 
	CString strText;
	int index = textures.GetCurSel();
	textures.GetText(index, strText);

	m_scene->updateModelSampler(m_model, activeSamplerName, strText.GetString());
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
void CModelData::initColors()
{
	_colorX.SetWindowTextA(_T("255"));
	_colorY.SetWindowTextA(_T("255"));
	_colorZ.SetWindowTextA(_T("255"));
	_diffX.SetWindowTextA(_T("1"));
	_diffY.SetWindowTextA(_T("1"));
	_diffZ.SetWindowTextA(_T("1"));
	_ambX.SetWindowTextA(_T("0"));
	_ambY.SetWindowTextA(_T("0"));
	_ambZ.SetWindowTextA(_T("0"));
	_spcX.SetWindowTextA(_T("1"));
	_spcY.SetWindowTextA(_T("1"));
	_spcZ.SetWindowTextA(_T("1"));
	_shine.SetWindowTextA(_T("18"));
	_colors.SetWindowTextA(_T("255"));
	_calcNrml.SetCheck(true);
}

void CModelData::OnBnClickedRdflat()
{
	renderMode = Flat;
}


void CModelData::OnBnClickedRdfrnch()
{
	renderMode = french;
}


void CModelData::OnBnClickedRdpng()
{
	renderMode = Phong;
}
