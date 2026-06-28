# Chapter 9: Templates - Exercises

## Section 1: Function Templates 🟢

1. Create generic `swap` function template

2. Implement generic `max`/`min` function templates

3. Create function template with multiple parameters

## Section 2: Class Templates 🟡

4. Implement generic `Stack` template class

5. Create generic `Queue` template

6. Design generic `Pair` template with two different types

## Section 3: Template Specialization 🟡

7. Create full template specialization for specific types

8. Implement partial template specialization for pointer types

9. Design specialization for container types

## Section 4: Multiple Template Parameters 🟡

10. Create class template with 2-3 type parameters

11. Implement heterogeneous tuple-like class

12. Design adapter class with template parameters

## Section 5: Default Template Parameters 🟡

13. Create template with default type parameter

14. Implement template with default non-type parameters

15. Design template with mixed defaults

## Section 6: Variadic Templates 🔴

16. Create function template with variable arguments

17. Implement recursive variadic template parameter unpacking

18. Design type-safe printf-like function 🏆

## Section 7: Template Constraints (enable_if) 🔴

19. Use `enable_if` to restrict template to integral types

20. Implement conditional compilation based on traits

21. Create SFINAE examples to handle overload resolution

## Section 8: Partial Specialization 🔴

22. Specialize template for pointer types

23. Implement array specialization

24. Design specialization for STL containers

## Section 9: Template Template Parameters 🔴

25. Create generic container adapter using template template params 🏆

26. Implement algorithm wrapper for different containers

27. Design flexible data structure using templates

## Section 10: Compile-Time Computation 🔴

28. Implement meta-programming with templates

29. Create compile-time factorial calculation

30. Design type selection based on conditions at compile time

---

## Tips for Success

- **Syntax clarity**: Remember `template<typename T>`
- **Instantiation**: Compiler creates code for each type used
- **Specialization**: More specific templates take precedence
- **enable_if**: Use type_traits for conditions
- **Error messages**: Template errors can be verbose
- **Separate compilation**: May need explicit instantiation
- **Code bloat**: Templates generate code for each specialization
- **Variadic unpacking**: Recursion is fundamental approach

## Difficulty Summary

- **Easy (🟢)**: 3 exercises - Function templates, basic syntax
- **Medium (🟡)**: 12 exercises - Class templates, parameters, specialization
- **Hard (🔴)**: 15 exercises - Advanced specialization, constraints, meta-programming

## Challenge Problems 🏆

- **Challenge 1**: Variadic template with type-safe printf
- **Challenge 2**: Template template parameters
- **Challenge 3**: Complex partial specialization patterns

## Expected Time Commitment

- Easy: 10-15 minutes per exercise
- Medium: 20-40 minutes per exercise
- Hard: 40-90 minutes per exercise
- Total: 12-20 hours for all exercises

## Common Pitfalls to Avoid

- Confusing `.` with `->` in nested types (`typename T::type`)
- Forgetting `typename` keyword with dependent names
- SFINAE errors that silently fall through
- Code bloat from excessive template instantiation
- Overly complex specialization rules
- Not testing all possible type combinations
- Template compilation errors hard to debug

## Learning Outcomes

After completing these exercises, you will:
✓ Write generic function and class templates
✓ Use template specialization effectively
✓ Apply constraints with enable_if and type_traits
✓ Handle variadic templates and unpacking
✓ Understand SFINAE and overload resolution
✓ Implement partial specialization patterns
✓ Use template template parameters
✓ Perform compile-time computation
✓ Avoid template-related pitfalls

## C++11 Exercise Example: Function Template

```cpp
template <typename T>
T maximum(T a, T b) {
    return a < b ? b : a;
}
```