# Behavioral Patterns

Behavioral patterns deal with object collaboration and the delegation of responsibility. They focus on how objects interact and distribute work.

---

# Observer Pattern

## Overview

The Observer pattern defines a one-to-many dependency between objects so that when one object changes state, all its dependents are notified automatically.

## Intent

- Define loose coupling between subjects and observers
- Notify multiple objects of state changes
- Update dependents automatically
- Create event systems

## C++11 Implementation

```cpp
// Observer interface
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const string& message) = 0;
};

// Subject
class Subject {
private:
    vector<shared_ptr<Observer>> observers;
    string state;
    
public:
    void attach(shared_ptr<Observer> observer) {
        observers.push_back(observer);
    }
    
    void notify() {
        for (auto& obs : observers) {
            obs->update(state);
        }
    }
    
    void setState(const string& newState) {
        state = newState;
        notify();
    }
};

// Concrete observers
class ConsoleObserver : public Observer {
public:
    void update(const string& message) override {
        cout << "Console: " << message << endl;
    }
};

class LoggerObserver : public Observer {
public:
    void update(const string& message) override {
        cout << "[LOG] " << message << endl;
    }
};

// Usage
auto subject = make_unique<Subject>();
auto console = make_shared<ConsoleObserver>();
auto logger = make_shared<LoggerObserver>();

subject->attach(console);
subject->attach(logger);

subject->setState("Temperature changed");
// Output:
// Console: Temperature changed
// [LOG] Temperature changed
```

## When to Use

✅ Multiple objects need to react to state changes
✅ Loosely coupled event systems
✅ MVC architectures
✅ Real-time data updates
✅ Event-driven programming

---

# Strategy Pattern

## Overview

The Strategy pattern defines a family of algorithms, encapsulates each one, and makes them interchangeable.

## C++11 Implementation

```cpp
// Strategy interface
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual void pay(double amount) = 0;
};

// Concrete strategies
class CreditCardStrategy : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paying $" << amount << " by credit card" << endl;
    }
};

class PayPalStrategy : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paying $" << amount << " via PayPal" << endl;
    }
};

class CryptoStrategy : public PaymentStrategy {
public:
    void pay(double amount) override {
        cout << "Paying $" << amount << " in cryptocurrency" << endl;
    }
};

// Context
class ShoppingCart {
private:
    unique_ptr<PaymentStrategy> strategy;
    
public:
    void setPaymentStrategy(unique_ptr<PaymentStrategy> s) {
        strategy = move(s);
    }
    
    void checkout(double total) {
        if (strategy) strategy->pay(total);
    }
};

// Usage
auto cart = make_unique<ShoppingCart>();

cart->setPaymentStrategy(make_unique<CreditCardStrategy>());
cart->checkout(99.99);

cart->setPaymentStrategy(make_unique<PayPalStrategy>());
cart->checkout(49.99);

cart->setPaymentStrategy(make_unique<CryptoStrategy>());
cart->checkout(25.00);
```

## When to Use

✅ Multiple algorithms for a task
✅ Switch algorithms at runtime
✅ Avoid conditional statements
✅ Families of related algorithms
✅ Testing different implementations

---

# Command Pattern

## Overview

The Command pattern encapsulates a request as an object, thereby letting you parameterize clients with different requests, queue requests, and log requests.

## C++11 Implementation

```cpp
// Command interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// Receiver
class Light {
private:
    bool isOn = false;
    
public:
    void on() {
        isOn = true;
        cout << "Light is ON" << endl;
    }
    
    void off() {
        isOn = false;
        cout << "Light is OFF" << endl;
    }
};

// Concrete commands
class TurnOnCommand : public Command {
private:
    shared_ptr<Light> light;
    
public:
    TurnOnCommand(shared_ptr<Light> l) : light(l) {}
    
    void execute() override { light->on(); }
    void undo() override { light->off(); }
};

class TurnOffCommand : public Command {
private:
    shared_ptr<Light> light;
    
public:
    TurnOffCommand(shared_ptr<Light> l) : light(l) {}
    
    void execute() override { light->off(); }
    void undo() override { light->on(); }
};

// Invoker
class RemoteControl {
private:
    vector<unique_ptr<Command>> commandHistory;
    
public:
    void executeCommand(unique_ptr<Command> cmd) {
        cmd->execute();
        commandHistory.push_back(move(cmd));
    }
    
    void undo() {
        if (!commandHistory.empty()) {
            commandHistory.back()->undo();
            commandHistory.pop_back();
        }
    }
};

// Usage
auto light = make_shared<Light>();
auto remote = make_unique<RemoteControl>();

remote->executeCommand(make_unique<TurnOnCommand>(light));   // Light is ON
remote->executeCommand(make_unique<TurnOffCommand>(light));  // Light is OFF
remote->undo();                                              // Light is ON
```

---

# State Pattern

## Overview

The State pattern allows an object to alter its behavior when its internal state changes.

## C++11 Implementation

