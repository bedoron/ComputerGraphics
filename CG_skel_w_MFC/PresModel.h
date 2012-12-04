#pragma once
#include "Scene.h"

// PresModel dialog

class PresModel : public CDialogEx
{
	DECLARE_DYNAMIC(PresModel)

	Scene* m_scene;
public:
	PresModel(CWnd* pParent = NULL);   // standard constructor
	virtual ~PresModel();

// Dialog Data
	enum { IDD = IDD_Prespective };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CEdit fovy;
	CEdit aspect;
	CEdit znear;
	CEdit zfar;
	DECLARE_MESSAGE_MAP()

public:
	float getFovy();
	float getAspect();
	float getZnear();
	float getzfar();
	void setScene(Scene* scene) { m_scene = scene; };
	afx_msg void OnBnClickedOk();
};
