#include "regex_validating_edit.h"

#include "ui/XTPValidatingControlEdit.h"

class CommandUpdater;

// set to CommandUpdter a double for value, a string for unit.
// the unit is equal to the set unit. save the unit for future use.
class QuantityEdit : public CXTPValidatingEditCtrl
{
public:
  QuantityEdit(CommandUpdater* command_updater, int id, string16 unit);
  virtual ~QuantityEdit() {};

  // get the value or unit from the user input string using regex extract.
  double value();
  string16 unit();  

  void set_value(double value);
  // if unit is set, the edit only allow this kid of unit.
  // set behind create at once.
  void set_unit(string16 unit);

private:
  virtual bool SemanticCheck(const CString &strText, 
    CString *pstrErrorMsg = 0) OVERRIDE;

  virtual void OnValidatedText(CString text) OVERRIDE;

  void UpdateWindowText(double value, string16 unit);

  bool HasSetUnit() const {
    return unit_ != EmptyString16();
  };

private: 
  double value_;
  string16 unit_;
  CommandUpdater* command_updater_;
  int id_;
};

class CXTPQuantityEdit : public CXTPValidatingControlEdit
{
public:
  CXTPQuantityEdit(CommandUpdater* command_updater, int id, string16 unit)
    : command_updater_(command_updater)
    , id_(id) 
    , unit_(unit) {
      CHECK(command_updater_ != NULL);
   };
  virtual ~CXTPQuantityEdit(){};

  void set_value(double value) {
    if (m_pEdit == NULL) // edit is no set.
      return;
    QuantityEdit* edit  = static_cast<QuantityEdit*>(m_pEdit);
    edit->set_value(value);
  }

private:
  virtual CXTPControlEditCtrl* CreateEditControl() {
    return new QuantityEdit(command_updater_, id_, unit_);
  }

private:
  CommandUpdater* command_updater_;
  int id_;
  string16 unit_;
};

