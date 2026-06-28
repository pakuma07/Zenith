# Singleton Pattern

## Overview

The Singleton pattern ensures that a class has only one instance and provides a global point of access to it.

## Intent

- Ensure a class has only one instance
- Provide global access to that instance
- Lazy initialization (create when needed)
- Thread-safe in multi-threaded environments

## Problem

Some classes should have only one instance:
- Database connections
- Logger
- Configuration manager
- Thread pools
- Window managers

Creating multiple instances wastes resources and can cause problems.

## Solution

Create a class with:
1. Private constructor (prevent instantiation)
2. Static instance variable
3. Static getter method to access instance
4. Delete copy constructor and assignment operator

## Structure

```
┌─────────────┐
│  Singleton  │
├─────────────┤
│ - instance  │ (static)
├─────────────┤
│ + getInstance() │ (static)
│ - Singleton()   │ (private)
│ - ~Singleton()  │ (private)
└─────────────┘
```

## C++11 Implementation

### Classic Singleton

```cpp
class Singleton {
private:
    static Singleton* instance;
    
    // Private constructor
    Singleton() {}
    
    // Delete copy operations
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    
public:
    // Get singleton instance
    static Singleton* getInstance() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
    
    // Public methods
    void doSomething() {
        cout << "Singleton operation" << endl;
    }
    
    // Destructor
    ~Singleton() {}
};

// Static member definition
Singleton* Singleton::instance = nullptr;
```

### Thread-Safe Singleton (Meyer's Singleton)

```cpp
class ThreadSafeSingleton {
private:
    ThreadSafeSingleton() {}
    
    ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;
    
public:
    static ThreadSafeSingleton& getInstance() {
        static ThreadSafeSingleton instance;  // Created once, thread-safe
        return instance;
    }
    
    void doSomething() {
        cout << "Thread-safe singleton operation" << endl;
    }
};
```

### Smart Pointer Singleton

```cpp
class SmartSingleton {
private:
    static unique_ptr<SmartSingleton> instance;
    
    SmartSingleton() {}
    
    SmartSingleton(const SmartSingleton&) = delete;
    SmartSingleton& operator=(const SmartSingleton&) = delete;
    
public:
    static SmartSingleton* getInstance() {
        if (!instance) {
            instance = make_unique<SmartSingleton>();
        }
        return instance.get();
    }
    
    void doSomething() {
        cout << "Smart pointer singleton" << endl;
    }
};

unique_ptr<SmartSingleton> SmartSingleton::instance = nullptr;
```

## Usage Examples

### Logger Singleton

```cpp
class Logger {
private:
    static Logger* instance;
    Logger() {}
    
public:
    static Logger* getInstance() {
        if (!instance) {
            instance = new Logger();
        }
        return instance;
    }
    
    void log(const string& message) {
        cout << "[LOG] " << message << endl;
    }
};

// Usage
Logger::getInstance()->log("Application started");
Logger::getInstance()->log("Processing data");
Logger::getInstance()->log("Application ended");
```

### Configuration Manager

```cpp
class ConfigManager {
private:
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }
    
    map<string, string> configs;
    
    ConfigManager() {
        // Load configuration
        configs["db_host"] = "localhost";
        configs["db_port"] = "5432";
        configs["timeout"] = "30";
    }
    
public:
    static string get(const string& key) {
        return getInstance().configs[key];
    }
    
    static void set(const string& key, const string& value) {
        getInstance().configs[key] = value;
    }
};

// Usage
cout << ConfigManager::get("db_host") << endl;
ConfigManager::set("timeout", "60");
```

### Database Connection Pool

```cpp
class DatabasePool {
private:
    static DatabasePool& getInstance() {
        static DatabasePool instance;
        return instance;
    }
    
    vector<shared_ptr<Connection>> connections;
    
    DatabasePool() {
        // Initialize connection pool
        for (int i = 0; i < 5; i++) {
            connections.push_back(make_shared<Connection>());
        }
    }
    
public:
    static shared_ptr<Connection> getConnection() {
        auto& pool = getInstance();
        if (!pool.connections.empty()) {
            auto conn = pool.connections.back();
            pool.connections.pop_back();
            return conn;
        }
        return nullptr;
    }
    
    static void releaseConnection(shared_ptr<Connection> conn) {
        getInstance().connections.push_back(conn);
    }
};
```

## Advantages

✅ **Controlled access** - Single, global access point
✅ **Lazy initialization** - Created only when needed
✅ **Resource efficiency** - Only one instance exists
✅ **Thread safety** - Meyer's singleton is thread-safe
✅ **Easy to use** - Simple static getter method

## Disadvantages

❌ **Global state** - Hides dependencies
❌ **Testing difficulties** - Hard to mock for unit tests
❌ **Hidden coupling** - All code depends on singleton
❌ **Thread safety complexity** - Must handle multi-threading
❌ **Performance** - Global access point can be bottleneck

## When to Use

✅ **Logger** - Single logging instance for entire app
✅ **Configuration Manager** - One config source
✅ **Database Connection Pool** - Manage single pool
✅ **Cache** - Single cache for application
✅ **Thread Pool** - Single thread manager
✅ **File System** - Single file system interface

## When NOT to Use

❌ Replacing global variables without good reason
❌ When you need multiple instances
❌ In highly concurrent systems (complex synchronization)
❌ For dependency injection scenarios
❌ When testability is critical

## Related Patterns

- **Factory Method** - Can use Singleton to manage instances
- **Abstract Factory** - Often used with Singleton
- **Facade** - Often implemented as Singleton

## Best Practices

1. **Use Meyer's Singleton** - Easiest thread-safe approach
2. **Avoid accessing globally** - Pass as dependency when possible
3. **Make destructor virtual** - If using inheritance
4. **Delete copy operations** - Prevent accidental copies
5. **Use smart pointers** - For automatic cleanup
6. **Document the purpose** - Why this class needs to be singleton
7. **Consider alternatives** - Dependency injection might be better

## Common Pitfalls

⚠️ **Too many singletons** - "Singleton creep"
⚠️ **Hidden dependencies** - Hard to see what's needed
⚠️ **Testing nightmare** - Difficult to unit test
⚠️ **Thread safety issues** - Improper synchronization
⚠️ **Memory leaks** - With `new` and no deletion

## Implementation Checklist

- [ ] Make constructor private
- [ ] Delete copy constructor and assignment
- [ ] Create static instance variable
- [ ] Implement static getInstance()
- [ ] Ensure thread safety (if needed)
- [ ] Consider memory management (smart pointers)
- [ ] Document why it's a singleton
- [ ] Unit test the singleton behavior
- [ ] Consider testing alternatives (fakes, mocks)
