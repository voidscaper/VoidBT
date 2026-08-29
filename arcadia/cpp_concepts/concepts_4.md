## 🏗️ The Master Checklist: SOLID, Loose Coupling, Exceptions & Over-Engineering

This is your **living document** for making design decisions. Every item has a **"Why"** and a **"When to Ignore"** so you know if it's a best practice or over-engineering.

---

## SECTION 1: SOLID PRINCIPLES (The Foundation)

### S - Single Responsibility Principle

> **"A class should have one, and only one, reason to change."**

| Check | Best Practice | Why | Over-Engineering? |
| :--- | :--- | :--- | :--- |
| ☐ | Does this class have ONE clear purpose? | Easier to test, debug, and modify | ❌ NOT over-engineering. Always apply. |
| ☐ | Can I describe what this class does in ONE sentence? | If you need "and", it's doing too much | ❌ NOT over-engineering. Always apply. |
| ☐ | Are I/O, business logic, and UI separated? | Changes in one don't break others | ⚠️ For small projects, some mixing is OK |
| ☐ | Does this class have < 10 public methods? | More methods = more responsibilities | ⚠️ 10 is a guideline, not a hard rule |

**Example in BT Context:**
```cpp
// ❌ BAD: One class does everything
class BTEngine {
    void loadTree();      // File I/O
    void tickTree();      // Logic
    void logStatus();     // Logging
    void renderDebug();   // UI
};

// ✅ GOOD: Separation of concerns
class TreeLoader { void load(); };
class TreeRunner { void tick(); };
class Logger { void log(); };
class DebugRenderer { void render(); };
```

**When to Ignore:** Tiny utility classes (like a simple math helper) don't need splitting.

---

### O - Open/Closed Principle

> **"Open for extension, closed for modification."**

| Check | Best Practice | Why | Over-Engineering? |
| :--- | :--- | :--- | :--- |
| ☐ | Can I add new behavior without modifying existing code? | Prevents bugs in working code | ❌ NOT over-engineering. Core OOP principle. |
| ☐ | Do I use inheritance, composition, or interfaces for extensibility? | Polymorphism allows extension | ❌ NOT over-engineering. Standard practice. |
| ☐ | Are my core classes `final` or sealed? | Prevents misuse | ⚠️ Only if you know the class won't be extended |
| ☐ | Do I use `virtual` methods for extension points? | Allows derived classes to override | ❌ NOT over-engineering. Required for inheritance. |

**Example in BT Context:**
```cpp
// ✅ GOOD: Open for extension via virtual methods
class TreeNode {
public:
    virtual Status tick(double delta) = 0;  // Extension point
    virtual void halt() { /* default */ }    // Can be overridden
};

// ✅ GOOD: Add new node type without modifying base
class CustomNode : public TreeNode {
    Status tick(double delta) override {
        // Custom logic here
    }
};

// ❌ BAD: Modifying core class to add new type
class TreeNode {
    enum Type { SEQUENCE, ACTION, CUSTOM };  // Adding this requires modification
};
```

**When to Ignore:** Simple, non-extensible utilities (like a math library) don't need this.

---

### L - Liskov Substitution Principle

> **"Derived classes must be substitutable for their base classes."**

| Check | Best Practice | Why | Over-Engineering? |
| :--- | :--- | :--- | :--- |
| ☐ | Does the derived class preserve the base class's behavior? | Violations cause subtle bugs | ❌ NOT over-engineering. Critical for polymorphism. |
| ☐ | Does the derived class strengthen preconditions? | Should NOT require more than base | ❌ NOT over-engineering. Fundamental OOP. |
| ☐ | Does the derived class weaken postconditions? | Should NOT promise less than base | ❌ NOT over-engineering. Fundamental OOP. |
| ☐ | Would replacing base with derived break anything? | If yes, you violated LSP | ❌ NOT over-engineering. Must be correct. |

**Example in BT Context:**
```cpp
// ✅ GOOD: Square IS-A Rectangle? NO! (Famous violation)
class Rectangle {
    virtual void setWidth(int w) { m_w = w; }
    virtual void setHeight(int h) { m_h = h; }
};

class Square : public Rectangle {  // ❌ Violates LSP!
    void setWidth(int w) override { m_w = m_h = w; }  // Changes behavior
    void setHeight(int h) override { m_h = m_w = h; }  // Changes behavior
};

// Usage that breaks:
void process(Rectangle& r) {
    r.setWidth(5);
    r.setHeight(10);
    // Expected: area = 50
    // Actual if Square: area = 100 (WRONG!)
}
```

