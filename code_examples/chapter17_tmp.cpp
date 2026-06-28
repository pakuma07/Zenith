// Chapter 17: Template Metaprogramming — Runnable Examples
// Compile: g++ -std=c++11 -Wall -Wextra chapter17_tmp.cpp -o chapter17_tmp

#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
using namespace std;

// ============================================================
// Example 1: Compile-Time Computation with Struct TMP
// ============================================================
template<int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};
template<>
struct Factorial<0> {
    static const int value = 1;
};

template<int N>
struct Fib {
    static const int value = Fib<N-1>::value + Fib<N-2>::value;
};
template<> struct Fib<0> { static const int value = 0; };
template<> struct Fib<1> { static const int value = 1; };

void example1_compiletime() {
    cout << "=== Example 1: Compile-Time Computation ===" << endl;
    cout << "Factorial<5>  = " << Factorial<5>::value  << endl;  // 120
    cout << "Factorial<10> = " << Factorial<10>::value << endl;  // 3628800
    cout << "Fib<10>       = " << Fib<10>::value       << endl;  // 55
    cout << "Fib<15>       = " << Fib<15>::value       << endl;  // 610

    // Use as array size — must be compile-time constant
    int lut[Factorial<5>::value];
    for (int i = 0; i < Factorial<5>::value; ++i) lut[i] = i;
    cout << "Array of size Factorial<5>=" << Factorial<5>::value
         << " created at compile time" << endl;
}

// ============================================================
// Example 2: Type Traits from Scratch
// ============================================================
template<typename T, typename U>
struct is_same { static const bool value = false; };
template<typename T>
struct is_same<T, T> { static const bool value = true; };

template<typename T>
struct is_pointer { static const bool value = false; };
template<typename T>
struct is_pointer<T*> { static const bool value = true; };

template<typename T>
struct remove_const { typedef T type; };
template<typename T>
struct remove_const<const T> { typedef T type; };

template<bool Cond, typename IfTrue, typename IfFalse>
struct conditional { typedef IfTrue type; };
template<typename IfTrue, typename IfFalse>
struct conditional<false, IfTrue, IfFalse> { typedef IfFalse type; };

void example2_type_traits() {
    cout << "\n=== Example 2: Type Traits from Scratch ===" << endl;
    cout << "is_same<int,int>    = " << is_same<int,int>::value    << endl;  // 1
    cout << "is_same<int,float>  = " << is_same<int,float>::value  << endl;  // 0
    cout << "is_pointer<int*>    = " << is_pointer<int*>::value    << endl;  // 1
    cout << "is_pointer<int>     = " << is_pointer<int>::value     << endl;  // 0
    cout << "is_pointer<const int*> = " << is_pointer<const int*>::value << endl; // 1

    remove_const<const int>::type a = 42;    // int
    remove_const<int>::type       b = 10;    // int (no-op)
    cout << "remove_const<const int>::type = " << a << endl;
    cout << "remove_const<int>::type       = " << b << endl;

    // conditional: pick type based on compile-time bool
    conditional<true,  int, double>::type x = 1;
    conditional<false, int, double>::type y = 2.5;
    cout << "conditional<true,int,double>  = " << x << endl;
    cout << "conditional<false,int,double> = " << y << endl;
}

// ============================================================
// Example 3: Tag Dispatch
// ============================================================
struct slow_tag {};
struct fast_tag {};

template<typename Iterator, typename Distance>
void advance_impl(Iterator& it, Distance n, slow_tag) {
    cout << "[slow] stepping " << n << " times" << endl;
    while (n-- > 0) ++it;
}

template<typename Iterator, typename Distance>
void advance_impl(Iterator& it, Distance n, fast_tag) {
    cout << "[fast] jumping " << n << " positions" << endl;
    it += n;
}

template<typename Iterator>
struct dispatch_tag { typedef slow_tag type; };
template<>
struct dispatch_tag<vector<int>::iterator> { typedef fast_tag type; };

template<typename Iterator, typename Distance>
void my_advance(Iterator& it, Distance n) {
    typedef typename dispatch_tag<Iterator>::type tag;
    advance_impl(it, n, tag{});
}

