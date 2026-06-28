# Chapter 14: File I/O - Exercises

## Section 1: Text File Writing 🟢

1. Write text to file using ofstream

2. Write multiple lines with line numbers

3. Write structured text with formatting

## Section 2: Text File Reading 🟡

4. Read entire file line by line

5. Read file word by word

6. Count lines and words in file

## Section 3: File Parsing 🟡

7. Parse CSV-like format

8. Extract specific fields from records

9. Validate data during parsing

## Section 4: String Streams 🟡

10. Parse string using istringstream

11. Format output using ostringstream

12. Implement custom string parser

## Section 5: Binary Files 🟡

13. Write binary data structure to file

14. Read binary file and reconstruct objects

15. Implement binary serialization

## Section 6: File Positioning 🟡

16. Seek to specific position in file

17. Read file in reverse

18. Implement random access to records

## Section 7: File State Checking 🟡

19. Check file state (good, eof, fail, bad)

20. Implement error recovery

21. Validate file before processing

## Section 8: Advanced Formatting 🟡

22. Format output with width and precision

23. Write formatted table to file

24. Implement custom formatting rules

## Section 9: Combining Streams 🔴

25. Read from file and write to another 🏆

26. Implement data transformation pipeline

27. Create log file with timestamps

## Section 10: Real-World File Operations 🔴

28. Implement configuration file reader

29. Create data export system (CSV, binary)

30. Design robust file I/O wrapper class

---

## Tips for Success

- **Check is_open()**: Always verify file opened
- **getline()**: For whole lines with newlines
- **operator>>**: For formatted input
- **Binary mode**: Use ios::binary for non-text
- **Positioning**: seekg for read, seekp for write
- **State flags**: Check good(), fail(), bad()
- **Formatting**: setw(), setprecision(), fixed
- **RAII**: File closes in destructor

## Difficulty Summary

- **Easy (🟢)**: 3 exercises - Basic text I/O
- **Medium (🟡)**: 18 exercises - Parsing, streams, positioning, formatting
- **Hard (🔴)**: 9 exercises - Transformation pipelines, robust systems

## Challenge Problems 🏆

- **Challenge 1**: Complex file transformation
- **Challenge 2**: Configuration file system
- **Challenge 3**: Robust error handling

## Expected Time Commitment

- Easy: 10-15 minutes per exercise
- Medium: 20-40 minutes per exercise
- Hard: 30-60 minutes per exercise
- Total: 8-15 hours for all exercises

## Common Pitfalls to Avoid

- Not checking if file opened
- Wrong stream object (cin vs ifstream)
- Forgetting binary mode for non-text
- Iterator invalidation with streams
- Position confusion (read vs write)
- Formatting state persistence
- Not flushing output
- Encoding issues with text

## Learning Outcomes

After completing these exercises, you will:
✓ Read and write text files effectively
✓ Parse structured data reliably
✓ Use string streams for formatting
✓ Work with binary files
✓ Implement random file access
✓ Check and handle file errors
✓ Format output professionally
✓ Create robust file I/O systems
✓ Handle real-world file scenarios

## C++11 Exercise Example: Thread Launch

```cpp
#include <thread>
#include <iostream>
using namespace std;

void worker() { cout << "worker running\n"; }
```