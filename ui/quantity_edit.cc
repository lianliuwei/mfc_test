#include "StdAfx.h"

#include "quantity_edit.h"

#include "base/utf_string_conversions.h"

namespace {
static const char* kQuantityFullMatch = 
  "^([+-]?(\\d+|\\d+\\.\\d+)([eE][+-]?\\d+)?)\\s*(\\w*)$";
static const char* kQuantityPartMatch =
  "^[+-]?((\\d*)|(\\d+\\.)|((\\d+|\\d+\\.\\d+)[eE][+-]?)|((\\d+|\\d+\\.\\d+)([eE][+-]?\\d+)?\\s*([a-zA-Z]*)))$";
static const TCHAR* kQuantityInitTooltip =
  _T("type \"double( unit)\"");
}
// RE need null ptr, NULL is wrong.
#define NULL_PTR ((void*)(0))

QuantityEdit::QuantityEdit() 
    : CXTPValidatingEditCtrl(
      kQuantityFullMatch, kQuantityPartMatch, kQuantityInitTooltip)
    , value_(0.0) {
  
}

double QuantityEdit::value()
{
  return value_;
}

string16 QuantityEdit::unit()
{
  return unit_;
}

void QuantityEdit::OnValidatedText( CString text )
{
  double value;
  string unit;
  bool ret = pcrecpp::RE(full_match()).FullMatch(
        UTF16ToASCII(text.GetString()), &value, NULL_PTR, NULL_PTR, &unit);
  if (ret == false) {
    LOG(WARNING) <<  "Validated text is no validated: " << text.GetString();
    return;
  }
  // update value and unit
  value_ = value;
  unit_ = ASCIIToUTF16(unit);
}
