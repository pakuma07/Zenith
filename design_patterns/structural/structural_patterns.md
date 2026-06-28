# Adapter Pattern

## Overview

The Adapter pattern converts the interface of a class into another interface clients expect, allowing incompatible interfaces to work together.

## Intent

- Make incompatible interfaces compatible
- Reuse existing classes with incompatible interfaces
- Act as middleware between two interfaces
- Convert data between different formats

## Problem

You have two classes with incompatible interfaces that need to work together:
- New class needs old implementation
- Third-party library with incompatible interface
- Different data formats that need translation
- Legacy code integration

## Solution

Create an Adapter class that:
1. Inherits from the interface clients expect
2. Wraps the incompatible class
3. Translates between interfaces

## Structure (Class Adapter)

```
┌──────────────┐         ┌─────────────────┐
│   Target     │         │   Adaptee       │
├──────────────┤         ├─────────────────┤
│+request()    │         │+specificRequest │
└──────────────┘         └─────────────────┘
       △                         △
       │                         │
       └──────────┬──────────────┘
                  │
              ┌───────────┐
              │  Adapter  │
              ├───────────┤
              │+request() │
              └───────────┘
```

## C++11 Implementation

### Class Adapter

```cpp
// Target interface (what client expects)
class Target {
public:
    virtual ~Target() = default;
    virtual void request() = 0;
};

// Adaptee (incompatible interface)
class Adaptee {
public:
    void specificRequest() {
        cout << "Adaptee specific request" << endl;
    }
};

// Adapter
class Adapter : public Target, private Adaptee {
public:
    void request() override {
        specificRequest();  // Call adaptee's method
    }
};

// Usage
unique_ptr<Target> target = make_unique<Adapter>();
target->request();
```

### Object Adapter

```cpp
// Target
class MediaPlayer {
public:
    virtual ~MediaPlayer() = default;
    virtual void play(const string& filename) = 0;
};

// Adaptee
class VLCPlayer {
public:
    void playVLC(const string& file) {
        cout << "Playing VLC: " << file << endl;
    }
};

// Adapter
class VLCAdapter : public MediaPlayer {
private:
    unique_ptr<VLCPlayer> vlc;
    
public:
    VLCAdapter() : vlc(make_unique<VLCPlayer>()) {}
    
    void play(const string& filename) override {
        vlc->playVLC(filename);
    }
};

// Usage
vector<unique_ptr<MediaPlayer>> players;
players.push_back(make_unique<VLCAdapter>());

for (auto& player : players) {
    player->play("video.mp4");
}
```

### Data Format Adapter

```cpp
// Expected interface
class PaymentProcessor {
public:
    virtual ~PaymentProcessor() = default;
    virtual void processPayment(double amount) = 0;
};

// Legacy payment system
class LegacyPaymentGateway {
public:
    void handleTransaction(const string& amount, const string& currency) {
        cout << "Processing: " << amount << " " << currency << endl;
    }
};

// Adapter
class LegacyPaymentAdapter : public PaymentProcessor {
private:
    unique_ptr<LegacyPaymentGateway> legacy;
    
public:
    LegacyPaymentAdapter() : legacy(make_unique<LegacyPaymentGateway>()) {}
    
    void processPayment(double amount) override {
        legacy->handleTransaction(to_string(amount), "USD");
    }
};
```

## Advantages

✅ **Reusability** - Use incompatible classes together
✅ **Single Responsibility** - Conversion logic isolated
✅ **Flexibility** - Adapter can be swapped
✅ **No modification** - Don't modify existing classes
✅ **Late binding** - Choose adapter at runtime

## Disadvantages

❌ **Complexity** - Extra layer of indirection
❌ **Performance** - Overhead from wrapping
❌ **Debugging** - Harder to trace through adapter

## When to Use

✅ Integrating incompatible interfaces
✅ Third-party library integration
✅ Legacycode modernization
✅ Data format conversion
✅ API compatibility

## Related Patterns

- **Bridge** - Similar structure, different intent
- **Decorator** - Similar wrapping approach
- **Proxy** - Similar structure for control

---

# Decorator Pattern

## Overview

The Decorator pattern attaches additional responsibilities to an object dynamically, providing a flexible alternative to subclassing.

## Intent

- Add responsibilities to objects dynamically
- Avoid rigid subclass hierarchies
- Combine behaviors flexibly
- Keep single responsibility principle

## Problem

You need to add features to objects, but:
- Subclassing leads to class explosion
- Feature combinations are many
- Features need runtime composition
- Inheritance tree becomes unmanageable

## Solution

Create Decorator classes that:
1. Wrap target object
2. Implement same interface
3. Add new behavior
4. Delegate to wrapped object

## C++11 Implementation