**When to Ignore:** Never ignore LSP for polymorphic hierarchies. If you can't follow LSP, use composition instead.

---

### I - Interface Segregation Principle

> **"Clients should not be forced to depend on interfaces they don't use."**

| Check | Best Practice | Why | Over-Engineering? |
| :--- | :--- | :--- | :--- |
| ☐ | Are my interfaces focused and minimal? | Reduces coupling, easier to implement | ❌ NOT over-engineering. Clean design. |
| ☐ | Are there any methods that some implementations don't need? | Split the interface | ❌ NOT over-engineering. Reduces unused code. |
| ☐ | Do I use pure virtual classes as interfaces? | C++ way of defining contracts | ❌ NOT over-engineering. Standard C++. |
| ☐ | Are my interfaces smaller than ~5 methods? | More methods = more dependencies | ⚠️ 5 is a guideline, not a hard rule |

**Example in BT Context:**
```cpp
// ❌ BAD: Fat interface
class ITreeNode {
    virtual Status tick(double) = 0;
    virtual void addChild(TreeNode*) = 0;  // Not all nodes have children
    virtual void halt() = 0;
    virtual void setParent(TreeNode*) = 0;  // Not all nodes need parent
};

// ✅ GOOD: Segregated interfaces
class ITickable {
    virtual Status tick(double) = 0;
};

class ICompositeNode : public ITickable {
    virtual void addChild(TreeNode*) = 0;
};

class IDecoratorNode : public ITickable {
    virtual void setChild(TreeNode*) = 0;
};

// ActionNode only implements ITickable (doesn't need child management)
```

**When to Ignore:** Very small systems where interfaces would add unnecessary complexity.

---

### D - Dependency Inversion Principle

> **"Depend on abstractions, not concretions."**

| Check | Best Practice | Why | Over-Engineering? |
| :--- | :--- | :--- | :--- |
| ☐ | Do I use interfaces/abstract classes for dependencies? | Reduces coupling, easier to mock | ❌ NOT over-engineering. Core principle. |
| ☐ | Do I inject dependencies (not create them inside)? | Makes testing easier | ❌ NOT over-engineering. Testability is critical. |
| ☐ | Are my dependencies passed via constructor? | Clear ownership, explicit needs | ❌ NOT over-engineering. Best practice. |
| ☐ | Is my code testable with mocks? | Low coupling = high testability | ❌ NOT over-engineering. Always aim for testable. |

**Example in BT Context:**
```cpp
// ❌ BAD: Depends on concrete implementation
class BehaviorTree {
    FileLogger m_logger;  // Hard dependency
    void loadTree() {
        m_logger.log("Loading tree...");  // Tight coupling
    }
};

// ✅ GOOD: Depends on abstraction
class ILogger {
    virtual void log(const std::string& msg) = 0;
};

class BehaviorTree {
    std::unique_ptr<ILogger> m_logger;  // Dependency injected
    
public:
    BehaviorTree(std::unique_ptr<ILogger> logger)  // Constructor injection
        : m_logger(std::move(logger)) {}
    
    void loadTree() {
        m_logger->log("Loading tree...");  // Loose coupling
    }
};

// Now we can inject FileLogger, ConsoleLogger, NullLogger, MockLogger...
```

**When to Ignore:** Utility classes that will never need to be swapped (like a simple math helper).

---

## SECTION 2: LOOSE COUPLING

| Check | Best Practice | Why | Over-Engineering? |
| :--- | :--- | :--- | :--- |
| ☐ | Do I use forward declarations instead of includes in headers? | Reduces compile-time dependencies | ❌ NOT over-engineering. Speeds compilation. |
| ☐ | Do I use interfaces (abstract classes) for communication? | Reduces direct dependencies | ❌ NOT over-engineering. Improves testability. |
| ☐ | Do I use event/observer patterns instead of direct calls? | Systems don't know about each other | ⚠️ **CAN be over-engineering for simple cases** |
| ☐ | Is my code modular (separate libraries/DLLs)? | Enables parallel development | ⚠️ **CAN be over-engineering for small projects** |
| ☐ | Do I use `std::function`/callbacks for behavior injection? | Strategy pattern without inheritance | ⚠️ **CAN be over-engineering if simple inheritance works** |

