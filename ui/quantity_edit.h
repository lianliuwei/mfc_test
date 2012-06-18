#include "regex_validating_edit.h"

#include "ui/XTPValidatingControlEdit.h"

class CommandUpdater;
namespace {
class QuantityEdit;
}

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

  virtual CXTPControlEditCtrl* CreateEditControl();

  void UpdateWindowText(double value, string16 unit);

private:
  CommandUpdater* command_updater_;
  double value_;
  string16 unit_;
  int id_;

  DECLARE_XTP_CONTROL(CXTPQuantityEdit)
};

