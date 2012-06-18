#include "regex_validating_edit.h"

#include "ui/XTPValidatingControlEdit.h"

class CommandUpdater;
class CXTPQuantityEdit;

// set to CommandUpdter a double for value, a string for unit.
// the unit is equal to the set unit. save the unit for future use.
class QuantityEdit : public CXTPValidatingEditCtrl
{
public:
  friend class CXTPQuantityEdit;

public:
  QuantityEdit();
  virtual ~QuantityEdit() {};

private:
  virtual bool SemanticCheck(const CString &strText, 
    CString *pstrErrorMsg = 0) OVERRIDE;

  virtual void OnValidatedText(CString text) OVERRIDE;

  bool HasSetUnit() {
    return unit() != EmptyString16();
  };

private:
  int id();
  double value();
  string16 unit();  
  CommandUpdater* command_updater();
};

// if unit is empty allow to type any unit.
// if unit is give, only allow that type of unit. and notify by converted value.
class CXTPQuantityEdit : public CXTPValidatingControlEdit
{
public:
  friend class QuantityEdit; // need access value unit id.

  CXTPQuantityEdit(CommandUpdater* command_updater, int id, string16 unit)
    : command_updater_(command_updater)
    , id_(id) 
    , unit_(unit) {
      CHECK(command_updater_ != NULL);
   };

  virtual ~CXTPQuantityEdit(){};

  void set_value(double value) {
    value_ = value;
    UpdateWindowText(value_, unit_);   
  }

private:
  CXTPQuantityEdit() {};
  virtual void Copy(CXTPControl* pControl, BOOL bRecursive);
  virtual void DoPropExchange(CXTPPropExchange* pPX);

  virtual CXTPControlEditCtrl* CreateEditControl() {
    return new QuantityEdit;
  }

  void UpdateWindowText(double value, string16 unit);

private:
  CommandUpdater* command_updater_;
  double value_;
  string16 unit_;
  int id_;

  DECLARE_XTP_CONTROL(CXTPQuantityEdit)
};

