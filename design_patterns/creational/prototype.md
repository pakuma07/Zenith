# Prototype Pattern

## Overview

The Prototype pattern creates new objects by copying an existing object (prototype) rather than creating from scratch.

## Intent

- Create objects by cloning prototypes
- Avoid expensive object creation
- Hide creation complexity
- Create variations of existing objects
- Specify new object's class at runtime

## Problem

Creating objects from scratch is expensive:
- Complex initialization logic
- Deep hierarchies to navigate
- Resource-intensive setup
- Similar objects with slight variations
- Need to clone at runtime without knowing type

## Solution

Define a Prototype interface with clone() method. Concrete prototypes implement clone() to create copies of themselves. Client clones prototypes instead of creating new objects.

## Structure

```
┌──────────────────┐
│   Prototype      │
├──────────────────┤
│+clone()          │ (virtual)
└──────────────────┘
        △
        │
   ┌────┴─────┐
   │           │
┌──────────┐ ┌──────────┐
│ConcreteA │ │ConcreteB │
├──────────┤ ├──────────┤
│+clone()  │ │+clone()  │
└──────────┘ └──────────┘
```

## C++11 Implementation

### Basic Prototype

```cpp
// Abstract prototype
class Prototype {
public:
    virtual ~Prototype() = default;
    virtual unique_ptr<Prototype> clone() const = 0;
    virtual void show() const = 0;
};

// Concrete prototype
class ConcretePrototype : public Prototype {
private:
    string name;
    int value;
    
public:
    ConcretePrototype(const string& n, int v) 
        : name(n), value(v) {}
    
    unique_ptr<Prototype> clone() const override {
        return make_unique<ConcretePrototype>(*this);
    }
    
    void show() const override {
        cout << "Name: " << name << ", Value: " << value << endl;
    }
};

// Usage
auto original = make_unique<ConcretePrototype>("Original", 10);
auto clone1 = original->clone();
auto clone2 = original->clone();

original->show();  // Name: Original, Value: 10
clone1->show();    // Name: Original, Value: 10
clone2->show();    // Name: Original, Value: 10
```

### Deep Copy Prototype

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual unique_ptr<Shape> clone() const = 0;
    virtual void draw() const = 0;
};

class Circle : public Shape {
private:
    int radius;
    vector<int> points;  // Complex data
    
public:
    Circle(int r) : radius(r) {
        // Expensive initialization
        for (int i = 0; i < 1000; i++) {
            points.push_back(i);
        }
    }
    
    unique_ptr<Shape> clone() const override {
        return make_unique<Circle>(*this);  // Deep copy
    }
    
    void draw() const override {
        cout << "Drawing circle with radius " << radius << endl;
    }
};

// Usage - Cloning is faster than recreating
auto original = make_unique<Circle>(10);
auto clone1 = original->clone();
auto clone2 = original->clone();
```

### Shape Prototype Registry

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual unique_ptr<Shape> clone() const = 0;
    virtual string getType() const = 0;
};

class Circle : public Shape {
private:
    int radius;
public:
    Circle(int r) : radius(r) {}
    unique_ptr<Shape> clone() const override {
        return make_unique<Circle>(*this);
    }
    string getType() const override { return "Circle"; }
};

class Rectangle : public Shape {
private:
    int width, height;
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    unique_ptr<Shape> clone() const override {
        return make_unique<Rectangle>(*this);
    }
    string getType() const override { return "Rectangle"; }
};

// Prototype registry
class ShapePrototypeFactory {
private:
    map<string, unique_ptr<Shape>> prototypes;
    
public:
    void registerPrototype(const string& key, unique_ptr<Shape> shape) {
        prototypes[key] = move(shape);
    }
    
    unique_ptr<Shape> createShape(const string& key) const {
        auto it = prototypes.find(key);
        if (it != prototypes.end()) {
            return it->second->clone();
        }
        return nullptr;
    }
};

// Usage
ShapePrototypeFactory factory;
factory.registerPrototype("circle", make_unique<Circle>(5));
factory.registerPrototype("rectangle", make_unique<Rectangle>(10, 20));

auto circle = factory.createShape("circle");
auto rect = factory.createShape("rectangle");
auto circle2 = factory.createShape("circle");
```

### Document Prototype

