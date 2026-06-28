# 09 — Computational Geometry

Use `long long` for integer coordinates whenever possible to avoid floating-point errors.

---

## 9.1 Point and Vector

```cpp
typedef long long ll;

struct Point {
    ll x, y;
    Point(ll x = 0, ll y = 0) : x(x), y(y) {}

    Point operator+(const Point& o) const { return {x+o.x, y+o.y}; }
    Point operator-(const Point& o) const { return {x-o.x, y-o.y}; }
    Point operator*(ll t)           const { return {x*t,   y*t}; }

    bool operator<(const Point& o)  const { return x < o.x || (x == o.x && y < o.y); }
    bool operator==(const Point& o) const { return x == o.x && y == o.y; }

    ll dot(const Point& o)   const { return x*o.x + y*o.y; }   // dot product
    ll cross(const Point& o) const { return x*o.y - y*o.x; }   // cross product

    ll norm2() const { return x*x + y*y; }  // squared magnitude
    double norm() const { return sqrt((double)norm2()); }
};
```

---

## 9.2 Cross Product Uses

```cpp
// cross(B-A, C-A):
// > 0  → C is to the LEFT  of line A→B  (counter-clockwise turn)
// = 0  → C is COLLINEAR with A and B
// < 0  → C is to the RIGHT of line A→B  (clockwise turn)

ll cross(Point O, Point A, Point B) {
    return (A - O).cross(B - O);
}

// Check if three points make a counter-clockwise turn
bool ccw(Point A, Point B, Point C) { return cross(A, B, C) > 0; }
```

---

## 9.3 Line Segment Intersection

```cpp
// Do segments AB and CD intersect?
bool onSegment(Point p, Point a, Point b) {
    return min(a.x,b.x) <= p.x && p.x <= max(a.x,b.x) &&
           min(a.y,b.y) <= p.y && p.y <= max(a.y,b.y);
}

bool segmentsIntersect(Point a, Point b, Point c, Point d) {
    ll d1 = cross(c, d, a), d2 = cross(c, d, b);
    ll d3 = cross(a, b, c), d4 = cross(a, b, d);
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
        ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) return true;
    if (d1 == 0 && onSegment(a, c, d)) return true;
    if (d2 == 0 && onSegment(b, c, d)) return true;
    if (d3 == 0 && onSegment(c, a, b)) return true;
    if (d4 == 0 && onSegment(d, a, b)) return true;
    return false;
}
```

---

## 9.4 Polygon Area (Shoelace Formula)

```cpp
// Signed area × 2 (avoid division for integer coordinates)
ll signedArea2(const vector<Point>& poly) {
    ll area = 0;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += poly[i].cross(poly[j]);
    }
    return area;  // positive = CCW, negative = CW
}

ll area2(const vector<Point>& poly) { return abs(signedArea2(poly)); }
// True area = area2(poly) / 2.0

// Check if polygon is CCW
bool isCCW(const vector<Point>& poly) { return signedArea2(poly) > 0; }
```

---

## 9.5 Convex Hull — O(N log N)

```cpp
// Andrew's monotone chain — returns points in CCW order
vector<Point> convexHull(vector<Point> pts) {
    int n = pts.size();
    if (n < 3) return pts;
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    n = pts.size();

    vector<Point> hull;
    // Lower hull
    for (int i = 0; i < n; ++i) {
        while (hull.size() >= 2 &&
               cross(hull[hull.size()-2], hull[hull.size()-1], pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }
    // Upper hull
    int lower_size = hull.size();
    for (int i = n - 2; i >= 0; --i) {
        while ((int)hull.size() > lower_size &&
               cross(hull[hull.size()-2], hull[hull.size()-1], pts[i]) <= 0)
            hull.pop_back();
        hull.push_back(pts[i]);
    }
    hull.pop_back();  // last point == first
    return hull;
}
// Change <= 0 to < 0 to include collinear points on the hull boundary
```

---

## 9.6 Point in Polygon

```cpp
// Ray casting — O(N) — works for any simple polygon
bool pointInPolygon(Point p, const vector<Point>& poly) {
    int n = poly.size(), crosses = 0;
    for (int i = 0; i < n; ++i) {
        Point a = poly[i], b = poly[(i+1)%n];
        if (a.y > b.y) swap(a, b);
        if (p.y <= a.y || p.y > b.y) continue;
        ll cp = (b - a).cross(p - a);
        if (cp > 0) ++crosses;
    }
    return crosses % 2 == 1;
}

// Returns: 0 = outside, 1 = on boundary, 2 = inside (winding number version)
```

---

## 9.7 Distance Formulas

```cpp
// Squared Euclidean distance (exact, integer)
ll dist2(Point a, Point b) { return (a-b).norm2(); }

// Euclidean distance (floating point)
double dist(Point a, Point b) { return (a-b).norm(); }

// Point to line distance (signed, integer cross product)
// Line through A and B; returns signed distance × |AB|
ll pointToLineDist2x(Point p, Point a, Point b) {
    return cross(a, b, p);  // = (b-a).cross(p-a)
}

// Closest pair of points — O(N log N) divide and conquer
// (Standard algorithm — see reference implementations)
```

---

## 9.8 Circle

```cpp
struct Circle {
    Point c; double r;
    Circle(Point c, double r) : c(c), r(r) {}
    bool contains(Point p) const {
        return dist2(c, p) <= (ll)(r * r + 1e-9);
    }
};

// Two circles intersect?
bool circlesIntersect(Circle a, Circle b) {
    double d = dist(a.c, b.c);
    return d <= a.r + b.r + 1e-9 && d >= abs(a.r - b.r) - 1e-9;
}
```

---

## 9.9 Common Geometry Pitfalls

```
✗ Using double for coordinates when integer arithmetic would do
✗ Comparing doubles with ==  (use fabs(a-b) < EPS)
✗ Forgetting collinear edge cases in segment intersection
✗ Using floating-point area when the answer must be exact
✗ Sorting points without a stable tie-breaking rule
✗ Convex hull with fewer than 3 points
✗ Ray casting failing on polygon edges (boundary is a special case)
```

```cpp
// Epsilon for floating-point comparisons
const double EPS = 1e-9;
bool eq(double a, double b) { return fabs(a - b) < EPS; }
bool lt(double a, double b) { return a < b - EPS; }
```

---

## 9.10 Summary

| Task | Algorithm | Complexity |
|------|-----------|------------|
| Polygon area | Shoelace formula | O(N) |
| Convex hull | Andrew's monotone chain | O(N log N) |
| Point in polygon | Ray casting | O(N) |
| Segment intersection | Cross product test | O(1) |
| Closest pair | Divide and conquer | O(N log N) |
| CCW/CW turn | Cross product sign | O(1) |

---

**Next**: [10 — Game Theory](10_game_theory.md)
