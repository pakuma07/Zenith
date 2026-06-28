# Builder Pattern

## Overview

The Builder pattern separates construction of a complex object from its representation, allowing the same construction process to create different representations.

## Intent

- Separate object construction from representation
- Build complex objects step by step
- Create different representations with same process
- Hide internal implementation details
- Make immutable objects
- Improve readability of construction code

## Problem

Creating complex objects with many parameters:
- Large number of constructor parameters
- Some parameters optional
- Parameter combinations invalid
- Object representation may vary
- Construction logic is complex

Result: Unmaintainable, hard-to-read constructors.

## Solution

Define a separate Builder class that:
1. Holds parameter values
2. Provides fluent interface (method chaining)
3. Builds the final object
4. Validates before building

## Structure

```
┌─────────────┐
│   Client    │
└──────┬──────┘
       │
       ▼
┌──────────────────┐      ┌────────────────┐
│Builder (abstract)│──────│ConcreteBuilder │
├──────────────────┤      ├────────────────┤
│+buildPart()      │      │-result         │
│+getProduct()     │      │+buildPart()    │
└──────────────────┘      │+getProduct()   │
                          └────────────────┘
                                 │
                                 ▼
                          ┌─────────────┐
                          │  Product    │
                          ├─────────────┤
                          │ data fields │
                          └─────────────┘
```

## C++11 Implementation

### Basic Builder

```cpp
class House {
private:
    string windows;
    string doors;
    string roof;
    string garage;
    
    // Constructor is private
    House() : windows("none"), doors("none"), 
              roof("none"), garage("none") {}
    
    friend class HouseBuilder;
    
public:
    void show() {
        cout << "House: " << windows << ", " << doors 
             << ", " << roof << ", " << garage << endl;
    }
};

class HouseBuilder {
private:
    unique_ptr<House> house;
    
public:
    HouseBuilder() : house(make_unique<House>()) {}
    
    HouseBuilder& buildWindows(const string& type) {
        house->windows = type;
        return *this;  // For chaining
    }
    
    HouseBuilder& buildDoors(const string& type) {
        house->doors = type;
        return *this;
    }
    
    HouseBuilder& buildRoof(const string& type) {
        house->roof = type;
        return *this;
    }
    
    HouseBuilder& buildGarage(const string& type) {
        house->garage = type;
        return *this;
    }
    
    House build() {
        return *house;
    }
};

// Usage
House house = HouseBuilder()
    .buildWindows("glass")
    .buildDoors("wooden")
    .buildRoof("tiles")
    .buildGarage("concrete")
    .build();

house.show();
```

### Pizza Builder

```cpp
class Pizza {
public:
    string dough;
    string sauce;
    vector<string> toppings;
    bool cheese = false;
    
    void display() {
        cout << "Pizza: " << dough << ", " << sauce;
        cout << ", cheese: " << (cheese ? "yes" : "no");
        cout << ", toppings: ";
        for (auto& t : toppings) cout << t << " ";
        cout << endl;
    }
};

class PizzaBuilder {
private:
    unique_ptr<Pizza> pizza;
    
public:
    PizzaBuilder() : pizza(make_unique<Pizza>()) {}
    
    PizzaBuilder& withDough(const string& d) {
        pizza->dough = d;
        return *this;
    }
    
    PizzaBuilder& withSauce(const string& s) {
        pizza->sauce = s;
        return *this;
    }
    
    PizzaBuilder& addTopping(const string& t) {
        pizza->toppings.push_back(t);
        return *this;
    }
    
    PizzaBuilder& addCheese() {
        pizza->cheese = true;
        return *this;
    }
    
    Pizza build() {
        return *pizza;
    }
};

// Usage
Pizza margherita = PizzaBuilder()
    .withDough("thin")
    .withSauce("tomato")
    .addCheese()
    .addTopping("basil")
    .build();

margherita.display();
```

### Database Connection Builder