```cpp
// Component
class Coffee {
public:
    virtual ~Coffee() = default;
    virtual double getCost() = 0;
    virtual string getDescription() = 0;
};

// Concrete component
class SimpleCoffee : public Coffee {
public:
    double getCost() override { return 2.0; }
    string getDescription() override { return "Simple Coffee"; }
};

// Decorator
class CoffeeDecorator : public Coffee {
protected:
    unique_ptr<Coffee> coffee;
    
public:
    CoffeeDecorator(unique_ptr<Coffee> c) : coffee(move(c)) {}
};

// Concrete decorators
class MilkDecorator : public CoffeeDecorator {
public:
    MilkDecorator(unique_ptr<Coffee> c) : CoffeeDecorator(move(c)) {}
    
    double getCost() override { return coffee->getCost() + 0.5; }
    string getDescription() override { return coffee->getDescription() + ", Milk"; }
};

class SugarDecorator : public CoffeeDecorator {
public:
    SugarDecorator(unique_ptr<Coffee> c) : CoffeeDecorator(move(c)) {}
    
    double getCost() override { return coffee->getCost() + 0.25; }
    string getDescription() override { return coffee->getDescription() + ", Sugar"; }
};

// Usage
auto coffee = make_unique<SimpleCoffee>();
coffee = make_unique<MilkDecorator>(move(coffee));
coffee = make_unique<SugarDecorator>(move(coffee));

cout << coffee->getDescription() << ": $" << coffee->getCost() << endl;
// Output: Simple Coffee, Milk, Sugar: $2.75
```

## Advantages

✅ **Flexible combination** - Combine behaviors at runtime
✅ **Single Responsibility** - Each decorator has one job
✅ **Open/Closed** - Open for extension, closed for modification
✅ **Dynamic** - Add/remove features without subclassing
✅ **Transparent** - Same interface as wrapped object

## Disadvantages

❌ **Complexity** - Multiple wrapper objects
❌ **Order matters** - Decorator order affects behavior
❌ **Debugging** - Hard to trace through layers
❌ **Performance** - Overhead from wrapping

## When to Use

✅ Dynamic feature addition
✅ Avoid subclass explosion
✅ Features can be combined
✅ Runtime configuration needed
✅ Single Responsibility Principle important

---

# Composite Pattern

## Overview

The Composite pattern composes objects into tree structures to represent part-whole hierarchies, letting clients treat individual objects and compositions uniformly.

## Intent

- Treat individual and composite objects uniformly
- Create tree structures
- Recursive hierarchies
- Client simplification

## C++11 Implementation

```cpp
// Component
class FileSystemItem {
public:
    virtual ~FileSystemItem() = default;
    virtual void display(int indent = 0) = 0;
    virtual int getSize() = 0;
};

// Leaf
class File : public FileSystemItem {
private:
    string name;
    int size;
    
public:
    File(const string& n, int s) : name(n), size(s) {}
    
    void display(int indent = 0) override {
        cout << string(indent, ' ') << "File: " << name << " (" << size << "B)" << endl;
    }
    
    int getSize() override { return size; }
};

// Composite
class Directory : public FileSystemItem {
private:
    string name;
    vector<unique_ptr<FileSystemItem>> items;
    
public:
    Directory(const string& n) : name(n) {}
    
    void add(unique_ptr<FileSystemItem> item) {
        items.push_back(move(item));
    }
    
    void display(int indent = 0) override {
        cout << string(indent, ' ') << "Dir: " << name << endl;
        for (auto& item : items) {
            item->display(indent + 2);
        }
    }
    
    int getSize() override {
        int total = 0;
        for (auto& item : items) {
            total += item->getSize();
        }
        return total;
    }
};

// Usage
auto root = make_unique<Directory>("root");
root->add(make_unique<File>("file1.txt", 100));
root->add(make_unique<File>("file2.txt", 200));

auto subdir = make_unique<Directory>("subdir");
subdir->add(make_unique<File>("file3.txt", 150));
root->add(move(subdir));

root->display();
cout << "Total size: " << root->getSize() << endl;
```

---

# Facade Pattern

## Overview

The Facade pattern provides a unified, simplified interface to a set of interfaces in a subsystem.

## C++11 Implementation

```cpp
// Complex subsystem
class DatabaseConnection { /* ... */ };
class CacheManager { /* ... */ };
class Logger { /* ... */ };

// Facade
class ApplicationFacade {
private:
    unique_ptr<DatabaseConnection> db;
    unique_ptr<CacheManager> cache;
    unique_ptr<Logger> logger;
    
public:
    ApplicationFacade() {
        db = make_unique<DatabaseConnection>();
        cache = make_unique<CacheManager>();
        logger = make_unique<Logger>();
    }
    
    void getUserData(int userId) {
        logger->log("Fetching user " + to_string(userId));
        if (cache->has(userId)) {
            logger->log("Cache hit");
            return;
        }
        logger->log("Querying database");
        db->query("SELECT * FROM users WHERE id = " + to_string(userId));
        cache->set(userId, "data");
    }
};

// Client only sees simple interface
auto app = make_unique<ApplicationFacade>();
app->getUserData(123);
```

---

All structural patterns follow similar principles of composition and delegation to provide flexible, maintainable designs.