```cpp
class TrafficLight;

// State interface
class State {
public:
    virtual ~State() = default;
    virtual void handle(TrafficLight& light) = 0;
};

// Concrete states
class RedState : public State {
public:
    void handle(TrafficLight& light) override;
};

class GreenState : public State {
public:
    void handle(TrafficLight& light) override;
};

class YellowState : public State {
public:
    void handle(TrafficLight& light) override;
};

// Context
class TrafficLight {
private:
    unique_ptr<State> currentState;
    
public:
    TrafficLight() : currentState(make_unique<RedState>()) {}
    
    void setState(unique_ptr<State> state) {
        currentState = move(state);
    }
    
    void change() {
        currentState->handle(*this);
    }
};

// Implementations
void RedState::handle(TrafficLight& light) {
    cout << "Red -> Green" << endl;
    light.setState(make_unique<GreenState>());
}

void GreenState::handle(TrafficLight& light) {
    cout << "Green -> Yellow" << endl;
    light.setState(make_unique<YellowState>());
}

void YellowState::handle(TrafficLight& light) {
    cout << "Yellow -> Red" << endl;
    light.setState(make_unique<RedState>());
}
```

---

# Template Method Pattern

## Overview

The Template Method pattern defines the skeleton of an algorithm in a method, deferring some steps to subclasses.

## C++11 Implementation

```cpp
class DataProcessor {
public:
    virtual ~DataProcessor() = default;
    
    // Template method - defines algorithm structure
    void process() {
        readData();
        validate();
        transform();
        save();
    }
    
private:
    virtual void readData() = 0;
    virtual void validate() = 0;
    virtual void transform() = 0;
    virtual void save() = 0;
};

// Concrete implementations
class CSVProcessor : public DataProcessor {
private:
    void readData() override { cout << "Reading CSV..." << endl; }
    void validate() override { cout << "Validating CSV..." << endl; }
    void transform() override { cout << "Transforming CSV..." << endl; }
    void save() override { cout << "Saving CSV..." << endl; }
};

class JSONProcessor : public DataProcessor {
private:
    void readData() override { cout << "Reading JSON..." << endl; }
    void validate() override { cout << "Validating JSON..." << endl; }
    void transform() override { cout << "Transforming JSON..." << endl; }
    void save() override { cout << "Saving JSON..." << endl; }
};

// Usage
CSVProcessor csv;
csv.process();  // Calls all steps in order
```

---

# Chain of Responsibility Pattern

## Overview

The Chain of Responsibility pattern passes a request along a chain of handlers where each handler decides to process it or pass it along.

## C++11 Implementation

```cpp
class Request {
public:
    int level;
    string message;
    Request(int l, const string& m) : level(l), message(m) {}
};

class Handler {
protected:
    shared_ptr<Handler> next;
    
public:
    virtual ~Handler() = default;
    
    void setNext(shared_ptr<Handler> n) { next = n; }
    
    virtual void handle(Request& request) {
        if (next) next->handle(request);
    }
};

class ConsoleHandler : public Handler {
public:
    void handle(Request& request) override {
        if (request.level <= 2) {
            cout << "Console: " << request.message << endl;
        } else {
            Handler::handle(request);
        }
    }
};

class FileHandler : public Handler {
public:
    void handle(Request& request) override {
        if (request.level <= 5) {
            cout << "File: " << request.message << endl;
        } else {
            Handler::handle(request);
        }
    }
};

class DatabaseHandler : public Handler {
public:
    void handle(Request& request) override {
        cout << "Database: " << request.message << endl;
    }
};

// Usage
auto chain = make_shared<ConsoleHandler>();
chain->setNext(make_shared<FileHandler>());
chain->setNext(make_shared<DatabaseHandler>());

Request req1(1, "Low priority");
Request req2(3, "Medium priority");
Request req3(10, "High priority");

chain->handle(req1);
chain->handle(req2);
chain->handle(req3);
```

---

# Iterator Pattern

## Overview

The Iterator pattern provides a way to access elements of a collection sequentially without exposing its underlying representation.

## C++11 Implementation

```cpp
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual bool hasNext() = 0;
    virtual int next() = 0;
};

class Collection {
public:
    virtual ~Collection() = default;
    virtual unique_ptr<Iterator> createIterator() = 0;
};

class ArrayIterator : public Iterator {
private:
    vector<int> items;
    int index = 0;
    
public:
    ArrayIterator(const vector<int>& v) : items(v) {}
    
    bool hasNext() override { return index < items.size(); }
    int next() override { return items[index++]; }
};

class ArrayCollection : public Collection {
private:
    vector<int> items;
    
public:
    void add(int item) { items.push_back(item); }
    
    unique_ptr<Iterator> createIterator() override {
        return make_unique<ArrayIterator>(items);
    }
};

// Usage
auto collection = make_unique<ArrayCollection>();
collection->add(1);
collection->add(2);
collection->add(3);

auto it = collection->createIterator();
while (it->hasNext()) {
    cout << it->next() << " ";
}
```

---

# Memento Pattern

## Overview

The Memento pattern captures and externalizes an object's internal state without violating encapsulation.

## C++11 Implementation

