#include "ui/XTPEnumControlCommboBox.h"

#include "base/logging.h"
#include "base/values.h"
#include "ui/command_updater.h"

// INFO the MFC no support IMPLEMENT_DYNCREATE for template class.
// so need a dump class to implement DYNCREATE.

template <typename EnumClass, typename EnumType=EnumClass::EnumType>
class EnumComboBox : public CXTPEnumControlCommboBox<EnumClass, EnumType>
{
public:
  EnumComboBox() {}
  EnumComboBox(CommandUpdater* command_updater, int id, 
      CXTPCommandBars* pCommandBars = NULL) 
    : CXTPEnumControlCommboBox(pCommandBars)
    , id_(id)
    , command_updater_(command_updater) {}

protected:
  virtual void Copy(CXTPControl* pControl, BOOL bRecursive) {
    // no Rumtime class no DYNAMIC_DOWNCAST
    // EnumComboBox* button = DYNAMIC_DOWNCAST(EnumComboBox, pControl);
    EnumComboBox* combobox = static_cast<EnumComboBox*>(pControl);
    DCHECK(combobox);
    CXTPEnumControlCommboBox::Copy(pControl, bRecursive);
    id_ = combobox->id_;
    command_updater_ = combobox->command_updater_;
  }

private:
  virtual void OnSelChanged() {
    CXTPEnumControlCommboBox::OnSelChanged();
    if (notify_) {
      EnumType select = GetSelect();
      scoped_ptr<base::Value> value(base::Value::CreateIntegerValue(select));
      command_updater_->ExecuteCommand(id_, *(value.get()));
    }
  }

  virtual void DoPropExchange(CXTPPropExchange* pPX) {
    NOTREACHED() << "No suppport Create for file.";
  }

private:
  int id_;
  CommandUpdater* command_updater_;
};

#define DECLARE_ENUM_COMMBOBOX(EnumClassType, ComboboxName) \
class ComboboxName : public EnumComboBox<EnumClassType> \
{ \
public: \
  ComboboxName() {} \
  ComboboxName(CommandUpdater* command_updater, int id, \
    CXTPCommandBars* pCommandBars = NULL) \
    : EnumComboBox(command_updater, id, pCommandBars) {} \
 \
private: \
  DECLARE_XTP_CONTROL(ComboboxName); \
};

#define IMPLEMENT_ENUM_COMMBOBOX(ComboboxName) \
  IMPLEMENT_XTP_CONTROL(ComboboxName, CXTPControlComboBox)
