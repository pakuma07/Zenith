# Flyweight Pattern

## Overview

The Flyweight pattern uses sharing to support large numbers of fine-grained objects efficiently by sharing common state.

## Intent

- Share objects to use less memory
- Support large numbers of similar objects
- Extract intrinsic (shared) state
- Separate from extrinsic (unique) state
- Reduce memory footprint
- Improve performance

## Problem

You need many similar objects:
- Text editor with millions of character objects
- Game with thousands of particle objects
- Drawing application with thousands of shapes
- Each object consumes significant memory

Creating one object per entity is memory-intensive:
```
Text: "Hello World" = 11 Character objects
Each Character object: font, style, color, etc.
Total memory: 11 * (font + style + color + ...) bytes
```

## Solution

Share intrinsic state (font, style) across objects while keeping extrinsic state (position, value) separate:

```
Intrinsic State (shared):
- Font
- Style
- Color

Extrinsic State (unique):
- Character value
- Position
- Index
```

## Structure

```
┌──────────────────┐      ┌──────────────────┐
│   FlyweightFactory       │   Flyweight      │
├──────────────────┤      ├──────────────────┤
│+getFlyweight()   │      │+operation()      │
└──────────────────┘      └──────────────────┘
         │                        △
         │                        │
         └────────┬───────────────┘
                  │
            ┌─────▼──────┐
            │ConcreteFly │
            │weight      │
            └────────────┘
```

## C++11 Implementation

### Character Flyweight

```cpp
#include <iostream>
#include <memory>
#include <map>
using namespace std;

// Intrinsic state (shared)
class Character {
private:
    char value;
    string font;
    int size;
    
public:
    Character(char v, const string& f, int s) 
        : value(v), font(f), size(s) {}
    
    void render(int x, int y) const {
        cout << "Char '" << value << "' at (" << x << "," << y 
             << ") font:" << font << " size:" << size << endl;
    }
};

// Factory for managing flyweights
class CharacterFactory {
private:
    map<char, shared_ptr<Character>> flyweights;
    string defaultFont = "Arial";
    int defaultSize = 12;
    
public:
    shared_ptr<Character> getCharacter(char c) {
        if (flyweights.find(c) == flyweights.end()) {
            flyweights[c] = make_shared<Character>(c, defaultFont, defaultSize);
        }
        return flyweights[c];
    }
    
    int getPoolSize() const {
        return flyweights.size();
    }
};

// Extrinsic state (unique position, etc)
struct CharacterPosition {
    int x, y;
    shared_ptr<Character> character;
};

// Client: Text document
class TextDocument {
private:
    vector<CharacterPosition> characters;
    CharacterFactory factory;
    
public:
    void addCharacter(char c, int x, int y) {
        CharacterPosition cp;
        cp.character = factory.getCharacter(c);
        cp.x = x;
        cp.y = y;
        characters.push_back(cp);
    }
    
    void render() {
        for (const auto& cp : characters) {
            cp.character->render(cp.x, cp.y);
        }
    }
    
    int getCharacterPoolSize() const {
        return factory.getPoolSize();
    }
};

// Usage
int main() {
    TextDocument doc;
    
    // Add many characters
    string text = "Hello World";
    int x = 0;
    for (char c : text) {
        doc.addCharacter(c, x++, 0);
    }
    
    cout << "Total characters: " << text.size() << endl;
    cout << "Unique characters (pool size): " << doc.getCharacterPoolSize() << endl;
    cout << "Memory saved: " << (text.size() - doc.getCharacterPoolSize()) 
         << " character objects" << endl << endl;
    
    doc.render();
    
    return 0;
}
```

### Game Particle Flyweight

```cpp
// Intrinsic state
class ParticleType {
private:
    string name;
    string texture;
    int color;
    float mass;
    
public:
    ParticleType(const string& n, const string& t, int c, float m)
        : name(n), texture(t), color(c), mass(m) {}
    
    void render(float x, float y) const {
        cout << "Rendering " << name << " at (" << x << "," << y << ")" << endl;
    }
};

// Factory
class ParticleFactory {
private:
    map<string, shared_ptr<ParticleType>> types;
    
public:
    shared_ptr<ParticleType> getParticleType(
        const string& name, const string& texture, int color, float mass) {
        
        if (types.find(name) == types.end()) {
            types[name] = make_shared<ParticleType>(name, texture, color, mass);
        }
        return types[name];
    }
    
    int getTypeCount() const { return types.size(); }
};

// Extrinsic state
struct Particle {
    float x, y;
    float velocityX, velocityY;
    shared_ptr<ParticleType> type;
};

// Game world
class ParticleSystem {
private:
    vector<Particle> particles;
    ParticleFactory factory;
    
public:
    void emitParticles(const string& typeName, int count, float x, float y) {
        auto type = factory.getParticleType(
            typeName, "particle.png", 0xFF0000, 1.0f);
        
        for (int i = 0; i < count; ++i) {
            Particle p;
            p.type = type;
            p.x = x + (rand() % 10);
            p.y = y + (rand() % 10);
            p.velocityX = (rand() % 5) - 2.5f;
            p.velocityY = (rand() % 5) - 2.5f;
            particles.push_back(p);
        }
    }
    
    void render() {
        for (const auto& p : particles) {
            p.type->render(p.x, p.y);
        }
    }
    
    int getParticleCount() const { return particles.size(); }
    int getTypeCount() const { return factory.getTypeCount(); }
};
```

