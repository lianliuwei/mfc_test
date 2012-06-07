#include "stdafx.h"

#include "app/command_bar_ids.h"

// The one and only application object
CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
  // initialize MFC and print and error on failure
  if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
    _tprintf(_T("Fatal Error: MFC initialization failed\n"));
    return 1;
  } 

  _tprintf(_T("%s"), kCommandBarXML);
  return 0;
}