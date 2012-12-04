
#pragma once

#include <string>
#include <vector>
using std::string;

#include "vec.h"

// ------------------------
//    Class CInputDialog
// ------------------------

class CInputDialog : public CDialog
{
    DECLARE_DYNAMIC(CInputDialog)

public:
    CInputDialog(CString title = "Input Dialog");
    virtual ~CInputDialog();

    virtual BOOL OnInitDialog();

    enum { IDD = IDD_INPUTDIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

private:
    CString mTitle;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
};

// ----------------------
//    Class CCmdDialog
// ----------------------

class CCmdDialog : public CInputDialog
{
public:
    CCmdDialog(CString title = "Input Dialog");
    virtual ~CCmdDialog();

    string GetCmd();

protected:
    CString mCmd;
    CEdit mCmdEdit;

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate (LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// ----------------------
//    Class CXyzDialog
// ----------------------

class CXyzDialog : public CInputDialog
{
public:
    CXyzDialog(CString title = "Input Dialog");
    virtual ~CXyzDialog();

    vec3 GetXYZ();

protected:
    float mX;
    float mY;
    float mZ;
    CEdit mXEdit;
    CEdit mYEdit;
    CEdit mZEdit;
	

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate (LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

// -------------------------
//    Class CCmdXyzDialog
// -------------------------

class CCmdXyzDialog : public CInputDialog
{
public:
    CCmdXyzDialog(CString title = "Input Dialog");
    virtual ~CCmdXyzDialog();

    string GetCmd();
	//void setTextX(string str);
    vec3 GetXYZ();
protected:
    CString mCmd;
    float mX;
    float mY;
    float mZ;
    CEdit mCmdEdit;
    CEdit mXEdit;
    CEdit mYEdit;
    CEdit mZEdit;
	

    virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate (LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();
	
    DECLARE_MESSAGE_MAP()
};
class CRDdialog : public CInputDialog
{
private:
	std::vector<string> _ids;
	std::vector<CButton*> RDbuttons;
	CButton rd;

public:
	CRDdialog(std::vector<string> ids);
	CRDdialog(CString title = "select");
	virtual ~CRDdialog();
	int getSelectedID();

	//virtual void DoDataExchange(CDataExchange* pDX);

    afx_msg int OnCreate (LPCREATESTRUCT lpcs);
    afx_msg void OnPaint();

};