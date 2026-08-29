/**
 * voidbt_factory.hpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Factory base abstract class
 * =============================================================================
 */

#pragma once

#include "src/base/core/voidbt_node.hpp"

#include <memory>

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
class VoidBTFactory: public RefCounted {
     GDCLASS(VoidBTFactory, RefCounted)
 private:
 protected:
     static void _bind_methods() {}
 public:
     VoidBTFactory() {}
     virtual ~VoidBTFactory() = default;
     virtual std::unique_ptr<VoidBTNode> create(godot::String type) = 0;
};
}  // namespace godot
