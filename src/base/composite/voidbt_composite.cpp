/**
 * voidbt_composite.cpp
 * =============================================================================
 * 
 * =============================================================================
 */

#include "voidbt_composite.h"

using namespace godot;

void VoidBTComposite::_bind_methods() {    
	ClassDB::bind_method(D_METHOD("tick", "delta"), &VoidBTComposite::tick);
	//ClassDB::bind_method(D_METHOD("set_amplitude", "p_amplitude"), &VoidBTNode::set_amplitude);

	//ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "amplitude"), "set_amplitude", "get_amplitude");
}

VoidBTComposite::VoidBTComposite() {
    UtilityFunctions::print("VoidBT Composite class constructor");
}

VoidBTComposite::~VoidBTComposite() {
    UtilityFunctions::print("VoidBT Composite class destructor");
}

int VoidBTComposite::tick(double delta) {
    UtilityFunctions::print("VoidBT Composite class tick");
    return 0;
}