#ifndef TCXUNITCONVERTER_H
#define TCXUNITCONVERTER_H

#include <base/string16.h>

// Converts the <dValue> from the <pszSrcUnit> to the <pszDstUnit>
// RETURN:
//    - Nonzero if the conversion was successfully done
//    - Zero if the conversion can't be performed
//      (an illegal unit or incompatible units)
BOOL TcxConvertUnit(
   double& dValue,      // [in] value to convert, [out] converted value
   LPCTSTR pszSrcUnit,  // [in] source unit
   LPCTSTR pszDstUnit   // [in] destination unit
   );

// Counts how many of the first characters of the given string can be
// interpreted as a valid unit or unit expression.
// RETURN:
//    - The number of characters that can be interpreted as a valid
//      unit expression (the index of the first character that breaks
//      the unit). If the entire string can be interpreted as a single
//      unit, it return the length of the string.
int TcxSpanUnitStr(
   LPCTSTR pszIn        // [in] text to check
   );


// Returns the number of units in the internal table.
int TcxGetNumOfUnits();

// Given a unit table index (i.e. [0..TcxGetNumOfUnits()-1]),
// returns a pointer to a static string with the abbreviation
// of that unit.
LPCTSTR TcxGetUnitAbbreviation(
   int iUnit            // [in] unit index
   );

// Given a unit table index (i.e. [0..TcxGetNumOfUnits()-1]),
// returns a pointer to a static string with the unit tag.
LPCTSTR TcxGetUnitTag(
   int iUnit            // [in] unit index
   );

// Given a unit table index (i.e. [0..TcxGetNumOfUnits()-1]),
// returns TRUE if that unit is a basic unit.
BOOL TcxUnitIsBasic(
   int iUnit            // [in] unit index
   );

// Convert Unit to suitable precision, eg. 0.0001V to 1 mV, 1000V to kV
// value and unit may be change.
bool TcxConvertUnitForEasyRead(double* value, string16* unit);

#endif // TCXUNITCONVERTER_H