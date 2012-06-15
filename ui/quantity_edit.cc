#include "StdAfx.h"

#include "quantity_edit.h"

#include "base/utf_string_conversions.h"
#include "base/stringprintf.h"
#include "base/memory/scoped_ptr.h"
#include "base/values.h"
#include "ui/command_updater.h"
#include "ui/osc_command_ids.h"
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

const char kValuePath[] = "value";
const char kUnitPath[] = "unit";

// RE need null ptr, NULL is wrong.
#define NULL_PTR ((void*)(0))

QuantityEdit::QuantityEdit(CommandUpdater* command_updater, int id, string16 unit) 
    : CXTPValidatingEditCtrl(
      kQuantityFullMatch, kQuantityPartMatch, kQuantityInitTooltip)
    , value_(0.0)
    , command_updater_(command_updater)
    , id_(id) {
  set_unit(unit);
}

double QuantityEdit::value() {
  return value_;
}

string16 QuantityEdit::unit() {
  return unit_;
}

void QuantityEdit::UpdateWindowText(double value, string16 unit)
{
  string16 show;
  if (unit == EmptyString16()) {
    show = StringPrintf(kShowQuantity, value, unit.c_str());
  } else {
    if (!TcxConvertUnitForEasyRead(&value, &unit)) {
      LOG(INFO) << "value is too large or too small. can no be convert.";
      return; // left the user typein.
    } else {
      show = StringPrintf(kShowQuantity, value, unit.c_str());
    }
  }
  SetWindowText(show.c_str());
}


void QuantityEdit::OnValidatedText(CString text)
{
  double value;
  string unit;
  bool ret = pcrecpp::RE(full_match()).FullMatch(
        UTF16ToASCII(text.GetString()), &value, NULL_PTR, NULL_PTR, &unit);
  if (ret == false) {
    LOG(WARNING) <<  "Validated text is no validated: " << text.GetString();
    return;
  }
  if (!HasSetUnit()) {
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
  }
  UpdateWindowText(value_, unit_);
  // set to the commandupdater.
  scoped_ptr<base::DictionaryValue> quantity(new base::DictionaryValue);
  quantity->SetDouble(string(kValuePath), value_);
  quantity->SetString(string(kUnitPath), unit_);
  command_updater_->ExecuteCommand(id_, *(quantity.get()));
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
  if (HasSetUnit()) {
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
  unit_ = unit;
}

void QuantityEdit::set_value(double value) {
  value_ = value;
  UpdateWindowText(value_, unit_);
}