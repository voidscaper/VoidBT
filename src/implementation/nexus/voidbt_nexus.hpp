/**
 * voidbt_nexus.hpp
 * =============================================================================
 * Nexus for all interaction between the gdscript and the gdextension
 * =============================================================================
 */

 #pragma once

 #include "voidbt_node.h"

 namespace godot {
    class VoidBTNexus: public VoidBTNode {
        GDCLASS(VoidBTNexus, VoidBTNode)

        private:

        protected:
            static void _bind_methods();
            
        public:
            VoidBTNexus();
            ~VoidBTNexus();
            bool bootstrap_bt_tree();
            int tick(double) override;  
    };
 }