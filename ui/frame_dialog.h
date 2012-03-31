#pragma once

#include "resources/resource.h"

// MainFrameDlg dialog
class MainFrameDlg : public CDialog
{
public:
  MainFrameDlg(CWnd* pParent = NULL);   // standard constructor

  enum {IDD = IDD_CODEJOCK_DLG_DIALOG};

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


protected:
  HICON m_hIcon;

  // Generated message map functions
  virtual BOOL OnInitDialog();
	afx_msg
  void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg
  void OnPaint();
	afx_msg
  HCURSOR OnQueryDragIcon();

  DECLARE_MESSAGE_MAP()
};
