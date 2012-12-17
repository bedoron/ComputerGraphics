#pragma once
#include "Scene.h"

// AddLight dialog

class AddLight : public CDialogEx
{
	DECLARE_DYNAMIC(AddLight)

private:
	Scene* _scene;
	int getLightType();
public:
	AddLight(CWnd* pParent = NULL);   // standard constructor
	virtual ~AddLight();

// Dialog Data
	enum { IDD = IDD_ADDLIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CEdit l_ambient;
	CEdit l_Parelel;
	CEdit l_point;
	CEdit DirectionX;
	CEdit DirectionY;
	CEdit DirectionZ;
	CEdit locationX;
	CEdit locationY;
	CEdit locationZ;
	CEdit IntencityX;
	CEdit IntencityY;
	CEdit IntencityZ;
	CEdit Angle;
	int type;
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRdAm();
	afx_msg void OnBnClickedRdPrl();
	afx_msg void OnBnClickedRdpnt();
	afx_msg void OnBnClickedOk();
	void setScene(Scene* scene){_scene=scene;}
};
