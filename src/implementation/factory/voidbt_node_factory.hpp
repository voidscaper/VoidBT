/**
 * voidbt_node_factory.hpp
 * ===============================================================================================================================================
 * Copyright 2026 Joshua Jacob <voidscaper>
 * Node Factory class
 * (1) The class is based on the "Register Factory" design pattern.
 * (2) A macro is defined that is used by VoidBTNode implementation classes to register their class names and the logic required to instantiate them.
 * (3) All the data required to instantiate the classes will be available in the blackboard.
 * ===============================================================================================================================================
 */

#pragma once

#include "src/base/factory/voidbt_factory.hpp"
#include "src/base/core/voidbt_node.hpp"
#include "src/base/core/voidbt_utils.hpp"

#include <functional>
#include <memory>
#include <unordered_map>

namespace godot {
class VoidBTNodeFactory: public VoidBTFactory {
    GDCLASS(VoidBTNodeFactory, VoidBTFactory)
 private:
      /* Creator is the alias for a function that returns
      a unique ptr to a VoidBTNode class*/
      using Creator = std::function<std::unique_ptr<VoidBTNode>()>;
      std::unordered_map<godot::String, Creator,
                         godot::StringHasher> m_register_map;
 protected:
      static void _bind_methods() {}
 public:
      VoidBTNodeFactory();
      ~VoidBTNodeFactory();

      bool register_class(godot::String, Creator);
      std::unique_ptr<VoidBTNode> create(godot::String type) override;
};

// Macro used to register a VoidBTNode implementation class
#define REGISTER_VOIDBT_NODE_CLASS(class_name, func) \
static inline bool _registered_##class_name = \
VoidBTFactory::register_class(class_name, func);
}  // namespace godot
