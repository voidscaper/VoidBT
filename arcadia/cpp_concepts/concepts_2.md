**Excellent request!** You're asking for a **living document** that evolves with you as you code. This checklist isn't just a list of features—it's a **decision matrix** that tells you *which* C++20 feature to use for *which* architectural problem.

Let me build you a comprehensive, categorized checklist that you can refer to while building your BT library.

---

## 🏗️ MASTER C++20 CHECKLIST FOR BT LIBRARY

---

### SECTION 1: TYPE SYSTEM & SAFETY
*Use these when defining interfaces, contracts, and compile-time guarantees*

| Scenario | C++20 Feature | Why | Example in BT Context |
| :--- | :--- | :--- | :--- |
| **Defining node type requirements** | **Concepts** (`<concepts>`) | Enforce compile-time constraints on template parameters with clear error messages | `template<typename T> concept Node = requires(T t) { { t.tick() } -> std::same_as<Status>; };` |
| **Constraining Blackboard value types** | **Concepts** | Prevent storing invalid types in Blackboard | `template<typename T> concept BlackboardValue = std::is_trivially_copyable_v<T> || std::is_same_v<T, std::string>;` |
| **Building type-safe node factories** | **`std::enable_if`** + Concepts | Conditional template instantiation for different node types | `template<Node T> std::unique_ptr<T> createNode();` |
| **Storing unknown types in Blackboard** | **`std::any`** (`<any>`) | Type-safe type erasure without virtual inheritance | `std::unordered_map<std::string, std::any> m_data;` |
| **Storing variant node states** | **`std::variant`** (`<variant>`) | Type-safe union for status returns | `std::variant<Success, Failure, Running, Idle> tick();` |
| **Optional Blackboard values** | **`std::optional`** (`<optional>`) | Explicit "value or no value" semantics | `std::optional<T> getValue(const std::string& key);` |
| **Avoiding dynamic_cast in type checks** | **`dynamic_cast`** → **`std::visit`** | Replace runtime type checking with compile-time dispatch | Use `std::visit` on `std::variant<ActionNode, ConditionNode, ControlNode>` |

**Code Example for Concepts in BT:**

```cpp
// ============ BT Concepts ============
#include <concepts>
#include <type_traits>

namespace bt {

// 1. Base concept for ALL nodes
template<typename T>
concept TreeNode = requires(T t, double delta) {
    { t.tick(delta) } -> std::same_as<Status>;
    { t.halt() } -> std::same_as<void>;
};

// 2. Concept for composite nodes (have children)
template<typename T>
concept CompositeNode = TreeNode<T> && requires(T t) {
    { t.addChild(std::declval<std::unique_ptr<TreeNode>>()) } -> std::same_as<void>;
    { t.getChildren() } -> std::same_as<std::span<const std::unique_ptr<TreeNode>>>;
};

// 3. Concept for decorator nodes (have one child)
template<typename T>
concept DecoratorNode = TreeNode<T> && requires(T t) {
    { t.setChild(std::declval<std::unique_ptr<TreeNode>>()) } -> std::same_as<void>;
    { t.getChild() } -> std::same_as<const TreeNode&>;
};

// 4. Concept for Blackboard values (non-raw pointer types)
template<typename T>
concept BlackboardValue = std::is_trivially_copyable_v<T> || 
                          std::is_same_v<T, std::string> ||
                          std::is_same_v<T, std::vector<int>> ||
                          std::is_base_of_v<Object, T>;

// 5. Usage: Constrain a factory method
template<TreeNode T>
std::unique_ptr<T> createNode(const std::string& name) {
    return std::make_unique<T>(name);
}

} // namespace bt
```

---

### SECTION 2: MEMORY & OWNERSHIP
*Use these when managing node lifecycles, tree ownership, and resource sharing*

