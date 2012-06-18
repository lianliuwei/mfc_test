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

QuantityEdit::QuantityEdit() 
    : CXTPValidatingEditCtrl(
      kQuantityFullMatch, kQuantityPartMatch, kQuantityInitTooltip) {
}

double QuantityEdit::value() {
  return static_cast<CXTPQuantityEdit*>(m_pControl)->value_;
}

string16 QuantityEdit::unit() {
  return static_cast<CXTPQuantityEdit*>(m_pControl)->unit_;
}

int QuantityEdit::id() {
  return static_cast<CXTPQuantityEdit*>(m_pControl)->id_;
}

CommandUpdater* QuantityEdit::command_updater() {
  return static_cast<CXTPQuantityEdit*>(m_pControl)->command_updater_;
}

void QuantityEdit::OnValidatedText(CString text)
{
  double input_value;
  string input_unit;
  bool ret = pcrecpp::RE(full_match()).FullMatch(
        UTF16ToASCII(text.GetString()), 
          &input_value, NULL_PTR, NULL_PTR, &input_unit);
  if (ret == false) {
    LOG(WARNING) <<  "Validated text is no validated: " << text.GetString();
    return;
  }
  if (!HasSetUnit()) {
    // update value and unit
    static_cast<CXTPQuantityEdit*>(m_pControl)->value_ = input_value;
    static_cast<CXTPQuantityEdit*>(m_pControl)->unit_ = ASCIIToUTF16(input_unit);
  } else {
    string16 unit16 = ASCIIToUTF16(input_unit);
    if (unit16 == EmptyString16()) // if no unit using default unit.
      unit16 = unit();
    BOOL ret = TcxConvertUnit(input_value, unit16.c_str(), unit().c_str());
    if (!ret) {
      LOG(ERROR) << "SematicCheck is fault. unit no match.";
      return;
    }
    static_cast<CXTPQuantityEdit*>(m_pControl)->value_ = input_value;
  }
  static_cast<CXTPQuantityEdit*>(m_pControl)->UpdateWindowText(value(), unit());
  // set to the commandupdater.
  scoped_ptr<base::DictionaryValue> quantity(new base::DictionaryValue);
  quantity->SetDouble(string(kValuePath), value());
  quantity->SetString(string(kUnitPath), unit());
  command_updater()->ExecuteCommand(id(), *(quantity.get()));
}

bool QuantityEdit::SemanticCheck(const CString &strText, 
                                 CString *pstrErrorMsg /*= 0*/ ) {
  string input_unit;
  bool ret = pcrecpp::RE(full_match()).FullMatch(
        UTF16ToASCII(strText.GetString()),
          NULL_PTR, NULL_PTR, NULL_PTR, &input_unit);
  if (ret == false) {
    LOG(ERROR) << "check the regex pattern, \
                  this must be true if pass syntax check.";
    return false;
  }
  if (HasSetUnit()) {
    if (input_unit == EmptyString()) return true; // using default unit.

    double dump = 0; // dump value
    string16 unit16 = ASCIIToUTF16(input_unit);
    // check if the same unit.
    return !!TcxConvertUnit(dump, unit16.c_str(), unit().c_str());
  } else {
    // if default is no set, user must type the unit.
    return input_unit != EmptyString();
  }
}

void CXTPQuantityEdit::UpdateWindowText(double value, string16 unit)
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
  // using the father set function, or cause many problem, 
  // has inner state to change.
  _SetEditText(show.c_str()); 
}
