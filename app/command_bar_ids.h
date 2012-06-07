#pragma once

#ifdef DEFINE_COMMAND_ID_STRING
#define STR(tok) _T(#tok)
#else
#define STR(tok) tok
#endif

#define ID_BT_OK          STR(6)
#define ID_BT_START       STR(10)
#define ID_BT_STOP        STR(11)

extern const TCHAR* kCommandBarXML;