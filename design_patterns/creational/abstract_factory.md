# Abstract Factory Pattern

## Overview

The Abstract Factory pattern provides an interface for creating families of related or dependent objects without specifying their concrete classes.

## Intent

- Create families of related objects
- Ensure products work together
- Decouple client from concrete classes
- Switch entire families of products easily

## Problem

You need to create multiple related objects that must work together:
- UI components for different platforms (Windows, Mac, Linux)
- Database and cache implementations that pair
- Furniture styles (modern, victorian) with matching pieces
- Game assets (characters, enemies, items) with consistent style

You want to ensure products from the same family are used together.

## Solution

Define abstract factory that declares methods for creating each product type. Implement concrete factories for each product family.

## Structure

```
         ┌──────────────────┐
         │ AbstractFactory  │
         ├──────────────────┤
         │ +createProductA()│
         │ +createProductB()│
         └──────────────────┘
                △
                │
        ┌───────┴────────┐
        │                │
   ┌──────────┐      ┌──────────┐
   │ConcreteA │      │ConcreteB │
   ├──────────┤      ├──────────┤
   │factory   │      │factory   │
   └──────────┘      └──────────┘
        │                 │
        ├─────────┬───────┤
        │         │       │
   ┌─────────┐ ┌──────┐ ┌──────┐
   │ProductA1│ │Product│ │Product
   │ProductA2│ │B1    │ │B2
   └─────────┘ └──────┘ └──────┘
```

## C++11 Implementation

### GUI Components Example

```cpp
// Abstract Products
class Button {
public:
    virtual ~Button() = default;
    virtual void click() = 0;
};

class Checkbox {
public:
    virtual ~Checkbox() = default;
    virtual void check() = 0;
};

// Concrete Products - Windows
class WindowsButton : public Button {
public:
    void click() override {
        cout << "Windows button clicked" << endl;
    }
};

class WindowsCheckbox : public Checkbox {
public:
    void check() override {
        cout << "Windows checkbox checked" << endl;
    }
};

// Concrete Products - Mac
class MacButton : public Button {
public:
    void click() override {
        cout << "Mac button clicked" << endl;
    }
};

class MacCheckbox : public Checkbox {
public:
    void check() override {
        cout << "Mac checkbox checked" << endl;
    }
};

// Abstract Factory
class GUIFactory {
public:
    virtual ~GUIFactory() = default;
    virtual unique_ptr<Button> createButton() = 0;
    virtual unique_ptr<Checkbox> createCheckbox() = 0;
};

// Concrete Factories
class WindowsFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<WindowsButton>();
    }
    
    unique_ptr<Checkbox> createCheckbox() override {
        return make_unique<WindowsCheckbox>();
    }
};

class MacFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<MacButton>();
    }
    
    unique_ptr<Checkbox> createCheckbox() override {
        return make_unique<MacCheckbox>();
    }
};

// Client
void createUI(GUIFactory& factory) {
    auto button = factory.createButton();
    auto checkbox = factory.createCheckbox();
    
    button->click();
    checkbox->check();
}
```

## Usage Examples

### Furniture Factory

```cpp
class Chair {
public:
    virtual ~Chair() = default;
    virtual void sitOn() = 0;
};

class Table {
public:
    virtual ~Table() = default;
    virtual void placeOn() = 0;
};

// Modern furniture
class ModernChair : public Chair {
public:
    void sitOn() override { cout << "Modern chair" << endl; }
};

class ModernTable : public Table {
public:
    void placeOn() override { cout << "Modern table" << endl; }
};

// Victorian furniture
class VictorianChair : public Chair {
public:
    void sitOn() override { cout << "Victorian chair" << endl; }
};

class VictorianTable : public Table {
public:
    void placeOn() override { cout << "Victorian table" << endl; }
};

// Abstract furniture factory
class FurnitureFactory {
public:
    virtual ~FurnitureFactory() = default;
    virtual unique_ptr<Chair> createChair() = 0;
    virtual unique_ptr<Table> createTable() = 0;
};

// Concrete factories
class ModernFactory : public FurnitureFactory {
public:
    unique_ptr<Chair> createChair() override {
        return make_unique<ModernChair>();
    }
    
    unique_ptr<Table> createTable() override {
        return make_unique<ModernTable>();
    }
};

class VictorianFactory : public FurnitureFactory {
public:
    unique_ptr<Chair> createChair() override {
        return make_unique<VictorianChair>();
    }
    
    unique_ptr<Table> createTable() override {
        return make_unique<VictorianTable>();
    }
};

// Usage
void furnishRoom(FurnitureFactory& factory) {
    auto chair = factory.createChair();
    auto table = factory.createTable();
    chair->sitOn();
    table->placeOn();
}
```

