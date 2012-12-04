#include "Scene.h"
#pragma once


// Frustum

class Frustum : public CDialogEx
{
	DECLARE_DYNAMIC(Frustum)
	Scene* m_scene;
public:
	Frustum(CWnd* pParent = NULL);
	virtual ~Frustum();
	enum { IDD = IDD_frustum };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	CEdit left;
	CEdit right;
	CEdit up;
	CEdit down;
	CEdit znear;
	CEdit zfar;
	bool mode;
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	float getleft();
	float getRight();
	float getUP();
	float getDown();
	float getZNear();
	float getZFar();
	void setScene(Scene* scene) { m_scene = scene; };
	afx_msg void OnBnClickedOk();
	void setName(string caption);
	void setMode(bool Ortho){mode = Ortho;}
};