```cpp
class DatabaseConnection {
private:
    string host;
    int port;
    string database;
    string username;
    string password;
    int poolSize;
    bool ssl;
    
    DatabaseConnection() : port(0), poolSize(10), ssl(false) {}
    friend class DatabaseBuilder;
    
public:
    void showConfig() {
        cout << "DB Config: " << host << ":" << port 
             << " db=" << database << " user=" << username
             << " pool=" << poolSize << " ssl=" << ssl << endl;
    }
};

class DatabaseBuilder {
private:
    unique_ptr<DatabaseConnection> db;
    
public:
    DatabaseBuilder() : db(make_unique<DatabaseConnection>()) {}
    
    DatabaseBuilder& setHost(const string& h) {
        db->host = h;
        return *this;
    }
    
    DatabaseBuilder& setPort(int p) {
        db->port = p;
        return *this;
    }
    
    DatabaseBuilder& setDatabase(const string& d) {
        db->database = d;
        return *this;
    }
    
    DatabaseBuilder& setUsername(const string& u) {
        db->username = u;
        return *this;
    }
    
    DatabaseBuilder& setPassword(const string& p) {
        db->password = p;
        return *this;
    }
    
    DatabaseBuilder& setPoolSize(int size) {
        db->poolSize = size;
        return *this;
    }
    
    DatabaseBuilder& enableSSL(bool enable = true) {
        db->ssl = enable;
        return *this;
    }
    
    DatabaseConnection build() {
        if (db->host.empty() || db->database.empty()) {
            throw runtime_error("Host and database required");
        }
        return *db;
    }
};

// Usage
auto dbConfig = DatabaseBuilder()
    .setHost("localhost")
    .setPort(5432)
    .setDatabase("myapp")
    .setUsername("admin")
    .setPassword("secret")
    .setPoolSize(20)
    .enableSSL()
    .build();

dbConfig.showConfig();
```

### HTTP Request Builder

```cpp
class HttpRequest {
public:
    string method;
    string url;
    map<string, string> headers;
    string body;
    int timeout;
    bool followRedirects;
    
    void send() {
        cout << method << " " << url << endl;
        cout << "Headers: ";
        for (auto& [k, v] : headers) cout << k << "=" << v << " ";
        cout << endl;
        if (!body.empty()) cout << "Body: " << body << endl;
    }
};

class HttpRequestBuilder {
private:
    unique_ptr<HttpRequest> request;
    
public:
    HttpRequestBuilder() : request(make_unique<HttpRequest>()) {
        request->method = "GET";
        request->timeout = 30;
        request->followRedirects = true;
    }
    
    HttpRequestBuilder& url(const string& u) {
        request->url = u;
        return *this;
    }
    
    HttpRequestBuilder& method(const string& m) {
        request->method = m;
        return *this;
    }
    
    HttpRequestBuilder& header(const string& key, const string& value) {
        request->headers[key] = value;
        return *this;
    }
    
    HttpRequestBuilder& body(const string& b) {
        request->body = b;
        request->method = "POST";
        return *this;
    }
    
    HttpRequestBuilder& timeout(int seconds) {
        request->timeout = seconds;
        return *this;
    }
    
    HttpRequestBuilder& followRedirects(bool follow) {
        request->followRedirects = follow;
        return *this;
    }
    
    HttpRequest build() {
        if (request->url.empty()) {
            throw runtime_error("URL is required");
        }
        return *request;
    }
};

// Usage
auto request = HttpRequestBuilder()
    .url("https://api.example.com/users")
    .method("POST")
    .header("Content-Type", "application/json")
    .header("Authorization", "Bearer token123")
    .body("{\"name\": \"John\"}")
    .timeout(60)
    .build();

request.send();
```

## Advantages

✅ **Readable code** - Method chaining makes construction clear
✅ **Optional parameters** - Add only needed parameters
✅ **Immutability** - Build then freeze object
✅ **Validation** - Validate in build() before returning
✅ **Flexibility** - Different builders for different needs
✅ **Separation** - Construction logic separate from business logic

## Disadvantages

❌ **Extra classes** - More code to write and maintain
❌ **Overhead** - Extra object allocation for builder
❌ **Complexity** - Overkill for simple objects
❌ **Learning curve** - Developers must understand pattern

## When to Use

✅ **Complex objects** - Many parameters or configuration
✅ **Optional parameters** - Some parameters optional
✅ **Different representations** - Same construction, different products
✅ **Immutable objects** - Build then freeze
✅ **Fluent API** - Method chaining for readability
✅ **Parameter validation** - Complex validation logic

## When NOT to Use

❌ Simple objects with few parameters
❌ All parameters required
❌ No optional parameters
❌ Single fixed representation

## Related Patterns

- **Composite** - Often uses builder to construct
- **Prototype** - Similar goal, different approach
- **Strategy** - Can use strategies in builder
- **Factory Method** - Can be used with builder

## Best Practices

1. **Use smart pointers** - Manage builder internal state
2. **Method chaining** - Return *this from setters
3. **Validate in build()** - Check invariants before returning
4. **Provide defaults** - Sensible default values
5. **Immutable product** - Mark fields const if possible
6. **Descriptive names** - with, set, add prefixes
7. **Reset method** - Option to create multiple objects
8. **Thread safety** - If shared across threads

## Implementation Checklist

- [ ] Create complex object class
- [ ] Create builder class with all parameters
- [ ] Provide setter methods returning *this
- [ ] Implement build() method
- [ ] Validate in build() before returning
- [ ] Provide sensible defaults
- [ ] Use smart pointers for state management
- [ ] Document all setter methods
- [ ] Test all parameter combinations
- [ ] Test validation logic
