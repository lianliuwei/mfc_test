#pragma once

#include "common/EnumBinder.h"

// help class for load the resource str. 
// CString has no resource id construction.
template <typename EnumName>
class ResID
{
public:
  INSERT_ENUM_BINDER_ITEMS(EnumName)
  UINT id;
};

#define ENUM_RES(EnumID, ResourceID) {EnumID, _T(#EnumID), _T(#EnumID), ResourceID}

#define BIND_ENUM_RES(originalEnumName, boundEnumClassName) \
  BIND_ENUM_CUSTOM(originalEnumName, boundEnumClassName, ResID<originalEnumName>)

template <typename EnumClass>
bool LoadResource() {
  for (int i = 0; i < EnumClass::GetSize(); i++) {
    UINT ID = EnumClass::GetAt(i).id;
    BOOL ret = EnumClass::ElementAt(i).m_stringUser.LoadString(ID);
    if (ret == FALSE) // can no load string.
      return false;
  }
  return true;
}