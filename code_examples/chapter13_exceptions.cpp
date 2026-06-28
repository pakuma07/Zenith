// Chapter 13: Exception Handling - Runnable Examples
// Compile with: g++ -std=c++11 chapter13_exceptions.cpp -o chapter13_exceptions

#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
using namespace std;

// ============================================================
// EXAMPLE 1: Basic Try-Catch
// ============================================================
void example1_basic_try_catch() {
    cout << "\n=== EXAMPLE 1: Basic Try-Catch ===" << endl;
    
    try {
        int arr[5] = {1, 2, 3, 4, 5};
        int index = 10;
        
        if (index >= 5) throw out_of_range("Index out of bounds!");
        cout << "arr[" << index << "] = " << arr[index] << endl;
    }
    catch (out_of_range& e) {
        cout << "Caught exception: " << e.what() << endl;
    }
}

// ============================================================
// EXAMPLE 2: Multiple Catch Blocks
// ============================================================
void divide(int a, int b) {
    if (b == 0) throw invalid_argument("Division by zero!");
    cout << a << " / " << b << " = " << (a / b) << endl;
}

void example2_multiple_catch() {
    cout << "\n=== EXAMPLE 2: Multiple Catch Blocks ===" << endl;
    
    try {
        divide(10, 0);
    }
    catch (invalid_argument& e) {
        cout << "Invalid argument: " << e.what() << endl;
    }
    catch (exception& e) {
        cout << "General exception: " << e.what() << endl;
    }
}

// ============================================================
// EXAMPLE 3: Custom Exceptions
// ============================================================
class BankException : public exception {
private:
    string message;
public:
    BankException(const string& msg) : message(msg) {}
    virtual const char* what() const noexcept {
        return message.c_str();
    }
};

class InsufficientFundsException : public BankException {
public:
    InsufficientFundsException(double amount, double balance)
        : BankException("Insufficient funds: need " + to_string(amount) + 
                       ", have " + to_string(balance)) {}
};

void example3_custom_exceptions() {
    cout << "\n=== EXAMPLE 3: Custom Exceptions ===" << endl;
    
    double balance = 100.0;
    double amount = 150.0;
    
    try {
        if (amount > balance) {
            throw InsufficientFundsException(amount, balance);
        }
        balance -= amount;
    }
    catch (InsufficientFundsException& e) {
        cout << "Bank error: " << e.what() << endl;
    }
}

// ============================================================
// EXAMPLE 4: Exception Hierarchy
// ============================================================
void example4_exception_hierarchy() {
    cout << "\n=== EXAMPLE 4: Exception Hierarchy ===" << endl;
    
    try {
        // Throw different types
        throw runtime_error("Runtime error example");
    }
    catch (const logic_error& e) {
        cout << "Logic error: " << e.what() << endl;
    }
    catch (const runtime_error& e) {
        cout << "Runtime error: " << e.what() << endl;
    }
    catch (const exception& e) {
        cout << "Generic exception: " << e.what() << endl;
    }
}

// ============================================================
// EXAMPLE 5: Re-throwing Exceptions
// ============================================================
void processData(int value) {
    try {
        if (value < 0) throw invalid_argument("Negative value");
    }
    catch (const exception& e) {
        cout << "processData caught: " << e.what() << endl;
        throw;  // Re-throw to caller
    }
}

void example5_rethrow() {
    cout << "\n=== EXAMPLE 5: Re-throwing Exceptions ===" << endl;
    
    try {
        processData(-5);
    }
    catch (const exception& e) {
        cout << "Caller caught: " << e.what() << endl;
    }
}

// ============================================================
// EXAMPLE 6: RAII with Exceptions (Resource Safety)
// ============================================================
class TransactionLog {
private:
    string operation;
    bool committed;
public:
    TransactionLog(const string& op) : operation(op), committed(false) {
        cout << "Starting transaction: " << operation << endl;
    }
    
    void commit() {
        committed = true;
        cout << "Transaction committed: " << operation << endl;
    }
    
    ~TransactionLog() {
        if (!committed) {
            cout << "Transaction rolled back: " << operation << endl;
        }
    }
};

