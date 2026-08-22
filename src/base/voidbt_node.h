/**
 * voidbt_node.h
 * =============================================================================
 * Base class for all VoidBT classes.
 * =============================================================================
 */

#pragma once

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {

enum class VoidBTStatus {
	IDLE,
	RUNNING,
	STOPPED
};

class VoidBTNode: public RefCounted {
	GDCLASS(VoidBTNode, RefCounted);

	protected:
		static void _bind_methods() {};

	public:
		VoidBTNode();
		~VoidBTNode();

		virtual int tick(double delta) = 0;
};

} // namespace godot