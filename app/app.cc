#include "stdafx.h"

#include "app/TcxUnitConverter.h"

// The one and only application object
CWinApp theApp;

using namespace std;

void ConvertValue(double value, TCHAR* src_unit, TCHAR* dst_unit);
void ConvertValue(double value, TCHAR* unit);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
  // initialize MFC and print and error on failure
  if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
    _tprintf(_T("Fatal Error: MFC initialization failed\n"));
    return 1;
  } 
  ConvertValue(10,_T("A"), _T("mA"));
  ConvertValue(10,_T("A"), _T("ma"));
  ConvertValue(10,_T("A"), _T("Ma"));
  ConvertValue(10,_T("A"), _T("MA"));

  ConvertValue(10,_T("V"), _T("mV"));
  ConvertValue(10,_T("V"), _T("uV"));
  // lost precision
  ConvertValue(10,_T("V"), _T("nV"));
  ConvertValue(1000000,_T("nV"), _T("V"));
  ConvertValue(10,_T("V"), _T("mv"));
  // lost precision
  ConvertValue(1,_T("MV"), _T("nV"));
  // lost precision
  ConvertValue(1,_T("GV"), _T("nV"));
  ConvertValue(10000000,_T("MV"));
  ConvertValue(10000000,_T("V"));
  ConvertValue(1003000009,_T("V"));
  ConvertValue(0.00000001,_T("V"));
  ConvertValue(0.00000001,_T("nV"));
  ConvertValue(0.00000001,_T("uV"));
  ConvertValue(-0.00000001,_T("V"));
  ConvertValue(0,_T("V"));

  return 0;
}

void ConvertValue(double value, TCHAR* src_unit, TCHAR* dst_unit) {
  double origin_value = value;
  BOOL ret = TcxConvertUnit(value, src_unit, dst_unit);
  if (ret)
    _tprintf(_T("%f%s = %f%s\n"), origin_value, src_unit, value, dst_unit);
  else
    _tprintf(_T("false convert\n"));
}

void ConvertValue(double value, TCHAR* unit) {
  double origin_value = value;
  string16 dst_unit(unit);
  BOOL ret = TcxConvertUnitForEasyRead(&value, &dst_unit);
  if (ret)
    _tprintf(_T("%f%s = %f%s\n"), origin_value, unit, value, dst_unit.c_str());
  else
    _tprintf(_T("false convert\n"));
}