void example6_raii_exceptions() {
    cout << "\n=== EXAMPLE 6: RAII with Exceptions ===" << endl;
    
    try {
        TransactionLog log("Database update");
        // Simulate work...
        if (true) throw runtime_error("Update failed!");
        log.commit();
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

// ============================================================
// EXAMPLE 7: Noexcept Specification
// ============================================================
void safeFunction() noexcept {
    cout << "This function won't throw" << endl;
}

void riskyFunction() {  // May throw
    throw runtime_error("Something went wrong");
}

void example7_noexcept() {
    cout << "\n=== EXAMPLE 7: Noexcept Specification ===" << endl;
    
    safeFunction();
    cout << "noexcept(safeFunction) = " << noexcept(safeFunction()) << endl;
    cout << "noexcept(riskyFunction) = " << noexcept(riskyFunction()) << endl;
    
    try {
        riskyFunction();
    }
    catch (const exception& e) {
        cout << "Caught: " << e.what() << endl;
    }
}

// ============================================================
// EXAMPLE 8: Exception Safety in Constructors
// ============================================================
class SafeObject {
private:
    int* data;
    int size;
public:
    SafeObject(int sz) : size(sz), data(nullptr) {
        data = new int[sz];
        if (!data) throw bad_alloc();
        cout << "SafeObject constructed" << endl;
    }
    
    ~SafeObject() {
        delete[] data;
        cout << "SafeObject destroyed" << endl;
    }
};

void example8_exception_safety() {
    cout << "\n=== EXAMPLE 8: Exception Safety ===" << endl;
    
    try {
        SafeObject obj(1000000);
        cout << "Object created successfully" << endl;
    }
    catch (const bad_alloc& e) {
        cout << "Memory allocation failed: " << e.what() << endl;
    }
}

// ============================================================
// EXAMPLE 9: Exception in Destructors
// ============================================================
class SafeResource {
public:
    ~SafeResource() noexcept {
        // Destructors should NOT throw
        cout << "Cleaning up safely" << endl;
    }
};

void example9_safe_destructors() {
    cout << "\n=== EXAMPLE 9: Safe Destructors ===" << endl;
    
    try {
        SafeResource res;
        throw runtime_error("Error during execution");
    }
    catch (const exception& e) {
        cout << "Caught: " << e.what() << endl;
        cout << "Destructor was called safely" << endl;
    }
}

// ============================================================
// EXAMPLE 10: Exception Neutral Code
// ============================================================
void processContainer(vector<int>& vec) {
    try {
        for (int& val : vec) {
            if (val < 0) throw invalid_argument("Negative value");
            val *= 2;
        }
    }
    catch (const exception& e) {
        cout << "Processing error: " << e.what() << endl;
        throw;  // Propagate to caller
    }
}

void example10_exception_neutral() {
    cout << "\n=== EXAMPLE 10: Exception Neutral Code ===" << endl;
    
    vector<int> data = {1, 2, 3, -4, 5};
    
    try {
        processContainer(data);
    }
    catch (const exception& e) {
        cout << "Error handled by caller" << endl;
    }
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 13: EXCEPTION HANDLING (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_basic_try_catch();
    example2_multiple_catch();
    example3_custom_exceptions();
    example4_exception_hierarchy();
    example5_rethrow();
    example6_raii_exceptions();
    example7_noexcept();
    example8_exception_safety();
    example9_safe_destructors();
    example10_exception_neutral();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Try-Catch**: Basic exception handling
   - try block contains risky code
   - catch block handles exception
   - Control jumps to catch

2. **Multiple Catch Blocks**: Different handlers
   - Match specific exception types
   - More specific catches first
   - Generic catch at end

3. **Custom Exceptions**: Domain-specific errors
   - Inherit from exception
   - what() returns message
   - Better error handling

4. **Exception Hierarchy**: Standard library structure
   - exception base class
   - logic_error, runtime_error
   - Standard derived classes

5. **Re-throwing**: Propagate up stack
   - throw; without arguments
   - Maintains original exception
   - Let higher level handle

6. **RAII + Exceptions**: Resource safety
   - Destructors always called
   - Cleanup happens automatically
   - Even with exceptions

7. **Noexcept**: Function guarantees
   - Promise not to throw
   - Compiler optimization hint
   - Check at compile time

8. **Constructor Safety**: Validate early
   - Throw if invalid state
   - Bad_alloc for memory errors
   - Object either valid or failed

9. **Destructor Safety**: Never throw
   - Mark with noexcept
   - Always clean up
   - Avoid double exception

10. **Exception Neutral**: Propagate safely
    - Catch and re-throw
    - Don't suppress errors
    - Let caller decide
*/
