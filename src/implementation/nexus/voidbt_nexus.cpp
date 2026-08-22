/**
 * voidbt_nexus.cpp
 * =============================================================================
 * Nexus for all interaction between the gdscript and the gdextension
 * =============================================================================
 */

 #include "voidbt_nexus.hpp"
// TODO: consider a common hpp that I can include in cpp files which are common for multiple classes.

 using namespace godot;

void VoidBTNexus::_bind_methods() {    
	UtilityFunctions::print("VoidBT Nexus bind methods");
}

 VoidBTNexus::VoidBTNexus() {
    UtilityFunctions::print("VoidBT Nexus constructor");
 }

 VoidBTNexus::~VoidBTNexus() {
    UtilityFunctions::print("VoidBT Nexus destructor");
 }

 bool VoidBTNexus::bootstrap_bt_tree() {
    UtilityFunctions::print("VoidBT Nexus bootstrap");
    return false;
 }

 int VoidBTNexus::tick(double delta) {
    UtilityFunctions::print("VoidBT Nexus tick");
    return 0;
}
