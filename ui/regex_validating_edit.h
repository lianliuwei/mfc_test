#pragma once

#include "pcre/files/pcrecpp.h"
#include "base/string_util.h"

#include "validating_edit.h"

// NOTE: Be very careful how you specify your regular expressions.
// For strictly formatted data, you will want a rigid regex
// and rely on automatic formatting in a derived class to make
// data entry easier (see the CDateEdit example to see this approach
// in action). For easier validations (say unsigned int) a much looser
// regex can be used without problems.

// using the pcre regex lib to check the input.
template <class TBaseEdit>
class RegexValidatingEdit : public ValidatingEdit<TBaseEdit>
{
public:
  // match nothing.
  RegexValidatingEdit() : full_match_("^$"), part_match_("^$") {}
  RegexValidatingEdit(string full_match, string part_match)
      : full_match_(full_match)
      , part_match_(part_match) {}

  virtual ~RegexValidatingEdit() {};

  // NOTE: the match string change no re check, 
  // because normal people init the class and no change.
  void set_full_match(string full_match) {
    full_match_ = full_match;
  }
  string full_match() const {
    return full_match_;
  }
  
  void set_part_match(string part_match) {
    part_match_ = part_match;
  }
  string part_match() const {
    return part_match_;
  }

private:
  // TODO make RE object member, 
  // the regex lib compile the partten so match faster.
  // now only support ASCII
  virtual bool SyntaxCheck (const CString &strText) {
    std::wstring w_str(strText.GetString());
    if (!IsStringASCII(w_str))
      return false; 
    string str = WideToASCII(w_str);
    // using default option
    return pcrecpp::RE(full_match_).FullMatch(str);
  }
  virtual bool MarchSoFar (const CString &strText) {
     std::wstring w_str(strText.GetString());
    if (!IsStringASCII(w_str))
      return false; 
    string str = WideToASCII(w_str);
    return pcrecpp::RE(part_match_).FullMatch(str);
  }

private:
  string full_match_;
  string part_match_;
};
