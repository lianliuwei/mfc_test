#include "regex_validating_edit.h"

#include "ui/XTPValidatingControlEdit.h"


class QuantityEdit : public CXTPValidatingEditCtrl
{
public:
  QuantityEdit() 
    : CXTPValidatingEditCtrl(
      "^([+-]?(\\d+|\\d+\\.\\d+)([eE][+-]?\\d+)?)\\s*(\\w*)$",
      "^[+-]?((\\d*)|(\\d+\\.)|((\\d+|\\d+\\.\\d+)[eE][+-]?)|((\\d+|\\d+\\.\\d+)([eE][+-]?\\d+)?\\s*([a-zA-Z]*)))$",
      _T("type \"double( unit)\"")) {};

  virtual ~QuantityEdit() {};
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

