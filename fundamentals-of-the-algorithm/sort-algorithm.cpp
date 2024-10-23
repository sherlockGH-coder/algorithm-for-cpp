#include <iostream>
#include <vector>
#include <ctime>
#define SMALL_DATA_SIZE 5000
#define MIDDLE_DATA_SIZE 500000
#define BIG_DATA_SIZE 50000000
#define swap(a, b) { \
    __typeof((a)) c = a; \
    a = b; b = c; \
}

__attribute__((constructor))
void __init_Rand__() {
    printf("init rand\n");
    srand(time(0));
}

std::vector<int> getRandomData(int n) {
    std::vector<int> num;
    for (int i = 0; i < n; i++) {
        num.push_back(rand() % 1000);
    }
    return num;
}

bool isValid(std::vector<int> &num) {
    for (int i = 1; i < num.size(); i++) {
        if (num[i] < num[i - 1]) {
            return false;
        }
    }
    return true;
}

#define TEST(func, arr) { \
    std::vector<int> num(arr); \
    int b = clock(); \
    func(num, 0, num.size()); \
    int e = clock(); \
    std::cout << "TEST " << #func << " : " << (isValid(num) ? "SUCCESS" : "FAILED") << " COST TIME: " << e - b << "ms" << std::endl; \
}


// 插入排序
void insert_sort(std::vector<int>& num, int l, int r) {
    for (int i = l + 1; i < r; i++) {
        for (int j = i; j > 0 && num[j] < num[j - 1]; j--) swap(num[j], num[j - 1]);
    }
    return ;
}

// 冒泡排序
void bubble_sort(std::vector<int>& num, int l, int r) {
    int flag = 1;
    for (int i = l + 1; i < r - 1 && flag; i++) {
        flag = 0;
        for (int j = l; j < r - i; j++) {
            if (num[j] <= num[j + 1]) continue;
            swap(num[j], num[j + 1]);
            flag ++;
        }
    }
    return ;
}

// 归并排序
void merge_sort(std::vector<int> &arr, int l, int r) {
    if (r - l <= 1) return ;
    int mid = (l + r) >> 1;
    merge_sort(arr, l, mid);
    merge_sort(arr, mid, r);
    // merge
    int p1 = l, p2 = mid, k = 0;
    std::vector<int> temp(r - l);
    while (p1 < mid || p2 < r) {
        if (p2 == r || (p1 < mid && arr[p1] <= arr[p2])) {
            temp[k++] = arr[p1++];
        } else {
            temp[k++] = arr[p2++];
        }
    }
    for (int i = l; i < r; i++) arr[i] = temp[i - l];
    return ;
}


int three_point_select(int a, int b, int c) {
    if (a > b) swap(a, b);
    if (a > c) swap(a, c);
    if (b > c) swap(b, c);
    return b;
}


void quick_sort(std::vector<int> &arr, int l, int r) {
    if (r - l <= 2) {
        if (r - l <= 1) return ;
        if (arr[l] > arr[l + 1]) swap(arr[l], arr[l + 1]);
        return ;
    }
    while (l < r) {
        // partition
        int x = l, y = r - 1, z = three_point_select(
            arr[l],
            arr[r - 1],
            arr[(l + r) >> 1]
        );
        do {
            while (arr[x] < z) ++x;
            while (arr[y] > z) --y;
            if (x <= y) {
                swap(arr[x], arr[y]);
                ++x, --y;
            }
        } while (x <= y);
        quick_sort(arr, l, y + 1);
        l = x;
    }
    return ;
}     
      
      
int main() {
    std::vector<int> arr = getRandomData(SMALL_DATA_SIZE);
    TEST(insert_sort, arr);
    TEST(bubble_sort, arr);
    TEST(merge_sort, arr);
    TEST(quick_sort, arr);
    return 0;
}