#include "StdAfx.h"

#include "quantity_edit.h"

#include "base/utf_string_conversions.h"
#include "base/stringprintf.h"
#include "common/TcxUnitConverter.h"

namespace {
static const char* kQuantityFullMatch = 
  "^([+-]?(\\d+|\\d+\\.\\d+)([eE][+-]?\\d+)?)\\s*(\\w*)$";
static const char* kQuantityPartMatch =
  "^[+-]?((\\d*)|(\\d+\\.)|((\\d+|\\d+\\.\\d+)[eE][+-]?)|((\\d+|\\d+\\.\\d+)([eE][+-]?\\d+)?\\s*([a-zA-Z]*)))$";
static const TCHAR* kQuantityInitTooltip =
  _T("type \"double( unit)\"");

static const TCHAR* kShowQuantity = _T("%g %ls");
}
// RE need null ptr, NULL is wrong.
#define NULL_PTR ((void*)(0))

QuantityEdit::QuantityEdit() 
    : CXTPValidatingEditCtrl(
      kQuantityFullMatch, kQuantityPartMatch, kQuantityInitTooltip)
    , value_(0.0)
    , set_unit_(false) {
  
}

double QuantityEdit::value() {
  return value_;
}

string16 QuantityEdit::unit() {
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
  if (!set_unit_) {
    // update value and unit
    value_ = value;
    unit_ = ASCIIToUTF16(unit);
  } else {
    string16 unit16 = ASCIIToUTF16(unit);
    if (unit16 == EmptyString16()) // if no unit using default unit.
      unit16 = unit_;
    BOOL ret = TcxConvertUnit(value, unit16.c_str(), unit_.c_str());
    if (!ret) {
      LOG(ERROR) << "SematicCheck is fault. unit no match.";
      return;
    }
    value_ = value;
    double value_show =  value_;
    string16 unit_show = unit_;
    if (!TcxConvertUnitForEasyRead(&value_show, &unit_show)) {
      LOG(ERROR) << "value is too large or too small. can no be convert.";
    } else {
      string16 show;
      show = StringPrintf(kShowQuantity, value_show, unit_show.c_str());
      SetWindowText(show.c_str());
    }
  }
  // set to the commandupdater.
}

bool QuantityEdit::SemanticCheck( const CString &strText, 
                                 CString *pstrErrorMsg /*= 0*/ ) {
  string unit;
  bool ret = pcrecpp::RE(full_match()).FullMatch(
        UTF16ToASCII(strText.GetString()), NULL_PTR, NULL_PTR, NULL_PTR, &unit);
  if (ret == false) {
    LOG(ERROR) << "check the regex pattern, \
                  this must be true if pass syntax check.";
    return false;
  }
  if (set_unit_) {
    if (unit == EmptyString()) return true; // using default unit.

    double value = 0; // dump value
    string16 unit16 = ASCIIToUTF16(unit);
    // check if the same unit.
    return !!TcxConvertUnit(value, unit16.c_str(), unit_.c_str());
  } else {
    // if default is no set, user must type the unit.
    return unit != EmptyString();
  }
}

void QuantityEdit::set_unit(string16 unit) {
  set_unit_ = (unit != EmptyString16());
  unit_ = unit;
}