### Tree Node Flyweight

```cpp
// Intrinsic state: rendering properties
class TreeNodeType {
private:
    string name;
    string iconFile;
    int color;
    
public:
    TreeNodeType(const string& n, const string& i, int c)
        : name(n), iconFile(i), color(c) {}
    
    void displayIcon() const {
        cout << "Icon: " << iconFile << endl;
    }
};

// Factory
class TreeNodeTypeFactory {
private:
    map<string, shared_ptr<TreeNodeType>> types;
    
public:
    shared_ptr<TreeNodeType> getNodeType(const string& name) {
        if (types.find(name) == types.end()) {
            types[name] = make_shared<TreeNodeType>(
                name, name + ".png", 0x000000);
        }
        return types[name];
    }
};

// Extrinsic state: tree structure
class TreeNode {
private:
    string value;
    shared_ptr<TreeNodeType> type;
    vector<unique_ptr<TreeNode>> children;
    
public:
    TreeNode(const string& v, shared_ptr<TreeNodeType> t)
        : value(v), type(t) {}
    
    void addChild(unique_ptr<TreeNode> child) {
        children.push_back(move(child));
    }
    
    void display(int depth = 0) const {
        cout << string(depth * 2, ' ') << value << " - ";
        type->displayIcon();
        for (const auto& child : children) {
            child->display(depth + 1);
        }
    }
};

// Usage
TreeNodeTypeFactory typeFactory;
auto folderType = typeFactory.getNodeType("Folder");
auto fileType = typeFactory.getNodeType("File");

auto root = make_unique<TreeNode>("root", folderType);
auto subdir = make_unique<TreeNode>("subdir", folderType);
subdir->addChild(make_unique<TreeNode>("file.txt", fileType));
root->addChild(move(subdir));
```

## Advantages

✅ **Reduce memory** - Dramatically reduce memory consumption
✅ **Improve performance** - Fewer objects to manage
✅ **Share state** - Common state shared across many objects
✅ **Scalability** - Handle thousands/millions of objects
✅ **Transparency** - Works transparently to clients
✅ **Thread safety** - Immutable flyweights are naturally thread-safe

## Disadvantages

❌ **Complexity** - Complex code with intrinsic/extrinsic state
❌ **Thread safety** - Shared state requires synchronization
❌ **CPU overhead** - Lookup in factory map has cost
❌ **Learning curve** - Understand intrinsic vs extrinsic
❌ **Premature optimization** - Only use when memory is issue

## When to Use

✅ Many similar objects consuming memory
✅ Can extract shared intrinsic state
✅ Object identity not critical
✅ Memory usage is bottleneck
✅ Thousands of objects needed
✅ Objects are mostly read-only
✅ Fine-grained objects (characters, particles)

## When NOT to Use

❌ Few objects needed
❌ Cannot extract intrinsic state
❌ Memory not a concern
❌ Objects need unique identity
❌ Mutable shared state required
❌ Simple solution sufficient

## Memory Analysis

### Without Flyweight
```
Object: 100 bytes
Objects needed: 10,000
Total: 100 * 10,000 = 1,000,000 bytes = 1 MB

Without unique positions: 976,562 bytes saved!
```

### With Flyweight
```
Shared type: 100 bytes (1 copy)
Position info: 8 bytes (10,000 copies)
Total: 100 + (8 * 10,000) = 80,100 bytes = 0.08 MB
```

## Related Patterns

- **Factory** - Creates and manages flyweights
- **Composite** - Flyweights often used in composite structures
- **Singleton** - Flyweight factory is often singleton

## Best Practices

1. **Identify intrinsic state** - What's truly shared?
2. **Immutable flyweights** - Don't modify shared state
3. **Use factory** - Centralize flyweight creation
4. **Thread safety** - Synchronize factory access
5. **Measure first** - Profile before optimizing
6. **Document clearly** - Explain intrinsic vs extrinsic
7. **Separate concerns** - Keep state separation clean

## Implementation Checklist

- [ ] Identify intrinsic state (shared)
- [ ] Identify extrinsic state (unique)
- [ ] Create Flyweight class (intrinsic only)
- [ ] Create Flyweight factory
- [ ] Factory caches flyweights
- [ ] Client holds references to flyweight
- [ ] Client holds extrinsic state
- [ ] Flyweights are immutable
- [ ] Test memory usage
- [ ] Verify correctness with/without
- [ ] Document intrinsic vs extrinsic
- [ ] Measure performance improvement

---

Flyweight pattern is essential when dealing with large numbers of similar objects where memory is a constraint. Use it when you can clearly separate shared and unique state.
