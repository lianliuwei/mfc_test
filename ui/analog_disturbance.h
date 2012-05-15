#pragma once

#include "resources/stress_resource.h"

class AnalogDisturbanceView : public CFormView
{
  DECLARE_DYNCREATE(AnalogDisturbanceView)

public:
  AnalogDisturbanceView();
  virtual ~AnalogDisturbanceView() {};

  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
    DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, 
    UINT nID, CCreateContext* pContext);

private:
  enum {IDD = IDD_ANALOG_DISTURBANCE};

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  DECLARE_MESSAGE_MAP()
};