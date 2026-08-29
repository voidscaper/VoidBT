/**
 * voidbt_utils.h
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Utility Header
 * =============================================================================
 */

#pragma once

#include <godot_cpp/variant/string.hpp>

namespace godot {
struct StringHasher {
    size_t operator()(const godot::String& str) const {
        godot::String copy = str;
        return static_cast<size_t>(copy.hash());
    }
};
}  // namespace godot
