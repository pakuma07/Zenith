# Factory Method Pattern

## Overview

The Factory Method pattern defines an interface for creating an object, but lets subclasses decide which class to instantiate.

## Intent

- Define interface for object creation
- Let subclasses decide the concrete class
- Hide instantiation details
- Promote loose coupling
- Centralize object creation

## Problem

You need to create objects, but:
- The exact class isn't known until runtime
- Different subclasses may need different object types
- Client shouldn't depend on concrete classes
- Object creation is complex or context-dependent

## Solution

Define an abstract method in a base class for creating objects. Let subclasses override this method to create their specific types.

## Structure

```
        ┌──────────────┐
        │ Creator      │
        ├──────────────┤
        │ +factoryMethod()│ (abstract)
        │ +operation() │
        └──────────────┘
               △
               │
        ┌──────┴──────┐
        │             │
   ┌─────────┐    ┌─────────┐
   │ConcreteA│    │ConcreteB│
   ├─────────┤    ├─────────┤
   │factoryM()│  │factoryM()│
   └─────────┘    └─────────┘
         │              │
         └──────┬───────┘
                │
         ┌──────▼──────┐
         │ Product     │
         ├─────────────┤
         │ +operation()│
         └─────────────┘
```

## C++11 Implementation

### Basic Factory Method

```cpp
// Abstract Product
class Transport {
public:
    virtual ~Transport() = default;
    virtual void deliver() = 0;
};

// Concrete Products
class Truck : public Transport {
public:
    void deliver() override {
        cout << "Delivering by truck (land)" << endl;
    }
};

class Ship : public Transport {
public:
    void deliver() override {
        cout << "Delivering by ship (sea)" << endl;
    }
};

// Abstract Creator
class Logistics {
public:
    virtual ~Logistics() = default;
    
    virtual unique_ptr<Transport> createTransport() = 0;
    
    void planDelivery() {
        auto transport = createTransport();
        transport->deliver();
    }
};

// Concrete Creators
class RoadLogistics : public Logistics {
public:
    unique_ptr<Transport> createTransport() override {
        return make_unique<Truck>();
    }
};

class SeaLogistics : public Logistics {
public:
    unique_ptr<Transport> createTransport() override {
        return make_unique<Ship>();
    }
};
```

### Parameter-Based Factory Method

```cpp
class DocumentFactory {
public:
    virtual ~DocumentFactory() = default;
    virtual unique_ptr<Document> createDocument() = 0;
    
    void openDocument() {
        auto doc = createDocument();
        doc->open();
        doc->save();
        doc->close();
    }
};

class PdfFactory : public DocumentFactory {
public:
    unique_ptr<Document> createDocument() override {
        return make_unique<PdfDocument>();
    }
};

class WordFactory : public DocumentFactory {
public:
    unique_ptr<Document> createDocument() override {
        return make_unique<WordDocument>();
    }
};
```

### Static Factory Method

```cpp
class Button {
public:
    virtual ~Button() = default;
    virtual void render() = 0;
};

class WindowsButton : public Button {
public:
    void render() override {
        cout << "Rendering Windows button" << endl;
    }
};

class MacButton : public Button {
public:
    void render() override {
        cout << "Rendering Mac button" << endl;
    }
};

class Dialog {
public:
    // Static factory method
    static unique_ptr<Button> createButton(const string& os) {
        if (os == "Windows") {
            return make_unique<WindowsButton>();
        } else if (os == "Mac") {
            return make_unique<MacButton>();
        }
        throw runtime_error("Unknown OS");
    }
};
```

## Usage Examples

### Shape Factory

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() = 0;
};

class Circle : public Shape {
public:
    void draw() override {
        cout << "Drawing circle" << endl;
    }
};

class Rectangle : public Shape {
public:
    void draw() override {
        cout << "Drawing rectangle" << endl;
    }
};

