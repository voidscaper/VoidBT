/**
 * voidbt_jsonparser.hpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Json parser header
 * =============================================================================
 */

#pragma once

#include "src/base/data/voidbt_data.hpp"

namespace godot {
class VoidBTJsonParser: public VoidBTData {
    GDCLASS(VoidBTJsonParser, VoidBTData)
 private:
 protected:
    static void _bind_methods() {}
 public:
    VoidBTJsonParser();
    ~VoidBTJsonParser();
    bool process(godot::Variant) override;
};
}  // namespace godot
