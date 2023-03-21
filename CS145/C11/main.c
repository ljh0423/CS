#include <stdio.h>

int lowsearch(int A[], int key, int n) {
    int low = 0;
    int high = n - 1;
    //low=0, high=n-1
    //there exists low ≤ k ≤ high, for A[k]≥key where k is minimum
    while (low != high) {
        int mid = (low + high) / 2;
        //mid=(low+high)/2 rounded down
        if (key <= A[mid]) {
          //low ≤ k ≤ mid, for A[k]≥key where k is minimum
          high = mid;
          //low ≤ k ≤ high, for A[k]≥key where k is minimum
        } else {
          //mid < k ≤ high, for A[k]≥key where k is minimum
          low = mid + 1;
          //low ≤ k ≤ high, for A[k]≥key where k is minimum
        }
    }
    //low ≤ k ≤ high, for A[k]≥key where k is minimum, low=high
    //k=low=high, A[low]=key
    if (low == n-1 && A[low] < key) return n; //if key is out of range (key > A[i] for all 0≤i<n)
    else return low;
}