void example3_tag_dispatch() {
    cout << "\n=== Example 3: Tag Dispatch ===" << endl;

    vector<int> v = {10, 20, 30, 40, 50};
    list<int>   l = {10, 20, 30, 40, 50};

    vector<int>::iterator vit = v.begin();
    list<int>::iterator   lit = l.begin();

    my_advance(vit, 3);           // [fast] jumping 3 positions
    my_advance(lit, 3);           // [slow] stepping 3 times
    cout << "vector[3] = " << *vit << endl;  // 40
    cout << "list[3]   = " << *lit << endl;  // 40
}

// ============================================================
// Example 4: Policy-Based Design
// ============================================================
struct AscendingPolicy {
    static bool compare(int a, int b) { return a < b; }
    static const char* name()         { return "Ascending"; }
};
struct DescendingPolicy {
    static bool compare(int a, int b) { return a > b; }
    static const char* name()         { return "Descending"; }
};

struct CheckedPolicy {
    static void check(int idx, int size) {
        if (idx < 0 || idx >= size) throw out_of_range("out of range");
    }
};
struct UncheckedPolicy {
    static void check(int, int) {}
};

template<typename SortP = AscendingPolicy, typename BoundsP = UncheckedPolicy>
class IntArray {
    int data[16];
    int sz;
public:
    IntArray() : sz(0) {}
    void push(int v)  { BoundsP::check(sz, 16); data[sz++] = v; }
    int  get(int i)   { BoundsP::check(i, sz); return data[i]; }
    void sort() {
        for (int i = 1; i < sz; ++i) {
            int key = data[i], j = i - 1;
            while (j >= 0 && SortP::compare(key, data[j])) { data[j+1] = data[j]; --j; }
            data[j+1] = key;
        }
    }
    void print() const {
        cout << SortP::name() << ": ";
        for (int i = 0; i < sz; ++i) cout << data[i] << " ";
        cout << endl;
    }
};

void example4_policy() {
    cout << "\n=== Example 4: Policy-Based Design ===" << endl;

    IntArray<AscendingPolicy,  UncheckedPolicy> asc;
    IntArray<DescendingPolicy, CheckedPolicy>   desc;

    int vals[] = {5, 2, 8, 1, 9, 3};
    for (int v : vals) { asc.push(v); desc.push(v); }

    asc.sort();  asc.print();    // Ascending: 1 2 3 5 8 9
    desc.sort(); desc.print();   // Descending: 9 8 5 3 2 1

    try   { desc.get(99); }
    catch (const out_of_range& e) { cout << "Caught: " << e.what() << endl; }
}

// ============================================================
// Example 5: Expression Templates (simplified)
// ============================================================
template<typename L, typename R>
struct AddExpr {
    const L& left;
    const R& right;
    AddExpr(const L& l, const R& r) : left(l), right(r) {}
    double operator[](int i) const { return left[i] + right[i]; }
};

class Vec4 {
public:
    double d[4];
    Vec4() { d[0]=d[1]=d[2]=d[3]=0.0; }
    Vec4(double a, double b, double c, double e) { d[0]=a; d[1]=b; d[2]=c; d[3]=e; }
    double  operator[](int i) const { return d[i]; }
    double& operator[](int i)       { return d[i]; }

    template<typename Expr>
    Vec4& operator=(const Expr& e) {
        for (int i = 0; i < 4; ++i) d[i] = e[i];
        return *this;
    }
};

AddExpr<Vec4, Vec4> operator+(const Vec4& a, const Vec4& b) {
    return AddExpr<Vec4, Vec4>(a, b);
}
template<typename L>
AddExpr<L, Vec4> operator+(const L& a, const Vec4& b) {
    return AddExpr<L, Vec4>(a, b);
}

void example5_expression_templates() {
    cout << "\n=== Example 5: Expression Templates ===" << endl;
    Vec4 a(1,2,3,4), b(10,20,30,40), c(100,200,300,400), result;

    // a + b + c: single loop, no temporaries
    result = a + b + c;
    for (int i = 0; i < 4; ++i) cout << result[i] << " ";  // 111 222 333 444
    cout << endl;
}

