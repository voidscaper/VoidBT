/**
 * voidbt_node.h
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Base class for all VoidBT core classes.
 * =============================================================================
 */

#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
enum class VoidBTStatus {
         FAILURE,
         SUCCESS,
         RUNNING
};
class VoidBTNode: public RefCounted {
      GDCLASS(VoidBTNode, RefCounted)
 private:
 protected:
      static void _bind_methods() {}
 public:
      virtual ~VoidBTNode() = default;
      virtual VoidBTStatus tick(double delta) = 0;
};
}  // namespace godot
