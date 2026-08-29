/**
 * voidbt_blackboard.hpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Blackboard implementation class header
 * =============================================================================
 */

#pragma once

#include "src/base/blackboard/voidbt_blackboard_base.hpp"
#include "src/base/core/voidbt_utils.hpp"

#include <unordered_map>

namespace godot {
class VoidBTBlackboard: public VoidBTBlackboardBase {
      GDCLASS(VoidBTBlackboard, VoidBTBlackboardBase)
 private:
      std::unordered_map<godot::String,
                         godot::Variant,
                         godot::StringHasher> m_data;
 protected:
      static void _bind_methods() {}
 public:
      VoidBTBlackboard();
      ~VoidBTBlackboard();
      void set_value(godot::String, godot::Variant) override;
      godot::Variant get_value(godot::String) override;
      bool has_key(godot::String) override;
      void remove_key(godot::String) override;
};
}  // namespace godot
