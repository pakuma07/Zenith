# Design Patterns Code Examples

Complete, runnable C++11 examples for all major design patterns.

## Compilation

Compile individual examples with:
```bash
g++ -std=c++11 -o example example_name.cpp
./example
```

## Table of Contents

1. **Creational Patterns** - Object creation
2. **Structural Patterns** - Object composition  
3. **Behavioral Patterns** - Object interaction

---

## CREATIONAL PATTERNS

### Singleton Example

```cpp
#include <iostream>
#include <memory>
using namespace std;

// Thread-safe singleton using Meyer's pattern
class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    
    void log(const string& message) {
        cout << "[LOG] " << message << endl;
    }
    
    // Delete copy operations
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
private:
    Logger() {}
};

int main() {
    Logger::getInstance().log("Application started");
    Logger::getInstance().log("Processing data");
    Logger::getInstance().log("Application ended");
    return 0;
}
```

### Factory Method Example

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() = 0;
};

class Circle : public Shape {
public:
    void draw() override { cout << "Drawing Circle" << endl; }
};

class Rectangle : public Shape {
public:
    void draw() override { cout << "Drawing Rectangle" << endl; }
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

int main() {
    auto shapes = {
        ShapeFactory::createShape("circle"),
        ShapeFactory::createShape("rectangle"),
        ShapeFactory::createShape("circle")
    };
    
    for (auto& shape : shapes) {
        if (shape) shape->draw();
    }
    return 0;
}
```

### Builder Example

```cpp
#include <iostream>
#include <string>
using namespace std;

class House {
public:
    string windows;
    string doors;
    string roof;
    string garage;
    
    void show() {
        cout << "House: " << windows << ", " << doors 
             << ", " << roof << ", " << garage << endl;
    }
};

class HouseBuilder {
private:
    shared_ptr<House> house;
    
public:
    HouseBuilder() : house(make_shared<House>()) {}
    
    HouseBuilder& windows(const string& w) {
        house->windows = w;
        return *this;
    }
    
    HouseBuilder& doors(const string& d) {
        house->doors = d;
        return *this;
    }
    
    HouseBuilder& roof(const string& r) {
        house->roof = r;
        return *this;
    }
    
    HouseBuilder& garage(const string& g) {
        house->garage = g;
        return *this;
    }
    
    House build() { return *house; }
};

int main() {
    House myHouse = HouseBuilder()
        .windows("glass")
        .doors("wooden")
        .roof("tiles")
        .garage("concrete")
        .build();
    
    myHouse.show();
    return 0;
}
```

---

## STRUCTURAL PATTERNS

### Adapter Example

```cpp
#include <iostream>
#include <memory>
using namespace std;

class MediaPlayer {
public:
    virtual ~MediaPlayer() = default;
    virtual void play(const string& file) = 0;
};

class VLCPlayer {
public:
    void playVLC(const string& file) {
        cout << "Playing VLC file: " << file << endl;
    }
};

class VLCAdapter : public MediaPlayer {
private:
    unique_ptr<VLCPlayer> vlc;
    
public:
    VLCAdapter() : vlc(make_unique<VLCPlayer>()) {}
    