**Example: Event System (When to Use vs Over-Engineering)**

```cpp
// ✅ GOOD: Direct call (simple, no over-engineering)
void onHit() {
    health -= 10;  // Simple, clear
}

// ⚠️ OK: Event system (necessary for complex systems)
// Use when: Multiple systems need to react to the same event
void onHit() {
    EventBus::emit("HealthChanged", health - 10);
    // UI, Sound, AI, Network all react without knowing about each other
}

// ❌ OVER-ENGINEERING: Event system for one subscriber
void onHit() {
    // Only one system cares!
    EventBus::emit("HealthChanged", health - 10);
    // Why not just call health.update()?
}
```

**When to Use Events:**
- > 3 systems react to the same event
- Systems shouldn't know about each other (decoupling)
- Future expansion expected

**When to Avoid Events:**
- Only 1-2 subscribers
- Systems already know about each other
- Performance-critical path (events add overhead)

---

## SECTION 3: EXCEPTION HANDLING

| Check | Best Practice | Why | Over-Engineering? |
| :--- | :--- | :--- | :--- |
| ☐ | Do I use exceptions for EXCEPTIONAL cases only? | Exceptions are expensive | ❌ NOT over-engineering. Critical for performance. |
| ☐ | Do I catch exceptions by const reference? | Prevents slicing, avoids copies | ❌ NOT over-engineering. Standard practice. |
| ☐ | Do I use RAII for resource cleanup? | Guarantees cleanup even with exceptions | ❌ NOT over-engineering. Essential C++. |
| ☐ | Do I avoid throwing exceptions in destructors? | Destructors can't propagate exceptions | ❌ NOT over-engineering. Critical safety rule. |
| ☐ | Do I use `noexcept` where appropriate? | Enables compiler optimizations | ⚠️ Only for functions that truly never throw |
| ☐ | Do I have a global exception handler? | Prevents crashes in production | ❌ NOT over-engineering. Essential for production. |
| ☐ | Do I use `std::optional` instead of exceptions for expected failures? | Better than exceptions for common cases | ❌ NOT over-engineering. Modern C++ best practice. |
| ☐ | Do I avoid using exceptions for control flow? | Exceptions are for errors, not logic | ❌ NOT over-engineering. Critical design rule. |

