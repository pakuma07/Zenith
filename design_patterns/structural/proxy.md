# Proxy Pattern

## Overview

The Proxy pattern provides a surrogate or placeholder for another object to control access to it.

## Intent

- Control access to another object
- Add functionality before/after real object
- Lazy initialization
- Logging and security
- Remote object access
- Resource management

## Problem

You need to control or manage access to an object:
- Object is expensive to create (lazy loading)
- Need access control/security
- Want to log access
- Object is remote (network)
- Need caching
- Multiple references to control

## Solution

Create a Proxy object that:
1. Implements the same interface as real object
2. Holds reference to real object
3. Adds extra behavior before/after delegation
4. Appears identical to client

## Structure

```
┌──────────────┐         ┌──────────────┐
│  Subject     │         │   Proxy      │
├──────────────┤         ├──────────────┤
│+request()    │         │+request()    │
└──────────────┘         └──────────────┘
       △                      △
       │                      │
       │           ┌──────────┴─────────┐
       │           │                    │
       │    ┌──────▼────────┐    ┌──────▼────────┐
       └────│ RealSubject   │    │ Virtual/Access│
            │ +request()    │    │ Proxy         │
            └───────────────┘    └───────────────┘
```

## C++11 Implementation

### Virtual Proxy (Lazy Loading)

```cpp
#include <iostream>
#include <memory>
using namespace std;

class Image {
public:
    virtual ~Image() = default;
    virtual void display() = 0;
};

class RealImage : public Image {
private:
    string filename;
    
    void loadImage() {
        cout << "Loading image: " << filename << endl;
        // Expensive operation
    }
    
public:
    RealImage(const string& f) : filename(f) {
        loadImage();
    }
    
    void display() override {
        cout << "Displaying: " << filename << endl;
    }
};

class ProxyImage : public Image {
private:
    string filename;
    mutable unique_ptr<RealImage> realImage;
    
public:
    ProxyImage(const string& f) : filename(f), realImage(nullptr) {}
    
    void display() override {
        if (!realImage) {
            realImage = make_unique<RealImage>(filename);
        }
        realImage->display();
    }
};

// Usage
int main() {
    vector<unique_ptr<Image>> images;
    
    // Only create proxy, real image not loaded yet
    images.push_back(make_unique<ProxyImage>("large_image_1.jpg"));
    images.push_back(make_unique<ProxyImage>("large_image_2.jpg"));
    images.push_back(make_unique<ProxyImage>("large_image_3.jpg"));
    
    cout << "Images created but not loaded" << endl << endl;
    
    // Real images loaded only when displayed
    cout << "Displaying images:" << endl;
    for (auto& img : images) {
        img->display();
    }
    
    return 0;
}
```

### Protection Proxy

```cpp
class BankAccount {
public:
    virtual ~BankAccount() = default;
    virtual void withdraw(double amount) = 0;
    virtual void deposit(double amount) = 0;
    virtual double balance() = 0;
};

class RealBankAccount : public BankAccount {
private:
    double balance_;
    
public:
    RealBankAccount(double initial) : balance_(initial) {}
    
    void withdraw(double amount) override {
        balance_ -= amount;
        cout << "Withdrawn: $" << amount << endl;
    }
    
    void deposit(double amount) override {
        balance_ += amount;
        cout << "Deposited: $" << amount << endl;
    }
    
    double balance() override {
        return balance_;
    }
};

class AccountProxy : public BankAccount {
private:
    unique_ptr<RealBankAccount> realAccount;
    string userPassword;
    string providedPassword;
    
public:
    AccountProxy(const string& password, double initial)
        : userPassword(password), providedPassword("") {
        realAccount = make_unique<RealBankAccount>(initial);
    }
    
    void setPassword(const string& pwd) {
        providedPassword = pwd;
    }
    
    bool authenticate() {
        return providedPassword == userPassword;
    }
    
    void withdraw(double amount) override {
        if (!authenticate()) {
            cout << "Access denied!" << endl;
            return;
        }
        realAccount->withdraw(amount);
    }
    
    void deposit(double amount) override {
        if (!authenticate()) {
            cout << "Access denied!" << endl;
            return;
        }
        realAccount->deposit(amount);
    }
    
    double balance() override {
        if (!authenticate()) {
            cout << "Access denied!" << endl;
            return -1;
        }
        return realAccount->balance();
    }
};
```

