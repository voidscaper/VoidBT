/**
 * voidbt_data.hpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Data abstract class
 * =============================================================================
 */

#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
class VoidBTData: public RefCounted {
    GDCLASS(VoidBTData, RefCounted)
 private:
 protected:
    static void _bind_methods() {}
 public:
    VoidBTData() {}
    virtual ~VoidBTData() = default;
    virtual bool process(godot::Variant) = 0;
};
}  // namespace godot
