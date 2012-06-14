#include "regex_validating_edit.h"

#include "ui/XTPValidatingControlEdit.h"


class QuantityEdit : public CXTPValidatingEditCtrl
{
public:
  QuantityEdit();
  virtual ~QuantityEdit() {};

  // get the value or unit from the user input string using regex extract.
  double value();
  string16 unit();  

  // if unit is set, the edit only allow this kid of unit.
  // set behind create at once.
  void set_unit(string16 unit);

private:
  virtual bool SemanticCheck(const CString &strText, 
    CString *pstrErrorMsg = 0) OVERRIDE;

  virtual void OnValidatedText(CString text) OVERRIDE;

private: 
  double value_;
  string16 unit_;
  bool set_unit_;
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

