/**
 * voidbt_blackboard_base.hpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Blackboard base class
 * =============================================================================
 */

#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
class VoidBTBlackboardBase: public RefCounted {
    GDCLASS(VoidBTBlackboardBase, RefCounted)
 private:
 protected:
    static void _bind_methods() {}
    // virtual void on_value_changed(godot::String) = 0;
    // virtual void serialize() = 0;
    // virtual void deserialize() = 0;

 public:
    VoidBTBlackboardBase() {}
    virtual ~VoidBTBlackboardBase() = default;
    // CRUD
    virtual void set_value(godot::String, godot::Variant) = 0;
    virtual godot::Variant get_value(godot::String) = 0;
    virtual bool has_key(godot::String) = 0;
    virtual void remove_key(godot::String) = 0;
};
}  // namespace godot
