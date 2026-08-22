/**
 * voidbt_composite.h
 * =============================================================================
 * Base class for all VoidBT composite classes.
 * =============================================================================
 */

#pragma once

#include "../voidbt_node.h"
using namespace godot;

namespace godot {
    class VoidBTComposite: public VoidBTNode {
        GDCLASS(VoidBTComposite, VoidBTNode);

        private:

        protected:
            static void _bind_methods();

        public:
            VoidBTComposite();
            ~VoidBTComposite();

            int tick(double) override;
    };
}