class ShapeFactory {
public:
    static unique_ptr<Shape> createShape(const string& type) {
        if (type == "circle") {
            return make_unique<Circle>();
        } else if (type == "rectangle") {
            return make_unique<Rectangle>();
        }
        return nullptr;
    }
};

// Usage
auto shape1 = ShapeFactory::createShape("circle");
auto shape2 = ShapeFactory::createShape("rectangle");
shape1->draw();  // Drawing circle
shape2->draw();  // Drawing rectangle
```

### Database Connection Factory

```cpp
class Database {
public:
    virtual ~Database() = default;
    virtual void connect() = 0;
    virtual void executeQuery(const string& query) = 0;
};

class MySQLDatabase : public Database {
public:
    void connect() override { cout << "Connecting to MySQL" << endl; }
    void executeQuery(const string& q) override { cout << "MySQL: " << q << endl; }
};

class PostgresDatabase : public Database {
public:
    void connect() override { cout << "Connecting to PostgreSQL" << endl; }
    void executeQuery(const string& q) override { cout << "PostgreSQL: " << q << endl; }
};

class DatabaseFactory {
public:
    static unique_ptr<Database> create(const string& type) {
        if (type == "mysql") {
            return make_unique<MySQLDatabase>();
        } else if (type == "postgres") {
            return make_unique<PostgresDatabase>();
        }
        throw runtime_error("Unknown database type");
    }
};
```

### Notification System

```cpp
class Notification {
public:
    virtual ~Notification() = default;
    virtual void send(const string& message) = 0;
};

class EmailNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Sending email: " << message << endl;
    }
};

class SMSNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Sending SMS: " << message << endl;
    }
};

class NotificationFactory {
public:
    static unique_ptr<Notification> create(const string& type) {
        if (type == "email") {
            return make_unique<EmailNotification>();
        } else if (type == "sms") {
            return make_unique<SMSNotification>();
        }
        return nullptr;
    }
};
```

## Advantages

✅ **Loose coupling** - Clients don't depend on concrete classes
✅ **Extensible** - Add new products without changing existing code
✅ **Single Responsibility** - Creation logic in one place
✅ **Open/Closed Principle** - Open for extension, closed for modification
✅ **Flexible** - Easy to switch implementations

## Disadvantages

❌ **Increased complexity** - More classes and interfaces
❌ **Over-engineering** - May be overkill for simple cases
❌ **Parallel class hierarchies** - Creator and Product both have subclasses
❌ **Indirection** - Extra layer between object creation and use

## When to Use

✅ **Multiple implementations** - Different versions of same interface
✅ **Runtime selection** - Choose implementation at runtime
✅ **Framework code** - Let subclasses decide what to create
✅ **Plugin systems** - Load implementations dynamically
✅ **Cross-platform** - Different implementations per platform

## When NOT to Use

❌ Simple, single product type
❌ Few variations of object
❌ Creation logic is trivial
❌ Premature optimization (YAGNI)

## Related Patterns

- **Abstract Factory** - Often uses Factory Method
- **Template Method** - Factory method commonly implemented as template method
- **Singleton** - Can be used with factory methods

## Best Practices

1. **Return interface, not concrete class** - Use base class pointers/references
2. **Use smart pointers** - For automatic memory management
3. **Parameter validation** - Check parameters before creating
4. **Default behavior** - Provide sensible defaults
5. **Cache when appropriate** - Reuse expensive objects
6. **Document subclasses** - List all product types available
7. **Consistent naming** - Use clear names for factory methods

## Implementation Checklist

- [ ] Define abstract Product class
- [ ] Create concrete Product classes
- [ ] Define abstract Creator class with factory method
- [ ] Implement concrete Creator classes
- [ ] Return Product as smart pointer (unique_ptr, shared_ptr)
- [ ] Validate parameters
- [ ] Handle unknown types gracefully
- [ ] Unit test all variants
- [ ] Document available product types
