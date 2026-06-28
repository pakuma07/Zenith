// Chapter 14: File I/O - Runnable Examples
// Compile with: g++ -std=c++11 chapter14_file_io.cpp -o chapter14_file_io

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

// ============================================================
// EXAMPLE 1: Writing Text Files
// ============================================================
void example1_write_text() {
    cout << "\n=== EXAMPLE 1: Writing Text Files ===" << endl;
    
    ofstream outfile("test.txt");
    
    if (!outfile.is_open()) {
        cerr << "Failed to open file" << endl;
        return;
    }
    
    outfile << "Hello, World!" << endl;
    outfile << "Line 2: " << 42 << endl;
    outfile << "Line 3: " << 3.14159 << endl;
    
    outfile.close();
    cout << "File written: test.txt" << endl;
}

// ============================================================
// EXAMPLE 2: Reading Text Files
// ============================================================
void example2_read_text() {
    cout << "\n=== EXAMPLE 2: Reading Text Files ===" << endl;
    
    ifstream infile("test.txt");
    
    if (!infile.is_open()) {
        cerr << "Failed to open file" << endl;
        return;
    }
    
    string line;
    int line_count = 0;
    while (getline(infile, line)) {
        cout << "Line " << ++line_count << ": " << line << endl;
    }
    
    infile.close();
}

// ============================================================
// EXAMPLE 3: Word-by-Word Reading
// ============================================================
void example3_read_words() {
    cout << "\n=== EXAMPLE 3: Word-by-Word Reading ===" << endl;
    
    // Create a file with words
    ofstream out("words.txt");
    out << "apple banana cherry date elderberry fig grape" << endl;
    out.close();
    
    ifstream in("words.txt");
    string word;
    int count = 0;
    
    cout << "Words in file: ";
    while (in >> word) {
        cout << word << " ";
        count++;
    }
    cout << "\nTotal words: " << count << endl;
    
    in.close();
}

// ============================================================
// EXAMPLE 4: Structured Data (CSV-like)
// ============================================================
void example4_structured_data() {
    cout << "\n=== EXAMPLE 4: Structured Data (CSV) ===" << endl;
    
    // Write
    ofstream out("data.csv");
    out << "Name,Age,Score" << endl;
    out << "Alice,25,95.5" << endl;
    out << "Bob,30,87.3" << endl;
    out << "Charlie,28,92.1" << endl;
    out.close();
    
    // Read and parse
    ifstream in("data.csv");
    string header;
    getline(in, header);
    cout << "Header: " << header << endl;
    
    string line;
    while (getline(in, line)) {
        cout << "Record: " << line << endl;
    }
    
    in.close();
}

// ============================================================
// EXAMPLE 5: String Streams
// ============================================================
void example5_string_streams() {
    cout << "\n=== EXAMPLE 5: String Streams ===" << endl;
    
    // Using stringstream for parsing
    string input = "10 20 30 40 50";
    istringstream iss(input);
    
    int num;
    vector<int> numbers;
    
    while (iss >> num) {
        numbers.push_back(num);
    }
    
    cout << "Parsed numbers: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;
    
    // Using stringstream for formatting
    ostringstream oss;
    oss << "Total: " << numbers.size() << " numbers";
    cout << oss.str() << endl;
}

// ============================================================
// EXAMPLE 6: Binary File Operations
// ============================================================
struct Record {
    int id;
    char name[50];
    double salary;
};

void example6_binary_files() {
    cout << "\n=== EXAMPLE 6: Binary File Operations ===" << endl;
    
    // Write binary
    ofstream out("binary.dat", ios::binary);
    
    Record r1 = {1, "Alice", 50000.0};
    Record r2 = {2, "Bob", 60000.0};
    
    out.write((char*)&r1, sizeof(Record));
    out.write((char*)&r2, sizeof(Record));
    out.close();
    
    cout << "Binary file written" << endl;
    
    // Read binary
    ifstream in("binary.dat", ios::binary);
    Record r;
    
    cout << "Binary file contents:" << endl;
    while (in.read((char*)&r, sizeof(Record))) {
        cout << "  ID: " << r.id << ", Name: " << r.name 
             << ", Salary: " << r.salary << endl;
    }
    
    in.close();
}