// ============================================================
// Example 6: Advanced constexpr
// ============================================================
constexpr int ce_factorial(int n) { return n <= 1 ? 1 : n * ce_factorial(n-1); }
constexpr int ce_gcd(int a, int b) { return b == 0 ? a : ce_gcd(b, a % b); }
constexpr int ce_pow(int base, int exp) { return exp == 0 ? 1 : base * ce_pow(base, exp-1); }

struct CompilePoint {
    int x, y;
    constexpr CompilePoint(int x, int y) : x(x), y(y) {}
    constexpr int distSq() const { return x*x + y*y; }
};

static_assert(ce_factorial(5) == 120, "factorial<5>");
static_assert(ce_gcd(12, 8)   == 4,   "gcd(12,8)");
static_assert(ce_pow(2, 8)    == 256,  "pow(2,8)");
static_assert(CompilePoint(3,4).distSq() == 25, "distSq");

void example6_constexpr() {
    cout << "\n=== Example 6: Advanced constexpr ===" << endl;
    constexpr int f6 = ce_factorial(6);
    constexpr int g  = ce_gcd(48, 18);
    constexpr int p  = ce_pow(2, 10);
    cout << "factorial(6) = " << f6 << endl;  // 720
    cout << "gcd(48, 18)  = " << g  << endl;  // 6
    cout << "pow(2, 10)   = " << p  << endl;  // 1024

    int runtime_n = 7;
    cout << "factorial(" << runtime_n << ") at runtime = "
         << ce_factorial(runtime_n) << endl;  // 5040

    int buf[ce_pow(2, 4)];   // int buf[16] — size is compile-time constant
    cout << "Array size = " << (int)(sizeof(buf)/sizeof(buf[0])) << endl;
}

// ============================================================
// Example 7: CRTP — Curiously Recurring Template Pattern
// ============================================================
template<typename Derived>
struct Shape {
    double area()      const { return static_cast<const Derived*>(this)->areaImpl(); }
    double perimeter() const { return static_cast<const Derived*>(this)->perimImpl(); }
    void describe() const {
        cout << "Area=" << area() << "  Perimeter=" << perimeter() << endl;
    }
};

struct Circle : Shape<Circle> {
    double r;
    explicit Circle(double radius) : r(radius) {}
    double areaImpl()  const { return 3.14159 * r * r; }
    double perimImpl() const { return 2 * 3.14159 * r; }
};

struct Square : Shape<Square> {
    double side;
    explicit Square(double s) : side(s) {}
    double areaImpl()  const { return side * side; }
    double perimImpl() const { return 4 * side; }
};

// CRTP mixin: inject comparison operators from == and <
template<typename T>
struct Comparable {
    bool operator!=(const T& r) const { return !(static_cast<const T&>(*this)==r); }
    bool operator> (const T& r) const { return r < static_cast<const T&>(*this); }
    bool operator<=(const T& r) const { return !(static_cast<const T&>(*this)>r); }
    bool operator>=(const T& r) const { return !(static_cast<const T&>(*this)<r); }
};

struct Price : Comparable<Price> {
    double v;
    explicit Price(double val) : v(val) {}
    bool operator==(const Price& o) const { return v == o.v; }
    bool operator< (const Price& o) const { return v <  o.v; }
};

void example7_crtp() {
    cout << "\n=== Example 7: CRTP ===" << endl;
    Circle c(5.0);
    Square s(4.0);
    cout << "Circle "; c.describe();   // Area=78.5...  Perimeter=31.4...
    cout << "Square "; s.describe();   // Area=16  Perimeter=16

    Price p1(10.0), p2(20.0);
    cout << "p1 < p2  : " << (p1 < p2)  << endl;  // 1
    cout << "p1 > p2  : " << (p1 > p2)  << endl;  // 0
    cout << "p1 != p2 : " << (p1 != p2) << endl;  // 1
    cout << "p1 <= p2 : " << (p1 <= p2) << endl;  // 1
}

// ============================================================
// Main
// ============================================================
int main() {
    example1_compiletime();
    example2_type_traits();
    example3_tag_dispatch();
    example4_policy();
    example5_expression_templates();
    example6_constexpr();
    example7_crtp();
    return 0;
}
