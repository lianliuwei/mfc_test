// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/i18n/rtl.h"

#include "base/file_path.h"
#include "base/logging.h"
#include "base/string_util.h"
#include "base/utf_string_conversions.h"
#include "base/sys_string_conversions.h"
// #include "unicode/coll.h"
// #include "unicode/locid.h"
// #include "unicode/uchar.h"
// #include "unicode/uscript.h"

#if defined(TOOLKIT_USES_GTK)
#include <gtk/gtk.h>
#endif

#if defined(OS_WIN)
#include <Windows.h>
#endif

namespace {

// Extract language, country and variant, but ignore keywords.  For example,
// en-US, ca@valencia, ca-ES@valencia.
// std::string GetLocaleString(const icu::Locale& locale) {
//   const char* language = locale.getLanguage();
//   const char* country = locale.getCountry();
//   const char* variant = locale.getVariant();
// 
//   std::string result =
//       (language != NULL && *language != '\0') ? language : "und";
// 
//   if (country != NULL && *country != '\0') {
//     result += '-';
//     result += country;
//   }
// 
//   if (variant != NULL && *variant != '\0') {
//     std::string variant_str(variant);
//     StringToLowerASCII(&variant_str);
//     result += '@' + variant_str;
//   }
// 
//   return result;
// }
    // define some other languages until ntdefs.h catches up
#ifndef LANG_NAPALI
#define LANG_NAPALI     0x61
#endif
#ifndef LANG_BURMESE
#define LANG_BURMESE    0x55 // Burma
#endif
#ifndef LANG_YIDDISH
#define LANG_YIDDISH    0x3d
#endif

bool IsRTLLang(LANGID lang)
{
    switch(lang)
    {
    case LANG_ARABIC:
    case LANG_HEBREW:
    case LANG_URDU:
    case LANG_FARSI:
    case LANG_YIDDISH:
    case LANG_SINDHI:
    case LANG_KASHMIRI:
        return true;
    }

    return false;
}

bool IsRtlLCID(LCID lcid)
{
    return IsRTLLang(PRIMARYLANGID(LANGIDFROMLCID(lcid)));
}

#define IN_RANGE(n1, b, n2) ((unsigned)((b)-n1)<=n2-n1)

inline BOOL IsRTLCharCore(WCHAR ch)
{
    // Bitmask of RLM, RLE, RLO, based from RLM in the 0 bit.
#define MASK_RTLPUNCT   0x90000001

    return (IN_RANGE(0x0590, ch, 0x08ff) || // In RTL block
        (IN_RANGE(0x200f, ch, 0x202e) &&    // Possible RTL punct char
        ((MASK_RTLPUNCT>>(ch-0x200f))&1))); // Mask of RTL punct chars
}

inline BOOL IsRTLChar(WCHAR ch)
{
    return (IN_RANGE(0x0590/* First RTL char */, ch, 0x202e/* RLO */) &&
        IsRTLCharCore(ch));
}

}  // namespace