| Scenario | C++20 Feature | Why | Example in BT Context |
| :--- | :--- | :--- | :--- |
| **Owning tree nodes** | **`std::unique_ptr`** | Exclusive ownership, RAII, zero overhead | Root node holds children as `std::unique_ptr<TreeNode>` |
| **Shared Blackboard references** | **`std::shared_ptr`** | Multiple nodes share the same Blackboard | `std::shared_ptr<Blackboard> m_blackboard;` |
| **Non-owning view of children** | **`std::span`** (`<span>`) | Safe, non-owning view of contiguous children | `std::span<std::unique_ptr<TreeNode>> getChildren();` |
| **Weak references to parents** | **`std::weak_ptr`** | Avoid circular references in tree | Parent holds child (unique_ptr), child holds parent (weak_ptr) |
| **Allocating many small nodes** | **Custom Memory Pool** + `std::unique_ptr` | Reduce heap fragmentation | Pool allocator for `TreeNode` objects |
| **Clone nodes for tree copying** | **Copy constructor** + **`std::unique_ptr` cloning** | Deep copy semantics | `virtual std::unique_ptr<TreeNode> clone() const = 0;` |
| **Aliasing constructor for shared_ptr** | **`std::shared_ptr` aliasing** | Share ownership without sharing pointer | Blackboard owned by parent, aliased to child |
| **Avoiding `new`/`delete`** | **`std::make_unique`**, **`std::make_shared`** | Exception safety, readability | `auto node = std::make_unique<SequenceNode>();` |

**Code Example for Memory Patterns:**

```cpp
// ============ Memory & Ownership ============
#include <memory>
#include <span>
#include <vector>

namespace bt {

class TreeNode {
public:
    virtual ~TreeNode() = default;
    
    // Clone pattern for deep copying trees
    virtual std::unique_ptr<TreeNode> clone() const = 0;
    
    // Lifecycle methods
    virtual Status tick(double delta) = 0;
    virtual void halt() = 0;
    
    // Parent management
    void setParent(std::weak_ptr<TreeNode> parent) { m_parent = parent; }
    std::weak_ptr<TreeNode> getParent() const { return m_parent; }
    
protected:
    std::weak_ptr<TreeNode> m_parent;
};

class CompositeNode : public TreeNode {
public:
    // Using span for safe non-owning view
    std::span<const std::unique_ptr<TreeNode>> getChildren() const {
        return { m_children.data(), m_children.size() };
    }
    
    void addChild(std::unique_ptr<TreeNode> child) {
        child->setParent(shared_from_this());  // Enable weak_ptr tracking
        m_children.push_back(std::move(child));
    }
    
    // Memory pooling - custom allocator
    static void* operator new(size_t size);
    static void operator delete(void* ptr);
    
private:
    std::vector<std::unique_ptr<TreeNode>> m_children;  // Exclusive ownership
};

// Pool allocator implementation (simplified)
class TreeNodePool {
    std::vector<std::byte> m_memory;
    size_t m_next_free = 0;
public:
    void* allocate(size_t size) {
        // ... pool allocation logic
    }
    void deallocate(void* ptr) {
        // ... return to pool
    }
};

} // namespace bt
```

---

### SECTION 3: CONCURRENCY & ASYNCHRONY
*Use these when nodes run in parallel, need to be halted, or perform async operations*

| Scenario | C++20 Feature | Why | Example in BT Context |
| :--- | :--- | :--- | :--- |
| **Asynchronous node execution** | **Coroutines** (`<coroutine>`) | Write async code that looks synchronous | `AsyncAction::tick()` co_awaits a task |
| **Support for `co_await` in nodes** | **`std::coroutine_handle`** | Custom awaitable for BT status | `co_await MoveToTarget(target);` returns `RUNNING` until complete |
| **Halting long-running coroutines** | **`std::stop_token`** (`<stop_token>`) | Cooperative cancellation | Pass stop token to async actions, check in loops |
| **Parallel node execution** | **`std::jthread`** (`<thread>`) | Auto-joining threads | Parallel node spawns jthreads for children |
| **Thread-safe Blackboard** | **`std::mutex`** + **`std::lock_guard`** | Protect shared data | `std::scoped_lock lock(m_mutex);` in getter/setter |
| **Performance-critical no-op locking** | **`std::atomic`** (`<atomic>`) | Lock-free operations for simple types | `std::atomic<int> m_refCount;` |
| **Waiting for completion without busy-wait** | **`std::condition_variable`** | Efficient blocking | Coroutine waits until navigation completes |
| **Avoiding `std::thread` manual join** | **`std::jthread`** | RAII thread joining | Child thread automatically joins when node destructs |

