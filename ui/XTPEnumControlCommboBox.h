#pragma once

#include <CommandBars/XTPControlComboBox.h>

#include "base/logging.h"
#include "base/auto_reset.h"

// there is no way to set current select without call the OnSelChanged().
// using state value method.
template <typename EnumClass, typename EnumType=EnumClass::EnumType>
class CXTPEnumControlCommboBox : public CXTPControlComboBox
{

public:
  CXTPEnumControlCommboBox(CXTPCommandBars* pCommandBars = NULL)     
      : CXTPControlComboBox(pCommandBars)
      , notify_(true) {
    bool ret = LoadResource<EnumClass>();
    CHECK(ret) << "can no load string resource.";
    EnumClass::Populate(*this);
  }
  virtual ~CXTPEnumControlCommboBox() {}

public:
  void SetSelect(EnumType select_enum, bool notify = true) {
    AutoReset<bool> auto_reset(&notify_, notify);
    bool ret = EnumClass::SetSel(*this, select_enum);
    DCHECK(ret) << "can not select " << select_enum;
  }
  EnumType GetSelect() {
    EnumType select;
    bool ret = EnumClass::GetCurSel(*this, select);
    DCHECK(ret) << "can no get current select";
    return select;
  }

  int InitStorage(int nItems, UINT nBytes) {
    return GetListBoxCtrl()->InitStorage(nItems, nBytes);
  }

protected:
  bool notify_;
};
