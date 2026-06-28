# Bridge Pattern

## Overview

The Bridge pattern decouples an abstraction from its implementation so that the two can vary independently.

## Intent

- Decouple abstraction from implementation
- Allow independent variation of both
- Avoid permanent binding to implementation
- Reduce class hierarchies
- Share implementations across multiple abstractions

## Problem

You have an abstraction with multiple implementations:
- Window abstraction on Windows, Linux, Mac
- Drawing API with different renderers
- Data storage with multiple backends
- Shape with multiple rendering engines

Creating all combinations creates a class explosion:
```
Shape (abstraction)
├── CircleWindows, CircleLinux, CircleMac
├── RectangleWindows, RectangleLinux, RectangleMac
└── PolygonWindows, PolygonLinux, PolygonMac
```

## Solution

Create two separate hierarchies:
1. **Abstraction** - High-level interface
2. **Implementation** - Low-level, platform-specific

The abstraction delegates to implementation, which can be swapped.

## Structure

```
┌──────────────────┐         ┌─────────────────┐
│  Abstraction     │         │ Implementation  │
├──────────────────┤         ├─────────────────┤
│+ operation()     │────────→│+ operationImpl() │
└──────────────────┘         └─────────────────┘
        △                            △
        │                            │
   ┌────┴────┐              ┌───────┴────────┐
   │Refined   │              │ConcreteImpl A   │
   │Abstract  │              │ConcreteImpl B   │
   └──────────┘              └────────────────┘
```

## C++11 Implementation

### Basic Bridge

```cpp
#include <iostream>
#include <memory>
using namespace std;

// Implementation interface (platform-specific)
class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void renderCircle(float x, float y, float radius) = 0;
    virtual void renderSquare(float x, float y, float size) = 0;
};

// Concrete implementations
class RasterRenderer : public Renderer {
public:
    void renderCircle(float x, float y, float radius) override {
        cout << "Raster: Drawing circle at (" << x << ", " << y 
             << ") with radius " << radius << endl;
    }
    
    void renderSquare(float x, float y, float size) override {
        cout << "Raster: Drawing square at (" << x << ", " << y 
             << ") with size " << size << endl;
    }
};

class VectorRenderer : public Renderer {
public:
    void renderCircle(float x, float y, float radius) override {
        cout << "Vector: Drawing circle at (" << x << ", " << y 
             << ") with radius " << radius << endl;
    }
    
    void renderSquare(float x, float y, float size) override {
        cout << "Vector: Drawing square at (" << x << ", " << y 
             << ") with size " << size << endl;
    }
};

// Abstraction (high-level)
class Shape {
protected:
    shared_ptr<Renderer> renderer;
    
public:
    Shape(shared_ptr<Renderer> r) : renderer(r) {}
    virtual ~Shape() = default;
    
    virtual void draw() = 0;
};

// Refined abstractions
class Circle : public Shape {
private:
    float x, y, radius;
    
public:
    Circle(shared_ptr<Renderer> r, float x, float y, float rad)
        : Shape(r), x(x), y(y), radius(rad) {}
    
    void draw() override {
        renderer->renderCircle(x, y, radius);
    }
};

class Square : public Shape {
private:
    float x, y, size;
    
public:
    Square(shared_ptr<Renderer> r, float x, float y, float s)
        : Shape(r), x(x), y(y), size(s) {}
    
    void draw() override {
        renderer->renderSquare(x, y, size);
    }
};

// Usage
int main() {
    // Create raster renderer
    auto rasterRenderer = make_shared<RasterRenderer>();
    
    Circle rasterCircle(rasterRenderer, 0, 0, 5);
    Square rasterSquare(rasterRenderer, 10, 10, 8);
    
    rasterCircle.draw();
    rasterSquare.draw();
    
    cout << endl;
    
    // Switch to vector renderer
    auto vectorRenderer = make_shared<VectorRenderer>();
    
    Circle vectorCircle(vectorRenderer, 0, 0, 5);
    Square vectorSquare(vectorRenderer, 10, 10, 8);
    
    vectorCircle.draw();
    vectorSquare.draw();
    
    return 0;
}
```

## Advantages

✅ **Decouple abstraction from implementation** - Independent variation
✅ **Reduce class explosion** - No combinatorial class growth
✅ **Runtime switching** - Change implementation at runtime
✅ **Single Responsibility** - Each class has focused responsibility
✅ **Better encapsulation** - Implementation details hidden
✅ **Flexibility** - Add new abstractions and implementations independently

## Disadvantages

❌ **Complexity** - Extra layer of indirection
❌ **Performance** - Overhead from virtual calls
❌ **Learning curve** - Less intuitive than inheritance
❌ **Overkill for simple cases** - May overcomplicate code

## When to Use

✅ Decouple abstraction from implementation required
✅ Multiple implementations of abstraction
✅ Both abstraction and implementation vary
✅ Want to avoid permanent binding
✅ Reduce class hierarchies
✅ Platform-specific implementations needed
✅ Multiple rendering/storage backends
✅ Want to switch implementations at runtime

## When NOT to Use

❌ Single implementation only
❌ Abstraction and implementation don't vary independently
❌ Simple cases (inheritance sufficient)
❌ Performance critical and overhead unacceptable
❌ No need to hide implementation details

## Related Patterns

- **Adapter** - Makes incompatible interfaces work
- **Abstract Factory** - Creates families of related objects
- **Decorator** - Adds functionality dynamically

---

Bridge pattern is powerful for managing complexity when both abstraction and implementation require independent variation.