    void play(const string& file) override {
        vlc->playVLC(file);
    }
};

int main() {
    unique_ptr<MediaPlayer> player = make_unique<VLCAdapter>();
    player->play("movie.mkv");
    return 0;
}
```

### Decorator Example

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Component {
public:
    virtual ~Component() = default;
    virtual void operation() = 0;
};

class ConcreteComponent : public Component {
public:
    void operation() override {
        cout << "Base operation" << endl;
    }
};

class Decorator : public Component {
protected:
    unique_ptr<Component> component;
    
public:
    Decorator(unique_ptr<Component> c) : component(move(c)) {}
};

class ConcreteDecorator : public Decorator {
public:
    ConcreteDecorator(unique_ptr<Component> c) : Decorator(move(c)) {}
    
    void operation() override {
        component->operation();
        cout << " + Extra operation" << endl;
    }
};

int main() {
    auto component = make_unique<ConcreteComponent>();
    component = make_unique<ConcreteDecorator>(move(component));
    component = make_unique<ConcreteDecorator>(move(component));
    
    component->operation();
    return 0;
}
```

---

## BEHAVIORAL PATTERNS

### Observer Example

```cpp
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const string& msg) = 0;
};

class Subject {
private:
    vector<shared_ptr<Observer>> observers;
    string state;
    
public:
    void attach(shared_ptr<Observer> obs) {
        observers.push_back(obs);
    }
    
    void notify() {
        for (auto& obs : observers) {
            obs->update(state);
        }
    }
    
    void setState(const string& s) {
        state = s;
        notify();
    }
};

class ConcreteObserver : public Observer {
private:
    string name;
    
public:
    ConcreteObserver(const string& n) : name(n) {}
    
    void update(const string& msg) override {
        cout << name << " notified: " << msg << endl;
    }
};

int main() {
    auto subject = make_unique<Subject>();
    
    subject->attach(make_shared<ConcreteObserver>("Observer1"));
    subject->attach(make_shared<ConcreteObserver>("Observer2"));
    
    subject->setState("State changed!");
    return 0;
}
```

### Strategy Example

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual void execute() = 0;
};

class StrategyA : public Strategy {
public:
    void execute() override {
        cout << "Executing Strategy A" << endl;
    }
};

class StrategyB : public Strategy {
public:
    void execute() override {
        cout << "Executing Strategy B" << endl;
    }
};

class Context {
private:
    unique_ptr<Strategy> strategy;
    
public:
    void setStrategy(unique_ptr<Strategy> s) {
        strategy = move(s);
    }
    
    void execute() {
        if (strategy) strategy->execute();
    }
};

int main() {
    auto context = make_unique<Context>();
    
    context->setStrategy(make_unique<StrategyA>());
    context->execute();
    
    context->setStrategy(make_unique<StrategyB>());
    context->execute();
    
    return 0;
}
```

### Command Example

```cpp
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class Light {
public:
    void on() { cout << "Light ON" << endl; }
    void off() { cout << "Light OFF" << endl; }
};

class TurnOn : public Command {
private:
    shared_ptr<Light> light;
public:
    TurnOn(shared_ptr<Light> l) : light(l) {}
    void execute() override { light->on(); }
};

class TurnOff : public Command {
private:
    shared_ptr<Light> light;
public:
    TurnOff(shared_ptr<Light> l) : light(l) {}
    void execute() override { light->off(); }
};

class Invoker {
private:
    vector<unique_ptr<Command>> commands;
    
public:
    void addCommand(unique_ptr<Command> cmd) {
        commands.push_back(move(cmd));
    }
    
    void executeAll() {
        for (auto& cmd : commands) {
            cmd->execute();
        }
    }
};

int main() {
    auto light = make_shared<Light>();
    auto invoker = make_unique<Invoker>();
    
    invoker->addCommand(make_unique<TurnOn>(light));
    invoker->addCommand(make_unique<TurnOff>(light));
    invoker->addCommand(make_unique<TurnOn>(light));
    
    invoker->executeAll();
    return 0;
}
```

---

## Additional Pattern Combinations

### Template Method + Strategy

Combine template method for algorithm structure with strategy for flexible implementations.

### Factory Method + Singleton

Use singleton factories to manage creation of objects.

### Decorator + Factory

Use factory to create decorated objects.

### Observer + Mediator

Combine for complex multi-object communication patterns.

---

## Best Practices Summary

1. **Composition over inheritance** - Prefer decorators and wrappers
2. **Dependency injection** - Pass dependencies explicitly
3. **Smart pointers** - Use unique_ptr and shared_ptr
4. **const correctness** - Mark methods const when appropriate
5. **Move semantics** - Use move(  ) for efficiency
6. **Return interfaces** - Return base class pointers/references
7. **Validate early** - Check preconditions in constructors/methods
8. **Single Responsibility** - Each class has one reason to change

---

For more detailed information, see the individual pattern documentation in the creational/, structural/, and behavioral/ directories.
