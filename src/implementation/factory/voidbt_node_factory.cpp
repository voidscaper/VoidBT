/**
 * voidbt_node_factory.cpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Node Factory class
 * =============================================================================
 */

#include "src/implementation/factory/voidbt_node_factory.hpp"
#include "src/implementation/core/voidbt_selector.hpp"

#include <memory>

using godot::VoidBTNodeFactory,
        godot::VoidBTNode,
        godot::VoidBTSelector,
        godot::String;

/*
 * =============================================================================
 * Constructor
 * =============================================================================
 */
VoidBTNodeFactory::VoidBTNodeFactory() {}

/*
 * =============================================================================
 * Destructor
 * =============================================================================
 */
VoidBTNodeFactory::~VoidBTNodeFactory() {}

/*
 * =============================================================================
 * Register Class
 * inert the key value pair of the class name and the function needed to create
 * an instance of the class
 * =============================================================================
 */
bool VoidBTNodeFactory::register_class(String class_name, Creator func) {
    m_register_map.insert_or_assign(class_name, func);
    return true;
}


/*
 * =============================================================================
 * Create
 * =============================================================================
 */
std::unique_ptr<VoidBTNode> VoidBTNodeFactory::create(String class_name) {
    return (std::make_unique<VoidBTSelector>());
}