**Code Example for Concurrency:**

```cpp
// ============ Concurrency & Asynchrony ============
#include <coroutine>
#include <stop_token>
#include <thread>
#include <atomic>
#include <mutex>

namespace bt {

// 1. Coroutine-based async action
struct AsyncAction : public ActionNode {
    struct promise_type {
        AsyncAction get_return_object() {
            return AsyncAction{ std::coroutine_handle<promise_type>::from_promise(*this) };
        }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_value(Status status) { m_status = status; }
        void unhandled_exception() { m_status = Status::FAILURE; }
        
        Status m_status = Status::RUNNING;
        std::stop_source m_stop_source;
    };
    
    std::coroutine_handle<promise_type> m_handle;
    
    // Coroutine that checks stop_token periodically
    Status tick(double delta) override {
        if (m_handle.promise().m_stop_source.stop_requested()) {
            return Status::FAILURE;
        }
        // ... resume coroutine
        return m_handle.promise().m_status;
    }
    
    void halt() override {
        m_handle.promise().m_stop_source.request_stop();
    }
};

// 2. Parallel node using jthread
class ParallelNode : public CompositeNode {
    Status tick(double delta) override {
        std::vector<std::jthread> threads;
        threads.reserve(getChildren().size());
        
        std::atomic<int> success_count{0};
        std::mutex result_mutex;
        
        for (auto& child : getChildren()) {
            threads.emplace_back([&child, &success_count, &result_mutex]() {
                auto status = child->tick(0.016);  // Simplified
                if (status == Status::SUCCESS) {
                    success_count++;
                }
                // ... handle results
            });
        }
        
        // Threads auto-join in destructor due to jthread
        return success_count == getChildren().size() ? Status::SUCCESS : Status::FAILURE;
    }
};

// 3. Thread-safe Blackboard
class ThreadSafeBlackboard {
    std::unordered_map<std::string, std::any> m_data;
    mutable std::shared_mutex m_mutex;  // C++17, but C++20 supports
    
public:
    template<BlackboardValue T>
    void setValue(const std::string& key, T value) {
        std::unique_lock lock(m_mutex);
        m_data[key] = value;
    }
    
    template<BlackboardValue T>
    std::optional<T> getValue(const std::string& key) const {
        std::shared_lock lock(m_mutex);  // C++17, C++20 refines
        auto it = m_data.find(key);
        if (it != m_data.end()) {
            return std::any_cast<T>(it->second);
        }
        return std::nullopt;
    }
};

} // namespace bt
```

---

### SECTION 4: INTERFACES & POLYMORPHISM
*Use these when defining node contracts, runtime dispatch, and extensibility*

| Scenario | C++20 Feature | Why | Example in BT Context |
| :--- | :--- | :--- | :--- |
| **Runtime polymorphism** | **Virtual functions** | Classic polymorphic behavior | `virtual Status tick(double) = 0;` |
| **Compile-time polymorphism** | **CRTP** (Curiously Recurring Template Pattern) | Static dispatch, zero overhead | `template<typename Derived> class NodeBase;` |
| **Behavior injection without subclassing** | **`std::function`** (`<functional>`) | Strategy pattern for actions | `ActionNode` stores `std::function<Status(double)> m_func;` |
| **Extensible blackboard interfaces** | **Concepts + `std::any`** | Type-safe but extensible | Blackboard stores any BlackboardValue type |
| **Node introspection** | **`std::type_info`** | Runtime type identification | `typeid(*node).name()` for debugging |
| **Formatting debug output** | **`std::format`** (`<format>`) | Type-safe, fast string formatting | `std::format("Node: {} status: {}", name, status);` |
| **Strongly typed node IDs** | **`enum class`** + **`std::variant`** | Type-safe node identifiers | `using NodeID = std::variant<SequenceType, ActionType, ConditionType>;` |
| **Safely downcasting nodes** | **`std::dynamic_pointer_cast`** | Safe runtime downcasting | `auto seq = std::dynamic_pointer_cast<SequenceNode>(node);` |

**Code Example for Polymorphism:**

