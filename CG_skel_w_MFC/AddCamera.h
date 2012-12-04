#pragma once
#include "Scene.h"

// AddCamera dialog

class AddCamera : public CDialogEx
{
	DECLARE_DYNAMIC(AddCamera)

public:
	AddCamera(CWnd* pParent = NULL);   // standard constructor
	virtual ~AddCamera();

// Dialog Data
	enum { IDD = IDD_ADDCAMERA };
	vec3 getEye();
	vec3 getAt();
	vec3 getUp();
	void setScene(Scene* scene) { m_scene = scene; };
	void resetData();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CEdit eyeX;
	CEdit eyeY;
	CEdit eyeZ;
	CEdit atX;
	CEdit atY;
	CEdit atZ;
	CEdit upX;
	CEdit upY;
	CEdit upZ;
	bool _mode;
	Scene* m_scene;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void refreshCameraData();
	void setMode(bool add){_mode=add;}
};
