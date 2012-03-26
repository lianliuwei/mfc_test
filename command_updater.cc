// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "command_updater.h"

#include <algorithm>

#include "base/logging.h"
#include "base/observer_list.h"
#include "base/stl_util.h"

CommandUpdater::CommandUpdaterDelegate::~CommandUpdaterDelegate() {
}

class CommandUpdater::Command {
 public:
  bool enabled;
  scoped_ptr<base::Value> value;

  ObserverList<CommandObserver> observers;

  Command() : enabled(true) {
    value.reset(base::Value::CreateNullValue());
  }
};

CommandUpdater::CommandUpdater(CommandUpdaterDelegate* handler)
    : delegate_(handler) {
      null_value_.reset(base::Value::CreateNullValue());
}

CommandUpdater::~CommandUpdater() {
  STLDeleteContainerPairSecondPointers(commands_.begin(), commands_.end());
}

bool CommandUpdater::IsCommandEnabled(int id) const {
  const CommandMap::const_iterator command(commands_.find(id));
  if (command == commands_.end())
    return false;
  return command->second->enabled;
}


base::Value* CommandUpdater::GetCommandParam( int id ) const {
  const CommandMap::const_iterator command(commands_.find(id));
  if (command == commands_.end())
    return null_value_.get();
  return (command->second->value).get();
}

bool CommandUpdater::SupportsCommand(int id) const {
  return commands_.find(id) != commands_.end();
}

void CommandUpdater::ExecuteCommand(int id, const base::Value& param) {
  if (IsCommandEnabled(id))
    delegate_->ExecuteCommand(id, param);
}

CommandUpdater::CommandObserver::~CommandObserver() {
}

void CommandUpdater::UpdateCommandEnabled(int id, bool enabled) {
  Command* command = GetCommand(id, true);
  if (command->enabled == enabled)
    return;  // Nothing to do.
  command->enabled = enabled;
  FOR_EACH_OBSERVER(CommandObserver, command->observers,
                    EnabledStateChangedForCommand(id, enabled));
}

void CommandUpdater::UpdateCommandParam( int id, const base::Value& param ) {
    Command* command = GetCommand(id, true);
    if (base::Value::Equals(&param, (command->value).get()))
      return;  // Nothing to do.

    command->value.reset(param.DeepCopy());
    FOR_EACH_OBSERVER(CommandObserver, command->observers,
      ParamChangedForCommand(id, param));
}

CommandUpdater::Command* CommandUpdater::GetCommand(int id, bool create) {
  bool supported = SupportsCommand(id);
  if (supported)
    return commands_[id];
  DCHECK(create);
  Command* command = new Command;
  commands_[id] = command;
  return command;
}

void CommandUpdater::AddCommandObserver(int id, CommandObserver* observer) {
  GetCommand(id, true)->observers.AddObserver(observer);
}

void CommandUpdater::RemoveCommandObserver(int id, CommandObserver* observer) {
  GetCommand(id, false)->observers.RemoveObserver(observer);
}

void CommandUpdater::RemoveCommandObserver(CommandObserver* observer) {
  for (CommandMap::const_iterator it = commands_.begin();
       it != commands_.end();
       ++it) {
    Command* command = it->second;
    if (command)
      command->observers.RemoveObserver(observer);
  }
}