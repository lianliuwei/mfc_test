// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file defines the type used to provide details for NotificationService
// notifications.
// 
// This file change from NotificationDetails

#ifndef COMMAND_PARAM_H_
#define COMMAND_PARAM_H_
#pragma once

#include "base/basictypes.h"

// Do not declare a NotificationDetails directly--use either
// "Param<detailsclassname>(detailsclasspointer)" or
// CommandUpdater::NoParams().
class CommandParam {
 public:
  CommandParam() : ptr_(NULL) {}
  CommandParam(const CommandParam& other) : ptr_(other.ptr_) {}
  ~CommandParam() {}

  // NotificationDetails can be used as the index for a map; this method
  // returns the pointer to the current details as an identifier, for use as a
  // map index.
  uintptr_t map_key() const { return reinterpret_cast<uintptr_t>(ptr_); }

  bool operator!=(const CommandParam& other) const {
    return ptr_ != other.ptr_;
  }

  bool operator==(const CommandParam& other) const {
    return ptr_ == other.ptr_;
  }

 protected:
   explicit CommandParam(const void* ptr) : ptr_(ptr) {}

  // Declaring this const allows Details<T> to be used with both T = Foo and
  // T = const Foo.
  const void* ptr_;
};

template <class T>
class Param : public CommandParam {
 public:
  // TODO(erg): Our code hard relies on implicit conversion
  Param(T* ptr) : CommandParam(ptr) {}  // NOLINT
  Param(const CommandParam& other)      // NOLINT
    : CommandParam(other) {}

  T* operator->() const { return ptr(); }
  // The casts here allow this to compile with both T = Foo and T = const Foo.
  T* ptr() const { return static_cast<T*>(const_cast<void*>(ptr_)); }
};

#endif  // COMMAND_PARAM_H_