namespace base {
namespace i18n {

// Represents the locale-specific ICU text direction.
static TextDirection g_icu_text_direction = UNKNOWN_DIRECTION;

// Convert the ICU default locale to a string.
// std::string GetConfiguredLocale() {
//   return GetLocaleString(icu::Locale::getDefault());
// }

// Convert the ICU canonicalized locale to a string.
// std::string GetCanonicalLocale(const char* locale) {
//   return GetLocaleString(icu::Locale::createCanonical(locale));
// }

// Convert Chrome locale name to ICU locale name
// std::string ICULocaleName(const std::string& locale_string) {
//   // If not Spanish, just return it.
//   if (locale_string.substr(0, 2) != "es")
//     return locale_string;
//   // Expand es to es-ES.
//   if (LowerCaseEqualsASCII(locale_string, "es"))
//     return "es-ES";
//   // Map es-419 (Latin American Spanish) to es-FOO depending on the system
//   // locale.  If it's es-RR other than es-ES, map to es-RR. Otherwise, map
//   // to es-MX (the most populous in Spanish-speaking Latin America).
//   if (LowerCaseEqualsASCII(locale_string, "es-419")) {
//     const icu::Locale& locale = icu::Locale::getDefault();
//     std::string language = locale.getLanguage();
//     const char* country = locale.getCountry();
//     if (LowerCaseEqualsASCII(language, "es") &&
//       !LowerCaseEqualsASCII(country, "es")) {
//         language += '-';
//         language += country;
//         return language;
//     }
//     return "es-MX";
//   }
//   // Currently, Chrome has only "es" and "es-419", but later we may have
//   // more specific "es-RR".
//   return locale_string;
// }

// void SetICUDefaultLocale(const std::string& locale_string) {
//   icu::Locale locale(ICULocaleName(locale_string).c_str());
//   UErrorCode error_code = U_ZERO_ERROR;
//   icu::Locale::setDefault(locale, error_code);
//   // This return value is actually bogus because Locale object is
//   // an ID and setDefault seems to always succeed (regardless of the
//   // presence of actual locale data). However,
//   // it does not hurt to have it as a sanity check.
//   DCHECK(U_SUCCESS(error_code));
//   g_icu_text_direction = UNKNOWN_DIRECTION;
// 
//   // If we use Views toolkit on top of GtkWidget, then we need to keep
//   // GtkWidget's default text direction consistent with ICU's text direction.
//   // Because in this case ICU's text direction will be used instead.
//   // See IsRTL() function below.
// #if defined(TOOLKIT_USES_GTK) && !defined(TOOLKIT_GTK)
//   gtk_widget_set_default_direction(
//       ICUIsRTL() ? GTK_TEXT_DIR_RTL : GTK_TEXT_DIR_LTR);
// #endif
// }

// bool IsRTL() {
// #if defined(TOOLKIT_GTK)
//   GtkTextDirection gtk_dir = gtk_widget_get_default_direction();
//   return (gtk_dir == GTK_TEXT_DIR_RTL);
// #else
//   return ICUIsRTL();
// #endif
// }

bool IsRTL()
{
  return IsRtlLCID(GetThreadLocale());
}

// bool ICUIsRTL() {
//   if (g_icu_text_direction == UNKNOWN_DIRECTION) {
//     const icu::Locale& locale = icu::Locale::getDefault();
//     g_icu_text_direction = GetTextDirectionForLocale(locale.getName());
//   }
//   return g_icu_text_direction == RIGHT_TO_LEFT;
// }

// TextDirection GetTextDirectionForLocale(const char* locale_name) {
//   UErrorCode status = U_ZERO_ERROR;
//   ULayoutType layout_dir = uloc_getCharacterOrientation(locale_name, &status);
//   DCHECK(U_SUCCESS(status));
//   // Treat anything other than RTL as LTR.
//   return (layout_dir != ULOC_LAYOUT_RTL) ? LEFT_TO_RIGHT : RIGHT_TO_LEFT;
// }
// 
// TextDirection GetFirstStrongCharacterDirection(const string16& text) {
//   const UChar* string = text.c_str();
//   size_t length = text.length();
//   size_t position = 0;
//   while (position < length) {
//     UChar32 character;
//     size_t next_position = position;
//     U16_NEXT(string, next_position, length, character);
// 
//     // Now that we have the character, we use ICU in order to query for the
//     // appropriate Unicode BiDi character type.
//     int32_t property = u_getIntPropertyValue(character, UCHAR_BIDI_CLASS);
//     if ((property == U_RIGHT_TO_LEFT) ||
//         (property == U_RIGHT_TO_LEFT_ARABIC) ||
//         (property == U_RIGHT_TO_LEFT_EMBEDDING) ||
//         (property == U_RIGHT_TO_LEFT_OVERRIDE)) {
//       return RIGHT_TO_LEFT;
//     } else if ((property == U_LEFT_TO_RIGHT) ||
//                (property == U_LEFT_TO_RIGHT_EMBEDDING) ||
//                (property == U_LEFT_TO_RIGHT_OVERRIDE)) {
//       return LEFT_TO_RIGHT;
//     }
// 
//     position = next_position;
//   }
// 
//   return LEFT_TO_RIGHT;
// }

#if defined(OS_WIN)
bool AdjustStringForLocaleDirection(string16* text) {
  if (!IsRTL() || text->empty())
    return false;

  // Marking the string as LTR if the locale is RTL and the string does not
  // contain strong RTL characters. Otherwise, mark the string as RTL.
  bool has_rtl_chars = StringContainsStrongRTLChars(*text);
  if (!has_rtl_chars)
    WrapStringWithLTRFormatting(text);
  else
    WrapStringWithRTLFormatting(text);

  return true;
}
#else
bool AdjustStringForLocaleDirection(string16* text) {
  // On OS X & GTK the directionality of a label is determined by the first
  // strongly directional character.
  // However, we want to make sure that in an LTR-language-UI all strings are
  // left aligned and vice versa.
  // A problem can arise if we display a string which starts with user input.
  // User input may be of the opposite directionality to the UI. So the whole
  // string will be displayed in the opposite directionality, e.g. if we want to
  // display in an LTR UI [such as US English]:
  //
  // EMAN_NOISNETXE is now installed.
  //
  // Since EXTENSION_NAME begins with a strong RTL char, the label's
  // directionality will be set to RTL and the string will be displayed visually
  // as:
  //
  // .is now installed EMAN_NOISNETXE
  //
  // In order to solve this issue, we prepend an LRM to the string. An LRM is a
  // strongly directional LTR char.
  // We also append an LRM at the end, which ensures that we're in an LTR
  // context.

  // Unlike Windows, Linux and OS X can correctly display RTL glyphs out of the
  // box so there is no issue with displaying zero-width bidi control characters
  // on any system.  Thus no need for the !IsRTL() check here.
  if (text->empty())
    return false;

  bool ui_direction_is_rtl = IsRTL();

  bool has_rtl_chars = StringContainsStrongRTLChars(*text);
  if (!ui_direction_is_rtl && has_rtl_chars) {
    WrapStringWithRTLFormatting(text);
    text->insert(0U, 1U, kLeftToRightMark);
    text->push_back(kLeftToRightMark);
  } else if (ui_direction_is_rtl && has_rtl_chars) {
    WrapStringWithRTLFormatting(text);
    text->insert(0U, 1U, kRightToLeftMark);
    text->push_back(kRightToLeftMark);
  } else if (ui_direction_is_rtl) {
    WrapStringWithLTRFormatting(text);
    text->insert(0U, 1U, kRightToLeftMark);
    text->push_back(kRightToLeftMark);
  }

  return true;
}

#endif  // !OS_WIN

// bool StringContainsStrongRTLChars(const string16& text) {
//   const UChar* string = text.c_str();
//   size_t length = text.length();
//   size_t position = 0;
//   while (position < length) {
//     UChar32 character;
//     size_t next_position = position;
//     U16_NEXT(string, next_position, length, character);
// 
//     // Now that we have the character, we use ICU in order to query for the
//     // appropriate Unicode BiDi character type.
//     int32_t property = u_getIntPropertyValue(character, UCHAR_BIDI_CLASS);
//     if ((property == U_RIGHT_TO_LEFT) || (property == U_RIGHT_TO_LEFT_ARABIC))
//       return true;
// 
//     position = next_position;
//   }
// 
//   return false;
// }

bool StringContainsStrongRTLChars(const string16& text)
{
    size_t length = text.length();
    for(size_t i=0; i<length; ++i)
    {
        if(IsRTLChar(text[i]))
        {
            return true;
        }
    }
    return false;
}

void WrapStringWithLTRFormatting(string16* text) {
  if (text->empty())
    return;

  // Inserting an LRE (Left-To-Right Embedding) mark as the first character.
  text->insert(0U, 1U, kLeftToRightEmbeddingMark);

  // Inserting a PDF (Pop Directional Formatting) mark as the last character.
  text->push_back(kPopDirectionalFormatting);
}

void WrapStringWithRTLFormatting(string16* text) {
  if (text->empty())
    return;

  // Inserting an RLE (Right-To-Left Embedding) mark as the first character.
  text->insert(0U, 1U, kRightToLeftEmbeddingMark);

  // Inserting a PDF (Pop Directional Formatting) mark as the last character.
  text->push_back(kPopDirectionalFormatting);
}

void WrapPathWithLTRFormatting(const FilePath& path,
                               string16* rtl_safe_path) {
  // Wrap the overall path with LRE-PDF pair which essentialy marks the
  // string as a Left-To-Right string.
  // Inserting an LRE (Left-To-Right Embedding) mark as the first character.
  rtl_safe_path->push_back(kLeftToRightEmbeddingMark);
#if defined(OS_MACOSX)
    rtl_safe_path->append(UTF8ToUTF16(path.value()));
#elif defined(OS_WIN)
    rtl_safe_path->append(path.value());
#else  // defined(OS_POSIX) && !defined(OS_MACOSX)
    std::wstring wide_path = base::SysNativeMBToWide(path.value());
    rtl_safe_path->append(WideToUTF16(wide_path));
#endif
  // Inserting a PDF (Pop Directional Formatting) mark as the last character.
  rtl_safe_path->push_back(kPopDirectionalFormatting);
}

string16 GetDisplayStringInLTRDirectionality(const string16& text) {
  if (!IsRTL())
    return text;
  string16 text_mutable(text);
  WrapStringWithLTRFormatting(&text_mutable);
  return text_mutable;
}

const string16 StripWrappingBidiControlCharacters(const string16& text) {
  if (text.empty())
    return text;
  size_t begin_index = 0;
  char16 begin = text[begin_index];
  if (begin == kLeftToRightEmbeddingMark ||
      begin == kRightToLeftEmbeddingMark ||
      begin == kLeftToRightOverride ||
      begin == kRightToLeftOverride)
    ++begin_index;
  size_t end_index = text.length() - 1;
  if (text[end_index] == kPopDirectionalFormatting)
    --end_index;
  return text.substr(begin_index, end_index - begin_index + 1);
}

}  // namespace i18n
}  // namespace base
