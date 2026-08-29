The core classes you need to inherit from and implement for traversal are the base task classes from your library, forming the foundation of every runnable node in your behavior tree . These classes define the lifecycle methods your `Nexus` will call during each traversal.

### 🧱 The Base Task Class Hierarchy

At the root is a base class (often called `BTTask`) that provides the common interface for all nodes . From this, four fundamental task types emerge:

| Class to Inherit From | Purpose | Example Logic in Your `Nexus` Traversal |
| :--- | :--- | :--- |
| **`BTAction`** | Performs an action or work (e.g., move, attack, play animation). | When traversal reaches this leaf, your `Nexus` calls its `_tick()` method to execute the action. |
| **`BTCondition`** | Checks a condition and returns `SUCCESS` or `FAILURE` (e.g., "Is target in range?"). | During traversal, the `Nexus` evaluates this leaf node by calling `_tick()` to check the condition from the blackboard. |
| **`BTComposite`** | Controls flow for its children (e.g., `Sequence`, `Selector`). | The `Nexus` calls its `_tick()` method, which internally determines which child to run and then calls that child's `_tick()`. |
| **`BTDecorator`** | Modifies the behavior/result of a single child (e.g., `Invert`, `Cooldown`). | The `Nexus` calls its `_tick()` method, which processes its child's result and returns a modified outcome. |

### 🔄 The Core Lifecycle Method: `_tick()`

Every node must implement a `_tick()` method. This is the primary method your `Nexus` calls during traversal to process the node's logic each frame. This method must return one of three statuses :

*   **`RUNNING`**: The task is still in progress and needs more frames to complete.
*   **`SUCCESS`**: The task completed its work successfully.
*   **`FAILURE`**: The task failed to complete.

For example, in your `Nexus` traversal loop, you would call `current_node->_tick(delta, blackboard)` to execute the logic and check the status.

### ⚠️ Essential Implementation Considerations

*   **Class Registration**: In GDExtension, both your base `Nexus` class and any inherited task classes must be properly registered with the engine . Use `GDREGISTER_CLASS` for normal classes and `GDREGISTER_ABSTRACT_CLASS` for abstract base classes.
*   **Method Signatures**: Ensure your overridden `_tick()` methods have the exact correct signature (including the `delta` parameter) or you'll encounter runtime errors that are difficult to debug .
*   **State Management**: Actions performing continuous behavior (like moving) should return `RUNNING` . Only return `SUCCESS` when the action is truly complete.
*   **Blackboard Access**: All nodes need a reference to the `Blackboard` to read and write context data. This is typically passed in when the task is initialized or ticked .

This hierarchy and the `_tick()` method are the backbone of any behavior tree system, forming the interface your `Nexus` will use to control the traversal.