```cpp
// ============ Interfaces & Polymorphism ============
#include <functional>
#include <format>
#include <variant>

namespace bt {

// 1. CRTP for static polymorphism (zero overhead)
template<typename Derived>
class NodeBase {
public:
    Status tick(double delta) {
        return static_cast<Derived*>(this)->onTick(delta);
    }
    
    void halt() {
        static_cast<Derived*>(this)->onHalt();
    }
};

class SequenceNode : public NodeBase<SequenceNode> {
public:
    Status onTick(double delta) {
        // Sequence logic
        return Status::SUCCESS;
    }
    void onHalt() {
        // Cleanup
    }
};

// 2. Strategy pattern using std::function
class ActionNode : public TreeNode {
    std::function<Status(double)> m_action;
    std::function<void()> m_cleanup;
    
public:
    ActionNode(std::function<Status(double)> action) 
        : m_action(std::move(action)) {}
    
    Status tick(double delta) override {
        return m_action(delta);
    }
    
    void halt() override {
        if (m_cleanup) m_cleanup();
    }
};

// 3. Type-safe node ID with std::variant
using NodeID = std::variant<
    std::monostate,  // Empty
    int,             // Index
    std::string,     // Name
    uintptr_t        // Pointer
>;

class NodeRegistry {
    std::unordered_map<NodeID, std::unique_ptr<TreeNode>> m_nodes;
    
public:
    template<TreeNode T>
    void registerNode(NodeID id, std::unique_ptr<T> node) {
        m_nodes[std::move(id)] = std::move(node);
    }
};

// 4. Modern debug formatting
class DebugFormatter {
public:
    static std::string formatStatus(Status status) {
        return std::format("Status: {}", static_cast<int>(status));
    }
    
    static std::string formatTree(const TreeNode& node, int indent = 0) {
        return std::format("{:>{}}Node: {}", "", indent, typeid(node).name());
        // Note: This is a simplified example; C++ doesn't have reflection yet
    }
};

} // namespace bt
```

---

### SECTION 5: FACTORY & REGISTRATION PATTERNS
*Use these when creating nodes dynamically, deserializing from files, or building trees at runtime*

| Scenario | C++20 Feature | Why | Example in BT Context |
| :--- | :--- | :--- | :--- |
| **Dynamic node creation** | **`std::function`** + **`std::unordered_map`** | Factory pattern without inheritance | `using NodeBuilder = std::function<std::unique_ptr<TreeNode>()>;` |
| **Type-erased node factories** | **`std::any`** + **`std::variant`** | Handle multiple return types | `std::unordered_map<std::string, NodeBuilder> m_factories;` |
| **Building trees from files** | **`std::filesystem`** (`<filesystem>`) | Cross-platform file handling | Recursively parse JSON/XML tree definitions |
| **String-to-type mapping** | **`std::map`** + **`std::type_index`** | Dynamic type lookup | `m_map["Sequence"] = std::type_index(typeid(SequenceNode));` |
| **Singleton factory pattern** | **Meyers Singleton** | Thread-safe lazy initialization | `static Factory& getInstance();` |
| **Node configuration from JSON** | **`std::string_view`** | Efficient string handling | Parse node attributes without copying strings |
| **Decorator chaining in registration** | **Variadic templates** | Type-safe decorator combinations | `registerNode<RetryDecorator<SequenceNode>>();` |
| **Injecting dependencies** | **`std::shared_ptr`** + **Constructor injection** | Clean dependency management | Pass Blackboard to node constructor |

**Code Example for Factory Pattern:**

