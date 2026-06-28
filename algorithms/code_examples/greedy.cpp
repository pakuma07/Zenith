// greedy.cpp
// Activity-selection problem solved with a greedy strategy.
#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Max non-overlapping activities (start, end). Greedy: always take the activity
// that finishes earliest, leaving the most room for the rest.
// Technique: sort by end time + greedy scan. Time O(n log n), Space O(1) extra.
int maxActivities(vector<pair<int,int>> intervals) {
    sort(intervals.begin(), intervals.end(),
         [](const pair<int,int>& a, const pair<int,int>& b){
             return a.second < b.second; // order by earliest finish time
         });
    int count = 0;
    int lastEnd = INT_MIN;              // finish time of the last chosen activity
    for (const auto& it : intervals) {
        if (it.first >= lastEnd) {      // starts after the last one ended -> no overlap
            count++;
            lastEnd = it.second;
        }
    }
    return count;
}

int main() {
    vector<pair<int,int>> acts = {{1,2}, {3,4}, {0,6}, {5,7}, {8,9}, {5,9}};
    cout << "max activities = " << maxActivities(acts) << endl;
    return 0;
}