**Exception Handling Decision Tree:**

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    WHEN TO THROW AN EXCEPTION                         │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│  Is this a NORMAL flow of execution?                                   │
│      │                                                                 │
│      ├── YES → DON'T use exceptions                                   │
│      │         Use:                                                   │
│      │         • std::optional for missing values                    │
│      │         • std::expected (C++23) for expected errors          │
│      │         • Error codes for performance-critical paths          │
│      │                                                               │
│      └── NO → Is this a RARE, UNRECOVERABLE error?                  │
│                │                                                      │
│                ├── YES → THROW the exception                         │
│                │         • Out of memory                              │
│                │         • File not found (critical)                 │
│                │         • Invalid state (shouldn't happen)          │
│                │                                                      │
│                └── NO → Is this a RECOVERABLE error?                │
│                          │                                            │
│                          ├── YES → Return std::optional             │
│                          │         • Key not found in blackboard    │
│                          │         • File parse failed (skip)       │
│                          │         • Node not found                 │
│                          │                                            │
│                          └── NO → Use error code                    │
│                                    • Performance-critical paths      │
│                                    • Library boundary (C API)       │
└─────────────────────────────────────────────────────────────────────────┘
```

**Example in BT Context:**

```cpp
// ============ BLACKBOARD: Use std::optional for expected failures ============
class Blackboard {
    std::unordered_map<std::string, std::any> m_data;
    
public:
    template<typename T>
    std::optional<T> getValue(const std::string& key) const {
        auto it = m_data.find(key);
        if (it == m_data.end()) {
            return std::nullopt;  // Expected case - not an error!
        }
        try {
            return std::any_cast<T>(it->second);
        } catch (const std::bad_any_cast&) {
            return std::nullopt;  // Type mismatch - expected case!
        }
    }
};

// Usage:
auto health = blackboard.getValue<int>("health");
if (health) {
    // Key exists, use it
} else {
    // Key doesn't exist - this is NORMAL, not exceptional!
    health = 100;  // Default value
}

// ============ TREE LOADING: Use exceptions for CRITICAL errors ============
std::unique_ptr<TreeNode> loadTree(const std::string& file) {
    if (!std::filesystem::exists(file)) {
        throw TreeLoadError("File not found: " + file);  // Critical error!
    }
    // ... parse file ...
}

// Usage:
try {
    auto tree = loadTree("config.bt");
} catch (const TreeLoadError& e) {
    // Log error and exit - this is exceptional!
    fatal_error(e.what());
}
```

**Exception Safety Levels:**

| Level | Guarantee | Use When |
| :--- | :--- | :--- |
| **No-throw** | Guarantees no exceptions | Destructors, move operations |
| **Strong** | On failure, state unchanged | Critical operations |
| **Basic** | On failure, no resources leaked | Most operations |
| **No guarantee** | On failure, anything can happen | Legacy code only |

**When to Ignore Exception Handling:**
- Performance-critical game loops (use error codes instead)
- Embedded systems without exception support
- Tiny utility functions that never fail

---

## SECTION 4: MODERN C++ BEST PRACTICES

| Check | Best Practice | Why | Over-Engineering? |
| :--- | :--- | :--- | :--- |
| ☐ | Use `auto` for variable declarations | Less boilerplate, enforces type correctness | ❌ NOT over-engineering. Standard practice. |
| ☐ | Use `const` whenever possible | Prevents accidental mutation, enables optimization | ❌ NOT over-engineering. Always do this. |
| ☐ | Use `constexpr` for compile-time evaluation | Performance, compile-time errors | ⚠️ Only when value is known at compile time |
| ☐ | Use `noexcept` for non-throwing functions | Enables compiler optimizations | ⚠️ Only if function truly never throws |
| ☐ | Use `std::move` for transferring ownership | Avoids copies, faster | ❌ NOT over-engineering. Essential for performance. |
| ☐ | Use `std::string_view` for read-only strings | No copies, efficient | ❌ NOT over-engineering. Modern best practice. |
| ☐ | Use `std::span` for array views | No copies, bounds-safe | ❌ NOT over-engineering. C++20 best practice. |
| ☐ | Use `std::optional` for optional returns | Clear intent, type-safe | ❌ NOT over-engineering. Better than pointers. |
| ☐ | Use `std::variant` for type unions | Type-safe alternative to unions | ⚠️ Overkill for simple unions |
| ☐ | Use `override` keyword | Catches mistakes, clear intent | ❌ NOT over-engineering. Always use. |
| ☐ | Use `= default`/`= delete` for special members | Clear intent, less boilerplate | ❌ NOT over-engineering. Always use. |

---

## SECTION 5: PERFORMANCE VS OVER-ENGINEERING

### When to Optimize

| Check | Best Practice | Why | Over-Engineering? |
| :--- | :--- | :--- | :--- |
| ☐ | Do I have a performance problem? | Measure before optimizing! | ❌ NOT over-engineering. Essential rule. |
| ☐ | Is this on the critical path (60fps loop)? | Only optimize hot paths | ❌ NOT over-engineering. Focus effort. |
| ☐ | Does this allocate memory in loops? | Allocations are slow | ❌ NOT over-engineering. Important optimization. |
| ☐ | Is this called frequently (>1000x/sec)? | Inline small functions, avoid virtual | ⚠️ Only if profiler shows it's a problem |
| ☐ | Am I using cache-friendly data layouts? | Performance killer | ❌ NOT over-engineering. Game dev essential. |

### When to NOT Optimize (Over-Engineering)

| Check | This IS Over-Engineering | Do This Instead |
| :--- | :--- | :--- |
| ☐ | Premature optimization of simple code | Write clean, readable code first |
| ☐ | Micro-optimizations in non-critical paths | Focus on algorithmic improvements |
| ☐ | Caching without a performance problem | Profile first, then cache if needed |
| ☐ | Complex memory pools for few objects | Use `std::vector` - it's fast enough |
| ☐ | Manual inlining (using `inline` keyword) | Let the compiler decide |

**The 80/20 Rule (Pareto Principle):**
- **80%** of performance comes from **20%** of the code
- Profile to find the 20%, optimize that
- **Never optimize the other 80%** (it's over-engineering)

---

## SECTION 6: DESIGN PATTERN DECISION TREE (When to Use)

| Pattern | Use When | Over-Engineering When |
| :--- | :--- | :--- |
| **Factory** | Need dynamic creation of types | Only 1-2 types, simple `new` works |
| **Singleton** | Logging, config, truly global | Shared_ptr + dependency injection works better |
| **Observer** | > 3 subsystems need to react | Direct calls if only 1-2 subsystems |
| **Strategy** | Behavior changes at runtime | Single implementation, no need |
| **Decorator** | Adding behavior dynamically | Simple inheritance would work |
| **Builder** | Complex object creation | Simple constructor with defaults |
| **Command** | Undo/redo, queuing, logging | Simple function call works |
| **State** | Object behavior changes with state | Simple if/else or switch |
| **Visitor** | Operations on object structure | RTTI or virtual functions simpler |

---

## SECTION 7: THE MASTER CHECKLIST (Daily Reference)

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    DAILY DESIGN CHECKLIST                             │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│  ☐ SOLID Principles: Do I have a reason to change each class?        │
│                                                                        │
│  ☐ Loose Coupling: Do I depend on abstractions, not concretions?     │
│                                                                        │
│  ☐ RAII: Are all resources managed by smart pointers/RAII wrappers?  │
│                                                                        │
│  ☐ Exception Handling: Are exceptions only for exceptional cases?    │
│                                                                        │
│  ☐ Const Correctness: Is everything const that can be const?         │
│                                                                        │
│  ☐ Modern C++: Am I using auto, constexpr, noexcept where possible?  │
│                                                                        │
│  ☐ Memory Management: Am I using the right smart pointer?            │
│                                                                        │
│  ☐ Performance: Have I profiled to find the REAL bottlenecks?        │
│                                                                        │
│  ☐ Testability: Can I test this class in isolation?                  │
│                                                                        │
│  ☐ Over-Engineering: Would a simpler solution work?                  │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## THE GOLDEN RULES

```
┌─────────────────────────────────────────────────────────────────────────┐
│                    THE 5 GOLDEN RULES                                  │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│  1. "KISS" - Keep It Simple, Stupid                                   │
│     The simplest solution that works is usually the best.             │
│                                                                        │
│  2. "YAGNI" - You Aren't Gonna Need It                               │
│     Don't add flexibility until you actually need it.                 │
│                                                                        │
│  3. "DRY" - Don't Repeat Yourself                                     │
│     Duplicate code is the root of all evil.                           │
│                                                                        │
│  4. "Test First" - If you can't test it, don't build it              │
│     Testability is a design goal, not an afterthought.                │
│                                                                        │
│  5. "Premature Optimization is Evil"                                 │
│     Make it work, make it right, make it fast - IN THAT ORDER.       │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## THE OVER-ENGINEERING DETECTOR

**Ask yourself these questions:**

```
☐ Is this solving a problem I don't actually have yet?
☐ Does this add complexity without clear benefit?
☐ Could a simpler solution work?
☐ Will this make the code harder for others to understand?
☐ Am I doing this because it's "cool" rather than "needed"?
☐ Does this add more than 10% complexity for less than 50% benefit?
```

**If you answered "YES" to 3+ questions → YOU ARE OVER-ENGINEERING!**

---

## FINAL SUMMARY

| When in doubt... | Use This |
| :--- | :--- |
| **Need dynamic polymorphism?** | `std::unique_ptr` + virtual functions |
| **Need shared access?** | `std::shared_ptr` |
| **Need to break cycles?** | `std::weak_ptr` |
| **Need optional return?** | `std::optional` |
| **Need multiple error types?** | `std::variant` or `std::expected` (C++23) |
| **Need configurable behavior?** | `std::function` or Strategy pattern |
| **Need to decouple systems?** | Observer pattern (only if > 3 subscribers) |
| **Need to create objects?** | Factory pattern (only if > 2 types) |
| **Need to manage resources?** | RAII (ALWAYS!) |

**The ultimate rule:** Write code that's clear, testable, and maintainable. Optimize when the profiler tells you to. Everything else is over-engineering. 🚀