// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__80941DC7_4E2C_448D_9EFB_39D39517AF9C__INCLUDED_)
#define AFX_MAINFRM_H__80941DC7_4E2C_448D_9EFB_39D39517AF9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "base/memory/scoped_ptr.h"
#include "command_updater.h"

class XTPConfigBar;

class CMainFrame : public CXTPMDIFrameWnd
                 , public CommandUpdater::CommandUpdaterDelegate
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

public:
  virtual void ExecuteCommand(int id, CommandParam& param);

private:
  void InitCommandState();

  bool osc_on_off_;
  scoped_ptr<CommandUpdater> command_updater_;
  scoped_ptr<XTPConfigBar> config_bar_;

// Operations
public:
	BOOL CreateRibbonBar();

  void LoadIcons();
	void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	CXTPTabClientWnd m_wndClient;

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTPStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewFullscreen();
	afx_msg void OnViewNormal();
	afx_msg void OnUpdateViewNormal(CCmdUI* pCmdUI);
	afx_msg void OnWindowArrange();
	afx_msg void OnUpdateWindowSwitch(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	void OnViewWorkspace();
	void OnUpdateViewWorkspace(CCmdUI* pCmdUI);
	void OnUpdateRibbonTab(CCmdUI* pCmdUI);

	BOOL m_bShowWorkspace;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__80941DC7_4E2C_448D_9EFB_39D39517AF9C__INCLUDED_)
