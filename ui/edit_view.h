#pragma once

class EditView : public CView
{
public:
  EditView() {};
  virtual ~EditView() {};

protected:
  virtual void OnDraw(CDC* pDC) {};

//	DECLARE_MESSAGE_MAP()
};