### Logging Proxy

```cpp
class Document {
public:
    virtual ~Document() = default;
    virtual void open() = 0;
    virtual void close() = 0;
    virtual void read() = 0;
};

class RealDocument : public Document {
private:
    string filename;
    
public:
    RealDocument(const string& f) : filename(f) {}
    
    void open() override { cout << "Opening: " << filename << endl; }
    void close() override { cout << "Closing: " << filename << endl; }
    void read() override { cout << "Reading: " << filename << endl; }
};

class LoggingDocumentProxy : public Document {
private:
    unique_ptr<RealDocument> document;
    string username;
    
public:
    LoggingDocumentProxy(const string& fn, const string& user)
        : document(make_unique<RealDocument>(fn)), username(user) {}
    
    void open() override {
        cout << "[LOG] " << username << " opened document" << endl;
        document->open();
    }
    
    void close() override {
        cout << "[LOG] " << username << " closed document" << endl;
        document->close();
    }
    
    void read() override {
        cout << "[LOG] " << username << " read document" << endl;
        document->read();
    }
};
```

### Caching Proxy

```cpp
class DataService {
public:
    virtual ~DataService() = default;
    virtual string getData(int id) = 0;
};

class RealDataService : public DataService {
public:
    string getData(int id) override {
        cout << "Fetching from database..." << endl;
        return "Data for ID: " + to_string(id);
    }
};

class CachingDataService : public DataService {
private:
    unique_ptr<RealDataService> service;
    map<int, string> cache;
    
public:
    CachingDataService() : service(make_unique<RealDataService>()) {}
    
    string getData(int id) override {
        if (cache.find(id) != cache.end()) {
            cout << "Cache hit for ID: " << id << endl;
            return cache[id];
        }
        
        cout << "Cache miss for ID: " << id << endl;
        string data = service->getData(id);
        cache[id] = data;
        return data;
    }
};
```

## Advantages

✅ **Lazy initialization** - Create expensive objects only when needed
✅ **Access control** - Regulate who can access object
✅ **Logging/audit** - Track object access
✅ **Caching** - Avoid repeated expensive operations
✅ **Remote access** - Access objects across network
✅ **Same interface** - Transparent to clients
✅ **Flexible** - Add behavior without modifying real object

## Disadvantages

❌ **Complexity** - Extra layer of indirection
❌ **Performance** - Overhead from proxy operations
❌ **Thread safety** - Need synchronization for shared objects
❌ **Debugging** - Harder to trace through proxy

## Variations

| Type | Purpose | Example |
|------|---------|---------|
| **Virtual** | Lazy initialization | Load expensive images on demand |
| **Protection** | Access control | Bank account access |
| **Remote** | Network access | RPC stubs, web services |
| **Logging** | Track operations | Audit trails |
| **Caching** | Performance | Database result caching |
| **Smart** | Reference counting | Smart pointers |

## When to Use

✅ Object creation is expensive
✅ Need access control/security
✅ Want to log/audit operations
✅ Accessing remote objects
✅ Need caching layer
✅ Lazy initialization needed
✅ Multiple references need coordination
✅ Want to add behavior transparently

## When NOT to Use

❌ Simple, cheap objects
❌ Direct access preferred
❌ Performance critical (overhead)
❌ Single simple operation
❌ No additional behavior needed

## Related Patterns

- **Decorator** - Similar structure, different intent (adds behavior)
- **Adapter** - Converts interface (Proxy maintains interface)
- **Facade** - Simplifies complex subsystem (Proxy controls access)

---

## Comparison: Proxy vs Decorator

| Aspect | Proxy | Decorator |
|--------|-------|-----------|
| **Intent** | Control access | Add behavior |
| **Responsibility** | Replace object | Enhance object |
| **Lifetime** | Same as real object | Wraps real object |
| **Interface** | Same as subject | Same as component |
| **Purpose** | Access control | Feature addition |

---

Proxy pattern is essential for lazy loading, access control, and adding operations transparently. Use it when you need to manage how clients access objects.
