/*
 * voidbt_blackboard.cpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Blackboard implementation class
 * =============================================================================
 */

#include "src/implementation/blackboard/voidbt_blackboard.hpp"

using godot::VoidBTBlackboard, godot::String, godot::Variant;

/*
 * =============================================================================
 * Constructor
 * =============================================================================
 */
VoidBTBlackboard::VoidBTBlackboard() {}

/*
 * =============================================================================
 * Destructor
 * =============================================================================
 */
VoidBTBlackboard::~VoidBTBlackboard() {}

/*
 * =============================================================================
 * Set value
 * If key doesn't exist, m_data creates the {key, value} pari.
 * If key exists, m_data returns a reference to it.
 * m_data[key] cannot fail with [] operator.
 * =============================================================================
 */

void VoidBTBlackboard::set_value(String key, Variant value) {
    m_data[key] = value;
}

/*
 * =============================================================================
 * Get value
 * =============================================================================
 */

Variant VoidBTBlackboard::get_value(String key) {
    return m_data[key];
}

/*
 * =============================================================================
 * Has key
 * =============================================================================
 */

bool VoidBTBlackboard::has_key(String key) {
    return true;
}

/*
 * =============================================================================
 * Remove key
 * erase data associated with key
 * =============================================================================
 */

void VoidBTBlackboard::remove_key(String key) {
    m_data.erase(key);
}
