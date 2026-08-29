/**
 * register_types.cpp
 * =============================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Register Types
 * =============================================================================
 */

#include "src/register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "src/base/core/voidbt_node.hpp"
#include "src/base/blackboard/voidbt_blackboard_base.hpp"
#include "src/base/nexus/voidbt_nexus_base.hpp"
#include "src/base/factory/voidbt_factory.hpp"
#include "src/base/data/voidbt_data.hpp"
#include "src/implementation/nexus/voidbt_nexus.hpp"

using godot::ModuleInitializationLevel,
      godot::ClassDB,
      godot::VoidBTNode,
      godot::VoidBTBlackboardBase,
      godot::VoidBTNexusBase,
      godot::VoidBTFactory,
      godot::VoidBTData,
      godot::VoidBTNexus;

void initialize_voidbt_module(ModuleInitializationLevel p_level) {
    if (p_level !=
        ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    // Register abstract classes
    GDREGISTER_ABSTRACT_CLASS(VoidBTNode);
    GDREGISTER_ABSTRACT_CLASS(VoidBTBlackboardBase);
    GDREGISTER_ABSTRACT_CLASS(VoidBTNexusBase);
    GDREGISTER_ABSTRACT_CLASS(VoidBTFactory);
    GDREGISTER_ABSTRACT_CLASS(VoidBTData);

    // Register Nexus
    GDREGISTER_CLASS(VoidBTNexus);
}

void uninitialize_voidbt_module(ModuleInitializationLevel p_level) {
    if (p_level !=
        ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT voidbt_library_init(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    const GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address,
        p_library,
        r_initialization);

    init_obj.register_initializer(initialize_voidbt_module);
    init_obj.register_terminator(uninitialize_voidbt_module);
    init_obj.set_minimum_library_initialization_level(
        ModuleInitializationLevel::MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}