```cpp
class Memento {
private:
    string state;
    friend class TextEditor;
    
    Memento(const string& s) : state(s) {}
    
public:
    string getState() const { return state; }
};

class TextEditor {
private:
    string content;
    
public:
    void type(const string& text) { content += text; }
    
    string getContent() const { return content; }
    
    Memento save() { return Memento(content); }
    
    void restore(const Memento& m) { content = m.getState(); }
};

class History {
private:
    vector<Memento> mementos;
    
public:
    void push(const Memento& m) { mementos.push_back(m); }
    
    Memento pop() {
        if (!mementos.empty()) {
            auto m = mementos.back();
            mementos.pop_back();
            return m;
        }
        return Memento("");
    }
};

// Usage
auto editor = make_unique<TextEditor>();
auto history = make_unique<History>();

editor->type("Hello");
history->push(editor->save());

editor->type(" World");
cout << editor->getContent() << endl;  // Hello World

editor->restore(history->pop());
cout << editor->getContent() << endl;  // Hello
```

---

# Visitor Pattern

## Overview

The Visitor pattern represents an operation to be performed on elements of an object structure, allowing you to define new operations without changing the classes of the elements.

## C++11 Implementation

```cpp
class Visitor;

class Element {
public:
    virtual ~Element() = default;
    virtual void accept(Visitor& v) = 0;
};

class ConcreteElementA : public Element {
public:
    void accept(Visitor& v) override;
};

class ConcreteElementB : public Element {
public:
    void accept(Visitor& v) override;
};

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visitA(ConcreteElementA& e) = 0;
    virtual void visitB(ConcreteElementB& e) = 0;
};

class ConcreteVisitor : public Visitor {
public:
    void visitA(ConcreteElementA& e) override {
        cout << "Visiting A" << endl;
    }
    
    void visitB(ConcreteElementB& e) override {
        cout << "Visiting B" << endl;
    }
};

void ConcreteElementA::accept(Visitor& v) { v.visitA(*this); }
void ConcreteElementB::accept(Visitor& v) { v.visitB(*this); }

// Usage
vector<unique_ptr<Element>> elements;
elements.push_back(make_unique<ConcreteElementA>());
elements.push_back(make_unique<ConcreteElementB>());

auto visitor = make_unique<ConcreteVisitor>();
for (auto& elem : elements) {
    elem->accept(*visitor);
}
```

---

# Mediator Pattern

## Overview

The Mediator pattern defines an object that encapsulates how a set of objects interact.

## C++11 Implementation

```cpp
class Colleague;

class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void send(const string& message, Colleague* colleague) = 0;
};

class Colleague {
protected:
    Mediator* mediator;
    
public:
    Colleague(Mediator* m) : mediator(m) {}
    
    virtual void send(const string& message) = 0;
    virtual void receive(const string& message) = 0;
};

class ConcreteColleague : public Colleague {
private:
    string name;
    
public:
    ConcreteColleague(const string& n, Mediator* m) 
        : Colleague(m), name(n) {}
    
    void send(const string& message) override {
        mediator->send(message, this);
    }
    
    void receive(const string& message) override {
        cout << name << " received: " << message << endl;
    }
};

class ConcreteMediator : public Mediator {
private:
    vector<shared_ptr<Colleague>> colleagues;
    
public:
    void addColleague(shared_ptr<Colleague> c) {
        colleagues.push_back(c);
    }
    
    void send(const string& message, Colleague* sender) override {
        for (auto& c : colleagues) {
            if (c.get() != sender) {
                c->receive(message);
            }
        }
    }
};
```

---

# Interpreter Pattern

## Overview

The Interpreter pattern defines a representation for a grammar and an interpreter to interpret sentences in that language.

## C++11 Implementation

```cpp
class Expression {
public:
    virtual ~Expression() = default;
    virtual int interpret() = 0;
};

class NumberExpression : public Expression {
private:
    int number;
    
public:
    NumberExpression(int n) : number(n) {}
    int interpret() override { return number; }
};

class PlusExpression : public Expression {
private:
    unique_ptr<Expression> left;
    unique_ptr<Expression> right;
    
public:
    PlusExpression(unique_ptr<Expression> l, unique_ptr<Expression> r)
        : left(move(l)), right(move(r)) {}
    
    int interpret() override {
        return left->interpret() + right->interpret();
    }
};

class MinusExpression : public Expression {
private:
    unique_ptr<Expression> left;
    unique_ptr<Expression> right;
    
public:
    MinusExpression(unique_ptr<Expression> l, unique_ptr<Expression> r)
        : left(move(l)), right(move(r)) {}
    
    int interpret() override {
        return left->interpret() - right->interpret();
    }
};

// Usage
auto expr = make_unique<PlusExpression>(
    make_unique<NumberExpression>(5),
    make_unique<MinusExpression>(
        make_unique<NumberExpression>(10),
        make_unique<NumberExpression>(2)
    )
);

cout << "Result: " << expr->interpret() << endl;  // 5 + (10 - 2) = 13
```

---

All behavioral patterns focus on object interaction and responsibility distribution, helping create flexible and maintainable code.
