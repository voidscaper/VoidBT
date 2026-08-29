/**
 * voidbt_nexus.hpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Nexus for all interactions between godot and the VoidBT gdextension
 * =============================================================================
 */

#pragma once

#include "src/base/nexus/voidbt_nexus_base.hpp"

namespace godot {
class VoidBTNexus: public VoidBTNexusBase {
    GDCLASS(VoidBTNexus, VoidBTNexusBase)
 private:
 protected:
    static void _bind_methods();
 public:
    VoidBTNexus();
    ~VoidBTNexus();
    bool bootstrap_bt_tree() override;
    // int tick(double) override;
    // bool initialize_bt(VoidBTNode);
};
}  // namespace godot
