#pragma once
#include "afxwin.h"
#include "Scene.h"
#include "vec.h"

// CModelData dialog

class CModelData : public CDialogEx
{
	DECLARE_DYNAMIC(CModelData)

public:
	CModelData(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModelData();

// Dialog Data
	enum { IDD = IDD_MODELDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit xcord;
	CEdit ycord;
	CEdit zcord;
	CEdit xrot;
	CEdit yrot;
	CEdit zrot;
	CEdit scale;
	CEdit _colorX;
	CEdit _colorY;
	CEdit _colorZ;
	CEdit _diffX;
	CEdit _diffY;
	CEdit _diffZ;
	CEdit _ambX;
	CEdit _ambY;
	CEdit _ambZ;
	CEdit _spcX;
	CEdit _spcY;
	CEdit _spcZ;
	CEdit _shine;
	CButton _calcNrml;
	CEdit _colors;
	CButton _cartoonize;
	int renderMode;
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	Model* m_model;
	Scene* m_scene;
	void setModel(Model* model) { m_model = model; };
	void setScene(Scene* scene) { m_scene = scene; };
	void updateCenter();
	CStatic model_name;
	
	int model_rotate;
	afx_msg void OnBnClickedRotate();
	afx_msg void OnBnSetfocusRotate();
	afx_msg void OnBnCheckChange();
	CComboBox rotation_select;
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeSelect();
	CEdit xscale;
	CEdit yscale;
	CEdit zscale;

	vec3 readAndClearScale();
	mat4 readAndClearRotation();
	mat4 readAndClearRotation(bool& rotated);
	void rotateModel(mat4 rotation);
	CButton draw_box;
	CButton draw_normals;
	CButton draw_vnormals;

	void refreshModelData();
	void initColors();
	afx_msg void OnBnClickedRdflat();
	afx_msg void OnBnClickedRdfrnch();
	afx_msg void OnBnClickedRdpng();
};