### Database and Cache Factory

```cpp
class Database {
public:
    virtual ~Database() = default;
    virtual void query(const string& sql) = 0;
};

class Cache {
public:
    virtual ~Cache() = default;
    virtual void set(const string& key, const string& value) = 0;
    virtual string get(const string& key) = 0;
};

// MySQL with Redis
class MySQLDatabase : public Database {
public:
    void query(const string& sql) override {
        cout << "MySQL: " << sql << endl;
    }
};

class RedisCache : public Cache {
public:
    void set(const string& key, const string& value) override {
        cout << "Redis SET " << key << " = " << value << endl;
    }
    
    string get(const string& key) override {
        cout << "Redis GET " << key << endl;
        return "value";
    }
};

// PostgreSQL with Memcached
class PostgresDatabase : public Database {
public:
    void query(const string& sql) override {
        cout << "PostgreSQL: " << sql << endl;
    }
};

class MemcachedCache : public Cache {
public:
    void set(const string& key, const string& value) override {
        cout << "Memcached SET " << key << " = " << value << endl;
    }
    
    string get(const string& key) override {
        cout << "Memcached GET " << key << endl;
        return "value";
    }
};

// Abstract factory
class DataStackFactory {
public:
    virtual ~DataStackFactory() = default;
    virtual unique_ptr<Database> createDatabase() = 0;
    virtual unique_ptr<Cache> createCache() = 0;
};

// Concrete factories
class MySQLRedisFactory : public DataStackFactory {
public:
    unique_ptr<Database> createDatabase() override {
        return make_unique<MySQLDatabase>();
    }
    
    unique_ptr<Cache> createCache() override {
        return make_unique<RedisCache>();
    }
};

class PostgresMemcachedFactory : public DataStackFactory {
public:
    unique_ptr<Database> createDatabase() override {
        return make_unique<PostgresDatabase>();
    }
    
    unique_ptr<Cache> createCache() override {
        return make_unique<MemcachedCache>();
    }
};
```

## Advantages

✅ **Ensures consistency** - Products from same family work together
✅ **Decoupling** - Clients don't depend on concrete classes
✅ **Easy switching** - Change entire product family easily
✅ **Centralized creation** - All creation logic in one place
✅ **Single Responsibility** - Each factory has one job

## Disadvantages

❌ **Complexity** - Many classes and interfaces
❌ **Over-engineering** - Overkill for simple scenarios
❌ **Adding products** - Need new method in all factories
❌ **Parallel hierarchies** - Both products and factories grow together

## When to Use

✅ **Multiple product families** - Related products that must work together
✅ **Platform independence** - Different implementations per platform
✅ **Flexible configuration** - Switch families at runtime
✅ **Complex initialization** - Multiple interdependent objects
✅ **Consistency required** - Products must be compatible

## When NOT to Use

❌ Single product family
❌ Few product variations
❌ Simple creation logic
❌ Only one variant at a time

## Related Patterns

- **Factory Method** - Often used by Abstract Factory
- **Singleton** - Factories often are singletons
- **Bridge** - Combines with abstract factory for abstraction/implementation
- **Strategy** - Can be used to select factory family

## Best Practices

1. **Use smart pointers** - For automatic memory management
2. **Return interfaces** - Clients should only see abstract types
3. **Document families** - List all product families
4. **Consider defaults** - Have sensible default factory
5. **Cache factories** - Avoid creating factories repeatedly
6. **Use static methods** - For factory creation if appropriate
7. **Validate invariants** - Ensure products from family work together

## Implementation Checklist

- [ ] Define abstract product interfaces
- [ ] Create concrete product classes for each family
- [ ] Define abstract factory interface
- [ ] Implement concrete factories for each family
- [ ] Ensure products from same family work together
- [ ] Use smart pointers for memory management
- [ ] Document all product families
- [ ] Validate that incompatible products can't be mixed
- [ ] Unit test each factory combination
- [ ] Document how to add new product families
