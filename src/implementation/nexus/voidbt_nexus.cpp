/**
* voidbt_nexus.cpp
* =============================================================================
* Copyright 2026 Joshua Jacob <voidscaper>
* Nexus for all interaction between the gdscript and the gdextension
* =============================================================================
*/

#include "src/implementation/nexus/voidbt_nexus.hpp"
// TODO(voidscaper): consider a common hpp that I can include in cpp files
// which are common for multiple classes.
// TODO(voidscaper): factory class that creates all bt related implementations
// TODO(voidscaper): the info class should inject context into the blackboard
// so that the factory can read it and create the implementations

using godot::VoidBTNexus;

/*
 * =============================================================================
 * Bind methods
 * =============================================================================
 */
void VoidBTNexus::_bind_methods() {
      UtilityFunctions::print("VoidBT Nexus bind methods");
      ClassDB::bind_method(D_METHOD("bootstrap_bt_tree"),
            &VoidBTNexus::bootstrap_bt_tree);
}

/*
 * =============================================================================
 * Constructor
 * =============================================================================
 */
VoidBTNexus::VoidBTNexus() {
      UtilityFunctions::print("VoidBT Nexus constructor");
}

/*
 * =============================================================================
 * Destructor
 * =============================================================================
 */
VoidBTNexus::~VoidBTNexus() {
      UtilityFunctions::print("VoidBT Nexus destructor");
}

/*
 * =============================================================================
 * Bootstrap
 * =============================================================================
 */
bool VoidBTNexus::bootstrap_bt_tree() {
      UtilityFunctions::print("VoidBT Nexus bootstrap");
      return true;
}

/*
 * =============================================================================
 * Constructor
 * =============================================================================
 */
// int VoidBTNexus::tick(double delta) {
//    UtilityFunctions::print("VoidBT Nexus tick");
//    return 0;
// }

/*
 * =============================================================================
 * Constructor
 * =============================================================================
 */
// bool VoidBTNexus::initialize_bt(VoidBTNode information) {

// }
