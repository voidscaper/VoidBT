/**
 * voidbt_action.hpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Action Class
 * =============================================================================
 */

#pragma once

#include "src/base/core/voidbt_node.hpp"

namespace godot {
class VoidBTAction: public VoidBTNode {
    GDCLASS(VoidBTAction, VoidBTNode)
 private:
 protected:
    static void _bind_methods() {}
 public:
    VoidBTAction();
    ~VoidBTAction();
    VoidBTStatus tick(double) override;
};
}  // namespace godot
