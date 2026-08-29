/**
 * register_types.hpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Register Types
 * =============================================================================
 */

#pragma once

#include <godot_cpp/core/class_db.hpp>

using godot::ModuleInitializationLevel;

void initialize_voidbt_module(ModuleInitializationLevel p_level);
void uninitialize_voidbt_module(ModuleInitializationLevel p_level);