```cpp
class Document {
private:
    string title;
    vector<string> content;
    map<string, string> metadata;
    
public:
    Document(const string& t) : title(t) {}
    
    void addContent(const string& text) {
        content.push_back(text);
    }
    
    void setMetadata(const string& key, const string& value) {
        metadata[key] = value;
    }
    
    unique_ptr<Document> clone() const {
        return make_unique<Document>(*this);
    }
    
    void show() const {
        cout << "Document: " << title << endl;
        for (const auto& line : content) {
            cout << "  " << line << endl;
        }
        cout << "Metadata: ";
        for (const auto& [k, v] : metadata) {
            cout << k << "=" << v << " ";
        }
        cout << endl;
    }
};

// Create template document
Document templateDoc("Template");
templateDoc.addContent("Introduction...");
templateDoc.addContent("Body...");
templateDoc.setMetadata("author", "Unknown");
templateDoc.setMetadata("version", "1.0");

// Clone for new documents
auto doc1 = templateDoc.clone();
auto doc2 = templateDoc.clone();
auto doc3 = templateDoc.clone();

doc1->show();
doc2->show();
doc3->show();
```

### Configuration Clone

```cpp
class Configuration {
private:
    string appName;
    string version;
    int maxConnections;
    bool debugMode;
    map<string, string> settings;
    
public:
    Configuration(const string& app) 
        : appName(app), maxConnections(100), debugMode(false) {}
    
    void setSetting(const string& key, const string& value) {
        settings[key] = value;
    }
    
    unique_ptr<Configuration> clone() const {
        return make_unique<Configuration>(*this);
    }
    
    void display() const {
        cout << "Config: " << appName << " v" << version;
        cout << " connections=" << maxConnections << endl;
    }
};

// Usage - Create base config, clone for variations
Configuration baseConfig("MyApp");
baseConfig.setSetting("log_level", "INFO");
baseConfig.setSetting("database", "postgres");

auto devConfig = baseConfig.clone();
auto testConfig = baseConfig.clone();
auto prodConfig = baseConfig.clone();

// Modify clones as needed
```

## Advantages

✅ **Performance** - Faster than creating from scratch
✅ **Simplicity** - Simple copying instead of complex initialization
✅ **Polymorphism** - Clone without knowing concrete type
✅ **Decoupling** - Client doesn't know creation details
✅ **Runtime specification** - Type specified at runtime
✅ **Avoids subclassing** - Alternative to factory hierarchies

## Disadvantages

❌ **Circular references** - Complex to handle cycles
❌ **Deep vs shallow** - Must copy correctly
❌ **Clone complexity** - Copying complex objects can be tricky
❌ **Side effects** - Copying may miss some initialization
❌ **Performance** - Copying large objects can be expensive

## When to Use

✅ **Expensive creation** - Object creation is complex/slow
✅ **Similar objects** - Need variations of existing objects
✅ **Runtime type selection** - Type specified at runtime
✅ **Avoid subclassing** - Alternative to factory method
✅ **Independent copies** - Need isolated versions
✅ **Template objects** - Clone and modify templates

## When NOT to Use

❌ Simple objects (just create new ones)
❌ Circular references (hard to handle)
❌ Singleton objects (don't clone)
❌ Objects with external resources (hard to copy)

## Related Patterns

- **Factory Method** - Alternative for object creation
- **Abstract Factory** - Can work with prototypes
- **Memento** - Similar approach to capturing state
- **Singleton** - Opposite (single instance vs clones)

## Best Practices

1. **Deep copy correctly** - Copy all nested objects
2. **Handle cycles** - Detect and handle circular references
3. **Override copy constructor** - Ensure correct deep copy
4. **Mark virtual** - Make clone() virtual
5. **Use smart pointers** - Return unique_ptr from clone()
6. **Test copying** - Ensure clones are independent
7. **Document copy behavior** - Explain what gets copied
8. **Consider performance** - Measure clone vs creation time

## Implementation Checklist

- [ ] Define Prototype interface with clone()
- [ ] Implement clone() in all concrete prototypes
- [ ] Ensure deep copy of all members
- [ ] Return clone as smart pointer
- [ ] Handle circular references if needed
- [ ] Create prototype registry if needed
- [ ] Test clone independence
- [ ] Verify complex member copying
- [ ] Performance test vs creation from scratch
- [ ] Document copy behavior
