// hash_tables.cpp
// Demonstrates a hash table with separate chaining (one linked list per bucket)
// and contrasts it with the STL's std::unordered_map.

// Hash table using separate chaining: a fixed array of buckets, each a list of
// (key,value) pairs. Collisions land in the same bucket's chain.
// Average insert/find O(1); worst case O(n) if all keys hash to one bucket.
class HashTable {
private:
    static const int SIZE = 11;                  // prime bucket count reduces clustering
    vector<list<pair<int, int>>> table;          // table[i] = chain of entries for bucket i

    // Maps a key to a bucket index via modulo. O(1).
    int hashFunc(int key) const {
        return key % SIZE;
    }

public:
    HashTable() : table(SIZE) {}  // allocate SIZE empty buckets

    // Insert or update. Hash to a bucket, scan its chain: if the key exists
    // overwrite the value, otherwise append a new pair. O(1) average.
    void insert(int key, int value) {
        int idx = hashFunc(key);
        for (auto& kv : table[idx]) {     // walk the collision chain
            if (kv.first == key) {
                kv.second = value;        // key present: update in place
                return;
            }
        }
        table[idx].push_back(make_pair(key, value));  // key absent: add to chain
    }

    // Look up by key; on hit, write the value through `out` and return true.
    // O(1) average (proportional to chain length).
    bool find(int key, int& out) const {
        int idx = hashFunc(key);
        for (const auto& kv : table[idx]) {  // scan only the relevant bucket
            if (kv.first == key) {
                out = kv.second;
                return true;
            }
        }
        return false;
    }

    // Dump every bucket and its chain, for visualizing collisions. O(SIZE + entries).
    void printBuckets() const {
        for (int i = 0; i < SIZE; ++i) {
            cout << "bucket[" << i << "] : ";
            for (const auto& kv : table[i]) {
                cout << "(" << kv.first << "," << kv.second << ") ";
            }
            cout << endl;
        }
    }
};

int main() {
    cout << "== Custom Hash Table (Chaining) ==" << endl;
    HashTable ht;
    ht.insert(1, 100);
    ht.insert(12, 1200); // collision with key 1 for SIZE=11
    ht.insert(23, 2300); // collision chain

    int value = 0;
    if (ht.find(12, value)) {
        cout << "Found key 12: " << value << endl;
    }
    ht.printBuckets();

    cout << "\n== STL unordered_map ==" << endl;
    // unordered_map is a production hash table; here used as a frequency counter.
    // operator[] inserts a value-initialized 0 on first access, so ++ counts occurrences.
    unordered_map<string, int> freq;
    freq["apple"]++;
    freq["banana"] += 2;
    freq["apple"]++;

    for (const auto& kv : freq) {
        cout << kv.first << " -> " << kv.second << endl;
    }

    return 0;
}
