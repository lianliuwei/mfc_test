#include "regex_validating_edit.h"

#include "ui/XTPValidatingControlEdit.h"


class QuantityEdit : public CXTPValidatingEditCtrl
{
public:
  QuantityEdit();

  virtual ~QuantityEdit() {};
  
private:
  virtual void OnValidatedText(CString text);

private:
  // get the value or unit from the user input string using regex extract.
  double value();
  string16 unit();

  double value_;
  string16 unit_;
};

class CXTPQuantityEdit : public CXTPValidatingControlEdit
{
public:
  CXTPQuantityEdit() {};
  virtual ~CXTPQuantityEdit(){};
private:
  virtual CXTPControlEditCtrl* CreateEditControl() {
    return new QuantityEdit;
  }
};