```cpp
// ============ Factory & Registration ============
#include <filesystem>
#include <functional>
#include <unordered_map>
#include <type_traits>
#include <string_view>

namespace bt {

// 1. Node Factory with type-safe registration
class NodeFactory {
    using NodeBuilder = std::function<std::unique_ptr<TreeNode>(std::shared_ptr<Blackboard>)>;
    std::unordered_map<std::string, NodeBuilder> m_builders;
    
public:
    template<TreeNode T, typename... Args>
    void registerNode(const std::string& name) {
        m_builders[name] = [](std::shared_ptr<Blackboard> bb) {
            return std::make_unique<T>(std::move(bb));
        };
    }
    
    // Variadic template for decorators
    template<DecoratorNode T, TreeNode ChildT, typename... Args>
    void registerDecorator(const std::string& name) {
        m_builders[name] = [](std::shared_ptr<Blackboard> bb) {
            auto child = std::make_unique<ChildT>(bb);
            return std::make_unique<T>(std::move(child), bb);
        };
    }
    
    std::unique_ptr<TreeNode> createNode(const std::string& name, 
                                        std::shared_ptr<Blackboard> bb) {
        auto it = m_builders.find(name);
        if (it != m_builders.end()) {
            return it->second(std::move(bb));
        }
        return nullptr;
    }
};

// 2. Tree builder using std::filesystem
class TreeBuilder {
    NodeFactory& m_factory;
    
public:
    std::unique_ptr<TreeNode> loadFromFile(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error("File not found");
        }
        
        // Parse JSON/XML manually or with a library
        auto json = parseJSON(path);
        return buildFromJSON(json);
    }
    
private:
    std::unique_ptr<TreeNode> buildFromJSON(const json& data) {
        auto type = data["type"].get<std::string>();
        auto bb = std::make_shared<Blackboard>();
        
        // Use std::string_view for efficient parsing
        std::string_view typeView = type;
        
        // ... recursive building
        return m_factory.createNode(type, bb);
    }
};

// 3. Meyers Singleton for factory
class GlobalNodeFactory {
    NodeFactory m_factory;
    
    GlobalNodeFactory() = default;  // Private constructor
    
public:
    static GlobalNodeFactory& getInstance() {
        static GlobalNodeFactory instance;  // Thread-safe in C++11+
        return instance;
    }
    
    NodeFactory& get() { return m_factory; }
};

// 4. Registration macro using C++20 features
#define REGISTER_NODE(TYPE, NAME) \
    static bool register_##TYPE = []() { \
        GlobalNodeFactory::getInstance().get().registerNode<TYPE>(NAME); \
        return true; \
    }();  // Static initialization trick

} // namespace bt
```

---

### SECTION 6: ERROR HANDLING & EXCEPTIONS
*Use these when nodes fail, trees corrupt, or invalid states occur*

| Scenario | C++20 Feature | Why | Example in BT Context |
| :--- | :--- | :--- | :--- |
| **Reporting fatal errors** | **Exceptions** (`<exception>`) | Unrecoverable errors | Blackboard missing required key |
| **Graceful degradation** | **`std::optional`** | Expected failures | `std::optional<Status> getStatus();` |
| **Error codes for performance** | **`enum class`** + **`std::error_code`** | Zero-overhead error reporting | `std::error_code` for tree validation |
| **Assertions for debugging** | **`static_assert`** | Compile-time invariants | `static_assert(TreeNode<SequenceNode>, "SequenceNode must be a TreeNode");` |
| **Runtime invariants** | **`assert`** (from `<cassert>`) | Debugging help | `assert(m_children.size() > 0 && "Sequence must have children");` |
| **Safe array access** | **`std::array::at()`** | Bounds-checked access | `children.at(index)` instead of `children[index]` |
| **Avoiding exceptions in game loops** | **`noexcept`** | Performance guarantee | Mark simple getters as `noexcept` |
| **Recording failure context** | **`std::source_location`** (`<source_location>`) | Rich error context | `logError("Node failed", std::source_location::current());` |

**Code Example for Error Handling:**

