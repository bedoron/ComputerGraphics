#pragma once

#include "Scene.h"
#include "afxwin.h"
// MoveInterval dialog

class MoveInterval : public CDialogEx
{
	DECLARE_DYNAMIC(MoveInterval)
	Scene* m_scene;
public:
	MoveInterval(CWnd* pParent = NULL);   // standard constructor
	virtual ~MoveInterval();

// Dialog Data
	enum { IDD = IDD_MOVEINTERVAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	GLfloat getInterval();
	DECLARE_MESSAGE_MAP()
public:
	CEdit _interval;
	afx_msg void OnBnClickedOk();
	void setScene(Scene* scene) { m_scene = scene; };
};
