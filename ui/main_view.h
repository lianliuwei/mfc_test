#pragma once

class MainView : public CWnd
{
public:
    MainView();
    virtual ~MainView();

protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};
