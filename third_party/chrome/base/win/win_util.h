// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// =============================================================================
// PLEASE READ
//
// In general, you should not be adding stuff to this file.
//
// - If your thing is only used in one place, just put it in a reasonable
//   location in or near that one place. It's nice you want people to be able
//   to re-use your function, but realistically, if it hasn't been necessary
//   before after so many years of development, it's probably not going to be
//   used in other places in the future unless you know of them now.
//
// - If your thing is used by multiple callers and is UI-related, it should
//   probably be in app/win/ instead. Try to put it in the most specific file
//   possible (avoiding the *_util files when practical).
//
// =============================================================================

#ifndef BASE_WIN_WIN_UTIL_H_
#define BASE_WIN_WIN_UTIL_H_
#pragma once

#include <windows.h>

#include <string>

#include "base/base_export.h"
#include "base/string16.h"

struct IPropertyStore;
struct _tagpropertykey;
typedef _tagpropertykey PROPERTYKEY;

namespace base {
namespace win {

// A Windows message reflected from other windows. This message is sent
// with the following arguments:
// hWnd - Target window
// uMsg - kReflectedMessage
// wParam - Should be 0
// lParam - Pointer to MSG struct containing the original message.
const int kReflectedMessage = WM_APP + 3;

BASE_EXPORT void GetNonClientMetrics(NONCLIENTMETRICS* metrics);

// Returns the string representing the current user sid.
BASE_EXPORT bool GetUserSidString(std::wstring* user_sid);

// Returns true if the shift key is currently pressed.
BASE_EXPORT bool IsShiftPressed();

// Returns true if the ctrl key is currently pressed.
BASE_EXPORT bool IsCtrlPressed();

// Returns true if the alt key is currently pressed.
BASE_EXPORT bool IsAltPressed();

// Returns false if user account control (UAC) has been disabled with the
// EnableLUA registry flag. Returns true if user account control is enabled.
// NOTE: The EnableLUA registry flag, which is ignored on Windows XP
// machines, might still exist and be set to 0 (UAC disabled), in which case
// this function will return false. You should therefore check this flag only
// if the OS is Vista or later.
BASE_EXPORT bool UserAccountControlIsEnabled();

// Adds the specified |command| using the specified |name| to the AutoRun key.
// |root_key| could be HKCU or HKLM or the root of any user hive.
BASE_EXPORT bool AddCommandToAutoRun(HKEY root_key, const string16& name,
                                     const string16& command);
// Removes the command specified by |name| from the AutoRun key. |root_key|
// could be HKCU or HKLM or the root of any user hive.
BASE_EXPORT bool RemoveCommandFromAutoRun(HKEY root_key, const string16& name);

// Reads the command specified by |name| from the AutoRun key. |root_key|
// could be HKCU or HKLM or the root of any user hive. Used for unit-tests.
BASE_EXPORT bool ReadCommandFromAutoRun(HKEY root_key,
                                        const string16& name,
                                        string16* command);

}  // namespace win
}  // namespace base

#endif  // BASE_WIN_WIN_UTIL_H_
