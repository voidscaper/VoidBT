/**
 * voidbt_selector.hpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Selector Class
 * =============================================================================
 */

#pragma once

#include "src/base/core/voidbt_node.hpp"

#include <vector>

namespace godot {
class VoidBTSelector: public VoidBTNode {
    GDCLASS(VoidBTSelector, VoidBTNode)
 private:
    std::vector<VoidBTNode*> m_nodes;
 protected:
    static void _bind_methods() {}
 public:
    VoidBTSelector();
    ~VoidBTSelector();
    VoidBTStatus tick(double) override;
};
}  // namespace godot
