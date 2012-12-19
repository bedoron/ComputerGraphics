#pragma once
#include "Scene.h"

// AddFog dialog

class AddFog : public CDialogEx
{
	DECLARE_DYNAMIC(AddFog)
private:
	Scene* scene;
public:
	AddFog(CWnd* pParent = NULL);   // standard constructor
	virtual ~AddFog();
	
// Dialog Data
	enum { IDD = IDD_ADDFOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CEdit _fogColorBlue;
	CEdit _fogColorGreen;
	CEdit _fogColorRed;
	CEdit _fogDensity;
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnBnClickedOk();
	void setScene(Scene* _scene){scene= _scene;}
};