// ============================================================
// EXAMPLE 7: File Positioning
// ============================================================
void example7_file_positioning() {
    cout << "\n=== EXAMPLE 7: File Positioning ===" << endl;
    
    // Create test file
    ofstream out("position.txt");
    out << "0123456789ABCDEFGHIJ" << endl;
    out.close();
    
    fstream file("position.txt");
    
    // Read character at position 5
    file.seekg(5, ios::beg);
    char ch;
    file.get(ch);
    cout << "Character at position 5: " << ch << endl;
    
    // Get file size
    file.seekg(0, ios::end);
    streampos size = file.tellg();
    cout << "File size: " << size << " bytes" << endl;
    
    // Read last 3 characters
    file.seekg(-3, ios::end);
    char last[4];
    file.read(last, 3);
    last[3] = '\0';
    cout << "Last 3 characters: " << last << endl;
    
    file.close();
}

// ============================================================
// EXAMPLE 8: File Flags and State
// ============================================================
void example8_file_flags() {
    cout << "\n=== EXAMPLE 8: File Flags and State ===" << endl;
    
    ofstream out("flags_test.txt");
    
    cout << "goodbit: " << out.good() << endl;
    cout << "badbit: " << out.bad() << endl;
    cout << "failbit: " << out.fail() << endl;
    cout << "eofbit: " << out.eof() << endl;
    
    out << "Test data" << endl;
    out.close();
    
    cout << "File closed" << endl;
}

// ============================================================
// EXAMPLE 9: Formatting Output
// ============================================================
void example9_formatted_output() {
    cout << "\n=== EXAMPLE 9: Formatted Output ===" << endl;
    
    ofstream out("formatted.txt");
    
    // Integer formatting
    out << "Hex: " << hex << 255 << endl;
    out << "Octal: " << oct << 255 << endl;
    out << "Decimal: " << dec << 255 << endl;
    
    // Float formatting
    out << fixed << setprecision(2);
    out << "Pi: " << 3.14159 << endl;
    out << "E: " << 2.71828 << endl;
    
    // Field width
    out << setw(10) << "Name" << setw(10) << "Age" << endl;
    out << setw(10) << "Alice" << setw(10) << 25 << endl;
    
    out.close();
    
    // Read back
    ifstream in("formatted.txt");
    string line;
    cout << "Formatted output file:" << endl;
    while (getline(in, line)) {
        cout << "  " << line << endl;
    }
    in.close();
}

// ============================================================
// EXAMPLE 10: Error Handling
// ============================================================
void example10_error_handling() {
    cout << "\n=== EXAMPLE 10: Error Handling ===" << endl;
    
    ifstream in("nonexistent_file.txt");
    
    if (!in.is_open()) {
        cerr << "Cannot open file" << endl;
        cerr << "failbit: " << in.fail() << endl;
        cerr << "badbit: " << in.bad() << endl;
    }
    
    // Clear and try again
    in.clear();
    
    ofstream out("success.txt");
    if (out.is_open()) {
        cout << "Successfully opened success.txt for writing" << endl;
        out << "Operation successful" << endl;
        out.close();
    }
}

// ============================================================
// Main Function
// ============================================================
int main() {
    cout << "======================================================" << endl;
    cout << "   CHAPTER 14: FILE I/O (C++11)" << endl;
    cout << "======================================================" << endl;
    
    example1_write_text();
    example2_read_text();
    example3_read_words();
    example4_structured_data();
    example5_string_streams();
    example6_binary_files();
    example7_file_positioning();
    example8_file_flags();
    example9_formatted_output();
    example10_error_handling();
    
    cout << "\n======================================================" << endl;
    cout << "All examples completed!" << endl;
    cout << "======================================================" << endl;
    
    return 0;
}

/*
LEARNING NOTES:

1. **Text File Writing**: ofstream
   - Open file in write mode
   - Use operator<< to output
   - close() or destructor

2. **Text File Reading**: ifstream
   - Open file in read mode
   - getline() for whole lines
   - operator>> for formatted input

3. **Word-by-Word Reading**: operator>>
   - Automatically skips whitespace
   - Returns false at EOF
   - Good for simple parsing

4. **Structured Data**: CSV format
   - Comma-separated values
   - Manual parsing of records
   - Suitable for simple data

5. **String Streams**: In-memory I/O
   - istringstream for parsing
   - ostringstream for building
   - No disk I/O overhead

6. **Binary Files**: Non-text data
   - ios::binary flag required
   - read/write for blocks
   - Precise data storage

7. **File Positioning**: Seek and tell
   - seekg: move read position
   - seekp: move write position
   - tellg/tellp: current position

8. **File State Flags**: Error checking
   - good(): all well
   - bad(): serious error
   - fail(): operation failed
   - eof(): end of file

9. **Formatted Output**: Precision and width
   - setprecision() for decimals
   - setw() for field width
   - hex/oct/dec for bases

10. **Error Handling**: Robust I/O
    - Always check is_open()
    - Test fail/bad bits
    - Use try-catch if needed
*/