```cpp
// ============ Error Handling ============
#include <exception>
#include <source_location>
#include <string_view>
#include <system_error>

namespace bt {

// 1. Custom exception for BT errors
class BTError : public std::runtime_error {
    std::source_location m_location;
public:
    BTError(const std::string& msg, 
            const std::source_location& loc = std::source_location::current())
        : std::runtime_error(msg), m_location(loc) {}
    
    void log() const {
        std::println("BT Error: {} at {}:{}", 
                     what(), 
                     m_location.file_name(), 
                     m_location.line());
    }
};

// 2. Error codes for common failures
enum class BTCode {
    Success = 0,
    NodeNotFound,
    InvalidTree,
    BlackboardKeyNotFound,
    Timeout
};

class BTCategory : public std::error_category {
public:
    const char* name() const noexcept override { return "BehaviorTree"; }
    std::string message(int ev) const override {
        switch (static_cast<BTCode>(ev)) {
            case BTCode::Success: return "Success";
            case BTCode::NodeNotFound: return "Node not found";
            case BTCode::InvalidTree: return "Invalid tree structure";
            default: return "Unknown error";
        }
    }
};

std::error_code make_error_code(BTCode code) {
    static const BTCategory category;
    return { static_cast<int>(code), category };
}

// 3. Node validation with source_location
class NodeValidator {
public:
    static bool validateNode(const TreeNode& node, 
                            const std::source_location& loc = std::source_location::current()) {
        if (typeid(node) == typeid(SequenceNode) && !hasChildren(node)) {
            std::println("Validation failed at {}:{}", loc.file_name(), loc.line());
            return false;
        }
        return true;
    }
    
private:
    static bool hasChildren(const TreeNode& node) {
        // ... check children
        return true;
    }
};

// 4. Safe Blackboard access with static_assert
template<BlackboardValue T>
class SafeBlackboard {
    std::unordered_map<std::string, std::any> m_data;
    
public:
    T getValue(const std::string& key) const {
        auto it = m_data.find(key);
        if (it == m_data.end()) {
            throw BTError(std::format("Key '{}' not found", key));
        }
        try {
            return std::any_cast<T>(it->second);
        } catch (const std::bad_any_cast&) {
            throw BTError(std::format("Type mismatch for key '{}'", key));
        }
    }
    
    std::optional<T> tryGetValue(const std::string& key) const noexcept {
        auto it = m_data.find(key);
        if (it == m_data.end()) {
            return std::nullopt;
        }
        try {
            return std::any_cast<T>(it->second);
        } catch (...) {
            return std::nullopt;
        }
    }
};

} // namespace bt
```

---

### SECTION 7: MODERN C++23 FEATURES
*Use these when C++23 support is mature enough in your environment*

| Scenario | C++23 Feature | Why | Example in BT Context |
| :--- | :--- | :--- | :--- |
| **Deducing this in virtual methods** | **`deducing this`** | Simplify CRTP and mixins | `template<typename Self> void tick(this Self&& self);` |
| **std::bind_back for callbacks** | **`std::bind_back`** | Easier partial function application | `auto action = std::bind_back(&ActionNode::tick, 0.016);` |
| **std::expected for error handling** | **`std::expected`** (`<expected>`) | Better than exceptions or optional | `std::expected<Status, Error> tick();` |
| **std::print/std::println** | **`std::print`** (`<print>`) | Cleaner debug output | `std::println("Status: {}", status);` |
| **Multidimensional operator[]** | **`operator[]` with multiple args** | Cleaner array access | `data[1, 2, 3]` for 3D data structures |

**Note:** These are listed as "future-ready" rather than essential. The C++20 checklist above is your core.

---

### SECTION 8: DESIGN PATTERN MAPPING

Here's how SOLID principles and GoF patterns map to your BT architecture with C++20 features:

| Pattern/SOLID | C++20 Implementation | BT Context |
| :--- | :--- | :--- |
| **Single Responsibility** | Classes with focused methods | `ActionNode` does work; `SequenceNode` manages flow |
| **Open/Closed** | `std::function` + `virtual` | Add new decorators without modifying existing nodes |
| **Liskov Substitution** | Concepts + inheritance | Any `TreeNode` can be used where a `TreeNode` is expected |
| **Interface Segregation** | Multiple small concepts | `TreeNode`, `CompositeNode`, `DecoratorNode` are separate concepts |
| **Dependency Inversion** | Constructor injection + `shared_ptr` | Nodes depend on `Blackboard` abstraction, not concrete |
| **Factory Method** | `std::function` builders + `unordered_map` | Dynamic node creation from string names |
| **Decorator** | Chain of `unique_ptr<TreeNode>` | `RetryDecorator<ActionNode>` wraps `ActionNode` |
| **Strategy** | `std::function` in `ActionNode` | Inject behavior into a leaf node |
| **Observer** | `std::function` callbacks | Node status changes notify listeners |
| **Composite** | `CompositeNode` with `children` vector | Tree structure composed of nodes |
| **Builder** | `TreeBuilder` with `std::filesystem` | Build tree from JSON/XML |
| **Singleton** | Meyers Singleton | `GlobalNodeFactory::getInstance()` (use sparingly!) |

