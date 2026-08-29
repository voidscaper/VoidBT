/**
 * voidbt_nexus_base.hpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Nexus base abstract class
 * =============================================================================
 */

#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
class VoidBTNexusBase : public RefCounted {
    GDCLASS(VoidBTNexusBase, RefCounted)
 private:
 protected:
    static void _bind_methods() {}
 public:
    VoidBTNexusBase() {}
    virtual ~VoidBTNexusBase() = default;
    virtual bool bootstrap_bt_tree() = 0;
};
}  // namespace godot
