#include <iostream>
#include <vector>
using namespace std;

// 二分查找
int binary_search(vector<int> &arr, int target) {
    int head = 0, tail = arr.size() - 1;
    while (head <= tail) {
        int mid = (head + tail) >> 1;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) head = mid + 1;
        else tail = mid - 1;
    }
    return -1;
}

// 01模型二分
int binary_search_v1(vector<int> &arr, int target) {
    int head = 0, tail = arr.size();
    while (head < tail) {
        int mid = (head + tail) >> 1;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) head = mid + 1;
        else tail = mid;
    }
    return -1;
}

// 10模型二分
int binary_search_v2(vector<int> &arr, int target) {
    int head = -1, tail = arr.size() - 1;
    while (head < tail) {
        int mid = (head + tail + 1) >> 1;
        if (arr[mid] == target) return mid;
        else if (arr[mid] < target) head = mid;
        else tail = mid - 1;
    }
    return -1;
}


int main() {
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    cout << binary_search(arr, 10) << endl;
    return 0;
}