---

## 📋 THE EXECUTABLE CHECKLIST

Print this and keep it at your desk:

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    C++20 BT LIBRARY CHECKLIST                          │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│  ☐ When creating node base classes...                                 │
│    ☐ Use std::unique_ptr for child ownership                         │
│    ☐ Use std::weak_ptr for parent back-references                    │
│    ☐ Mark tick() as virtual or use CRTP for static dispatch          │
│    ☐ Use Concepts to constrain template parameters                    │
│                                                                        │
│  ☐ When implementing Blackboard...                                   │
│    ☐ Use std::any for type-erased storage                            │
│    ☐ Use std::optional for missing keys                              │
│    ☐ Use Concepts to constrain allowed types                         │
│    ☐ Use std::shared_ptr for shared Blackboard ownership             │
│                                                                        │
│  ☐ When creating Action nodes...                                     │
│    ☐ Consider std::function for strategy injection                   │
│    ☐ Consider coroutines for async actions                           │
│    ☐ Use std::stop_token for cancellation support                    │
│                                                                        │
│  ☐ When building factories...                                       │
│    ☐ Use std::unordered_map<std::string, std::function>             │
│    ☐ Use Meyers Singleton (if absolutely necessary)                  │
│    ☐ Use std::filesystem for file loading                            │
│    ☐ Use std::string_view for efficient parsing                      │
│                                                                        │
│  ☐ When handling errors...                                          │
│    ☐ Use std::optional for expected failures                         │
│    ☐ Use exceptions for fatal errors                                │
│    ☐ Use std::source_location for error context                     │
│    ☐ Use static_assert for compile-time validation                  │
│                                                                        │
│  ☐ When adding concurrency...                                       │
│    ☐ Use std::jthread for auto-joining threads                      │
│    ☐ Use std::atomic for lock-free counters                          │
│    ☐ Use std::mutex + std::lock_guard for thread-safe Blackboard    │
│    ☐ Use coroutines for async operations (C++20)                    │
│                                                                        │
│  ☐ When formatting output...                                        │
│    ☐ Use std::format for type-safe formatting                       │
│    ☐ Use std::println for quick debugging (C++23)                   │
│                                                                        │
│  ☐ When testing...                                                  │
│    ☐ Use Concepts for compile-time tests                            │
│    ☐ Use static_assert for compile-time invariants                  │
│    ☐ Use std::optional for test result handling                     │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## 🚀 YOUR PATH FORWARD

| Phase | Focus | C++20 Features to Master |
| :--- | :--- | :--- |
| **1. Foundation** | TreeNode, Blackboard | `std::any`, `std::unique_ptr`, `std::optional`, Concepts |
| **2. Building Blocks** | Action, Condition, Sequence | `std::function`, CRTP, `std::variant` |
| **3. Advanced Nodes** | Decorators, Parallel | Coroutines, `std::stop_token`, `std::jthread` |
| **4. Factory & IO** | Tree builders, serialization | `std::filesystem`, `std::string_view`, `std::format` |
| **5. Polishing** | Error handling, debugging | `std::source_location`, `std::expected` (C++23) |

---

**TL;DR:** Every time you hit an architectural decision, go back to this checklist. It's your compass:

1. **Need runtime polymorphism?** → `virtual` + CRTP
2. **Need compile-time safety?** → Concepts + `static_assert`
3. **Need dynamic creation?** → `std::function` factory
4. **Need state?** → Blackboard with `std::any`
5. **Need async?** → Coroutines + `std::stop_token`
6. **Need ownership?** → `std::unique_ptr` + `std::shared_ptr`
7. **Need error handling?** → `std::optional` + exceptions
8. **Need performance?** → `noexcept` + `std::span` + memory pools

Build in iterations. Start with Phase 1, get a working prototype, then enhance with each phase. When you show me your code, I'll help you see which parts of the checklist apply!